/********************************************************************
* Copyright (c) 1991 InterSoft Co.  All Rights Reserved
* THIS IS UNPUBLISHED PROPRIETARY SOURCE CODE OF InterSoft Co.
* The copyright notice above does not evidence any
* actual or intended publication of such source code.
*
* $Id: scrmenu.cc,v 1.4 2004/09/29 21:06:27 damianm Exp $
*
* DESCRIPTION
* Implementation of scroll menu class
*********************************************************************/

#include <unistd.h>

#include <ifound.h>
#include <ifound/priv/gfuncs.h>
#include <stdio.h>
#include <winapp/menu.h>
#include <ifound/stdc.h>
#include <local/dirview.h>
#include <sys/stat.h>
#include <local/osfile.h>

#define MAX_GROUPS	17

#undef HAVE_USERS

// clase ScrollMenu

// metodos privados

bool ScrollMenu::incCurrItem(Int cant)
{
	idCheck();

	Int theItem = curritem+cant;

	// solo redibujo
	if (theItem < nitems) {

		// must skip all lines
		while (theItem < nitems && items[theItem]->isLine())
			theItem++;

		if (theItem < nitems) {
			curritem = theItem;
			newCurrItem(floor + curritem);
			return true;
		}
	}

	String s = NULL_STRING, s1;
	Int attr;

	theItem--;
	while (s == NULL_STRING) {
		theItem++;
		fpRead(floor + theItem, s, s1, attr);
	}

	cant = theItem - nitems + 1;
	curritem = nitems - 1;

	for (Int j = 0; j < cant; j++) {
		fpRead(floor + nitems, s, s1, attr);
		if (s == HIGH_VALUE && s1 == HIGH_VALUE)
			return j == 0 ? false : true;

		// corro todo uno para arriba
		tr_delete(items[0]);
		for (Int i = 0; i < nitems-1; i++)
			items[i] = items[i+1];

		floor++;

		items[nitems-1] = tr_new MenuItem(s, floor + nitems - 1,
				hilite ? HKSEQ : '\0', attr);
		items[nitems-1]->getSecText() = s1;
	}

	newCurrItem(floor + curritem);
	return true;
}

bool ScrollMenu::decCurrItem(Int cant)
{
	idCheck();

	Int theItem = curritem - cant;

	// just redraw
	if (theItem >= 0) {

		// must skip all lines
		while (theItem >= 0 && items[theItem]->isLine())
			theItem--;

		if (theItem >= 0) {
			curritem = theItem;
			newCurrItem(floor + curritem);
			return true;
		}
	}

	// no more options
	if (floor == 0)
		return false;

	String s = NULL_STRING, s1;
	Int attr;

	theItem++;
	while (s == NULL_STRING) {
		theItem--;
		fpRead(floor+theItem, s, s1, attr);
	}

	cant = -theItem;
	curritem = 0;

	if (cant > floor || s == HIGH_VALUE)
		cant = floor;

	for (Int j = 0; j < cant; j++) {
		fpRead(--floor, s, s1, attr);
		assert(s != HIGH_VALUE);
		tr_delete(items[nitems-1]);
		for (Int i = nitems-1; i > 0; i--)
			items[i] = items[i-1];

		items[0] = tr_new MenuItem(s, floor, hilite, attr);
		items[0]->getSecText() = s1;
	}

	newCurrItem(floor + curritem);
	return true;
}

void ScrollMenu::newSMenu(SFPI f, Int nf, Int nc)
{
	idCheck();

	fpread = f;
	floor  = 0;
	nfil   = nf > MAX_ITEMS ? MAX_ITEMS : nf;
	ncol   = nc;
}

void ScrollMenu::calcDim(Int &nf, Int &nc)
{
	idCheck();

	nf = nfil;
	nc = ncol;
}

