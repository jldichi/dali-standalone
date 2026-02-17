/********************************************************************
* Copyright (c) 1991 InterSoft Co.  All Rights Reserved
* THIS IS UNPUBLISHED PROPRIETARY SOURCE CODE OF InterSoft Co.
* The copyright notice above does not evidence any
* actual or intended publication of such source code.
*
* $Id: osflink.cc,v 1.2 2004/09/29 15:36:44 damianm Exp $
*
*
* DESCRIPTION
* Implementation of OsFiles
*********************************************************************/

#include <local/osfile.h>
#include <local/phyfile.h>
#include <ifound/stdc.h>
#include <errno.h>
#include <unistd.h>

void OsFile::unLink()
{
	for (Int i = 0; i < nparts_d; i++)
		pfiles(i)->unLink();
}

bool OsFile::empty()
{
	unLink();
	close();
	return open(OsFile::READ|OsFile::WRITE|OsFile::FAIL|
				OsFile::CREAT|OsFile::TRUNC);
}

void OsFile::rename(const String &n, const String &e)
{
	for (Int i = 0; i < nparts_d; i++)
		pfiles(i)->rename(n, e);
	name_d	= n;
	ext_d	= e;
}

void OsFile::rename(const String &p, const String &n, const String &e)
{
	checkMethod("rename");
	pfiles()->rename(p, n, e);
	name_d = n;
	ext_d = e;
}
	
void PhysFile::rename(const String &n, const String &e)
{
	rename(path_d, n, e);
}

void PhysFile::rename(const String &p, const String &n, const String &e)
{
	if (osfile()->name() == NULL_STRING) {
		errno = EBADF;
		while (osError("rename"))
			;
		return;
	}
	String new_name = fullName(p, n, e);
	String old_name = fullName();

	do {
#ifdef HAVE_RENAME	  
		if (::rename(toCharPtr(old_name), toCharPtr(new_name)) != ERROR)
			break;
#else
		::unlink(toCharPtr(new_name));
		if (::link(toCharPtr(old_name), toCharPtr(new_name)) != ERROR
			&& ::unlink(toCharPtr(old_name)) != ERROR) 
			break;
#endif
	} while (osError("rename"));
	path_d = p;
}

void PhysFile::unLink()
{		
	if (osfile()->name() == NULL_STRING) {
		errno = EBADF;
		while (osError("unlink"))
			;
		return;
	}

	do {
		String fname = fullName();
		if (::unlink(toCharPtr(fname)) != ERROR || errno==ENOENT) 
			break;
	} while (osError("unlink"));
}

