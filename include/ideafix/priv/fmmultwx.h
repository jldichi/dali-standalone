/*
 * $Id: fmmultwx.h,v 1.1.1.1 1996/04/16 18:51:42 gustavof Exp $
 */

#ifndef FMMULTWX_H
#define FMMULTWX_H

#pragma interface

#include <wxwin/enter_wx.h>
#include <wx_scrol.h>
#include <wxwin/leave_wx.h>

class FmMultiLine;

class FmMultiScrollBar : public wxScrollBar {
	FmMultiLine *multi;
	bool inside;

	static void FmMultiAction(wxObject &object, wxEvent &event);
	void onScroll(Int pos);

public:
	FmMultiScrollBar(FmMultiLine *theMulti, Int x, Int y, Int w, Int h);
};

class FmMultiScrollBarX : public wxScrollBar {
	FmMultiLine *multi;
	bool inside;

	static void FmMultiAction(wxObject &object, wxEvent &event);

public:
	FmMultiScrollBarX(FmMultiLine *theMulti, Int x, Int y, Int w, Int h);
};

#endif

