/***********************************************************************
* Copyright (c) 1991 InterSoft Co.  All Rights Reserved
* THIS IS UNPUBLISHED PROPRIETARY SOURCE CODE OF InterSoft Co.
* The copyright notice above does not evidence any
* actual or intended publication of such source code.
*
* $Id: itypes.h,v 1.4 1999/02/11 18:00:36 diegon Exp $
*
* DESCRIPTION
* Class definitions for IdeaFix basic types.
*********************************************************************/

#ifndef	ITYPES_H
#define	ITYPES_H

#include <ifound.h>

#ifdef __GNUC__
#pragma interface
#endif

#include <ifound/str.h>
#include <ifound/numdefs.h>

#ifdef ceil
#	undef ceil
#endif
#ifdef floor
#	undef floor
#endif

// To have the store and load prototypes without including istream.h

class OutputStream;
class InputStream;

const String &typeName(Int t);

enum {
	SIGN_IGNORE,	// Show always as positive.
	SIGN_LEADING,	// Show with leading cero if necesary.
	SIGN_TRAIL,		// Show with trailing minus if necesary.
	SIGN_PAREN,		// Show number with brakets if negative.
	SIGN_CREDIT		// Credit sign format.
};

class Connection;

// Num class 

// This class was documented by Oscar (/usr2/acct/oscar/doc/itypes/itypes.doc)
class Num : public NUM
{

	friend class SvNumField;

// private:

	void trunc(Int ndec, short up = 0);
	char *_store(char *buff, Int tots, Int nsd) const;
	const char *_load(const char *buff, Int tots, Int nsd);

public:

	Num()				{ exp = -MAX_EXP; len = 0; }
    Num(const Num &n)   { *this = n; }
	Num(int j)			{ if (j) LToNum(j, this); else exp = len = 0; }
	Num(Int j)			{ if (j) LToNum(j, this); else exp = len = 0; }
	Num(double k)		{ if (k) FToNum(k, this); else exp = len = 0; }
	Num(const char *cs) { StrToNum(cs, this); }
	Num(const String &r) { StrToNum(toCharPtr(r), this); }

	Num(const char *&buff, Int l, Int nd = 0) { load(buff, l, nd); }

	friend Num toNum(char c)
	{ Num a; ByteToNum(c, (NUM *) &a); return a; }
	friend Num toNum(short i)
	{ Num a; IToNum(i, (NUM *) &a); return a; }
	friend Num toNum(short unsigned i)
	{ Num a; IToNum(i, (NUM *) &a); return a; }
	friend Num toNum(Int j)
	{ Num a; LToNum(j, (NUM *) &a); return a; }
	friend Num toNum(UnInt j)
	{ Num a; LToNum(j, (NUM *) &a); return a; }
	friend Num toNum(double k)
	{ Num a; FToNum(k, (NUM *) &a); return a; }
	friend Num toNum(const char *cs)
	{ Num a; StrToNum(cs, (NUM *) &a); return a; }
	friend Num toNum(const String &str)
	{ return Num(toCharPtr(str)); }

	friend short toShort(const Num &num);
	friend char toByte(const Num &num);
	friend Int toInt(const Num &num)
	{ return NumToL((const NUM *) &num); }
	friend double toDouble(const Num &num)
	{ return NumToF((const NUM *) &num); }

	friend const char *toCharPtr(const Num &num, Int n = 255, Int p = INT_MAX)
	{ return NumToStr((const NUM *) &num, n, p); }
	friend String toString(const Num &num, Int n = 255, Int p = INT_MAX);

	String format(Int size, Int ndec = 0, bool separ = true, 
				  Int sign = SIGN_LEADING, char filler = ' ') const;
	String format(const String &mask) const;

	Num& operator=(const Num &a);

	Num& operator+=(const Num &a)  { (void) NumAdd(this, &a); return *this; }
	Num& operator-=(const Num &a)  { (void) NumSub(this, &a); return *this; }
	Num& operator*=(const Num &a)  { (void) NumMul(this, &a); return *this; }
	Num& operator/=(const Num &a)  { (void) NumDiv(this, &a); return *this; }

	friend Num operator<<(const Num &a, int b)   { return a.shift(b); }
	friend Num operator>>(const Num &a, int b)   { return a.shift(-b); }
	Num &operator<<=(int b) { (void) NumShift(this, b); return *this;  }
	Num &operator>>=(int b) { (void) NumShift(this, -b); return *this; }

	friend Num operator++(Num &a)  { return a += 1L; }
	friend Num operator--(Num &a)  { return a -= 1L; }

	friend Num operator+ (Num a, const Num &b) { return a += b; }
	friend Num operator- (Num a, const Num &b) { return a -= b; }
	friend Num operator* (Num a, const Num &b) { return a *= b; }
	friend Num operator/ (Num a, const Num &b) { return a /= b; }

