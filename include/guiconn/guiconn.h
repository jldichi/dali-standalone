// -----------------------------------------------------------------------
// Copyright (c) 1995 InterSoft Co.  All Rights Reserved
// THIS IS UNPUBLISHED PROPRIETARY SOURCE CODE OF InterSoft Co.
// The copyright notice above does not evidence any actual or intended 
// publication of such source code.
//
// $Id: guiconn.h,v 1.1.1.1 1996/04/16 18:51:52 gustavof Exp $
//
// Description:
// -----------------------------------------------------------------------

#ifndef GUICONN_H
#define GUICONN_H

#ifdef __GNUC__
#pragma interface
#endif

#include <ifound/str.h>

class OptionParser;
class ISApp;
class FramedObject;
class wxWindowsGUIConnection;
class CrackerGUIConnection;

class GUIConnection
{
public:

	GUIConnection(const String &name);
	virtual ~GUIConnection();

	inline const String &appName() const;
	inline ISApp *app() const;

	void createRootClass();
	void deleteRootClass();

	virtual bool ok() const;
	virtual void error(Int errCode);

	virtual Int execute() = 0;
	virtual bool createObject(FramedObject *obj) = 0;
	virtual void destroyObject(FramedObject *obj) = 0;
	virtual Int mainLoop(FramedObject *mainObj) = 0;

	virtual void exitLoop(Int retVal);
	virtual bool editOptions(OptionParser *op);
	
	virtual wxWindowsGUIConnection *wxWindowsConn();
	virtual CrackerGUIConnection *CrackerConn();

protected:

	Int retValue() const;
	Int setRetValue(Int val);
	bool keepGoing() const;

	Int startMainLoop();
	Int stopMainLoop(Int fromStartMainLoop);

private:

	String appName_d;
	String compData_d;
	ISApp *rc_d;
	Int retValue_d;
};

#include <guiconn/guiconn.icc>

#endif

