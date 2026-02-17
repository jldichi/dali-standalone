// --------------------------------------------------------------------------
// Copyright (c) 1995 InterSoft Co.  All Rights Reserved
// THIS IS UNPUBLISHED PROPRIETARY SOURCE CODE OF InterSoft Co.
// The copyright notice above does not evidence any actual or intended 
// publication of such source code.
//
// $Id: no_imain.cc,v 1.1.1.1 1996/04/16 18:51:59 gustavof Exp $
//
// Description:
// Routines that implement dummy functions for imain.cc
// --------------------------------------------------------------------------

#include <ifound.h>
#include <ifound/str.h>

class OptionParser;
class ISApp;

OptionParser *
getAppCmdLine()
{
	abnormal;
	return NULL;
}

String 
getAppCompileData()
{
	abnormal;
	return NULL_STRING;
}

ISApp *
createAppRootClass()
{
	abnormal;
	return NULL;
}

