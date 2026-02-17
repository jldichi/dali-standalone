/********************************************************************
* Copyright (c) 1991 InterSoft Co.  All Rights Reserved
* THIS IS UNPUBLISHED PROPRIETARY SOURCE CODE OF InterSoft Co.
* The copyright notice above does not evidence any
* actual or intended publication of such source code.
*
* $Id: osferror.cc,v 1.3 2008/01/24 14:35:49 hardaiz Exp $
*
* DESCRIPTION
* Error managing for OsFiles
*********************************************************************/

#include <ifound.h>
#include <local/osfile.h>
#include <local/phyfile.h>
#include <local/strfile.h>

#include <errno.h>

static IFPCCPI errFunc = NULL;

void OsFile::setHandler(IFPCCPI f)
{
	errFunc = f;
}

Int PhysFile::osError(const String &sys_call)
{
	// To avoid recursion
	static bool aborting = false;
	if (aborting) return false;

	osfile()->errNo(errno);

#ifdef HAVE_ERRLIST
	char *sysMsg = errno >= sys_nerr ? NULL : sys_errlist[errno]; 
#else
	char *sysMsg = "";
#endif
	String fname = fullName();
	String msg;	
	if(sysMsg != NULL)
	{
		msg = toString("%S: %s (%d)\n\tOsFile: %S\n", &sys_call, sysMsg, 
							  errno, &fname);
	}
	else
	{
		msg = toString("%S: %s (%d)\n\tOsFile: %S\n", &sys_call, "IDEAFIX ERROR - errno not found",
							  errno, &fname);
	}

	if (errFunc != NULL) {
		if (errFunc(toCharPtr(msg), errno)) {
			aborting = true;
			stop(ERR);
			return false;
		}
		return true;
	}

	return ariError(toCharPtr(msg));
}

void PhysFile::tryLockMsg(bool disp)
{
	if (disp)
		notice(libMsg("TRYLOCK"));
}
