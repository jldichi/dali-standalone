// --------------------------------------------------------------------------
// Copyright (c) 1995 InterSoft Co.  All Rights Reserved
// THIS IS UNPUBLISHED PROPRIETARY SOURCE CODE OF InterSoft Co.
// The copyright notice above does not evidence any actual or intended 
// publication of such source code.
//
// $Id: guiconn.cc,v 1.1.1.1 1996/04/16 18:52:32 gustavof Exp $
//
// Description:
// --------------------------------------------------------------------------

#pragma implementation "guiconn.h"
#pragma implementation "guiconn.icc"

#include <ifound/isapp.h>
#include <guiconn/guiconn.h>
#include <ifound/priv/pconfig.h>

// common part
GUIConnection::GUIConnection(const String &name)
:	appName_d(name)
,	rc_d(NULL)
{
	_processConfig()->appConn_d = this;
}

GUIConnection::~GUIConnection()
{
	deleteRootClass();
	_processConfig()->appConn_d = NULL;
}

void 
GUIConnection::createRootClass()
{
	rc_d = createAppRootClass();
}

void 
GUIConnection::deleteRootClass()
{
	tr_delete(rc_d);
	rc_d = NULL;
}

bool 
GUIConnection::ok() const
{
	return true;
}

void 
GUIConnection::error(Int code)
{
	::error("Connection error %ld\n", code);
}

void 
GUIConnection::exitLoop(Int retVal)
{
	assert(retVal != ERR);
	retValue_d = retVal;
}

bool
GUIConnection::editOptions(OptionParser *)
{
	return true;
}

wxWindowsGUIConnection *
GUIConnection::wxWindowsConn()
{
	return NULL;
}

CrackerGUIConnection *
GUIConnection::CrackerConn()
{
	return NULL;
}


