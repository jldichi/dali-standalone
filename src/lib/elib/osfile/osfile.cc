/********************************************************************
* Copyright (c) 1991 InterSoft Co.  All Rights Reserved
* THIS IS UNPUBLISHED PROPRIETARY SOURCE CODE OF InterSoft Co.
* The copyright notice above does not evidence any
* actual or intended publication of such source code.
*
* $Id: osfile.cc,v 1.17 2008/05/29 16:42:19 cristiand Exp $
*
* DESCRIPTION: Implementation of OsFiles
*********************************************************************/

#ifdef __GNUC__
#	pragma implementation
#endif

#include <local/osfile.h>
#include <local/phyfile.h>
#include <local/physlout.h>

#include <fcntl.h>
#include <sys/stat.h>
#include <errno.h>

#ifdef HAVE_UNDERSCORE_NAMES
#	define stat _stat
#endif

OsFile::OsFile()
:	name_d(NULL_STRING),
	ext_d(NULL_STRING),
	errno_d(0),
	nparts_d(0),
	pfiles_d(NULL),
	plentry_d(NULL),
	nreads_d(0),
	nwrites_d(0),
	bsize_d(0)
{
}

OsFile::OsFile(const String &fname, Int fflags)
:	name_d(NULL_STRING),
	ext_d(NULL_STRING),
	errno_d(0),
	nparts_d(0),
	pfiles_d(NULL),
	plentry_d(NULL),
	nreads_d(0),
	nwrites_d(0),
	bsize_d(0)
{
	open(NULL_STRING, fname, NULL_STRING, fflags);
	// Check it is really a file.
	if (!isFileIfExists())
		error(name() + " " + libMsg("NOTREGFILE"));
}

OsFile::OsFile(const String &fpath, const String &fname,  const String &fext, 
			   Int fflags)
:	name_d(NULL_STRING),
	ext_d(NULL_STRING),
	errno_d(0),
	nparts_d(0),
	pfiles_d(NULL),
	plentry_d(NULL),
	nreads_d(0),
	nwrites_d(0),
	bsize_d(0)
{
	open(fpath, fname, fext, fflags);
	// Check it is really a file.
	if (!isFileIfExists())
		error(name() + " " + libMsg("NOTREGFILE"));
}

OsFile::OsFile(PhysLayout *path, const String &ln, 
			   const String &ext, Int fflags, Int bz, const String &pn)
:	name_d(NULL_STRING),
	ext_d(NULL_STRING),
	errno_d(0),
	nparts_d(0),
	pfiles_d(NULL),
	plentry_d(NULL),
	nreads_d(0),
	nwrites_d(0),
	bsize_d(0)
{                     
	open(path, ln, ext, fflags, bz, pn);
	// Check it is really a file.
	if (!isFileIfExists())
		error(name() + " " + libMsg("NOTREGFILE"));
}

bool OsFile::open(const String &fpath, const String &fname, 
				  const String &fext, Int fflags)
{
	closeAndDelete();

	name_d = fname;
	ext_d = fext;
	bsize_d = 0;
	errno_d = 0;
	plentry_d = NULL;
		
	nparts_d = 1;
	pfiles_d = tr_new PhysFile[nparts_d];
	assert(pfiles_d != NULL);	
	pfiles_d->open(this, fpath);

	return open(fflags);
}

bool OsFile::open(PhysLayout *path, const String &ln, const String &fext, 
				  Int fflags, Int bz, const String &pn)
{
	assert(bz >= 0);
	
	closeAndDelete();

	lName_d			= ln;
	name_d			= pn != NULL_STRING ? pn : lName_d;
	ext_d			= fext;
	errno_d			= 0;
	bsize_d			= bz;

	plentry_d		= path->find(lName_d);

	if (plentry_d == NULL)
		error(libMsg("NOPARTS"), &name_d, &ext_d, &path->fileName());
		
	nparts_d		= plentry_d->nParts();
	pfiles_d		= tr_new PhysFile[nparts_d];
	assert(pfiles_d != NULL);

	for (Int j = 0; j < nparts_d; j++)
		pfiles_d[j].open(this, plentry_d->part(j));

	return open(fflags);
}

bool OsFile::open(Int fflags)
{
	errno_d = 0;

	Int flags;
	
	// map flags into os ones
	if (fflags & READ)
		flags = (fflags & WRITE) ? O_RDWR : O_RDONLY;
	else
		flags = (fflags & WRITE) ? O_WRONLY : O_RDONLY;

#ifdef HAVE_TEXT_FILES
	flags |= O_BINARY;
#endif

	if (fflags & CREAT) {
		flags |= O_CREAT;
		if (fflags & EXCL) flags |= O_EXCL;
	}
	if (fflags & TRUNC)  flags |= O_TRUNC;
	if (fflags & APPEND) flags |= O_APPEND;
#if HAVE_SYNC
	if (fflags & SYNC)   flags |= O_SYNC;
#endif

	setPhyFileFlags(flags);
	
	// check if file exists
	if (fflags & FAIL)
		checkIfExists();

	// Reset counters
	resetCounters();

	return errno_d ? false : true;
}

