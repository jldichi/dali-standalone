/********************************************************************
* Copyright (c) 1995 InterSoft Co.  All Rights Reserved
* THIS IS UNPUBLISHED PROPRIETARY SOURCE CODE OF InterSoft Co.
* The copyright notice above does not evidence any
* actual or intended publication of such source code.
*
* $Id: ftstamp.cc,v 1.2 1998/04/24 17:14:31 mauro Exp $
*
* DESCRIPTION
*********************************************************************/

#include <ifound/itypes.h>
#include <sys/types.h>
#include <sys/stat.h>

#ifdef HAVE_UNDERSCORE_NAMES
#	define stat _stat
#endif

Time timeStamp(const String &file)
{
    Time t;
	struct stat st;
	if (statFile(file, &st) == 0)
		t = osTime(st.st_mtime);
	return t;
}
