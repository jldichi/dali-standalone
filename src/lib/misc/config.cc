/********************************************************************
* Copyright (c) 1991 InterSoft Co.  All Rights Reserved
* THIS IS UNPUBLISHED PROPRIETARY SOURCE CODE OF InterSoft Co.
* The copyright notice above does not evidence any
* actual or intended publication of such source code.
*
* config.cc,v 1.1.1.1 1996/04/16 18:51:58 gustavof Exp
*
* DESCRIPTION
* Configuration information
*********************************************************************/
#include <signal.h>
#include <stdio.h>
#include <fcntl.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <unistd.h>
#include <errno.h>
#include <string.h>
#include <stdlib.h>

#include <ifound.h>
#include <ifound/start.h>
#include <ifound/stdc.h>
#include <ifound/msgtable.h>
#include <ifound/priv/gfuncs.h>
#include <ifound/priv/pconfig.h>
#ifdef __VC__
	extern "C" int fprintf(FILE *, const char *, ...);
#endif

#ifdef HAVE_UNDERSCORE_NAMES
#	define	read	_read
#	define	stat	_stat
#	define	fstat	_fstat
#endif

#ifdef IDEAWIN
extern "C" int WInitApp();
extern void setSysName(String);
#endif

#ifdef __NTSERVER__
static void setNTsig();
#endif
extern void _initInternalConfig();
extern void _initMsgTables();
extern void _loadConfig();
extern void	_initHandlers();

static bool abortOnExit = false;
static ProcessConfig *processConfig = NULL;
static void setupSignals();
static void catchFd60();

// +++ Prototypes +++


// --- Prototypes ---


// +++ Config +++

#define ARG_REALLOC		8
#define MUST_STARTUP	"startUp function must be called.\n"
#define PFA_LINEFMT		"%s\t%s\t%s\t%d\t%s\t%d\t%s\t%s\n"

declare(PtrArray, char);
implement(PtrArray, char);

// --- Config ---

// process config routines
void procUpdatePerm(bool v)
{
	_processConfig()->update = v;
}

bool procUpdatePerm()
{
	return _processConfig()->update;
}

void procAddPerm(bool v)
{
	_processConfig()->add = v;
}

bool procAddPerm()
{
	return _processConfig()->add;
}

void procDelPerm(bool v)
{
	_processConfig()->del = v;
}

bool procDelPerm()
{
	return _processConfig()->del;
}

#if HAVE_SAVEMEM
bool procSaveMem()
{
	return _processConfig()->savemem;
}
#endif // HAVE_SAVMEM

void setProcUserId(int uid)
{
	_processConfig()->userid = uid;
}

void setProcUserGid(int gid)
{
	_processConfig()->usergid = gid;
}

int procUserId()
{
	return _processConfig()->userid;
}

int procUserGid()
{
	return _processConfig()->usergid;
}

Int procPid()
{
	return _processConfig()->pid;
}

const String &procTtyName()
{
	return _processConfig()->ttyName;
}

char **procArgs()
{
	return _processConfig()->argv;
}

Int procSig()
{
	return _processConfig()->sig;
}

Time procInitTime()
{
	return _processConfig()->initTime;
}

Date procInitDate()
{
	return _processConfig()->initDate;
}

const String &procTty()
{
	return _processConfig()->ttyWm;
}

const String &getTty()
{
	return _processConfig()->ttyName;
}

#ifndef IDEAWIN
const String &sysName()
{
	return _processConfig()->sysName;
}

const String &appName()
{
	return _processConfig()->name;
}
#endif

Int getPid()
{
	return _processConfig()->pid;
}

Int argCnt()
{
	return _processConfig()->argc;
}

char **argRefVal()
{
	return _processConfig()->argv;
}

const int &trgRefCnt()
{
	return _processConfig()->argc;
}

String argVal(Int n)
{
	if (n < 0 || n >= _processConfig()->argc)
		return NULL_STRING;
	return _processConfig()->argv[n];
}

// this functions are called by signal handlers

static RETSIGTYPE stopThis(int sig)
{
	stop(sig | ABNORMAL_STOP);
}

static RETSIGTYPE abortHand(int sig)
{
#if HAVE_SETUID
	setuid(geteuid());
#endif
	SigSet(sig, SIG_DFL);
	abortOnExit = true;
	_processConfig()->sig = sig;
	error(libMsg("INT_SIG"), toCharPtr(_processConfig()->name), sig);
}

