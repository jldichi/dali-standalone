//- Copyright Notice
// -----------------------------------------------------------------------
// (C) Copyright 1998 InterSoft S.A.  All Rights Reserved
// THIS IS UNPUBLISHED PROPRIETARY SOURCE CODE OF InterSoft S.A.
// The copyright notice above does not evidence any actual or intended 
// publication of such source code.
//
// $Id: timefld.h,v 1.6 2002/03/20 17:11:45 hardaiz Exp $
// -----------------------------------------------------------------------

#ifndef	TIMEFIELD_H
#define	TIMEFIELD_H

#include <sqldb/fwork/field.h>

/**
 * This class is used to represent SQL time fields.
 * 
 * @version
 * @author Eduardo Chiocconi
 * @see Field
 */
class TimeField : public Field {

public:
	TimeField(const String &name, const ConstraintList &cl, Int dim = 0);
	~TimeField();

	// Get Information methods.
	inline Int getTypeCode() const			{ return TIME_TYPE_CODE; }
	inline bool hasLength() const			{ return false; }
	inline Int length() const				{ return ERR; }
	inline bool hasDecimals() const			{ return false; }
	Object *clone() const;

	// Debug
	void dump() const;
                                                                       
private:

	TimeField(const TimeField &c);
	TimeField &operator=(const TimeField &c);

};

#endif	// TIMEFIELD_H
