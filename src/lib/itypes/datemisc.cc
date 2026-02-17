/********************************************************************
* Copyright (c) 1991 InterSoft Co.  All Rights Reserved
* THIS IS UNPUBLISHED PROPRIETARY SOURCE CODE OF InterSoft Co.
* The copyright notice above does not evidence any
* actual or intended publication of such source code.
*
* $Id: datemisc.cc,v 1.7 2004/09/29 15:08:40 damianm Exp $
*
* DESCRIPTION:
*				Implementation of Date class
*				Biggest date : 31/12/142000
*				Smallest date : 1/1/-100000
*
*********************************************************************/
#include <string.h>
#include <stdlib.h>

#include <ifound.h>
#include <ifound/itypes.h>
#include <ifound/stdc.h>
#include <ifound/priv/timedefs.h>
#include <ifound/priv/gfuncs.h>

// Useful constant: days between 1/1/1970 and 1/1/1984
#ifdef TIME_FROM68
#define DATE_OFFSET		5844L
#else
#define DATE_OFFSET		5113L
#endif
#define SECS_PER_DAY	86400L

#ifdef IDEAWIN
#define DATE_SEP_ "\n"
#define ST_EXT_ "st"
#define ND_EXT_ "nd"
#define TH_EXT_ "th"
#endif

static inline LDATE SDATE(long d, long m, long y)
{
	return d + long(31) * (m + long(12) * y);
}

// Translate from number of seconds since 1/1/1970 (ostime) to
// IdeaFix time units


void _date_to_dmy(LDATE idate, long *day, long *month, long *year)
{
	long year_corr = 4715;
	long y, m, aux;

	assert(day != NULL && month != NULL && year != NULL);

	// Consider date boundaries

	if (long(idate) == LONG_MIN) {
		*day = *month = *year = INT_MIN;
		return;
	}

	if (long(idate) == LONG_MAX) {
		*day = *month = *year = INT_MAX;
		return;
	}

	if (idate >= -146540) {
		aux = (4*idate + 2313939) / 146097;
		idate += 1 + aux - aux/4;
	}

	// unpack it, it's a normal date

	idate += 2447225;
	if (idate < 0) {
		idate += 1000 * 36525;
		year_corr += 100000;
	}
	y   = (40*idate - 4884) / 14610;
	aux = (1461*y) / 4;
	m   = (10000 * (idate-aux)) / 306001;

	*day   = long(idate - aux - (306001 * m) / 10000);
	*month = long(m - (m <= 13 ? 1 : 13));
	*year  = long(y - year_corr - (*month > 2));
}

LDATE _dmy_to_date(long day, long month, long year)
{
	long b = 0, s_month = month, s_year = year;
	LDATE idate;		// packed date

	if (day < 1 || month < 1 || year < -100000)
		return LONG_MIN;

	if (day > 31  || month > 12 || year > 142000)
		return LONG_MAX;

	if (month <= 2) {
		year--;
		month += 12;
	}

	if (SDATE(day, month, year) >= SDATE(15,10,1582)) {
		long a = year/100;
		b = 2 - a + a / 4;
	}

	idate = (1461L * year - (year>0 ? 0 : 3)) / 4
			+ (306001L * (month + 1)) / 10000 + day + b - 724706L;

	// check it's a valid date, else it's null date, unpack and compare

	long c_day, c_month, c_year;

	_date_to_dmy(idate, &c_day, &c_month, &c_year);

	if (c_day != day || c_month != s_month || c_year != s_year)
		return LONG_MIN;		// invalid date
	else
		return idate;
}

LDATE _str_to_date(const char *sd, DateFmt fmt)
{
	// handle special cases

	if (sd == HIGH_VALUE) return LONG_MAX;
	if (sd == LOW_VALUE || *sd == '\0') return LONG_MIN;
	if (!strcmp(sd, TODAY)) return _osdate_to_date(_osTime());

	// parse a date with the following format ddmm[-]yyyyyy or
	// [d]d/[m]m/[-]yyyyyy

	const char *q1e = NULL, *q2b = NULL, *q2e = NULL, *q3b = NULL;
	long vals[3];
	if ((q1e = strchr(sd, '/')) != NULL) {
		q2b = q1e+1;
		if ((q2e = strchr(q2b, '/')) != NULL)
			q3b = q2e+1;
	}
	else {
		q1e = sd + 2;
		q2b = q1e;
		q2e = q2b + 2;
		q3b = q2e;
	}
	if (q2b == NULL || q3b == NULL)
		vals[0] = vals[1] = vals[2] = 0;
	else {
		int ncopied;
		char cb[3];
		ncopied = q1e-sd;
		if (ncopied > 2)
			ncopied = 2;
		strncpy(cb, sd, ncopied);
		cb[ncopied] = '\0';
		vals[(fmt == INTER_FMT) ? 0 : 1] = atol(cb);
		ncopied = q2e-q2b;
		if (ncopied > 2)
			ncopied = 2;
		strncpy(cb, q2b, ncopied);
		cb[ncopied] = '\0';
		vals[(fmt == INTER_FMT) ? 1 : 0] = atol(cb);
		vals[2] = atol(q3b);

		static Int century = ERR;
		static Int wrap = ERR;

		if (century == ERR) {

			String wrapStr = String("CENTURY_WRAP").getEnv();

			if (!wrapStr) {
				//
				// We use as default wrapstr today's year + 20 years.
				// This is because we normally work with 20 years
				// ahead and 80 years backwards.
				//
				Date tday = Date(NOW);
				wrapStr = toString(tday.year() + 20);
			}

			Int len = wrapStr.length();
			century = toInt(wrapStr(0, len - 2)) * 100;
			wrap = toInt(wrapStr(len - 2));

		}

		// The computation of the year follows the following rules:
		// We call WRAP to the last 2 digits of 'CENTURY_WRAP' and
		// we call CENTURY to the first 2 digits * 100.
		// if the year string has length 2, these assumptions are made
		// [0, WRAP]	-> CENTURY century assumed else
		// [WRAP+1, 99]	-> CENTURY - 100 century assumed

		if (strlen(q3b) <= 2)
			if (vals[2] <= wrap)
				vals[2] += century;
			else
				vals[2] += century - 100;
	}
	return _dmy_to_date(vals[0], vals[1], vals[2]);
}

