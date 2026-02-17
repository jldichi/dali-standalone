/********************************************************************
* Copyright (c) 1987 InterSoft Co.  All Rights Reserved
* THIS IS UNPUBLISHED PROPRIETARY SOURCE CODE OF InterSoft Co.
* The copyright notice above does not evidence any
* actual or intended publication of such source code.
*
*
* $Id: stdlen.c,v 1.1.1.1 1996/04/16 18:52:02 gustavof Exp $
*
*
* DESCRIPTION:
*
* ROUTINE       |  PURPOSE
*---------------+---------------------------------------------------
*StrDspLen
*********************************************************************/

#include <ideafix.h>

int StrDspLen(register char *p)
{
	register int i = 0;

	while (*p) {
		if (isprint((UChar) *p)) ++i;
		else if (*p == '\033') {
			while (*p && !isalpha((UChar) *p) ) ++p;
			if (!*p) break;
		}
		++p;
	}
	return i;
}
