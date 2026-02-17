/* LINTLIBRARY */
/********************************************************************
* Copyright (c) 1992 InterSoft Co.  All Rights Reserved
* THIS IS UNPUBLISHED PROPRIETARY SOURCE CODE OF InterSoft Co.
* The copyright notice above does not evidence any
* actual or intended publication of such source code.
*
*
* $Id: anyldbuf.cc,v 1.3 2007/03/07 15:44:08 hardaiz Exp $
*
*
* DESCRIPTION:
*
* ROUTINE       |  PURPOSE
*---------------+---------------------------------------------------
*
*********************************************************************/

#include <string.h>
#ifdef _AIX
#include <strings.h>
#endif

#include <ifound/anyvalue.h>
#include <ifound/stdc.h>
#include <ifound/priv/gfuncs.h>

void AnyValue::load(const char *&buff)
{
	if (ty == TY_STRING)
		((String *) datum.s)->disconnect();

	ty = ldint(buff);
	buff += sizeof(short);

	switch (ty) {
	case TY_STRING:
		{
			String tmp;
			mCopy(datum.s, &tmp, sizeof(tmp));
			((String *) datum.s)->connect();
			((String *) datum.s)->load(buff);
		}
		break;
	case TY_NUMERIC:
		((Num *) datum.n)->load(buff);
		break;
	case TY_DATE:
		datum.i = toInt(Date::load(buff));
		break;
	case TY_TIME:
		datum.i = toInt(Time::load(buff));
		break;
	case TY_BOOL:
		datum.i = ldlong(buff);
		buff += sizeof(datum.i);
		break;
	case TY_ORDPAIR:
		datum.pair.x = ldlong(buff);
		buff += sizeof(datum.pair.x);
		datum.pair.y = ldlong(buff);
		buff += sizeof(datum.pair.y);
		break;
	case TY_PTR:
		datum.oid.cid = ldlong(buff);
		buff += sizeof(datum.oid.cid);
		datum.oid.iid = ldlong(buff);
		buff += sizeof(datum.oid.iid);
		break;
	}
}

void AnyValue::load(FILE *fp)
{
	if (ty == TY_STRING)
		((String *) datum.s)->disconnect();

	ty = ldShort(fp);

	switch (ty) {
	case TY_STRING:
		{
			String tmp;
			mCopy(datum.s, &tmp, sizeof(tmp));
			((String *) datum.s)->connect();
			*((String *) datum.s) = ldString(fp);
		}
		break;
	case TY_NUMERIC:
		*((Num *) datum.n) = ldNum(fp);
		break;
	case TY_DATE:
		datum.i = toInt(Date::load(fp));
		break;
	case TY_TIME:
		datum.i = toInt(Time::load(fp));
		break;
	case TY_BOOL:
		datum.i = ldInt(fp);
		break;
	case TY_ORDPAIR:
		datum.pair.x = ldInt(fp);
		datum.pair.y = ldInt(fp);
		break;
	case TY_PTR:
		datum.oid.cid = ldInt(fp);
		datum.oid.iid = ldInt(fp);
		break;
	}
}
