/********************************************************************
* Copyright (c) 1994 InterSoft Co.  All Rights Reserved
* THIS IS UNPUBLISHED PROPRIETARY SOURCE CODE OF InterSoft Co.
* The copyright notice above does not evidence any
* actual or intended publication of such source code.
*
* $Id: termout.h,v 1.1.1.1 1996/04/16 18:51:38 gustavof Exp $
*
* DESCRIPTION
*********************************************************************/

#ifndef TERMINALOUTPUT_H
#define TERMINALOUTPUT_H

#include <ideafix/priv/gridout.h>

#include <cfix.h>

#define WAIT_TIME	3		// time between refreshes

#ifdef IDEAWIN
class IWReport;
#endif

class TerminalOutput : public GridOutput {

	window wp;			// associated window
	attr_type backgr;	// window background
	IFP getcmd; 		// read function for interactive outputs
	short base;			// Base offset
	Int last_refresh;

	Int _InitOutput();
	void _LogInFile();
	Int _GetCmd(bool f);
	void _Refresh();
	void _ReadPage(Int when, Int from, Int n);
	Int _OutPage(bool more, bool less);
	Int __GetCmd(bool more);
#ifdef	IDEAWIN
	friend class IWReport;
#endif

public:

	TerminalOutput(Int f_org, Int c_org, Int height, Int width, 
				   attr_type backgr, IFP getcmd);
	~TerminalOutput();

	TerminalOutput *getTerminalOutput()	{ return this; }
};

#endif
