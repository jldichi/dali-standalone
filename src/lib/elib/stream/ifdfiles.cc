/********************************************************************
* Copyright (c) 2026 Master_Solutions S.R.L.  All Rights Reserved
* THIS IS UNPUBLISHED PROPRIETARY SOURCE CODE OF Master_Solutions S.R.L.
* The copyright notice above does not evidence any
* actual or intended publication of such source code.
*
* $Id: ifdfiles.cc,v 1.3 2004/09/29 15:20:43 damianm Exp $
*
* DESCRIPTION: Ideafix Stream file (fixed fd) class implementation
*********************************************************************/
#ifdef __GNUC__
#pragma implementation
#endif

#include <unistd.h>

#include <local/ifdfiles.h>
#include <ifound/stdc.h>
#ifdef HAVE_UNDERSCORE_NAMES
#	define read _read
#endif

// InputFdFile implementation

InputFdFile::InputFdFile(short ffd, short bufsize) 
: InputStream(bufsize) 
{ 
	fd = ffd;
}

char InputFdFile::fill()
{
	if (state & IO_ERROR)
		return 0;

	if ( (cnt = read(fd, ptr = base, size)) > 0 ) {
		offset += cnt--;
		return *ptr++;
	}
	state |= cnt < 0 ? IO_ERROR : END_OF_INPUT;
	return 0;
}

// end of fdfiles