	Num  operator^(const Num &a) const;

	Num  operator-() const { Num ret = *this; ret.exp = -exp; return ret; }
	Int  operator! () const { return ! len; }

	friend Int  cmp(const Num &a, const Num &b)
	{ return NumCmp((const NUM *) &a, (const NUM *) &b); }
	friend bool operator< (const Num &a, const Num &b)
	{ return NumCmp((const NUM *) &a, (const NUM *) &b) < 0; }
	friend bool operator> (const Num &a, const Num &b)
	{ return NumCmp((const NUM *) &a, (const NUM *) &b) > 0; }
	friend bool operator<=(const Num &a, const Num &b)
	{ return NumCmp((const NUM *) &a, (const NUM *) &b) <= 0; }
	friend bool operator>=(const Num &a, const Num &b)
	{ return NumCmp((const NUM *) &a, (const NUM *) &b) >= 0; }
	friend bool operator==(const Num &a, const Num &b)
	{ return NumCmp((const NUM *) &a, (const NUM *) &b) == 0; }
	friend bool operator!=(const Num &a, const Num &b)
	{ return NumCmp((const NUM *) &a, (const NUM *) &b) != 0; }

	Num  ceil (Int ndec = 0) const;
	Num  floor(Int ndec = 0) const;
	Num  round(Int ndec = 0) const;

	Num sqrt() const;

	Int nthDig(Int n) const
	{ return NumNthDig(this, n); }

	Int totDigits(Int &nint, Int &ndec) const
	{ return NumTotDigits(this, &nint, &ndec); }

	Num shift(int n = 1) const;

	bool isNull() const { return NumIsNull(this); }
	bool isMax() const  { return NumIsMax(this) ; }
	bool isMin() const	{ return NumIsMin(this) ; }
	bool isZero() const	{ return NumIsZero(this); }

	void store(OutputStream &stream) const;
	void load(InputStream &stream);
	void store(char *&buff, Int l, Int nd = 0) const;
	void load(const char *&buff, Int l, Int nd = 0);
	void store(char *&buff) const;
	void load(const char *&buff);

	static bool sameRep(Int l1, Int d1, Int l2, Int d2);
	static Int storeLength();
	
	static void intLength(Int l, Int nd, Int &tots, Int &nsd);

	static Int conLength(Int l, Int nd);

	friend Connection &operator<<(Connection &c, const Num &n);
	friend Connection &operator>>(Connection &c, Num &n);
	friend void stNum(FILE *f, const Num &n);
	friend Num ldNum(FILE *f);

	void debug(const char *name);
};

#if (__GNUC__ >= 8)
using namespace idx;
#endif
// Date class

// This class was documented by Oscar (/usr2/acct/oscar/doc/itypes/itypes.doc)

// Forward declarations needed for friend functions used in inline methods
class Date;
class Time;
Date toDate(Int i);
Date toDate(const String &s);
Date toDate(Time t);
Int  toInt(const Date &d);

class Date {

	friend class Time;

// private:

	Int idate;

	void recalc_values() const;
	const char *form(const char *mask, long seconds = -1L) const;

public:

	Date() 							{ idate = LONG_MIN; }
	Date(const Date &d)             { *this = d; }
	Date(const Num &d, const Num &m, const Num &y);
	Date(const char *s);
	Date(Int d, Int m, Int y);

	friend const char *toCharPtr(Date d, DateFmt dfmt = DEFAULT_FMT);
	friend Date toDate(Int i);
	friend Date toDate(const String &s) { return Date(toCharPtr(s)); }
	friend Date toDate(Time t);
	friend Time toTime(Date d);
	friend String toString(Date d, DateFmt dfmt = DEFAULT_FMT)
	{ return String(toCharPtr(d, dfmt)); }

	friend Int  toInt(const Date &d) { return d.idate; }

	const char *format(const char *mask);
	String format(const String &mask) const;

	Int  day() const;
	Int  month() const;
	Int  year() const;
	Int  dayOfWeek() const;
	Int  dayOfYear() const;
	Int  weekOfYear() const;
	Int  quarter() const;
	Date lastOfMonth() const;
	Date firstOfMonth() const;
	String dayOfWeekName() const;
	String monthName() const;
	Int monthsSince(const Date b) const; 
	Date addMonths(Int) const;
	String isHoliday() const;	
	static Date easter(Int);
    
