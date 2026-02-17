/***********************************************************************
* Copyright (c) 1991 InterSoft Co.  All Rights Reserved
* THIS IS UNPUBLISHED PROPRIETARY SOURCE CODE OF InterSoft Co.
* The copyright notice above does not evidence any
* actual or intended publication of such source code.
*
* $$
*
* DESCRIPTION
* Operating system dependent defines
*********************************************************************/

/*
	Comments on the port

	Variable arguments do not work correctly in rp/rpdorep.c with
	the following function:
	
		static int rpprintf(struct s_rep *p, char *fmt, ...);

	it works correctly if the static keyword is removed.
*/

#ifndef HPUX11_H
#define HPUX11_H

#define __THROW

#define _OS_	"HP-UX"
#define _OSVER_	"B.11.23 U"
#define _OS_SHORT_NAME_	"hpux11"
#define hpux
#define hppa

#include <strings.h>
#include <string.h>


#if defined(ACADEMIC_VERSION)
#define EXTRA_BANNER_INFO   " (Academic-Beta) "
#elif defined (BETA_VERSION)
#   define  EXTRA_BANNER_INFO   " (Beta) " 
#else
#       define  EXTRA_BANNER_INFO   ""
#endif


/* Typedefs */
typedef size_t SizeT;
typedef int UidT;
typedef int GidT;
typedef int OffT;
typedef int PidT;
typedef int KeyT;
typedef unsigned short ModeT;
typedef union u_SemunT {
	int val;
	struct semid_ds *buf;
	unsigned short *array;
} SemunT;

/* Insert here locally needed defines */

#ifndef _INCLUDE_HPUX_SOURCE
#define _INCLUDE_HPUX_SOURCE
#endif
#ifndef _INCLUDE_XOPEN_SOURCE
#define _INCLUDE_XOPEN_SOURCE
#endif
#ifndef _INCLUDE_POSIX_SOURCE
#define _INCLUDE_POSIX_SOURCE
#endif

