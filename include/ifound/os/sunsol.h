/***********************************************************************
* Copyright (c) 1991 InterSoft Co.  All Rights Reserved
* THIS IS UNPUBLISHED PROPRIETARY SOURCE CODE OF InterSoft Co.
* The copyright notice above does not evidence any
* actual or intended publication of such source code.
*
* $Id: sunsol.h,v 1.36 2010/09/28 19:45:38 hardaiz Exp $
*
* DESCRIPTION
*              O.S: Solaris 2.4 for Sun.
*              (This header father is uware.h)
*********************************************************************/
/*
	Comments on the port
*/

#ifndef SUNSOL_H
#define SUNSOL_H

#ifndef SunSol
#define SunSol	1
#endif

// Define necesario para mantener compatibilidad con Red Hat 7.1
#define __THROW

#define _OS_	"Sun Solaris"
#define _OSVER_	"SunOS 5.10"
#define _OS_SHORT_NAME_	"sunsol"

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
/*#define	_STYPES*/
#undef _POSIX_SOURCE

#ifdef __cplusplus
#undef _XOPEN_SOURCE
#else
#define _XOPEN_SOURCE	1
#endif

/*
#define _POSIX_VERSION			1
definido en /usr/include/sys/unistd.h
*/

/* Compiler dependant capabilities */
#undef HAVE_LONGLONGBUG
/* Visual C++ compatibility */
#define	DLLIMPORT
#define DLLDEPEND
#define DLLEXPORT

#include <string.h>
#include <strings.h>

#define memCopy memcpy
#define memMove memmove

/* Non compiler dependant capabilities */
#undef BAD_STRNCPY
#undef CANT_DEL_WHOLE_FILE
#undef CHKSUM
#undef DEBUG_LOCK
#define DIRENT 					1
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
#undef HAVE_BAD_CVT
#undef HAVE_BCOPY
#undef HAVE_BSDTTY_H
#undef HAVE_BZERO
#undef HAVE_CLOSEDIR_BUG
#undef HAVE_COMMAND_COM
#define HAVE_EMULTIHOP			1
#define HAVE_ENOLINK			1
#undef HAVE_ERRLIST
#define HAVE_ETC_SHADOW			1
#define HAVE_FCNTL				1
#undef HAVE_FDOPENBUG
#define HAVE_FIONREAD			1
#undef HAVE_FIORDCHK
#undef HAVE_FREEMEM
#define HAVE_FSYNC				1
#define HAVE_FULL_SIGNALS		1
#define HAVE_F_SETOWN			1
#undef HAVE_GETDFREE
#define HAVE_GETGROUPS			1
#undef HAVE_GETPWANAM
#define HAVE_GETSPNAM			1
#define HAVE_GETHOSTID			1
#undef HAVE_HEAP_WALK
#define HAVE_INETD				1
#define HAVE_IPC				1
#define HAVE_JOB_CONTROL		1
#define	HAVE_LEX_DIFF_BEHAVIOR	1
#define HAVE_LOCALTIME			1
#define HAVE_LOCKF				1
#undef HAVE_LOCKING
#define HAVE_LONG_ARGV			1
#define HAVE_LTCHARS			1
#define HAVE_MAIL				1
#undef HAVE_MMDF			   
#undef HAVE_MEMMOVE
#define HAVE_MEMCPYBUG				1
#define HAVE_MKTEMP				1
#undef HAVE_OP_CONST_VOID_PTR
#define HAVE_O_SYNC				1
#define HAVE_OPENDIR			1
#undef HAVE_PASTE
#define HAVE_PW_COMMENT			1
#define HAVE_PID				1
#define HAVE_PIPES				1
#define HAVE_PROC				1
#define HAVE_PROC_DIR			1
#undef HAVE_PSEUDO_CONCURR
#undef HAVE_RANLIB
#define HAVE_REAL_LOCKS			1
#undef HAVE_RENAME
#undef	HAVE_S_ISSOCK_WORKING_OK
#undef HAVE_RESTARTABLE_SYSCALLS
#undef HAVE_SAVEMEM
#define HAVE_SECURITY			1
#define HAVE_SEMAPHORES			1
#undef HAVE_SETRLIMIT
#define HAVE_SETSID				1
#define HAVE_SETUID				1
#define HAVE_SGR				1
#define HAVE_SHARE_LIB_TERMINFO_DIR	1
#undef HAVE_SHORT_ENV
#undef HAVE_SHORT_FIlE_NAMES
#undef HAVE_SHORT_MEMSET
#define HAVE_SIGABRT			1
#undef HAVE_SIGACTION
#define HAVE_SIGPWR				1
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
#define HAVE_SYSLOG				1
#undef HAVE_VARARGS_BUG
#undef HAVE_GRAPHICS
#define HAVE_MMF				1
#define HAVE_USEFULFTRUNCATE	1

