/********************************************************************
* Copyright (c) 1992 InterSoft Co.  All Rights Reserved
* THIS IS UNPUBLISHED PROPRIETARY SOURCE CODE OF InterSoft Co.
* The copyright notice above does not evidence any
* actual or intended publication of such source code.
*
* $Id: anyoper.cc,v 1.1.1.1 1996/04/16 18:51:56 gustavof Exp $
*
* DESCRIPTION
*********************************************************************/

#include <ifound/anyvalue.h>
#include <ifound/stdc.h>

AnyValue AnyValue::operator+(const AnyValue &av) const
{
	if (ty == TY_REFERENCE)
		return *datum.alias + av;
	else if (av.ty == TY_REFERENCE)
		return *this + *av.datum.alias;
	else {
		switch (ty) {
		case TY_STRING:
			return toString(*this) + toString(av);
		case TY_NUMERIC:
			return toNum(*this) + toNum(av);
		case TY_DATE:
			return toDate(*this) + toInt(toNum(av));
		case TY_TIME:
			return toTime(*this) + toTime(av);
		default: 
			return NULL_STRING;
		}
	}
}

AnyValue AnyValue::operator-(const AnyValue &av) const
{
	if (ty == TY_REFERENCE)
		return *datum.alias - av;
	else if (av.ty == TY_REFERENCE)
		return *this - *av.datum.alias;
	else {
		switch (ty) {
		case TY_NUMERIC:
			return toNum(*this) - toNum(av);
		case TY_DATE:
			return toDate(*this) - toInt(toNum(av));
		case TY_TIME:
			return toTime(*this) - toTime(av);
		default: 
			return NULL_STRING;
		}
	}
}

AnyValue AnyValue::operator/(const AnyValue &av) const
{
	if (ty == TY_REFERENCE)
		return *datum.alias / av;
	else if (av.ty == TY_REFERENCE)
		return *this / *av.datum.alias;
	else {
		switch (ty) {
		case TY_NUMERIC:
			return toNum(*this) / toNum(av);
		default: 
			return NULL_STRING;
		}
	}
}

AnyValue AnyValue::operator*(const AnyValue &av) const
{
	if (ty == TY_REFERENCE)
		return *datum.alias * av;
	else if (av.ty == TY_REFERENCE)
		return *this * *av.datum.alias;
	else {
		switch (ty) {
		case TY_NUMERIC:
			return toNum(*this) * toNum(av);
		default: 
			return NULL_STRING;
		}
	}
}

