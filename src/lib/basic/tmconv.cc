/********************************************************************
* Copyright (c) 1987 InterSoft Co.  All Rights Reserved
* THIS IS UNPUBLISHED PROPRIETARY SOURCE CODE OF InterSoft Co.
* The copyright notice above does not evidence any
* actual or intended publication of such source code.
*
* $Id: tmconv.cc,v 1.5 2004/09/29 16:16:09 damianm Exp $
*
* DESCRIPTION
*
* ROUTINE       |  PURPOSE
*---------------+---------------------------------------------------
* SetDateFmt	|	Set the format of the date flds
* GetDateFmt	|	Return the format used for dates
* DayName		|	Return the day name.
* MonthName		|	Return the month name.
* StrToD		|	Convert string to DATE.
* DToStr		|	Convert DATE to string.
* StrNXToD		|	Convert a string to date witout expand STR_TODAY
* DMYToD		|	Convert the strings date,month & year to DATE format
* DToDMY		|	The inverse function of DMYToD
* Month(DATE)	|	Return the month number.
* Day(DATE)		|	Return the day number.
* Year(DATE)	|	Return the year number.
* StrToT		|	Convert ASCII to TIME.
* TToStr		|	Convert TIME to ASCII.
* StrNXToT		|	Conver ASCII to TIME but no expand HOUR_STR
*********************************************************************/

#include <string.h>

#include <ideafix.h>
#include <cfix.h>
#include <cfix/priv/macros.h>
#include <ideafix/priv/gndefs.h>
#include <ifound/priv/timedefs.h>
#include <ifound/priv/gfuncs.h>

/* ++ Prototypes ++ */

static short two_char_int(char **s);
char *DayName(DATE n);
DATE DMYToD(short day, short month, short year);
void DToStr(DATE ifecha, char *fecha, int format);
void DToDMY(DATE ifecha, short *day, short *month, short *year);
short Day(DATE fecha);

/* -- Prototypes -- */



/* Variables taken from other modules                               */

extern "C" DLLDEPEND short _datefmt; /* 0 = International, 1 = American */

extern "C" void SetDateFmt(int n)
{
	_datefmt = (short) (n  ? DFMT_AMERICAN : DFMT_INTERNAT);
}

extern "C" int GetDateFmt(void)
{
	return _datefmt;
}

/*
 * Return the name of the day
 */

extern "C" char *DayName(DATE n)
{
	int m = (int)n % 7;
	if (m < 0) m += 7;
	return (char *) toCharPtr(dayOfWeekName(m));
}

// Return the name of the month
extern "C" char *MonthName(int n)
{
    return n < 1 || n > 12 ? NULL : (char *) toCharPtr(monthName(n - 1));
}

static short two_char_int(char **s)
{
	short i, n;
	for (i = n = 0; i < 2 && isdigit(**s); ++*s, i++)
		n = n * 10 + **s - '0';
	return n;
}

extern "C" DATE StrNXToD(char *fecha)
{
	return str_eq(fecha, TODAY_STR) ? TODAY_DATE : StrToD(fecha);
}

extern "C" DATE StrToD(char *fecha)
{
	LDATE ret;

	/* Special Cases */
	if (fecha == NULL)
		return NULL_DATE;

	if (fecha == HIGH_VALUE || str_eq(fecha, HIGH_VAL_IMAGE))
		return MAX_DATE;

	if (fecha[0] == '\0')
		return NULL_DATE;

	if (str_eq(fecha, TODAY_STR)) return Today();

	ret = _str_to_date(fecha,
			(_datefmt == DFMT_AMERICAN ? USA_FMT : INTER_FMT));

	return ret>MAX_DATE ? NULL_DATE
		:  ret<MIN_DATE ? NULL_DATE : (DATE) INT(ret);
}

extern "C" DATE DMYToD(short day, short month, short year)
{
	LDATE ret = _dmy_to_date((long) day, (long) month, (long) year);
	return ret>MAX_DATE ? MAX_DATE
		:  ret<MIN_DATE ? MIN_DATE : (DATE) INT(ret);
}

