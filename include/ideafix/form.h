/********************************************************************
* Copyright (c) 1994 InterSoft Co.  All Rights Reserved
* THIS IS UNPUBLISHED PROPRIETARY SOURCE CODE OF InterSoft Co.
* The copyright notice above does not evidence any
* actual or intended publication of such source code.
*
* $Id: form.h,v 1.2 1996/05/14 15:56:53 gustavof Exp $
*
* DESCRIPTION
* Form API
*********************************************************************/

#ifndef FORM_H
#define FORM_H

#ifdef __GNUC__
#	pragma interface
#endif

/* toDo: for the moment... */
#include <ideafix/formtype.h>

#ifdef __GNUC__
#	pragma interface
#endif

class FormRep;
class FmFieldRep;
class MsgTable;
class FmField;

class Form {

// private:

	// representation
	FormRep *rep;

public:

	// Constructors & destructors
	Form(const String &name, enum Fm::Flag flags = Fm::EABORT);
	virtual ~Form();

	// execute the form
	Fm::Event execute(Fm::Fp precond = NULL, Fm::Fp postcond = NULL);

	// display the form
	void display();

	// hide the form
	void undisplay();

	// clear the specified fields in the form
	void clearFlds(Int from = 0, Int to = INT_MAX, Int rowno = 0);

	// update the specified fields in the form
	void showFlds(Int from = 0, Int to = INT_MAX, Int rowno = 0);

#ifndef WXFM
	// set/get form status
	void status(Fm::Status s);
	Fm::Status status() const;
#endif

	// recalculate fields in the form
	void recalc(Int currRow = 0);

	// Access to fields
	FmField operator()(Int nfld, Int rowno = 0);
	FmField operator()(const String &fldname, Int rowno = 0);
	FmField field(Int nfld, Int rowno = 0);
	FmField field(const String &fldname, Int rowno = 0);
};

class FmField {

// private:

	FmFieldRep *rep;
	Int row;

public:

	FmField(FmFieldRep *r, Int rowno = 0);		// toDo: private?

	// Constructors & destructors
	FmField(const FmField &f);
	virtual ~FmField();

#if 0
	// index
	FmField operator[](Int nfld);

	// assignment from another field
	FmField &operator=(const FmField &f);

	// Int access
	FmField &operator=(Int i);
	friend Int toInt(const FmField &f);
	operator Int() const;

	// Num access
	FmField &operator=(const Num &n);
	friend Num toNum(const FmField &f);
	operator Num() const;

	// String access
	FmField &operator=(const String &s);
	friend String toString(const FmField &f);
	operator String() const;

	// Date access
	FmField &operator=(Date d);
	friend Date toDate(const FmField &f);
	operator Date() const;

	// Time access
	FmField &operator=(Time t);
	friend Time toTime(const FmField &f);
	operator Time() const;
#endif
	Fm::Action locked();
	Fm::Action eof();

	// (pablog)
	Int fldNo();
	Int rowNo();
	FormRep *formRep();
};

#endif
