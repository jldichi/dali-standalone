/********************************************************************
* Copyright (c) 1987 InterSoft Co.  All Rights Reserved
* THIS IS UNPUBLISHED PROPRIETARY SOURCE CODE OF InterSoft Co.
* The copyright notice above does not evidence any
* actual or intended publication of such source code.
*
* $Id: tmfday.c,v 1.1.1.1 1996/04/16 18:52:00 gustavof Exp $
*
* DESCRIPTION
* Rutinas de manejo de DATE.                      
*
* ROUTINE       |  PURPOSE
*---------------+---------------------------------------------------
*FirstMonthDay	Return the date of the first day in a month.
*LastMonthDay	Return the date of the last day in a month.
*      Module Library. Routine to return the number of
*      HalfMonth84 base at year 1984.
*********************************************************************/

#include <ideafix.h>

DATE FirstMonthDay(DATE f)
{
	short month, year;
	DToDMY(f, NULL, &month, &year);
	return DMYToD(1, month, year);
}

DATE LastMonthDay(DATE f)
{
	short month, year;

	if (f == MIN_DATE || f == MAX_DATE)
		return NULL_DATE;

	DToDMY(f, NULL, &month, &year);
	if (++month == 13) {
		year++;
		month = 1;
	}
	return DMYToD(1, month, year) - 1;
}
/*
* return the number of fortnigth base at year 1984.
*/

int HalfMonth84(DATE d)
{
	short day, month, year;
	DToDMY(d, &day, &month, &year);
    return (day>15) + 2 * (month-1) + 24*(year-1984);
}