	friend Int cmp(const Date &a, const Date &b)
		{ return a.idate == b.idate ? 0 : (a.idate < b.idate ? -1 : 1); }
	bool operator< (Date b) const	{ return idate <  b.idate; }
	bool operator> (Date b) const	{ return idate >  b.idate; }
	bool operator<=(Date b) const	{ return idate <= b.idate; }
	bool operator>=(Date b) const	{ return idate >= b.idate; }
	bool operator==(Date b) const	{ return idate == b.idate; }
	bool operator!=(Date b) const	{ return idate != b.idate; }

	Date &operator=(const Date &d);
	

#if (__GNUC__ >= 8)
	Date &operator+=(Int b)  { idate = idx::add(idate,  b); 	return *this; }
	Date &operator-=(Int b)  { idate = idx::add(idate, -b); 	return *this; }
	Date &operator*=(Int b)  { idate = idx::mult(idate, b); 	return *this; }
	Date &operator/=(Int b)  { idate = idx::div(idate,  b);		return *this; }
	Date &operator++(int)	 { idate = idx::add(idate,  1); 	return *this; }
	Date &operator++()	     { idate = idx::add(idate,  1); 	return *this; }
	Date &operator--()	     { idate = idx::add(idate, -1); 	return *this; }
	Date &operator--(int)	 { idate = idx::add(idate, -1); 	return *this; }
#else
	Date &operator+=(Int b)  { idate = add(idate,  b); 	return *this; }
	Date &operator-=(Int b)  { idate = add(idate, -b); 	return *this; }
	Date &operator*=(Int b)  { idate = mult(idate, b); 	return *this; }
	Date &operator/=(Int b)  { idate = div(idate,  b);  return *this; }
	Date &operator++(int)	 { idate = add(idate,  1); 	return *this; }
	Date &operator++()	     { idate = add(idate,  1); 	return *this; }
	Date &operator--()	     { idate = add(idate, -1); 	return *this; }
	Date &operator--(int)	 { idate = add(idate, -1); 	return *this; }
#endif



	Date operator+ (Int b) const  { return toDate(add(idate,  b));}
	Date operator- (Int b) const  { return toDate(add(idate, -b));}
	Int  operator- (Date b) const { return add(idate, -b.idate);  }

	Date operator+=(const Num & b)
		{ idate = add(idate,  toInt(b)); return *this; }
	Date operator-=(const Num & b)
		{ idate = add(idate, -toInt(b)); return *this; }
	Date operator+ (const Num & b) const
		{ return toDate(add(idate,  toInt(b))); }
	Date operator- (const Num & b) const
		{ return toDate(add(idate, -toInt(b))); }

	bool isNull() const { return bool(idate == LONG_MIN); }

	void store(OutputStream &stream) const;
	void load(InputStream &stream);

	static Int storeLength() { return sizeof(long); }

	void store(char *&buff) const;
	void store(FILE *fp) const;
	static Date load(const char *&buff);
	static Date load(FILE *fp);
	void storeShort(char *&buff) const;
	static Date loadShort(const char *&buff);

	friend Date osDate(Int s);
	Int osDate();

	static void setFmt(DateFmt fmt);
	static DateFmt getFmt();

	friend Connection &operator<<(Connection &c, const Date &d);
	friend Connection &operator>>(Connection &c, Date &d);

};


// Time class

// Forward declarations needed for friend functions used in inline methods
Time toTime(Int secs);
Time toTime(const Num &secs);
Time toTime(const String &s);
Time toTime(Date d);

// This class was documented by Oscar (/usr2/acct/oscar/doc/itypes/itypes.doc)
class Time {

	friend class Date;
	friend Date toDate(Time t);

// private:

	long itime;

	void recalc_values() const;

public:

	Time() 							 { itime = LONG_MIN; }                  
	Time(const Time &t)              { *this = t; }
	Time(const char *s);
	Time(Int h, Int m, Int s);
	Time(Int d, Int h, Int m, Int s);
	Time(const Num &h, const Num &m, const Num &s);
	Time(const Num &d, const Num &h, const Num &m, const Num &s);

	friend Time toTime(Int secs);
	friend Time toTime(const Num &secs);

	friend Time toTime(const String &s);
	friend Time toTime(Date d);

	friend Int  toInt(const Time &t) { return t.itime; }

	friend const char *toCharPtr(Time t, TimeFmt fmt = DEFAULT_TFMT);
	const char *format(const char *mask);
	String format(const String &mask) const;
	friend String toString(Time t, TimeFmt fmt = DEFAULT_TFMT) 
		{ return String(toCharPtr(t, fmt)); }

	Int second()	const;
	Int minute()	const;
	Int hour()		const;

	Int seconds()	const;
	Int minutes()	const;
	Int hours()		const;
	Int days()		const;

