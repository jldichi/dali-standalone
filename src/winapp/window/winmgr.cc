/********************************************************************
* Copyright (c) 1991 InterSoft Co.  All Rights Reserved
* THIS IS UNPUBLISHED PROPRIETARY SOURCE CODE OF InterSoft Co.
* The copyright notice above does not evidence any
* actual or intended publication of such source code.
*
* $Id: winmgr.cc,v 1.1.1.1 1996/04/16 18:52:45 gustavof Exp $
*
* DESCRIPTION
* Implementation of window manager class
*********************************************************************/

#include <ifound.h>
#include <signal.h>
#include <errno.h>

#ifdef HAVE_IPC
#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/sem.h>
#include <sys/msg.h>
#endif

#include <local/strfile.h>
#include <winapp/winmgr.h>
#include <winapp/window.h>
#include <ifound/stdc.h>
#include <winapp/winmsg.h>

static ixBool checkSem(int sid);

#define TIMEOUT	5

static  ixBool sig_rcv    = FALSE;
static  int reint       = FALSE;

WinMgr *WinMgr::head  = NULL;
WinMgr *WinMgr::defWm = NULL;

Window *WinMgr::getRootWindow()
{
	return rootWindow;
}

void WinMgr::insNode()
{
	if (head==NULL) {
		head = this;
		next = NULL;
		return;
	}

	for (WinMgr *aux=head; aux->next; aux=aux->next); 

	aux->next = this;
	this->next = NULL;
}

void WinMgr::delNode()
{
	if (head==this) {
		head = this->next;
		return;
	}

	for (WinMgr *aux=head; aux->next && aux->next!=this; aux=aux->next);

	aux->next = aux->next->next;
}

void WinMgr::refresh()
{
	initFunc(WM_WIREFRESH);
	execCmd();
}

void WinMgr::redraw()
{
	initFunc(WM_WIREDRAW);
	execCmd();
}

void WinMgr::initFunc(short cmd)
{
	if (cmd >= LASTCOMMAND) {
		stdMsg << libMsg("OUTOFRAN") << endMsg;
		stop(1);
	}
	ncmd  = cmd;
	index = buffer;
#ifdef HAVE_IPC
	push(ncmd);
#else
	paramNum = 0;
#endif
}

#ifndef HAVE_IPC

extern "C" void	_WiCreate(),
			_WiDelete(),
			_WiDeleteAll(),
			_WiSwitchTo(),
			_WiRefresh(),
			_WiRedraw(),
			_WiSetBorder(),
			_WiStatusLine(),
			_WiGetAttr(),
			_WiOrgCol(),
			_WiOrgRow(),
			_WiLine(),
			_WiCol(),
			_WiWidth(),
			_WiHeight(),
			_WiInChar(),
			_WiInAttr(),
			_WiGetTab(),
			_WiSetTab(),
			_WiParent(),
			_WiFlush(),
			_WiGetc(),
			_WiGets(),
			_WiKeyHelp(),
			_WiInterrupts(),
			_WiCurrent(),
			_WiCreateProces(),
			_WiDeleteProces(),
			_WiSettty(),
			_WiResettty(),
			_WiGetField(),
			_WiAplHelp(),
			_WiSetAplHelp(),
			_WiProcess(),
			_WiSetRaw(),
			_WiKeyReady(),
			_WiSetServices(),
			_WiMove(),
			_WiResize(),
			_WiService();

static struct {
	void (*fp)();
} commands[] = {
	NULL /* DUMMY */,
	NULL /* _WiStop */,
	_WiCreate,
	_WiDelete,
	_WiDeleteAll,
	_WiSwitchTo,
	_WiRefresh,
	_WiRedraw,
	_WiSetBorder,
	_WiStatusLine,
	_WiGetAttr,
	_WiOrgCol,
	_WiOrgRow,
	_WiLine,
	_WiCol,
	_WiWidth,
	_WiHeight,
	_WiInChar,
	_WiInAttr,
	_WiGetTab,
	_WiSetTab,
	_WiParent,
	_WiFlush,
	_WiGetc,
	_WiGets,
	_WiKeyHelp,
	_WiInterrupts,
	_WiCurrent,
	_WiCreateProces,
	_WiDeleteProces,
	_WiSettty,
	_WiResettty,
	_WiGetField,
	_WiAplHelp,
	_WiSetAplHelp,
	_WiProcess,
	_WiSetRaw,
	_WiKeyReady,
	_WiSetServices,
	_WiMove,
	_WiResize,
	_WiService,
};

