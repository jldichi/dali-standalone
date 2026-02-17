/***********************************************************************
* Copyright (c) 1991 InterSoft Co.  All Rights Reserved
* THIS IS UNPUBLISHED PROPRIETARY SOURCE CODE OF InterSoft Co.
* The copyright notice above does not evidence any
* actual or intended publication of such source code.
*
* $Id: pconfig.h,v 1.3 2008/04/25 22:05:16 spark Exp $
*
* DESCRIPTION
* Process configuration
*********************************************************************/

#ifndef PCONFIG_H
#define PCONFIG_H

#include <ifound/str.h>
#include <ifound/itypes.h>

class PrServerConnection;
class GUIConnection;

struct ProcessConfig {
	String name;
	int argc;
	char **argv;
	Time initTime;
	Date initDate;
	Int pid;
	int userid;
	int usergid;
	bool add;
	bool del;
	bool update;
	bool background;
	Int sig;
	String ttyName;
	String ttyWm;
	String sysName;
#if HAVE_SAVEMEM
	bool savemem;
#endif
	PrServerConnection *conn; // Presentation server client
	GUIConnection *appConn_d; // GUI application connection
	Int netDelay; // Microseconds. PrSrv-Cli connection (WiCreateProces) time.
	bool showNMeth;

	ProcessConfig();
};

ProcessConfig *_processConfig();

struct InternatConfig {
	String dayNames[7];
	String monthNames[12];
	char dec_point;
	char thousand_sep;
	DateFmt date_fmt;

	InternatConfig();
};

#endif
