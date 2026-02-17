//- Copyright Notice
// -----------------------------------------------------------------------
// (C) Copyright 1998 InterSoft S.A.  All Rights Reserved
// THIS IS UNPUBLISHED PROPRIETARY SOURCE CODE OF InterSoft S.A.
// The copyright notice above does not evidence any actual or intended 
// publication of such source code.
//
// $Id: fnamelist.h,v 1.3 2002/03/19 21:12:26 hardaiz Exp $
// -----------------------------------------------------------------------

#ifndef FIELDNAMELIST_H
#define	FIELDNAMELIST_H

#include <sqldb/fwork/objlist.h>
#include <sqldb/fwork/fieldname.h>

/**
 * This class is used to store field names in a list.
 * 
 * @version
 * @author Eduardo Chiocconi
 *
 * @see String
 * @see FieldName
 * @see ObjectList
 */
class FieldNameList : public ObjectList {

public:
	// Constructor & Destructor.
	FieldNameList();
	~FieldNameList();

 	// Set methods.
 	inline void add(const FieldName *c);
	// Get Information methods.
	inline const FieldName &getField(Int i) const;
	bool exists(const String &fname) const;

private:
	FieldNameList(const FieldNameList &cl);
	FieldNameList &operator=(const FieldNameList &cl);
};

#include <sqldb/fwork/fnamelist.icc>

#endif // FIELDNAMELIST_H
