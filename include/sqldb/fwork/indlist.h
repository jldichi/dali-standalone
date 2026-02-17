//- Copyright Notice
// -----------------------------------------------------------------------
// (C) Copyright 1998 InterSoft S.A.  All Rights Reserved
// THIS IS UNPUBLISHED PROPRIETARY SOURCE CODE OF InterSoft S.A.
// The copyright notice above does not evidence any actual or intended 
// publication of such source code.
//
// $Id: indlist.h,v 1.3 2002/03/20 17:16:27 hardaiz Exp $
// -----------------------------------------------------------------------

#ifndef INDEXLIST_H
#define	INDEXLIST_H

#include <sqldb/fwork/ifield.h>
#include <sqldb/fwork/objlist.h>
#include <sqldb/fwork/index.h>

/**
 * This class is used to store index objects in a list.
 * 
 * @version
 * @author Eduardo Chiocconi
 *
 * @see Index
 * @see ObjectList
 */
class IndexList : public ObjectList {

public:
	// Constructor & Destructor.
	IndexList();
	~IndexList();

 	// Set methods.
 	inline void add(const SQLFWIndex *c);
	// Get Information methods.
	inline const SQLFWIndex &getIndex(Int i);

private:
	IndexList(const IndexList &cl);
	IndexList &operator=(const IndexList &cl);
};

#include <sqldb/fwork/indlist.icc>

#endif // INDEXLIST_H
