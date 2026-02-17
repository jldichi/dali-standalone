//- Copyright Notice
// -----------------------------------------------------------------------
// (C) Copyright 1998 InterSoft S.A.  All Rights Reserved
// THIS IS UNPUBLISHED PROPRIETARY SOURCE CODE OF InterSoft S.A.
// The copyright notice above does not evidence any actual or intended 
// publication of such source code.
//
// $Id: condbuil.h,v 1.2 2002/03/19 21:12:40 hardaiz Exp $
// -----------------------------------------------------------------------

#ifndef	CONDBUIL_H
#define	CONDBUIL_H

#include <ideafix.h>
#include <ifound.h>

class IndexMetadata;
class CursorImpl;
class ConditionSlotArray;
class IndexValue;

/**
 * This class is used to build the where-condition that will be used to
 * defined the rows that must be retrieved by a cursor
 *
 * @version
 * @author Edgardo König
 * @see ConditionSlotArray
 */
class ConditionBuilder {

public:
	ConditionBuilder(IndexMetadata *indexMD, CursorImpl *cursorImpl);
	~ConditionBuilder();
	
	void	setFrom(IndexValue	*indexVal);
	void	setTo(IndexValue	*indexVal);
	String	getCondition();

	// Debug
	void	dump() const;
	
private:
	// Arrays of ConditionSlots that store the information about the values
	// set to the cursor bounds
	ConditionSlotArray *from_d;
	ConditionSlotArray *to_d;

	// Information about the index (structure, fields, etc)
	IndexMetadata	*indexMD_d;

	// Implementation of the cursor
	CursorImpl		*cursorImpl_d;
};

#endif	// CONDBUIL_H
