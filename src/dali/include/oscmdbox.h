/********************************************************************
* Copyright (c) 1991 InterSoft Co.  All Rights Reserved
* THIS IS UNPUBLISHED PROPRIETARY SOURCE CODE OF InterSoft Co.
* The copyright notice above does not evidence any
* actual or intended publication of such source code.
*
* $Id: oscmdbox.h,v 1.1.1.1 1996/04/16 18:52:53 gustavof Exp $
*
* DESCRIPTION:	Definitions for execute OS's panel 
*********************************************************************/
#ifndef EXECOSCMDBOX_H
#define EXECOSCMDBOX_H

#ifdef __GNUC__
#	pragma interface
#endif

#ifdef ISDEBUG
#	include <ifound/iddebug.h>
#endif

#include <winapp/panel.h>

class StrCell;
class String;
class StrHistory;

// clase para el Dialog de ExecOSCommand, captura el comando
class ExecOSCmdDlog : public Panel
{
private:
	String *cmd;

	ExecOSCmdDlog(const ExecOSCmdDlog&) ;	
	void operator=(const ExecOSCmdDlog&) ;
	
public:
	StrCell		*cmdFld;
	Label		*staticText;

	ExecOSCmdDlog(StrHistory *his);
	virtual ~ExecOSCmdDlog();
	const String &value()	{ return *cmd; }
	virtual Int execute(UnChar c = 0);
};

Int cmdOk(ExecOSCmdDlog &p);

#endif
