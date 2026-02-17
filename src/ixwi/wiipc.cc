/********************************************************************
* Copyright (c) 1987 InterSoft Co.  All Rights Reserved
* THIS IS UNPUBLISHED PROPRIETARY SOURCE CODE OF InterSoft Co.
* The copyright notice above does not evidence any
* actual or intended publication of such source code.
*
* $Id: wiipc.cc,v 1.3 2004/09/29 21:36:17 damianm Exp $
*
* DESCRIPTION
*********************************************************************/

#include <ideafix.h>
#include <cfix.h>

#ifdef HAVE_IPC

#include <sys/types.h>
#define SYS_TYPES
#include <fcntl.h>
#include <unistd.h>
#include <sys/stat.h>
#include <sys/ipc.h>
#include <errno.h>
#include <sys/sem.h>
#include <sys/msg.h>
#include <ideafix/priv/gndefs.h>
#include <ideafix/priv/wmcom.h>
#include <ifound/priv/macros.h>

/* ++ Prototypes ++ */

static key_t WiTtyKey(char *tty, int c);

/* -- Prototypes -- */

extern int errno;


/*
* Open a comunication channel
*/

extern "C" int WiOpenChannel(char *tty, int c, bool create_it)
{
	int ret;
	key_t key = WiTtyKey(tty, c);

	ret = msgget(key, 0);
	if ( create_it ) {
		/*
		* Remove the queue if exists
		*/
		if (ret != ERR) {
			struct msqid_ds buff;
			msgctl(ret, IPC_RMID, &buff);
		}
		ret = msgget(key, IPC_CREAT | 0666);
		if (ret == ERR && errno == ENOSPC) {
			fprintf(stderr, _WiErrmsg(NO_MORE_QUEUE), errno);
			stop(1 | ABNORMAL_STOP);
		}
	}
	return ret;

}

extern "C" int WiOpenSem(char *tty, bool create_it)
{
	int semid;

	semid = semget(WiTtyKey(tty, 0), 2, create_it ? (IPC_CREAT | 0666)
												  : 0);

	if (semid == ERR) {
		if (!create_it && errno==ENOENT) return ERR;
		Error(_WiErrmsg(NO_MORE_SEM), errno);
	}

	return semid;
}

static int _f_open = 0;

extern "C" void WiSetFOpen(int i)
{
	_f_open = i;
}

static key_t WiTtyKey(char *tty, int c)
{
	struct stat st;

	if (tty == NULL || *tty == '\0') 
		fstat(_f_open, &st);
	else {
		int f = _global_open(tty, _f_open, 0);
		if (f == ERR) return ERR;
		fstat(f, &st);
		_global_close(f);
	}

	c = (c & 1) | ((st.st_size & 0xFF) << 1);

	return (Int)(c&0xFF) << 24 | (Int)(st.st_dev&0xFF) << 16
	  | (st.st_ino&0xFFFF); 
}

/*
 * Remove Semaphore
 */
extern "C" void WiRmSem(int semid)
{
#ifdef HAVE_USEUSEMUN
	union semun a;
#else
	SemunT a;
#endif	

	if (semid != ERR)
		semctl(semid, 0, IPC_RMID, a);
}

/*
 * Remove a comunication Channel
 */
extern "C" void WiRmChannel(int channel)
{
	struct msqid_ds buff;

	if (channel != ERR) 
		msgctl(channel, IPC_RMID, &buff);
}

/*
 * Send a message 
 */
extern "C" int WiSendMsg(int msgid, char *_msg, int msgsize)
{
	return msgsnd(msgid, (struct msgbuf *)_msg, msgsize-sizeof(Int),0);
}

/*
 * Receive a message 
 */
extern "C" int WiReceiveMsg(int msgid, char *_msg, int msgsize, Int msgtype)
{
	switch(msgrcv(msgid, (struct msgbuf *) _msg, msgsize-sizeof(Int), 
		  msgtype, 0)) {
	case ERR:
		return (errno==EINTR) ? INTPTS : ERR;
	default:
		return OK;
	}
}

extern "C" int WiDrain(int msgid, int pid)
{
	int execrcv = ERR;
	char message[sizeof(Int)];

	while(msgrcv(msgid, (struct msgbuf *) message, 0, (Int) pid, 
				 IPC_NOWAIT|MSG_NOERROR) != ERR)
		execrcv = OK;

	return execrcv; 
}
#endif

