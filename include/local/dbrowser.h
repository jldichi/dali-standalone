/***********************************************************************
* Copyright (c) 1991 InterSoft Co.  All Rights Reserved
* THIS IS UNPUBLISHED PROPRIETARY SOURCE CODE OF InterSoft Co.
* The copyright notice above does not evidence any
* actual or intended publication of such source code.
*
* $Id: dbrowser.h,v 1.3 1998/05/19 20:55:11 eduardoc Exp $
*
* DESCRIPTION
* Class definitions for the Directory Browser data type
*********************************************************************/
#ifndef DBROWSER_H
#define DBROWSER_H

#ifdef __GNUC__
#pragma interface
#endif

#include <ifound.h>
#ifdef __NT__
#undef ClosePrinter
#undef CreateCursor
#include <windows.h>
#include <windowsx.h>
#endif

#include <ifound/str.h>
#include <ifound/rexp.h>
#include <ifound/dirdefs.h>

#define D_OK		0
#define D_ERROR		1
#define D_EOF		2

class Time;

class DirBrowser {

// private:

	String dir;
	RegExp *rexp;
#if HAVE_OPENDIR
	DIR *dirp;
	dirent *dp;
#elif __NT__
	WIN32_FIND_DATA fdata;	
	HANDLE dirp;
#elif HAVE_FIND
	struct FIND *dirp;
#else
	struct ffblk dirp;
#endif
	Int st;

	virtual void _next();

	DirBrowser(const DirBrowser &);
	void operator=(const DirBrowser &);                  

public:

	DirBrowser(const String &d, RegExp const *re);
	virtual ~DirBrowser();

	virtual void reset(const String &d, RegExp const *re);
	virtual void reset();
	virtual void setDir(const String &d);
	virtual void setFiles(RegExp const *re);
	virtual void next();
	void operator++()							{ next(); }
	void operator++(int)						{ next(); }
	virtual operator void *();
	virtual String name() const;
	virtual String fullName() const;
	virtual void info(Int &mode, Int &size, Int &uid, Int &gid,
			Time &modTs);

};

#endif
