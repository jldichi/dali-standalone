/*********************************************************************
* Copyright (c) 1991 InterSoft Co.  All Rights Reserved
* THIS IS UNPUBLISHED PROPRIETARY SOURCE CODE OF InterSoft Co.
* The copyright notice above does not evidence any
* actual or intended publication of such source code.
*
* $Id: iris6.h,v 1.13 2008/06/02 19:29:39 spark Exp $
*
* DESCRIPTION
* Operating system dependent defines
*********************************************************************/
#ifndef IRIS5_H
#define IRIS5_H

#define _OS_	"IRIX"
#define _OSVER_	"6.4"
#define _OS_SHORT_NAME_	"iris5"

#if defined(ACADEMIC_VERSION)
#define EXTRA_BANNER_INFO   " (Academic-Beta) "
#elif defined (BETA_VERSION)
#   define  EXTRA_BANNER_INFO   " (Beta) " 
#else
#       define  EXTRA_BANNER_INFO   ""
#endif

/* Typedefs */
typedef unsigned int SizeT;
typedef long UidT;
typedef long GidT;
typedef long OffT;
typedef long PidT;
typedef int KeyT;
typedef unsigned long ModeT;
typedef union u_SemunT {
	int val;
	struct semid_ds *buf;
	unsigned short *array;
} SemunT;

/* Insert here locally needed defines */

/*
#ifndef _POSIX_SOURCE
#	define _POSIX_SOURCE			1
#endif
#ifndef _XOPEN_SOURCE
#	define _XOPEN_SOURCE			1
#endif
*/
#ifndef _XOPEN_SOURCE_EXTENDED
#	define _XOPEN_SOURCE_EXTENDED		1
#endif
#ifndef _ALL_SOURCE
#	define _ALL_SOURCE			1
#endif
#ifndef _ANSI_C_SOURCE
#	define _ANSI_C_SOURCE			1
#endif

/* Compiler dependant capabilities */
#undef HAVE_LONGLONGBUG
/* Visual C++ compatibility */
#define	DLLIMPORT
#define DLLDEPEND