extern "C" {
	void WiConnect(const char *tty, const char *mod, const char *vers);
	void WiDisconnect(const char *tty);
	void WiFlush();
}

#endif

void WinMgr::execCmd()
{
#ifdef HAVE_IPC
	int msglen;
	static char buffaux[COM_BUFF_SIZE];
	struct msgbuf *msg = (struct msgbuf *) buffaux;

	do {
		// copio datos en el buffer auxiliar
		msg->mtype = my_pid;
		msglen = index-buffer;
		mCopy(msg->mtext, buffer, msglen);

		if (sendMsg(snd_channel,buffaux, msglen+sizeof(long))==ERROR) {
			stdMsg << libMsg("ERRSMSG") << endMsg;
			stop(1);
		}

		/* espero la respuesta del wm */

		/* si me interrumpen reitento	*/
		while((msglen=receiveMsg(rcv_channel, buffaux, COM_BUFF_SIZE
								  , (long)my_pid))==INTPTS && reint) 
			reint = FALSE;

	} while(msglen==INTPTS);

	if (msglen==ERROR && ncmd != WM_WIDELETEPRO) {
		stdMsg << libMsg("ERRRMSG") << endMsg;
		stop(1);
	}

	mCopy(buffer, msg->mtext, msglen);
	
#else
	WiFlush();
	commands[ncmd].fp();		
	paramNum = 0;
#endif
	// coloco el puntero del buffer en offset 0
	index = buffer;
}

static RETSIGTYPE dummy(int)
{
	sig_rcv = TRUE;
}

static ixBool drain(int msgid, int pid)
{
	int execrcv = FALSE;
	char msg[sizeof(long)];

	while(msgrcv(msgid, (struct msgbuf *) msg, 0, (long) pid, 
				 IPC_NOWAIT|MSG_NOERROR)!=ERROR)
		execrcv = TRUE;

	return execrcv; 
}

static  int sendchannel = ERROR;

static RETSIGTYPE onAlarm(int)
{
#ifdef HAVE_IPC
	assert (sendchannel != ERROR);
	if (drain(sendchannel, 1)) { 
		stdMsg << libMsg("CANTCOMM") << endMsg;
		stop(1);
	}
	reint = TRUE;
#endif
}

void WinMgr::connect(const char *tty, const char *module, 
									  const char *version)
{
#ifdef HAVE_IPC
	char *Argv[4];
	char buff[10];
	int pid, semid;

	if ((semid=openSem(tty, FALSE))==ERROR ||
		 	   !checkSem (semid)) {
		stdMsg << libMsg("WMNOTRUN") << endMsg;
		stop(1);
	}

	if ((snd_channel = openChannel(tty, 's', FALSE)) == ERROR) {
		stdMsg << libMsg("WMNOTRUN") << endMsg;
		stop(1);
	}

	if ((rcv_channel = openChannel(tty, 'r', FALSE)) == ERROR) {
		stdMsg << libMsg("WMNOTRUN") << endMsg;
		stop(1);
	}


	/* averiguo si soy hijo o no del WM */
	my_pid = 1;

	sendchannel = snd_channel;
	(void) SigSet(SIGALRM, onAlarm);

	alarm(TIMEOUT);

	/* no es hijo del WM */
	while (!createProces(getpid(), module, "Ideafix 4", version)) {

		(void) SigSet(SIGALRM, SIG_IGN);
		sendchannel = ERROR;

		if (String("SDB").getEnv() != NULL) {
			sig_rcv = TRUE;
			continue;
		}

		sig_rcv = FALSE;
		SigSet(SIGUSR2, dummy);

		/* si soy el padre -> ejecuto el proceso hijo del shell */
		if ((pid = fork()) != 0) {    

			if (pid == ERROR) { /* ERROR */ }	

			SigSet(SIGUSR2, SIG_IGN);

			Argv[0] = RETSHELL;
			strcpy(buff, toCharPtr(pid));
			Argv[1] = buff;
			Argv[2] = (char*)tty;
			Argv[3] = NULL;
			(void) execvp(Argv[0],Argv);
			/* ERROR */
			exit(1); // stop(255);
			
		}
		while (!sig_rcv) pause();
	}

	(void) SigSet(SIGALRM, SIG_IGN);
	sendchannel = ERROR;

	my_pid = getpid();

	/* Now define the interrupts handling & Messager funct. */
	(void)SigSet(SIGINT, SIG_IGN);
	(void)SigSet(SIGQUIT,SIG_IGN);
	(void)SigSet(SIGTERM,stop);
	(void)SigSet(SIGEMT, stop);   // deberia ser quit
#else
	WiConnect(tty, module, version);
#endif
}

