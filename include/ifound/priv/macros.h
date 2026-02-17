/********************************************************************
* Copyright (c) 1994 InterSoft Co.  All Rights Reserved
* THIS IS UNPUBLISHED PROPRIETARY SOURCE CODE OF InterSoft Co.
* The copyright notice above does not evidence any
* actual or intended publication of such source code.
*
* $Id: macros.h,v 1.2 2000/05/15 20:02:05 charlie Exp $
*
* DESCRIPTION
* General purpose private macro definitions
*********************************************************************/

#ifndef	PRIV_MACROS_H
#define PRIV_MACROS_H

#include <ifound/osdep.h>
#include <ifound/btypes.h>

#define lduint(p)												\
	((unsigned short)((((((char *)(p))[0])) << 8) |				\
	((((char*)(p))[1]) & 0xFF)))
#define ldint(p)												\
	((short)((((((char *)(p))[0])) << 8) | ((((char *)(p))[1]) & 0xFF)))
#define stint(i, p)												\
	(((char *)(p))[0] = (char)((i) >> 8), ((char *)(p))[1] = (char)(i))
#define stuint(i, p)	stint(i, p)

#define uMemCmp(ret, ax, bx, l)										\
	do { 															\
		register char *__a__ = ax;									\
		register char *__b__ = bx;									\
		register char *__to__ = (__a__) + (l);						\
		do {														\
			(ret) = (UnChar)(*(__a__)++) - (UnChar)(*(__b__)++);		\
		} while (!(ret) && (__a__) < __to__);						\
	} while (0)

/* Default PATH_MAX */
#ifndef PATH_MAX
#	ifdef linux
#		include <linux/limits.h>
#	else
#		define PATH_MAX 512
#	endif
#endif

/* Separator between sysname and appname */
#define SYSNAME_SEPAR	":"

#define ABNORMAL_STOP	0x80000000

#endif // PRIV_MACROS_H