#define HAVE_SYNC				1
#define HAVE_SYSTTOLD_H			1
#undef HAVE_SYS_TIMEB_H
#undef HAVE_SYS_VFS_H
#undef HAVE_TEMPNAM_BUG
#define HAVE_TERMIO_H			1
#undef HAVE_TERMIOS_H
#undef HAVE_TEXT_FILES
#undef HAVE_TIMEOFFSET
#define HAVE_TIMEZONE			1
#define HAVE_TIME_H				1
#define HAVE_TMPFILE			1
#define DEFAULT_TMPDIR			"/tmp"
#undef HAVE_TM_ZONE
#undef HAVE_TRUNCATE
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
#define SYSDIR					1
#undef SYSNDIR
#undef USE_FGETC
#undef USG
#define _G_USE_PROTOS			1
#define HAVE_SARWITHSWAP		1
#undef HAVE_NOPROBLEMSCLOSING012

/* Compiler dependant parameter types */

#define	HAVE_STATFS4PARAM	1
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
#undef HAVE_PRAGMA_ALIGN
#define _far
#define stddbg stderr
#undef NEED_WCHAR_T
#undef NO_CONTROL_192_255
#undef _16BIT
#undef IDEAWIN

/* Non compiler dependant parameter types */
#define ABORTRETTYPE 			void
#define ALLOCRETTYPE			void *
#define BCOPYPARAM3TYPE			unsigned int
#define BINDPARAM2TYPE			const struct sockaddr *	
#define BINDPARAM3TYPE			unsigned int
#define BZEROPARAM2TYPE			unsigned int
#define CHOWNPARAM2TYPE 		UidT
#define CHOWNPARAM3TYPE 		GidT
#define CFTIMEPARAM2TYPE		char *
#define CONNECTPARAM1TYPE		int
#define CONNECTPARAM2TYPE		const struct sockaddr *
#define CONNECTPARAM3TYPE		unsigned int
#define ECVTRETTYPE                   char *
#define ECVTPARAM2TYPE                int
#define EXECVPPARAM1TYPE		const char * 
#define EXECVPPARAM2TYPE		char * const *
#define FCNTLPARAM3TYPE			...
#define FTELLRETTYPE			long
#define FTELLPARAM1TYPE			FILE *
#define FPUTCPARAM1TYPE			int
#define FREADPARAM1TYPE			void *
#define FTRUNCATEPARAM2TYPE		OffT
#define FWRITEPARAM1TYPE		void const *
#define GCVTPARAM2TYPE		        int    
#define GETCWDPARAM1TYPE		char *
#define GETCWDPARAM2TYPE 		SizeT
#define GETGRGIDPARAMTYPE		GidT
#define GETHOSTBNPARAMTYPE		const char *
#define GETHOSTNPARAM2TYPE		int
#define GETPWUIDPARAMTYPE		UidT
#define GETPEERNAMEPARAM2TYPE		struct sockaddr *
#define GETPEERNAMEPARAM3TYPE		socklen_t *
#define GETSERVBNPARAM1TYPE		const char *
#define GETSERVBNPARAM2TYPE		const char *
#define IOCTLPARAM2TYPE			int
#define LISTENPARAM2TYPE		int
#define LDIVRETTYPE				ldiv_t
#define MKDIRPARAM1TYPE			const char *
#define MSGCTLPARAM3TYPE		struct msqid_ds*
#define MSGRCVPARAM2TYPE		void *
#define MSGRCVPARAM3TYPE		SizeT
#define MSGSNDPARAM2TYPE		const void *
#define MSGSNDPARAM3TYPE		SizeT
#define MUNMAPPARAM1TYPE		char *
#define MUNMAPPARAM2TYPE		SizeT
#define OPENPARAM3TYPE			...
#define OPENRETTYPE			int
#define PAUSERETTYPE 			int
#define POPENPARAMTYPE			const char *
#define PUTENVPARAMTYPE			char *
#define QSORTPARAM2TYPE			unsigned
#define QSORTPARAM3TYPE			unsigned
#define QSORTRETTYPE			void
#define RCMDPARAM3TYPE			const char *
#define RCMDPARAM4TYPE			const char *
#define RCMDPARAM5TYPE			const char *
#define REALPATHPARAM1TYPE		const char *
#define READPARAM3TYPE			unsigned
#define READRETTYPE				int
#define RETSIGTYPE				void
#define SBRKPARAMTYPE			int
#define SBRKRETTYPE 			void *
/* #define SEMCTLPARAM4TYPE		...  */
#define SEMOPPARAM3TYPE			unsigned
#define SLEEPPARAMTYPE			unsigned
#define SLEEPRETTYPE			unsigned
#define SPRINTFRETTYPE			int
#define SRANDRETTYPE			void
#define STRDUPPARAMTYPE			const char *
#define	STATFSPARAM1TYPE		const char *
#define TIMEPARAM1TYPE			long *
#define TIMERETTYPE			long
#define TEMPNAMPARAM1TYPE		const char *
#define TEMPNAMPARAM2TYPE		const char *
#define WRITEPARAM3TYPE			unsigned
#define WRITERETTYPE			int
#define _FLSBUFPARAM1TYPE		unsigned int
#define POLLPARAM1TYPE 			struct pollfd *
#define POLLPARAM2TYPE			unsigned long
#define POLLPARAM3TYPE			int
#define FCVTRETTYPE 			char *
#define FCVTPARAM2TYPE			int
#define BITFIELDSTYPE			unsigned long
#define FDOPENPARAM2TYPE		char const *
#define CONNECTRETTYPE			int
#define SOCKETRETTYPE			int
#define EXITRETTYPE				void
#define SYNCRETTYPE				void
#define WINDOWTYPE				int
#define ACCEPTPARAM2TYPE		struct sockaddr *
#define ACCEPTPARAM3TYPE		void *
#define CREATPARAM2TYPE			long unsigned int

