/********************************************************************
* Copyright (c) 1991 InterSoft Co.  All Rights Reserved
* THIS IS UNPUBLISHED PROPRIETARY SOURCE CODE OF InterSoft Co.
* The copyright notice above does not evidence any
* actual or intended publication of such source code.
*
* $Id: wiipc.cc,v 1.1.1.1 1996/04/16 18:52:44 gustavof Exp $
*
* DESCRIPTION
* Implementation of window class
*********************************************************************/

#include <ifound.h>
#include <local/strfile.h>
#include <winapp/winmgr.h>
#include <ifound/stdc.h>

#include <sys/stat.h>
#include <sys/ipc.h>
#include <errno.h>
#include <sys/sem.h>
#include <sys/msg.h>

#include <sys/types.h>
#define SYS_TYPES

static key_t ttyKey(const char *tty, int c);

/*
* Open a comunication channel
*/

int WinMgr::openChannel(const char *tty, int c, ixBool create_it)
{
	int ret;
	key_t key = ttyKey(tty, c);

	ret = msgget(key, 0);
	if ( create_it ) {
		/*
		* Remove the queue if exists
		*/
		if (ret != ERROR) {
			struct msqid_ds buff;
			msgctl(ret, IPC_RMID, &buff);
		}
		ret = msgget(key, IPC_CREAT | 0666);
		if (ret == ERROR && errno == ENOSPC) {
			stdMsg << libMsg("NOMQ") << endMsg;
			stop(1);
		}
	}
	return ret;

}

int WinMgr::openSem(const char *tty, ixBool create_it)
{
	int semid;

	semid = semget(ttyKey(tty, 0), 2, create_it ? (IPC_CREAT | 0666)
												  : 0);

	if (semid==ERROR) {
		if (!create_it && errno==ENOENT) return ERROR;
		stdMsg << libMsg("NOMQ") << endMsg;
		stop(1);
	}

	return semid;
}

static key_t ttyKey(const char *tty, int c)
{
	struct stat st;
	if (tty) {
		int f = open(tty, 0, 0666);
		if (f == ERROR) return ERROR;
		fstat(f, &st);
		close(f);
	}
	else
		fstat(0, &st);

	c = int(c & 1 | (st.st_size & 0xFF) << 1);

	return (long)(c&0xFF) << 24 | (long)(st.st_dev&0xFF) << 16
	  | (st.st_ino&0xFFFF); 
}

/*
 * Remove Semaphore
 */
void WinMgr::rmSem(int semid)
{
	SemunT a;

	if (semid != ERROR)
		semctl(semid, 0, IPC_RMID, a);
}

/*
 * Remove a comunication Channel
 */
void WinMgr::rmChannel(int channel)
{
	struct msqid_ds buff;

	if (channel != ERROR) 
		msgctl(channel, IPC_RMID, &buff);
}

/*
 * Send a message 
 */
int WinMgr::sendMsg(int msgid, char *_msg, int msgsize)
{
	return msgsnd(msgid, (struct msgbuf *)_msg, msgsize-sizeof(long),0);
}

/*
 * Receive a message 
 */
int WinMgr::receiveMsg(int msgid,char *_msg,int msgsize,long msgtype)
{
	int res = msgrcv(msgid, (struct msgbuf *) _msg, 
					 msgsize-sizeof(long), msgtype, 0);

	switch(res) {
	case ERROR:
		return (errno==EINTR) ? INTPTS : ERROR;
	default:
		return res;
	}
}
