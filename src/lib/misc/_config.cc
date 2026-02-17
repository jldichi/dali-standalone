/********************************************************************
* Copyright (c) 1991 InterSoft Co.  All Rights Reserved
* THIS IS UNPUBLISHED PROPRIETARY SOURCE CODE OF InterSoft Co.
* The copyright notice above does not evidence any
* actual or intended publication of such source code.
*
* _config.cc,v 1.1.1.1 1996/04/16 18:51:59 gustavof Exp
*
* DESCRIPTION
* Configuration information (In MSWINDOWS are in dll)
*********************************************************************/
#include <signal.h>
#include <stdio.h>
#include <fcntl.h>
#include <sys/stat.h>
#include <errno.h>
#include <stdlib.h>
#include <string.h>


#include <ifound.h>
#include <ifound/start.h>
#include <ifound/stdc.h>
#include <ifound/msgtable.h>
#include <ifound/priv/gfuncs.h>
#include <ifound/priv/pconfig.h>
#include <ifound/priv/macros.h>

#ifdef __VC__
	extern "C" int fprintf(FILE *, const char *, ...);
#endif

#ifdef IDEAWIN
#	include <ixwi/wi.h>
#	include <mswi/ideadll.h>
#	undef stop
#	include <ideafix/priv/gndefs.h>
#endif
#define DATA_SUBDIR		"data"
#define DATADIR_SEPAR	PATH_SEP
#define MUST_STARTUP	"startUp function must be called.\n"

extern "C" {
 	extern int errno;
 	extern int sys_nerr;
#if !defined(IDEAWIN) && !defined(hpux) && !defined(AIX5)
 	extern const char *const sys_errlist[];
#else
 	extern char *sys_errlist[];
#endif
}

declare(PtrArray, char);
implement(PtrArray, char);

static InternatConfig *internatConfig = NULL;
static InternatConfig *_internatConfig();

static MsgTable *_libMsg = NULL;		// Library message table.
static MsgTable *_appMsg = NULL;		// Application message table.

extern "C" void SetDateFmt(int n);

extern "C" void SetDecPointToComma()
{
	internatConfig->dec_point = ',';
}

extern "C" void SetDecPointToPoint()
{
	internatConfig->dec_point = '.';
}

extern "C" void SetThousandSepToComma()
{
	internatConfig->thousand_sep = ',';
}

extern "C" void SetThousandSepPoint()
{
	internatConfig->thousand_sep = '.';
}

char decPoint()
{
	return _internatConfig()->dec_point;
}

char thousandSep()
{
	return _internatConfig()->thousand_sep;
}

DateFmt dateFmt()
{
	return _internatConfig()->date_fmt;
}

void dateFmt(DateFmt dfmt)
{
	_internatConfig()->date_fmt = dfmt;
}

const String &dayOfWeekName(Int d)
{
	if (d < 0 || d > 6)
		return NULL_STRING;
	return internatConfig->dayNames[d];
}

const String &monthName(Int m)
{
	if (m < 0 || m > 11)
		return NULL_STRING;
	return internatConfig->monthNames[m];
}

static int pres_flag = MSG_WAIT;

// handling routines

// display handler function pointer
static IFPICCPCCP display_handler = NULL;

// displayError handler function pointer
static IFPICCPCCP display_error_handler = NULL;

// fatalError handler function pointer
static IFPICCPCCP fatal_error_handler = NULL;

static IFPICCPCCP ari_error_handler = NULL;

static int execDisplay(int pres, const String &label, const String &msg)
{
	if (display_handler == NULL)
		return ERR;
	return display_handler(pres, toCharPtr(label), toCharPtr(msg));
}

static int execFatalError(int pres, const String &label, const String &msg)
{
	if (fatal_error_handler == NULL)
		return ERR;
	int ret = fatal_error_handler(pres, toCharPtr(label), toCharPtr(msg));
	stop(ABNORMAL_STOP|1);
	return ret;
}

static int execDisplayError(int pres, const String &label, const String &msg)
{
	if (display_error_handler == NULL)
		return ERR;
	return display_error_handler(pres, toCharPtr(label), toCharPtr(msg));
}

static int execAriError(int pres, const String &label, const String &msg)
{
	if (ari_error_handler == NULL)
		return ERR;
	return ari_error_handler(pres, toCharPtr(label), toCharPtr(msg));
}

