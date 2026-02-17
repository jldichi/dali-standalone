/********************************************************************
* Copyright (c) 1987 InterSoft Co.  All Rights Reserved
* THIS IS UNPUBLISHED PROPRIETARY SOURCE CODE OF InterSoft Co.
* The copyright notice above does not evidence any
* actual or intended publication of such source code.
*
* wirem.cc,v 1.1.1.1 1996/04/16 18:52:16 gustavof Exp
*
* DESCRIPTION:
*********************************************************************/

#include <ideafix.h>
#include <cfix.h>
#include <ifound/priv/gfuncs.h>
#include <ifound/str.h>
#include <signal.h>
#include <sys/types.h>
#include <unistd.h>
#include <stdlib.h>

#define SYS_TYPES

#include <ideafix/priv/gndefs.h>
#include <ideafix/priv/wmcom.h>
#include <cracker/pserconn.h>

#include <ifound/str.h>
// to avoid define timezone structure when I'm using
// shared libraries on SCO
#if !defined(SHIDEAIX) && !defined(SHIDEAES)
#ifndef __VC__
#	include <sys/time.h>
#else
#	include <time.h>
#endif
#endif
#include <errno.h>

static int my_pid = 0;
static bool init = false;

static void WiDisconnectAll(void)
{
	static bool recurs = false;
	if (!recurs)  {
		recurs = true;
		WiDisconnect(NULL);
	}
}

static void quit(void)
{
	WiDisconnectAll();
	onStop((FP)_global_abort);
	stop(0);
}

static int ARIDialog(int inflag, const char *label, const char *msg)
{
	wdflag flags = 0;
	wdflag ret;
	static bool aborting = false;
	if (aborting)
		return ERR;

	if (inflag & MSG_ERROR)	flags |= WD_ERROR;
	if (inflag & MSG_RELEASE) flags |= WD_RELEASE;
	assert(inflag & MSG_ERROR);
	ret = WiVDialog((inflag & MSG_WAIT) ?
			flags|WD_ABORT|WD_RETRY|WD_IGNORE :
			flags, WD_RETRY, label, (char*)msg, NULL);

	if (ret == WD_ABORT) {
		aborting = true;
		stop(1 | ABNORMAL_STOP);
	}
	return ret == WD_RETRY;
}

#if defined(HAVE_IPC) || defined (HAVENT_UI)
#include <errno.h>
#include <sys/types.h>
#ifdef HAVE_IPC
#include <sys/ipc.h>
#include <sys/sem.h>
#include <sys/msg.h>
#endif
#define SYS_TYPES

#include <ideafix/priv/gndefs.h>
#include <ifound/str.h>
#include <local/rcmdserv.h>

/* ++ Prototypes ++ */

static void WmRemote(int cmd);
static void dummy(void);
static void dummy1(void);
static void onalarm(void);
static int WiCheckSem(int sid);

/* -- Prototypes -- */

#define TIMEOUT	5

extern int errno;
extern int reint;
extern bool sig_rcv;
static int snd_channel = ERR;
static int rcv_channel = ERR;
static int semid = 0;

#ifndef HAVENT_UI
/* Put A remote command */
static void WmRemote(int cmd)
{
	static int wm_ncmds = ERR;
	int result, msg_size;
	char buff[WM_COM_BUFF_SIZE];
	struct msgbuf *message = (struct msgbuf *) buff;

	WiFlush();

    if (wm_ncmds == ERR)
		(void) _WmCommands(&wm_ncmds);

	if (cmd >= wm_ncmds) {
		fprintf(stderr,_WiErrmsg(ABNOR_SENT), cmd);
		stop(1 | ABNORMAL_STOP);
	}

	message->mtype = my_pid;
	
	/* save old handler to SIG_ALARM and set new function onalarm. */

	FPI oldh=SigSet(SIGALRM, (FPI) onalarm);

	do {
		msg_size =_EnCodeParameters(cmd, message->mtext, true)+sizeof(Int);

		if (WiSendMsg(snd_channel, buff, msg_size)==ERR) {
			fprintf(stderr,_WiErrmsg(ERR_SEND), errno);
			stop(1 | ABNORMAL_STOP);
		}

		/* espero la respuesta del wm */

		/* si me interrumpen reitento	*/
		reint=true;
		while((result=WiReceiveMsg(rcv_channel, buff, WM_COM_BUFF_SIZE
								  , Int(my_pid)))==INTPTS && reint)
			reint = false;

	} while(result==INTPTS);

	/* now I can restore old SIG_ALARM handler */

	(void)SigSet(SIGALRM, oldh);

	if (result!=OK && cmd != WM_WIDELETEPRO) {
		fprintf(stderr,_WiErrmsg(ERR_RECEI), errno);
		stop(1 | ABNORMAL_STOP);
	}

	_DecodeParameters(&cmd, message->mtext, false);
}

