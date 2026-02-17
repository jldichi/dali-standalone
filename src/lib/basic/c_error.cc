/********************************************************************
* Copyright (c) 1987 InterSoft Co.  All Rights Reserved
* THIS IS UNPUBLISHED PROPRIETARY SOURCE CODE OF InterSoft Co.
* The copyright notice above does not evidence any
* actual or intended publication of such source code.
*
* $Id: c_error.cc,v 1.9 2010/09/28 19:50:32 hardaiz Exp $
*
* DESCRIPTION
* Error function that display a message an then abort the program
*
* ROUTINE       |  PURPOSE
*---------------+---------------------------------------------------
*Error			|	Display the Error msg and abort the program.
*Warning		|	Display the Warning msg and return.
*Notice			|	Display the Notice msg and return.
*SetMessager	|	Set the routine to display the Error msg.
*********************************************************************/

#include <errno.h>
#include <string.h>

#include <ideafix.h>
#include <cfix.h>
#include <ideafix/priv/gndefs.h>

extern "C" IFPICCPCCP SetMessager(IFPICCPCCP f)
{
   	IFPICCPCCP aux = displayHandler(f);
	displayErrorHandler(f);
	fatalErrorHandler(f);
	return aux;
}

#if (!defined(__NT__) && \
     !defined(hppa) && \
     !defined(hpux) && \
     !defined(AIX5) && \
     !defined(Linux7) && \
     !defined(LinuxGcc3) && \
     !defined(LinuxGcc4) && \
     !defined(SunSol) )
 	extern "C" IFPICCPVL SetAriMessager(IFPICCPVL handler)
#else
 	extern "C" IFPICCPCCP SetAriMessager(IFPICCPCCP handler)
#endif
{
	return ariErrorHandler((IFPICCPCCP) handler);
}
     
extern "C" int errno;

// Display an Error message and ask the user to retry, abort or ignore.
int _SysError(char *func)
{
	int theErrno = errno;

	return AriError(_DbErrmsg(IO_AB_RE_IG), func, strerror(theErrno), theErrno);
}
