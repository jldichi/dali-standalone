//- Copyright Notice
// -----------------------------------------------------------------------
// (C) Copyright 1998 InterSoft S.A.  All Rights Reserved
// THIS IS UNPUBLISHED PROPRIETARY SOURCE CODE OF InterSoft S.A.
// The copyright notice above does not evidence any actual or intended 
// publication of such source code.
//
// $Id: conslist.h,v 1.6 2002/03/19 20:46:04 hardaiz Exp $
// -----------------------------------------------------------------------

#ifndef CONSLIST_H
#define	CONSLIST_H

#include <sqldb/fwork/constraint.h>
#include <sqldb/fwork/objlist.h>

/**
 * This class is used to store constraint objects in a list.
 * 
 * @version
 * @author Eduardo Chiocconi
 *
 * @see Constraint
 * @see ObjectList
 */
class ConstraintList : public ObjectList {

public:
	ConstraintList();
	ConstraintList(const ConstraintList &cl);
	~ConstraintList();

	// Set Information methods.
	inline void add(const Constraint *c);
	// Get Information methods.
	inline const Constraint &getConstraint(Int i) const;

	// Debug method.
	void dump() const;
	ConstraintList *clone() const;

private:
	ConstraintList &operator=(const ConstraintList &cl);
};

#include <sqldb/fwork/conslist.icc>

#endif // CONSLIST_H
