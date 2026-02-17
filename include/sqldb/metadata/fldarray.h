//- Copyright Notice
// -----------------------------------------------------------------------
// (C) Copyright 1998 InterSoft S.A.  All Rights Reserved
// THIS IS UNPUBLISHED PROPRIETARY SOURCE CODE OF InterSoft S.A.
// The copyright notice above does not evidence any actual or intended 
// publication of such source code.
//
// $Id: fldarray.h,v 1.6 2002/03/19 21:21:28 hardaiz Exp $
// -----------------------------------------------------------------------

#ifndef FLDARRAY_H
#define	FLDARRAY_H

#include <ideafix.h>
#include <ideafix/priv/dbdefs.h>
#include <sqldb/metadata/fieldmd.h>

/**
 * This class is used to encapsulate an array of fields.
 * 
 * @version
 * @author Eduardo Chiocconi.
 * @see	FieldMetadata
 */
class FieldArray {

public:
	FieldArray(Int len);
	~FieldArray();
	
	// Set methods.
	void setElementAt(Int i, const FieldMetadata &fmd);

	// Get methods.
	inline struct s_field *fieldArrayRep() const;
	inline Int length() const;
	inline const FieldMetadata &elementAt(Int i) const;
	Int recordLength() const;
	FieldMetadata *belongsToFieldArray(const FieldMetadata &fmd) const;

	void dump() const;

private:
	struct s_field *fieldArray_d;
	FieldMetadata **fields_d;
	Int arrayLen_d;
};

#include <sqldb/metadata/fldarray.icc>

#endif	// FLDARRAY_H
