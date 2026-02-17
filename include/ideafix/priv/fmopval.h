/*
 * Copyright (c) 1995 InterSoft Co.  All Rights Reserved
 * THIS IS UNPUBLISHED PROPRIETARY SOURCE CODE OF InterSoft Co.
 * The copyright notice above does not evidence any
 * actual or intended publication of such source code.
 *
 * $Id: fmopval.h,v 1.1.1.1 1996/04/16 18:51:41 gustavof Exp $
 *
 */

#ifndef FMOPTVAL_H
#define FMOPTVAL_H

#ifdef __GNUG__
#pragma interface
#endif

#include <ideafix.h>
#include <ifound.h>
#include <ideafix/priv/fmopname.h>

class wxColour;
class OptionValue;
class FormalArg;
class gtype(PtrArray, FormalArg);
class FmObject;
class gtype(PtrArray, FmObject);

// pseudo class for all kind of objects
struct It {
	enum {
		form,
		multiline,
		subform,
		group,

		charfield,
		num,
		date,
		time,
		boolfield,
		floatfield,

		checkbox,
		radiobox,
		listbox,
		slider,
		meter,

		action,
		toolbar,
		
		button,
		bitmap,
		separator,
		text,
		rectangle,
		ellipse,
		polyline
	};
};

// pseudo class for sections of object options
struct St {
	enum {
		none		= 0,
		
		form		= 1 << It::form,
		multiline	= 1 << It::multiline,
		subform		= 1 << It::subform,
		group		= 1 << It::group,
		
		charfield	= 1 << It::charfield,
		num			= 1 << It::num,
		date		= 1 << It::date,
		time		= 1 << It::time,
		boolfield	= 1 << It::boolfield,
		floatfield	= 1 << It::floatfield,
		
		checkbox	= 1 << It::checkbox,
		radiobox	= 1 << It::radiobox,
		listbox		= 1 << It::listbox,
		slider		= 1 << It::slider,
		meter		= 1 << It::meter,

		action		= 1 << It::action,
		toolbar		= 1 << It::toolbar,

		button		= 1 << It::button,
		bitmap		= 1 << It::bitmap,
		separator	= 1 << It::separator,
		text		= 1 << It::text,
		rectangle	= 1 << It::rectangle,
		ellipse		= 1 << It::ellipse,
		polyline	= 1 << It::polyline,

		allStatic	= bitmap | separator | text | rectangle | 
						ellipse | polyline,
					  
		textField	= charfield | num | date | time | boolfield | floatfield,
		numField	= slider | num,
		valueField	= textField | checkbox | radiobox | listbox | 
						slider | meter,
		allField	= valueField | multiline | group | subform,

		all			= 0xffffffff
	};
};

// pseudo class for all types of options
class FmOption {
public:
	typedef FmOpt::Name Name;
	
	enum Type {
		voidOpt,
		stringOpt,
		numOpt,
		geometryOpt,
		boolOpt,
		colorOpt,
		msgOpt,
		exprOpt,
		filenameOpt,
		dbfieldOpt,
		subformOpt,
		objlistOpt
	};
	enum ColorType {
		colorIsVoid,
		colorIsNum,
		colorIsStr
	};
	
private:
	Name name_;
	OptionValue *value_;
	
	struct OptionTable {
		const char	*name_;		// last = NULL
		Int			opjects_;	// St mask
		Type		optType_;
		Name		optName_;
	};
	static const OptionTable optTable[];
	
	void init(const OptionTable *t, const OptionValue &value);
	void init(const OptionTable *t, OptionValue *value);
	static const OptionTable *find(const String &name);
	static const OptionTable *find(Name name);
	static OptionValue nullValue_;
	
public:
	inline FmOption(const String &name, const OptionValue &value);
	inline FmOption(Name name, const OptionValue &value);
	inline FmOption(const String &name, OptionValue *value);
	inline FmOption(Name name, OptionValue *value);
	inline FmOption(const FmOption &opt);
	inline FmOption();
	~FmOption();

	inline static Type type(const String &name);
	inline static Type type(Name name);

	inline const FmOption &operator=(const FmOption &opt);

	inline Name name() const;
	inline operator Name() const;
	inline bool operator==(Name) const;
	inline bool operator!=(Name) const;

	inline const OptionValue *operator->() const;
	inline const OptionValue *value() const;
	
	inline Type type() const;
	inline operator Type() const;
	inline bool operator==(Type) const;
	inline bool operator!=(Type) const;

	inline operator bool() const;
	inline bool operator!() const;

	String toString() const;
};

//
// *** class OptionValue
//
class OptionValue {
	FmOption::Type type_;
public:
	inline OptionValue();
	inline OptionValue(FmOption::Type t);
	virtual ~OptionValue();
	
	virtual OptionValue *clone() const;
	virtual String asString() const;
	virtual String typeName() const;
	
	inline FmOption::Type type() const;
	
	virtual String stringVal() const;
	virtual String fileNameVal() const;
	virtual String exprVal() const;
	virtual Num numVal() const;
	virtual void geomVal(Int &w, Int &h, Int &x, Int &y) const;
	virtual void msgVal(String &f, String &s, String &m) const;
	virtual bool boolVal() const;
	virtual wxColour *getColorVal() const;
	virtual String stringColorVal() const;
	virtual FmOption::ColorType colorType() const;
	virtual Int dbFieldVal() const;
	virtual gtype(PtrArray, FormalArg) *subFormVal(String &name);
	virtual gtype(PtrArray, FmObject) *objectListVal() const;
};

