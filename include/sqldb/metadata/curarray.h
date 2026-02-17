//- Copyright Notice
// -----------------------------------------------------------------------
// (C) Copyright 1998 InterSoft S.A.  All Rights Reserved
// THIS IS UNPUBLISHED PROPRIETARY SOURCE CODE OF InterSoft S.A.
// The copyright notice above does not evidence any actual or intended 
// publication of such source code.
//
// $Id: curarray.h,v 1.9 2002/03/19 22:27:08 hardaiz Exp $
// -----------------------------------------------------------------------

#ifndef CURARRAY_H
#define	CURARRAY_H

#include <ideafix.h>
#include <ideafix/priv/dbdefs.h>
#include <sqldb/metadata/cursormd.h>
#include <sqldb/metadata/tablemd.h>

class SQLFWCursor;
class SQLFWSchema;

/**
 * This class is used to encapsulate an array of s_dbcur structures. As
 * a consecuence, it encapsulates the array of cursor opened over a table.
 *
 * @version
 * @author Eduardo Chiocconi.
 * @see
 */
class CursorArray {

public:
	CursorArray(const TableMetadata &tabMD);
	~CursorArray();

	// Set methods.
	dbcursor addCursor(const SQLFWCursor &c, Int lmode);
	bool delCursor(dbcursor cursorId);
	inline Int occupiedSlots() const;

	// Get methods.
	inline struct s_dbcur *cursorArrayRep() const;
	inline Int length() const;
	CursorMetadata *elementAt(Int i) const;

	void dump() const;

private:
	TableMetadata &tabMD_d;
	struct s_dbcur *cursorArray_d;
	CursorMetadata **cursors_d;
	Int arrayLen_d;
	Int occupiedSlots_d;

	inline const TableMetadata &tableMetadata() const;
};

#include <sqldb/metadata/curarray.icc>

#endif	// CURARRAY_H