void display(const String &label, const String &msg)
{
	execDisplay(pres_flag|MSG_DISPLAY, label, msg);
}

void displayError(const String &msg)
{
	execDisplayError(pres_flag|MSG_ERROR, libMsg("ERROR"), msg);
}

Int fatalError(const String &msg)
{
	return execFatalError(pres_flag|MSG_FATALERROR, libMsg("FATALERROR"),
						  msg);
}

//static 
IFPICCPCCP displayHandler(IFPICCPCCP handler)
{
	//static IFPICCPCCP oldh = display_handler;
	IFPICCPCCP oldh = display_handler;  // For error messages in Cracker & GIF
	display_handler = handler;
	return oldh;
}

//static 
IFPICCPCCP displayErrorHandler(IFPICCPCCP handler)
{
	//static IFPICCPCCP oldh = display_error_handler;
	IFPICCPCCP oldh = display_error_handler; // For error messages in Cracker & GIF
	display_error_handler = handler;
	return oldh;
}

//static 
IFPICCPCCP fatalErrorHandler(IFPICCPCCP handler)
{
	//static IFPICCPCCP oldh = fatal_error_handler;
	IFPICCPCCP oldh = fatal_error_handler;   // For error messages in Cracker & GIF
	fatal_error_handler = handler;
	return oldh;
}

//static 
IFPICCPCCP ariErrorHandler(IFPICCPCCP handler)
{
	//static IFPICCPCCP oldh = ari_error_handler;
	IFPICCPCCP oldh = ari_error_handler;  // For error messages in Cracker & GIF
	ari_error_handler = handler;
	return oldh;
}

void error(const String &fmt, ...)
{
	va_list ap ;
#ifdef HAVE_VARARGS_BUG
    const String *&s = &fmt;
    va_start(ap, s);
#elif HAVE_VC_VARARGS_BUG
	const char *s=toCharPtr(fmt);
    	va_start(ap, s + 12);
#else
    	va_start(ap, fmt);
#endif
	String msg = toStr(fmt, ap);
	va_end(ap);
	execFatalError(MSG_FATALERROR|pres_flag, libMsg("ERROR"), msg);
}

extern "C" void Error(const char *fmt, ...)
{
	va_list ap;
	va_start(ap, fmt);
	String msg = toStr(fmt, ap);
	va_end(ap);
	execFatalError(MSG_FATALERROR|pres_flag, libMsg("ERROR"), msg);
}

void notice(const String &fmt, ...)
{
	va_list ap;
#ifdef HAVE_VARARGS_BUG
    const String *&s= &fmt;
    va_start(ap, s);
#elif HAVE_VC_VARARGS_BUG
	const char *s=toCharPtr(fmt);
    	va_start(ap, s + 12);
#else
    va_start(ap, fmt);
#endif
	String msg = toStr(fmt, ap);
	va_end(ap);
	execDisplay(MSG_DISPLAY|pres_flag, libMsg("NOTICE"), msg);
}

extern "C" int Notice(const char *fmt, ...)
{
	va_list ap;
	va_start(ap, fmt);
	String msg = toStr(fmt, ap);
	va_end(ap);
	return execDisplay(MSG_DISPLAY|pres_flag, libMsg("NOTICE"), msg);
}

void warning(const String &fmt, ...)
{
	va_list ap;
#ifdef HAVE_VARARGS_BUG
    const String *&s = &fmt;
    va_start(ap, s);
#elif HAVE_VC_VARARGS_BUG
	const char *s=toCharPtr(fmt);
    	va_start(ap, s + 12);
#else
    va_start(ap, fmt);
#endif
	String msg = toStr(fmt, ap);
	va_end(ap);
	execDisplayError(MSG_DISPLAY|pres_flag, libMsg("WARNING"), msg);
}

extern "C" int Warning(const char *fmt, ...)
{
	va_list ap;
	va_start(ap, fmt);
	String msg = toStr(fmt, ap);
	va_end(ap);
	return execDisplayError(MSG_DISPLAY|pres_flag, libMsg("WARNING"), msg);
}

