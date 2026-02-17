/********************************************************************
* Copyright (c) 1992 InterSoft Co.  All Rights Reserved
* THIS IS UNPUBLISHED PROPRIETARY SOURCE CODE OF InterSoft Co.
* The copyright notice above does not evidence any
* actual or intended publication of such source code.
*
* $Id: anyvalue.cc,v 1.6 2007/03/07 15:43:14 hardaiz Exp $
*
* DESCRIPTION
*********************************************************************/

#ifdef __GNUC__
#pragma implementation
#endif

#include <string.h>
#ifdef _AIX
#include <strings.h>
#endif

#include <ifound/anyvalue.h>
#include <ifound/stdc.h>

#if HAVE_SETRLIMIT
#include <sys/time.h>
#include <sys/resource.h>
#endif

// toDo: considerar NULL, MIN, MAX en todos los casos!

void AnyValue::set(const AnyValue &v)
{
	if (this == &v)
		return;

	if (ty == TY_STRING)
		((String *) datum.s)->disconnect();

	mCopy(&datum, &v.datum, sizeof(datum));
	ty = v.ty;

	if (ty == TY_STRING)
		((String *) datum.s)->connect();

}

AnyValue &AnyValue::operator=(const AnyValue &v)
{
	if (this == &v)
		return *this;
		
	if (ty == TY_REFERENCE)
		*datum.alias = v;
	else if (v.ty == TY_REFERENCE)
		*this = *v.datum.alias;
	else set(v);

	return *this;
}

AnyValue::AnyValue()
{
	ty = ERR;
}

AnyValue::AnyValue(const AnyValue &v)
{
	ty = ERR;
	set(v);
}

AnyValue::AnyValue(const char *s)
{
	ty = TY_STRING;
	String tmp(s);
	mCopy(datum.s, &tmp, sizeof(tmp));
	((String *) datum.s)->connect();
}

AnyValue::AnyValue(const String &s)
{
	ty = TY_STRING;
	mCopy(datum.s, &s, sizeof(s));
	((String *) datum.s)->connect();
}

AnyValue::AnyValue(bool i)
{
	ty = TY_BOOL;
	datum.i = Int(i);
}

AnyValue::AnyValue(const Boolean &b)
{
	ty = TY_BOOL;
	datum.i = toInt(b);
}

AnyValue::AnyValue(Int i)
{
	ty = TY_NUMERIC;
	*(Num *) datum.n = toNum(i);
}

AnyValue::AnyValue(UnInt i)
{
        ty = TY_NUMERIC;
        *(Num *) datum.n = toNum(i);
}

#ifdef HAVE_BOOL
AnyValue::AnyValue(int i)
{
	ty = TY_NUMERIC;
	*(Num *) datum.n = toNum(short(i));
}

AnyValue::AnyValue(unsigned int i)
{
        ty = TY_NUMERIC;
        *(Num *) datum.n = toNum(short(i));
}
#endif 

AnyValue::AnyValue(short s)
{
	ty = TY_NUMERIC;
	*(Num *) datum.n = toNum(s);
}

AnyValue::AnyValue(unsigned short s)
{
        ty = TY_NUMERIC;
        *(Num *) datum.n = toNum(s);
}

AnyValue::AnyValue(double f)
{
	ty = TY_NUMERIC;
	*(Num *) datum.n = toNum(f);
}

AnyValue::AnyValue(const Num &n)
{
	ty = TY_NUMERIC;
	*(Num *) datum.n = n;
}

AnyValue::AnyValue(const Date &d)
{
	ty = TY_DATE;
	datum.i = toInt(d);
}

AnyValue::AnyValue(const Time &t)
{
	ty = TY_TIME;
	datum.i = toInt(t);
}

AnyValue::AnyValue(const Int xp, const Int yp)
{
	ty = TY_ORDPAIR;
	datum.pair.x = xp;
	datum.pair.y = yp;
}

AnyValue::AnyValue(const ObjectId &obj)
{
	ty = TY_PTR;
	datum.oid.cid = obj.classId();
	datum.oid.iid = obj.instId();
}

AnyValue::AnyValue(AnyValue *a)
{
	assert(a != NULL);
	ty = TY_REFERENCE;
	datum.alias = a;
}

AnyValue::~AnyValue()
{
	if (ty == TY_STRING)
		((String *) datum.s)->disconnect();
}

