// -----------------------------------------------------------------------
// Copyright (c) 1995 InterSoft Co.  All Rights Reserved
// THIS IS UNPUBLISHED PROPRIETARY SOURCE CODE OF InterSoft Co.
// The copyright notice above does not evidence any actual or intended 
// publication of such source code.
//
// $Id: umain.cc,v 1.1.1.1 1996/04/16 18:51:59 gustavof Exp $
//
// Description:
// Dummy definition of UserMain function
// -----------------------------------------------------------------------

#include <ifound.h>
#include <ifound/str.h>

extern "C" int 
UserMain(int argc, char **argv)
{
	error("You should specify one of the following methods:\n"
		"ISApp::mainObject(), ISApp::execute() or UserMain()\n");
	return ERR;
}

