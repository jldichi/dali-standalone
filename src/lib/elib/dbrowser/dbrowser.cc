/********************************************************************
* Copyright (c) 2026 Master_Solutions S.R.L.  All Rights Reserved
* THIS IS UNPUBLISHED PROPRIETARY SOURCE CODE OF Master_Solutions S.R.L.
* The copyright notice above does not evidence any
* actual or intended publication of such soucone code.
*
* $Id: dbrowser.cc,v 1.5 1998/05/14 13:35:30 pablor Exp $
*
* DESCRIPTION
*********************************************************************/

#ifdef __GNUC__
#pragma implementation
#endif

#include <ifound.h>
#include <local/dbrowser.h>
#include <ifound/itypes.h>
#include <ifound/parray.h>
#include <ifound/dirdefs.h>


#ifdef HAVE_DOS_H
#include <dos.h>
#endif


#include <sys/stat.h>

#ifdef HAVE_UNDERSCORE_NAMES
#	define stat _stat
#endif

DirBrowser::DirBrowser(const String &d, RegExp const *re)
:	rexp(NULL),
	dirp(NULL)
{
	reset(d, re);
}

DirBrowser::~DirBrowser()
{
#if HAVE_OPENDIR
	if (dirp)
		closedir(dirp);
#elif HAVE_FIND
#ifdef __NT__
	if(dirp != INVALID_HANDLE_VALUE)
		FindClose(dirp);
#endif
#endif
}

void DirBrowser::reset()
{
	reset(dir, rexp);
}

void DirBrowser::setDir(const String &d)
{
	reset(d, rexp);
}

void DirBrowser::setFiles(RegExp const *re)
{
	reset(dir, re);
}

void DirBrowser::info(Int &mode, Int &size, Int &uid, Int &gid,
		Time &mTime)
{
	struct stat st;
	statFile(fullName(), &st);
	mode = Int(st.st_mode);
	size = Int(st.st_size);
	uid = Int(st.st_uid);
	gid = Int(st.st_gid);
	mTime = osTime(st.st_mtime);
}

void DirBrowser::reset(const String &d, RegExp const *re)
{
	if (!d)
		dir = ".";
	else
		dir = d;

	rexp = (RegExp *) re;
	assert(rexp != NULL);

#if HAVE_OPENDIR
	if (dirp)
		closedir(dirp);

	if ((dirp = opendir((char *) toCharPtr(dir))) != NULL) {
		if ((dp = readdir(dirp)) != NULL) {
			st = D_OK;
			bool matches = rexp->match(dp->d_name);
			while (st == D_OK && !matches) {
				_next();
				if (st == D_OK)
					matches = rexp->match(dp->d_name);
			}
		} else {
			st = D_EOF;
			closedir(dirp);
			dirp = NULL;
		}
	} else
		st = D_ERROR;
#elif __NT__
	if(dirp)
		FindClose(dirp);
	String tmp = dir + String(PATH_SLASH) + "*.*";
	if ((dirp = FindFirstFile(toCharPtr(tmp), &fdata)) != INVALID_HANDLE_VALUE) {
		st = D_OK;
		bool matches = rexp->match(name());
		while (st == D_OK && !matches) {
			_next();
			if (st == D_OK)
				matches = rexp->match(name());
		}
	}
	else{
		st = D_ERROR;
		dirp = NULL;
	}
#elif HAVE_FIND
	String tmp = dir + String(1, PATH_SEP[0]) + "*.*";
	if ((dirp = findfirst(toCharPtr(tmp), 0)) != NULL) {
		st = D_OK;
		bool matches = rexp->match(dirp->name);
		while (st == D_OK && !matches) {
			_next();
			if (st == D_OK)
				matches = rexp->match(dirp->name);
		}
	} else
		st = D_ERROR;
#else
	String tmp = dir + String(1, PATH_SEP[0]) + "*.*";
	if (findfirst(toCharPtr(tmp), &dirp, 0) == 0) {
		st = D_OK;
		bool matches = rexp->match(dirp.ff_name);
		while (st == D_OK && !matches) {
			_next();
			if (st == D_OK)
				matches = rexp->match(dirp.ff_name);
		}
	} else
		st = D_ERROR;
#endif
}

void DirBrowser::_next()
{
#if HAVE_OPENDIR
	assert(dirp != NULL);
	if ((dp = readdir(dirp)) == NULL) {
		st = D_EOF;
		closedir(dirp);
		dirp = NULL;
	}
#elif __NT__
	assert(dirp != NULL);
	if(!FindNextFile(dirp,&fdata)){
			st=D_EOF;
			FindClose(dirp);
			dirp = NULL;
	}
#elif HAVE_FIND
	if ((dirp = findnext()) == NULL) {
		st = D_EOF;
	}
#else
	if (findnext(&dirp) != 0) {
		st = D_EOF;
	}
#endif
}

void DirBrowser::next()
{
	bool matches = false;
	do {
		_next();
		if (st == D_OK)
			matches = rexp->match(name());
	} while (st == D_OK && !matches);
}

DirBrowser::operator void *()
{
	return (st == D_OK) ? this : NULL;
}

String DirBrowser::name() const
{
	if (st != D_OK)
		return NULL_STRING;

#if HAVE_OPENDIR
	return dp->d_name;
#elif __NT__
	return fdata.cFileName;
#elif HAVE_FIND
	return dirp->name;
#else
	return dirp.ff_name;
#endif
}

String DirBrowser::fullName() const
{
	if (st != D_OK)
		return NULL_STRING;
	String tmp = dir;
	if (tmp[tmp.length()-1] != PATH_SLASH[0])
		tmp += PATH_SLASH;
	tmp += name();
	return tmp;
}
