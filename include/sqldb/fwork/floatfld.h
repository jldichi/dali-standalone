//- Copyright Notice
// -----------------------------------------------------------------------
// (C) Copyright 1998 InterSoft S.A.  All Rights Reserved
// THIS IS UNPUBLISHED PROPRIETARY SOURCE CODE OF InterSoft S.A.
// The copyright notice above does not evidence any actual or intended 
// publication of such source code.
//
// $Id: floatfld.h,v 1.10 2002/03/20 17:11:16 hardaiz Exp $
// -----------------------------------------------------------------------

#ifndef	FLOATFIELD_H
#define	FLOATFIELD_H

#include <sqldb/fwork/field.h>

/**
 * This class is used to represent SQL float fields.
 * 
 * @version
 * @author Eduardo Chiocconi
 * @see Field
 */
class FloatField : public Field {

public:
	FloatField(const String &name, const ConstraintList &cl, Int dim = 0);
	~FloatField();

	// Get Information methods.
	inline Int getTypeCode() const			{ return FLOAT_TYPE_CODE; }
	inline bool hasLength() const			{ return false; }
	inline bool hasDecimals() const			{ return false; }
	Object *clone() const;

	// Debug
	void dump() const;

private:

	FloatField(const FloatField &f);
	FloatField &operator=(const FloatField &f);

};

#endif	// FLOATFIELD_H