const char *_date_to_str(LDATE l, char *pbuf, DateFmt fmt)
{
	char *buf = pbuf;
	char sep[50];

	if (l == LONG_MIN) return LOW_VALUE;
	if (l == LONG_MAX) return HIGH_VALUE;
	Date d = toDate(Int(l));

	sep[0] = '\0';
	if ((fmt & LMONTHNAME_FMT) || (fmt & LDAYNAME_FMT)) {
#ifdef IDEAWIN
        String msg(DATE_SEP_);
#else
        String msg = libMsg("DATE_SEP");
#endif

		if (!msg.isNull()) {
			strcpy(sep, toCharPtr(msg));
		}
	}
    else if (fmt & SEPAR_FMT)
		strcpy(sep, "/");

	if (fmt & USA_FMT) {
		if (fmt & MONTHNAME_FMT || fmt & LMONTHNAME_FMT) {
			(void) strcpy(buf, toCharPtr(d.dayOfWeekName()));
			buf += fmt & LMONTHNAME_FMT ? strlen(buf) : 3;
			(void) sprintf(buf, ",");
			buf += 1;
		} else {
			(void) strcpy(buf, toCharPtr(d.month(), -2));
			buf += strlen(buf);
		}
	} else {
		if (fmt & DAYNAME_FMT || fmt & LDAYNAME_FMT) {
			(void) strcpy(buf, toCharPtr(d.dayOfWeekName()));
			buf += fmt & LDAYNAME_FMT ? strlen(buf) : 3;
			(void) sprintf(buf, " %02d", d.day());
			buf += 3;
		} else {
			(void) strcpy(buf, toCharPtr(d.day(), -2));
			buf += strlen(buf);
		}
	}

	strcpy(buf, sep);
	buf += strlen(sep);
	if (fmt & DAYNAME_FMT || fmt & LDAYNAME_FMT) {
		strcpy(buf, " ");
		buf += strlen(" ");
	}

	if (fmt & USA_FMT) {
		if (fmt & DAYNAME_FMT || fmt & LDAYNAME_FMT) {
			char *ext;
			(void) strcpy(buf, toCharPtr(d.monthName()));
			buf += fmt & LDAYNAME_FMT ? strlen(buf) : 3;
#ifdef IDEAWIN
			if ((d.day() % 10) == 1) ext = toCharPtr(String(ST_EXT_));
			else if ((d.day() % 10) == 2) ext = toCharPtr(String(ND_EXT_));
			else ext = toCharPtr(String(TH_EXT_));
#else
			if ((d.day() % 10) == 1) ext = toCharPtr(libMsg("ST_EXT"));
			else if ((d.day() % 10) == 2) ext = toCharPtr(libMsg("ND_EXT"));
			else ext = toCharPtr(libMsg("TH_EXT"));
#endif
			sprintf(buf, " %02d%s", d.day(), ext);
			buf += 3+strlen(ext);
		} else {
			(void) strcpy(buf, toCharPtr(d.day(), -2));
			buf += strlen(buf);
		}
	} else {
		if (fmt & MONTHNAME_FMT || fmt & LMONTHNAME_FMT) {
			(void) strcpy(buf, toCharPtr(d.monthName()));
			buf += fmt & LMONTHNAME_FMT ? strlen(buf) : 3;
		} else {
			(void) strcpy(buf, toCharPtr(d.month(), -2));
			buf += strlen(buf);
		}
	}

	strcpy(buf, sep);
	buf += strlen(sep);
	if (fmt & DAYNAME_FMT || fmt & LDAYNAME_FMT) {
		strcpy(buf, " ");
		buf += strlen(" ");
	}

	if (fmt & YEAR2_FMT)
		strcpy(buf, toCharPtr(d.year() % 100, -2));
	else
		strcpy(buf, toCharPtr(d.year()));

	return pbuf;
}

long _date_to_osdate(LDATE idate)
{
	return (idate + DATE_OFFSET) * SECS_PER_DAY;
}

LDATE _osdate_to_date(long s)
{
	return (s / SECS_PER_DAY) - DATE_OFFSET;
}

// end of date
