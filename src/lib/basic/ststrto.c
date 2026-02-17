/********************************************************************
* Copyright (c) 1987 InterSoft Co.  All Rights Reserved
* THIS IS UNPUBLISHED PROPRIETARY SOURCE CODE OF InterSoft Co.
* The copyright notice above does not evidence any
* actual or intended publication of such source code.
*
* $Id: ststrto.c,v 1.2 2004/12/20 17:09:59 damianm Exp $
*
* DESCRIPTION
* String convertions routines Library module
*
* ROUTINE       |  PURPOSE
*---------------+---------------------------------------------------
* StrToI		| Convert string to integer.
* StrToL		| Convert string to long.
* StrToF		| Convert string to double.
*********************************************************************/

#include <stdlib.h>

#include <ideafix.h>
#include <ideafix/priv/gndefs.h>

/* ++ Prototypes ++ */

static int _Special(char *s);

/* -- Prototypes -- */

/*
 * Convert Special values ( DAY, MONTH, YEAR )
 */
static int _Special(char *s)
{
	short day, month, year;

	DToDMY(Today(), &day, &month, &year);
	if (str_eq(s, DAY_STR)) return day;
	if (str_eq(s, MONTH_STR)) return month;
	if (str_eq(s, YEAR_STR)) return year;
	return 0;
}

/*
 * Convert string to short.
 */
short StrToI(char *s)
{
	if (!s)
		return NULL_SHORT;

	return s == HIGH_VALUE || str_eq(s, HIGH_VAL_IMAGE) ? MAX_SHORT
		: !s[0] ? NULL_SHORT
		: isalpha((UChar) s[0]) ? (short) _Special(s)
		: (short) atoi(s);
}

/*
 * Convert string to long.
 */
long StrToL(char *s)
{
	if (!s)
		return NULL_LONG;

	return s == HIGH_VALUE || str_eq(s, HIGH_VAL_IMAGE) ? MAX_LONG
		: !s[0] ? NULL_LONG
		: isalpha((UChar) s[0]) ? _Special(s)
		: atol(s);
}


/*
 * Convert string to double.
 */
double StrToF(char *s)
{
	if (!s)
		return NULL_DOUBLE;

	if (s == HIGH_VALUE || str_eq(s, HIGH_VAL_IMAGE)) 
		return MAX_DOUBLE;

	if (!s[0])
		return NULL_DOUBLE;

	if (isalpha((UChar)s[0]))
		return (double) _Special(s);

	{
		double faux;
		char *pnt;

		if ((pnt = strchr(s, ',')) != NULL)
			*pnt = '.';
		faux = atof(s);
		if (pnt != NULL)
			*pnt = DECPOINT();
		return faux;
	}
}
