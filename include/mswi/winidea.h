/********************************************************************
* Copyright (c) 1987 InterSoft Co.  All Rights Reserved
* THIS IS UNPUBLISHED PROPRIETARY SOURCE CODE OF InterSoft Co.
* The copyright notice above does not evidence any
* actual or intended publication of such source code.
*
* $Id: winidea.h,v 1.2 1999/02/03 15:46:05 mauro Exp $
*
* DESCRIPTION: windows.h interface header file
*********************************************************************/

#ifndef _INC_WINIDEA
#define _INC_WINIDEA

#ifdef CreateCursor
#  undef CreateCursor
#endif

#ifdef ClosePrinter
#	undef ClosePrinter
#endif

#if defined  (__VC__) && defined (IDEAWIN)
#	undef Free
#	undef Alloc
#	define DATE	dummyDATE
#	include <windows.h>
#	undef DATE
#	define Alloc(n)(def_Alloc(n,__FILE__,__LINE__))
#	define Free(p)(def_Free(p,__FILE__,__LINE__))
#else
#	include <windows.h>
#endif

#define CreateCursor    DbCreateCursor
#define ClosePrinter	_ClosePrinter

#define WINERROR ERROR
#define ERROR (-1)
#endif  /* _INC_WINIDEA */