bool AnyValue::isNull() const 
{
	switch (ty) {
	case TY_STRING:
		return ((String *) datum.s)->isNull();
	case TY_NUMERIC:
		return ((Num *) datum.n)->isNull();
	case TY_DATE:
		return toDate(datum.i).isNull();
	case TY_TIME:
		return toTime(datum.i).isNull();
	case TY_BOOL:
		return toBoolean(datum.i).isNull();
	case TY_ORDPAIR:
		return (datum.pair.x == ERR) || (datum.pair.y == ERR);
	case TY_PTR:
		return (datum.oid.iid == 0) || (datum.oid.cid == 0);
	case TY_REFERENCE:
		return datum.alias->isNull();
	case ERR:
		return true;
	}
	assert(0);
	return BOOL_NULL;
}

bool AnyValue::isString() const
{
	if (ty == TY_REFERENCE)
		return datum.alias->isString();
	return ty == TY_STRING;
}

bool AnyValue::isVoid() const
{
	return ty == ERR;
}

String &string(const AnyValue &av)
{
	if (av.ty == TY_REFERENCE)
		return string(*av.datum.alias);
	assert(av.ty == TY_STRING);
	return *(String *) av.datum.s;
}

Num &num(const AnyValue &av)
{
	if (av.ty == TY_REFERENCE)
		return num(*av.datum.alias);
	assert(av.ty == TY_NUMERIC);
	return *(Num *) av.datum.n;
}

String toString(const AnyValue &av)
{
	switch (av.ty) {
	case TY_STRING:
		return string(av);
	case TY_NUMERIC:
		return toString(num(av));
	case TY_DATE:
		return toString(toDate(av.datum.i));
	case TY_TIME:
		return toString(toTime(av.datum.i));
	case TY_BOOL:
		return toString(toBoolean(av.datum.i));
	case TY_ORDPAIR:
		return String("(") + toString(av.datum.pair.x) + "," +
				toString(av.datum.pair.y) + String(")");
	case TY_PTR:
		return String("*(") + toString(av.datum.oid.cid) + "," +
				toString(av.datum.oid.iid) + String(")");
	case TY_REFERENCE:
		return toString(*av.datum.alias);
	default:
		return NULL_STRING;
	}
}

Num toNum(const AnyValue &av)
{
	switch (av.ty) {
	case TY_STRING:
		return toNum(string(av));
	case TY_NUMERIC:
		return num(av);
	case TY_DATE:
	case TY_TIME:
	case TY_BOOL:
		return Num(av.datum.i);
	case TY_REFERENCE:
		return toNum(*av.datum.alias);
	default:
		return toNum(NULL_STRING);
	}
}

Date toDate(const AnyValue &av)
{
	switch (av.ty) {
	case TY_STRING:
		return toDate(string(av));
	case TY_NUMERIC:
		return toDate(toInt(num(av)));
	case TY_DATE:
		return toDate(av.datum.i);
	case TY_TIME:
		return toDate(toTime(av.datum.i));
	case TY_REFERENCE:
		return toDate(*av.datum.alias);
	default:
		return toDate(NULL_STRING);
	}
}

Int toInt(const AnyValue &av)
{
	switch (av.ty) {
	case TY_STRING:
		return toInt(string(av));
	case TY_NUMERIC:
		return toInt(num(av));
	case TY_DATE:
	case TY_TIME:
	case TY_BOOL:
		return av.datum.i;
	case TY_REFERENCE:
		return toInt(*av.datum.alias);
	default:
		return INT_NULL;
	}
}

Time toTime(const AnyValue &av)
{
	switch (av.ty) {
	case TY_STRING:
		return toTime(string(av));
	case TY_NUMERIC:
		return toTime(toInt(num(av)));
	case TY_DATE:
		return toTime(toDate(av.datum.i));
	case TY_TIME:
		return toTime(av.datum.i);
	case TY_REFERENCE:
		return toTime(*av.datum.alias);
	default:
		return toTime(NULL_STRING);
	}
}

bool toBool(const AnyValue &av)
{
	switch (av.ty) {
	case TY_STRING:
		return bool(toInt(string(av)));
	case TY_NUMERIC:
		return bool(toInt(num(av)));
	case TY_BOOL:
		return bool(av.datum.i);
	case TY_REFERENCE:
		return toBool(*av.datum.alias);
	default:
		return BOOL_NULL;
	}
}