void ScrollMenu::fillWindow(bool leaving)
{
	idCheck();

	wi->setBorder(border, label);

	PoppableMenu::fillWindow(leaving);

	if (floor) {
		wi->goTo(0, -1);
		*wi << C_UARROW;
		wi->goTo(0, wi->width());
		*wi << C_UARROW;
		wi->goTo(curritem, 1);
	}

	String s, s1;
	Int attr;

	if (nitems == nfil) {
		fpRead(floor+nitems, s, s1, attr);
		if (s != HIGH_VALUE || s1 != HIGH_VALUE) {
			Int fils = wi->height()-1;
			wi->goTo(fils, -1);
			*wi << C_DARROW;
			wi->goTo(fils, wi->width());
			*wi << C_DARROW;
			wi->goTo(curritem, 1);
		}
	}
}

void ScrollMenu::fpRead(Int i, String &str, String &secStr, Int &attr)
{
	idCheck();

	secStr = HIGH_VALUE;
	attr = A_NORMAL;

	if (fpread)
		str = fpread(i);
	else
		str = HIGH_VALUE;
}

void ScrollMenu::refreshItems()
{
	idCheck();

	String s, s1;
	Int attr;
	Int i;

	for (i = 0; i < nfil; i++) {

		fpRead(floor + i, s, s1, attr);

		if (s == HIGH_VALUE && s1 == HIGH_VALUE)
			break;

		assert(items[i]);
		tr_delete(items[i]);

		items[i] = tr_new MenuItem(s, floor + i, hilite, attr);
		items[i]->getSecText() = s1;
	}

	assert(nitems == i);
}

void ScrollMenu::fillItems()
{
	idCheck();

	String s, s1;
	Int attr;

	// if curritem isn't in the first page update floor and
	// adjust curritem

	floor = curritem < nfil-1 ? 0 : curritem - (nfil-1);
	curritem -= floor;

	Int i;
	for (i=0; i < nfil; i++) {
	    fpRead(i+floor, s, s1, attr);
		if (s == HIGH_VALUE && s1 == HIGH_VALUE)
			break;

		if (items[i])
			tr_delete(items[i]);

		items[i] = tr_new MenuItem(s, i+floor, hilite, attr);
		items[i]->getSecText() = s1;
	}
	nitems = i;
}

// metodos publicos

ScrollMenu::ScrollMenu(const String &s, SFPI f, Int nf, Int nc,
		Int fo, Int co, bool hi)
:	PoppableMenu(s, fo, co, hi)
{
	idCheck();

	newSMenu(f, nf, nc);
}

ScrollMenu::ScrollMenu(SFPI f, Int nf, Int nc, Int fo, Int co,
		bool hi)
:	PoppableMenu(fo, co, hi)
{
	idCheck();

	newSMenu(f, nf, nc);
}

ScrollMenu::~ScrollMenu()
{
	idCheck();

	fpread = NULL;
	nfil = ncol = floor  = 0;
}

bool ScrollMenu::display(Int fil, Int col)
{
	idCheck();

	if (wi->hidden())
		fillItems();
	return PoppableMenu::display(fil, col);
}

void ScrollMenu::unDisplay()
{
	idCheck();

	for (Int i=0 ; i < MAX_ITEMS; i++) {
		if (items[i]) {
			tr_delete(items[i]);
			items[i] = NULL;
		}
	}
	PoppableMenu::unDisplay();
}

void ScrollMenu::textOpt(Int i, String &str, String &secStr)
{
	idCheck();

	Int attr;
	fpRead(i, str, secStr, attr);
	return;
}

bool ScrollMenu::incFind()
{
	idCheck();

	// marco que estoy en modo busqueda incremental
	wi->goTo(-1, wi->width()-2);
	*wi << C_BULLET;

	UnChar c;
	bool done = false, result = false, found;
	Int i;
	String srchStr = NULL_STRING, str, str1;
	Int attr;
	while (!done && (*wi >> c, c) != K_END) {
		switch (c) {
		case K_ENTER:
			result = done = true;
			break;
		case K_META:
			srchStr = NULL_STRING;
			break;
		default:
			if (!isprint(c))
				wi->beep();
			else {
				srchStr += toUpperCase(c);
				i = curritem+floor;
				found = false;
				do {
					fpRead(i, str, str1, attr);
					if (str == HIGH_VALUE) {
						found = true;
						srchStr = NULL_STRING;
						wi->beep();
					} else {
						if (str != NULL_STRING) {
							if (str.toUpper().index(srchStr) != ERR) {
								incCurrItem(i-curritem-floor);
								fillWindow();
								wi->goTo(-1, wi->width()-2);
								*wi << C_BULLET;
								found = true;
							}
						}
						i++;
					}
				} while (!found);
			}
			break;
		}
	}
	// 'desmarco' que estoy en modo busqueda incremental
	fillWindow();
	return result;
}

