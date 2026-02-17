/* LINTLIBRARY */
/********************************************************************
* Copyright (c) 1992 InterSoft Co.  All Rights Reserved
* THIS IS UNPUBLISHED PROPRIETARY SOURCE CODE OF InterSoft Co.
* The copyright notice above does not evidence any
* actual or intended publication of such source code.
*
*
* $Id: tsconn.cc,v 1.1.1.1 1996/04/16 18:52:00 gustavof Exp $
*
*
* DESCRIPTION:
*
* ROUTINE       |  PURPOSE
*---------------+---------------------------------------------------
*
*********************************************************************/

#include <ifound/tstamp.h>
#include <essentia/connect.h>

Connection &operator<<(Connection &c, const TimeStamp &ts)
{
	Int tmp = (Int(ts.nsec) << 16) | ts.uid;
	return c << toInt(ts.time) << tmp;
}

Connection &operator>>(Connection &c, TimeStamp &ts)
{
	Int tmp, t;
	c >> t >> tmp;
	ts.time = toTime(t);
	ts.nsec = UnShort(tmp >> 16);
	ts.uid = UnShort(tmp & 0xffff);
	return c;
}
