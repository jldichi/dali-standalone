//- Copyright Notice
// -----------------------------------------------------------------------
// (C) Copyright 1998 InterSoft S.A.  All Rights Reserved
// THIS IS UNPUBLISHED PROPRIETARY SOURCE CODE OF InterSoft S.A.
// The copyright notice above does not evidence any actual or intended 
// publication of such source code.
//
// $Id: objlist.h,v 1.8 2002/03/19 20:44:43 hardaiz Exp $
// -----------------------------------------------------------------------

#ifndef OBJECTLIST_H
#define	OBJECTLIST_H

#include <sqldb/fwork/object.h>
#include <ifound.h>
#include <ifound/itypes.h>

class gtype(PtrArray, Object);

/**
 * This class is used to store objects of any kind in a list.
 * 
 * @version
 * @author Eduardo Chiocconi
 *
 * @see Object
 */
class ObjectList {

public:
	// Constructor & Destructor.
	ObjectList();
	ObjectList(const ObjectList &cl);
	virtual ~ObjectList();

	// Set methods.
	void add(const Object *c);
	// Get Information methods.
	const Object &get(Int i) const;
	Int length() const;

	String convertToString(String listSep) const;

	// Debug
	void dump() const;

private:
	gtype(PtrArray, Object) &list;

	ObjectList &operator=(const ObjectList &cl);
};

#endif // OBJECTLIST_H