/* Miscelaneous */
#define PSUID_CMD				"ps -lp%ld"
#define PSUID_POS				2
#define MAX_WINDOWS				512
#define FM_MAX_OPEN				128
#define AR_MOD_SEP				'/'
#define	STDERR					stderr
#define DEFAULT_PATH			":/bin:/usr/ideafix/bin"
#define DEFAULT_SHELL			"/bin/sh"
#define MAIL_HOME				"/var/mail"
#define NEW_LINE				"\n"
#define PATH_SEP				":"
#define PATH_SLASH				"/"
#define PATH_SLASH_UNIV			"/"
#define VEC_DEL_SIZE(a)
#undef VOLUME_LETTER_SEP
#define quote(a)				#a
#define COD_ZERO 				0x80

#ifdef __cplusplus
extern "C" {
#endif

typedef RETSIGTYPE (*SigHandlerT)(int);

#ifdef __cplusplus
}
#endif

/* Cfix stuff */
#define CFIX_HAVE_SHLIB		1
#define CFIX_HAVE_ESS_SHLIB	1
#undef CFIX_HAVE_OVERLAYS
#undef CFIX_NEED_EXE_EXT
#undef CFIX_NEED_MINUSYP
#undef CFIX_NEED_QPATH
#define CFIX_C_EXT		".c"
#define CFIX_EXE_EXT		".exe"
#define CFIX_OBJ_EXT		".o"
#define CFIX_LIB_EXT		".a"
#define CFIX_COMPILER		"gcc -c "
#define CFIX_OPTIM		" -O "
#define CFIX_DEBUG		" -g "
#define CFIX_INAv1              " -DIXSCA "
#define CFIX_LINK		"g++ "
#define CFIX_LINK_ORA		"g++ "
#define CFIX_LINK_IFX		"esql "
#define CFIX_LIBDIRSFLAG	" -L"
#define CFIX_LIBDIR		"/lib "
#define CFIX_SHLIBDIR		"/SH521 "
#define CFIX_LINKSTRIP		" -s "
#define CFIX_LINKDEBUG		" -g "
#define CFIX_OUTFILE		"-o "
#define CFIX_IXDBLIB		" -ldb -lix -les -lsql -lsqlimpl -lidea -liext -ldb -lix -les -lsql -liqlsrv -lsqlimpl -lidea -liext "
#define CFIX_ESDBLIB		" -ldb -lix -les -lsql -lsqlimpl -lidea -liext -ldb -lix -les -lsql -liqlsrv -lsqlimpl -lidea -liext "
#define CFIX_SQLFWDBLIB		" -ldb -lix -les -lsql -lsqlimpl -lidea -liext -ldb -lix -les -lsql -liqlsrv -lsqlimpl -lidea -liext "
#define CFIX_SCKTLIB		""
#define CFIX_IDEALIB		" -lidea -liext -lidea -liext -lstdc++ -lsocket -lnsl "
#define CFIX_IXDBSHLIB		" -L/SH521 -ldb521_s -lix521_s -les521_s -lsql521_s -liqlsrv521_s -lsqlimpl521_s -lidea521_s -liext521_s "
#define CFIX_ESDBSHLIB		" -L/SH521 -ldb521_s -lix521_s -les521_s -lsql521_s -liqlsrv521_s -lsqlimpl521_s -lidea521_s -liext521_s "
#define CFIX_SQLFWSHLIB		" -L/SH521 -ldb521_s -lix521_s -les521_s -lsql521_s -liqlsrv521_s -lsqlimpl521_s -lidea521_s -liext521_s "

