/********************************************************************
* Copyright (c) 2026 Master_Solutions S.R.L.  All Rights Reserved
* THIS IS UNPUBLISHED PROPRIETARY SOURCE CODE OF Master_Solutions S.R.L.
* The copyright notice above does not evidence any
* actual or intended publication of such soucone code.
*
* $Id: dirview.cc,v 1.3 1998/04/27 18:50:10 mauro Exp $
*
* DESCRIPTION
*********************************************************************/

#ifdef __GNUC__
#pragma implementation
#endif

#include <local/dirview.h>
#include <local/osfile.h>
#include <local/dbrowser.h>
#include <ifound/parray.h>
#include <ifound/stdc.h>

#include <sys/stat.h>

// S_IFCHR|S_IFBLK|S_IFIFO|S_IFNAM
#ifndef S_IFIFO
#	define S_IFIFO 0
#endif
#ifndef S_IFBLK
#	define S_IFBLK 0x6000
#endif

declare(PtrArray, DirElem);
declare(PtrArrayCursor, DirElem);

#define DIRELEM_REALLOC	32

DirElem::DirElem(const String &nm, Int sz, Int md, Int ui, Int gi,
		Time tm)
:	name_d(nm),
	size_d(sz),
	mode_d(md),
	uid_d(ui),
	gid_d(gi),
	modTime_d(tm)
{
}

DirElem::DirElem(const String &nm, Int sz, Int md, Int ui, Int gi)
:	name_d(nm),
	size_d(sz),
	mode_d(md),
	uid_d(ui),
	gid_d(gi),
	modTime_d()
{
}

DirElem &
DirElem::operator=(const DirElem &de)
{
	if (this != &de) {
		name_d = de.name_d;   
		size_d = de.size_d;
		mode_d = de.mode_d;
		uid_d = de.uid_d;
		gid_d = de.gid_d;
		modTime_d = de.modTime_d;
	}
	return *this;
}

DirElem::~DirElem()
{
}

extern "C" int byName(void const *a, void const *b)
{
	DirElem const *da = *(DirElem const **) a;
	DirElem const *db = *(DirElem const **) b;

	if (da == NULL) return (db == NULL ? 0 : -1);
	return (db == NULL ? 1 : int(::cmp(da->name(), db->name())));
}

extern "C" int byTime(void const *a, void const *b)
{
	DirElem const *da = *(DirElem const **) a;
	DirElem const *db = *(DirElem const **) b;

	if (da == NULL) return (db == NULL ? 0 : -1);
	return (db == NULL ? 1 : int(::cmp(db->modTime(), da->modTime())));
}

extern "C" int bySize(void const *a, void const *b)
{
	DirElem const *da = *(DirElem const **) a;
	DirElem const *db = *(DirElem const **) b;

	if (da == NULL)	return (db == NULL ? 0 : -1);
	return (db == NULL ? 1 : da->size() - db->size());
}

extern "C" int byMode(void const *a, void const *b)
{
	DirElem const *da = *(DirElem const **) a;
	DirElem const *db = *(DirElem const **) b;

	if (da == NULL)	return (db == NULL ? 0 : -1);
	return (db == NULL ? 1 : da->mode() - db->mode());
}

DirView::DirView()
:	dv_d(*tr_new PtrArray(DirElem)(0, DIRELEM_REALLOC)),
	dir_d(HIGH_VALUE),
	rexp_d(NULL_STRING)
{
}

DirView::DirView(const String &direc, const String &re, Int ftype,
		Int sortBy)
:	dv_d(*tr_new PtrArray(DirElem)(0, DIRELEM_REALLOC))
{
	reset(direc, re, ftype, sortBy);
}

DirView::DirView(DirBrowser &dbrowser, Int ftype, Int sortBy)
:	dv_d(*tr_new PtrArray(DirElem)(0, DIRELEM_REALLOC))
{
	reset(dbrowser, ftype, sortBy);
}

void
DirView::sort(Int sortBy)
{
	switch (sortBy) {
	case DV_NOSORT:
		break;
	case DV_BYTIME:
		dv_d.sort(byTime);
		break;
	case DV_BYNAME:
		dv_d.sort(byName);
		break;
	case DV_BYSIZE:
		dv_d.sort(bySize);
		break;
	case DV_BYMODE:
		dv_d.sort(byMode);
		break;
	default:
		assert(0);
	}
}

void
DirView::clear()
{
	for (PtrArrayCursor(DirElem) cdv(dv_d); cdv; ++cdv)
		if (*cdv)
			tr_delete(*cdv);

	dv_d.reAlloc(0);
}

Int
DirView::nElems() const
{
	return dv_d.dim();
}

Int
DirView::add(DirElem *el)
{
	return dv_d.add(el);
}

#ifndef S_IFNAM
#	define S_IFNAM	0
#endif

void
DirView::reset(const String &direc, const String &sre,
		Int ftype, Int sortBy)
{
	rexp_d = sre;
	dir_d = direc;
	LRegExp re(rexp_d);
	DirBrowser db(direc, &re);
	reset(db, ftype, sortBy);
}

void
DirView::reset(DirBrowser &db, Int ftype, Int sortBy)
{
	bool good;

	clear();

	for (; db; ++db) {

		Int mode;
		Int size;
		Int uid;
		Int gid;
		Time mTime;
		db.info(mode, size, uid, gid, mTime);

		if ((ftype & DV_REGFILES) && (mode & S_IFREG))
			good = true;
		else if ((ftype & DV_DIRFILES) && (mode & S_IFDIR)) {
			String dbName = db.name();
			good = !((dbName == "." && !(ftype & DV_SELFFILE)) ||
				(dbName == ".." && !(ftype & DV_PARENTFILE)));
		} else if ((ftype & DV_SPCFILES) && (mode &
				(S_IFCHR|S_IFBLK|S_IFIFO|S_IFNAM)))
			good = true;
		else
			good = false;

		if (good)
			dv_d.add(tr_new DirElem(db.name(), size, mode, uid, gid, mTime));
	}

	sort(sortBy);

}

DirView::~DirView()
{
	clear();
	tr_delete(&dv_d);
}

void
DirView::elem(Int i, String &nm, Int &sz, Int &md, Int &ui,
		Int &gi) const
{
	Time dummy;
	elem(i, nm, sz, md, ui, gi, dummy);
}

void
DirView::elem(Int i, String &nm, Int &sz, Int &md, Int &ui,
		Int &gi, Time &modTime) const
{
	DirElem *de = dv_d.isInRange(i) ? dv_d.elem(i) : NULL;
	if (de) {
		nm = de->name();
		sz = de->size();
		md = de->mode();
		ui = de->uid();
		gi = de->gid();
		modTime = de->modTime();
	} else {
		nm = HIGH_VALUE;
		sz = ERROR;
		md = ERROR;
		ui = ERROR;
		gi = ERROR;
	}
}

const DirElem *
DirView::elem(Int i) const
{
	return dv_d.elem(i);
}
