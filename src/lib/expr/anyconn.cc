/********************************************************************
* Copyright (c) 1992 InterSoft Co.  All Rights Reserved
* THIS IS UNPUBLISHED PROPRIETARY SOURCE CODE OF InterSoft Co.
* The copyright notice above does not evidence any
* actual or intended publication of such source code.
*
* $Id: anyconn.cc,v 1.2 1998/04/24 16:25:22 mauro Exp $
*
* DESCRIPTION
*********************************************************************/

#include <ifound/anyvalue.h>
#include <essentia/connect.h>
#include <ifound/stdc.h>

Connection &operator<<(Connection &c, const AnyValue &av)
{
 	c << (Int)av.ty;
	switch (av.ty) {
	case TY_STRING:
		c << string(av);
		break;
	case TY_NUMERIC:
		c << num(av);
		break;
	case TY_BOOL:
	case TY_DATE:
	case TY_TIME:
		c << toInt(av);
		break;
	case TY_ORDPAIR:
		c << av.x() << av.y();
		break;
	case TY_PTR:
		c << Int(toObjectId(av).classId()) << Int(toObjectId(av).instId());
		break;
	case TY_REFERENCE:
		assert(0);
		break;
	default:
		break;
	}
	return c;
}

Connection &operator>>(Connection &c, AnyValue &av)
{
	String s;
	Num n;
	Int i, j;

	Int typ;
	c >> typ;
	switch (typ) {
	case TY_STRING:
		c >> s;
		av = AnyValue(s);
		break;
	case TY_NUMERIC:
		c >> n;
		av = AnyValue(n);
		break;
	case TY_BOOL:
		c >> i;
		av = AnyValue(bool(i));
		break;
	case TY_DATE:
		c >> i;
		av = AnyValue(toDate(i));
		break;
	case TY_TIME:
		c >> i;
		av = AnyValue(toTime(i));
		break;
	case TY_ORDPAIR:
		c >> i >> j;
		av = AnyValue(i, j);
		break;
	case TY_PTR:
		c >> i >> j;
		av = AnyValue(ObjectId(i, j));
		break;
	case TY_REFERENCE:
		assert(0);
		break;
	default:
		av = AnyValue();
		break;
	}
	return c;
}
