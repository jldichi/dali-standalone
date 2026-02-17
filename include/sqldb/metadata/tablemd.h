//- Copyright Notice
// -----------------------------------------------------------------------
// (C) Copyright 1998 InterSoft S.A.  All Rights Reserved
// THIS IS UNPUBLISHED PROPRIETARY SOURCE CODE OF InterSoft S.A.
// The copyright notice above does not evidence any actual or intended 
// publication of such source code.
//
// $Id: tablemd.h,v 1.26 2002/03/19 21:24:13 hardaiz Exp $
// -----------------------------------------------------------------------

#ifndef TABLEMD_H
#define	TABLEMD_H

#include <ideafix.h>
#include <ideafix/priv/dbdefs.h>
#include <cfix.h>

#include <sqldb/metadata/mdmisc.h>
#include <sqldb/metadata/fldarray.h>
#include <sqldb/metadata/privmd.h>
#include <sqldb/metadata/aliasdic.h>

// Oracle limits the amount of fields in a table.
#define MAX_FIELD_AMOUNT  253

class CursorArray;
class IndexArray;
class SQLFWSchema;                 
class SQLFWIndex;
class CursorMetadata;

/**
 * This class is used to represent table metadata. It encapsulates s_table
 * CFIX structure.
 * Many objects will share the same TableMetadata (ie; BufferedTables,
 * SQLFWTables, etc.), but only a BufferedTable (the first that uses the
 * TableMetadata) will be responsible for its deletion.
 * 
 * @version
 * @author Eduardo Chiocconi
 *
 * @see FieldArray
 * @see IndexArray
 * @see CursorArray
 * @see TablePrivilegesMetatada
 */
class TableMetadata {

public:
	// This table metadata is used for hardwired tablemetadata.
	TableMetadata(const SQLFWSchema *sch);
	TableMetadata(const char *name, const char *descr, const SQLFWSchema *sch);
	~TableMetadata();

	// Set table dictionary methods.
	void setTabName(const char *name);
	void setTabPrivileges(const TablePrivilegesMetadata *tpm);
	inline void setTabNFlds(UChar nFlds);
	inline void setTabNKeys(UChar nKeys);
	inline void setPartialLen(Int len);
	inline void setTabSize(Int size);
	void setTabDescr(const char *decr);
	inline void setTabFlags(tab_flags tflags);
	inline void setSchemaOfTable(struct s_schema *sc);
	inline void setTabRecordLen(UShort rLen);
	inline void setActualTabPrivileges(UShort privs);
	inline void setTabNPrivileges(UShort nPrivs);
	void setTable(SQLFWTable *tab);
	inline void setSchema(const SQLFWSchema *sch);
	void setIndex(dbindex ind, SQLFWIndex *in);
	void setTabArrayOfFields(const FieldArray &fa);
	void setTabArrayOfIndices(const IndexArray &ia);
	void setTabArrayOfCursors(const CursorArray &ca);
	inline void incReference();
	inline void decReference();
	inline bool isReferenced() const;
	inline Int referenceCount() const;
	inline void incNTabFlds();
	inline void incNIndices();
	void clearTable();
	bool store(enum StoreMode mode);
	inline void setTableHasAVectorField();

	// Get methods.
	inline struct s_table *tableRep() const;
	inline char *tableName() const;
	inline char *tableDescr() const;
	inline tab_flags tableFlags() const;
	inline UChar nFields() const;
	inline UChar nKeys() const;
	inline UShort recordLen() const;
	inline Int tableSize() const;
	inline UShort partialRecordLen() const;
	inline char *permissionTable() const;
	inline UShort nPermissions() const;
	inline struct s_schema *schemaOfTable() const;
	inline const FieldArray &fieldArray() const;
	inline struct s_field *fieldArrayRep() const;
	inline const IndexArray &indexArray() const;
	inline struct s_index *indexArrayRep() const;
	inline const CursorArray &cursorArray() const;
	inline struct s_dbcur *cursorArrayRep() const;
	inline UChar nAllocFields() const;
	inline UChar nAllocKeys() const;
	inline UChar nAllocCursors() const;
	inline UShort userPrivileges() const;
	inline UShort relDepth() const;	
	inline SQLFWTable *getTable() const;
	inline SQLFWSchema *getSchema() const;
	inline SQLFWTable *getAliasTable(dbtable tab) const;
	SQLFWIndex *getIndex(dbindex ind) const;
	inline FieldMetadata *getField(dbfield fld) const;
	FieldMetadata *getFieldByName(const String &fldName) const;
	CursorMetadata *getCursor(dbcursor cur) const;
	const SQLFWIndex &getPrimaryKey() const;
	inline bool hasBeenAttached() const;
	inline bool hasAliasBeenAttached(dbtable tab) const;
	bool getNewAlias(dbtable alias);
	inline AliasDictionary *getAliasDictionary() const;
	inline bool isTemporary() const;
	bool canBeRemoved() const;
	inline bool hasAVectorField() const;

	void setVectorTable(const SQLFWTable &table);
	inline bool hasUniqueVectorTable() const;
	inline void setHasUniqueVectorTable();
	inline SQLFWTable &getVectorTable() const;
	void setVectorAliasTable(dbtable tab, const SQLFWTable &table);
	SQLFWTable &getVectorAliasTable(dbtable tab) const;
	void removeAliasFromAliasDictionary(dbtable tab) const;

	// debug
	void dump() const;

private:
	Int references_d;	// number of objects that are referencing this metadata
	struct s_table *tabRep_d;
	FieldArray *fieldArray_d;
	IndexArray *indexArray_d;
	CursorArray *cursorArray_d;
	TablePrivilegesMetadata *permArray_d;
	SQLFWSchema *schema_d;
	Int partialLen_d;
	bool hasAVectorField_d;
	SQLFWTable *vecTable_d;
	bool hasUniqueVectorTable_d;
	AliasDictionary *ad_d;

	inline AliasDictionary *aliasDictionary() const;

	// to initialize a s_table struct
	void initializeSTable(struct s_table* st);
	String dumpTablePermissions() const;

	// inline private seters.
	inline void setTabAllocNFlds(UChar nFlds);
	inline void setTabAllocNKeys(UChar nKeys);

	// inline private getters.
	inline void initAliasDictionary();
};

#include <sqldb/metadata/tablemd.icc>

#endif	// TABLEMD_H