	friend Int cmp(const Time &a, const Time &b)
		{ return a.itime == b.itime ? 0 : (a.itime < b.itime ? -1 : 1); }
	bool operator< (Time b) const	{ return itime <  b.itime; }
	bool operator> (Time b) const	{ return itime >  b.itime; }
	bool operator<=(Time b) const	{ return itime <= b.itime; }
	bool operator>=(Time b) const	{ return itime >= b.itime; }
	bool operator==(Time b) const	{ return itime == b.itime; }
	bool operator!=(Time b) const	{ return itime != b.itime; }

	Time &operator=(const Time &b);
	

#if (__GNUC__ >= 8)
	Time &operator+=(Int b)   { itime = idx::add(itime, b); 	  return *this; }
	Time &operator-=(Int b)   { itime = idx::add(itime,-b); 	  return *this; }
	Time &operator*=(Int b)   { itime = idx::mult(itime, b); 	  return *this; }
	Time &operator/=(Int b)   { itime = idx::div(itime, b); 	  return *this; }
	Time &operator+=(Time b)  { itime = idx::add(itime, b.itime); return *this; }
	Time &operator-=(Time b)  { itime = idx::add(itime,-b.itime); return *this; }
	Time &operator++()		  { itime = idx::add(itime,  1);      return *this; }
	Time &operator--()		  { itime = idx::add(itime, -1); 	  return *this; }
#else
	Time &operator+=(Int b)   { itime=add(itime, b); 	  	  return *this; }
	Time &operator-=(Int b)   { itime=add(itime,-b); 	  	  return *this; }
	Time &operator*=(Int b)   { itime=mult(itime, b); 	  	  return *this; }
	Time &operator/=(Int b)   { itime=div(itime, b); 	  	  return *this; }
	Time &operator+=(Time b)  { itime=add(itime, b.itime); 	  return *this; }
	Time &operator-=(Time b)  { itime=add(itime,-b.itime); 	  return *this; }
	Time &operator++()		  { itime = add(itime,  1);       return *this; }
	Time &operator--()		  { itime = add(itime, -1); 	  return *this; }
#endif


	Time operator+ (Time b) const { return toTime(add(itime, b.itime));}
	Time operator- (Time b) const { return toTime(add(itime,-b.itime));}

	void store(OutputStream &stream) const;
	void load(InputStream &stream);

	bool isNull() const { return bool(itime == LONG_MIN); }

	void store(char *&buff) const;
	void store(FILE *fp) const;
	static Time load(const char *&buff);
	static Time load(FILE *fp);
	void storeShort(char *&buff) const;
	static Time loadShort(const char *&buff);

	Int osTime();

	static Int storeLength() { return sizeof(long); }

	friend Time osTime(Int s);
	friend Connection &operator<<(Connection &c, const Time &t);
	friend Connection &operator>>(Connection &c, Time &t);
};

// DALI-STANDALONE: Forward declaration for ADL visibility (friend function with primitive arg)
Time osTime(Int s);

// This class was documented by Oscar (/usr2/acct/oscar/doc/itypes/itypes.doc)
class Boolean {

// private:

	int val;

public:

	Boolean() 						{ val = BOOL_NULL;	}
    Boolean(const Boolean &b)       { *this = b; 		}
	Boolean(bool v) 				{ val = v; 			}

#ifdef HAVE_BOOL
	Boolean(int i) 					{ val = i; 			}
#endif 

	Boolean(const char *s);

	friend Boolean toBoolean(Int i);
	friend Boolean toBoolean(const Num &v);
	friend Boolean toBoolean(const String &s)
	{ return Boolean(toCharPtr(s)); }

	friend Int  toInt(const Boolean &t)
	{ return t.val == BOOL_NULL ? INT_NULL : t.val; }

	friend const char *toCharPtr(Boolean t);
	const char *format(const char *mask = NULL) const;

	friend String toString(Boolean t) { return String(toCharPtr(t)); }
	String format(const String &mask) const
		{ return format(toCharPtr(mask)); }

	friend Int cmp(const Boolean &a, const Boolean &b)
		{ return a.val-b.val; }
	bool operator< (Boolean b) const	{ return val <  b.val; }
	bool operator> (Boolean b) const	{ return val >  b.val; }
	bool operator<=(Boolean b) const	{ return val <= b.val; }
	bool operator>=(Boolean b) const	{ return val >= b.val; }
	bool operator==(Boolean b) const	{ return val == b.val; }
	bool operator!=(Boolean b) const	{ return val != b.val; }

	Boolean &operator=(const Boolean &b);
	operator bool() const	{ return val == BOOL_NULL ? false : val; }
	bool isNull() const		{ return val == BOOL_NULL; }
	bool operator!() const	{ return !bool(*this); }

	void store(OutputStream &stream) const;
	void load(InputStream &stream);

	static Int storeLength();
	void store(char *&buff) const;
	static Boolean load(const char *&buff);
};

#endif