/* Non compiler dependant capabilities */
#undef BAD_STRNCPY
#undef CANT_DEL_WHOLE_FILE
#undef CHKSUM
#undef DEBUG_LOCK
#define DIRENT					1
#undef END_OF_TEXT_FILE_CHAR
#undef ENOTDIR_IS_ENOENT
#undef EXEC_TAKES_TIME
#undef HAVE_64KLIMIT
#define HAVE_ABORT				1
#define HAVE_AF_UNIX			1 
#undef HAVE_AF_UNIX_LEN
#define HAVE_ALTZONE			1
#undef HAVE_SET_STD_HANDLE
#define HAVE_AR					1
#undef HAVE_AR_AIAMAG
#undef HAVE_AR_FL_HDR
#undef HAVE_AR_NAMLEN
#undef HAVE_AR_NUMERIC
#undef HAVE_ASYNC_IO
#define HAVE_BAD_CVT			1
#define HAVE_BCOPY				1
#define HAVE_BZERO				1
#undef HAVE_BSDTTY_H
#undef HAVE_CLOSEDIR_BUG
#undef HAVE_COMMAND_COM
#undef HAVE_EMULTIHOP
#undef HAVE_ENOLINK
#undef HAVE_ERRLIST	
#undef HAVE_ETC_SHADOW
#define HAVE_FCNTL				1
#undef HAVE_FDOPENBUG
#define HAVE_FIONREAD			1
#undef HAVE_FIORDCHK
#undef HAVE_FREEMEM
#define HAVE_FSYNC				1
#define HAVE_FULL_SIGNALS		1
#define HAVE_F_SETOWN			1
#undef HAVE_GETDFREE
#undef HAVE_GETGROUPS
#undef HAVE_GETPWANAM
#undef HAVE_GETSPNAM
#undef HAVE_HEAP_WALK
#define HAVE_INETD				1
#define HAVE_IPC				1
#define HAVE_JOB_CONTROL		1
#define	HAVE_LEX_DIFF_BEHAVIOR	1
#define HAVE_LOCALTIME			1
#define HAVE_LOCKF				1
#undef HAVE_LOCKING
#define HAVE_LONG_ARGV			1
#undef HAVE_LTCHARS
#define HAVE_MAIL				1
#undef HAVE_MMDF				
#define HAVE_MEMMOVE			1
#define HAVE_MKTEMP				1
#undef HAVE_OP_CONST_VOID_PTR
#define HAVE_O_SYNC				1
#define HAVE_OPENDIR			1
#undef HAVE_PASTE
#undef HAVE_PW_COMMENT
#define HAVE_PID				1
#define HAVE_PIPES				1
#define HAVE_PROC				1
#define HAVE_PROC_DIR			1
#undef HAVE_PSEUDO_CONCURR
#undef HAVE_RANLIB
#define HAVE_REAL_LOCKS			1
#undef HAVE_RENAME
#define	HAVE_S_ISSOCK_WORKING_OK	1
#undef HAVE_RESTARTABLE_SYSCALLS
#undef HAVE_SAVEMEM
#define HAVE_SECURITY			1
#define HAVE_SEMAPHORES			1
#define HAVE_SETRLIMIT			1
#define HAVE_SETSID				1
#define HAVE_SETUID				1
#define HAVE_SGR				1
#undef HAVE_SHARE_TERMINFO_DIR
#undef HAVE_SHORT_ENV
#undef HAVE_SHORT_FILE_NAMES
#undef HAVE_SHORT_MEMSET
#define HAVE_SIGABRT			1
#undef HAVE_SIGACTION
#undef HAVE_SIGPWR
#define HAVE_SIGSET				1
#undef HAVE_SIGVEC
#define HAVE_SIGEMT				1
#undef HAVE_SIGUNUSED
#define HAVE_SIGSYS				1
#undef HAVE_SOUND_BEEP
#undef HAVE_STKLEN
#define HAVE_STRSTR				1
#define HAVE_STRTOK				1
#undef HAVE_SWAP
#define HAVE_SYNC				1
#undef HAVE_SYSTTOLD_H
#undef HAVE_SYS_TIMEB_H
#undef HAVE_SYS_VFS_H
#undef HAVE_TEMPNAM_BUG
#define HAVE_TERMIO_H			1
#undef HAVE_TERMIOS_H
#undef HAVE_TEXT_FILES
#define HAVE_TIMEOFFSET			1
#define HAVE_TIMEZONE			1
#define HAVE_TIME_H				1
#define HAVE_TMPFILE			1
#define DEFAULT_TMPDIR			"/tmp"
#undef HAVE_TM_ZONE
#define HAVE_TRUNCATE			1
#define HAVE_TTY				1
#define HAVE_UMASK				1
#define HAVE_UNISTD_H			1
#undef HAVE_GETUSERPW
#define HAVE_USERS				1
#define HAVE_UTIME_H			1
#undef HAVE_USEUSEMUN
#define HAVE_WAITPID			1
#undef HAVE_XCC
#undef HAVE_ZPM
#undef LOG_DBG
#define NEED_AR_PORTAR			1
#undef NEED_CLOSE_FOR_SYNC
#undef NEED_MEM_DEBUG
#undef NEED_NETBIOS_FOPEN
#undef NO_ADVISORY_LOCK
#undef NO_EXTERN_CONST
#undef NO_MKRMDIR
#undef NO_SERVICES
#undef OPEN_CANT_CREATE
#undef SYSTEM_CANT_EXEC_AS_ROOT
#define SPANISH					1
#define STATFS_HAS_LEN			1
#define	HAVE_STATFS4PARAM			1
#undef SYSDIR
#undef SYSNDIR
#undef USE_FGETC
#undef USG
#define _G_USE_PROTOS			1
#define HAVE_SARWITHSWAP		1
#define HAVE_NOPROBLEMSCLOSING012	1
#define HAVE_SYSLOG				1
#undef HAVE_VARARGS_BUG
#undef HAVE_GRAPHICS
#define HAVE_MMF				1
#define HAVE_USEFULFTRUNCATE	1

