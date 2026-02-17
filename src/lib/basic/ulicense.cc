/********************************************************************
* Copyright (c) 1998 InterSoft Co.  All Rights Reserved
* THIS IS UNPUBLISHED PROPRIETARY SOURCE CODE OF InterSoft Co.
* The copyright notice above does not evidence any
* actual or intended publication of such source code.
*
* $Id: ulicense.cc,v 1.17 2004/09/29 16:43:55 damianm Exp $
*
* DESCRIPTION
* Manages the user licenses for the Ideafix RunTime module (number 0). 
* This licenses are checked for all the cfix programs, Program() and wcmd()
* These functions are called from Start() (in \lib\basic\c_start.cc).
**********************************************************************/
#ifndef  __NT__
#include <stdlib.h>
#include <unistd.h>
#endif  /* __NT__ */

#include <string.h>
#include <sys/types.h>
#include <signal.h>

#include <ideafix.h>
#include <cfix.h>
#include <cfix/priv/macros.h>
#include <ideafix/priv/gndefs.h>
#include <ideafix/priv/ixsystem.h>
#include <ifound/priv/gfuncs.h>
#include <ifound/priv/pconfig.h>

#ifdef HAVE_SECURITY
#ifndef  __NT__  
#include <local/activ.h>      
#include <local/ulicense.h>
#include <fcntl.h>
#include <sys/stat.h>
#include <utime.h>
#include <utmp.h>
#include <sys/mman.h>
#include <sys/shm.h>
#include <sys/file.h>
#include <errno.h>
#endif	/* __NT__ */
#endif /* HAVE_SECURITY */
 

#ifdef HAVE_SECURITY
#ifndef __NT__


// The strings are encrypty by a simple XOR, so when a user does a "strings <filename>" they don't
// show up.

#define MAX_STR	256
#define X(c)	(c^0x8A)

// This is the user licenses filename (/usr/lib/terminfo/a/ansi-l)
// or (/usr/share/lib/terminfo/a/ansi-l).

#ifdef HAVE_SHARE_LIB_TERMINFO_DIR

static unsigned char mangled_licensefilename[] = {
	X('/'), X('u'), X('s'), X('r'), X('/'), 
    X('s'), X('h'), X('a'), X('r'), X('e'), X('/'),
	X('l'), X('i'), X('b'),X('/'), 
	X('t'), X('e'), X('r'), X('m'), X('i'), X('n'), X('f'), X('o'), X('/'),
	X('a'), X('/'), X('a'), X('n'), X('s'), X('i'), X('-'), X('l'), X('\0') };

#elif defined (HAVE_SHARE_TERMINFO_DIR)

static unsigned char mangled_licensefilename[] = {
    X('/'), X('u'), X('s'), X('r'), X('/'),
    X('s'), X('h'), X('a'), X('r'), X('e'),
    X('/'), X('t'), X('e'), X('r'), X('m'), X('i'), X('n'), X('f'), X('o'), X('/'),
    X('a'), X('/'), X('a'), X('n'), X('s'), X('i'), X('-'), X('l'), X('\0') }; 

#else

static unsigned char mangled_licensefilename[] = {
	X('/'), X('u'), X('s'), X('r'), X('/'), X('l'), X('i'), X('b'),X('/'), 
	X('t'), X('e'), X('r'), X('m'), X('i'), X('n'), X('f'), X('o'), X('/'),
	X('a'), X('/'), X('a'), X('n'), X('s'), X('i'), X('-'), X('l'), X('\0') };

#endif

// This is the message that is displayed when the user's licenses limit is reached
// it is encrypted so no one can do a "strings <filename", and then put a breakpoint on the access to
// that string and break the protection.
// the string is: "\nThe Users Limit for the Ideafix RunTime module has been reached!\n"

static unsigned char mangled_limitreached[] = 
				{ X('\n'), X('T'), X('h'), X('e') , X(' '), X('U'), X('s'),
				  X('e'), X('r'), X('s'), X(' '), X('L'), X('i'), X('m'),
				  X('i'), X('t'), X(' '), X('f'), X('o'),
				  X('r'), X(' '), X('t'), X('h'), X('e'), X(' '), X('I'),
				  X('d'), X('e'), X('a'), X('f'), X('i'), X('x'), X(' '),
				  X('R'), X('u'), X('n'), X('T'), X('i'), X('m'), X('e'),
				  X(' '), X('m'), X('o'), X('d'), X('u'), X('l'), X('e'),
				  X(' '), X('h'), X('a'), X('s'), X(' '), X('b'), X('e'),
				  X('e'), X('n'), X(' '), X('r'), X('e'), X('a'), X('c'),
				  X('h'), X('e'), X('d'), X('!'), X('\n'), X('\0') }; 

// these are the messages displayed when the user's licenses limit is reached for
// all the other packages (Aurus, Denarius, Ianus, etc.);

// "\nAurus Users Limit Reached\n"
static unsigned char mangled_auruslimit[] =
					{
					X('\n'), X('A'), X('u'), X('r'), X('u'), X('s'),
					X(' '), X('o'), X('r'), X(' '), X('M'), X('o'),
					X('d'), X('u'), X('l'), X('e'), X(' '),
					X(' '), X('U'), X('s'), X('e'), X('r'), X('s'), 
					X(' '), X('L'), X('i'), X('m'), X('i'), X('t'),
					X(' '), X('R'), X('e'), X('a'), X('c'), X('h'),
					X('e'), X('d'), X('!'), X('\n'), X('\0') 
					};
			 
