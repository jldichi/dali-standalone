//- Copyright Notice
// -----------------------------------------------------------------------
// (C) Copyright 1998 InterSoft S.A.  All Rights Reserved
// THIS IS UNPUBLISHED PROPRIETARY SOURCE CODE OF InterSoft S.A.
// The copyright notice above does not evidence any actual or intended 
// publication of such source code.
//
// $Id: boolfld.h,v 1.6 2002/03/20 15:12:59 hardaiz Exp $
// -----------------------------------------------------------------------

#ifndef	BOOLFIELD_H
#define	BOOLFIELD_H

#include <sqldb/fwork/field.h>
#include <ideafix/priv/dbdefs.h>

/**
 * This class is used to represent SQL bool fields.
 * 
 * @version
 * @author Eduardo Chiocconi
 * @see Field
 */
class BoolField : public Field {

public:
	BoolField(const String &name, const ConstraintList &cl, Int dim = 0);
	~BoolField();

	// Get Information methods.
	inline Int getTypeCode() const			{ return BOOL_TYPE_CODE; }
	inline bool hasLength() const			{ return false; }
	inline bool hasDecimals() const			{ return false; }
	Object *clone() const;

	// Debug
	void dump() const;

private:
	Int len_d;

	BoolField(const BoolField &b);
	BoolField &operator=(const BoolField &b);
};

#endif	// BITFIELD_H