void ScrollMenu::reFill()
{
	idCheck();

	if (wi->hidden())
		return;
	curritem = ERR;
	fillItems();
	fillWindow();
}

void ScrollMenu::refresh()
{
	idCheck();

	refreshItems();
	fillWindow();
}


// fin clase ScrollMenu

// clase FileMenu

// metodes privados

/*
	Chars used to show file read/write permissions
*/
#define RDWR_CHAR	'\0'
#define	RDONLY_CHAR	C_BULLET
#define NORD_CHAR	'-'

bool canSearch(Int mode, Int fileuid, Int filegid, Int ouruid,
		GidT *ourgids, Int nourgids)
{
#ifndef HAVE_USERS
	return true;
#else
	/*
		Root can do what he pleases
	*/
	if (ouruid == 0)
		return true;

	Int tmpMask = S_IXOTH;
	Int i;

	for (i = 0; i < nourgids; i++) {
		if (ourgids[i] == GidT(filegid)) {
			tmpMask = S_IXGRP;
			break;
		}
	}

	if (ouruid == fileuid)
		tmpMask = S_IXUSR;

	return (mode & tmpMask);
#endif
}

bool canRead(Int mode, Int fileuid, Int filegid, Int ouruid,
		GidT *ourgids, Int nourgids)
{
#ifndef HAVE_USERS
	return true;
#else
	/*
		Root can do what he pleases
	*/
	if (ouruid == 0)
		return true;

	Int tmpMask = S_IROTH;
	Int i;

	for (i = 0; i < nourgids; i++) {
		if (ourgids[i] == GidT(filegid)) {
			tmpMask = S_IRGRP;
			break;
		}
	}

	if (ouruid == fileuid)
		tmpMask = S_IRUSR;

	return (mode & tmpMask);
#endif
}

bool canWrite(Int mode, Int fileuid, Int filegid, Int ouruid,
		GidT *ourgids, Int nourgids)
{
#ifndef HAVE_USERS
	return true;
#else
	/*
		Root can do what he pleases
	*/
	if (ouruid == 0)
		return true;

	Int tmpMask = S_IWOTH;
	Int i;

	for (i = 0; i < nourgids; i++) {
		if (ourgids[i] == GidT(filegid)) {
			tmpMask = S_IWGRP;
			break;
		}
	}

	if (ouruid == fileuid)
		tmpMask = S_IWUSR;

	return (mode & tmpMask);
#endif
}

void FileMenu::fpRead(Int nro, String &str, String &secStr, Int& attr)
{
	idCheck();

	static GidT grpList[MAX_GROUPS];
	static int ngrps = ERR;

	if (ngrps == ERR) {
		grpList[0] = getegid();
		ngrps = 1;
#ifdef HAVE_GETGROUPS
		int ng;
		if ((ng = getgroups(MAX_GROUPS-1, &grpList[1])) != ERR)
			ngrps += ng;
#endif
	}

	if (dv.dir() == HIGH_VALUE || dv.dir() != dir
			|| dv.rexp() != file)
		dv.reset(dir, file, DV_REGFILES, DV_BYNAME);

	String nm;
	Int md, sz, uid, gid;
	dv.elem(nro, nm, sz, md, uid, gid);
	if (nm) {

#ifndef HAVE_USERS
		bool rd = canRead(md, 0, 0, 0, NULL, 0);
		bool wr = canWrite(md, 0, 0, 0, NULL, 0);
#else
		bool rd = canRead(md, uid, gid, geteuid(), grpList, ngrps);
		bool wr = canWrite(md, uid, gid, geteuid(), grpList, ngrps);
#endif

		char temp[2];
		temp[1] = '\0';

		if (rd) {
			/*
				Everything OK
			*/
			if (wr)
				temp[0] = RDWR_CHAR;
			/*
				Read only
			*/
			else
				temp[0] = RDONLY_CHAR;
		} else
			/*
				Cannot read, can't do anything
			*/
			temp[0] = NORD_CHAR;

		secStr = temp;

		str = nm;
		attr = A_NORMAL;
		return;
	}

	str = HIGH_VALUE;
	secStr = HIGH_VALUE;
}

