/* LINTLIBRARY */
/********************************************************************
* Copyright (c) 1992 InterSoft Co.  All Rights Reserved
* THIS IS UNPUBLISHED PROPRIETARY SOURCE CODE OF InterSoft Co.
* The copyright notice above does not evidence any
* actual or intended publication of such source code.
*
*
* $Id: unlink.cc,v 1.3 2004/09/29 17:23:49 damianm Exp $
*
*
* DESCRIPTION:
*
* ROUTINE       |  PURPOSE
*---------------+---------------------------------------------------
*
*********************************************************************/

#include <unistd.h>

#include <ifound/str.h>
#include <ifound/stdc.h>

#include <sys/stat.h>

#ifdef HAVE_UNDERSCORE_NAMES
#	define stat _stat
#endif

int unlinkFile(const String &file)
{
	struct stat st;

	if (statFile(file, &st) == ERR)
		return ERR;

	// if is a directory I have to call rmdir function
	if (st.st_mode & S_IFDIR)
		return rmdir(toCharPtr(file));

	return unlink(toCharPtr(file));
}

