//- Copyright Notice
// -----------------------------------------------------------------------
// (C) Copyright 1998 InterSoft S.A.  All Rights Reserved
// THIS IS UNPUBLISHED PROPRIETARY SOURCE CODE OF InterSoft S.A.
// The copyright notice above does not evidence any actual or intended 
// publication of such source code.
//
// $Id: constraint.h,v 1.22 2004/12/16 18:04:07 damianm Exp $
// -----------------------------------------------------------------------

#ifndef	CONSTRAINT_H
#define	CONSTRAINT_H

#include <sqldb/fwork/object.h>

#include <sqldb/metadata/fieldmd.h>

#include <essentia/incheck.h>
#include <ifound/str.h>

/**
 * This class is used to represent constraints in a table.
 * If an error takes place while creating a constraint, then 
 * constraintOK_d member is set to false.
 * 
 * @author	Eduardo Chiocconi.
 *
 * @see		FieldConstraint
 * @see		TableConstraint
 */
class Constraint : public Object {

public:
	// Constructor & Destructor.
	Constraint();
	virtual ~Constraint();

	// Get Information methods.
	bool isConstraintValid() const;
	virtual bool isDefault() const = 0;
	virtual bool isNull() const = 0;
	virtual bool isInCheck() const = 0;
	virtual bool isReferentialConstraint() const = 0;
	virtual String convertToString() const = 0;
	virtual Object *clone() const = 0;
	
	// Setters

	// Debug
	void dump() const;

protected:
	bool constraintOK_d;

private:
	Constraint(const Constraint &c);
	Constraint &operator=(const Constraint &c);
};

/**
 * This class is used to represent SQL field constraints such as:
 *	- defaults.
 *	- checks.
 *	- not nulls.
 *	- in checks.
 *	- referential.
 *
 * @author	Eduardo Chiocconi.
 *
 * @see		Constraint
 * @see		CheckConstraint
 * @see		DefaultConstraint
 * @see		NotNullConstraint
 */
class FieldConstraint : public Constraint {

public:
	// Constructor & Destructor.
	FieldConstraint();
	virtual ~FieldConstraint();

	virtual bool isNull() 	const { return false;};
	virtual bool isDefault() const { return false;};
	virtual bool isCheck() 	const { return false;};
	virtual bool isInCheck() const { return false;};
	virtual bool isReferentialConstraint() const { return false;};
	virtual String convertToString() const = 0;
	virtual String getSQLStmt() const = 0;
	virtual Object *clone() const = 0;

	// Debug
	void dump() const;

private:
	FieldConstraint(const FieldConstraint &c);
	FieldConstraint &operator=(const FieldConstraint &c);
};

/**
 * This class is used to represent SQL table constraints such as:
 *	- primary key.
 *	- foreign keys.
 *
 * @author	Eduardo Chiocconi.
 * @see		Constraint
 * @see		PrimaryKeyConstraint
 * @see		UniqueConstraint
 */
class TableConstraint : public Constraint {

public:
	// Constructor & Destructor.
	TableConstraint();
	virtual ~TableConstraint();

	inline bool isNull() const { return false; }
	inline bool isDefault() const { return false; }
	inline bool isInCheck() const { return false; }
	virtual bool isReferentialConstraint() const { return false;};
	virtual String convertToString() const = 0;
	virtual String getSQLStmt() const = 0;
	virtual String constraintId() const = 0;
	virtual Object *clone() const = 0;

	// Debug
	void dump() const;

private:
	TableConstraint(const TableConstraint &c);
	TableConstraint &operator=(const TableConstraint &c);
};


// -------------- Field Constraints. -----------------------
/**
 * This class is used to represent SQL Not Null field constraint.
 *
 * @author	Eduardo Chiocconi.
 * @see		FieldConstraint
 */
class NotNullConstraint : public FieldConstraint {

public:
	// Constructor & Destructor.
	NotNullConstraint();
	~NotNullConstraint();

	inline bool isNull() const { return true; };

	String convertToString() const;
	String getSQLStmt() const;
	Object *clone() const;

	// Debug
	void dump() const;

private:
	NotNullConstraint(const NotNullConstraint &c);
	NotNullConstraint &operator=(const NotNullConstraint &c);
};

/**
 * This class is used to represent SQL check field constraints.
 *
 * @author	Eduardo Chiocconi.
 * @see		FieldConstraint
 */ 
class CheckConstraint : public FieldConstraint {

public:
	// Constructor & Destructor.
	CheckConstraint(const String &ownerField, const String &c);
	~CheckConstraint();

	inline bool isCheck() const { return true; }

	String convertToString() const;
	String getSQLStmt() const;
	Object *clone() const;

	// Debug
	void dump() const;

private:
	String constraint_d;
	String checkExpr_d;
	String ownerField_d;

	inline String getOwnerField() const { return ownerField_d; };
	inline String getCheckExpr() const { return checkExpr_d; };
	CheckConstraint(const CheckConstraint &c);
	CheckConstraint &operator=(const CheckConstraint &c);
};

