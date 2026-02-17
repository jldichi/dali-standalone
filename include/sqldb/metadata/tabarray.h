//- Copyright Notice
// -----------------------------------------------------------------------
// (C) Copyright 1998 InterSoft S.A.  All Rights Reserved
// THIS IS UNPUBLISHED PROPRIETARY SOURCE CODE OF InterSoft S.A.
// The copyright notice above does not evidence any actual or intended 
// publication of such source code.
//
// $Id: tabarray.h,v 1.13 2002/03/19 22:27:27 hardaiz Exp $
// -----------------------------------------------------------------------

#ifndef TABARRAY_H
#define	TABARRAY_H

#include <ideafix.h>
#include <ideafix/priv/dbdefs.h>

class BufferedTable;
class SQLFWSchema;

#define FIRST_USED_POSITION			1
#define TABLEARRAY_RESIZE_FACTOR	4

/**
 * This class is used to encapsulate the array of table related to a schema.
 * Once a schema is opened, then a TableArray is created. There is a one to
 * one relation between an array position and a schema table. Each one of the
 * position has a pointer to a BufferedTable. This object has the table
 * buffer and a pointer to the table metadata.
 * When creating an array, we have to provide the length of the array that
 * can be known at the time the schema is opened. Once the array has been
 * created, BufferedTables can be manipulated in the array by setting or
 * getting elements from it.
 *
 * @version
 * @author Eduardo Chiocconi
 * @see BufferedTable
 */
class TableArray {

public:
	TableArray(Int len, const SQLFWSchema &sc);
	~TableArray();

	// Set methods.
	void setElementAt(Int i, BufferedTable *bt);
	void invalidateSlot(Int slot);
	
	// Get methods.
	inline struct tab_array *tableArrayRep() const;
	inline Int length() const;
	inline const BufferedTable &elementAt(Int i) const;
	Int findTable(const char *name) const;
	bool isAnAlias(Int slot) const;

	// Reallocation methods. They are usually used when creating aliases.
	Int findNewSlot();
	// This add is for alias.
	Int addNewBufferedTable(Int i, struct s_table *tabptr);
	// This add is used when CreateTable CFIX function is used.
	Int addBufferedTable(Int i, SQLFWSchema *sc);
	bool freeSlot(Int slot);
	bool isEmptySlot(Int i) const;
	bool isBufferedTableEmpty(Int slot) const;

	// Debug
	void dump() const;

private:
	struct tab_array *tabArrayRep_d;
	BufferedTable **tabArray_d;
	Int arrayLen_d;
	schema schId_d;		// optimization: we hold the schema id to qualify the
						// BufferedTable in destruction.

	void initializeTabArray(Int from, Int to);
	Int resize();
	inline schema getSchemaId() const;

	void deleteAllAliases();
	void deleteAllTables();
	void deleteAlias(Int slot);

	bool tableHasBeenAttached(Int slot) const;
};

#include <sqldb/metadata/tabarray.icc>

#endif	// TABARRAY_H