static RETSIGTYPE brokenPipe()
{
	error(libMsg("BROKPIPE"), toCharPtr(_processConfig()->name));
}

// setup signal handlers

static void setupSignals()
{
#if HAVE_FULL_SIGNALS

	// Set default signals

	// hangup
	SigSet(SIGHUP, stopThis);

	// interrupt (rubout)
	if (SigSet(SIGINT, stopThis) == SigHandlerT(SIG_IGN))
		SigSet(SIGINT, SIG_IGN);

	// quit (ASCII FS)
	SigSet(SIGQUIT, abortHand);

	// illegal instr.
	SigSet(SIGILL, abortHand);

	// trace trap (not reset when caught)
	SigSet(SIGTRAP, abortHand);

	// used by abort, replace SIGIOT
#	ifdef HAVE_SIGABRT
	SigSet(SIGABRT, SIG_DFL);
#	else
	SigSet(SIGIOT, SIG_DFL);
#	endif

#ifdef HAVE_SIGEMT
	// EMT instruction
	SigSet(SIGEMT, abortHand);
#endif

#ifdef HAVE_SIGUNUSED

	/* Ya que el glibc 2.26 SIGUNUSED ha sido eliminado 
       y se nos aconseja usar SIGSYS en su lugar (que 
       tiene el mismo valor) */

#	ifndef SIGUNUSED
		SigSet(SIGSYS, abortHand);
#	else
		SigSet(SIGUNUSED, abortHand);
#	endif

#endif

	// floating point exception
	SigSet(SIGFPE, abortHand);

	// bus error
	SigSet(SIGBUS, abortHand);

	// segmentation violation
	SigSet(SIGSEGV, abortHand);

#ifdef HAVE_SIGSYS
	// bad argument to system call
	SigSet(SIGSYS, abortHand);
#endif

	// write on a pipe with no one to read from it
	SigSet(SIGPIPE, brokenPipe);

	// alarm clock
	SigSet(SIGALRM,	SIG_IGN);

	// software termination signal from kill
	SigSet(SIGTERM,	stopThis);

	SigSet(SIGUSR1, SIG_IGN);		// user defined signal 1
	SigSet(SIGUSR2, SIG_IGN);		// user defined signal 2
	SigSet(SIGCLD, SIG_DFL);		// death of a child

#	ifdef HAVE_SIGPWR
	SigSet(SIGPWR, SIG_DFL);		// power-fail restart
#	endif

#endif
#ifdef __NTSERVER__
	setNTsig();
#endif
}

static char *findNext(char *p, char *&var)
{
	while (*p && isspace(*p))
		p++;

	if (*p) {
		if (*p == '\"') {
			var = ++p;
			while (*p && *p != '\"')
				p++;
		} else {
			var = p;
			while (*p && !isspace(*p))
				p++;
		}

		*p = '\0';
		return p+1;
	} else
		return 0;
}

static PtrArray(char) *argv()
{
	static PtrArray(char) *argv_ = NULL;

	if (argv_ == NULL)
		argv_ = tr_new PtrArray(char)(1, ARG_REALLOC);

	return argv_;
}

static void readArgs(const String &file, const char *nm, int &ac,
		char **&av)
{
	int i = 1;

	int fd = _global_open(toCharPtr(file), O_RDONLY,0666);

	if (fd == ERR)
		error(toString(libMsg("CANTOPENARGF"), toCharPtr(file)));

	struct stat st;
	if (fstat(fd, &st) == ERR)
		error(toString(libMsg("CANTOPENARGF"), toCharPtr(file)));

	char *buff = tr_new char[st.st_size + 1];
	char *p;

	// read in the whole file
	if (_global_read(fd, buff, st.st_size) != st.st_size)
		error(toString(libMsg("CANTOPENARGF"), toCharPtr(file)));

	_global_close(fd);

	// Make sure we stop
	buff[st.st_size] = '\0';

	p = buff;
	char *str = NULL;
	argv()->add(nm);
	while ((p = findNext(p, str))) {
		argv()->add(str);
		i++;
	}
	ac = i;
	av = &argv()->elem(0);
}