//
// *** class StringVal
//
class StringVal : public OptionValue {
	typedef OptionValue inherited;
	String val_;

public:
	inline StringVal(const StringVal &val);
	inline StringVal(const String &s);

	virtual OptionValue *clone() const;
	virtual String asString() const;
	virtual String typeName() const;
	
	virtual String stringVal() const;
};

//
// *** class NumVal
//
class NumVal : public OptionValue {
	typedef OptionValue inherited;
	Num val_;

public:
	inline NumVal(const NumVal &val);
	inline NumVal(const String &s);
	inline NumVal(Num n);
	inline NumVal(Int i);

	virtual OptionValue *clone() const;
	virtual String asString() const;
	virtual String typeName() const;
	
	virtual Num numVal() const;
};

//
// *** class GeometryVal
//
class GeometryVal : public OptionValue {
	typedef OptionValue inherited;
	Int width_;
	Int height_;
	Int x_;
	Int y_;

public:
	inline GeometryVal(const GeometryVal &val);
	inline GeometryVal(Int w = ERR, Int h = ERR, Int x = ERR, Int y = ERR);
	inline GeometryVal(const String &w, const String &h,
					   const String &x, const String &y);

	virtual OptionValue *clone() const;
	virtual String asString() const;
	virtual String typeName() const;
	
	virtual void geomVal(Int &w, Int &h, Int &x, Int &y) const;
};

//
// *** class BoolVal
//
class BoolVal : public OptionValue {
	typedef OptionValue inherited;
	bool val_;

public:
	inline BoolVal(const BoolVal &val);
	inline BoolVal(bool b = true);

	virtual OptionValue *clone() const;
	virtual String asString() const;
	virtual String typeName() const;
	
	virtual bool boolVal() const;
};

//
// *** class ColorVal
//
class ColorVal : public OptionValue {
	typedef OptionValue inherited;
	wxColour &val_;
	String strval_;
	FmOption::ColorType colorType_;

public:
	ColorVal();
	ColorVal(const ColorVal &val);
	ColorVal(UnChar r, UnChar g, UnChar b);
	ColorVal(const String &r, const String &g, const String &b);
	ColorVal(const String &col);
	~ColorVal();
	
	virtual OptionValue *clone() const;
	virtual String asString() const;
	virtual String typeName() const;
	
	virtual wxColour *getColorVal() const;
	virtual String stringColorVal() const;
	virtual FmOption::ColorType colorType() const;
};

//
// *** class MsgVal
//
class MsgVal : public OptionValue {
	typedef OptionValue inherited;
	String file_;
	String sect_;
	String msg_;

public:
	inline MsgVal(const MsgVal &val);
	inline MsgVal(const String &file, const String &sect, const String &msg);

	virtual OptionValue *clone() const;
	virtual String asString() const;
	virtual String typeName() const;
	
	virtual void msgVal(String &f, String &s, String &m) const;
};

//
// *** class ExprVal
//
class ExprVal : public OptionValue {
	typedef OptionValue inherited;
	String val_;

public:
	inline ExprVal(const ExprVal &val);
	inline ExprVal(const String &s);

	virtual OptionValue *clone() const;
	virtual String asString() const;
	virtual String typeName() const;
	
	virtual String exprVal() const;
};

//
// *** class FileNameVal
//
class FileNameVal : public OptionValue {
	typedef OptionValue inherited;
	String val_;

public:
	inline FileNameVal(const FileNameVal &val);
	inline FileNameVal(const String &s);

	virtual OptionValue *clone() const;
	virtual String asString() const;
	virtual String typeName() const;
	
	virtual String fileNameVal() const;
};

//
// *** class DbFieldVal
//
class DbFieldVal : public OptionValue {
	typedef OptionValue inherited;
	Int fld_;

public:
	inline DbFieldVal(const DbFieldVal &val);
	inline DbFieldVal(Int);

	virtual OptionValue *clone() const;
	virtual String asString() const;
	virtual String typeName() const;
	
	virtual Int dbFieldVal() const;
};

//
// *** class SubFormVal
//
class SubFormVal : public OptionValue {
	typedef OptionValue inherited;
	String name_;
	gtype(PtrArray, FormalArg) &args_;

public:
	SubFormVal(const SubFormVal &val);
	SubFormVal(const String &name);
	~SubFormVal();
	Int addArg(const String &argName, Int type);

	virtual OptionValue *clone() const;
	virtual String asString() const;
	virtual String typeName() const;
	
	virtual gtype(PtrArray, FormalArg) *subFormVal(String &name);
};

//
// *** class ObjectListVal
//
class ObjectListVal : public OptionValue {
	typedef OptionValue inherited;
	gtype(PtrArray, FmObject) &objs_;

public:
	ObjectListVal(const ObjectListVal &val);
	ObjectListVal();
	~ObjectListVal();
	void addObject(FmObject *obj);

	virtual OptionValue *clone() const;
	virtual String asString() const;
	virtual String typeName() const;
	
	virtual gtype(PtrArray, FmObject) *objectListVal() const;
};

#include <ideafix/priv/fmopval.icc>

#endif
