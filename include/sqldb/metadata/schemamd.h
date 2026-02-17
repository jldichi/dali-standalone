//- Copyright Notice
// -----------------------------------------------------------------------
// (C) Copyright 1998 InterSoft S.A.  All Rights Reserved
// THIS IS UNPUBLISHED PROPRIETARY SOURCE CODE OF InterSoft S.A.
// The copyright notice above does not evidence any actual or intended 
// publication of such source code.
//
// $Id: schemamd.h,v 1.12 2002/03/19 22:28:11 hardaiz Exp $
// -----------------------------------------------------------------------

#ifndef SCHEMAMD_H
#define SCHEMAMD_H

#include <ideafix.h>
#include <ideafix/priv/dbdefs.h>
#include <sqldb/metadata/privmd.h>

class SQLFWCursor;
class CursorMetadata;
class TableArray;
class BufferedTable;
class SQLFWIndex;
class IndexMetadata;
class SQLFWTable;
class TableMetadata;                                    
class SQLFWSchema;

/**
 * This is class SchemaMetadata. It is used to encapsulate s_schema CFIX
 * structure. 
 * 
 * @version
 * @author Eduardo Chiocconi
 * @see TableArray
 */
class SchemaMetadata {

public:
	SchemaMetadata(schema id, const char *name, UShort mode, UShort owner,
					const char *descr, Int nTabs, const SQLFWSchema &sc);
	~SchemaMetadata();

	// Set methods for schema dictionary.
	inline void setSchPrivileges(SchemaPrivilegesMetadata* privileges);
	inline void setNTables(UChar tabs);
	inline void setNAllocTables(UChar nat);
	inline void setSchActualPrivileges(UShort priv);
	inline void setSchema(const SQLFWSchema *sc);
	inline void setTableArray(const TableArray *ta);
	inline void setBufferedTableName(Int pos, const String &tname);
	bool cleanUpSchemaPermissions();
	void initAllTables();
	bool attachAlias(dbtable tab) const;
	bool attachTableMD(dbtable tab) const;
	bool delAliasTable(dbtable tab);
	bool delTable(dbtable tab);
	// toDo: add methods to set union.

	// Get methods.
	inline struct s_schema *schemaRep() const;
	inline char *schemaName() const;
	inline schema schemaId() const;
	inline char *schemaDescr() const;
	inline UChar nTables() const;
	inline UShort nPermissions() const;
	inline struct dbprivg* schemaPermissions() const;
	inline const TableArray &schemaTables() const;
	inline short openMode() const;
	inline UChar nAllocTables() const;
	inline UShort schemaOwner() const;
	inline UShort actualUserPrivileges() const;
	inline SQLFWSchema *getSchema() const;
	SQLFWTable *getTable(dbtable tab) const;
	SQLFWIndex *getIndex(dbindex ind) const;
	TableMetadata *getTableMD(dbtable tab) const;
	IndexMetadata *getIndexMD(dbindex ind) const;
	FieldMetadata *getFieldMD(dbtable tab, dbfield fld) const;
	CursorMetadata *getCursorMD(dbtable tab, dbcursor cur) const;
	Int findTableSlot(const char *name) const;
	const String &getTableName(dbtable tab) const;
	bool isAnAlias(dbtable tab) const;
	bool isTableAttached(dbtable tid) const;
	// toDo: add methods to access union members.

	dbcursor createCursorMD(const SQLFWCursor &c);
	void dump() const;

private:
	schema schId_d;
	struct s_schema *sp_d;
	TableArray *tabArray_d;

	// Set private methods.
	inline void setSchemaId(schema sc);
	inline void setName(const char *name);
	inline void setDescr(const char *desc);
	inline void setOpenMode(short omode);
	inline void setSchOwner(UShort owner);
	String dumpSchemaPermissions() const;
	void initializeSSchema();
};

#include <sqldb/metadata/schemamd.icc>

#endif	// SCHEMAMD_H
