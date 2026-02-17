/***********************************************************************
* Copyright (c) 1991 InterSoft Co.  All Rights Reserved
* THIS IS UNPUBLISHED PROPRIETARY SOURCE CODE OF InterSoft Co.
* The copyright notice above does not evidence any
* actual or intended publication of such source code.
*
* osdep.h,v 1.1.1.1 1996/04/16 18:51:31 gustavof Exp
*
* DESCRIPTION
* Operating system dependant defines
*********************************************************************/

#ifndef OSDEP_H
#define OSDEP_H

#ifdef DEBUG
#	define	ISDEBUG	1
#endif

#ifdef ISDEBUG
#	define TRACE_MEMORY	1
#endif

/*
	Defines that we expect the compiler to define for the following
	operating systems are...

	AIX 3.x/4.x/5.x					_AIX
	A/UX 3 (Apple Unix)			AUX
	SunOS						sun
	Silicon Graphics Indigo		mips && sgi
	HP9000/800 w/HPUX			hppa && hpux
	Sco Unix 3.2v2+				M_UNIX
	Sco Unix 3.2v5.0.x			M_UNIX && _M_SYSV _M_SYS5
	Borland C++/DOS				MSDOS && !__ZTC__
	Symantec C++/DOS			MSDOS && __ZTC__ && __SC__
	Zortech C++/DOS				MSDOS && __ZTC__ && !__SC__
	Symantec C++/Windows		MSDOS && __ZTC__ && __SC__ && _WINDOWS
	Symantec C++/Windows 32		MSDOS && __ZTC__ && __SC__ && _WINDOWS && __NT__
 	MS - Visual C++			_WIN32 && _M_IX86 && __NT__ && __VC__
 	MS - Visual C++	/Windows 32	_WIN32 && _M_IX86 && _WINDOWS __NT__ && __VC__
	Xenix 286					M_XENIX && CPU_I286
	Xenix 386					M_XENIX && CPU_I386
	Irix 4						mips && sgi && _IRIX4
	Irix 5						mips && sgi && !_IRIX4
	Linux RedHat 4.x				Linux4
	Linux RedHat 5.x				Linux5
	Linux RedHat 6.x				Linux6
	Linux RedHat 7.x				Linux7
	Interactive 386				<default>
	Solaris Intel				sun && __svr4__ && __i386__
	Solaris Sun				sun && __svr4__
*/

#ifdef __GNUC__

/* const & noreturn */
#if __GNUC__ < 2 || (__GNUC__ == 2 && __GNUC_MINOR__ < 5) || (defined(__cplusplus) && __GNUC__ == 2 && __GNUC_MINOR__ <= 5)
  /* Old GCC way. */
#	define CONSTVALUE_FUNC	__const__
#	define CONSTVALUE_FUNC_2
#	define NORETURN_FUNC	__volatile__
#	define NORETURN_FUNC_2
#else
  /* New GCC way. */
#	define	CONSTVALUE_FUNC
#	if defined(const) || !defined(__STDC__)
#		define	CONSTVALUE_FUNC_2	/* We don't want empty __attribute__ (()). */
#	else
#		define	CONSTVALUE_FUNC_2	__attribute__ ((const))
#	endif
#	define NORETURN_FUNC
#	ifdef noreturn
#		define	NORETURN_FUNC_2	/* We don't want empty __attribute__ (()). */
#	else
#		define	NORETURN_FUNC_2	__attribute__ ((noreturn))
#	endif
#endif

#else	/* Not GCC.  */

/* No idempotent functions.  */
#define	CONSTVALUE_FUNC
#define	CONSTVALUE_FUNC_2

/* No functions-of-no-return.  */
#define	NORETURN_FUNC
#define	NORETURN_FUNC_2

#endif /* const & noreturn */

#if (__GNUC__ >= 3) || ((__GNUC__ == 2) && (__GNUC_MINOR__ >= 6))
#ifdef __cplusplus
#	define HAVE_BOOL		1
#endif //__cplusplus
#	define HAVE_OP_VEC_NEW	1
#endif

