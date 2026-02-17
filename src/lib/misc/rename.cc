/********************************************************************
* Copyright (c) 1995 longerSoft Co.  All Rights Reserved
* THIS IS UNPUBLISHED PROPRIETARY SOURCE CODE OF longerSoft Co.
* The copyright notice above does not evidence any
* actual or intended publication of such source code.
*
* $Id: rename.cc,v 1.5 2010/09/28 19:52:53 hardaiz Exp $
*
* DESCRIPTION:
*
*********************************************************************/
#include <ideafix.h>

#if defined(SunSolx86) || defined(hpux) || defined(AIX5) || defined(SunSol)
#include <unistd.h>
#endif

#include <ifound/str.h>
#include <ifound/stdc.h>

bool renameFile(const String &oldName, const String &newName)
{
	const char *old_name = toCharPtr(oldName);
	const char *new_name = toCharPtr(newName);

#ifdef HAVE_RENAME	  
	return rename(old_name, new_name) != ERR;
#else
	unlink(new_name);
	return link(old_name, new_name) != ERR && unlink(old_name) != ERR; 
#endif
}
