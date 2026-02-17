/********************************************************************
* Copyright (c) 1992 InterSoft Co.  All Rights Reserved
* THIS IS UNPUBLISHED PROPRIETARY SOURCE CODE OF InterSoft Co.
* The copyright notice above does not evidence any
* actual or intended publication of such source code.
*
* $Id: anystbuf.cc,v 1.2 1998/04/24 16:25:25 mauro Exp $
*
* DESCRIPTION
*********************************************************************/

#include <ifound/anyvalue.h>
#include <ifound/priv/gfuncs.h>

void AnyValue::store(char *&buff) const
{
	assert(ty != TY_REFERENCE);
	
	stint((short)ty, buff);

	buff += sizeof(short);

	switch (ty) {
	case TY_STRING:
		((String*)datum.s)->store(buff);
		break;
	case TY_NUMERIC:
		((Num*)datum.n)->store(buff);
		break;
	case TY_DATE:
		{
			Date d = toDate(datum.i);
			d.store(buff);
		}
		break;
	case TY_TIME:
		{
			Time t = toTime(datum.i);
			t.store(buff);
		}
		break;
	case TY_BOOL:
		stlong(datum.i, buff);
		buff += sizeof(datum.i);
		break;
	case TY_ORDPAIR:
		stlong(datum.pair.x, buff);
		buff += sizeof(datum.pair.x);
		stlong(datum.pair.y, buff);
		buff += sizeof(datum.pair.y);
		break;
	case TY_PTR:
		stlong(long(datum.oid.cid), buff);
		buff += sizeof(datum.oid.cid);
		stlong(long(datum.oid.iid), buff);
		buff += sizeof(datum.oid.iid);
		break;
	}
}

void AnyValue::store(FILE *fp) const
{
	assert(ty != TY_REFERENCE);
	
	stShort(fp, short(ty));

	switch (ty) {
	case TY_STRING:
		stString(fp, *((String *) datum.s));
		break;
	case TY_NUMERIC:
		stNum(fp, *((Num *) datum.n));
		break;
	case TY_DATE:
		{
			Date d = toDate(datum.i);
			d.store(fp);
		}
		break;
	case TY_TIME:
		{
			Time t = toTime(datum.i);
			t.store(fp);
		}
		break;
	case TY_BOOL:
		stBool(fp, datum.i);
		break;
	case TY_ORDPAIR:
		stInt(fp, datum.pair.x);
		stInt(fp, datum.pair.y);
		break;
	case TY_PTR:
		stInt(fp, Int(datum.oid.cid));
		stInt(fp, Int(datum.oid.iid));
		break;
	}
}
