/********************************************************************
* Copyright (c) 1992 InterSoft Co.  All Rights Reserved
* THIS IS UNPUBLISHED PROPRIETARY SOURCE CODE OF InterSoft Co.
* The copyright notice above does not evidence any
* actual or intended publication of such source code.
*
*
* $Id: slog.h,v 1.3 1997/08/05 16:12:41 guiller Exp $
*
*
* DESCRIPTION:
*				Class definitions for SysLog Type.
*
*********************************************************************/

#ifndef SYSLOG_H
#define SYSLOG_H

#ifdef __GNUC__
#pragma interface
#endif

#include <ifound.h>

#ifndef STR_H
#	include <ifound/str.h>
#endif

class SysLog
{
#ifdef __NT__
	void * hev; // HANDLE
#endif
	String aplname;

public:

	// constructor y destructor
	explicit SysLog(const String &name, bool reserved = false);
	SysLog(const SysLog &sl)  { *this = sl; };
	~SysLog();

	SysLog &operator=(const SysLog &sl);

	// rutinas que envian los mensajes
	void logError(const String &msg);
	void logWarning(const String &msg);
	void logAlert(const String &msg);
	void logDebug(const String &msg);
	void logInfo(const String &msg);
	void logNotice(const String &msg);
};

#ifdef __NT__
extern "C" void sysWarning(char * appname, char *fmt, ...);
extern "C" void sysError(char * appname, char *fmt, ...);
extern "C" void sysInfo(char * appname, char *fmt, ...);
#endif

#endif // SYSLOG_H
