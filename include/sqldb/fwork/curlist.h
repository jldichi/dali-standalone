//- Copyright Notice
// -----------------------------------------------------------------------
// (C) Copyright 1998 InterSoft S.A.  All Rights Reserved
// THIS IS UNPUBLISHED PROPRIETARY SOURCE CODE OF InterSoft S.A.
// The copyright notice above does not evidence any actual or intended 
// publication of such source code.
//
// $Id: curlist.h,v 1.3 2002/03/20 17:15:49 hardaiz Exp $
// -----------------------------------------------------------------------

#ifndef CURSORLIST_H
#define	CURSORLIST_H

#include <ifound.h>
#include <sqldb/fwork/objlist.h>
#include <sqldb/fwork/cursor.h>

/**
 * This class is used to store cursor objects in a list.
 * 
 * @version
 * @author Eduardo Chiocconi
 *
 * @see Cursor
 * @see ObjectList
 */
class CursorList : public ObjectList {

public:
	// Constructor & Destructor.
	CursorList();
	~CursorList();

 	// Set methods.
 	inline void add(const SQLFWCursor *c);
	// Get Information methods.
	inline const SQLFWCursor &getCursor(Int i);

private:
	CursorList(const CursorList &cl);
	CursorList &operator=(const CursorList &cl);
};

#include <sqldb/fwork/curlist.icc>

#endif // CURSORLIST_H