/* Compiler dependant capabilities */
#undef HAVE_LONGLONGBUG
/* Visual C++ compatibility */
#define	DLLIMPORT
#define	DLLEXPORT
#define DLLDEPEND

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
#define	HAVE_AF_UNIX			1
#undef HAVE_AF_UNIX_LEN
#undef HAVE_ALTZONE
#undef HAVE_SET_STD_HANDLE
#define HAVE_AR					1
#undef HAVE_AR_AIAMAG
#undef HAVE_AR_FL_HDR
#undef HAVE_AR_NAMLEN
#undef HAVE_AR_NUMERIC
#undef HAVE_ASYNC_IO
#undef HAVE_BAD_CVT
#define HAVE_BCOPY				1
#define HAVE_BSDTTY_H			1
#define HAVE_BZERO				1
#undef HAVE_CLOSEDIR_BUG
#undef HAVE_COMMAND_COM
#define HAVE_EMULTIHOP			1
#define HAVE_ENOLINK			1
#define HAVE_ERRLIST			1
#undef HAVE_ETC_SHADOW
#define HAVE_FCNTL				1
#undef HAVE_FDOPENBUG
#define HAVE_FIONREAD			1
#undef HAVE_FIORDCHK
#define	HAVE_FLOATS_BUG	1
#undef HAVE_FREEMEM
#define	HAVE_FSYNC				1
#define HAVE_FULL_SIGNALS		1
#undef HAVE_F_SETOWN
#undef HAVE_GETDFREE
#undef HAVE_GETGROUPS
#undef HAVE_GETPWANAM
#undef HAVE_GETSPNAM
#undef HAVE_HEAP_WALK
#define HAVE_INETD				1
#define HAVE_IPC				1
#define HAVE_JOB_CONTROL		1
#define	HAVE_LEX_DIFF_BEHAVIOR		1
#define HAVE_LOCALTIME			1
#define HAVE_LOCKF				1
#undef HAVE_LOCKING
#define HAVE_LONG_ARGV			1
#define HAVE_LTCHARS			1
#define HAVE_LTOA1PARAM			1
#define HAVE_MAIL				1
#undef HAVE_MEMMOVE
#define HAVE_MEMMOVE			1
#define memMove	memmove
#define HAVE_MKTEMP				1
#undef HAVE_OP_CONST_VOID_PTR
#define HAVE_O_SYNC				1
#define HAVE_OPENDIR			1
#undef HAVE_PASTE
#undef HAVE_PW_COMMENT
#define HAVE_PID				1
#define HAVE_PIPES				1
#define HAVE_PROC				1
#undef HAVE_PROC_DIR
#undef HAVE_PSEUDO_CONCURR
#undef HAVE_RANLIB
#define HAVE_REAL_LOCKS			1
#undef HAVE_RENAME
#define HAVE_S_ISSOCK_WORKING_OK       1
#undef HAVE_RESTARTABLE_SYSCALLS
#undef HAVE_SAVEMEM
#define HAVE_SECURITY			1
#define HAVE_SEMAPHORES			1
#undef HAVE_SETRLIMIT
#undef HAVE_SETSID
#define HAVE_SETUID				1
#define HAVE_SGR				1
#undef HAVE_SHARE_TERMINFO_DIR
#undef HAVE_SHORT_ENV
#undef HAVE_SHORT_FILE_NAMES
#undef HAVE_SHORT_MEMSET
#define HAVE_SIGABRT			1
#undef HAVE_SIGACTION
#undef HAVE_SIGPWR
#undef HAVE_SIGSET
#undef HAVE_SIGVEC
#define HAVE_SIGEMT				1
#undef HAVE_SIGUNUSED
#define HAVE_SIGSYS				1
#undef HAVE_SOUND_BEEP
#undef HAVE_STATFS4PARAM
#undef HAVE_STKLEN
#undef HAVE_STRSTR
#define HAVE_STRTOK				1
#undef HAVE_SWAP
#define HAVE_SYNC				1
#undef HAVE_SYSTTOLD_H
#undef HAVE_SYS_TIMEB_H
#define HAVE_SYS_VFS_H			1
#define HAVE_TERMIO_H			1
#undef HAVE_TERMIOS_H
#undef HAVE_TEXT_FILES
#undef HAVE_TGETENT_DIFFERENT_RETURN_VALUE 
#undef HAVE_TIMEOFFSET
#define HAVE_TIMEZONE			1
#define HAVE_TIME_H				1
#define HAVE_TMPFILE			1
#define DEFAULT_TMPDIR			"/tmp"
#define HAVE_TM_ZONE			1
#undef HAVE_TRUNCATE
#define HAVE_TTY				1
#define HAVE_UMASK				1
#define HAVE_UNISTD_H			1
#undef HAVE_GETUSERPW
#define HAVE_USERS				1
#define HAVE_UTIME_H			1
#undef HAVE_USEUSEMUN
#undef HAVE_WAITPID
#undef HAVE_XCC
#undef HAVE_ZPM
#undef LOG_DBG
#define NEED_AR_PORTAR			1
#undef NEED_CLOSE_FOR_SYNC
#undef NEED_MEM_DEBUG
#undef NEED_NETBIOS_FOPEN
#undef NO_ADVISORY_LOCK
#define NO_EXTERN_CONST			1
#undef NO_MKRMDIR
#undef NO_SERVICES
#undef OPEN_CANT_CREATE
#define SPANISH					1
#undef STATFS_HAS_LEN
#define SYSDIR					1
#define SYSNDIR					1
#undef USE_FGETC
#undef USG
#define _G_USE_PROTOS			1
#undef HAVE_SARWITHSWAP
#define HAVE_NOPROBLEMSCLOSING012	1
#define HAVE_SYSLOG				1
#define HAVE_SYSLOG_HP			1
#undef HAVE_VARARGS_BUG
#undef HAVE_GRAPHICS
#undef HAVE_MMF
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
#undef HAVE_PRAGMA_ALIGN
#define _far
#define stddbg stderr                     
#undef NEED_WCHAR_T
#undef NO_CONTROL_192_255
#undef _16BIT
#undef IDEAWIN

