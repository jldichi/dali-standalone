//- Copyright Notice
// -----------------------------------------------------------------------
// (C) Copyright 1998 InterSoft S.A.  All Rights Reserved
// THIS IS UNPUBLISHED PROPRIETARY SOURCE CODE OF InterSoft S.A.
// The copyright notice above does not evidence any actual or intended 
// publication of such source code.
//
// $Id: ifarray.h,v 1.8 2002/03/19 21:21:02 hardaiz Exp $
// -----------------------------------------------------------------------

#ifndef IFARRAY_H
#define	IFARRAY_H

#include <ideafix.h>
#include <ideafix/priv/dbdefs.h>
#include <sqldb/metadata/ifieldmd.h>

#define IFARRAY_RESIZE_FACTOR	4

/**
 * This class encapsulates the array of index fields of an index. It is an
 * encapsulation for s_index_field structure array.
 *
 * @version
 * @author Eduardo Chiocconi.
 * @see	IndexFieldMetadata
 */
class IndexFieldArray {

public:
	IndexFieldArray(Int len, const IndexMetadata *imd);
	~IndexFieldArray();

	// Get methods.
	inline struct s_index_field *indexFieldArrayRep() const;
	inline Int length() const;
	Int usedSlots() const;
	inline IndexFieldMetadata *elementAt(Int i) const;
	Int findUnusedSlot();
	inline IndexMetadata *indexMD() const;

	// Debug
	void dump() const;

private:
	struct s_index_field *indexFieldArray_d;
	IndexFieldMetadata **iffields_d;
	Int arrayLen_d;
	IndexMetadata *indexMD_d;

	Int resize();
};

#include <sqldb/metadata/ifarray.icc>

#endif //	IFARRAY_H