extern "C" void DToStr(DATE ifecha, char *fecha, int fmt)
{
	DateFmt dfmt;

	// Special cases
	if (ifecha == NULL_DATE) {
		fecha[0] = 0;
		return;
	}

	if (ifecha == TODAY_DATE) {
		if (fmt & DFMT_NEXPAND) {
			strcpy(fecha, TODAY_STR);
			return;
		}
		ifecha = Today();
	}
	/* convert flags */
	dfmt = 0;

	if (_datefmt == DFMT_AMERICAN)
		dfmt |= USA_FMT;

	if (fmt & DFMT_SEPAR)
		dfmt |= SEPAR_FMT;

	if (!(fmt & DFMT_YEAR4))
		dfmt |= YEAR2_FMT;

	if (fmt & DFMT_LMONTHNAME)
		dfmt |= LMONTHNAME_FMT;

	if (fmt & DFMT_MONTHNAME)
		dfmt |= MONTHNAME_FMT;

	if (fmt & DFMT_LDAYNAME)
		dfmt |= LDAYNAME_FMT;

	if (fmt & DFMT_DAYNAME)
		dfmt |= DAYNAME_FMT;

	_date_to_str(SDATETOL(ifecha), fecha, dfmt);
}

extern "C" void DToDMY(DATE ifecha, short *day, short *month, short *year)
{
	long d, m, y;

	_date_to_dmy(SDATETOL(ifecha), &d, &m, &y);

	if (day)
		*day = (short) d;
	if (month)
		*month = (short) m;
	if (year)
		*year = (short) y;
}

extern "C" short Day(DATE fecha)
{
	short dia;
	DToDMY(fecha, &dia, NULL, NULL);
	return dia;
}

extern "C" short Month(DATE fecha)
{
	short mes;
	DToDMY(fecha, NULL, &mes, NULL);
	return mes;
}

extern "C" short Year(DATE fecha)
{
	short anio;
	DToDMY(fecha, NULL, NULL, &anio);
	return anio;
}

// Idem StrToT pero no expande HOUR_STR a la hora actual
extern "C" TIME StrNXToT(char *s)
{
	return str_eq(s, HOUR_STR) ? HOUR_TIME : StrToT(s);
}

/*
	StrToT: Convierte ASCII a TIME 
	s   : String con una hora 
	Si el string contiene HOUR_STR, se retorna la hora actual.
*/

extern "C" TIME StrToT(char *s)
{
	register TIME t;

	/* Check for null value */
	if (s == NULL)
		return NULL_TIME;

	if (s == HIGH_VALUE || str_eq(s, HIGH_VAL_IMAGE))
		return MAX_TIME;

	if (s[0] == '\0')
		return NULL_TIME;

	if (str_eq(s, HOUR_STR))
		return Hour();

	/* Hours */
	t = two_char_int(&s) * 1800;
	if (!isdigit((UChar) s[0]))
		s++;

	/* Minutes */
	t += two_char_int(&s) * 30;

	if (s[0]) {
		/* Seconds */
		if (!isdigit((UChar) s[0]))
			s++;
		if (isdigit((UChar) s[0]))
			t += two_char_int(&s) / 2;
		else
			s--;

		if (s[0] && s[1] == 'P')
			t += 12 * 60 * 30;
	}

	/* toDo: try to see if AM/PM exists and add */

	return t + 1;
}

/*
 * TToStr: Convierte un TIME a ASCII
 *
 * t   : hora en formato interno.
 * dest: destino de la conversion.
 * fmt : formato "  "   "  "   "
 *
 */

extern "C" void TToStr(TIME t, char *dest, int fmt)
{
	register short hour = (short) HOURS(t);

	if (t == HOUR_TIME) {
		if (fmt & TFMT_NEXPAND) {
			strcpy(dest, HOUR_STR);
			return;
		}
		t = Hour();
		hour = (short) HOURS(t);
	}

	// Check for null value
	if (t == 0 || t > (TIME)(24 * 60 * 30)) {
		dest[0] = '\0';
		return;
	}
	sprintf(dest, "%02d%s%02d",	(fmt & TFMT_HS12) && hour > 12 ? hour - 12
			: hour, fmt & TFMT_SEPAR ? ":" : "", MINUTES(t));

	dest += strlen(dest);

	if (fmt & TFMT_SECONDS)
		sprintf(dest, "%s%02d", fmt & TFMT_SEPAR ? ":" : "", SECONDS(t));

	if (fmt & TFMT_HS12)
		strcat(dest, HOURS(t) > 12 ? " PM" : " AM");
}
