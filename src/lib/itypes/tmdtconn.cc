/********************************************************************
* Copyright (c) 1991 InterSoft Co.  All Rights Reserved
* THIS IS UNPUBLISHED PROPRIETARY SOURCE CODE OF InterSoft Co.
* The copyright notice above does not evidence any
* actual or intended publication of such source code.
*
* $Id: tmdtconn.cc,v 1.1.1.1 1996/04/16 18:51:55 gustavof Exp $
*
* DESCRIPTION
* save and load date & time to connection
*********************************************************************/


#include <ifound/itypes.h>
#include <essentia/connect.h>

Connection &operator<<(Connection &c, const Date &d)
{
	c << d.idate;
	return c;
}

Connection &operator>>(Connection &c, Date &d)
{
	c.recFirstInt(d.idate);
	return c;
}

Connection &operator<<(Connection &c, const Time &t)
{
	c << t.itime;
	return c;
}

Connection &operator>>(Connection &c, Time &t)
{
	c.recFirstInt(t.itime);
	return c;
}
