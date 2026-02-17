/********************************************************************
* Copyright (c) 1991 InterSoft Co.  All Rights Reserved
* THIS IS UNPUBLISHED PROPRIETARY SOURCE CODE OF InterSoft Co.
* The copyright notice above does not evidence any
* actual or intended publication of such source code.
*
* $Id: dali.cc,v 1.2 2007/02/01 13:48:04 spark Exp $
*
* DESCRIPTION: Implementation of dali editor
**********************************************************************/

#include <dali.h>
#include <editor.h>
#include <ifound/main.h>

dali::dali()
{
}
	
dali::~dali()
{
}   

Int dali::execute()
{
    WiConnect((char *) ProcTty(), toCharPtr(appName()), 0); 
	Editor::execute();

// #ifndef ISDEBUG
	stop(0);
// #endif
	
	return 0;
}

OptionParser *dali::cmdLine()
{
	static OptionParser op(versionStr(DALI_VERSION));
	op	 <<	Opt(Editor::op_noDalirc, "default",	"[OP_BUILTIN]")
		 <<	Opt(Editor::op_printDir, "dir",		"[OP_PRINTCWD]")
		 <<	Opt(Editor::op_findTag,	 "tag",		"[OP_OPENTAG]")
		 <<	Opt(Editor::op_execOSCmd,"shell",	"[OP_SHELLCMD]")
		 <<	Opt(Editor::op_loadWsp,	 "wsp",		"[OP_OPENWSP]");
	return &op;
}

String dali::appName()
{
	return "dali";
}