// metodos publicos

FileMenu::FileMenu(const String &l, const String &f, Int nf, Int nc,
		Int fo, Int co, const String &thecwd)
:	ScrollMenu(l, NULL , nf, nc, fo, co),
	dv(*tr_new DirView),
	file(f)
{
	idCheck();

	dir       = cwd = thecwd;
	scursor   = true;
	dspalways = true;
}

FileMenu::FileMenu(const String &l, Int nf, Int nc, Int fo, Int co,
		const String &thecwd)
:	ScrollMenu(l, NULL, nf, nc, fo, co),
	dv(*tr_new DirView),
	file(NULL_STRING)
{
	idCheck();

	dir       = cwd = thecwd;
	scursor   = true;
	dspalways = true;
}

FileMenu::FileMenu(Int nf, Int nc, Int fo, Int co,
					const String &thecwd, const String &pfile)
:	ScrollMenu(NULL, nf, nc, fo, co),
	dv(*tr_new DirView),
	file(pfile)
{
	idCheck();

	dir       = cwd = thecwd;
	scursor   = true;
	dspalways = true;
}

FileMenu::~FileMenu()
{
	idCheck();

	DirView *pdv = &dv;
	tr_delete(pdv);
}

void FileMenu::setDir(const String &s)
{
	idCheck();

	// el directorio es igual al existente
	if (dir == s) return;

	if (s) dir = s;
	else dir = cwd;

	// si la window esta hide entonces me voy
	if (wi->hidden()) return;

	fillItems();

	fillWindow();
}

void FileMenu::setFile(const String &re)
{
	idCheck();

	// es el misma que ya tenia
#ifndef HAVE_USERS
	if (file == re.toUpper()) return;
	file = re.toUpper();
#else
	if (file == re) return;
	file = re;
#endif

	// si la window esta hide entonces me voy
	if (wi->hidden()) return;

	fillItems();

	fillWindow();
}

Int FileMenu::execute(UnChar c)
{
	idCheck();

	if (!active || !display())
		return REJECT_EXIT;

	Int ret;
	if ((ret = ScrollMenu::execute(c)) == ACCEPT && action != NULL)
				 ret = action->execute();

	return ret;
}

// fin clase FileMenu

// clase DirMenu

void DirMenu::fpRead(Int nro, String &str, String &secStr, Int &attr)
{
	idCheck();

	static GidT grpList[MAX_GROUPS];
	static int ngrps = ERR;

	if (ngrps == ERR) {
		grpList[0] = getegid();
		ngrps = 1;
#ifdef HAVE_GETGROUPS
		int ng;
		if ((ng = getgroups(MAX_GROUPS-1, &grpList[1])) != ERR)
			ngrps += ng;
#endif
	}

	if (dv.dir() == HIGH_VALUE || dv.dir() != dir)
		dv.reset(dir, NULL_STRING, DV_DIRFILES|DV_PARENTFILE,
				DV_BYNAME);

	String nm;
	Int sz, md, uid, gid;
	dv.elem(nro, nm, sz, md, uid, gid);
	if (nm) {
		char temp[2];

		temp[1] = '\0';

#ifndef HAVE_USERS
		if (!canSearch(md, 0, 0, 0, NULL, 0))
#else
		if (!canSearch(md, uid, gid, geteuid(), grpList, ngrps))
#endif

			temp[0] = NORD_CHAR;
		else {
#ifndef HAVE_USERS
			bool rd = canRead(md, 0, 0, 0, NULL, 0);
			bool wr = canWrite(md, 0, 0, 0, NULL, 0);
#else
			bool rd = canRead(md, uid, gid, geteuid(), grpList, ngrps);
			bool wr = canWrite(md, uid, gid, geteuid(), grpList, ngrps);
#endif
			if (rd) {
				/*
					Everything OK
				*/
				if (wr)
					temp[0] = RDWR_CHAR;
				/*
					Read only
				*/
				else
					temp[0] = RDONLY_CHAR;
			} else
				/*
					Cannot read, can't do anything
				*/
				temp[0] = NORD_CHAR;
		}

		secStr = temp;
		str = nm;
		attr = A_NORMAL;
		return;
	}

	str = HIGH_VALUE;
	secStr = HIGH_VALUE;
}

