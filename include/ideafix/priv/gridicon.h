// -------------------------------------------------------------------------
// Copyright (c) 1995 InterSoft Co.  All Rights Reserved
// THIS IS UNPUBLISHED PROPRIETARY SOURCE CODE OF InterSoft Co.
// The copyright notice above does not evidence any actual or intended 
// publication of such source code.
//
// $Id: gridicon.h,v 1.1.1.1 1996/04/16 18:51:42 gustavof Exp $
//
// Description:
// -------------------------------------------------------------------------

#ifndef GRIDICON_H
#define GRIDICON_H

#define  TY_ICON 5378668

class wxBitmap;
class wxDC;

class gtype(PtrHashTab, GridIcon);

class GridIcon 
{
protected:
	wxBitmap* bitmap_;
	String name_;

public:

	GridIcon(const String &nm = NULL_STRING);
	~GridIcon(void);

	void assign(const String &nm);
	String format(const String &mask);
	String name() const;
	inline wxBitmap *bitmap(void) const;
	void blit(wxDC *dc, Int x, Int y, Int w, Int h, Int xc, Int wc);
};
#endif
