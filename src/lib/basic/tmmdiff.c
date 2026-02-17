/********************************************************************
* Copyright (c) 1987 InterSoft Co.  All Rights Reserved
* THIS IS UNPUBLISHED PROPRIETARY SOURCE CODE OF InterSoft Co.
* The copyright notice above does not evidence any
* actual or intended publication of such source code.
*
*
* $Id: tmmdiff.c,v 1.1.1.1 1996/04/16 18:52:00 gustavof Exp $
*
*
* DESCRIPTION:
*
* ROUTINE       |  PURPOSE
*---------------+---------------------------------------------------
*MonthDiff		|Number of months between two dates
*********************************************************************/

#include <ideafix.h>

long MonthDiff(DATE a, DATE b)
{
	short m1, m2, y1, y2;

	DToDMY(a, NULL, &m1, &y1);
	DToDMY(b, NULL, &m2, &y2);
	return (y1*12L+m1) - (y2*12L+m2);
}
