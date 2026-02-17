/********************************************************************
* Copyright (c) 1994 InterSoft Co.  All Rights Reserved
* THIS IS UNPUBLISHED PROPRIETARY SOURCE CODE OF InterSoft Co.
* The copyright notice above does not evidence any
* actual or intended publication of such source code.
*
* $Id: types.h,v 1.2 2003/03/27 15:55:26 albertoi Exp $
*
* DESCRIPTION
*********************************************************************/

#ifndef CFIX_TYPES_H
#define CFIX_TYPES_H

typedef unsigned long attr_type;
typedef short DATE;
typedef unsigned short TIME;
typedef unsigned short UShort;
typedef unsigned long ULong;
typedef unsigned char UChar;
#if !defined(HAVE_BOOL)
	#ifdef PIQL
		#define bool char 
	#else
	typedef signed char bool;
	#endif
#endif

#ifndef INT_TYPEDEF
#define INT_TYPEDEF
typedef long Int;
typedef unsigned long UnInt;
#endif

typedef short type;
typedef short sqltype;

#include <ifound/typeid.h>

#ifndef FALSE
#	define	FALSE	(0)
#endif
#ifndef TRUE
#	define TRUE		(1)
#endif
#ifndef NO
#	define NO		FALSE
#endif
#ifndef YES
#	define YES		TRUE
#endif
#define NULL_BOOL	(-1)

//#define MAX_LONG ((long)0x7FFFFFFF)
//#define MIN_LONG ((long)0x80000000)
#define MAX_LONG ((long)0x7FFFFFFFFFFFFFFF)
#define MIN_LONG ((long)0x8000000000000000)
#define NULL_LONG	MIN_LONG

#define MAX_SHORT ((short) 0x7FFF)
#define MIN_SHORT ((short)(-32768))
#define NULL_SHORT	MIN_SHORT

/* for internal use only */
#define _MAX_BYTE	((signed char) 0x7F)
#define _MIN_BYTE	((signed char)(-128))
#define _NULL_BYTE		_MIN_BYTE

#define NULL_BYTE		NULL_SHORT	
#define MAX_BYTE		MAX_SHORT	
#define MIN_BYTE		MIN_SHORT	

#define MAX_DOUBLE	(1e50)
#define MIN_DOUBLE	(-1e50)
#define NULL_DOUBLE	MIN_DOUBLE

#define NULL_STR	""
#define MIN_DATE	((DATE) (-32768))
#define TODAY_DATE	((DATE) 0x7FFF)
#define MAX_DATE	((DATE) (TODAY_DATE - 1))
#define NULL_DATE	MIN_DATE

#define MIN_TIME	((TIME) 0)	    /* 00:00:00 */
#define MAX_TIME	((TIME) 43200)  /* 23:59:58 */
#define NULL_TIME	MIN_TIME       	
#define HOUR_TIME	((TIME) 0xFFFF)

#endif
