/********************************************************************
* Copyright (c) 1987 InterSoft Co.  All Rights Reserved
* THIS IS UNPUBLISHED PROPRIETARY SOURCE CODE OF InterSoft Co.
* The copyright notice above does not evidence any
* actual or intended publication of such source code.
*
*
* $Id: ststrtxt.c,v 1.1.1.1 1996/04/16 18:52:03 gustavof Exp $
*
*
* DESCRIPTION:
*
* ROUTINE       |  PURPOSE
*---------------+---------------------------------------------------
* StrTxt
*********************************************************************/

#include <ideafix.h>

char *StrTxt(char *s, int n)
{
	static char *last = NULL;
	register char *x, *y;

	x = (s == NULL || *s == '\0') ? last : s;
	if (x == NULL || *x == '\0')
		return "";

	for (y = x; *y && y - x < n; ++y)
		;

	if (*y) {
		while (*y != ' ' && y > x)
			--y;

		if (y>x) {
			*y = '\0';
			last = y+1;
		} else
			last = x+n;
	} else
		last = y;

	return x;
}
