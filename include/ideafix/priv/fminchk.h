/*
 * $Id: fminchk.h,v 1.1.1.1 1996/04/16 18:51:41 gustavof Exp $
 */

#ifndef FMINCHK_H
#define FMINCHK_H

#pragma interface

#include <ideafix/priv/fmrep.h>
#include <ideafix/priv/fmexpr.h>
#include <cfix.h>

class EnumObject;
class InCheck;
class IxIntegrity;
class gtype(Array, String);

class FmCheck {
	FmValueObj *field_;
	bool negated_;
	
protected:
	FmCheck(FmValueObj *field, bool negated = false);
	
	virtual bool load(FILE *in);
	virtual void store(FILE *out) const;
	virtual String formatToString() const;

public:
	virtual ~FmCheck();

	virtual void realize();

	virtual bool valid(const String &value);
	virtual Int lastIndex() const;
	
    inline FmValueObj *field();
	inline bool negated() const;
	inline void setNegated(bool n);
};

class InListCheck : public FmCheck {
	typedef FmCheck inherited;
	
	gtype(Array, String)	&values_;
	gtype(Array, String)	&descrs_;
	
	Int lastIndex_;
	
public:
	InListCheck(FmValueObj *field, bool negated = false);
	InListCheck(FmValueObj *field, InCheck *chk);
	InListCheck(FmValueObj *field, EnumObject *enumObj, bool negated = false);
	~InListCheck();
	
	void realize();
	
	void addValue(const String &value, const String &descr = NULL_STRING);

	Int numValues() const;
	const String &getValue(Int index) const;
	const String &getDescr(Int index) const;
	
	Int maxValueLength() const;
	Int maxDescrLength() const;
	Int minDescrLength() const;
	
	Int lastIndex() const;
	bool valid(const String &str);
	
	String formatToString() const;
	bool load(FILE *in);
	void store(FILE *out) const;
};

class DisplayField;
class KeyValue;

class gtype(PtrArray, DisplayField);
class gtype(PtrArray, KeyValue);

class InTableCheck : public FmCheck {
	typedef FmCheck inherited;
	
	dbindex index_;					// in table access index
	String	tableName_;			// table name that index belongs to
	String	indexName_;			// name of the index

	PtrArray(DisplayField)	&dspFields_;
	PtrArray(KeyValue)		&keys_;

public:
	InTableCheck(FmValueObj *field, bool negated = false);
	InTableCheck(FmValueObj *field, IxIntegrity *chk);
	~InTableCheck();

	void realize();
	
	bool valid(const String &str);
	
	String formatToString() const;
	bool load(FILE *in);
	void store(FILE *out) const;
};

class DisplayField {
	dbfield dbField_;
	String tableName_;
	String fieldName_;

public:
	inline DisplayField();
	inline DisplayField(dbfield fld, const String &tblName, const String &fldName);
	
	void realize();
	
	inline dbfield field() const;
	inline const String &tableName() const;
	inline const String &fieldName() const;
	
	bool load(FILE *in);
	void store(FILE *out) const;
};

class KeyValue {
public:
	enum Id {
		exprKey,
		dbKey,
		fmKey,
		constKey
	};
	
	virtual ~KeyValue();
	
	virtual void realize() = 0;
	
	virtual String value(Int row) = 0;
    
    static KeyValue *loadById(FILE *in, FmValueObj *obj);
    static void storeId(FILE *in, Id id);
    
	virtual bool load(FILE *in) = 0;
	virtual void store(FILE *out) const = 0;
};

class DbKeyValue : public KeyValue {
	dbfield	dbField_;
    String tableName_;
    String fieldName_;
    
public:
	inline DbKeyValue();
	inline DbKeyValue(dbfield fld, const String &tblName, const String &fldName);
	virtual ~DbKeyValue();

	virtual void realize();
	
	virtual String value(Int row);

	virtual bool load(FILE *in);
	virtual void store(FILE *out) const;
};

class FmKeyValue : public KeyValue {
	FmValueObj *fmField_;
	String formName_;
	String fieldName_;
	
public:
	inline FmKeyValue();
	inline FmKeyValue(const String &formName, const String &fldName);
	virtual ~FmKeyValue();

	virtual void realize();
	
	virtual String value(Int row);

	virtual bool load(FILE *in);
	virtual void store(FILE *out) const;
};

class ExprKeyValue : public KeyValue, public FmExpr {
	
public:
	inline ExprKeyValue(FmValueObj *obj);
	inline ExprKeyValue(FmValueObj *obj, const String &value);
	virtual ~ExprKeyValue();

	virtual void realize();
	
	virtual String value(Int row);

	virtual bool load(FILE *in);
	virtual void store(FILE *out) const;
};

class ConstKeyValue : public KeyValue {
	String value_;
	
public:
	inline ConstKeyValue();
	inline ConstKeyValue(const String &value);
	virtual ~ConstKeyValue();

	virtual void realize();
	
	virtual String value(Int row);

	virtual bool load(FILE *in);
	virtual void store(FILE *out) const;
};

#include <ideafix/priv/fminchk.icc>

#endif

