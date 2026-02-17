/********************************************************************
* Copyright (c) 1987 InterSoft Co.  All Rights Reserved
* THIS IS UNPUBLISHED PROPRIETARY SOURCE CODE OF InterSoft Co.
* The copyright notice above does not evidence any
* actual or intended publication of such source code.
*
*
* $Id: critsect.cc,v 1.3 2010/09/17 20:23:13 leonardol Exp $
*
*********************************************************************/

#include <ideafix.h>
#include <cfix.h>
#include <cfix/priv/macros.h>
#include <ideafix/priv/gndefs.h>
#include <local/critsect.h>

// Implementation of class CCriticalSection
bool CCriticalSection::enter()
{
	semid = semget(key, nsems, 0666|IPC_CREAT|IPC_EXCL);
	if (semid < 0) {
		semid = semget(key, 0, 0666);
		if (semid < 0) {
			return false;
		}

		semarray[0].sem_num = 0;
		semarray[0].sem_op = -1;
		semarray[0].sem_flg = SEM_UNDO;

		if (semop(semid, semarray, 1) < 0 ) {
			return false;
		}

		return true;
	}

	mysemun.val = 1;
	if (semctl(semid, 0, SETVAL, mysemun) < 0) {
		return false;
	}

	semarray[0].sem_num = 0;
	semarray[0].sem_op = -1;
	semarray[0].sem_flg = SEM_UNDO;

	if (semop( semid, semarray, 1) < 0) {
		return false;
	}

	return true;
};

bool CCriticalSection::exit()
{
	semid = semget( key, 0, 0);

	if (semid < 0) {
		return false;
	}

	semarray[0].sem_num     = 0;
	semarray[0].sem_op      = +1;
	semarray[0].sem_flg     = SEM_UNDO;

	if (semop(semid, semarray, 1) < 0) {
		return false;
	}

	return true;
};

bool CCriticalSection::destroy()
{
	if (semctl(semid, 1, IPC_RMID, mysemun) < 0) {
		return false;
	}

	return true;
};

bool CCriticalSection::init(int _v)
{
	semid = semget(key, nsems, 0664 | IPC_CREAT | IPC_EXCL);
	if (semid < 0)
	{
		return false;
	}

	mysemun.val = _v;
	if (semctl(semid, 0, SETVAL, mysemun) < 0)
	{
		return false;
	}

	return true;
}

bool CCriticalSection::P(int _v)
{
	semarray[0].sem_num = 0;
	semarray[0].sem_op = -_v;
	semarray[0].sem_flg = 0;

	if (semop(semid, semarray, 1) < 0)
	{
		return false;
	}

	return true;
}

bool CCriticalSection::V(int _v)
{
	semarray[0].sem_num = 0;
	semarray[0].sem_op = _v;
	semarray[0].sem_flg = 0;

	if (semop(semid, semarray, 1) < 0)
	{
		return false;
	}

	return true;
}