Boolean toBoolean(const AnyValue &av)
{
	switch (av.ty) {
	case TY_STRING:
		return toBoolean(toInt(string(av)));
	case TY_NUMERIC:
		return toBoolean(toInt(num(av)));
	case TY_BOOL:
		return toBoolean(av.datum.i);
	case TY_REFERENCE:
		return toBoolean(*av.datum.alias);
	default:
		return BOOL_NULL;
	}
}

ObjectId toObjectId(const AnyValue &av)
{
	switch (av.ty) {
	case TY_PTR:
		return ObjectId(av.datum.oid.cid, av.datum.oid.iid);
	case TY_REFERENCE:
		return toObjectId(*av.datum.alias);
	default:
		return ObjectId();
	}
}

Int AnyValue::x() const
{
	// toDo: convertir (por lo menos del string "(x,y)")
	switch (ty) {
	case TY_ORDPAIR:
		return datum.pair.x;
	case TY_PTR:
		return datum.oid.cid;
	case TY_REFERENCE:
		return datum.alias->x();
	default:
		return ERR;
	}
}

Int AnyValue::y() const
{
	// toDo: convertir (por lo menos del string "(x,y)")
	switch (ty) {
	case TY_ORDPAIR:
		return datum.pair.y;
	case TY_PTR:
		return datum.oid.iid;
	case TY_REFERENCE:
		return datum.alias->y();
	default:
		return ERR;
	}
}

Int cmp(const AnyValue &av1, const AnyValue &av2)
{
	if (av1.ty == TY_REFERENCE)
		return cmp(*av1.datum.alias, av2);
	else if (av2.ty == TY_REFERENCE)
		return cmp(av1, *av2.datum.alias);
	else {
		switch (av1.ty) {
		case TY_STRING:
			return ::cmp(string(av1), toString(av2));
		case TY_NUMERIC:
			return ::cmp(num(av1), toNum(av2));
		case TY_DATE:
			return ::cmp(toDate(av1), toDate(av2));
		case TY_TIME:
			return ::cmp(toTime(av1), toTime(av2));
		case TY_BOOL:
			return toBool(av1) - toBool(av2);
		case TY_PTR:
			return ::cmp(toObjectId(av1), toObjectId(av2));
		case TY_ORDPAIR:
			return (av1.x() - av2.x()) + (av1.y() - av2.y());
		case ERR:
			return av2.ty == ERR;
		default:
			assert(0);
		}
		return 0;
	}
}

void AnyValue::clear()
{
	switch (ty) {
	case TY_STRING:
		string(*this) = NULL_STRING;
		break;
	case TY_NUMERIC:
		num(*this) = toNum(NULL_STRING);
		break;
	case TY_TIME:
	case TY_DATE:
		datum.i = INT_NULL;
		break;
	case TY_BOOL:
		datum.i = BOOL_NULL;
		break;
	case TY_PTR:
		datum.oid.cid = datum.oid.iid = 0;
		break;
	case TY_ORDPAIR:
		datum.pair.x = datum.pair.y = ERR;
		break;
	case TY_REFERENCE:
		datum.alias->clear();
	case ERR:
		break;
	default: 
		assert(0);
	}
}

AnyValue &AnyValue::operator+=(const AnyValue &av)
{
	if (ty == TY_REFERENCE)
		*datum.alias += av;
	else if (av.ty == TY_REFERENCE)
		*this += *av.datum.alias;
	else {
		switch (ty) {
		case TY_STRING:
			string(*this) += toString(av);
			break;
		case TY_NUMERIC:
			switch (av.ty) {
			case TY_NUMERIC:
				num(*this) += toNum(av);
				break;
			case TY_DATE:
				{
					Date a = toDate(*this);
					Num b = toNum(av);
					Date c = a + toInt(b);
					datum.i = toInt(c);
					ty = TY_DATE;
				}
				break;
			case TY_TIME:
				{
					Time a = toTime(*this);
					Time b = toTime(av);
					datum.i = toInt(a + b);
					ty = TY_TIME;
				}
				break;
			default:
				break;
			}
			break;
		case TY_DATE:
			{
				// Re-written because of a bug in gcc for HP-UX.
				// datum.i = toInt(Date(*this)+toInt(Num(av)));
				Date a = toDate(*this);
				Num b = toNum(av);
				Date c = a + toInt(b);
				datum.i = toInt(c);
			}
			break;
		case TY_TIME:
			{
				// Re-written because of a bug in gcc for HP-UX.
				// datum.i = toInt(Time(*this)+Time(av));
				Time a = toTime(*this);
				Time b = toTime(av);
				datum.i = toInt(a + b);
			}
			break;
		default: 
			clear();
		}
	}
	return *this;
}

