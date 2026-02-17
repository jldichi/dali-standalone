// -----------------------------------------------------------------------
// Copyright (c) 1995 InterSoft Co.  All Rights Reserved
// THIS IS UNPUBLISHED PROPRIETARY SOURCE CODE OF InterSoft Co.
// The copyright notice above does not evidence any actual or intended 
// publication of such source code.
//
// $Id: isapp.cc,v 1.1.1.1 1996/04/16 18:51:59 gustavof Exp $
//
// Description:
// Application startup mechanism implementation
// -----------------------------------------------------------------------

#ifdef __GNUC__
#pragma implementation "isapp.h"
#pragma implementation "mainobj.h"
#pragma implementation "mainobj.icc"
#endif

#include <ifound.h>
#include <ifound/isapp.h>
#include <ifound/priv/pconfig.h>
#include <ifound/oparser.h>
#include <ifound/mainobj.h>
#include <ifound/gfuncs.h>

extern "C" int UserMain(int argc, char **argv);

ISApp::~ISApp()
{
}

OptionParser *
ISApp::cmdLine()
{
	return NULL;
}

String 
ISApp::appName()
{
	return NULL_STRING;
}

FramedObject *
ISApp::mainObject()
{
	return NULL;
}

Int
ISApp::execute()
{
	return UserMain(int(argCnt()), argRefVal());
}


FramedObject::~FramedObject()
{
	assert(remoteId()==ERR && !mainFrame());
}

bool 
FramedObject::canUseWxWindows() const
{
	return false;
}

bool
FramedObject::wxWindowsCreate()
{
	abnormal;
	return false;
}

void
FramedObject::wxWindowsDestroy()
{
	abnormal;
}

bool 
FramedObject::canUseCracker() const
{
	return false;
}

bool 
FramedObject::crackerCreate(RequestConnection &)
{
	abnormal;
	return false;
}

void
FramedObject::crackerDestroy(RequestConnection &)
{
	abnormal;
}

bool 
FramedObject::processRequest(RequestConnection &)
{
	abnormal;
	return false;
}

void 
parseAppArgs()
{
	OptionParser *op = getAppCmdLine();
	if (op) {
		op->compileData(getAppCompileData());
		op->parse(_processConfig()->argc, _processConfig()->argv);
	}
}