/* Compiler dependant parameter types */

#undef HAVE_PRIV_DERIV_BUG
#define HAVE_STD_TCP_HEADERS	1
#define HAVE_SYS_UTSNAME_H		1
#undef HAVE_GENERIC_HPP
#undef HAVE_CTRLC
#undef HAVE_DISKFREESPACE
#undef HAVE_STRICT_CHECK
#undef HAVE_FIND
#undef HAVE_DOS_H
#undef DONT_ALLOW_REDEF
#undef HAVE_LOCK_CHAIN
#undef HAVE_CDINPATH
#undef TIME_FROM68
#undef _16BIT
#define _far
#undef HAVE_PRAGMA_ALIGN
#undef NEED_WCHAR_T
#define stddbg stderr
#undef NO_CONTROL_192_255
#undef IDEAWIN


/* Non compiler dependant parameter types */
#define ABORTRETTYPE			void
#define ALLOCRETTYPE			void *
#define BCOPYPARAM3TYPE			unsigned int
#define	BZEROPARAM2TYPE			int
#define BINDPARAM2TYPE			const void *
#define BINDPARAM3TYPE			int
#define	CFTIMEPARAM2TYPE		char *
#define CHOWNPARAM2TYPE 		UidT
#define CHOWNPARAM3TYPE 		GidT
#define	SOCKETRETTYPE			int
#define	CONNECTRETTYPE			int
#define	CONNECTPARAM1TYPE		int
#define CONNECTPARAM2TYPE		const void *
#define CONNECTPARAM3TYPE		int
#define EXECVPPARAM1TYPE		char const * 
#define EXECVPPARAM2TYPE 		char * const *
#define FCNTLPARAM3TYPE			...
#define FTELLRETTYPE			long
#define FTELLPARAM1TYPE			FILE *
#define FTRUNCATEPARAM2TYPE		OffT
#define FDOPENPARAM2TYPE		const char *
#define FPUTCPARAM1TYPE			int
#define FREADPARAM1TYPE			void *
#define FWRITEPARAM1TYPE		const void *
#define GCVTPARAM2TYPE          int    
#define GETCWDPARAM1TYPE		char *
#define GETCWDPARAM2TYPE		unsigned int
#define GETGRGIDPARAMTYPE		GidT
#define GETHOSTBNPARAMTYPE		const char *
#define GETHOSTNPARAM2TYPE		int
#define GETSERVBNPARAM1TYPE		char const *
#define GETSERVBNPARAM2TYPE		char const *
#define GETPWUIDPARAMTYPE 		UidT
#define IOCTLPARAM2TYPE			int
#define LISTENPARAM2TYPE		int
#define MKDIRPARAM1TYPE			char const *
#define MSGSNDPARAM2TYPE		const void *
#define MSGCTLPARAM3TYPE		...
#define MSGRCVPARAM2TYPE		void *
#define MSGRCVPARAM3TYPE		SizeT
#define MSGSNDPARAM2TYPE 		const void *
#define MSGSNDPARAM3TYPE		SizeT
#define MUNMAPPARAM1TYPE		char *
#define MUNMAPPARAM2TYPE		SizeT
#define OPENPARAM3TYPE			...
#define OPENRETTYPE				int
#define PAUSERETTYPE			int
#define POPENPARAMTYPE			char const *
#define PUTENVPARAMTYPE			char *
#define QSORTPARAM2TYPE			unsigned
#define QSORTPARAM3TYPE			unsigned
#define QSORTRETTYPE			void
#define RCMDPARAM3TYPE			char *
#define RCMDPARAM4TYPE			char *
#define RCMDPARAM5TYPE			char *
#define READPARAM3TYPE			unsigned
#define READRETTYPE				signed int
#define REALPATHPARAM1TYPE		const char *
#define RETSIGTYPE				void
#define SBRKPARAMTYPE			int
#define SBRKRETTYPE				void *
/* #define SEMCTLPARAM4TYPE		... */
#define SEMOPPARAM3TYPE			unsigned
#define SLEEPPARAMTYPE			unsigned
#define SLEEPRETTYPE			unsigned
#define SPRINTFRETTYPE          int
#define SRANDRETTYPE			void
#define	STRDUPPARAMTYPE			const char *
#define STATFSPARAM1TYPE		const char *
#define WRITEPARAM3TYPE			unsigned
#define WRITERETTYPE			signed int
#define _FLSBUFPARAM1TYPE		unsigned
#define POLLPARAM1TYPE 			struct pollfd *
#define POLLPARAM2TYPE			unsigned long
#define POLLPARAM3TYPE			int
#define FCVTPARAM2TYPE			int
#define ECVTPARAM2TYPE			int
#define FCVTRETTYPE 			char *
#define ECVTRETTYPE 			char *
#define BITFIELDSTYPE			unsigned long
#define TIMEPARAM1TYPE			long *
#define TIMERETTYPE			long
#define TEMPNAMPARAM1TYPE		char const *
#define TEMPNAMPARAM2TYPE		char const *
#define ACCEPTPARAM2TYPE		void *
#define ACCEPTPARAM3TYPE		int *k
#define WINDOWTYPE				int
#define EXITRETTYPE				void
#define SYNCRETTYPE				void
#define EXITRETTYPE				void
#define GETPEERNAMEPARAM2TYPE	void *
#define GETPEERNAMEPARAM3TYPE	int *