// metodos publicos

DirMenu::DirMenu(const String &l, FileMenu &fm, Int nf, Int nc,
				 Int fo, Int co, const String &thecwd)
:	ScrollMenu(l, NULL, nf, nc, fo, co),
	dv(*tr_new DirView)
{
	idCheck();

	fmenu     = &fm;
	dir       = cwd = thecwd;
	scursor   = true;
	dspalways = true;
}

DirMenu::DirMenu(FileMenu &fm , Int nf, Int nc, Int fo, Int co,
				const String &thecwd)
:	ScrollMenu(NULL, nf, nc, fo, co),
	dv(*tr_new DirView)
{
	idCheck();

	fmenu     = &fm;
	dir       = cwd = thecwd;
	scursor   = true;
	dspalways = true;
}

DirMenu::DirMenu(Int nf, Int nc, Int fo, Int co, const String &thecwd)
:	ScrollMenu(NULL, nf, nc, fo, co),
	dv(*tr_new DirView)
{
	idCheck();

	fmenu     = NULL;
	dir       = cwd = thecwd;
	scursor   = true;
	dspalways = true;
}

DirMenu::~DirMenu()
{
	idCheck();

	DirView *pdv = &dv;
	tr_delete(pdv);
}

void DirMenu::setDir(const String &s)
{
	idCheck();

	// el directorio es igual al existente
	if (dir == s) return;

	if (s) dir = s;
	else dir = cwd;

	// si la window esta hide entonces me voy
	if (wi->hidden()) return;

	fillItems();

	fillWindow();
}

String DirMenu::buildDirStr(const String &s)
{
	idCheck();

    // Check if it's an absolute directory
    if (isAbsolutePath(s))
    	return s;

    String newdir;

	if (s == "..") {

		if (dir != String(PATH_SLASH)) {
			Int len = dir.length()-
					  String(dir.field(-1, String(PATH_SLASH))).length()-1;
			newdir = dir(0, len ? len : 1);
		}
	} else {
		if (dir == String(PATH_SLASH))
			newdir = dir + s;
		else
			newdir = dir + PATH_SLASH + s;
	}
	return newdir;
}

Int DirMenu::execute(UnChar c)
{
	idCheck();

	if (!active || !display())
		return REJECT_EXIT;

	String s, s1;
	Int attr;
	Int		ret;
	bool 	undisplay1 = wi->hidden();
	bool 	undisplay2 = false;

	if (fmenu) {
		if (fmenu->wi->hidden())
			undisplay2 = true;
		fmenu->display();
	}

	while ((ret = ScrollMenu::execute(c)) == ACCEPT && lastkey == K_ENTER) {
		fpRead(opt, s, s1, attr);

		// toDo: Habria que hacer un stat() pero mientras...

		if (s1[0] != NORD_CHAR) {
			String newdir=buildDirStr(s);
			if (newdir != NULL_STRING)
				setDir(newdir);

			if (fmenu)
				fmenu->setDir(dir);

			if (action != NULL)
				 ret = action->execute();
		}
	}

	if (undisplay1) unDisplay();
	if (undisplay2) fmenu->unDisplay();

	return ret;
}

// fin clase DirMenu
