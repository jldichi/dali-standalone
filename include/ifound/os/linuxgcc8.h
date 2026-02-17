/***********************************************************************
* Copyright (c) 1991 InterSoft Co.  All Rights Reserved
* THIS IS UNPUBLISHED PROPRIETARY SOURCE CODE OF InterSoft Co.
* The copyright notice above does not evidence any
* actual or intended publication of such source code.
*
* DESCRIPTION
* Operating system dependent defines
*********************************************************************/

#ifndef LINUX_H
#define LINUX_H

#ifndef LinuxGcc8
#define LinuxGcc8				1
#endif

#define _OS_				"LinuxGcc8"
#define _OS_SHORT_NAME_		"linuxgcc8"
#define _OSVER_				" "			// Completar con una descrip. del S.O. y kernel

#if defined(ACADEMIC_VERSION)
#define EXTRA_BANNER_INFO   "  "
#elif defined (BETA_VERSION)
#   define  EXTRA_BANNER_INFO   "  " 
#else
#   define  EXTRA_BANNER_INFO   ""
#endif

/* Typedefs */
typedef unsigned int UidT;
typedef unsigned int GidT;
typedef int OffT;
typedef int PidT;
typedef int KeyT;
typedef unsigned int ModeT;
typedef union u_SemunT {
	int val;
	struct semid_ds *buf;
	unsigned short *array;
} SemunT;
#include <sys/types.h>

typedef size_t SizeT;
//#ifndef time_t
//typedef long int time_t;
//#endif


/* Insert here locally needed defines */
#undef _POSIX_SOURCE
#undef _POSIX_VERSION
#define __USE_BSD		1

#define	stddbg	stderr
/* Compiler dependant capabilities */
/* Visual C++ compatibility */
#undef DLLEXPORT
#define	DLLIMPORT
#define	DLLEXPORT
#define DLLDEPEND

//Esto empieza a joder en los gcc 3.x
//Lo defino vacio. hardaiz.
#undef __THROW
#define __THROW

