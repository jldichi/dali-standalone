/********************************************************************
* Copyright (c) 1991 InterSoft Co.  All Rights Reserved
* THIS IS UNPUBLISHED PROPRIETARY SOURCE CODE OF InterSoft Co.
* The copyright notice above does not evidence any
* actual or intended publication of such source code.
*
* stdc.h,v 1.1.1.1 1996/04/16 18:51:31 gustavof Exp
*
* DESCRIPTION
* Include with standard 'C' compiler function prototypes
*********************************************************************/

#ifndef	STDC_H
#define	STDC_H

#include <ifound/osdep.h>
#include <ifound/functype.h>
#include <string.h>
#ifdef __cplusplus
#	define STRUCT class
#else
#	define STRUCT struct
#endif

#ifdef HAVE_UNDERSCORE_NAMES
/* C Run-Time library translations */
//Windows 2000
//#	define fileno		_fileno
//#	define stat		_stat
//Fin Windows 2000
#	define chdir 		_chdir
#	define creat 		_creat
#	define dup 		_dup
#	define dup2		_dup2
#	define execl		_execl
#	define execvp		_execvp
#	define ecvt 		_ecvt
#	define fcvt 		_fcvt
#	define getch 		_getch
#	define getcwd		_getcwd
#	define getpid		_getpid
#	define isatty 		_isatty
#	define kbhit 		_kbhit
#	define lseek 		_lseek
#	define ltoa 		_ltoa
#	define locking 		_locking
#	define mkdir 		_mkdir
#	define mktemp 		_mktemp
#	define pclose 		_pclose
#	define popen 		_popen
#	define putenv 		_putenv
#	define rmdir 		_rmdir
#	define sleep 		_sleep
#	define sopen 		_sopen
#	define strdup 		_strdup
#	define strcmpi 		_strcmpi
#	define sys_errlist	_sys_errlist
#	define unlink 		_unlink
#	define alloca		malloc
#ifdef _DLL
#	define sys_nerr		_sys_nerr_dll
#else
#	define sys_nerr		_sys_nerr
#endif
#endif

struct msqid_ds;
struct msgbuf
#ifdef Linux7
{
   long int mtype;
   char mtext[1];
}
#endif
;

struct group;
struct sembuf;
struct stat;
struct passwd;
struct sockaddr;
struct pollfd;
struct statfs;

#ifndef STDIN_FILENO
#	define	STDIN_FILENO	0
#endif
#ifndef STDOUT_FILENO
#	define STDOUT_FILENO	1
#endif
#ifndef STDERR_FILENO
#	define STDERR_FILENO	2
#endif

#ifdef HAVE_TEXT_FILES
#	define FOPEN_READ_MODE	"rb"
#	define FOPEN_WRITE_MODE	"wb"
#else
#	define FOPEN_READ_MODE	"r"
#	define FOPEN_WRITE_MODE	"w"
#endif

#ifdef HAVE_STD_TCP_HEADERS
struct hostent;
struct servent;
#endif /* HAVE_STD_TCP_HEADERS */

#ifdef HAVE_SYS_UTSNAME_H
struct utsname;
#endif /* HAVE_SYS_UTSNAME_H */

#ifdef __cplusplus
extern "C" {
#endif

/* +++ Prototypes +++ */

/* --- Prototypes --- */

#ifdef __cplusplus
}
#endif

#ifndef HAVE_PID
#ifdef __NT__
#	define getpid()	GetCurrentProcessId()
#else
#	define getpid()	(1)
#endif
#endif

#define mSet(a, b, c)				memset((a), (b), (c))
#if HAVE_BZERO
#	define mZero(a, b)				bzero((a), (int)(b))
#else
#	define mZero(a, b)				memset((a), 0, (int)(b))
#endif

#ifdef HAVE_BCOPY
#	define mCopy(a, b, c)		bcopy((b), (a), (int)(c))
#else
#if defined HAVE_MEMCPYBUG
#	define mCopy(a, b, c)		memCopy((a), (b), (int)(c))
#else
#	define mCopy(a, b, c)		memcpy((a), (b), (int)(c))
#endif
#endif

#ifdef HAVE_MEMMOVE
#	define mMove(a, b, c)		memmove((a), (b), (int)(c))
#else
#	define mMove(a, b, c)		memMove((a), (b), (int)(c))
#endif

#define SigSet(a, b)	Signal((a), (SigHandlerT)(b))

#ifdef __cplusplus
extern "C" {
#endif
	SigHandlerT Signal(int sig, SigHandlerT fp);
#ifdef __cplusplus
}
#endif

#if HAVE_BAD_CVT
#	ifdef __cplusplus
extern "C" {
#	endif
	extern FCVTRETTYPE fCvt(double, int, int *, int *);
	extern FCVTRETTYPE eCvt(double, int, int *, int *);
#	ifdef __cplusplus
}
#	endif
#else
	#define fCvt(a, b, c, d)	fcvt(a, b, c, d)
	#define eCvt(a, b, c, d)	ecvt(a, b, c, d)
#endif

#endif

