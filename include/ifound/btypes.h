/***********************************************************************
* Copyright (c) 1994 InterSoft Co.  All Rights Reserved
* THIS IS UNPUBLISHED PROPRIETARY SOURCE CODE OF InterSoft Co.
* The copyright notice above does not evidence any
* actual or intended publication of such source code.
*
* $Id: btypes.h,v 1.3 2003/12/04 18:15:49 albertoi Exp $
*
* DESCRIPTION
* Basic type definitions
*********************************************************************/

#ifndef	BTYPES_H
#define	BTYPES_H


#ifndef BYTE_TYPEDEF
typedef signed char Byte;
typedef unsigned char UnByte;
typedef Byte *BytePtr;
#endif

#ifndef CHAR_TYPE
#define CHAR_TYPE
typedef signed char Char;
typedef unsigned char UnChar;
#endif

#ifndef SHORT_TYPE
#define SHORT_TYPE
typedef signed short Short;
typedef unsigned short UnShort;
#endif

#ifndef INT_TYPEDEF
#define INT_TYPEDEF
typedef signed long Int;
typedef unsigned long UnInt;
#endif

#ifndef LONG_TYPEDEF
#define LONG_TYPEDEF
#ifndef HAVE_LONGLONGBUG
	typedef long signed long Long;
	typedef long unsigned long UnLong;
#else
	typedef signed long Long;
	typedef unsigned long UnLong;
#endif
#endif

#ifndef HAVE_BOOL
#ifndef __VC__
	typedef signed char bool;
#	ifdef true
#		undef true
#	endif
#	define true ((signed char) 1)
#	ifdef false
#		undef false
#	endif
#	define false ((signed char) 0)
#else 
#	ifndef _cplusplus
#		define HAVE_BOOL
#	endif
#endif
#endif

typedef short WideChar;
typedef short DateFmt;
typedef short TimeFmt;
typedef char FmtBuff[256];

typedef short TypeId;
#include <ifound/typeid.h>

/* Options to format every date - read from config file */
#define INTER_FMT	(0)
#define USA_FMT		(1 << 0)

/* Options to format a particular date - passed as parameter on each call */
#define YEAR2_FMT	(1 << 1)
#define SEPAR_FMT	(1 << 2)
#define DAYNAME_FMT	(1 << 3)
#define LDAYNAME_FMT	(1 << 4)
#define MONTHNAME_FMT	(1 << 5)
#define LMONTHNAME_FMT	(1 << 6)
#define DEFAULT_FMT	(SEPAR_FMT)

/* Options to format a particular time */
#define SEPAR_TFMT		(1 << 1)
#define SECONDS_TFMT	(1 << 2)
#define DATE_TFMT		(1 << 3)
#define FULL_TFMT		(SEPAR_TFMT|SECONDS_TFMT|DATE_TFMT)
#define DEFAULT_TFMT	(SEPAR_TFMT|SECONDS_TFMT)

#ifndef false
#ifdef __cplusplus
#	define false	false
#else
#	define false	0
#endif
#endif

#ifndef true
#ifdef __cplusplus
#	define true		true
#else
#	define true		1
#endif
#endif

#ifndef BOOL_NULL
#	define BOOL_NULL	(-1)
#endif

#ifndef NO
#	define NO		false
#endif

#ifndef YES
#	define YES		true
#endif

#define TODAY		"NOW"
#define NOW			"NOW"

/* Useful constants */

#undef BYTE_MAX
#undef BYTE_MIN
#undef BYTE_NULL
#undef UN_INT_MAX
#undef INT_MAX
#undef INT_MIN
#undef INT_NULL
#undef LONG_MAX
#undef LONG_MIN
#undef LONG_NULL
#undef SHORT_MAX
#undef SHORT_MIN
#undef SHORT_NULL
#undef UN_SHORT_MAX
#undef DOUBLE_MAX
#undef DOUBLE_MIN
#undef DOUBLE_NULL
#undef SPECIAL_VALUE

#define BYTE_MAX		(127)
#define BYTE_MIN		(-BYTE_MAX-1)
#define BYTE_NULL		BYTE_MIN

//#define UN_INT_MAX		0xffffffff
//#define INT_MAX			2147483647L
//#define INT_MIN			(-INT_MAX-1)
//<<<<<<< .mine
#define UN_INT_MAX		0xFFFFFFFFFFFFFFFF
//=======
#define UN_INT_MAX 		0xFFFFFFFFFFFFFFFF
//>>>>>>> .r331
#define INT_MAX			LONG_MAX	
#define INT_MIN			LONG_MIN	
#define INT_NULL		INT_MIN

#define LONG_MAX 		0x7FFFFFFFFFFFFFFF
#define LONG_MIN 		0x8000000000000000
#define LONG_NULL       LONG_MIN

// DAMSU BUG SE VE NULO

/*
#define LONG_MAX		INT_MAX
#define LONG_MIN		INT_MIN
#define LONG_NULL		INT_NULL
*/
#define SHORT_MAX		32767
#define SHORT_MIN		(-SHORT_MAX-1)
#define SHORT_NULL		SHORT_MIN

#define UN_SHORT_MAX		0xffff

#define DOUBLE_MAX		(1e50)
#define DOUBLE_MIN		(-DOUBLE_MAX)
#define DOUBLE_NULL		DOUBLE_MIN

#define SPECIAL_VALUE		(INT_MAX - 1)

#endif