/* Non compiler dependant capabilities */
#define NO_NEW_HANDLER
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
#define HAVE_BZERO				1
#undef HAVE_BSDTTY_H
#undef HAVE_CLOSEDIR_BUG
#undef HAVE_COMMAND_COM
#define	HAVE_LEX_DIFF_BEHAVIOR	1
#undef HAVE_EMULTIHOP
#undef HAVE_ENOLINK
#define	HAVE_ENUM_SETRLIMIT		1
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
#define HAVE_GETGROUPS			1
#undef HAVE_GETPWANAM
#define HAVE_GETSPNAM			1
#undef HAVE_GETUSERPW
#undef HAVE_HEAP_WALK
#define	HAVE_INETD				1
#define HAVE_IPC				1
#define HAVE_JOB_CONTROL		1
#define HAVE_LOCALTIME			1
#define	HAVE_INLINE_STAT		1
#undef HAVE_LOCKF
#undef HAVE_LOCKING
#define HAVE_LONG_ARGV			1
#undef HAVE_LTCHARS
#define HAVE_MAIL				1
#undef HAVE_MMDF				
#define HAVE_MMF				1
#define HAVE_MEMMOVE			1
#define HAVE_MKNOD				1
//#define HAVE_MKTEMP				1
#undef HAVE_OP_CONST_VOID_PTR
#define HAVE_O_SYNC				1
#define HAVE_OPENDIR			1
#undef HAVE_PASTE
#undef HAVE_PW_COMMENT
#define HAVE_PID				1
#define HAVE_PIPES				1
#undef HAVE_POLL
#undef HAVE_PRAGMA_ALIGN
#define HAVE_PROC				1
#define HAVE_PROC_DIR			1
#undef HAVE_PSEUDO_CONCURR
#define HAVE_RANLIB				1
#define HAVE_REAL_LOCKS			1
#define HAVE_RENAME
#define HAVE_S_ISSOCK_WORKING_OK       1
#define HAVE_RESTARTABLE_SYSCALLS	1
#undef HAVE_SAVEMEM
#define HAVE_SECURITY			1
#define HAVE_SEMAPHORES			1
#define HAVE_SETRLIMIT			1
#define HAVE_SETSID				1
#define HAVE_SETUID				1
#undef HAVE_SGR
#define HAVE_SHARE_TERMINFO_DIR	1
#undef HAVE_SHORT_ENV
#undef HAVE_SHORT_FILE_NAMES
#undef HAVE_SHORT_MEMSET
#define HAVE_SIGABRT			1
#define HAVE_SIGPWR				1
#define HAVE_SIGSET				1
#define HAVE_SIGVEC 			1
#define	HAVE_SIGACTION			1
#undef HAVE_SIGEMT
#define HAVE_SIGUNUSED			1
#undef HAVE_SIGSYS
#undef HAVE_SOUND_BEEP
#undef HAVE_STKLEN
#define HAVE_STRSTR				1
#define HAVE_STRTOK				1
#undef HAVE_SWAP
#define HAVE_SYNC				1
#undef HAVE_SYSTTOLD_H
#undef HAVE_SYS_TIMEB_H
#define HAVE_SYS_VFS_H			1
#define HAVE_TEMPNAM_BUG		1
#define HAVE_TERMIOS_H			1
//#undef HAVE_TERMIO_H			
#undef HAVE_TEXT_FILES
#undef HAVE_TIMEOFFSET
#define HAVE_TIMEZONE			1
#define HAVE_TIME_H				1
#define HAVE_TMPFILE			1
#define DEFAULT_TMPDIR			"/tmp"
#undef HAVE_TM_ZONE
#define HAVE_TRUNCATE			1
#define HAVE_TTY				1
#define HAVE_UMASK				1
#define HAVE_UNISTD_H			1
#define HAVE_USERS				1
#define HAVE_USEFULFTRUNCATE	1
#define HAVE_UTIME_H			1
//#define HAVE_USEUSEMUN			1
#undef HAVE_USEUSEMUN
#define HAVE_WAITPID			1
#undef HAVE_XCC
#undef HAVE_ZPM
#undef LOG_DBG
#define NEED_AR_PORTAR			1
#undef NEED_CLOSE_FOR_SYNC
#undef NEED_MEM_DEBUG
#undef NEED_NETBIOS_FOPEN
#undef NEED_WCHAR_T
#undef NO_ADVISORY_LOCK
#undef NO_EXTERN_CONST
#undef NO_MKRMDIR
#undef NO_SERVICES
#undef OPEN_CANT_CREATE
#undef SYSTEM_CANT_EXEC_AS_ROOT
//#undef SPANISH
#define SPANISH					1
//#define ENGLISH					1
#undef ENGLISH
#undef STATFS_HAS_LEN
#undef SYSDIR
#undef SYSNDIR
#undef USE_FGETC
#undef USG
#define _G_USE_PROTOS			1
#undef HAVE_SARWITHSWAP
#define HAVE_NOPROBLEMSCLOSING012	1
#define HAVE_SYSLOG				1
#undef HAVE_VARARGS_BUG
#undef HAVE_GRAPHICS
// do not uncomment this line
//#define PATH_MAX 			1024

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
#undef NO_CONTROL_192_255
#undef IDEAWIN
#undef _16BIT
#define _far
#define __far