/**
 * This class is used to represent SQL hardwired-in constraints.
 *
 * @author	Eduardo Chiocconi.
 * @see		FieldConstraint
 */
class InCheckConstraint : public FieldConstraint {

public:
	// Constructor & Destructor.
	InCheckConstraint(const FieldMetadata *fmd, const InCheck &ic);
	~InCheckConstraint();

	inline bool isInCheck() const { return true; }

	String convertToString() const;
	String getSQLStmt() const;
	Object *clone() const;

	// Debug
	void dump() const;
	String print() const;

private:
	InCheck *incheck_d;
	String constraint_d;
	FieldMetadata *fmd_d;

	inline InCheck *inCheck() const { return incheck_d; };
	inline FieldMetadata *fieldMetadata() const { return fmd_d; };
	InCheckConstraint(const InCheckConstraint &c);
	InCheckConstraint &operator=(const InCheckConstraint &c);
};

/**
 * This class is used to represent SQL foreign keys constraints.
 *
 * @author	Eduardo Chiocconi.
 * @see		FieldConstraint
 */
class ReferentialConstraint : public TableConstraint {

public:
	// Constructor & Destructor.
	ReferentialConstraint(const String &tabName, const String &inField, const InCheck &ic);
	~ReferentialConstraint();

	bool isReferentialConstraint() const { return true;};

	String convertToString() const;
	String getSQLStmt() const;
	String constraintId() const;
	Object *clone() const;

	bool referencedIndexWasCreated();
	bool referencedIndexIsUnique();
	
	// Debug
	void dump() const;
	String print() const;

private:
	InCheck *inCheck_d;
	String inField_d;
	String tabName_d;
	String constraint_d;

	inline String getInField() const { return inField_d; };
	inline String getTableName() const { return tabName_d; };
	inline InCheck *inCheck() const { return inCheck_d; };
	String getTableFields();
	String getInTableFields() const;
	ReferentialConstraint(const ReferentialConstraint &c);
	ReferentialConstraint &operator=(const ReferentialConstraint &c);
};

/**
 * This class is used to represent SQL default field constraints.
 *
 * @author	Eduardo Chiocconi.
 * @see		FieldConstraint
 */
class DefaultConstraint : public FieldConstraint {

public:
	// Constructor & Destructor.
	DefaultConstraint(const String &c, const FieldMetadata *fmd);
	~DefaultConstraint();

	inline bool isDefault() const { return true; }

	String convertToString() const;
	String getSQLStmt() const;
	Object *clone() const;

	// Debug
	void dump() const;

private:
	String constraint_d;
	String defExpr_d;
	FieldMetadata *fmd_d;

	inline String getDefaultExpr() const { return defExpr_d; };
	DefaultConstraint(const DefaultConstraint &c);
	DefaultConstraint &operator=(const DefaultConstraint &c);
	inline FieldMetadata *getFieldMetadata() const { return fmd_d; };

};

// -------------- Table Constraints. -----------------------

class SQLFWIndex;
class SQLFWTable;
class SQLFWField;
class FieldList;

/**
 * This class represents SQL Primary key constraints.
 *
 * @author	Eduardo Chiocconi.
 * @see		TableConstraint
 */
class PrimaryKeyConstraint : public TableConstraint {

public:
	// Constructor & Destructor.
	PrimaryKeyConstraint(const String &tab, const FieldList &pkflds);
	~PrimaryKeyConstraint();

	String convertToString() const;
	String getSQLStmt() const;
	String constraintId() const;
	Object *clone() const;

	// Debug
	void dump() const;

private:
	String constraint_d;
	String table_d;
	FieldList *fieldList_d;

	inline String table() const { return table_d; }
	inline FieldList *getFieldList() const { return fieldList_d; };
	PrimaryKeyConstraint(const PrimaryKeyConstraint &c);
	PrimaryKeyConstraint &operator=(const PrimaryKeyConstraint &c);
};

/**
 * This class is used to create unique constraints over a table. This is
 * the same as creating a unique index.
 *
 * @author	Eduardo Chiocconi.
 * @see		TableConstraint
 */
class UniqueConstraint : public TableConstraint {

public:
	// Constructor & Destructor.
	UniqueConstraint(const String &indexName, const FieldList &pkflds);
	~UniqueConstraint();

	String convertToString() const;
	String getSQLStmt() const;
	String constraintId() const;
	Object *clone() const;

	// Debug
	void dump() const;

private:
	String constraint_d;
	String indName_d;
	FieldList *fieldList_d;

	inline FieldList *getFieldList() const { return fieldList_d; };
	inline String getIndexName() const { return indName_d; };
	UniqueConstraint(const ReferentialConstraint &c);
	UniqueConstraint &operator=(const ReferentialConstraint &c);
};

#endif	// CONSTRAINT_H
