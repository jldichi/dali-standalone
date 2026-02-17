/***********************************************************************
* Copyright (c) 1991 InterSoft Co.  All Rights Reserved
* THIS IS UNPUBLISHED PROPRIETARY SOURCE CODE OF InterSoft Co.
* The copyright notice above does not evidence any
* actual or intended publication of such source code.
*
* $Id: dirdefs.h,v 1.1.1.1 1996/04/16 18:51:31 gustavof Exp $
*
* DESCRIPTION
* Directory definitions
*********************************************************************/

#ifndef DIRDEFS_H
#define DIRDEFS_H

#include <ifound/osdep.h>

#if HAVE_DOS_H
#	include <dos.h>
#else
#	if defined(DIRENT) || defined(_POSIX_VERSION)
#		include <dirent.h>
#		define DIR_NAMELEN(dirent) (strlen((dirent)->d_name))
#	else
#		define DIR_NAMELEN(dirent) ((dirent)->d_namlen)
#		ifdef USG
#			ifdef SYSNDIR
#				include <sys/ndir.h>
#			else
#				include <ndir.h>
#			endif
#		else
#			include <sys/dir.h>
#		endif
#	endif
#endif

#if HAVE_CLOSEDIR_BUG
#	ifdef __cplusplus
		extern "C" {
#	endif

	extern int closeDir(DIR *d);

#	ifdef __cplusplus
		}
#	endif

#	define closedir(a)	closeDir(a)
#endif

#endif