// "\nDenarius Users Limit Reached!\n"
static unsigned char mangled_denariuslimit[] =
					{
					X('\n'), X('D'), X('e'), X('n'), X('a'), X('r'),
					X('i'), X('u'), X('s'), 
					X(' '), X('o'), X('r'), X(' '), X('M'), X('o'),
					X('d'), X('u'), X('l'), X('e'), X(' '),
					X(' '), X('U'), X('s'), X('e'), X('r'), X('s'), 
					X(' '), X('L'), X('i'), X('m'), X('i'), X('t'),
					X(' '), X('R'), X('e'), X('a'), X('c'), X('h'),
					X('e'), X('d'), X('!'), X('\n'), X('\0') 
					};

// "\nIanus Users Limit Reached!\n"
static unsigned char mangled_ianuslimit[] = 	
					{
					X('\n'), X('I'), X('a'), X('n'), X('u'), X('s'),
					X(' '), X('o'), X('r'), X(' '), X('M'), X('o'),
					X('d'), X('u'), X('l'), X('e'), X(' '),
					X(' '), X('U'), X('s'), X('e'), X('r'), X('s'), 
					X(' '), X('L'), X('i'), X('m'), X('i'), X('t'),
					X(' '), X('R'), X('e'), X('a'), X('c'), X('h'),
					X('e'), X('d'), X('!'), X('\n'), X('\0') 
					};

// "\nSIRHU Users Limit Reached!\n"
static unsigned char mangled_varulimit[] =
					{
					X('\n'), X('V'), X('a'), X('r'), X('i'), X('o'), X('s'),
					X(' '), X('o'), X('r'), X(' '), X('M'), X('o'),
					X('d'), X('u'), X('l'), X('e'), X(' '),
					X(' '), X('U'), X('s'), X('e'), X('r'), X('s'), 
					X(' '), X('L'), X('i'), X('m'), X('i'), X('t'),
					X(' '), X('R'), X('e'), X('a'), X('c'), X('h'),
					X('e'), X('d'), X('!'), X('\n'), X('\0') 
					};

// "\nCBA Users Limit Reachad!\n"
static unsigned char mangled_cbalimit[] =
					{
					X('\n'), X('C'), X('B'), X('A'),
					X(' '), X('o'), X('r'), X(' '), X('M'), X('o'),
					X('d'), X('u'), X('l'), X('e'), X(' '),
					X(' '), X('U'), X('s'), X('e'), X('r'), X('s'), 
					X(' '), X('L'), X('i'), X('m'), X('i'), X('t'),
					X(' '), X('R'), X('e'), X('a'), X('c'), X('h'),
					X('e'), X('d'), X('!'), X('\n'), X('\0') 
					};
 

// This is the function used to decrypt the strings

static unsigned char *unmangle(const unsigned char *p)
{

static unsigned char str[MAX_STR] = { '\0' };

	register unsigned char *q = str;

	while ((*q++ = X(*p))) {
		// Just in case macro F expands '*p' more than once!
		p++;
		// no need for this, but it's ok, don't feel "asserted" today.
		if( (q-str) == MAX_STR-1)
			break;
	}

	return str;

}

// This is the function used to return and decrypt the corresponding limit reached
// messages for every package.

static unsigned char *construct_message(int pkg_num)
{
	if ( (pkg_num >= PKG_RUNTIME) && (pkg_num < (PKG_RUNTIME+SWBASE_PKGS)) )
 	   return unmangle(mangled_limitreached);
 	else  if ( (pkg_num >= APPL_AURUS_BASICO) && (pkg_num < (APPL_AURUS_BASICO+AURUS_MODULES)) )
	   return unmangle(mangled_auruslimit);
	else  if ( (pkg_num >= APPL_DENARIUS_BASICO) && (pkg_num < (APPL_DENARIUS_BASICO+DENARIUS_MODULES)) )
	   return unmangle(mangled_denariuslimit);
	else  if ( (pkg_num >= APPL_IANUS_BASICO) && (pkg_num < (APPL_IANUS_BASICO+IANUS_MODULES)) )
 	   return unmangle(mangled_ianuslimit);
	else if ( (pkg_num >= APPL_VARIAS) && (pkg_num < (APPL_VARIAS+VARIAS_MODULES)) ) 
	   return unmangle(mangled_varulimit);
	else if ( (pkg_num >= APPL_CBA_BASICO) && (pkg_num < (APPL_CBA_BASICO+CBA_MODULES)) ) 
	   return unmangle(mangled_cbalimit);
	// It can't reach this point
	return unmangle(mangled_limitreached); 
}

// These are the messages printed when something goes wrong in the entire
// process of tracking current logged on users. These are disguised to
// not alert the user of the real problem that could lead
// to cracking the protection.

extern "C" Int ActivationPackageUsers(Int);
extern "C" void PurgeUserLicensesFile(void);
extern "C" void crypt_license(unsigned char *buffer, long len, int mode);

// keeps track of the package type
static int License_PkgType = PKG_RUNTIME;

extern "C" void SetModuleID(int pkg)
{
	License_PkgType = pkg;
}

/**
 *
 * Implementation of class CUserLicensesFile
 *
 *
 */

/**
 * 
 * bool CUsersLicensesFile::deleteFile()
 *
 * deletes the users licenses file
 *
 * returns: 
 *
 * true = file deleted
 * false = file couldn't be deleted
 *
 *
 */
