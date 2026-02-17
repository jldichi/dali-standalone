/********************************************************************
* Copyright (c) 1991 InterSoft Co.  All Rights Reserved
* THIS IS UNPUBLISHED PROPRIETARY SOURCE CODE OF InterSoft Co.
* The copyright notice above does not evidence any
* actual or intended publication of such source code.
*
* $Id: ulicense.h,v 1.7 2002/05/15 16:26:47 hardaiz Exp $
*
* DESCRIPTION
*
* Definitions for users licenses system
*********************************************************************/

#ifndef __ULICENSE__H_
#define __ULICENSE__H_

#define USERS_LIMIT_REACHED(pkg_num)	(const char*)construct_message(pkg_num)
#define CANTOPEN_FILE			"\nterminfo error -2\n"	
#define GENERAL_LICENSE_ERROR		"\nterminfo error -4\n"	
#define BAD_LICENSE_MAGIC		"\nterminfo error -9\n"
#define LICENSE_TAMPERED_ERROR		"\nterminfo error -10\n"
	
#define AVAILABLE_ENTRY		(char)'\0'
#define LICENSE_BAD		-1
#define LICENSE_EXPIRED		-2 
#define MAX_TTY_NAME		256
#define LICENSE_FILENAME     	(const char*)unmangle(mangled_licensefilename)	
#define LICENSE_MAGIC		(int)0x8a8a8a8a
#define LICENSE_ENCRYPT		1
#define LICENSE_DECRYPT		0
#define SEMAPHORE_KEY		(key_t) 9478
#define SHAREDMEM_KEY		(key_t) 4398

bool ResetCrackerUserLicenses();

/**
 * class CUsersLicensesEntry
 *
 * Represents one entry in the user licenses file
 */

class CUsersLicensesEntry {
	bool deleted;					// is this entry deleted?
	char szttyName[MAX_TTY_NAME];			// Tty name
	int iPkgType;					// Package type of the program running
	int	pid;					// PID of the process
public:
	CUsersLicensesEntry() : deleted(false) {};
	~CUsersLicensesEntry() {};

	int getPkgType() { return iPkgType; };		// returns the entry package type
	int setPkgType(int pkgtype) { return (iPkgType = pkgtype);};

	void setTtyName(char *str) { strncpy(szttyName, str, MAX_TTY_NAME); };
	char *getTtyName(void)	{ return szttyName; }
	void setPid(int xxpid) { pid = xxpid; };
	int getPid() { return pid; };
	void erase() { deleted=true; };
	void unerase() { deleted=false; }
	bool isErased() { return deleted; };
	bool operator==(CUsersLicensesEntry& ulentry)
	{

		if( (getPid() == ulentry.getPid()) &&
		    (!strcmp( getTtyName() , ulentry.getTtyName() )) &&
			(getPkgType() == ulentry.getPkgType()) &&
			(isErased() == ulentry.isErased()) ) {
				 return true;
				}
			return false;

	}
		
};

/**
 * class CUsersLicensesHeader
 *
 * used as the header of the licenses file
 */

class CUsersLicensesHeader
{

	public:

	int iMagic;

	CUsersLicensesHeader() { iMagic = LICENSE_MAGIC; };
	~CUsersLicensesHeader() {};
	
	bool isValid(void) { return (iMagic == LICENSE_MAGIC) ? true : false; }; 
};


/**
 * class CUsersLicensesFile
 *
 * Used to manage the user licenses file
 */

class CUsersLicensesFile
{
			
	public:


	CUsersLicensesFile() {};
	~CUsersLicensesFile() {};

	void encrypt(CUsersLicensesHeader*);		// Encrypts the header
	void decrypt(CUsersLicensesHeader*);		// Decrypts the header
	void encrypt(CUsersLicensesEntry*);		// Encrypts an entry
	void decrypt(CUsersLicensesEntry*);		// Decrypts an entry
	bool createFile();				// creates the users licenses file
	bool deleteFile();				// deletes the users licenses file
	bool fileExists();				// Checks if the license file exists.
	void purgeLicensesFile();			// Erases orphans entries from the file 
	bool addEntry(CUsersLicensesEntry*);		// Adds one entry to the license file
	bool delEntry(CUsersLicensesEntry*);		// Deletes one entry from the license file
	int numPackageTtys(int);			// counts the number of entries in the
							// license file that belong to a
							// certain package.
};

/**
 * used to encrypt the license file
 */

#define decrypt_license(p, l)	crypt_license(p, l, LICENSE_DECRYPT)
#define encrypt_license(p, l)	crypt_license(p, l, LICENSE_ENCRYPT)

#endif