int ariError(const String &fmt, ...)
{
	va_list ap;
#ifdef HAVE_VARARGS_BUG
    const String *&s = &fmt;
    va_start(ap, s);
#elif HAVE_VC_VARARGS_BUG
	const char *s=toCharPtr(fmt);
    	va_start(ap, s + 12);
#else
    va_start(ap, fmt);
#endif
	String msg = toStr(fmt, ap);
	va_end(ap);
	return execAriError(MSG_ERROR|pres_flag, libMsg("ERROR"), msg);
}

#define MSGEXT	".msg"
FILE *openDataDirFile(const String &data, const String &language,
					  const String &file)
{
	for (StrCursor cur(data, DATADIR_SEPAR); cur; ++cur) {
		String cnffile = cur.value() + PATH_SLASH + language +
									   PATH_SLASH + file;
		FILE *fp = fopen(toCharPtr(cnffile), FOPEN_READ_MODE);
#ifdef __NT__
		if(!fp)
			fp =fopen(toCharPtr(cnffile.baseName()+MSGEXT), FOPEN_READ_MODE);
#endif
		if (fp != NULL)
			return fp;
	}
	return NULL;
}

extern "C" int AriError(const char *fmt, ...)
{
	va_list ap;
	va_start(ap, fmt);
	String msg = toStr(fmt, ap);
	va_end(ap);
	return execAriError(MSG_ERROR|pres_flag, libMsg("ERROR"), msg);
}

void msgCtrl(int flag)
{
	pres_flag = flag;
	if (flag & MSG_RELEASE) {
		execDisplay(MSG_RELEASE, NULL_STRING, NULL_STRING);
		pres_flag = MSG_WAIT;
	}
}

/*
	Contents of each line of the config file:

	comma separator
	decimal point
	date_fmt			(INTER | USA)
	sunday
	monday
	tuesday
	...
	...
	january
	february
	march
	...
	...
*/
void _loadConfig()
{
	char line[1024];
#ifdef IDEAWIN
	sDLL * pDLL = GetDLLData();
#endif

	// load config file
	String data		= String("DATADIR").getEnv();
	String language	= String("LANGUAGE").getEnv();
	FILE *fp	= NULL;
	if (!data.isNull())
		fp = openDataDirFile(data, language, "config");
	else {
		data = String("IDEAFIX").getEnv() + PATH_SLASH + DATA_SUBDIR;
		String cnffile = data + PATH_SLASH + language + PATH_SLASH + "config";
		fp = fopen(toCharPtr(cnffile), FOPEN_READ_MODE);
	}

	if (fp == NULL)
		return;

	Int i;
	DateFmt dfmt;

	if (fgets(line, sizeof(line), fp) == NULL)
		goto ret;
	line[strlen(line) - 1] = '\0';
	internatConfig->dec_point = line[0];
#ifdef IDEAWIN
	pDLL->dec_point=line[0];
#endif

	if (fgets(line, sizeof(line), fp) == NULL)
		goto ret;
	line[strlen(line) - 1] = '\0';
	internatConfig->thousand_sep = line[0];
#ifdef IDEAWIN
	pDLL->thousand_sep=line[0];
#endif
	if (fgets(line, sizeof(line), fp) == NULL)
		goto ret;
	line[strlen(line) - 1] = '\0';
	dfmt = String(line).toUpper() == "INTER" ? INTER_FMT : USA_FMT;
	internatConfig->date_fmt = dfmt;
	SetDateFmt(dfmt);

	// Read day of week names
	for (i = 0; i < 7; ++i) {
		if (fgets(line, sizeof(line), fp) == NULL)
			goto ret;
		internatConfig->dayNames[i] = String(line, strlen(line) - 1);
	}

	// Read month names
	for (i = 0; i < 12; ++i) {
		if (fgets(line, sizeof(line), fp) == NULL)
			goto ret;
		internatConfig->monthNames[i] = String(line, strlen(line) - 1);
	}

ret:

	fclose(fp);
}

InternatConfig::InternatConfig()
{
	dayNames[0] = "SUNDAY";
	dayNames[1] = "MONDAY";
	dayNames[2] = "TUESDAY";
	dayNames[3] = "WEDNESDAY";
	dayNames[4] = "THURSDAY";
	dayNames[5] = "FRIDAY";
	dayNames[6] = "SATURDAY";

	monthNames[0] = "JANUARY";
	monthNames[1] = "FEBRUARY";
	monthNames[2] = "MARCH";
	monthNames[3] = "APRIL";
	monthNames[4] = "MAY";
	monthNames[5] = "JUNE";
	monthNames[6] = "JULY";
	monthNames[7] = "AUGUST";
	monthNames[8] = "SEPTEMBER";
	monthNames[9] = "OCTOBER";
	monthNames[10] = "NOVEMBER";
	monthNames[11] = "DECEMBER";

	dec_point = '.';
	thousand_sep = ',';
#ifdef IDEAWIN
	sDLL * pDLL = GetDLLData();
	pDLL->dec_point = '.';
	pDLL->thousand_sep = ',';
#endif
	date_fmt = INTER_FMT;
}

