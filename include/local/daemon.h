/********************************************************************
*
* Copyright (c) 1995 InterSoft Co.  All Rights Reserved
* THIS IS UNPUBLISHED PROPRIETARY SOURCE CODE OF InterSoft Co.
* The copyright notice above does not evidence any
* actual or intended publication of such source code.
*
* $Id: daemon.h,v 1.2 1996/04/18 12:28:10 diego Exp $
*
* DESCRIPTION:
*
*********************************************************************/
#ifndef DAEMON_H
#define DAEMON_H

#include <ifound.h>
#include <local/configf.h>

class SysLog;

class Daemon {

	friend class SignalAction;

private:

	// Error, Warning and Notice routines
	static SysLog *sysLog;
	static void createSysLog();
	static int logError(int , const char *, const char *);
	static int logNotice(int , const char *, const char *);
	static int logWarning(int , const char *, const char *);


protected:

	Configuration *config;

	virtual void configure() { if (config) config->set(); }
	virtual void stop() { ::stop(0); }

public:	

	explicit Daemon(bool debug = false);
	virtual ~Daemon();

	virtual Int execute() = 0;
};

#endif //DAEMON_H