#if (!defined(__GNUC__) || __GNUC__ < 2 || ((__GNUC__ == 2) && (__GNUC_MINOR__ <= 7)) || ((__GNUC__ == 2) && (__GNUC_MINOR__ <= 90)) )
#	undef HAVE_EXPLICIT
#	undef HAVE_MUTABLE
#else
#	define HAVE_EXPLICIT	1
#	define HAVE_MUTABLE		1
#endif

#if defined(_AIX)
#   if (__GNUC__ == 2)
#	include <ifound/os/aix4.h>
#   else
#	include <ifound/os/aix5.h>
#   endif
#elif defined(sun)
#	if defined(__svr4__) && defined(__i386__)
#		include <ifound/os/sunsolx86.h>
#	elif defined(__svr4__)
#		include <ifound/os/sunsol.h>
#	else
#		include <ifound/os/sun4.h>
#	endif
#elif defined(__hppa) && defined(__hpux)
#	include <ifound/os/hpux11.h>
#elif !defined(hppa) && defined(hpux)
#	include <ifound/os/hpux11i.h>
#elif defined(linux)
#include <linux/version.h>
#ifndef LINUX_VERSION_CODE
#	include <linux/version-up.h>
#endif
#	if   ( __GNUC__ >= 8 )
#		include <ifound/os/linuxgcc8.h>
#	elif ( __GNUC__ >= 5 )
#		include <ifound/os/linuxgcc5.h>
#	elif ( __GNUC__ == 4 && LINUX_VERSION_CODE>=132624 )
#               include <ifound/os/linuxgcc4.h>
#	elif ( __GNUC__ == 3)
#		include <ifound/os/linuxgcc3.h>
#	elif ( __GNUC__ == 2 && __GNUC_MINOR__ >= 91 && LINUX_VERSION_CODE>=132096)
#		include <ifound/os/linux7.h>
#	elif ( __GNUC__ == 2 && __GNUC_MINOR__ == 91 && LINUX_VERSION_CODE>=131598) 
#               include <ifound/os/linux6.h>
#       elif ( __GNUC__ == 2 && __GNUC_MINOR__ == 90 )
#               include <ifound/os/linux5.h>
#       else
#               include <ifound/os/linux4.h>
#       endif
#elif defined(mips) && defined(sgi)
#	if defined(_IRIX4)
#		include <ifound/os/iris4.h>
#	else
#		include <ifound/os/iris5.h>
#	endif
#elif defined(AUX)
#	include <ifound/os/aux3.h>
#elif defined(M_UNIX)
#	if defined(_M_SYS5) && defined(_M_SYSV)
#	include <ifound/os/sco5.h>
#	else
#	include <ifound/os/sco.h>
#	endif
#elif defined(__NT__)
#	ifdef __SC__
#		ifdef __NTSERVER__
#			include <ifound/os/nt.h>
#			include <ifound/ntadic.h>
#		else
#			include <ifound/os/scwin.h>
#			include <ifound/ntadic.h>
#		endif
#	elif defined(__VC__)
#		include <ifound/os/vc.h>
#		include <ifound/ntadic.h>
#	endif
#elif defined(_WINDOWS)
#	ifdef __SC__
#		include <ifound/os/scwin.h>
#	elif defined(__VC__)
#		include <ifound/os/vcwin.h>
#		include <ifound/ntadic.h>
#	endif
#elif defined(MSDOS)
#	if __ZTC__
#		ifdef __SC__
#			include <ifound/os/sc.h>
#		else
#			include <ifound/os/ztc.h>
#		endif
#	else
#		include <ifound/os/borland.h>
#	endif
#elif defined(M_XENIX)
#	if defined(CPU_I286)
#		include <ifound/os/x286.h>
#	else
#		include <ifound/os/x386.h>
#	endif
#elif defined(__DGUX)
#	include	<ifound/os/dgux.h>
#elif defined(__SVR4__) || defined(__svr4__)
#	include	<ifound/os/uware.h>
#else
#	include <ifound/os/ix386.h>
#endif

/* This includes some default defines */
#include <ifound/os/defaults.h>

#endif
