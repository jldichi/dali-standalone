/********************************************************************
*
* Copyright (c) 1995 InterSoft Co.  All Rights Reserved
* THIS IS UNPUBLISHED PROPRIETARY SOURCE CODE OF InterSoft Co.
* The copyright notice above does not evidence any
* actual or intended publication of such source code.
*
* $Id: critsect.h,v 1.16 2010/09/17 20:23:13 leonardol Exp $
*
* DESCRIPTION:
*
* defines a critical section using a binary semaphore
*
*********************************************************************/
#ifndef __CRITSECT__H__
#define __CRITSECT__H__

#ifndef	  __NT__

#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/sem.h>
#include <sys/shm.h>

// Added to use semaphores in OS'es where this union is not defined
#if defined(sun) || (defined (hpux) && defined(hppa)) || defined(_AIX) || defined (Linux6) || defined (Linux7) || defined(LinuxGcc3) || defined(LinuxGcc4) || defined(LinuxGcc5) || defined(LinuxGcc8) || (defined (M_UNIX) && defined (_M_SYSV))
	union semun {
		int val;
		struct semid_ds *buf;
		ushort *array;
	};
#endif

#else
typedef int key_t;
#endif	// __NT__

// Because of how this key is used in the code below, this key must NOT be repeated
// among other sempahores running in the system.

#define ULICENSE_SEMAPHORE_KEY		(key_t)8887

/**
 * class CCriticalSection
 *
 * This class implements a critical section based on a binary sempahore.
 */
class CCriticalSection 
{
	int semid;
	int nsems;
	key_t	key;
#ifndef __NT__
	struct sembuf semarray[1];
	union semun mysemun;
#endif
	
public:
	CCriticalSection(key_t xxkey = IPC_PRIVATE) : semid(0), key(xxkey), nsems(1) { };
	~CCriticalSection() {};
	
	// Viejo uso, para compatibilidad con verificacion de licencias
	// Tambien se utilizan estos metodos en FW
	bool enter();
	bool exit(); 
	bool destroy();

	// Nuevos metodos, mas genericos
	bool init(int _v = 0);
	bool P(int _v = 1);
	bool V(int _v = 1);
};

/**
 * class CCriticalSection_
 *
 * This class is just a wrapper of CCriticalSection, and it uses the exit() function
 * in its destructor to ensure that when the object is destroyed we leave the
 * critical section, The semaphore is released, but not ERASED.
 * It is used to define the macros BEGIN_CRITICAL_SECTION and END_CRITICAL_SECTION
 */
class CCriticalSection_ : public CCriticalSection {
public:
	CCriticalSection_(key_t xxkey) : CCriticalSection(xxkey) { };
	~CCriticalSection_() { assert(exit()==true); }; 
};

/**
 * Take into account that these macros DON'T destroy the sempahore, they RELEASE
 * the sempahore when leaving the scope.
 */
#ifndef	BEGIN_CRITICAL_SECTION
#define BEGIN_CRITICAL_SECTION(k)	{ CCriticalSection_ cs(k); assert(cs.enter()==true);
#endif // BEGIN_CRITICAL_SECTION

#ifndef	END_CRITICAL_SECTION
#define END_CRITICAL_SECTION		} 
#endif // END_CRITICAL_SECTION

#endif