/* Non compiler dependant parameter types */
#define ABORTRETTYPE 			void
#define ALLOCRETTYPE			void *
#define BCOPYPARAM3TYPE			size_t
#define	BZEROPARAM2TYPE			unsigned int
#define BINDPARAM2TYPE			const struct sockaddr *
#define BINDPARAM3TYPE			unsigned int
#define CFTIMEPARAM2TYPE 		char *
#define CHOWNPARAM2TYPE 		UidT
#define CHOWNPARAM3TYPE 		GidT
#define	SOCKETRETTYPE			int
#define	CONNECTRETTYPE			int
#define	CONNECTPARAM1TYPE		int
#define CONNECTPARAM2TYPE		const struct sockaddr *
#define CONNECTPARAM3TYPE		unsigned int
#define EXECVPPARAM1TYPE		const char * 
#define EXECVPPARAM2TYPE		char * const *
#define FCNTLPARAM3TYPE			...
#define FTELLRETTYPE			long
#define FTELLPARAM1TYPE			FILE *
#define FDOPENPARAM2TYPE		const char *
#define FPUTCPARAM1TYPE			int
#define FREADPARAM1TYPE			void *
#define FTRUNCATEPARAM2TYPE		long
#define FWRITEPARAM1TYPE		const void *
#define GCVTPARAM2TYPE          int    
#define GETCWDPARAM1TYPE		char *
#define GETCWDPARAM2TYPE 		SizeT
#define GETGRGIDPARAMTYPE		GidT
#define GETPWUIDPARAMTYPE		UidT
#define IOCTLPARAM2TYPE			long unsigned int
#define LISTENPARAM2TYPE        int
#define MKDIRPARAM1TYPE			const char *
#define MSGCTLPARAM3TYPE		struct msqid_ds *
#define MSGRCVPARAM2TYPE		void *
#define MSGRCVPARAM3TYPE		unsigned int
#define MSGSNDPARAM2TYPE		const void *
#define MSGSNDPARAM3TYPE		unsigned int
#define MUNMAPPARAM1TYPE		void *
#define MUNMAPPARAM2TYPE		SizeT
#define OPENPARAM3TYPE			...
#define OPENRETTYPE				int
#define PAUSERETTYPE 			int
#define POPENPARAMTYPE			const char *
#define PUTENVPARAMTYPE			char *
#define QSORTPARAM2TYPE			SizeT
#define QSORTPARAM3TYPE			SizeT
#define QSORTRETTYPE			void
#define	RCMDPARAM3TYPE		   	const char *
#define	RCMDPARAM4TYPE			const char *
#define	RCMDPARAM5TYPE			const char *
#define READPARAM3TYPE			SizeT
#define READRETTYPE				int
#define REALPATHPARAM1TYPE		const char *
#define RETSIGTYPE				void
#define SBRKPARAMTYPE 			int
#define SBRKRETTYPE 			void *
/* #define SEMCTLPARAM4TYPE		SemunT */
#define SEMOPPARAM3TYPE			unsigned
#define SLEEPPARAMTYPE			unsigned
#define SLEEPRETTYPE			unsigned
#define SPRINTFRETTYPE			int
#define SRANDRETTYPE			void
#define	STRDUPPARAMTYPE			const char *
#define STATFSPARAM1TYPE		const char *
#define TIMEPARAM1TYPE			long int *
#define TIMERETTYPE				long int 
#define WRITEPARAM3TYPE			SizeT
#define WRITERETTYPE			int
#define _FLSBUFPARAM1TYPE		unsigned char
#define POLLPARAM1TYPE 			struct pollfd *
#define POLLPARAM2TYPE			unsigned long
#define POLLPARAM3TYPE			int
#define ECVTPARAM2TYPE			int
#define ECVTRETTYPE 			char *
#define FCVTPARAM2TYPE			int
#define FCVTRETTYPE 			char *
#define EXITRETTYPE				void
#define	SYNCRETTYPE				void
#define TEMPNAMPARAM1TYPE		char const *
#define TEMPNAMPARAM2TYPE		char const *
#define GETHOSTBNPARAMTYPE		char const *
#define GETSERVBNPARAM1TYPE		char const *
#define GETSERVBNPARAM2TYPE		char const *
#define GETHOSTNPARAM2TYPE		unsigned
#define GETPEERNAMEPARAM2TYPE	struct sockaddr *
#define GETPEERNAMEPARAM3TYPE	unsigned int *
#define NPGETPEERNAMEPARAM3TYPE	unsigned int
#define WINDOWTYPE				int
#define ACCEPTPARAM2TYPE		struct sockaddr *
#define ACCEPTPARAM3TYPE		unsigned int *
#define BITFIELDSTYPE			unsigned long
#define SYS_ERRLISTRETTYPE		const char * const
#define SETRLIMITPARAM1TYPE		enum __rlimit_resource
#define SETRLIMITPARAM2TYPE		struct rlimit *
#define CREATPARAM2TYPE			SizeT

