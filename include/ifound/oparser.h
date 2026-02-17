/********************************************************************
* Copyright (c) 1991 InterSoft Co.  All Rights Reserved
* THIS IS UNPUBLISHED PROPRIETARY SOURCE CODE OF InterSoft Co.
* The copyright notice above does not evidence any
* actual or intended publication of such source code.
*
* $Id: oparser.h,v 1.1.1.1 1996/04/16 18:51:32 gustavof Exp $
*
* DESCRIPTION
* Interface to option parsing class
*********************************************************************/

#ifndef OPARSER_H
#define OPARSER_H

#include <ifound.h>

#ifdef __GNUC__
#pragma interface
#endif

#define NEG_PREFIX	"no-"
#define STR_REP		"str"
#define NUM_REP		"num"
#define BOOL_REP	""
//#include <ifound/itypes.h>
#include <ifound/str.h>

// only used for WinOptionParser
class wxPanel;
class wxFrame;
class wxText;
class wxCheckBox;
class wxRadioBox;
class wxListBox;

class Option {

protected:

	WideChar lett; // option letter
	String seq;	// option sequence
	String dsc;	// option description

public:

	inline Option(const String &s, WideChar l, const String &desc);
	inline Option(const Option &o);

	Option &operator=(const Option &o);

	inline WideChar letter() const;
	inline const String &sequence() const;
	String description() const;

	virtual void setValue(bool negation = false, const char *v = NULL);
	virtual bool hasArguments() const = 0;
	virtual String argRep() const = 0;

	// only used for WinOptionParser
	virtual void createItem(wxPanel *thePanel) = 0;
	virtual void assignItem() = 0;
};

class StrOption : public Option {

// private:

	String &str;
	Int &val;
	String values;

	// only used for WinOptionParser
	union {
		wxText *text_;
		wxRadioBox *rbox_;
	};

	static Int dummyVal;

	Int findVal(const String &s);

public:

	// Constructors & destructors
	StrOption(String &var, const String &s,	const String &desc, 
			const String &def = NULL_STRING,
			const String &vals = NULL_STRING);

	StrOption(String &var, Int &nval, const String &s, const String &desc, 
			const String &def = NULL_STRING,
			const String &vals = NULL_STRING);

	StrOption(String &var, const String &s, WideChar l, const String &desc, 
			const String &def = NULL_STRING,
			const String &vals = NULL_STRING);

	StrOption(String &var, Int &nval, const String &s, WideChar l,
			const String &desc,
			const String &def = NULL_STRING,
			const String &vals = NULL_STRING);

	StrOption(const StrOption &so);

	void setValue(bool negation = false, const char *v = NULL);
	bool hasArguments() const;
	String argRep() const;

	// only used for WinOptionParser
	virtual void createItem(wxPanel *thePanel);
	virtual void assignItem();
};

class NumOption : public Option {

// private:

	Int &n;
	Int min, max;

	// only used for WinOptionParser
	wxText *text_;

public:

	// Constructors & destructors
	NumOption(Int &var, const String &s, const String &desc, Int def = 0,
			Int minimum = INT_MIN, Int maximum = INT_MAX);

	NumOption(Int &var, const String &s, WideChar l, const String &desc,
			Int def = 0, Int minimum = INT_MIN, Int maximum = INT_MAX);

	NumOption(const NumOption &no);

	void setValue(bool negation = false, const char *v = NULL);
	bool hasArguments() const;
	String argRep() const;

	// only used for WinOptionParser
	virtual void createItem(wxPanel *thePanel);
	virtual void assignItem();
};

class BoolOption : public Option {

// private:

	bool &b;

	// only used for WinOptionParser
	wxCheckBox *checkbox_;

public:

	// Constructors & destructors
	BoolOption(bool &var, const String &s, const String &desc,
			bool def = false);

	BoolOption(bool &var, const String &s, WideChar l, const String &desc,
			bool def = false);

	BoolOption(const BoolOption &bo);

	void setValue(bool negation = false, const char *v = NULL);
	bool hasArguments() const;
	String argRep() const;

	// only used for WinOptionParser
	virtual void createItem(wxPanel *thePanel);
	virtual void assignItem();
};


class gtype(PtrArray,Option);

class OptionParser {

	friend class WinOptionParser;

// private:

	String version;
	String compileData_;
	gtype(PtrArray,Option) &opts;
    
    OptionParser(const OptionParser &);
    void operator=(const OptionParser &);

	// only used for WinOptionParser
	wxListBox *listbox_;

public:

	// Constructors & destructors
	OptionParser(const String &vers = NULL_STRING);
	virtual ~OptionParser();

	void addOption(Option *opt);
	inline OptionParser &operator<<(Option *opt);
	inline void compileData(const String &cdata);
	String help();
	Int parse(int &argc, char **argv);

	// only used for WinOptionParser
	virtual bool execute();
	void assign();
	inline wxListBox *listBox();
};

inline
Option *Opt(String &v, const String &s, const String &desc,
		const String &def = NULL_STRING,
		const String &vals = NULL_STRING);

inline
Option *Opt(String &v, Int &nv, const String &s, const String &desc,
		const String &def = NULL_STRING,
		const String &vals = NULL_STRING);

inline
Option *Opt(Int &v, const String &s, const String &desc,
		Int def = 0, Int minimum = INT_MIN, Int maximum = INT_MAX);

inline
Option *Opt(bool &v, const String &s, const String &d, bool def = false);

/*
	The same utility functions but allowing the specification of a
	single letter.
*/
inline
Option *Opt(String &v, const String &s, WideChar l, const String &desc,
		const String &def = NULL_STRING,
		const String &vals = NULL_STRING);

inline
Option *Opt(String &v, Int &nv, const String &s, WideChar l,
		const String &desc,
		const String &def = NULL_STRING,
		const String &vals = NULL_STRING);

inline
Option *Opt(Int &v, const String &s, WideChar l, const String &desc,
		Int def = 0, Int minimum = INT_MIN, Int maximum = INT_MAX);

inline
Option *Opt(bool &v, const String &s, WideChar l, const String &d,
		bool def = false);

#include <ifound/oparser.icc>

#endif	// OPARSER_H
