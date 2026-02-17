//- Copyright Notice
// -----------------------------------------------------------------------
// (C) Copyright 1998 InterSoft S.A.  All Rights Reserved
// THIS IS UNPUBLISHED PROPRIETARY SOURCE CODE OF InterSoft S.A.
// The copyright notice above does not evidence any actual or intended 
// publication of such source code.
//
// $Id: datefld.h,v 1.6 2002/03/20 15:13:10 hardaiz Exp $
// -----------------------------------------------------------------------

#ifndef	DATEFIELD_H
#define	DATEFIELD_H

#include <sqldb/fwork/field.h>

/**
 * This class is used to represent SQL date fields.
 * 
 * @version
 * @author Eduardo Chiocconi
 * @see Field
 */
class DateField : public Field {

public:
	DateField(const String &name, const ConstraintList &cl, Int dim = 0);
	~DateField();

	// Get Information methods.
	inline Int getTypeCode() const			{ return DATE_TYPE_CODE; }
	inline bool hasLength() const			{ return false; }
	inline Int length() const				{ return ERR; }
	inline bool hasDecimals() const			{ return false; }
	Object *clone() const;

	// Debug
	void dump() const;

private:

	DateField(const DateField &d);
	DateField &operator=(const DateField &d);

};

#endif	// DATEFIELD_H