ProcessConfig::ProcessConfig()
{
	// assign defaults
	name = NULL_STRING;
	argc = 0;
	argv = NULL;
	pid = 0;
	userid = 0;
	usergid = 0;
	add = true;
	del = true;
	update = true;
	background = false;
	sig = 0;
	ttyName = NULL_STRING;
	ttyWm = NULL_STRING;
#if HAVE_SAVEMEM
	savemem = false;
#endif
	conn = NULL;
	appConn_d = NULL;
	netDelay = ERR;
	showNMeth = false;
}

struct FuncSlot {
	FP function;
	FuncSlot *next;
};

static FuncSlot *funcTab = NULL;
static FuncSlot *abFuncTab = NULL;

GUIConnection *_appConnection()
{
	return _processConfig()->appConn_d;
}

GUIConnection *appConnection()
{
	GUIConnection *conn = _processConfig()->appConn_d;
	assert(conn);
	return conn;
}

void setArgs(int ac, char **av)
{
	_processConfig()->argc = ac;
	_processConfig()->argv = av;
}

#if defined (SHIDEAIX)
	class libideaix_s {
#elif defined (SHIDEAES)
	class libideaes_s {
#elif defined (SAMENAMESHD)
	class libidea_s {
#elif defined(_DLL)
	#ifdef __NTIDEA__
		class ntidea {
	#elif defined(IDEAWIN)
		class libidea {
	#else
		class libidead {
	#endif
#else
	class libidea {
#endif
public:
	static void foo()
	{
		;
	}
};

static void foolink()
{
#if defined (SHIDEAIX)
	libideaix_s::foo();
#elif defined (SHIDEAES)
	libideaes_s::foo();
#elif defined (SAMENAMESHD)
	libidea_s::foo();
#elif defined (_DLL)
	#ifdef __NTIDEA__
		ntidea::foo();
	#elif defined(IDEAWIN)
		libidea::foo();
	#else
		libidead::foo();
	#endif
#else
	libidea::foo();
#endif
}

bool dummycall = false;

void startUp(const String &fName, int &argc, char **&argVal)
{
#ifdef HAVE_FDOPENBUG
	static int fd60 = 0;
	if (fd60 == 0) {
		catchFd60();
		fd60 = 1;
	}
#endif // HAVE_FDOPENBUG

	foolink();
	processConfig = tr_new ProcessConfig;
	_initInternalConfig();

	// get application and sys names
	// The fName format is 'system_name:app_name'
	const String aName = fName.field(1, SYSNAME_SEPAR);
	if (aName != NULL_STRING) {
#ifdef __NT__
		processConfig->name		= aName.replace(PATH_SLASH,PATH_SLASH_UNIV).baseFullName();
#else
		processConfig->name		= aName.baseFullName();
#endif
		processConfig->sysName	= fName.field(0, SYSNAME_SEPAR);
	} else {
		processConfig->name		= fName.baseFullName();
		processConfig->sysName	= NULL_STRING;
	}

	// Divided in 3 assignments 'cause of a compiler bug
	processConfig->add = true;
	processConfig->del = true;
	processConfig->update = true;

 	// Set Privileges.  Adjust argc & argv.
	char **argv;
	for (argv = argVal + 1; argc > 1 && **argv == '!'; --argc, ++argv) {

		for (const char *aux = *argv + 1; *aux; ++aux) {

		 	switch (*aux) {
			case 'a':
			case 'A':   // ADD
			case 'i':
			case 'I':   // INSERT
				processConfig->add = false;
				break;
			case 'u':
			case 'U':
				processConfig->update = false;
				break;
			case 'd':
			case 'D':
				processConfig->del = false;
				break;
			case 't':
			case 'T':
				if (argv[0][2])
					processConfig->ttyWm = &argv[0][2];
				else {
					processConfig->ttyWm = *++argv;
					argc--;
				}
				break;
			case 'e':
			case 'E':
				if (argv[0][2])
					setEnvironment(&argv[0][2], true);
				else {
					setEnvironment(*++argv, true);
					argc--;
				}
				break;
			case 'm':
			case 'M':
				processConfig->showNMeth = true;
				break;
			}
		}
	}

	if (argv != argVal + 1) {
	    for (Int i = 0; i < argc; i++)
    		argVal[i+1] = argv[i];
	}

	_initMsgTables();

	processConfig->argv = argVal;
	processConfig->argc = argc;

	processConfig->background = false;
	processConfig->initTime = Time(NOW);
	processConfig->initDate = Date(TODAY);

#ifndef HAVE_TTY
	processConfig->ttyName = "console";
#else
	char *tname;

	if ((tname = ttyname(0)))
		processConfig->ttyName = tname;
#endif

#if HAVE_USERS
	processConfig->pid = getpid();
	processConfig->userid = geteuid();
	processConfig->usergid = getegid();
#endif

	// default handlers
	_initHandlers();

	setupSignals();

	// read arguments from specified file
	if (processConfig->argc > 1 && processConfig->argv[1][0] == '@') {
		readArgs(processConfig->argv[1]+1, processConfig->argv[0],
				processConfig->argc, processConfig->argv);
	}

	argc = processConfig->argc;
	argVal = processConfig->argv;

	_loadConfig();

#ifdef IDEAWIN
	setSysName(processConfig->sysName);
	WInitApp();
#endif
}

#if HAVE_FULL_SIGNALS

/*
	Date
	Initial time
	Final time
	Process id
	Process name
	Signal
	User id
	Tty name
*/
#if HAVE_UNISTD_H
#	include <unistd.h>
#endif

static void processFailure(int exitCode)
{
	char aux[160];
	char dateInit[10];
	char timeInit[10];
	char timeDeath[10];
	int pfa;
	const char *pfaFile = readEnvVar("PFA");

	// If the env. var. PFA is not defined or has nothing in it.
	if (!pfaFile || !(*pfaFile))
		aux[0] = '\0';
	else
		sprintf(aux, "%${IDEAFIX}/adm/%s.pfa", pfaFile);

	if (!aux[0])
		return;

	pfa = _global_open(aux, O_RDWR|O_APPEND|O_CREAT, 0666);

	if (pfa == -1)
		return;

	// Lock exclusively
	if (lockFile(pfa, true, false, 0, 0) == ERR) {
		_global_close(pfa);
		return;
	}

	// Write the information
	strcpy(timeDeath, toCharPtr(Time(NOW)));
	strcpy(timeInit, toCharPtr(_processConfig()->initTime));
	strcpy(dateInit, toCharPtr(_processConfig()->initDate));
	sprintf(aux, PFA_LINEFMT, dateInit, timeInit, timeDeath,
			_processConfig()->pid, _processConfig()->argv[0],
			_processConfig()->sig, userName(_processConfig()->userid),
			toCharPtr(_processConfig()->ttyName));

	_global_write(pfa, aux, strlen(aux));
	_global_close(pfa);
}
#endif

void onStop(FP func)
{
	register FuncSlot *fp;

	// search for duplicates

	for (fp = funcTab; fp; fp = fp->next) {
		// was function already there?
		if (fp->function == func)
			return;
	}

	fp = tr_new FuncSlot;
	fp->function = func;
	fp->next = funcTab;
	funcTab = fp;
}

void onAbnormalStop(FP func)
{
	FuncSlot *fp;

	// search for duplicates

	for (fp = abFuncTab; fp; fp = fp->next) {
		// was function already there?
		if (fp->function == func)
			return;
	}

	fp = tr_new FuncSlot;
	fp->function = func;
	fp->next = abFuncTab;
	abFuncTab = fp;
}

void DLLEXPORT _stop(int val)
{
	static bool inStop = false;

	// avoid nested calls
	if (inStop) return;
	inStop = true;

	bool abNormalStop = val & ABNORMAL_STOP;
	int exitCode = int(val & 0xFF);
	if (abNormalStop && exitCode == 0)
		exitCode = 1;

	// abnormal termination ...
	if (abNormalStop) {
		for (FuncSlot *fp = abFuncTab; fp; fp = fp->next)
			fp->function();
	}

	// normal termination ...
	for (FuncSlot *fp = funcTab; fp; fp = fp->next)
		fp->function();

#if HAVE_FULL_SIGNALS
	/*
		If exit value informs error & the process executed
		the Start() function, then we add the process to the
		process failure accounting file.
	*/
	if (abNormalStop)
		processFailure(exitCode);
#endif

	if (abortOnExit)
		_global_abort();
	else
		_global_exit(exitCode);
}

void turnIntoDaemon(IFPICCPCCP fatalHandler, IFPICCPCCP dispErrHandler,
					IFPICCPCCP dispHandler)
{
#ifdef HAVE_FULL_SIGNALS
	Int processid;
	if ((processid = fork()) < 0)
		error(libMsg("CANTFORK1ST"));
	else if (processid != 0)
		::stop(0);

#	ifdef HAVE_SETSID
	setsid();
#	else
	if (setpgrp() == ERR)
		error(libMsg("CANTCHGPGRP"));
	SigSet(SIGHUP, SIG_IGN);
	if ((processid = fork()) < 0)
		error(libMsg("CANTFORK2ND"));
	else if (processid != 0)
		::stop(0);
#	endif
	_processConfig()->pid = getpid();
#endif

	// This could have problems on some machines, if you have any
	// problems just comment these close()'s.
#ifdef HAVE_NOPROBLEMSCLOSING012
	_global_close(0);
	_global_close(1);
	_global_close(2);
#endif

	chdir(PATH_SLASH);

#ifdef HAVE_UMASK
	umask(0);
#endif

	if (fatalHandler != NULL)
		fatalErrorHandler(fatalHandler);

	if (dispErrHandler != NULL)
		displayErrorHandler(dispErrHandler);

	if (dispHandler != NULL)
		displayHandler(dispHandler);
}

ProcessConfig *_processConfig()
{
	if (processConfig == NULL) {
#ifndef IDEAWIN
		fprintf(stderr, MUST_STARTUP);
#else
//	Wprintf("Must strartUp");
#endif
		_global_exit(1);
	}
	return processConfig;
}

extern "C" bool crackerClient()
{
	return _processConfig()->conn != NULL;
}

extern "C" bool CrackerClient()
{
	return crackerClient();
}

#ifdef HAVE_FDOPENBUG
extern "C" void CatchFd60()
{
	catchFd60();
}

static
void
catchFd60()
{
	int f = ERR;
	int ftab[60];
	int i = 0;
	char buffer[100];

	// Look for file descriptor. Once reached, capture it and NEVER
	// leave it. We have to make sure the process can open more than
	// 60 files. EMFILE is errno msg for TOO_MANY_OPENED_FILES.
	errno = 0;
	tmpnam(buffer);
	f = _global_open(buffer, 0400, 0666);
	const int oldf = f;

	for (i=0; i < 61 && errno != EMFILE; i++) {
		ftab[i] = dup(f);
		if (ftab[i] < 0 && errno != EMFILE) {
			fprintf(stderr, "Error opening temporary file (%s) (%ld). errno = (%ld)\n", __FILE__, __LINE__, errno);
			_global_exit(-1);
		}
		if (ftab[i] == 60) {
			fprintf(stderr, "This file descriptor (%d) shouldn't be opened.\n", ftab[i]);
			_global_exit(-1);
		}
		if (ftab[i] == 59) {
			mode_t prevmask = umask(0);
			errno = 0;
			int p = _global_open("/tmp/60.err", 0400, 0666);
			if (p < 0 && errno != EMFILE) {
				fprintf(stderr, "Couldn't open file /tmp/60.err when trying to catch file descriptor 60. errno = (%ld)\n", errno);
				_global_exit(-1);
			}
			umask(prevmask);
			break;
		}
	}

	int top = i+1;
	// Free the opened files, except fd = 60.
	for (i=0; i < top; i++) {
		if (ftab[i] > 0 && ftab[i] == 60) {
			fprintf(stderr, "This file descriptor shouldn't be closed.\n");
			_global_exit(-1);
		}
		if (ftab[i] > 0)
			_global_close(ftab[i]);
	}

	_global_close(oldf);
	unlink(buffer);
}
#endif // HAVE_FDOPENBUG

#ifdef __NTSERVER__
#undef CreateCursor
#undef ClosePrinter
#include <windows.h>
extern "C" bool EndTransaction();
extern "C" void CloseAllSchemas();

static BOOL signal_handler(DWORD CtrlType)
{
	if (CtrlType == CTRL_LOGOFF_EVENT)
		return TRUE;
	EndTransaction();
	if(CtrlType != CTRL_C_EVENT )
		ExitProcess(9);

	CloseAllSchemas();
	ExitProcess(15);
	return 0;
}

static void setNTsig()
{
	SetConsoleCtrlHandler((PHANDLER_ROUTINE) signal_handler,TRUE);
}

#endif
// end of config
