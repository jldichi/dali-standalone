//- Copyright Notice
// -----------------------------------------------------------------------
// (C) Copyright 1998 InterSoft S.A.  All Rights Reserved
// THIS IS UNPUBLISHED PROPRIETARY SOURCE CODE OF InterSoft S.A.
// The copyright notice above does not evidence any actual or intended 
// publication of such source code.
//
// $Id: fieldlist.h,v 1.5 2002/03/20 15:12:26 hardaiz Exp $
// -----------------------------------------------------------------------

#ifndef FIELDLIST_H
#define	FIELDLIST_H

#include <sqldb/fwork/ifield.h>
#include <sqldb/fwork/objlist.h>

/**
 * This class is used to store field objects in a list.
 * 
 * @version
 * @author Eduardo Chiocconi
 *
 * @see Field
 * @see ObjectList
 */
class FieldList : public ObjectList {

public:
	// Constructor & Destructor.
	FieldList();
	~FieldList();

 	// Set methods.
 	inline void add(const Field *c);
	// Get Information methods.
	inline const Field &getField(Int i) const;

private:
	FieldList(const FieldList &cl);
	FieldList &operator=(const FieldList &cl);
};

#include <sqldb/fwork/fieldlist.icc>

#endif // FIELDLIST_H
