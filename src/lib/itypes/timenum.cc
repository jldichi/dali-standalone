/********************************************************************
* Copyright (c) 1991 InterSoft Co.  All Rights Reserved
* THIS IS UNPUBLISHED PROPRIETARY SOURCE CODE OF InterSoft Co.
* The copyright notice above does not evidence any
* actual or intended publication of such source code.
*
*
* $Id: timenum.cc,v 1.1.1.1 1996/04/16 18:51:55 gustavof Exp $
*
*
* DESCRIPTION:
*				Implementation of time class
*
*********************************************************************/

#include <ifound/itypes.h>
#include <ifound/stdc.h>
#include <ifound/priv/itypesp.h>
#include <ifound/priv/timedefs.h>

static long make_time(const Num &d,const Num&h,const Num&m,const Num&s)
{
	Num itime;

	itime = d * SECONDS_DAY + h * SECONDS_HOUR + m * SECONDS_MINUTE + s;

	if (itime <= Int(LONG_MIN)) return LONG_MIN;

	if (itime >= Int(LONG_MAX)) return LONG_MAX;

	return toInt(itime);
}

Time::Time(const Num &h, const Num &m, const Num &s)
{
	itime = make_time(0L, h, m, s);
}

Time::Time(const Num &d, const Num &h, const Num &m, const Num &s)
{
	itime = make_time(d, h, m, s);
}

Time toTime(const Num &secs)
{
	Time t;
	t.itime = toInt(secs);
	return t;
}

// end of time