/* Conecta el proceso con el wm */


static void dummy(void)
{
	sig_rcv = true;
}

static void dummy1(void)
{
}

static void onalarm(void)
{
	if (WiDrain(snd_channel, 1)!=ERR) {
		fprintf(stderr,_WiErrmsg(CANT_CON));
		stop(1 | ABNORMAL_STOP);
	}
	reint = true;
}
#endif

char *keyHelpConversion(int , const char **fmt, int , Int *,
					    va_list *, char *wbuff, char *, Int *)
{
	char *cp = wbuff;
	if (**fmt != '_')
	    *cp = '\0';
	else {
		(*fmt)++;
		while (isalnum((UnChar)**fmt) || **fmt == '_')
			*cp++ = *(*fmt)++;
		*cp = '\0';
	    cp = wbuff;
		int keyCode = _FindCharCode(cp);
		if (keyCode != ERR)
			WiKeyHelp(UnChar(keyCode), (UnChar*)cp);
	}
	return cp;
}

// Cristiand AYSA RetShell zombie.
pid_t fatherPid = 0; 

void killTheRetShell(int c)
{
	warning("Segmentation Fault! The Program will be terminate abnormally.\n");	
	quit();	
	kill(SIGTERM, fatherPid);
	kill(SIGTERM, getpid());
	exit(-1);
}


