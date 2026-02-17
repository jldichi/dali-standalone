//- Copyright Notice
// -----------------------------------------------------------------------
// (C) Copyright 1998 InterSoft S.A.  All Rights Reserved
// THIS IS UNPUBLISHED PROPRIETARY SOURCE CODE OF InterSoft S.A.
// The copyright notice above does not evidence any actual or intended 
// publication of such source code.
//
// $Id: bitfld.h,v 1.10 2002/03/20 15:12:48 hardaiz Exp $
// -----------------------------------------------------------------------

#ifndef	BITFIELD_H
#define	BITFIELD_H

#include <sqldb/fwork/field.h>
#include <ideafix/priv/dbdefs.h>

/**
 * This class is used to represent SQL bit fields.
 * 
 * @version
 * @author Eduardo Chiocconi
 * @see Field
 */
class BitField : public Field {

public:
	BitField(const String &name, Int len, const ConstraintList &cl, Int dim = 0);
	~BitField();

	// Get Information methods.
	inline Int getTypeCode() const			{ return BIT_TYPE_CODE; }
	inline bool hasLength() const			{ return true; }
	inline Int length() const				{ return len_d; }
	bool hasDecimals() const				{ return false; }
	Object *clone() const;

	// Debug
	void dump() const;

private:
	Int len_d;

	BitField(const BitField &b);
	BitField &operator=(const BitField &b);
};

#endif	// BITFIELD_H