/* Non compiler dependant parameter types */
#define ABORTRETTYPE			void
#define ALLOCRETTYPE			void *
#define BCOPYPARAM3TYPE			size_t  //long unsigned int
#define	BZEROPARAM2TYPE			size_t  //int
#define BINDPARAM2TYPE			const void *
#define BINDPARAM3TYPE			int
#define	CFTIMEPARAM2TYPE		char *
#define CHOWNPARAM2TYPE 		UidT
#define CHOWNPARAM3TYPE 		GidT
#define CONNECTPARAM1TYPE		int
#define CONNECTPARAM2TYPE		const void *
#define CONNECTPARAM3TYPE		int
#define CREATPARAM2TYPE			short unsigned int
#define EXECVPPARAM1TYPE		char const * 
#define EXECVPPARAM2TYPE		char * const *
#define FCNTLPARAM3TYPE			...
#define FTELLRETTYPE			long
#define FTELLPARAM1TYPE			FILE *
#define FPUTCPARAM1TYPE			int
#define FREADPARAM1TYPE			void *
#define FTRUNCATEPARAM2TYPE		OffT
#define FWRITEPARAM1TYPE		void const *
#define GCVTPARAM2TYPE			int
#define GETCWDPARAM1TYPE		char *
#define GETCWDPARAM2TYPE 		SizeT
#define GETGRGIDPARAMTYPE		GidT
#define GETHOSTBNPARAMTYPE		const char *
#define GETHOSTNPARAM2TYPE		unsigned int
#define GETPEERNAMEPARAM2TYPE	void *
#define GETPEERNAMEPARAM3TYPE	socklen_t *
#define GETPWUIDPARAMTYPE		UidT
#define GETSERVBNPARAM1TYPE		const char *
#define GETSERVBNPARAM2TYPE		const char *
#define IOCTLPARAM2TYPE			int
#define LSEEKRETTYPE				long int
#define LSEEKPARAM2TYPE			long int
#define LOCKFPARAM3TYPE			long int
#define MKDIRPARAM1TYPE			char const *
#define MSGCTLPARAM3TYPE		struct msqid_ds *
#define MSGRCVPARAM2TYPE		void *
#define MSGRCVPARAM3TYPE		unsigned int
#define MSGSNDPARAM2TYPE		const void *
#define MSGSNDPARAM3TYPE		unsigned int
#define MUNMAPPARAM1TYPE		char *
#define MUNMAPPARAM2TYPE		size_t
#define OPENPARAM3TYPE			...
#define OPENRETTYPE				int
#define PAUSERETTYPE			int
#define POPENPARAMTYPE			char const *
#define PUTENVPARAMTYPE			char const *
#define QSORTPARAM2TYPE			unsigned
#define QSORTPARAM3TYPE			unsigned
#define QSORTRETTYPE			void
#define RCMDPARAM2TYPE			int
#define RCMDPARAM3TYPE			const char *
#define RCMDPARAM4TYPE			const char *
#define RCMDPARAM5TYPE			const char *
#define READPARAM3TYPE			unsigned
#define READRETTYPE				long int
#define REALPATHPARAM1TYPE		const char *
#define RETSIGTYPE				void
#define SBRKPARAMTYPE			int
#define SBRKRETTYPE				void *
/* #define SEMCTLPARAM4TYPE		... */
#define SEMOPPARAM3TYPE			unsigned
#define SLEEPPARAMTYPE			unsigned
#define SLEEPRETTYPE			unsigned
#define SPRINTFRETTYPE			int
#define SRANDRETTYPE			void
#define	STRDUPPARAMTYPE			const char *
#define STATFSPARAM1TYPE		const char *
#define SYSLOGRETTYPE			int
#define TIMEPARAM1TYPE			long int *
#define TIMERETTYPE				long int
#define TEMPNAMPARAM1TYPE		const char *
#define TEMPNAMPARAM2TYPE		const char *
#define WRITEPARAM3TYPE			unsigned
#define WRITERETTYPE				long int
#define _FLSBUFPARAM1TYPE		unsigned char
#define POLLPARAM1TYPE 			struct pollfd *
#define POLLPARAM2TYPE			unsigned long
#define POLLPARAM3TYPE			int
#define ECVTRETTYPE 			char *
#define ECVTPARAM2TYPE			int
#define FCVTRETTYPE 			char *
#define FCVTPARAM2TYPE			int
#define BITFIELDSTYPE			unsigned long
#define FDOPENPARAM2TYPE		char const *
#define CONNECTRETTYPE			int
#define SOCKETRETTYPE			int
#define EXITRETTYPE				void
#define SYNCRETTYPE				void
#define WINDOWTYPE				int
#define ACCEPTPARAM2TYPE		void *
#define ACCEPTPARAM3TYPE		socklen_t *

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
#define CFIX_C_EXT			".c"
#define CFIX_EXE_EXT		".exe"
#define CFIX_OBJ_EXT		".o"
#define CFIX_LIB_EXT		".a"
#define CFIX_COMPILER		"gcc -c "
#define CFIX_OPTIM			" -O0 "
#define CFIX_DEBUG			" -g "
#define CFIX_LINK			"g++ "
#define CFIX_LINK_ORA		"g++ "
#define CFIX_LINK_IFX		"esql "
#define CFIX_LIBDIRSFLAG	" -L"
#define CFIX_LIBDIR		"/lib "
#define CFIX_SHLIBDIR		"/SH521 "
#define CFIX_LINKSTRIP		" -s "
#define CFIX_LINKDEBUG		" -g "
#define CFIX_OUTFILE		"-o "
#define CFIX_IXDBLIB		" -ldb -lix -les -lsql -lsqlimpl -liqlsrv -lidea -liext -ldb -lix -les -lsql -lsqlimpl -lidea -liext "
#define CFIX_ESDBLIB		" -ldb -lix -les -lsql -lsqlimpl -liqlsrv -lidea -liext -ldb -lix -les -lsql -lsqlimpl -lidea -liext "
#define CFIX_SQLFWDBLIB		" -ldb -lix -les -lsql -lsqlimpl -liqlsrv -lidea -liext -ldb -lix -les -lsql -lsqlimpl -lidea -liext "
#define CFIX_SCKTLIB		""
#define CFIX_IDEALIB		" -liext -lidea -liqlsrv -liext -lidea -lm"
#define CFIX_IXDBSHLIB		" -ldb521_s -lix521_s -les521_s -lsql521_s -lsqlimpl521_s -liqlsrv521_s -lidea521_s -liext521_s "
#define CFIX_ESDBSHLIB		" -ldb521_s -lix521_s -les521_s -lsql521_s -lsqlimpl521_s -liqlsrv521_s -lidea521_s -liext521_s "
#define CFIX_SQLFWSHLIB		" -ldb521_s -lix521_s -les521_s -lsql521_s -lsqlimpl521_s -liqlsrv521_s -lidea521_s -liext521_s "
#define CFIX_SCKTSHLIB		""       
#define CFIX_IDEASHLIB		" -lidea521_s -liext521_s "
#define CFIX_MAX_MODS		100
#define CFIX_INCLUDE 		"/include"

#define CFIX_INAv1              " -DIXSCA "
#define CFIX_IXDBLIB_INA        " -lixsca -ldb -lix -les -lsql -lsqlimpl -ldb -lix -les -lsql -lsqlimpl -ldb -lpq "
#define CFIX_ESDBLIB_INA        " -lixsca -ldb -lix -les -lsql -lsqlimpl -ldb -lix -les -lsql -lsqlimpl -ldb -lpq "
#define CFIX_SQLFWDBLIB_INA     " -lixsca -ldb -lix -les -lsql -lsqlimpl -ldb -lix -les -lsql -lsqlimpl -ldb -lpq "
#define CFIX_IXDBSHLIB_INA      " -ldb521_s -lix521_s -les521_s -lsql521_s -lsqlimpl521_s -liqlsrv521_s -lixsca521_s -lpq "
#define CFIX_ESDBSHLIB_INA      " -ldb521_s -lix521_s -les521_s -lsql521_s -lsqlimpl521_s -liqlsrv521_s -lixsca521_s -lpq "
#define CFIX_SQLFWSHLIB_INA     " -ldb521_s -lix521_s -les521_s -lsql521_s -lsqlimpl521_s -liqlsrv521_s -lixsca521_s -lpq "


#endif