/* Miscelaneous */
#define PSUID_CMD				"ps -lp%ld"
#define PSUID_POS				1
#define MAX_WINDOWS				512
#define	STDERR					stderr
#define FM_MAX_OPEN				128
#define AR_MOD_SEP				'/'
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
typedef RETSIGTYPE (*SigHandlerT)(int);

/* Cfix stuff */
#define IDEA_VERSION_Q "610"

#define CFIX_HAVE_SHLIB		1
#define CFIX_HAVE_ESS_SHLIB 1
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
#define CFIX_DEBUG			" -g"
#define CFIX_INAv1          " -DIXSCA "
#define CFIX_LINK			"g++ "
#define CFIX_LINK_IFX		"esql "
#define CFIX_LINK_ORA		"g++ "
#define CFIX_LIBDIRSFLAG	" -L"
#define CFIX_LIBDIR			"/lib "
#define CFIX_SHLIBDIR		"/SH"IDEA_VERSION_Q""
#define CFIX_LINKSTRIP		" -s "
#define CFIX_LINKDEBUG		" -g "
#define CFIX_OUTFILE		" -o "
#define CFIX_IXDBLIB       	" -ldb -lix -lsql "
#define CFIX_ESDBLIB       	" -ldb -lix -lsql "
#define CFIX_SQLFWDBLIB		" -ldb -lix -lsql "
#define CFIX_SCKTLIB		" "
#define CFIX_IDEALIB		" -liext -lidea -lm"
#define CFIX_IXDBSHLIB     	" -ldb -lix -lsql "
#define CFIX_ESDBSHLIB 		" -ldb -lix -lsql "
#define CFIX_SQLFWSHLIB    	" -ldb -lix -lsql "

//Defines para INA x Telnet
#define CFIX_IXDBLIB_INA        " -lixsca -ldb -lix -les -lsql -lsqlimpl -ldb -lix -les -lsql -lsqlimpl -ldb -lpq "
#define CFIX_ESDBLIB_INA        " -lixsca -ldb -lix -les -lsql -lsqlimpl -ldb -lix -les -lsql -lsqlimpl -ldb -lpq "
#define CFIX_SQLFWDBLIB_INA     " -lixsca -ldb -lix -les -lsql -lsqlimpl -ldb -lix -les -lsql -lsqlimpl -ldb -lpq "
#define CFIX_IXDBSHLIB_INA      " -ldb"IDEA_VERSION_Q"_s -lix"IDEA_VERSION_Q"_s -les"IDEA_VERSION_Q"_s -lsql"IDEA_VERSION_Q"_s -lsqlimpl"IDEA_VERSION_Q"_s -liqlsrv"IDEA_VERSION_Q"_s -lixsca"IDEA_VERSION_Q"_s -lpq "
#define CFIX_ESDBSHLIB_INA      " -ldb"IDEA_VERSION_Q"_s -lix"IDEA_VERSION_Q"_s -les"IDEA_VERSION_Q"_s -lsql"IDEA_VERSION_Q"_s -lsqlimpl"IDEA_VERSION_Q"_s -liqlsrv"IDEA_VERSION_Q"_s -lixsca"IDEA_VERSION_Q"_s -lpq "
#define CFIX_SQLFWSHLIB_INA     " -ldb"IDEA_VERSION_Q"_s -lix"IDEA_VERSION_Q"_s -les"IDEA_VERSION_Q"_s -lsql"IDEA_VERSION_Q"_s -lsqlimpl"IDEA_VERSION_Q"_s -liqlsrv"IDEA_VERSION_Q"_s -lixsca"IDEA_VERSION_Q"_s -lpq "

#define CFIX_SCKTSHLIB		" "
#define CFIX_IDEASHLIB		" -lidea -liext -lm"
#define CFIX_MAX_MODS		100
#define CFIX_INCLUDE 		"/include"

/* because of COD_ZERO, invented by Eugen, we must define it somewere*/
#define	COD_ZERO	0x1f

#endif