/* Miscelaneous */
#define PSUID_CMD				"ps -lp%ld"
#define PSUID_POS				2
#define MAX_WINDOWS				512
#define	STDERR					stderr
#define FM_MAX_OPEN				128
#define AR_MOD_SEP				'/'
#define DEFAULT_PATH			":/bin:/usr/ideafix/bin"
#define DEFAULT_SHELL			"/bin/sh"
#define MAIL_HOME				"/usr/mail"
#define NEW_LINE				"\n"
#define PATH_SEP				":"
#define PATH_SLASH				"/"
#define PATH_SLASH_UNIV			"/"
#define VEC_DEL_SIZE(a)
#undef VOLUME_LETTER_SEP
#define quote(a)				#a
typedef RETSIGTYPE (*SigHandlerT)(int);

/* Cfix stuff */
#define CFIX_HAVE_SHLIB		1
#define CFIX_HAVE_ESS_SHLIB	1
#undef CFIX_HAVE_OVERLAYS
#undef CFIX_NEED_EXE_EXT
#undef CFIX_NEED_MINUSYP
#undef CFIX_NEED_QPATH
#define CFIX_C_EXT			".c"
#define CFIX_EXE_EXT		".exe"
#define CFIX_OBJ_EXT		".o"
#define CFIX_LIB_EXT		".a"
#define CFIX_COMPILER		"cc -c "
#define CFIX_OPTIM			" -O "
#define CFIX_DEBUG			" -g "
#define CFIX_LINK			"cc "
#define CFIX_LIBDIRSFLAG	" -L"
#define CFIX_LIBDIR			"/lib "
#define CFIX_SHLIBDIR		" -L/SH521"
#define CFIX_LINKSTRIP		" -s "
#define CFIX_LINKDEBUG		" -g "
#define CFIX_OUTFILE		"-o "
#define CFIX_IXDBLIB		" -lix -les -lsql "
#define CFIX_ESDBLIB		" -lix -les -lsql "
#define CFIX_SQLFWDBLIB		" -lix -les -lsql "
#define CFIX_SCKTLIB		""
#define CFIX_IDEALIB		" -lidea "
#define CFIX_IXDBSHLIB		" -lix521_s -les521_s -lsql521_s "
#define CFIX_ESDBSHLIB		" -lix521_s -les521_s -lsql521_s " 
#define CFIX_SQLFWSHLIB		" -lix521_s -les521_s -lsql521_s "
#define CFIX_SCKTSHLIB		""
#define CFIX_IDEASHLIB		" -lidea521_s -lc_s "
#define CFIX_MAX_MODS		100
#define CFIX_INCLUDE 		"/include"

/* because of COD_ZERO, invented by Eugen, we must define it somewere*/
#define	COD_ZERO	0x1f

#endif
