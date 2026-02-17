/********************************************************************
* Copyright (c) 1992 InterSoft Co.  All Rights Reserved
* THIS IS UNPUBLISHED PROPRIETARY SOURCE CODE OF InterSoft Co.
* The copyright notice above does not evidence any
* actual or intended publication of such source code.
*
* $Id: incheck.h,v 1.3 1999/02/08 22:31:13 eduardoc Exp $
*
* DESCRIPTION
* Class definitions for In Check type.
*********************************************************************/

#ifndef INCHECK_H
#define INCHECK_H

#ifdef __GNUC__
#pragma interface
#endif

#include <ifound.h>

#include <ifound/anyvalue.h>
#include <ifound/str.h>

class gtype(PtrArray, AnyValue);
class Connection;
class DbEssSet;
struct s_schema;

enum InCheckType {
	IC_INTEGRITY,
	IC_INLIST,
	IC_EMPTY
};


class InCheck {

	friend void convertSv(InCheck &ic);
	friend void sendConvertedSv(InCheck &ic, Connection &c);
	friend void convertIx(InCheck &ic, s_schema *sp);
	friend void sendConvertedIx(InCheck &ic, Connection &c);

	friend class DbEssSet;
	friend void sendConvertedDb(InCheck &ic, Connection &c);

// private:

    InCheck(const InCheck &);
    void operator=(const InCheck &);

protected:

	Boolean neg;
	gtype(PtrArray, AnyValue) &keyValues;
	gtype(PtrArray, AnyValue) &dspFields;

public:

	InCheck();
	explicit InCheck(Connection &c);
	explicit InCheck(bool negated);
	explicit InCheck(Int nk, Int nd = 0, bool negated = false);
	virtual ~InCheck();

	// Key value adm.
	void addKeyVal(const AnyValue &val);
	void setNValues(Int n);
	Int nKeyValues() const;
	AnyValue &keyValue(Int i) const;

	// Display field adm.
	void addDspFld(const AnyValue &val);
	Int nDspFields() const;
	AnyValue &dspField(Int i) const;

	bool negation() const				{ return bool(neg); }
	void setNegation(bool v = true)		{ neg = v; }

	operator void *()			{ return neg.isNull() ? NULL : this; }
	bool operator!()			{ return neg.isNull(); }

	void store(char *&buff) const;
	void store(FILE *fp) const;
	static InCheck *loadIx(const char *&buff);
	static InCheck *loadIx(FILE *fp);

	static InCheck *loadDb(const char *&buff);
	static InCheck *loadDb(FILE *fp);

	virtual InCheckType getType() const;
	static InCheckType receive(Connection &c);
	static void sendEmpty(Connection &c);

	void print() const;
	String dumpInCheck() const;
};

class Integrity : public InCheck {

// private:

	Integrity(const Integrity &);

protected:

	ObjectId ind;

public:

	Integrity();
	explicit Integrity(Connection &c);
	Integrity(bool negated, const ObjectId &indid);
	explicit Integrity(const ObjectId &indid, Int nk = 0, Int nd = 0,
			bool negated = false);
	virtual ~Integrity();

	void setIndex(const ObjectId &i)	{ ind = i; }
	const ObjectId &index()				{ return ind; }

	// toDo: dejar?
	String tabName() const;
	String indName() const;

	InCheckType getType() const;
};

class InList : public InCheck {

// private:

	InList(const InList &);

public:

	InList();
	explicit InList(Connection &c);
	explicit InList(bool negated);
	explicit InList(Int nk, Int nd = 0, bool negated = false);
	virtual ~InList();

	Int maxDspLen() const;
	InCheckType getType() const;
};

#endif
