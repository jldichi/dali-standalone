// -----------------------------------------------------------------------
// Copyright (c) 1995 InterSoft Co.  All Rights Reserved
// THIS IS UNPUBLISHED PROPRIETARY SOURCE CODE OF InterSoft Co.
// The copyright notice above does not evidence any actual or intended 
// publication of such source code.
//
// $Id: mainobj.h,v 1.1.1.1 1996/04/16 18:51:34 gustavof Exp $
//
// Description:
// -----------------------------------------------------------------------

#ifndef MAINOBJ_H
#define MAINOBJ_H

#ifdef __GNUC__
#pragma interface
#endif

#include <ifound.h>

class wxFrame;
class RequestConnection;

class FramedObject
{
public:

	explicit inline FramedObject();
	virtual ~FramedObject();
	
	// for wxWindows
	virtual bool canUseWxWindows() const;
	virtual bool wxWindowsCreate();
	virtual void wxWindowsDestroy();

	// for CRACKER
	virtual bool canUseCracker() const;
	virtual bool crackerCreate(RequestConnection &conn);
	virtual void crackerDestroy(RequestConnection &conn);

	virtual bool processRequest(RequestConnection &conn);

	inline wxFrame *mainFrame() const;
	inline Int remoteId() const;

protected:

	inline void setMainFrame(wxFrame *frame);
	inline void setRemoteId(Int id);

private:

	Int remoteId_d;
	wxFrame *mainFrame_x;
};

#include <ifound/mainobj.icc>

#endif

