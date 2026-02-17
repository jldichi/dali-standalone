//- Copyright Notice
// -----------------------------------------------------------------------
// (C) Copyright 1998 InterSoft S.A.  All Rights Reserved
// THIS IS UNPUBLISHED PROPRIETARY SOURCE CODE OF InterSoft S.A.
// The copyright notice above does not evidence any actual or intended 
// publication of such source code.
//
// $Id: field.h,v 1.13 2002/03/19 21:10:15 hardaiz Exp $
// -----------------------------------------------------------------------

#ifndef	FWORKFIELD_H
#define	FWORKFIELD_H

#include <ifound/str.h>
#include <sqldb/fwork/object.h>
#include <sqldb/fwork/conslist.h>
#include <sqldb/fwork/dtypesimpl.h>
#include <sqldb/metadata/fieldmd.h>

/**
 * This class is used to represent SQL fields. It should be regarded as an
 * abstract class as there is no sense in having a generic Field. There should
 * only be specific-like fields (CharacterField, IntegerField, etc.)
 * 
 * @version
 * @author Eduardo Chiocconi
 *
 * @see BitField
 * @see IntegerField
 * @see FloatField
 * @see DecimalField
 * @see CharacterField
 * @see NumericField
 */
class Field : public Object {

public:
	Field(const String &name, const ConstraintList &c, Int dim = 0);
	virtual ~Field();

	// Get Information methods.
	inline String name() const;
	inline ConstraintList& constraintList() const;

	virtual Int getTypeCode() const = 0;
	virtual bool hasLength() const = 0;
	virtual Int length() const;
	virtual bool hasDecimals() const = 0;
	virtual Int nDecs() const;
	inline  Int getDim() const			{ return dim_d; }
	String getTypeAsString() const;

	// Obtain default constraint and other ones.
	String getDefault() const;
	String getConstraints() const;

	String convertToString() const;
	Object *clone() const = 0;

	// Debug
	virtual void dump() const;

protected:
	String name_d;
	ConstraintList *cl_d;
	Int dim_d;
	static DataTypesImpl *dtypesImpl_d;

	// These are protected to prevent its use
	Field();

private:
	
	const DataTypesImpl *getDataTypesManager() const;

	Field(const Field &c);
	Field &operator=(const Field &c);
};

#include <sqldb/fwork/field.icc>

#endif	// FWORKFIELD_H