bool CUsersLicensesFile::deleteFile() {
	return (remove(LICENSE_FILENAME) < 0 ) ? false : true;
}

/**
 *
 * bool CUsersLicensesFile::createFile()
 *
 * creates the users licenses file
 *
 * returns: 
 *
 * true = file created
 * false = file couldn't be created
 *
 */
bool CUsersLicensesFile::createFile() {
	CUsersLicensesHeader lh;
	int f, fd;

	// If the file exists, we erase it first
	deleteFile();

	if ((fd = open(LICENSE_FILENAME, O_CREAT | O_TRUNC | O_WRONLY)) < 0) 
		return false;
	
	// we set the access privilges to -rw-rw-rw
	if (chmod(LICENSE_FILENAME, 0666) < 0) {
		close(fd);
		deleteFile();
		return false;
	}

	// We encrypts the users licenses header first
	encrypt( &lh );

	if (write(fd, &lh, sizeof(CUsersLicensesHeader)) != sizeof(CUsersLicensesHeader)) {
		close(fd);
		deleteFile();
		return false;
	}

	close(fd);

	return true;
}
   
/**
 * ResetCrackerUserLicenses()
 *
 * This function erases the shared memory used by all the
 * cracker user licenses system.
 * Is here for ixset to use, so when a user changes the number of cracker's user
 * linceses, ixset erases the shared memory, so the next time irexecd
 * is run, it recreates the shared memory with the appropiate size.
 */
bool ResetCrackerUserLicenses()
{
	int shmid;

    // if we can't get the shared memory segment, then it isn't created so
    // we have nothing to erase.
    if ((shmid = shmget(SHAREDMEM_KEY, 0, (SHM_R | SHM_W))) < 0) {
		return true;
	}

    // here we try to erase the shared memory segment
    if (shmctl(shmid, IPC_RMID, 0) < 0) {
                return false;
	}

    return true;
}   


/**
 *
 * bool CUsersLicensesFile::fileExists()
 *
 * Checks if the user licenses file exists
 * 
 * returns:
 *
 * true = file exists
 * false = file doesn't exist
 *
 */
bool CUsersLicensesFile::fileExists() {
	// We try to open the license file, if we can't find it,
	// return false

	if (access(LICENSE_FILENAME, F_OK) != 0) return false;

	return true;
}

/**
 *
 * int CUsersLicensesFile::numPackageTtys(int pkg_type)
 *
 * 
 * returns the number of entrys in the users licenses file for the specified
 * package (pkg_type). All the entrys for a same tty are counted as one.
 *
 * pkg_type  = package type to look for
 *
 * returns:
 *
 * number of entries for the specified package
 *
 */
int CUsersLicensesFile::numPackageTtys(int pkg_type) {
	int fd;
	struct stat mibuf;
	struct flock miflock;
	void *mmap_ptr;
	CUsersLicensesEntry* ulentry;
	CUsersLicensesHeader* ulheader;
	CUsersLicensesHeader ulheader_temp;
	int numEntries = 0, pkgEntries = 0;

	assert(fileExists() == true);

	if ((fd = open(LICENSE_FILENAME, O_RDONLY)) < 0) {
		fprintf(stderr, CANTOPEN_FILE);
		stop(LICENSE_BAD);
	}

	// We set a r/w lock to the entire file
	// to behave correctly with concurrency.
	// we don't remove the file lock later
	// i assume and hope :) that when the file descriptor
	// is closed everything is correctly reseted.

	miflock.l_type = F_RDLCK;
	miflock.l_whence = SEEK_END; 
	miflock.l_start=0;
	miflock.l_len=0;

	if (fcntl(fd, F_SETLKW, &miflock) < 0) {
		fprintf(stderr, GENERAL_LICENSE_ERROR);
		close(fd);
		stop(LICENSE_BAD);
	}

	// We determine the filesize
	if (fstat(fd, &mibuf) != 0) {
		close(fd);
		fprintf(stderr, GENERAL_LICENSE_ERROR);
		stop(LICENSE_BAD);
	}

	// If the file size is minor than the size of the CUsersLicensesHeader
	// then that is incorrect.
	if (mibuf.st_size < sizeof(CUsersLicensesHeader)) {
		close(fd);
		fprintf(stderr, GENERAL_LICENSE_ERROR);
		stop(LICENSE_BAD);
	}

	// If the file size is exactly the same size of the CUsersLicensesHeader
	// then no users are currently logged on.
	if (mibuf.st_size == sizeof(CUsersLicensesHeader)) return 0;

	// we memory map the file content
	mmap_ptr = (void*) -1;
	mmap_ptr =  mmap(0, mibuf.st_size, PROT_READ, MAP_SHARED, fd, 0);
	if (mmap_ptr == (void*) -1) {
		close(fd);
		fprintf(stderr, GENERAL_LICENSE_ERROR);
		stop(LICENSE_BAD);
	}

	// We check for the header magic number
	ulheader = (CUsersLicensesHeader*) mmap_ptr;
	ulheader_temp = *ulheader;
	decrypt(&ulheader_temp);

	if (ulheader_temp.isValid() == false) {
		close(fd);
		fprintf(stderr, BAD_LICENSE_MAGIC);
		stop(LICENSE_BAD);
	}

	// We obtain the total number of entries in the file
	numEntries = (mibuf.st_size - sizeof(CUsersLicensesHeader)) / 
					sizeof(CUsersLicensesEntry);

	// Now we loop through the file counting the entries for the
	// specified package with the same tty as the current process
	int nodes_repeated_temp=0;
	int f, g;
	int *invalids;
	int clients_number;
	CUsersLicensesEntry* oth_ulentry;
	CUsersLicensesEntry  ulentry_temp1;
	CUsersLicensesEntry  ulentry_temp2;

	clients_number = numEntries;
	
	// (invalids) is for storing the entries i don't have to
	// check again.        
	invalids = new int[clients_number];
	for(f = 0; f < clients_number; f++) invalids[f] = 0;

	// here is the algorithm
	ulentry = (CUsersLicensesEntry*) (ulheader+1);

	for(f = 0; f < clients_number; f++) {
		ulentry_temp1 = *ulentry;
		decrypt(&ulentry_temp1);
	
		nodes_repeated_temp = 0;              
		oth_ulentry = (CUsersLicensesEntry*) (ulheader+1);
 
		for(g = 0; g < clients_number; g++) {
			ulentry_temp2 = *oth_ulentry;
			decrypt(&ulentry_temp2);
	
			if (invalids[f] != 1) {    
		 		if (!strcmp(ulentry_temp1.getTtyName(), ulentry_temp2.getTtyName())) {
					nodes_repeated_temp++;	
					if (nodes_repeated_temp > 1 ) invalids[g] = 1;
				}
			}
			oth_ulentry++;
		}
		ulentry++;
	}

	ulentry = (CUsersLicensesEntry*) (ulheader+1);

	for(f = 0; f < clients_number; f++) {
		ulentry_temp1 = *ulentry;
		decrypt(&ulentry_temp1);
	
		if (invalids[f] != 1) {
			if (ulentry_temp1.isErased() == false &&
				ulentry_temp1.getPkgType() == License_PkgType) {
				if (ttyname(0)) {
					if (strcmp(ulentry_temp1.getTtyName(), (char*)ttyname(0)) )
						pkgEntries++;
				}
				else if (ttyname(1)) {
						if (strcmp(ulentry_temp1.getTtyName(), (char*)ttyname(1)) )
							pkgEntries++;
					}
				else if (ttyname(2)) {
						if (strcmp(ulentry_temp1.getTtyName(), (char*)ttyname(2)) )
							pkgEntries++;
					}
			}
		}
		ulentry++;
	}
	
	delete[] invalids;

	munmap(mmap_ptr, mibuf.st_size);
	close(fd);

	return pkgEntries;
}

