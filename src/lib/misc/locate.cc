/********************************************************************
* Copyright (c) 1987 InterSoft Co.  All Rights Reserved
* THIS IS UNPUBLISHED PROPRIETARY SOURCE CODE OF InterSoft Co.
* The copyright notice above does not evidence any
* actual or intended publication of such source code.
*
* $Id: locate.cc,v 1.1.1.1 1996/04/16 18:51:57 gustavof Exp $
*
* DESCRIPTION
*
* ROUTINE	|	PURPOSE
*-----------+--------------------------------------------------------
* Locate	|	Use binary search to find a datum in an ordered array
*********************************************************************/

#include <ideafix.h>

int locate(const char *dato, const char *tabla, UnInt nel,
		UnInt size, IFPCPCP comp)
{
	register UnInt inc, ind;
	register int msg = 1;

	for (inc = 1; inc <= nel; inc <<= 1)
		;
	ind = (inc >>= 1) - 1;
	nel--;

	while (inc) {
		inc >>= 1;
		msg = ind > nel ? -1 : comp((char *) dato, (char *) tabla+ind*size);
		if (!msg) return (int) ind;
		if (msg > 0) ind += inc;
		else         ind -= inc;
	}
	return ~((int) ind + (msg > 0));
}
