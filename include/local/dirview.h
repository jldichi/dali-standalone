/***********************************************************************
* Copyright (c) 1991 InterSoft Co.  All Rights Reserved
* THIS IS UNPUBLISHED PROPRIETARY SOURCE CODE OF InterSoft Co.
* The copyright notice above does not evidence any
* actual or intended publication of such source code.
*
* $Id: dirview.h,v 1.1.1.1 1996/04/16 18:51:49 gustavof Exp $
*
* DESCRIPTION
* Class definitions for the Directory Viewer data type
*********************************************************************/

#ifndef DIRVIEW_H
#define DIRVIEW_H

#ifdef __GNUC__
#pragma interface
#endif

#include <ifound.h>

#include <ifound/itypes.h>

// Sort types
#define DV_NOSORT	1
#define DV_BYNAME	2
#define DV_BYSIZE	3
#define DV_BYMODE	4
#define DV_BYTIME	5

// File types
#define DV_REGFILES	(1<<0)
#define DV_SPCFILES	(1<<1)
#define DV_DIRFILES	(1<<2)
#define DV_SELFFILE (1<<3)
#define DV_PARENTFILE	(1<<4)
#define DV_ALLFILES \
		(DV_REGFILES|DV_SPCFILES|DV_DIRFILES|DV_SELFFILE|DVPARENTFILE)

class RegExp;
class DirBrowser;

class DirElem
	// Directory viewer element
{

// private:

	String name_d;
	Int size_d;
	Int mode_d;
	Int uid_d;
	Int gid_d;
	Time modTime_d;

public:

	DirElem(const String &nm, Int sz, Int md, Int ui, Int gi,
			Time tm);
	DirElem(const String &nm, Int sz, Int md, Int ui, Int gi);
	inline DirElem(const DirElem &de);
	~DirElem();

    DirElem &operator=(const DirElem &de);

	inline const String &name() const;
	inline Int size() const;
	inline Int mode() const;
	inline Int uid() const;
	inline Int gid() const;
	inline Time modTime() const;
};

DirElem::DirElem(const DirElem &de)
{ *this = de; }

const String &DirElem::name() const
{ return name_d; }

Int DirElem::size() const
{ return size_d; }

Int DirElem::mode() const
{ return mode_d; }

Int DirElem::uid() const
{ return uid_d; }

Int DirElem::gid() const
{ return gid_d; }

Time DirElem::modTime() const
{ return modTime_d; }

class gtype(PtrArray, DirElem);

class DirView
	// Directory viewer
{

public:

	DirView();
	DirView(const String &dir, const String &re, Int ftype, Int sortBy);
	DirView(DirBrowser &dbrowser, Int ftype, Int sortBy);
	virtual ~DirView();

	virtual void reset(const String &nm, const String &re, Int ftype,
			Int sortBy);
	virtual void reset(DirBrowser &dbrowser, Int ftype, Int sortBy);
	void elem(Int i, String &nm, Int &sz, Int &md, Int &ui, Int &gi,
			Time &tm) const;
	void elem(Int i, String &nm, Int &sz, Int &md, Int &ui, Int &gi) const;
	const DirElem *elem(Int i) const;
	Int nElems() const;
	Int add(DirElem *el);
	void sort(Int sortType = DV_BYNAME);
	void clear();
	inline const String &dir() const;
	inline void dir(const String &dir);
	inline const String &rexp();

protected:

	gtype(PtrArray, DirElem) &dv_d;
	String dir_d;
	String rexp_d;

	DirView(const DirView &);	
	void operator=(const DirView &);

};

void
DirView::dir(const String &d)
{
	dir_d = d;
}

const String &
DirView::dir() const
{
	return dir_d;
}

const String &
DirView::rexp()
{
	return rexp_d;
}

#endif
