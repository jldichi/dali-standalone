/********************************************************************
* Copyright (c) 1992 InterSoft Co.  All Rights Reserved
* THIS IS UNPUBLISHED PROPRIETARY SOURCE CODE OF InterSoft Co.
* The copyright notice above does not evidence any
* actual or intended publication of such source code.
*
* $Id: anyvalue.h,v 1.7 2004/09/30 19:47:30 damianm Exp $
*
* DESCRIPTION
* Class definitions for AnyValue Type.
*********************************************************************/

#ifndef ANYVALUE_H
#define ANYVALUE_H

#include <ifound.h>

#ifdef __GNUC__
#pragma interface
#endif

#include <ifound/itypes.h>
#include <essentia/objectid.h>

class Connection;

class AnyValue {

	friend String 	&string(const AnyValue &av);
	friend Num 		&num(const AnyValue &av);
	friend String 	toString(const AnyValue &av);
	friend Num		toNum(const AnyValue &av);
	friend Date		toDate(const AnyValue &av);
	friend Time		toTime(const AnyValue &av);
	friend bool 	toBool(const AnyValue &av);
	friend Boolean 	toBoolean(const AnyValue &av);
	friend Int 		toInt(const AnyValue &av);
	friend ObjectId	toObjectId(const AnyValue &av);

// private:

	short ty;
	union {
		Int i;
		char s[sizeof(String)];
		char n[sizeof(Num)];
		struct {
			Int x;
			Int y;
		} pair;
		struct {
			UnInt cid;
			UnInt iid;
		} oid;
		AnyValue *alias;
		void *dummy;			// to avoid alignment problems
	} datum;

public:

	AnyValue();
	AnyValue(const AnyValue &v);
	AnyValue &operator=(const AnyValue &v);
	AnyValue(const char *s);
	AnyValue(Int i);
	AnyValue(UnInt i);
	AnyValue(double f);
	AnyValue(const String &s);
	AnyValue(bool i);
#ifdef HAVE_BOOL
	AnyValue(int i);
	AnyValue(unsigned int i);
#endif
	AnyValue(const Boolean &b);
	AnyValue(const Num &n);
	AnyValue(const Date &d);
	AnyValue(const Time &t);
	AnyValue(const Int x, const Int y);
	AnyValue(const ObjectId &obj);
	AnyValue(AnyValue *a);
	AnyValue(short s);
	AnyValue(unsigned short s);

	~AnyValue();

	bool isNull() const;
	bool isVoid() const;

	Int x() const;
	Int y() const;
	inline short getType() const;
	bool isString() const;
	void clear();
	void set(const AnyValue &v);

	friend Int cmp(const AnyValue &av1, const AnyValue &av2);

	friend Connection &operator<<(Connection &c, const AnyValue &av);
	friend Connection &operator>>(Connection &c, AnyValue &av);

	AnyValue &operator+=(const AnyValue &av);
	AnyValue &operator-=(const AnyValue &av);
	AnyValue &operator/=(const AnyValue &av);
	AnyValue &operator*=(const AnyValue &av);
	AnyValue operator+(const AnyValue &av) const;
	AnyValue operator-(const AnyValue &av) const;
	AnyValue operator/(const AnyValue &av) const;
	AnyValue operator*(const AnyValue &av) const;

	void store(FILE *fp) const;
	void store(char *&buff) const;
	void load(FILE *fp);
	void load(const char *&buff);
	friend const char *toCharPtr(const AnyValue &av);

	void print();
};

#include <ifound/anyvalue.icc>

// Needed for Symantec
#ifdef __cplusplus
extern "C" {
#endif

#ifndef NULL_ANYVALUE_DEF
#	ifdef NO_EXTERN_CONST
		extern DLLDEPEND AnyValue NULL_ANYVALUE;
#	else
		extern DLLDEPEND const AnyValue NULL_ANYVALUE;
#	endif
#endif

#ifdef __cplusplus
}
#endif


#endif // ANYVALUE_H
