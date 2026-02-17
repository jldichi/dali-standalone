// -----------------------------------------------------------------------
// Copyright (c) 1995 InterSoft Co.  All Rights Reserved
// THIS IS UNPUBLISHED PROPRIETARY SOURCE CODE OF InterSoft Co.
// The copyright notice above does not evidence any actual or intended 
// publication of such source code.
//
// $Id: isapp.h,v 1.1.1.1 1996/04/16 18:51:34 gustavof Exp $
//
// Description:
// Class definitions for uniform application startup mechanism
// -----------------------------------------------------------------------

#ifndef ISAPP_H
#define ISAPP_H

#include <ifound/str.h>

#pragma interface

class OptionParser;
class FramedObject;

class ISApp
{

public:

	virtual ~ISApp();

	static OptionParser *cmdLine();
	static String appName();

	virtual FramedObject *mainObject();		// main object
	virtual Int execute();					// or execute

};

ISApp *createAppRootClass();
OptionParser * getAppCmdLine();
String getAppCompileData();
void parseAppArgs();

#endif // ISAPP_H
