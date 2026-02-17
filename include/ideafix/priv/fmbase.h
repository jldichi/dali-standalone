/********************************************************************
* Copyright (c) 1994 InterSoft Co.  All Rights Reserved
* THIS IS UNPUBLISHED PROPRIETARY SOURCE CODE OF InterSoft Co.
* The copyright notice above does not evidence any
* actual or intended publication of such source code.
*
* $Id: fmbase.h,v 1.1.1.1 1996/04/16 18:51:40 gustavof Exp $
*
* DESCRIPTION
*********************************************************************/

#ifndef WXFMBASE_H
#define WXFMBASE_H

#pragma interface

#include <ideafix/priv/fmobj.h>

class wxItem;
class wxPanel;
class wxStaticItem;
class gtype(PtrArray, wxItem);
class gtype(PtrArray, wxStaticItem);

class FmBaseField : public FmFieldObj {
	typedef FmFieldObj inherited;

	gtype(PtrArray, wxItem)	&items_;

protected:
	virtual wxItem *new_wxItem(wxPanel *panel,
				int x, int y, int w, int h, long style, char *name) = 0;
	
public:
	FmBaseField();
	~FmBaseField();
	
	FM_DECLARE_ABSTRACT_CLASS();

	virtual bool internal() const;

	virtual void _realize();
	virtual void _unrealize();

	// get object's associated item(s)
	virtual wxItem *item(Int row = 0) const;
	
	virtual void updateOffset();
};


class FmBaseItem : public FmObject {
	typedef FmObject inherited;

	gtype(PtrArray, wxItem)	&items_;

protected:
	virtual wxItem *new_wxItem(wxPanel *panel,
				int x, int y, int w, int h, long style, char *name) = 0;
	
public:
	FmBaseItem();
	~FmBaseItem();

	FM_DECLARE_ABSTRACT_CLASS();
	
	virtual void _realize();
	virtual void _unrealize();

	// get object's associated item(s)
	virtual wxItem *item(Int row = 0) const;
	
	virtual void updateOffset();
};


class FmBaseStatic : public FmObject {
	typedef FmObject inherited;

	gtype(PtrArray, wxStaticItem)	&items_;
	
protected:
	virtual wxStaticItem *new_wxStatic(wxPanel *panel,
				int x, int y, int w, int h, long style, char *name) = 0;
	
public:
	FmBaseStatic();
	~FmBaseStatic();

	FM_DECLARE_ABSTRACT_CLASS();
	
	virtual void _realize();
	virtual void _unrealize();

	// get object's associated item(s)
	wxStaticItem *getStatic(Int row = 0) const;

	virtual void updateOffset();
};

#include <ideafix/priv/fmbase.icc>

#endif
