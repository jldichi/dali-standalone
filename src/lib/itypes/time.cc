/********************************************************************
* Copyright (c) 1991 InterSoft Co.  All Rights Reserved
* THIS IS UNPUBLISHED PROPRIETARY SOURCE CODE OF InterSoft Co.
* The copyright notice above does not evidence any
* actual or intended publication of such source code.
*
* $Id: time.cc,v 1.2 1999/08/02 17:01:24 eduardoc Exp $
*
* DESCRIPTION
* Implementation of time class
*********************************************************************/

#include <ifound/itypes.h>
#include <ifound/stdc.h>
#include <ifound/priv/gfuncs.h>
#include <ifound/priv/itypesp.h>
#include <ifound/priv/timedefs.h>

Time toTime(Int secs)
{
	Time t;
	t.itime = secs;
	return t;
}

Time toTime(const String &s)
{
	Time t;
	t.itime = _str_to_time(toCharPtr(s));
	return t;
}

Time toTime(Date d)
{
	Time t;
	if (d.idate == LONG_MAX) t.itime = LONG_MAX;
	else if (d.idate == LONG_MIN) t.itime = LONG_MIN;
	else t.itime = d.idate * 60L * 60L * 24L; 
	return t;
}

static Int last_day = 0, last_hour = 0, last_minute = 0, last_second = 0;
static long last_itime = 0;

Time::Time(const char *s)
{
	itime = _str_to_time(s);
}

Time::Time(Int h, Int m, Int s)
{
	itime = _dhms_to_time(0, h, m, s);
}

Time::Time(Int d, Int h, Int m, Int s)
{
	itime = _dhms_to_time(d, h, m, s);
}

Int Time::osTime()
{
	return _time_to_ostime(LTIME(itime));
}

void Time::recalc_values() const
{
	if (last_itime != itime) {
		last_itime = itime;
		_time_to_dhms(itime, &last_day, &last_hour, &last_minute,
				&last_second);
	}
}

Int Time::second() const
{
	recalc_values();
	return last_second;
}

Int Time::minute() const
{
	recalc_values();
	return last_minute;
}

Int Time::hour() const
{
	recalc_values();
	return last_hour;
}

Int Time::seconds() const
{
	return itime;
}

Int Time::minutes() const
{
	return itime / SECONDS_MINUTE;
}

Int Time::hours() const
{
	return itime / SECONDS_HOUR;
}

Int Time::days() const
{
	return Int(itime / SECONDS_DAY);
}

const char *toCharPtr(Time t, TimeFmt tfmt)
{
	static FmtBuff fBuff = { '\0' };
	return _time_to_str(t.itime, fBuff, tfmt);
}

Time osTime(Int s)
{
	Time t;
	t.itime = _osTimeZoneAdj(_ostime_to_time(s));
	return t;
}

Time &Time::operator=(const Time &b)
{
	if (this != &b)
		itime = b.itime;
	return *this;
}

// end of time
