//- Copyright Notice
// -----------------------------------------------------------------------
// (C) Copyright 1998 InterSoft S.A.  All Rights Reserved
// THIS IS UNPUBLISHED PROPRIETARY SOURCE CODE OF InterSoft S.A.
// The copyright notice above does not evidence any actual or intended 
// publication of such source code.
//
// $Id: condslot.h,v 1.2 2002/03/19 21:26:23 hardaiz Exp $
// -----------------------------------------------------------------------

#ifndef	CONDSLOT_H
#define	CONDSLOT_H

#include <ideafix.h>
#include <ifound.h>

/**
 * This class is used to encapsulate all the information needed to build
 * a condition, that will be used in a cursor bound
 *
 * @version
 * @author	Edgardo König
 * @see
 */
class ConditionSlot {

public:
	ConditionSlot(String fieldName, bool isNull, String value, bool hasNotNull, String notNull, String minValue, int sqltype, int type);

	// Getters
	inline	String	fieldName();
	inline	String	value();
	inline	bool	isNull();
	inline	bool	hasNotNull();
	inline	String	notNull();
	inline	String	minValue();

	// Just debuggind methods TODO: remove them
	inline	int		getSQLType();
	inline	int		getType();
	String	getTypes();
	
private:

	// Name of the field
	String	fieldName_d;

	// Value to be used with =, >, <, >=, <=. It's the string representation
	// We must ask the Implementation to give it us.
	String	value_d;

	// It indicates whether the value represents a null
	bool	isNull_d;

	// It indicates whether this field in the index has a 'not null' clause
	bool	hasNotNull_d;

	// String that has the condition to be added when the field has a 'not null'
	// clause in the index.
	String	notNull_d;

	// Some values TODO: remove them
	String	minValue_d;
	int		sqltype_d;
	int		type_d;
};

#include <sqldb/fwork/condslot.icc>

#endif	// CONDSLOT_H
