//- Copyright Notice
// -----------------------------------------------------------------------
// (C) Copyright 1998 InterSoft S.A.  All Rights Reserved
// THIS IS UNPUBLISHED PROPRIETARY SOURCE CODE OF InterSoft S.A.
// The copyright notice above does not evidence any actual or intended 
// publication of such source code.
//
// $Id: indarray.h,v 1.7 2002/03/19 21:23:56 hardaiz Exp $
// -----------------------------------------------------------------------

#ifndef INDARRAY_H
#define	INDARRAY_H

#include <ideafix.h>
#include <ideafix/priv/dbdefs.h>
#include <sqldb/metadata/indexmd.h>

#define	IND_RESIZE_FACTOR	1

/**
 *  This class lets handle a group of IndexMetadatas as an array.
 *
 * @version
 * @author Eduardo Chiocconi.
 * @see IndexMetadata
 */
class IndexArray {

public:
	IndexArray(Int len);
	~IndexArray();
	
	// Set methods.

	// Get methods.
	inline struct s_index *indexArrayRep() const;
	inline struct s_index *indexArrayRepAt(Int i) const;	
	inline Int length() const;
	const IndexMetadata &elementAt(Int i) const;
	const SQLFWIndex &getPrimaryKey() const;
	Int findUnusedSlot();
	bool delIndex(dbindex ind);

	void dump() const;

private:
	struct s_index *indexArray_d;
	IndexMetadata **ifields_d;
	Int arrayLen_d;

	Int resize();
};

#include <sqldb/metadata/indarray.icc>

#endif	// INDARRAY_H
