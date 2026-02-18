/********************************************************************
* Copyright (c) 2026 Master_Solutions S.R.L.  All Rights Reserved
* THIS IS UNPUBLISHED PROPRIETARY SOURCE CODE OF Master_Solutions S.R.L.
* The copyright notice above does not evidence any
* actual or intended publication of such source code.
*
* ofdfiles.cc,v 1.1.1.1 1996/04/16 18:52:43 gustavof Exp
*
* DESCRIPTION
* Ideafix Stream file (fixed fd) class implementation
*********************************************************************/

#ifdef __GNUC__
#pragma implementation
#endif

#include <unistd.h>

#include <ifound.h>
#include <ifound/itypes.h>
#include <local/ofdfiles.h>
#include <ifound/stdc.h>
#include <ifound/terminal.h>
#ifndef HAVE_TTY
#define getChar getchar
#endif
#ifdef HAVE_UNDERSCORE_NAMES
#	define	write _write
#endif

// OutputFdFile implementation

OutputFdFile::OutputFdFile(short ffd, short bufsize)
: OutputStream(bufsize)
{
	fd = ffd;
}

void OutputFdFile::flush()
{
	if (state & IO_ERROR)
		return;

	int n = ptr - base;
	if (n && write(fd, ptr = base, n) != n)
		state |= IO_ERROR;
	else
		cnt = size - 1;
	return;
}

void OutputFdFile::endMsg()
{
	*this << endl;
}

#ifdef IDEAWIN
#include <ixwi/wi.h>
#endif

Int OutputFdFile::pause()
{
#ifdef IDEAWIN
	return WiDialog(WD_OK,WD_OK,"",toCharPtr(libMsg("HITANY")));
#else
	*this << libMsg("HITANY") << endl;
#endif
	return getChar();
}

Int OutputFdFile::pause(const String &o1, const String &o2)
{
	char hit_key;
#ifdef IDEAWIN
	return WiDialog(WD_OK|WD_CANCEL,WD_OK,"",toCharPtr(o1))==WD_OK;
#else

	*this << o1 << ' ' << o2 << endl;

	do {
		hit_key = getChar();
	} while (hit_key != o1[0] && hit_key != o2[0]);
#endif
	return (hit_key == o1[0])? 0 : 1;
}

// end of fdfiles