/**
 *
 * bool CUsersLicensesFile::addEntry(CUsersLicensesEntry* pnew_ulentry)
 *
 *
 * Adds one entry to the users licenses file.
 *
 * pnew_ulentry = entry to add to the file
 *
 * returns:
 *
 * true = entry added.
 * false = couldn't add entry.
 *
 */
bool CUsersLicensesFile::addEntry(CUsersLicensesEntry* pnew_ulentry) {
	struct stat mibuf;
	struct flock miflock;
	void *mmap_ptr;
	CUsersLicensesEntry* ulentry;
	CUsersLicensesHeader* ulheader;
	CUsersLicensesHeader ulheader_temp;
	CUsersLicensesEntry ulentry_temp, ulentry_temp2;
	int fd, numEntries = 0;

	assert(fileExists() == true);

	if ((fd = open(LICENSE_FILENAME, O_RDWR)) < 0) {
		fprintf(stderr, CANTOPEN_FILE);
		stop(LICENSE_BAD);
	}

	// We set a r/w lock to the entire file
	// to behave correctly with concurrency.
	// we don't remove the file lock later
	// i assume and hope :) that when the file descriptor
	// is closed everything is correctly reseted.

	miflock.l_type = F_RDLCK|F_WRLCK;
	miflock.l_whence = SEEK_END; 
	miflock.l_start=0;
	miflock.l_len=0;

	if (fcntl(fd, F_SETLKW, &miflock) < 0) {
		fprintf(stderr, GENERAL_LICENSE_ERROR);
		close(fd);
		stop(LICENSE_BAD);
	}

	// We determine the filesize
	if (fstat(fd, &mibuf) != 0) {
		close(fd);
		fprintf(stderr, GENERAL_LICENSE_ERROR);
		stop(LICENSE_BAD);
	}

	// If the file size is minor than the size of the CUsersLicensesHeader
	// then that is incorrect.
	if (mibuf.st_size < sizeof(CUsersLicensesHeader)) {
		close(fd);
		fprintf(stderr, GENERAL_LICENSE_ERROR);
		stop(LICENSE_BAD);
	}

	// We obtain the total number of entries in the file
	numEntries = (mibuf.st_size - sizeof(CUsersLicensesHeader)) / 
				sizeof(CUsersLicensesEntry);


	// We try to recycle a deleted entry from the users licenses file
	// we memory map the file content
	mmap_ptr = (void*) -1;
	mmap_ptr =  mmap(0, mibuf.st_size, PROT_READ|PROT_WRITE, MAP_SHARED, fd, 0);

	if (mmap_ptr == (void*) -1) {
		close(fd);
		fprintf(stderr, GENERAL_LICENSE_ERROR);
		stop(LICENSE_BAD);
	}

	// We check for the header magic number
	ulheader = (CUsersLicensesHeader*) mmap_ptr;
	ulheader_temp = *ulheader;
	decrypt(&ulheader_temp);

	if (ulheader_temp.isValid() == false) {
		close(fd);
		fprintf(stderr, BAD_LICENSE_MAGIC);
		stop(LICENSE_BAD);
	}

	ulentry = (CUsersLicensesEntry*) (ulheader+1);

	for(int f = 0; f < numEntries; f++) {
		ulentry_temp = *ulentry;
		decrypt(&ulentry_temp);	
 
		if (ulentry_temp.isErased() == true) {
			ulentry_temp2 = *pnew_ulentry;
			ulentry_temp2.unerase();
			encrypt(&ulentry_temp2);

			*ulentry = ulentry_temp2;
			munmap(mmap_ptr, mibuf.st_size);
			close(fd);
			return true;
		}
		ulentry++;
	}

	munmap(mmap_ptr, mibuf.st_size);
	
	// We couldn't recycle a deleted entry, so we must add one entry to
	// the file.
	// We go to the end of the file
	if (lseek(fd, 0, SEEK_END) < 0) {
		close(fd);
		fprintf(stderr, GENERAL_LICENSE_ERROR);
		stop(LICENSE_BAD);
	}
	
	// We add one entry
	ulentry_temp2 = *pnew_ulentry;
	encrypt(&ulentry_temp2);

	if (write(fd, &ulentry_temp2, sizeof(CUsersLicensesEntry)) != sizeof(CUsersLicensesEntry)) {
		close(fd);
		fprintf(stderr, GENERAL_LICENSE_ERROR);
		stop(LICENSE_BAD);
	}

	close(fd);
	return true;
}

