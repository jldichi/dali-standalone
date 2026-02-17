//- Copyright Notice
// -----------------------------------------------------------------------
// (C) Copyright 1998 InterSoft S.A.  All Rights Reserved
// THIS IS UNPUBLISHED PROPRIETARY SOURCE CODE OF InterSoft S.A.
// The copyright notice above does not evidence any actual or intended 
// publication of such source code.
//
// $Id: ifield.h,v 1.6 2002/03/19 21:10:45 hardaiz Exp $
// -----------------------------------------------------------------------

#ifndef IFIELD_H
#define IFIELD_H

#include <sqldb/fwork/object.h>
#include <sqldb/fwork/field.h>

class IndexField : public Object {

public:
	IndexField(const Field &tabfld, bool asc);
	~IndexField();

	// Set methods.
	void setAscending();
	void setDescending();

	// Get Information methods.
	bool isAscending() const;
	const Field &fieldInTable() const;
	inline String name() const;

	String convertToString() const;
	Object *clone() const;

	void dump() const;

private:
	Field *tfld_d;
	bool ascending_d;
	
	IndexField(const IndexField &);
	IndexField &operator=(const IndexField &);
};

#include <sqldb/fwork/ifield.icc>

#endif  // IFIELD_H
