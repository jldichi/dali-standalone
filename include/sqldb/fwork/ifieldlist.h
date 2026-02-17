//- Copyright Notice
// -----------------------------------------------------------------------
// (C) Copyright 1998 InterSoft S.A.  All Rights Reserved
// THIS IS UNPUBLISHED PROPRIETARY SOURCE CODE OF InterSoft S.A.
// The copyright notice above does not evidence any actual or intended 
// publication of such source code.
//
// $Id: ifieldlist.h,v 1.5 2002/03/19 21:11:11 hardaiz Exp $
// -----------------------------------------------------------------------

#ifndef IFIELDLIST_H
#define	IFIELDLIST_H

#include <sqldb/fwork/ifield.h>
#include <sqldb/fwork/objlist.h>

/**
 * This class is used to store index field objects in a list.
 * 
 * @version
 * @author Eduardo Chiocconi
 *
 * @see IndexField
 * @see ObjectList
 */
class IndexFieldList : public ObjectList {

public:
	// Constructor & Destructor.
	IndexFieldList();
	~IndexFieldList();

	// Set methods.
	inline void add(const IndexField *c);
	// Get Information methods.
	inline const IndexField &getIndexField(Int i) const;
	FieldList *toFieldList() const;

private:
	IndexFieldList(const IndexFieldList &cl);
	IndexFieldList &operator=(const IndexFieldList &cl);
};

#include <sqldb/fwork/ifieldlist.icc>

#endif // IFIELDLIST_H