String libMsg(const String &msg)
{
	assert(_libMsg != NULL);
	return _libMsg->find(msg);
}

String appMsg(const char *message)
{
	assert(_appMsg != NULL);
	return _appMsg->find(message) ;
}

String appMsg(const String &message)
{
	assert(_appMsg != NULL);
	return _appMsg->find(message);
}

String libMsg(const char *msg)
{
	assert(_libMsg != NULL);
	return _libMsg->find(msg);
}

static InternatConfig *_internatConfig()
{
	if (internatConfig == NULL) {
#ifndef IDEAWIN
		fprintf(stderr, MUST_STARTUP);
#endif
		_global_exit(1);
	}
	return internatConfig;
}

void _initInternalConfig()
{
	internatConfig = tr_new InternatConfig;
}

void _initMsgTables()
{
	_libMsg = tr_new MsgTable("lib");
	_appMsg = tr_new MsgTable(appName());
}

#ifndef IDEAWIN
static int xputc(int ch, FILE *stream)
{
	return fputc(toascii(ch), stream);
}

static int handlerDefault(int pres, const char *, const char *msg)
{
	if (!(pres & MSG_RELEASE)) {
		doFormat(IFPICP(xputc), (char *) stderr, msg, NULL);
		fputc('\n', stderr);
	}
	return ERR;
}
#else
static int handlerDefault(int pres, const char *, const char *msg)
{
	if (!(pres & MSG_RELEASE))

		MessageBox(NULL,msg,toCharPtr(libMsg("ERROR")),MB_OK|MB_ICONSTOP);
	return ERR;
}
#endif

// default handler for abort, retry or ignore errors
static int ariHandlerDefault(int pres, const char *label, const char *msg)
{
	static bool aborting = false;
#ifdef IDEAWIN
	wdflag x;
#else
	int x;
#endif
	if (aborting)
		return false;

	if (!(pres & MSG_RELEASE)) {
#ifndef IDEAWIN
		String tmp = libMsg("AB_RE_IG");
		doFormat(IFPICP(xputc), (char *) stderr, msg, NULL);
		fputc('\n', stderr);
		fprintf(stderr, "%S", &tmp);
		fflush(stderr);
		x = toupper(fgetc(stdin));
		if (x != '\n' || x != '\r') {
			int ch;
			while ((ch = fgetc(stdin)) != '\n' && ch != '\r')
				;
		}

		if (x == 'A') {
			aborting = true;
			stop(errno|ABNORMAL_STOP);
		}

		return !(x == 'I');
#else
		x = WiVDialog(WD_ABORT|WD_RETRY|WD_IGNORE,WD_RETRY, label,
					  (char *) msg, NULL);
		if (x == WD_ABORT) {
			aborting = true;
			stop(errno|ABNORMAL_STOP);
		}
		return !(x == WD_IGNORE);
#endif
	}
	return 0;
}

void _initHandlers()
{
	// default handlers
	display_handler = handlerDefault;
	display_error_handler = handlerDefault;
	fatal_error_handler = handlerDefault;
	ari_error_handler = ariHandlerDefault;
}

#ifdef IDEAWIN
const String &sysName()
{
	static String sName = NULL_STRING;
	sDLL * pDLL = GetDLLData();
	sName = pDLL->sysName; 	// I must do this every time, because I'm in dll
							// toDo - put all this in dll struct
	return sName;
}

const String &appName()
{
	static String appName = NULL_STRING;
	sDLL * pDLL = GetDLLData();
	appName =pDLL->ModuleName;
	return appName;
}

void setSysName(String sysName)
{
	sDLL * pDLL = GetDLLData();
	strncpy(pDLL->sysName,toCharPtr(sysName),SYSNAMELEN-1);
	pDLL->sysName[SYSNAMELEN] = '\0';
}

#endif
