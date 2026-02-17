/********************************************************************
* Copyright (c) 1991 InterSoft Co.  All Rights Reserved
* THIS IS UNPUBLISHED PROPRIETARY SOURCE CODE OF InterSoft Co.
* The copyright notice above does not evidence any
* actual or intended publication of such source code.
*
*
* $Id: osfstat.cc,v 1.2 1998/04/30 22:14:11 mauro Exp $
*
*
* DESCRIPTION:
*				Implementation of OsFiles
*
*********************************************************************/

#include <local/osfile.h>
#include <local/phyfile.h>
#include <sys/types.h>
#include <sys/stat.h>			// fstat function

#ifdef HAVE_UNDERSCORE_NAMES
#	define stat	_stat
#	define fstat	_fstat
#endif

Int OsFile::size()
{
	checkMethod("size");
	return pfiles()->size();
}
	
Time OsFile::mtime()
{
	checkMethod("mtime");
	return pfiles()->mtime();
}

Int OsFile::owner()
{
	checkMethod("owner");
	return pfiles()->owner();
}

Int PhysFile::owner()
{
	updateFd("owner");		// update file descriptor

	do {
		struct stat st;
	    if (fstat(fd_d, &st) != ERROR)
			return st.st_uid;
	} while (osError("owner"));
	return ERR;
}

Time PhysFile::mtime()
{
	updateFd("mtime");		// update file descriptor
	do {
		struct stat st;
	    if (fstat(fd_d, &st) != ERROR)
			return osTime(st.st_mtime);
	} while (osError("mtime"));
	return NULL_VALUE;
}

Int PhysFile::size()
{
	updateFd("size");		// update file descriptor

	do {
		struct stat st;
	    if (fstat(fd_d, &st) != ERROR)
			return st.st_size;
	} while (osError("size"));
	return ERR;
}
