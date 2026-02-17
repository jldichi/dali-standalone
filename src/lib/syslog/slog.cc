/********************************************************************
* Copyright (c) 1992 InterSoft Co.  All Rights Reserved
* THIS IS UNPUBLISHED PROPRIETARY SOURCE CODE OF InterSoft Co.
* The copyright notice above does not evidence any
* actual or intended publication of such source code.
*
* $Id: slog.cc,v 1.9 2007/03/30 19:39:14 hardaiz Exp $
*
* DESCRIPTION
*********************************************************************/

#if defined(SunSolx86)
#include <syslog.h>
#endif

#ifdef __GNUC__
#pragma implementation
#endif
#include <local/slog.h>
#include <ifound/str.h>
#include <ifound/stdc.h>
#include <ifound/priv/gfuncs.h>

#ifdef HAVE_SYSLOG

#ifndef __NT__
#	ifdef HAVE_SYSLOG_HP
#	include <syslog.h>
#	else
#	include <sys/syslog.h>
#	endif
#include <stdarg.h>
#else
#	include <windows.h>
#	include <windowsx.h>
#endif

SysLog::SysLog(const String &name, bool reserved)
: aplname(name)
{
#ifndef __NT__
	openlog(toCharPtr(aplname), LOG_PID, reserved ? LOG_LOCAL0 : LOG_USER);
#else
	(HANDLE) hev=RegisterEventSource(NULL,toCharPtr(aplname));
#endif
}

SysLog::~SysLog()
{
#ifndef __NT__
	closelog();
#else
	CloseHandle((HANDLE) hev);
#endif
}

SysLog &SysLog::operator=(const SysLog &sl)
{
   if ( this != &sl )
        aplname = sl.aplname;

   return *this;
}

void SysLog::logError(const String &msg)
{
#ifndef __NT__
	syslog(LOG_ERR, toCharPtr(msg));
#else
	char *str[2];
	str[0]=toCharPtr(msg);
	str[1]=NULL;
	if(hev != NULL )
		ReportEvent(
			(HANDLE) hev,
			EVENTLOG_ERROR_TYPE,
			1,
			1,
			NULL,
			1,
			0,
			(const char**) str,
			NULL);
#endif
}

void SysLog::logWarning(const String &msg)
{
#ifndef __NT__
	syslog(LOG_WARNING, toCharPtr(msg));
#else
	char *str[2];
	str[0]=toCharPtr(msg);
	str[1]=NULL;
	ReportEvent((
		HANDLE) hev,
		EVENTLOG_WARNING_TYPE,
		1,
		1,
		NULL,
		1,
		0,
		(const char**) str,
		NULL);
#endif
}

void SysLog::logAlert(const String &msg)
{
#ifndef __NT__
	syslog(LOG_ALERT, toCharPtr(msg));
#else
	char *str[2];
	str[0]=toCharPtr(msg);
	str[1]=NULL;
	ReportEvent(
		(HANDLE) hev,
		EVENTLOG_WARNING_TYPE,
		2,
		1,
		NULL,
		1,
		0,
		(const char**) str,
		NULL);
#endif
}

void SysLog::logDebug(const String &msg)
{
#ifndef __NT__
	syslog(LOG_DEBUG, toCharPtr(msg));
#else
	char *str[2];
	str[0]=toCharPtr(msg);
	str[1]=NULL;
	ReportEvent((
		HANDLE) hev,
		EVENTLOG_INFORMATION_TYPE,
		2,
		1,
		NULL,
		1,
		0,
		(const char**) str,
		NULL);
#endif
}

void SysLog::logInfo(const String &msg)
{
#ifndef __NT__
	syslog(LOG_INFO, toCharPtr(msg));
#else
	char *str[2];
	str[0]=toCharPtr(msg);
	str[1]=NULL;
	ReportEvent(
		(HANDLE) hev,
		EVENTLOG_INFORMATION_TYPE,
		1,
		1,
		NULL,
		1,
		0,
		(const char**) str,
		NULL);
#endif
}

void SysLog::logNotice(const String &msg)
{
#ifndef __NT__
	syslog(LOG_NOTICE, toCharPtr(msg));
#else
	char *str[2];
	str[0]=toCharPtr(msg);
	str[1]=NULL;
	ReportEvent(
		(HANDLE) hev,
		EVENTLOG_INFORMATION_TYPE,
		3,
		1,
		NULL,
		1,
		0,
		(const char **) str,
		NULL);
#endif
}

#ifdef __NT__

// Extern functions to write formated strings.

static char lbuf[1024] = {0};

static int spsub(int c, char **str)
{
	return (*(*str)++ = (char) c) & 0xff;
}

extern "C" void sysWarning(char * appname, char *fmt, ...)
{
	int err = GetLastError();
	va_list ap;
	char *str = lbuf;
	va_start(ap, fmt);
	(void) doFormat((IFPICP)spsub, (char *) &str, (char *)fmt, ap);
	*str = '\0';
	SysLog syslog(appname);
	syslog.logWarning(lbuf);
	va_end(ap);
	SetLastError(err);
}


extern "C" void sysInfo(char * appname, char *fmt, ...)
{
	int err = GetLastError();
	va_list ap;
	char *str = lbuf;
	va_start(ap, fmt);
	(void) doFormat((IFPICP)spsub, (char *) &str, (char *)fmt, ap);
	*str = '\0';
	SysLog syslog(appname);
	syslog.logInfo(lbuf);
	va_end(ap);
	SetLastError(err);
}

extern "C" void sysError(char * appname, char *fmt, ...)
{
	int err = GetLastError();
	va_list ap;
	char *str = lbuf;
	va_start(ap, fmt);
	(void) doFormat((IFPICP)spsub, (char *) &str, (char *)fmt, ap);
	*str = '\0';
	SysLog syslog(appname);
	syslog.logError(lbuf);
	va_end(ap);
	SetLastError(err);
}
#endif

#else 	// HAVE_SYSLOG

SysLog::SysLog(const String &name, bool reserved)
: aplname(name)
{
}

SysLog::~SysLog()
{
}

SysLog &SysLog::operator=(const SysLog &sl)
{ 
   if ( this != &sl )
        aplname = sl.aplname;
        
   return *this;     
}
		
void SysLog::logError(const String &msg)
{
	warning("Log error: %s", toCharPtr(msg));
}

void SysLog::logWarning(const String &msg)
{
	warning("Log warning: %s", toCharPtr(msg));
}

void SysLog::logAlert(const String &msg)
{
	warning("Log alert: %s", toCharPtr(msg));
}

void SysLog::logDebug(const String &msg)
{
	warning("Log debug: %s", toCharPtr(msg));
}

void SysLog::logInfo(const String &msg)
{
	warning("Log info: %s", toCharPtr(msg));
}

void SysLog::logNotice(const String &msg)
{
	warning("Log notice: %s", toCharPtr(msg));
}
#endif
