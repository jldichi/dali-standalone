//- Copyright Notice
// -----------------------------------------------------------------------
// (C) Copyright 1998 InterSoft S.A.  All Rights Reserved
// THIS IS UNPUBLISHED PROPRIETARY SOURCE CODE OF InterSoft S.A.
// The copyright notice above does not evidence any actual or intended 
// publication of such source code.
//
// $Id: fieldname.h,v 1.3 2002/03/19 21:11:45 hardaiz Exp $
// -----------------------------------------------------------------------

#ifndef	FIELDNAME_H
#define	FIELDNAME_H

#include <sqldb/fwork/object.h>
#include <ifound/str.h>

class FieldMetadata;

/**
 * This class is used to represent field names of any kind. It is specially
 * used to table fields, but can be used for any other pourpose.
 *
 * @author	Eduardo Chiocconi.
 * @see		String
 */
class FieldName : public Object {

public:
	FieldName(const String &fname, FieldMetadata *fmd);
	~FieldName();

	inline const String &fieldName() const;
	inline FieldMetadata *fieldMetadata() const;

	// Debug
	inline void dump() const;
	inline Object *clone() const;
	inline String convertToString() const;

private:
	String fName_d;
	FieldMetadata *fmd_d;

	FieldName(const FieldName &fn);
	FieldName &operator=(const FieldName &fn);
};

#include <sqldb/fwork/fieldname.icc>

#endif	// FIELDNAME_H