//Defines para INA x Telnet
#define CFIX_IXDBLIB_INA        " -lixsca -ldb -lix -les -lsql -lsqlimpl -ldb -lix -les -lsql -lsqlimpl -ldb -lpq "
#define CFIX_ESDBLIB_INA        " -lixsca -ldb -lix -les -lsql -lsqlimpl -ldb -lix -les -lsql -lsqlimpl -ldb -lpq "
#define CFIX_SQLFWDBLIB_INA     " -lixsca -ldb -lix -les -lsql -lsqlimpl -ldb -lix -les -lsql -lsqlimpl -ldb -lpq "
#define CFIX_IXDBSHLIB_INA      " -ldb521_s -lix521_s -les521_s -lsql521_s -lsqlimpl521_s -liqlsrv521_s -lixsca521_s -lpq "
#define CFIX_ESDBSHLIB_INA      " -ldb521_s -lix521_s -les521_s -lsql521_s -lsqlimpl521_s -liqlsrv521_s -lixsca521_s -lpq "
#define CFIX_SQLFWSHLIB_INA     " -ldb521_s -lix521_s -les521_s -lsql521_s -lsqlimpl521_s -liqlsrv521_s -lixsca521_s -lpq "

#define CFIX_SCKTSHLIB		""       
#define CFIX_IDEASHLIB		" -lidea521_s -liext521_s -lstdc++ -lsocket -lnsl"
#define CFIX_MAX_MODS		100
#define CFIX_INCLUDE 		"/include"
#endif
