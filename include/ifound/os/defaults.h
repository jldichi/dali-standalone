/* *******************************************************
* Defaults definitions
*
******************************************************** */

#ifndef OS_DEFAULTS_H
#define OS_DEFAULTS_H

#ifndef BZEROPARAM2TYPE
#define BZEROPARAM2TYPE 	int
#endif

#ifndef LDIVRETTYPE
#define LDIVRETTYPE 	ldiv_t
#endif

// comentado porque es llamado despues del header propio del s.o. y entonces
// no se puede undefinir.
//#ifndef HAVE_GETHOSTID
//#define HAVE_GETHOSTID 		1
//#endif

#ifndef SETRLIMITPARAM2TYPE
#define SETRLIMITPARAM2TYPE   const struct rlimit *
#endif

#ifndef SETRLIMITPARAM1TYPE
#define SETRLIMITPARAM1TYPE   int
#endif

#ifndef LISTENPARAM2TYPE
#define LISTENPARAM2TYPE                int
#endif

#ifndef NPGETPEERNAMEPARAM3TYPE
#define NPGETPEERNAMEPARAM3TYPE    int
#endif

#ifndef SYS_ERRLISTRETTYPE
#define SYS_ERRLISTRETTYPE		char * 
#endif

#ifndef MSGRCVPARAM2TYPE
#define MSGRCVPARAM2TYPE		void *
#endif

#ifndef MSGRCVRETTYPE
#define MSGRCVRETTYPE			int
#endif

#ifndef LSEEKRETTYPE
#define LSEEKRETTYPE			long
#endif

#ifndef LSEEKPARAM2TYPE
#define LSEEKPARAM2TYPE			long
#endif

#ifndef LOCKFPARAM3TYPE
#define LOCKFPARAM3TYPE			long
#endif

#ifndef TIMERETTYPE
#define TIMERETTYPE			long
#endif

#ifndef TIMEPARAM1TYPE
#define TIMEPARAM1TYPE			long *
#endif

#ifndef SYSLOGRETTYPE
#define SYSLOGRETTYPE		void
#endif

#ifndef RCMDPARAM2TYPE
#define RCMDPARAM2TYPE		unsigned short
#endif

#endif
