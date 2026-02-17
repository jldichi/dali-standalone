//- Copyright Notice
// -----------------------------------------------------------------------
// (C) Copyright 1998 InterSoft S.A.  All Rights Reserved
// THIS IS UNPUBLISHED PROPRIETARY SOURCE CODE OF InterSoft S.A.
// The copyright notice above does not evidence any actual or intended 
// publication of such source code.
//
// $Id: cslotarr.h,v 1.2 2002/03/19 21:26:43 hardaiz Exp $
// -----------------------------------------------------------------------

#ifndef	CSLOTARRAY_H
#define	CSLOTARRAY_H

#include <ideafix.h>
#include <sqldb/metadata/indexmd.h>
#include <sqldb/fwork/indexval.h>
#include <sqldb/fwork/curimpl.h>
#include <ifound.h>
#include <sqldb/fwork/condslot.h>

#define	CSLOTARRAY_RESIZE_FACTOR	4

/**
 * This class encapsulates all the conditions, that were set to a cursor
 * bound
 *
 * @version
 * @author	Edgardo König
 * @see
 */
class ConditionSlotArray {

public:
	ConditionSlotArray(IndexMetadata *indMD, CursorImpl *cursorImpl);
	~ConditionSlotArray();
	
	inline	Int	nFields() const;
	inline	CursorImpl	*cursorImpl() const;

	Int findUnusedSlot();
	void analyzeFrom(IndexValue *indexVal);
	void analyzeTo(IndexValue *indexVal);

	ConditionSlot *elementAt(Int i);
		
	// Debug
	void	dump()	const;
	
private:

	// Method in charge of resizing the internal array
	Int				resize();

	// Information about the index structure.
	IndexMetadata	*indexMD_d;

	// Implementation of the cursor
	CursorImpl		*cursorImpl_d;

	// Array of Conditions
	ConditionSlot	**cslot_d;

	// Length of the array
	Int				arrayLen_d;

	// Number of slots already occupied
	Int				nFields_d;
};

#include <sqldb/fwork/cslotarr.icc>
#endif	// CSLOTARRAY_H