void OsFile::setPhyFileFlags(Int flags)
{
	for (Int i = 0; i < nparts_d; i++)
		pfiles(i)->flags(flags);
}

#ifdef _DLL
class ntiext {
#elif	SHAREDLIB
class libiext_s {
#else
class libiext {
#endif
public:
	static void foo()
	{
		;
	}	
};

void foolink()
{
#ifdef _DLL
	ntiext::foo();
#elif	SHAREDLIB
	libiext_s::foo();
#else
	libiext::foo();
#endif
}

bool dummyccall = false;

OsFile::~OsFile()
{
    if (dummyccall)
        foolink();
	closeAndDelete();
}

void OsFile::checkIfExists()
{
	for (Int i = 0; i < nparts_d && errno_d == 0; i++)
		pfiles(i)->updateFd("open", true);
}

void OsFile::closeAndDelete()
{
	close();    
	//assert(pfiles_d != NULL);
	if (pfiles_d != NULL) {
		//tr_delete_vec(pfiles_d);
		delete [] (pfiles_d);
	}
	nparts_d = 0;
	pfiles_d = NULL;
}

void OsFile::resetCounters()
{
	for (Int i = 0; i < nparts_d; i++)
		pfiles(i)->resetCounters();
}
	
void OsFile::close()
{
	for (Int i = 0; i < nparts_d; i++)
		pfiles(i)->close();
}

bool OsFile::syncCache()
{
	return false;
}

bool OsFile::flush()
{
	return false;
}

void OsFile::syncAllCache()
{
	PhysFile::syncAllCache();
}

void OsFile::flushAll()
{
	PhysFile::flushAll();
}

short OsFile::getFd()
{ 
	checkMethod("getFd");
	return pfiles()->fd();
}

PhysFile *OsFile::physFile(Long &offset)
{
	if (plentry_d == NULL || nparts_d == 1)
		return pfiles();

	return pfiles(plentry_d->part(offset, bsize_d));
}

Int OsFile::write(const void *buff, Long offset, Int length)
{
	PhysFile* pf = physFile(offset);

	nwrites_d++;
    return pf->write(buff, Int(offset), length);
}

Int OsFile::awrite(const void *buff, Long offset, Int length)
{
	PhysFile* pf = physFile(offset);

	nwrites_d++;
	return pf->awrite(buff, Int(offset), length);
}
	
void OsFile::waitForWrites()
{
	PhysFile::waitForWrites();
}

Int OsFile::read(void *buff, Long offset, Int length)
{
	PhysFile* pf = physFile(offset);

	nreads_d++;
	return pf->read(buff, Int(offset), length);
}

Int	OsFile::read(Int part, void *buff, Int offset, Int length)
{
	assert(plentry_d != NULL);

	nreads_d++;

	return pfiles(part)->read(buff, offset, length);
}

Int OsFile::sync()
{
	for (Int i = 0; i < nparts_d; i++)
		pfiles(i)->sync();
	return OK;
}

String OsFile::fullName(Int part)
{
	assert(plentry_d != NULL);
	return pfiles(part)->fullName();
}

String OsFile::fullName()
{
	checkMethod("fullName");
	return pfiles()->fullName();
}

bool OsFile::isOpen()
{
	checkMethod("isOpen");
	return pfiles()->isOpen();
}

const String &OsFile::path()
{
	checkMethod("path");
	return pfiles()->path();
}

Int OsFile::nReads()
{
	return PhysFile::nReads();
}

Int OsFile::nWrites()
{
	return PhysFile::nWrites();
}

PhysFile *OsFile::pfiles(Int i)
{
	assert(pfiles_d != NULL);
	assert(i >= 0 && i < nparts_d);

	return &pfiles_d[i];
}

const PhysFile *OsFile::pfiles(Int i) const
{
	assert(pfiles_d != NULL);
	assert(i >= 0 && i < nparts_d);
	return &pfiles_d[i];
}

bool OsFile::isPacked()
{
	return plentry_d != NULL && plentry_d->packedEntry() != NULL;
}

Int OsFile::nObjReads()
{
	return nreads_d;
}

Int OsFile::nObjWrites()
{
	return nwrites_d;
}

bool OsFile::isFileIfExists()
{
	struct stat st;
	if (stat(toCharPtr(name()) , &st) < 0) {
		if (errno == ENOENT)
			return true;
		return false;
	}
	return (st.st_mode & S_IFMT) == S_IFREG;
}

bool OsFile::isDir()
{
	struct stat st;
	if (stat(toCharPtr(name()), &st) < 0) {
		return false;
	}
	return (st.st_mode & S_IFMT) == S_IFDIR;
}

void OsFile::freeAnFd()
{
	pfiles(0)->freeAnFd("OsFile::freeAnFd");
}

Int	OsFile::write(Int part, void *buff, Int offset, Int length)
{
	assert(plentry_d != NULL);

	nwrites_d++;

	return pfiles(part)->write(buff, offset, length);
}

Int	OsFile::awrite(Int part, void *buff, Int offset, Int length)
{
	assert(plentry_d != NULL);

	nwrites_d++;

	return pfiles(part)->awrite(buff, offset, length);
}
