//- Copyright Notice
// -----------------------------------------------------------------------
// (C) Copyright 1998 InterSoft S.A.  All Rights Reserved
// THIS IS UNPUBLISHED PROPRIETARY SOURCE CODE OF InterSoft S.A.
// The copyright notice above does not evidence any actual or intended 
// publication of such source code.
//
// $Id: decfld.h,v 1.9 2002/03/20 15:13:22 hardaiz Exp $
// -----------------------------------------------------------------------

#ifndef	DECFIELD_H
#define	DECFIELD_H

#include <sqldb/fwork/field.h>

/**
 * This class is used to represent SQL decimal fields.
 * 
 * @version
 * @author Eduardo Chiocconi
 * @see Field
 */
class DecimalField : public Field {

public:
	// Dinamically created field.
	DecimalField(const String &name, const ConstraintList &cl, Int len, Int ndec, Int dim=0);
	~DecimalField();

	// Get Information methods.
	inline Int getTypeCode() const			{ return DEC_TYPE_CODE; }
	inline bool hasLength() const	   		{ return true; }
	inline Int length() const				{ return len_d; }
	inline bool hasDecimals() const			{ return true; }
	inline Int nDecs() const				{ return ndecs_d; }
	Object *clone() const;

	// Debug
	void dump() const;

private:
	Int len_d;
	Int ndecs_d;

	DecimalField(const DecimalField &c);
	DecimalField &operator=(const DecimalField &c);

};

#endif	// DECFIELD_H