extern "C" void WiConnect(char *tty, char *module, char *version)
{
	// check if this routine was called more than once
	if (init) return;
	// Create Connection for debugging mode
	bool connError = false;
	Int PRDBG = toInt(String("PRDBG").getEnv());
	if (PRDBG == 1 || PRDBG == 2) {
		_processConfig()->conn = tr_new PrServerConnection(RCMD_DBG_SERVICE,
														PRDBG == 1);
		if (_processConfig()->conn->status() != OKEY)
        	connError = true;
	}

	// Now I test if I was launched by the irexecd.
#ifndef HAVENT_UI
	if (PrServerConnection::isRemoteChannel(STDIN_FILENO)) {
		_processConfig()->conn = tr_new PrServerConnection();
		if (_processConfig()->conn->status() != OKEY)
			connError = true;
	}
#else
	String sdstr=String("IXSOCKDESCR").getEnv();
	Int sd = atol(toCharPtr(sdstr));
	if(!sd)
		stop(1);
#ifdef HAVE_WINSOCK
	WSADATA data;
	WSAStartup(MAKEWORD(1,1),&data);

#endif
	_processConfig()->conn = tr_new PrServerConnection(sd);
	if (_processConfig()->conn->status() != OKEY)
		connError = true;
#endif // HAVENT_UI

	// if I had a connection error show the message and finish the app
	if (connError) {
#ifdef __VC__
		char xxx[1024];
		sprintf(xxx, toCharPtr(libMsg("[CRACKER_CONN]")), _processConfig()->conn->status(), errno);
		String tmp = toString(xxx);
#else
		String tmp = doFormat(libMsg("[CRACKER_CONN]"), _processConfig()->conn->status(), errno);
#endif
		NetConnection::error(toCharPtr(tmp));
		assert(0);
	}

	if (crackerClient()) {
		my_pid = getpid();

		// Create process (and calculate net delay).
#if defined(SHIDEAIX) || defined(SHIDEAES)
		WiCreateProces(my_pid, module, NULL, version);
		_processConfig()->netDelay = ERR;
#else

#ifdef HAVE_SYSTEMTIME
		SYSTEMTIME beg,end;
		GetSystemTime(&beg);
#else
		int ret;
		timeval beg, end;
		ret = gettimeofday(&beg, NULL); assert(ret == 0);
#endif
		WiCreateProces(my_pid, module, NULL, version);
#ifdef HAVE_SYSTEMTIME
		GetSystemTime(&end);
		if(end.wSecond < beg.wSecond) end.wSecond += 60;
		_processConfig()->netDelay = 	(end.wSecond - beg.wSecond)*1000 +
										end.wMilliseconds - beg.wMilliseconds;
#else
		ret = gettimeofday(&end, NULL); assert(ret == 0);

		_processConfig()->netDelay = end.tv_sec * 1000000 + end.tv_usec -
									 (beg.tv_sec * 1000000 + beg.tv_usec);
#endif
#endif
	} else {
#ifndef HAVENT_UI
		char *Argv[4];
		char buff[10];
		int pid;

		if ((semid=WiOpenSem(tty, false))==ERR ||
			 	   WiCheckSem (semid)    ==ERR) {
			fprintf(stderr,_WiErrmsg(WM_NOT_RUN));
			stop(1 | ABNORMAL_STOP);
		}

		if ((snd_channel = WiOpenChannel(tty, 's', false)) == ERR) {
			fprintf(stderr,_WiErrmsg(WM_NOT_RUN));
			stop(1 | ABNORMAL_STOP);
		}

		if ((rcv_channel = WiOpenChannel(tty, 'r', false)) == ERR) {
			fprintf(stderr,_WiErrmsg(WM_NOT_RUN));
			stop(1 | ABNORMAL_STOP);
		}

		ExecWm = WmRemote;

		// find out if the wm is my dad
		my_pid = 1;
		
		(void) SigSet(SIGALRM, (FPI) onalarm);

		alarm(TIMEOUT);

		// Cristiand AYSA Retshell Zombie.
		fatherPid = getpid();
		SigSet(SIGSEGV, killTheRetShell);
		
		// the wm is not my dad
		while ( ! WiCreateProces(getpid(), module, NULL, version)) {

			(void) SigSet(SIGALRM, SIG_IGN);

			if (readEnvVar("SDB")) {
				sig_rcv = true;
				continue;
			}

			// if I couldn't find the retshell show a warning
			String dummyString;
			if (!findInPath(RETSHELL, NULL_STRING, NULL_STRING, dummyString)) {
				Warning(_WiErrmsg(RET_NOT_FOUND));
				sig_rcv = true;
				continue;
			}

			sig_rcv = false;
			(void)SigSet(SIGUSR2, (FPI) dummy);
			
			
			
			// if I'm dad -> execute the process that is the shell's son
			if ((pid = fork()) != 0) {

				if (pid == ERR) { /* ERR */ }

				(void)SigSet(SIGUSR2, SIG_IGN);

				Argv[0] = RETSHELL;
				sprintf(buff, "%d", pid);
				Argv[1] = buff;
				Argv[2] = tty;
				Argv[3] = NULL;
				execvp((EXECVPPARAM1TYPE) Argv[0], (EXECVPPARAM2TYPE) Argv);
				/* ERR */
				_exit(255);

			}
			while (!sig_rcv) {
			/*
				this code is necessary for some machines which do a context
				switch in this place between this process and retshell when
				it executes a kill to this process, in this case this process
				hangs up because when !sig_rcv is tested its value is
				false, then receives a signal, calls pause routine and
				hangs up.
			 */
				(void) SigSet(SIGALRM, dummy1);
				alarm(1);
				_global_pause();
				(void) SigSet(SIGALRM, SIG_IGN);
			}
		}
		my_pid = getpid();
#else
		error(libMsg("MUSTBECRACKERCLIENT"), userName(procUserId()),
			  appName());
#endif
	}

	// now define the interrupts handling & Messager funct.
	SigSet(SIGINT, SIG_IGN);
#ifdef HAVE_FULL_SIGNALS
	SigSet(SIGALRM, SIG_IGN);
	SigSet(SIGQUIT,SIG_IGN);
#endif

#if HAVE_SIGEMT
	SigSet(SIGEMT, (FPI) quit);
#endif
	onStop(WiDisconnectAll);
   	displayHandler(WiHandler);
	displayErrorHandler(WiHandler);
	fatalErrorHandler(WiHandler);
	ariErrorHandler(ARIDialog);
	setEnableIntHandler(WiInterrupts);

	addConvertFunction('K', keyHelpConversion);
	init = true;

	// if the signal is ERROR it means that this process doesn't have
	// permission to execute. I had to wait until here because I must
	// be connected to the cracker to show a nice error message.
	if (_processConfig()->sig == ERR) {
		error(libMsg("USERHASNOPERM"), userName(procUserId()),
			  toCharPtr(appName()));
	}
}

#ifndef HAVENT_UI
static int WiCheckSem(int sid)
{
	struct sembuf x[1];

	x[0].sem_num = 0;
	x[0].sem_op =  0;
	x[0].sem_flg = IPC_NOWAIT;

	if (semop(sid, x, 1) == ERR) {
		if (errno != EAGAIN) Error(_WiErrmsg(NO_MORE_SEM), errno);
		return OK;
	}
	return ERR;
}
#endif

/* desconecta el proceso con el wm */

extern "C" void WiDisconnect(char *)
{
	if (crackerClient()) {
		if (init == false || _processConfig()->conn == NULL)
			return;

		init = false;
		// WiMsg("Before WiDeleteProces");
		WiDeleteProces(my_pid);
		// WiMsg("After WiDeleteProces");

		prServerConn(PrCmd::wiDisconnect).flush();

		tr_delete(_processConfig()->conn);
	} else {
#ifndef HAVENT_UI
		if (init == false || (snd_channel == ERR && rcv_channel == ERR))
			return;

		init = false;
		(void) WiDeleteProces(my_pid);
		snd_channel = rcv_channel = ERR;
#endif
	}
}

#endif // HAVE_IPC