static ixBool checkSem(int sid) 
{
#ifndef HAVE_IPC
	return TRUE;
#else
	struct sembuf x[1];

	x[0].sem_num = 0;
	x[0].sem_op =  0;
	x[0].sem_flg = IPC_NOWAIT;

	if (semop((int)sid, x, 1) == ERROR) {
		if (errno != EAGAIN) {
			stdMsg << libMsg("NOMQ") << endMsg;
			stop(1);
		}
		return TRUE;
	}
	return FALSE;
#endif
}

/* desconecta el proceso con el wm */
void WinMgr::disConnect(const char *tty)
{
#ifdef HAVE_IPC
	if (snd_channel == ERROR && rcv_channel == ERROR) 
		return; 

	(void) deleteProces(my_pid);
	snd_channel = rcv_channel = ERROR;
#else
	WiDisconnect(tty);
#endif
}

short WinMgr::createProces(int pid, const char *module, 
							const char *name, const char *version)
{
#ifndef HAVE_IPC
	return 0;
#else
	initFunc(WM_WICREATEPRO);
	push((long)pid);
	push((UnChar*)module);
	push((UnChar*)name);
	push((UnChar*)version);
	execCmd();
	short res; pop(res);
	return res;
#endif
}

short WinMgr::deleteProces(int pid)
{
#ifndef HAVE_IPC
	return 0;
#else
	initFunc(WM_WIDELETEPRO);
	push((long)pid);
	execCmd();
	short res; pop(res);
	return res;
#endif
}

#define MAXARGS 50
#define COD_ERR 10
#define MAX_CMD_LENGTH	2048

int WinMgr::execCommand(char *titulo, char *cmd)
{
	register char *p;
	int pid, argc, inquotes;
	int cond;			/*   condition   */
	char *argv[MAXARGS], aux[MAX_CMD_LENGTH+1];

	inquotes = 0;
	argc	 = 1;
	argv[0]  = p = strncpy(aux, cmd, MAX_CMD_LENGTH);
	aux[MAX_CMD_LENGTH] = 0;
	
	while ( *p && argc < MAXARGS ) {
		if (*p == (char)inquotes || (!inquotes && isspace((UnChar) *p)))
		{
			*p++ = 0;
			while (isspace((UnChar) *p)) ++p;
			if (*p == 0) break;
			if (*p == '\'' || *p == '"')
				inquotes = *p++;
			else 
				inquotes = 0;
			argv[argc++] = p;
		}
		else ++p;
	}

	argv[argc] = NULL;

	if ((pid=fork()) == ERROR) return ERROR;

	if (pid == 0) {
		execvp((char const *) argv[0], (char const * const *) argv);
		_exit(COD_ERR);
	}

	createProces(pid, argv[0], titulo, NULL);

	if (wait(&cond) == ERROR || (cond >> 8) == COD_ERR)
		cond = ERROR;

	deleteProces(pid);

	return cond;
}

void WinMgr::interrupts(ixBool flag)
{
	initFunc(WM_WIINTERRUPTS);
	push((short)flag);
	execCmd();
}

WinMgr::WinMgr(String tty, String module, String version)
{
	headwin = NULL;

#ifdef HAVE_IPC
	index = buffer;
#endif

	connect(toCharPtr(tty), toCharPtr(module), toCharPtr(version));

	if (defWm == NULL) defWm = this;

	if (head == NULL) {
		connectWinMsg();
		onStop(Window::delAll);
	}

	insNode();

	rootWindow = tr_new Window();
	rootWindow->win = SCREEN;
	rootWindow->wm  = this;

	currwin = ERROR;
}

WinMgr::~WinMgr()
{
	disConnect(NULL);

	if (this == defWm) defWm = NULL;

	delNode();

	if (head == NULL) disconnectWinMsg();

#ifdef HAVE_IPC
	index = buffer;				
	snd_channel = rcv_channel = ERROR;
#endif

	// borro la window root
	tr_delete(rootWindow); 

	rootWindow = headwin = NULL;
}

void WinMgr::refreshCurrWin()
{
	initFunc(WM_WICURRENT);
	execCmd();
	pop(currwin);
}

void WinMgr::setServices(ixBool val)
{
	initFunc(WM_WISETSERVICES);
	push((short) val);

	execCmd();
}

ixBool WinMgr::execService(const String &name)
{
	initFunc(WM_WISERVICE);
	push(name);

	execCmd();

	short res; pop(res);
	return res;
}

void WinMgr::setRaw(Int value)
{
	initFunc(WM_WISETRAW);
	push((short) value);

	execCmd();
}
