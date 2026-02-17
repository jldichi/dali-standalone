/********************************************************************
* Copyright (c) 1994 InterSoft Co.  All Rights Reserved
* THIS IS UNPUBLISHED PROPRIETARY SOURCE CODE OF InterSoft Co.
* The copyright notice above does not evidence any
* actual or intended publication of such source code.
*
* $Id: fmrepwx.h,v 1.1.1.1 1996/04/16 18:51:40 gustavof Exp $
*
* DESCRIPTION
* wxWindows related classes: FmFrame & FmPanel
*********************************************************************/

#ifndef FMREPWX_H
#define FMREPWX_H

#pragma interface

#include <wxwin/enter_wx.h>
#include <wx_frame.h>
#include <wx_panel.h>
#include <wxwin/leave_wx.h>

class FmFrame : public wxFrame
{
	typedef wxPanel inherited;

	friend class FormRep;
	
	// Form that is wrapped by this frame
	FormRep *fm_;

protected:

	FmFrame(FormRep *theForm);
	~FmFrame(void);

public:
	void OnMenuCommand(int commandId);
	virtual void OnSize(int width, int height);
};

class FmPanel : public wxPanel
{ 
	typedef wxPanel inherited;
	
	friend class FormRep;
	
	// Form that is displayed in this panel
	FormRep *fm_;

	bool shown_;

protected:

	FmPanel(FormRep *theForm);
	~FmPanel(void);
	
	void AllowFocus(Bool allow);

public:
    virtual Bool Show(Bool show);
    inline Bool Shown() const;

	// Redefine OnChangeFocus function to process item changes
	virtual void OnChangeFocus(wxItem *fromItem, wxItem *toItem);
	// Redefine OnFunctionKey to process keyboard functions correctly
	virtual Bool OnFunctionKey(wxKeyEvent &event);

    virtual void OnSetFocus();
};


#include <ideafix/priv/fmrepwx.icc>

#endif  WXFMREPWX_H


