/********************************************************************
* Copyright (c) 1991 InterSoft Co.  All Rights Reserved
* THIS IS UNPUBLISHED PROPRIETARY SOURCE CODE OF InterSoft Co.
* The copyright notice above does not evidence any
* actual or intended publication of such source code.
*
* $Id: tmdtstld.cc,v 1.3 1998/05/19 21:17:32 eduardoc Exp $
*
* DESCRIPTION
* store/load date & time to/from buffer
*********************************************************************/

#include <ifound.h>
#include <ifound/itypes.h>
#include <ifound/priv/gfuncs.h>
#include <cfix/priv/macros.h>

// This include is in order to be able to use SDATETOL, etc.
#include <cfix/types.h>

#define LONG_OFFSET UnInt(-LONG_MIN)

Date Date::load(FILE *fp)
{
	Date d;
	d.idate = ldInt(fp);
	return d;
}

Date Date::load(const char *&p)
{
	Date d;
	UnInt u = ldlong(p);
	d.idate = u > LONG_OFFSET ? Int(u - LONG_OFFSET)
			: (Int)(u) + (Int)(-LONG_OFFSET);
	p += sizeof(Int);
	return d;
}

void Date::store(FILE *fp) const
{
	stInt(fp, idate);
}

void Date::store(char *&p) const
{
	UnInt u;
	u = idate >= 0 ? idate + LONG_OFFSET 
	  : LONG_OFFSET - UnInt(-idate);
	stlong(u, p);
	p += sizeof(UnInt);
}

Time Time::load(FILE *fp)
{
	Time t;
	t.itime = ldInt(fp);
	return t;
}

Time Time::load(const char *&p)
{
	Time t;
	UnInt u = ldlong(p);
	t.itime = u > LONG_OFFSET ?  Int(u - LONG_OFFSET)
			: Int(u) + Int(-LONG_OFFSET);
	p += sizeof(Int);
	return t;
}

void Time::store(FILE *fp) const
{
	stInt(fp, itime);
}

void Time::store(char *&p) const
{
	UnInt u;
	u = itime >= 0 ? itime + LONG_OFFSET 
	  : LONG_OFFSET - UnInt(-itime);
	stlong(u, p);
	p += sizeof(UnInt);
}

Date Date::loadShort(const char *&p)
{
	Date d;
	short tmp = short(lduint(p) + Int(SHORT_MIN));
	d.idate = SDATETOL(tmp);
	p += sizeof(short);
	return d;
}

void Date::storeShort(char *&p) const
{
	UnShort tmp = UnShort(LDATETOS(idate) - Int(SHORT_MIN));

 	stuint((UnShort)tmp, p);

	p += sizeof(short);
}

Time Time::loadShort(const char *&p)
{
	Time t;
	t.itime = STIMETOL(lduint(p));
	p += sizeof(short);
	return t;
}

void Time::storeShort(char *&p) const
{
	stuint(LTIMETOS(itime), p);
	p += sizeof(short);
}