/**
 *
 * bool CUsersLicensesFile::delEntry(CUsersLicensesEntry* pold_ulentry)
 *
 * deletes an entry from the users licenses file.
 *
 * pold_ulentry is compared with each entry (through the == operator)
 * and if thery are equal, the entry is deleted.
 *
 * returns:
 *
 * true = entry deleted.
 * false = entry wasn't deleted.
 *
 */
bool CUsersLicensesFile::delEntry(CUsersLicensesEntry* pold_ulentry) {
	struct stat mibuf;
	struct flock miflock;
	void *mmap_ptr;
	CUsersLicensesEntry* ulentry;
	CUsersLicensesHeader* ulheader;
	CUsersLicensesHeader ulheader_temp;
	CUsersLicensesEntry ulentry_temp;
	int fd, numEntries = 0;

	assert(fileExists() == true);

	if ((fd = open(LICENSE_FILENAME, O_RDWR)) < 0) {
		fprintf(stderr, CANTOPEN_FILE);
		stop(LICENSE_BAD);
	}

	// We set a r/w lock to the entire file
	// to behave correctly with concurrency.
	// we don't remove the file lock later
	// i assume and hope :) that when the file descriptor
	// is closed everything is correctly reseted.

	miflock.l_type = F_RDLCK|F_WRLCK;
	miflock.l_whence = SEEK_END; 
	miflock.l_start=0;
	miflock.l_len=0;

	if (fcntl(fd, F_SETLKW, &miflock) < 0) {
		fprintf(stderr, GENERAL_LICENSE_ERROR);
		close(fd);
		stop(LICENSE_BAD);
	}

	// We determine the filesize
	if (fstat(fd, &mibuf) != 0) {
		close(fd);
		fprintf(stderr, GENERAL_LICENSE_ERROR);
		stop(LICENSE_BAD);
	}

	// If the file size is minor than the size of the CUsersLicensesHeader
	// then that is incorrect.
	if (mibuf.st_size < sizeof(CUsersLicensesHeader)) {
		close(fd);
		fprintf(stderr, GENERAL_LICENSE_ERROR);
		stop(LICENSE_BAD);
	}

	// We obtain the total number of entries in the file
	numEntries = (mibuf.st_size - sizeof(CUsersLicensesHeader)) / 
				sizeof( CUsersLicensesEntry );


	// We find our entry and mark it as deleted
	// we memory map the file content
	mmap_ptr = (void*) -1;
	mmap_ptr =  mmap(0, mibuf.st_size, PROT_READ|PROT_WRITE, MAP_SHARED, fd, 0);

	if (mmap_ptr == (void*) -1) {
		close(fd);
		fprintf(stderr, GENERAL_LICENSE_ERROR);
		stop(LICENSE_BAD);
	}

	// We check for the header magic number
	ulheader = (CUsersLicensesHeader*) mmap_ptr;
	ulheader_temp = *ulheader;
	decrypt(&ulheader_temp);

	if (ulheader_temp.isValid() == false) {
		close(fd);
		fprintf(stderr, BAD_LICENSE_MAGIC);
		stop(LICENSE_BAD);
	}

	ulentry = (CUsersLicensesEntry*) (++ulheader);

	for(int f = 0; f < numEntries; f++) {
		ulentry_temp = *ulentry;
		decrypt(&ulentry_temp);
  
		if (ulentry_temp == *pold_ulentry) {
			ulentry_temp.erase();
			encrypt(&ulentry_temp);
			*ulentry = ulentry_temp;	
			munmap(mmap_ptr, mibuf.st_size);
			close(fd);
			return true;
		}
		ulentry++;
	}

	// if we arrived here we couldn't find our entry in the licenses
	// file. this is a severe error.
	munmap(mmap_ptr, mibuf.st_size);
	close(fd);
	fprintf(stderr, GENERAL_LICENSE_ERROR);
	stop(LICENSE_BAD);

	return false;
}

