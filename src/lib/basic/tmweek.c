/********************************************************************
* Copyright (c) 1987 InterSoft Co.  All Rights Reserved
* THIS IS UNPUBLISHED PROPRIETARY SOURCE CODE OF InterSoft Co.
* The copyright notice above does not evidence any
* actual or intended publication of such source code.
*
* $Id: tmweek.c,v 1.1.1.1 1996/04/16 18:52:00 gustavof Exp $
*
* DESCRIPTION
* Module Library. Routine to return the number of week in the year.
*
* ROUTINE       |  PURPOSE
*---------------+---------------------------------------------------
*	Week		| Return the number of week in the year.
*********************************************************************/

#include <ideafix.h>

/*
* Week		| Return the number of week into the year*10000 + year.
*/

long Week(DATE d)
{
	DATE first, monday;
	short year;

	/*
	* Calc. the first day of the year
	*/
	DToDMY(d, NULL, NULL, &year);
	first = DMYToD(1, 1, year);
	/*
	* found the first Monday of the year
	*/
	monday = (first+5)/7;
	monday = (DATE) (7*monday + 1);

	if (d >= monday) {
		d -= monday;
		return (long) (d/7+1) * 10000L + year;
	}
	return 530000L + year - 1;
}
