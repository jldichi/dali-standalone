//- Copyright Notice
// -----------------------------------------------------------------------
// (C) Copyright 1998 InterSoft S.A.  All Rights Reserved
// THIS IS UNPUBLISHED PROPRIETARY SOURCE CODE OF InterSoft S.A.
// The copyright notice above does not evidence any actual or intended 
// publication of such source code.
//
// $Id: intfld.h,v 1.8 2002/03/20 17:11:28 hardaiz Exp $
// -----------------------------------------------------------------------

#ifndef	INTFIELD_H
#define	INTFIELD_H

#include <sqldb/fwork/field.h>

/**
 * This class is used to represent SQL integer fields.
 * 
 * @version
 * @author Eduardo Chiocconi
 * @see Field
 */
class IntegerField : public Field {

public:
	IntegerField(const String &name, const ConstraintList &cl, Int dim = 0);
	~IntegerField();

	// Get Information methods.
	inline Int getTypeCode() const			{ return INTEGER_TYPE_CODE; }
	inline bool hasLength() const			{ return false; }
	inline Int length() const				{ return ERR; }
	inline bool hasDecimals() const			{ return false; }
	Object *clone() const;

	// Debug
	void dump() const;

private:
	IntegerField(const IntegerField &c);
	IntegerField &operator=(const IntegerField &c);

};

#endif	// INTFIELD_H