/**
 * 
 * void CUsersLicensesFile::purgeLicensesFile()
 *
 *
 * Purges the users licenses file, erasing the orphan
 * entries in it.
 * 
 *
 */
void CUsersLicensesFile::purgeLicensesFile() {
	long  f;
	void *mmap_ptr;
	struct stat mibuf;
	struct flock miflock;
	//struct utmp *UTMP;
	int fd, numEntries = 0;
	CUsersLicensesHeader* ulheader;
	CUsersLicensesHeader ulheader_temp;
	CUsersLicensesEntry* ulentry;
	CUsersLicensesEntry ulentry_temp;

	assert(fileExists() == true);

	// We try to open the license file, if we can't find it,
	// it's ok, we are just trying to purge the file
  	// and CheckUserLicenses() will handle the no existence
	// of the licenses file. 
	if ((fd = open(LICENSE_FILENAME, O_RDWR)) < 0) {
		fprintf(stderr, CANTOPEN_FILE);
		return;
	}

	// We set a r/w lock to the entire file
	// to behave correctly with concurrency
	// we don't remove the file lock later
	// i assume and hope :) that when the file descriptor
	// is closed all is correctly reseted

	miflock.l_type = F_RDLCK|F_WRLCK;
	miflock.l_whence = SEEK_END;
	miflock.l_start=0;
	miflock.l_len=0;

	if (fcntl(fd, F_SETLKW, &miflock) < 0) {
		// we don't care about errors here, let 
		// CheckUserLicenses do all the work.
		close(fd);
		fprintf(stderr, GENERAL_LICENSE_ERROR);
		return;
	}

	// We determine the filesize
	if (fstat(fd, &mibuf) != 0) {
		close(fd);
		fprintf(stderr, GENERAL_LICENSE_ERROR);
		return;
	}

	// If the file size is minor than the size of the CUsersLicensesHeader
	// then that is incorrect.
	if (mibuf.st_size < sizeof(CUsersLicensesHeader)) {
		close(fd);
		fprintf(stderr, GENERAL_LICENSE_ERROR);
		return;
	}

	// We obtain the total number of entries in the file
	numEntries = (mibuf.st_size - sizeof(CUsersLicensesHeader)) / 
				sizeof( CUsersLicensesEntry );

	// we memory map the file content
	mmap_ptr = (void*) -1;
	mmap_ptr =  mmap(0, mibuf.st_size, PROT_READ|PROT_WRITE, MAP_SHARED, fd, 0);

	if (mmap_ptr == (void*) -1) {
		close(fd);
		fprintf(stderr, GENERAL_LICENSE_ERROR);
		stop(LICENSE_BAD);
	}

	// We check for the header magic number

	ulheader = (CUsersLicensesHeader*) mmap_ptr;

	ulheader_temp = *ulheader;
	decrypt( &ulheader_temp );

	if (ulheader_temp.isValid() == false) {
		close(fd);
		fprintf(stderr, BAD_LICENSE_MAGIC);
		stop(LICENSE_BAD);
	}

	ulentry = (CUsersLicensesEntry*) (ulheader+1);

	// if the process in the list isn't alive, we erase the
	// entry, because it's an orphan.
	// we try this only if we are root.

	for(f = 0; f < numEntries; f++) {
		ulentry_temp = *ulentry;
		decrypt( &ulentry_temp );

		if (ulentry_temp.isErased() == false) {
			if (kill(ulentry_temp.getPid(), 0) < 0) {
				if (errno != EPERM)	{
					ulentry_temp.erase();
					encrypt(&ulentry_temp);
					*ulentry = ulentry_temp;
				}
			}
		}
	}

	munmap(mmap_ptr, mibuf.st_size);
	close(fd);
	return;

//	ulentry = (CUsersLicensesEntry*) (ulheader+1);

	// We search the license file trying to find a ttyname
	// that doesn't corresponds to a current logged on user.

	//int modificated = 0;

	//for( f=0 ; f < NumEntries ; f++ )
	//{

	 // if( ulentry->isErased() == false)
	  //{
		// we go to the beginning of the UTMP file 
	//	setutent();

	//	int logged = 0;

	//	while((UTMP=getutent()))
	//	{
//
//			if(UTMP->ut_type==USER_PROCESS)
//			{
//				// szttyName is /dev/tty??
//				// and ut_line is tty?? so we skip the
//				// /dev/ in szttyname to do the compare	
//			
//				char ttybuff[MAX_TTY_NAME];
//				strcpy(ttybuff, ulentry->getTtyName());
//				char *tmp = 0;
//				tmp = (strrchr(ttybuff, '/')+1);
//			
//				assert( tmp != 0 );
//				assert( UTMP->ut_line != 0);
//			
//				if( !strcmp(tmp, UTMP->ut_line) )
//					logged=1;	
//			}
//
//		}
//		
//		if(logged==0)
//		{
//			modificated = 1;
//			ulentry->erase();
//			ulentry->setPkgType(PKG_RUNTIME);
//			ulentry->setPid(0);
//
//		}
//
//	  }
//		// we close the UTMP
//		endutent();
//
//	} 

}


/**
 * 
 * void CUsersLicensesFile::encrypt(CUsersLicensesEntry* ulentry)
 *
 * encrypts the CUsersLicensesEntry passed to it.
 *
 *
 */

void CUsersLicensesFile::encrypt(CUsersLicensesEntry* ulentry)
{
		
	crypt_license((unsigned char*)ulentry->getTtyName(), MAX_TTY_NAME, LICENSE_ENCRYPT);
}


