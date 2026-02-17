// -------------------------------------------------------------------------
// Copyright (c) 1995 InterSoft Co.  All Rights Reserved
// THIS IS UNPUBLISHED PROPRIETARY SOURCE CODE OF InterSoft Co.
// The copyright notice above does not evidence any actual or intended 
// publication of such source code.
//
// $Id: gridcol.h,v 1.1.1.1 1996/04/16 18:51:41 gustavof Exp $
//
// Description:
// -------------------------------------------------------------------------

#ifndef GRIDCOL_H
#define GRIDCOL_H

#include <ifound.h>
#include <ifound/str.h>

// need Grid enums
#include <ideafix/grid.h>

class String;
class Num;
class Boolean;
class Time;
class Date;
class GridIcon;

class GridColumn
{

	useIdCheck(GridColumn_id);

protected:

	String head_;         // Header
	String mask_;         // Format mask
	Int width_;           // Width of the column
	Int type_;            // Type of the column
	String buff_;         // Buffer for this column
          
	Int real_offset_;
	Int real_width_;
	bool locked_;
	bool showed_;

	bool clickable_;
	bool sizeable_;
	bool lockable_;
	bool printable_;

public:

	GridColumn(const String &h, Int w, Int t = TY_STRING,
			Int flags = ERR, String mask = NULL_STRING);
	virtual ~GridColumn(void);
	
	virtual void* userData() { return NULL;}  // pointer to user data (if any)
	virtual String value()	 { return NULL_STRING; } // string of ^^( no format)
	virtual void value(const String&) { return; }    // string to ^^
	inline bool locked(void) const;
	inline void locked(bool lk);

	inline bool show(void) const;
	inline void show(bool sh);

	inline bool clickable(void) const;
	inline void clickable(bool cl);

	inline bool sizeable(void) const;
	inline void sizeable(bool cl);

	inline bool lockable(void) const;
	inline void lockable(bool cl);

	inline bool printable(void) const;
	inline void printable(bool cl);

	inline Int type(void) const;
	inline const String &header(void) const;

	inline String buff(void);
	virtual String format(String str);

	inline Int width(void) const;
	void width(Int width);

	inline Int offset(void) const;
	inline void offset(Int offset);

	inline void flags(Int f);

	virtual void operator<<(const String &);
	virtual void operator<<(const char *);
};

class SpecialCol : public GridColumn
{

public:

	SpecialCol(void);
	~SpecialCol(void);
	void lock(bool) {};
};

class StringColumn : public GridColumn
{

	String *theString_d;

public:

	StringColumn(String &theString, const String &header, Int w,Int f,
		String mask);
	StringColumn(const String &header, Int w, Int f, String mask);
	~StringColumn(void);
	void* userData();
	String value();
	void   value(const String&);
	String format(String str);

};

class NumColumn : public GridColumn
{

	Num *theNum_d;
	Int ndec_, ndig_, sign_;
	char filler_;
	bool separ_;
	bool is_mask_;

public:

	NumColumn(Num &theNum, const String &header, Int w, Int f,
			String mask );
	NumColumn(Num &theNum, const String &header, Int w, Int f, Int ndig,
			  Int ndec , bool separ, Int sign, char filler);
	NumColumn(const String &header, Int w, Int f, String mask);
	NumColumn(const String &header, Int w, Int f, Int ndig,
			  Int ndec , bool separ, Int sign, char filler);
	~NumColumn(void);
	void* userData();
	String value();
	void   value(const String&);
	String format(String str);
};

class DateColumn : public GridColumn
{

	Date *theDate_d;

public:

	DateColumn(Date &theDate, const String &header, Int w, Int f,
			String mask);
	DateColumn(const String &header, Int w, Int f, String mask);
	~DateColumn(void);
	void* userData();
	String value();
	void   value(const String&);
	String format(String str);
};

class TimeColumn : public GridColumn
{

	Time *theTime_d;

public:

	TimeColumn(Time &theTime, const String &header, Int w, Int f,
			String mask);
	TimeColumn(const String &header, Int w, Int f, String mask);
	~TimeColumn(void);
	void* userData();
	String value();
	void   value(const String&);
	String format(String str);
};

class BoolColumn : public GridColumn
{

	Boolean *theBoolean_d;

public:

	BoolColumn(Boolean &theBoolean, const String &header, Int w, Int f,
			String mask);
	BoolColumn(const String &header, Int w, Int f, String mask);
	~BoolColumn(void);
	void* userData();
	String value();
	void   value(const String&);
	String format(String str);
};

class IntColumn : public GridColumn
{

	Int *theInt_d;

public:

	IntColumn(Int &theInt, const String &header, Int w, Int f,
			String mask);
	IntColumn(const String &header, Int w, Int f, String mask);
	~IntColumn(void);
	void* userData();
	String value();
	void   value(const String&);
	String format(String str);
};

class IconColumn : public GridColumn
{

	GridIcon *theIcon_d;

public:

	IconColumn(GridIcon &theIcon, const String &header, Int w, Int f,String mask);
	IconColumn(const String &header, Int w, Int f,String mask);
	~IconColumn(void);
	void* userData();
	String value();
	void   value(const String&);
	String format(String str);
};

#include <ideafix/priv/gridcol.icc>

#endif
