//- Copyright Notice
// -----------------------------------------------------------------------
// (C) Copyright 1998 InterSoft S.A.  All Rights Reserved
// THIS IS UNPUBLISHED PROPRIETARY SOURCE CODE OF InterSoft S.A.
// The copyright notice above does not evidence any actual or intended 
// publication of such source code.
//
// $Id: aliasdic.h,v 1.3 2002/03/19 21:23:18 hardaiz Exp $
// -----------------------------------------------------------------------

#ifndef ALIASDIC_H
#define	ALIASDIC_H

#include <ideafix.h>

class SQLFWTable;

/**
 * This class is used to encapsulate a pair (dbtable, SQLFWTable *)
 *
 * @version	
 * @author	Pablo Rubinstein
 *
 * @see	SQLFWTable
 */

class Alias { 

public:
	Alias(dbtable tabid, SQLFWTable *table);
	~Alias();
	dbtable getTableId() const;
	SQLFWTable *getTable() const;

	// Debug methods.
	void dump() const;

private:
	dbtable id_d;
	SQLFWTable *table_d;

	Alias(const Alias &alias);
	Alias &operator=(const Alias &alias);
};

class gtype(PtrArray, Alias);

/**
 * This class is used to manage table aliases. It is a dictionary that relates
 * table identifications with SQLFWTable objects.
 * 
 * @version
 * @author Pablo Rubinstein
 *
 * @see	TableMetadata
 * @see	SQLFWTable
 */

class AliasDictionary {

public:
	AliasDictionary();
	~AliasDictionary();
	SQLFWTable *getTable(dbtable tabid) const;
	bool addAlias(dbtable tabid, SQLFWTable *table);
	bool delAlias(dbtable tabid);
	Int count() const;

	// Debug methods.
	void dump() const;

private:
	gtype(PtrArray, Alias) *list_d;

	Int find(dbtable tabid) const;

	AliasDictionary(const AliasDictionary &am);
	AliasDictionary &operator=(const AliasDictionary &am);
};

#endif	// ALIASDIC_H