/**
 * 
 * void CUsersLicensesFile::decrypt(CUsersLicensesEntry* ulentry)
 *
 * decrypts the CUsersLicensesEntry passed to it.
 *
 *
 */

void CUsersLicensesFile::decrypt(CUsersLicensesEntry* ulentry)
{
	
	crypt_license((unsigned char*)ulentry->getTtyName(), MAX_TTY_NAME, LICENSE_DECRYPT);

}

/**
 * 
 * void CUsersLicensesFile::encrypt(CUsersLicensesHeader* ulheader)
 *
 * encrypts the CUsersLicensesHeader passed to it.
 *
 *
 */

void CUsersLicensesFile::encrypt(CUsersLicensesHeader* ulheader)
{
		
	crypt_license((unsigned char*)&ulheader->iMagic, sizeof(int), LICENSE_ENCRYPT);
}

/**
 * 
 * void CUsersLicensesFile::decrypt(CUsersLicensesHeader* ulheader)
 *
 * decrypts the CUsersLicensesHeader passed to it.
 *
 *
 */

void CUsersLicensesFile::decrypt(CUsersLicensesHeader* ulheader)
{
	
	crypt_license((unsigned char*)&ulheader->iMagic, sizeof(int), LICENSE_DECRYPT);
}

/**
 *
 * We bound the CheckUserLicenses_atexit to be
 * executed at the end of the cfix program.
 *
 * because the function can get called twice,
 * one from Stop() and one from  atexit() (a compiler
 * built-in function, used so we are called when a cfix
 * program is terminated using exit())
 * we use this flag to know we have been already 
 * executed.
 *
 */

static bool CheckUserLicenses_atexit_executed = FALSE;

/**
 *
 * void crypt_license(unsigned char *buffer, long len, int mode)
 *
 * encrypts the buffer passed to it.
 * it is used to encrypt the license file used the inode of
 * of license file as they encryption key.
 * these prevents a user for copying a backup of the license
 * file on top of the original one when the users limit is reached.
 * (it turns out that this method doesn't work because the cp command
 * when overwriting files uses the same inode number, so a cp won't
 * change the inode number and the encryption key will remain the same,
 * anyway, we keep this method).
 *
 * mode = LICENSE_ENCRYPT -> encrypt the buffer
 * mode = LICENSE_DECRYPT -> decrypt the buffer
 *
 */

extern "C" void crypt_license(unsigned char *buffer, long len, int mode)
{
struct stat mibuf;
unsigned char key;


	// We determine the license file inode number
	if( stat( LICENSE_FILENAME , (struct stat *)&mibuf) != 0 )
	{
		fprintf(stderr, GENERAL_LICENSE_ERROR);

		// we don't want to execute the atexit function because i think
		// everything is a mess with this error and i don't want to make it worst

		CheckUserLicenses_atexit_executed = TRUE;

		stop(LICENSE_BAD);
	}

	// we use the inode number as the key to our encryption method

	key = (unsigned char) mibuf.st_ino;
		
	// we don't use the mode because we are using a horrible
	// xor encryption which is symmetric.

	if( (len%2) == 0)
	{
		for( long f=0; f < len;  f+=2)
		{
		buffer[f] ^= key;
		buffer[f+1] ^= key + 40;
		}
	}
	else
	{
		for(long f=0; f< len; f++ )
			buffer[f] ^= key;
	}
		
}


/**
 *
 * void encrypt_license_file(void)
 *
 *
 * This function gets called when the user licenses file is created.
 * it is called after the file created, so we can obtain the inode number
 * and use it as the encryption key.
 *
 */

//extern "C" void encrypt_license_file(void)
//{
//FILE *fp;
//void *buffer;
//struct stat mibuf;

//	if( (fp = fopen(LICENSE_FILENAME, "rb+") ) == NULL )
//	{
//		fprintf(stderr, GENERAL_LICENSE_ERROR);
//		unlink(LICENSE_FILENAME);
//		stop(LICENSE_BAD);
//	}
//
//	
//	// We determine the filesize
//	if( fstat( fileno(fp) , (struct stat *)&mibuf) != 0 )
//	{
//		fclose(fp);
//		fprintf(stderr, GENERAL_LICENSE_ERROR);
//		unlink(LICENSE_FILENAME);
//		stop(LICENSE_BAD);
//	}
//	
//	// we try to read the entire file content to memory
//	buffer = (void*)0;
//	buffer = (void*)malloc(mibuf.st_size);
//	if( buffer == (void*) 0 )
//	{
//		fclose(fp);
//		fprintf(stderr, GENERAL_LICENSE_ERROR);
//		unlink(LICENSE_FILENAME);
//		stop(LICENSE_BAD);
//	}
//
//	if( fread(buffer, mibuf.st_size, 1, fp) != 1 )
//	{
//		fclose(fp);
//		free(buffer);
//		fprintf(stderr, GENERAL_LICENSE_ERROR);
//		unlink(LICENSE_FILENAME);
//		stop(LICENSE_BAD);
//	}

	// we encrypt the license file
//	encrypt_license((unsigned char*)buffer, mibuf.st_size);

	// we save it to disk
//	rewind(fp);

//	if( fwrite(buffer, mibuf.st_size, 1, fp) != 1 )
//	{
//		fclose(fp);
//		free(buffer);
//		fprintf(stderr, GENERAL_LICENSE_ERROR);
//		unlink(LICENSE_FILENAME);
//		stop(LICENSE_BAD);
//	}