AnyValue &AnyValue::operator-=(const AnyValue &av)
{
	if (ty == TY_REFERENCE)
		*datum.alias -= av;
	else if (av.ty == TY_REFERENCE)
		*this -= *av.datum.alias;
	else {
		switch (ty) {
		case TY_STRING:
			string(*this) = NULL_STRING;
			break;
		case TY_NUMERIC:
			num(*this) -= toNum(av);
			break;
		case TY_DATE:
			{
				Date a = toDate(*this);
				Num b = toNum(av);
				Date c = a - toInt(b);
				switch (av.ty) {    
				case TY_DATE:
					*(Num*)datum.n = toNum(c);
					ty = TY_NUMERIC;
					break;
				case TY_NUMERIC:
					datum.i = toInt(c);
					break;
				default:
					clear();
				}
			}
			break;         
		case TY_TIME:
			{
				Time a = toTime(*this);
				Time b = toTime(av);
				switch (av.ty) {
				case TY_TIME:
					*(Num*)datum.n = toNum(a - b);
					ty = TY_NUMERIC;
					break;
				case TY_NUMERIC:
					datum.i = toInt(a - b);
					break;
				default:
					clear();
				}
			}
			break;
		default: 
			clear();
		}
	}
	return *this;
}

AnyValue &AnyValue::operator/=(const AnyValue &av)
{
	if (ty == TY_REFERENCE)
		*datum.alias /= av;
	else if (av.ty == TY_REFERENCE)
		*this /= *av.datum.alias;
	else {
		switch (ty) {
		case TY_NUMERIC:
			num(*this) /= toNum(av);
			break;
		case TY_DATE:
			datum.i = toInt(toDate(datum.i) /= toInt(toNum(av)));
			break;
		case TY_TIME:
			datum.i = toInt(toTime(datum.i) /= toInt(toTime(av)));
			break;
		default:
			clear();
			break;
		}
	}
	return *this;
}

AnyValue &AnyValue::operator*=(const AnyValue &av)
{
	if (ty == TY_REFERENCE)
		*datum.alias *= av;
	else if (av.ty == TY_REFERENCE)
		*this *= *av.datum.alias;
	else {
		switch (ty) {
		case TY_NUMERIC:
			num(*this) *= toNum(av);
			break;
		case TY_DATE:
			datum.i = toInt(toDate(datum.i) *= toInt(toNum(av)));
			break;
		case TY_TIME:
			datum.i = toInt(toTime(datum.i) *= toInt(toTime(av)));
			break;
		default: 
			clear();
			break;
		}
	}
	return *this;
}

const char *toCharPtr(const AnyValue &av)
{
	switch (av.ty) {
	case TY_STRING:
		return toCharPtr(string(av));
	case TY_NUMERIC:
		return toCharPtr(num(av));
	case TY_DATE:
		return toCharPtr(toDate(av.datum.i));
	case TY_TIME:
		return toCharPtr(toTime(av.datum.i));
	case TY_BOOL:
		return toCharPtr(toBoolean(av.datum.i));
	case TY_REFERENCE:
		return toCharPtr(*av.datum.alias);
	default:
		return NULL;
	}
}

void AnyValue::print()
{
	switch (ty) {
	case TY_STRING:
		printf("TY_STRING '%s'\n", toCharPtr(*(String *) datum.s));
		break;
	case TY_NUMERIC:
		printf("TY_NUMERIC '%ld'\n", toInt(*(Num *) datum.n));
		break;
	case TY_BOOL:
		printf("TY_BOOL '%ld'\n", !!datum.i);
		break;
	case TY_DATE:
		printf("TY_DATE\n");
		break;
	case TY_TIME:
		printf("TY_TIME\n");
		break;
	case TY_ORDPAIR:
		printf("TY_ORDPAIR '%ld, %ld'\n", datum.pair.x, datum.pair.y);
		break;
	case TY_PTR:
		printf("TY_PTR '%lu, %lu'\n", datum.oid.cid, datum.oid.iid);
		break;
	case TY_REFERENCE:
		printf("TY_REFERENCE TO : ");
		datum.alias->print();
		break;
	case ERR:
		printf("NULL_ANYVALUE\n");
		break;
	default:
		assert(0);
	}
}

#ifdef __VC__
short AnyValue::getType() const
{
	return ty;
}
#endif

