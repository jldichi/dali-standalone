//- Copyright Notice
// -----------------------------------------------------------------------
// (C) Copyright 1998 InterSoft S.A.  All Rights Reserved
// THIS IS UNPUBLISHED PROPRIETARY SOURCE CODE OF InterSoft S.A.
// The copyright notice above does not evidence any actual or intended 
// publication of such source code.
//
// $Id: numfld.h,v 1.8 2002/03/20 17:11:36 hardaiz Exp $
// -----------------------------------------------------------------------

#ifndef	NUMFIELD_H
#define	NUMFIELD_H

#include <ideafix.h>
#include <ideafix/priv/dbdefs.h>
#include <ifound/str.h>
#include <sqldb/fwork/field.h>

/**
 * This class is used to represent SQL numeric fields.
 * 
 * @version
 * @author Eduardo Chiocconi
 * @see Field
 */
class NumericField : public Field {

public:
	NumericField(const String &name, Int len, Int ndec, const ConstraintList &cl, Int dim=0);
	~NumericField();

	// Get Information methods.
	inline Int getTypeCode() const			{ return NUM_TYPE_CODE; }
	inline bool hasLength() const			{ return true; }
	inline bool hasDecimals() const			{ return true; }
	inline Int length() const				{ return len_d; }
	inline Int nDecs() const				{ return ndecs_d; }
	Object *clone() const;

	// Debug
	void dump() const;

private:
	Int len_d;
	Int ndecs_d;

	NumericField(const NumericField &c);
	NumericField &operator=(const NumericField &c);

};

#endif	// NUMFIELD_H