//	fclose(fp);
//	PutSafeStamp();
//	free(buffer);
	
//}

/**
 *
 * void DeleteUserLicensesFile(void)
 *
 * Deletes the file used to keep track of logged on users using cfix
 *
 *
 */

extern "C" void DeleteUserLicensesFile(void)
{
CUsersLicensesFile licfile;

	licfile.deleteFile();
}

/**
 *
 * void CreateUserLicensesFile()
 *
 * This function creates the necessary file to keep track
 * of the logged on users
 *
 *
 */

extern "C" bool CreateUserLicensesFile()
{
CUsersLicensesFile ulfile;

	return ulfile.createFile();

};


/** 
 *
 * void CheckUserLicenses_atexit(void)
 *
 *
 * This function is called when a cfix program is terminated.
 * It decrements the reference count of the user's tty on the
 * license file, and erases the entry when it reaches 0.
 *
 *
 */

extern "C" void CheckUserLicenses_atexit()
{
CUsersLicensesEntry ulentry;
CUsersLicensesFile licfile;

	if( CheckUserLicenses_atexit_executed == TRUE )
		return;

	// We update the flag to notice we were executed

	CheckUserLicenses_atexit_executed = TRUE;

	// Now we re-create the users licenses entry for this process
	// and we erase it.

	// We check if stdin is a tty, if it's not something strange
	// is happening.
	
	if (!isatty(0) && !isatty(1) && !isatty(2) ) {
		fprintf(stderr, GENERAL_LICENSE_ERROR);
		stop(LICENSE_BAD);
	}

	if (ttyname(0)) ulentry.setTtyName( (char*)ttyname(0) );
	else if (ttyname(1)) ulentry.setTtyName( (char*)ttyname(1) );
	else if (ttyname(2)) ulentry.setTtyName( (char*)ttyname(2) );
	else {
		fprintf(stderr, GENERAL_LICENSE_ERROR);
		stop(LICENSE_BAD);
	}
	ulentry.setPid( getpid() );
	ulentry.setPkgType( License_PkgType );

	if( licfile.delEntry(&ulentry) == true )
			return;

	// We didn't find our entry in the license file
	// something is wrong, maybe someone tried to
	// tamper the license file
	
	fprintf(stderr, GENERAL_LICENSE_ERROR);
	stop(LICENSE_BAD);
}

/**
 *
 * void CheckUserLicenses(void)
 *
 * This function checks if the RunTime package is active
 * and performs tracking of the number of licenses in use
 *
 *
 */
extern "C" void CheckUserLicenses(void) {
	CUsersLicensesFile licfile;
	CUsersLicensesEntry ulentry;
	Int iMaxTtys;

	// if the package is not active the program
	// is terminated. Maybe this should be
  	// checked, the termination is abnormal and
	// maybe we should call shutDown() or a function
	// like that.	

	// We check the activation of the package this process belongs to.
	CheckPerm(License_PkgType);

	// Now obtain the number of users allowed for the package. 
	iMaxTtys = ActivationPackageUsers(License_PkgType);

	// if iMaxTtys == 0 it is an unlimited user license
	// so, we don't need to check anything.
	if (iMaxTtys == 0) return;

	// First of all, we purge the Licenses file to clean
	// orphan entries.
	licfile.purgeLicensesFile();

	// Now we check the number of different tty's running programs belonging to
	// this process package. If the limit has been reached, this program
	// is not allowed to run.
	if (licfile.numPackageTtys(License_PkgType) == iMaxTtys) {
		fprintf(stderr, USERS_LIMIT_REACHED(License_PkgType));
		stop(LICENSE_EXPIRED);
	}

	// The limit has not been reached, so we add an entry corresponding
	// to this process to the users licenses file.
	
	// We check if stdin is a tty, if it's not something strange
	// is happening.
	if (!isatty(0) && !isatty(1) && !isatty(2)) {
		fprintf(stderr, GENERAL_LICENSE_ERROR);
		stop(LICENSE_BAD);
	}

	// We create an entry for the current process
	if (ttyname(0)) ulentry.setTtyName( (char*)ttyname(0) );
	else if (ttyname(1)) ulentry.setTtyName( (char*)ttyname(1) );
	else if (ttyname(2)) ulentry.setTtyName( (char*)ttyname(2) );
	else {
		fprintf(stderr, GENERAL_LICENSE_ERROR);
		stop(LICENSE_BAD);
	}

	ulentry.setPid( getpid() );
	ulentry.setPkgType( License_PkgType );

	// We add the entry to the users licenses file
	licfile.addEntry(&ulentry);

	// We install the callback function that gets called
	// when a cfix program is terminated and cleans up the
	// license file (CheckUserLicenses_atexit)
	OnStop(CheckUserLicenses_atexit);
	atexit(CheckUserLicenses_atexit);
	return;

}

/**
 *
 * void PurgeUserLicensesFile(void)
 *
 *
 * This function tries to remove from the users license file
 * those entrys that are orphans and are holding users licenses.
 * Tries to clean the licenses file after an abnormal termination
 * of a cfix program, that leaves the licenses file in a corrupt
 * state.
 *
 */

extern "C" void PurgeUserLicensesFile(void)
{
CUsersLicensesFile	licfile;

		licfile.purgeLicensesFile();	
}

#endif		/* __NT__ 	*/
#endif		/* HAVE_SECURITY */
