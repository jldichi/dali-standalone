/********************************************************************
* Copyright (c) 1991 InterSoft Co.  All Rights Reserved
* THIS IS UNPUBLISHED PROPRIETARY SOURCE CODE OF InterSoft Co.
* The copyright notice above does not evidence any
* actual or intended publication of such source code.
*
* $Id: date.cc,v 1.4 1999/08/02 17:01:24 eduardoc Exp $
*
* DESCRIPTION
*				Implementation of Date class
*				Biggest date : 31/12/142000
*				Smallest date : 1/1/-100000
*********************************************************************/

#include <ifound.h>

#ifdef __GNUC__
#	pragma implementation "itypes.h"
#endif

#include <ifound/itypes.h>
#include <ifound/stdc.h>
#include <ifound/priv/timedefs.h>

static DateFmt default_fmt = INTER_FMT;

void Date::setFmt(DateFmt fmt)
{
	default_fmt = fmt;
}

DateFmt Date::getFmt()
{
	return default_fmt;
}

Date::Date(Int d, Int m, Int y)
{
	idate = _dmy_to_date(long(d), long(m), long(y));
}

Date::Date(const char *sd)
{
	idate = _str_to_date(sd, default_fmt);
}

const char *toCharPtr(Date d, DateFmt dfmt)
{
	static FmtBuff fBuff = { '\0' };
	return _date_to_str(d.idate, fBuff, default_fmt | dfmt);
}

Date toDate(Int i)
{ 
	Date d; 
	d.idate = i; 
	return d; 
}

Date toDate(Time t)
{
	Date d;
	if (t.itime == LONG_MAX) d.idate = LONG_MAX;
	else if (t.itime == LONG_MIN) d.idate = LONG_MIN;
	else d.idate = t.itime / (60L * 60L * 24L);
	return d;
}

// store results of last date conversion to perform optimizations

// idate 0 corresponds to 01/01/1984
static long last_idate = 0, last_day = 1, last_month = 1, last_year = 1984;

void Date::recalc_values() const
{
	if (idate!=last_idate) {
		last_idate = idate;
		_date_to_dmy(idate, &last_day, &last_month, &last_year);
	}
}

Int Date::day() const
{
	recalc_values();
	return last_day;
}

Int Date::month() const
{
	recalc_values();
	return last_month;
}

Int Date::year() const
{
	recalc_values();
	return last_year;
}

Int Date::dayOfWeek() const
{
	return Int(idate < 0 ? 6 - (-idate - 1) % 7 : idate % 7);
}

Int Date::dayOfYear() const
{
	recalc_values();
	Date d(1, 1, last_year);
	return idate - d.idate;
}

Int Date::weekOfYear() const
{
	recalc_values();
	Date d(1, 1, last_year);
	return (d.dayOfWeek() + dayOfYear()) / 7;
}

Int  Date::quarter() const
{
	return (month() - 1) / 3 + 1;
}

Date Date::lastOfMonth() const
{
	recalc_values();
	Int mon = last_month;
	Date d(last_day, last_month, last_year);
	while(mon == d.month()) ++d;
	return --d;
}

Date Date::firstOfMonth() const
{
	recalc_values();
	return Date(1, last_month, last_year);
}

Int Date::monthsSince(const Date b) const
{
	long d, m, y;
	_date_to_dmy(b.idate, &d, &m, &y);
	recalc_values();
	return (last_year * Int(12) + last_month) - (y * Int(12) + m);
}

Date Date::addMonths(Int mnth) const
{
	Int m, y;

	recalc_values();

	y = Int(last_year + mnth / Int(12));
	m = Int(last_month + mnth % Int(12));

	if (m > 12) {
		y++;
		m -= 12;
	} else if (m < 1) {
		// in case of negative months
		y--;
		m += 12;
	}
	return Date(last_day, m, y);
}

Int Date::osDate()
{
	return _date_to_osdate(LDATE(idate));
}


Date osDate(Int s)
{
	Date t;
	t.idate = _osdate_to_date(s);
	return t;
}

Date &Date::operator=(const Date &d)
{
	if (this != &d)
		idate = d.idate;
	return *this;
}

// end of date
