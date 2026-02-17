//- Copyright Notice
// -----------------------------------------------------------------------
// (C) Copyright 1998 InterSoft S.A.  All Rights Reserved
// THIS IS UNPUBLISHED PROPRIETARY SOURCE CODE OF InterSoft S.A.
// The copyright notice above does not evidence any actual or intended 
// publication of such source code.
//
// $Id: sqldb.h,v 1.41 2007/04/20 21:28:22 hardaiz Exp $
// -----------------------------------------------------------------------

#ifndef SQLDB_H
#define SQLDB_H

#include <ideafix.h>

class SchemaMetadata;
class TableMetadata;
class CursorMetadata;
class IndexMetadata;
class FieldMetadata;
class FieldArray;
class SQLFWSchema;
class SQLFWTable;
class SQLFWIndex;
class SQLFWCursor;
class BufferedTable;
class TableArray;
class SQLFWConnection;
class MetadataSchemaConnections;

#include <sqldb/metadata/mdmisc.h>

/**
 * This class encapsulates all metadata.
 */
class SQLDB {

public:
	// Methods to access objects that perform actions. These objects are
	// accessed through the metadata.
	static SQLFWSchema *getSchema(schema sc);
	static SQLFWTable &getTable(schema sc, dbtable tab);
	static SQLFWIndex *getIndex(schema sc, dbindex ind);
	static const BufferedTable &getBufferedTable(schema sc, dbtable tab);
	static schema getCurrentSchema();

	// Get methods for metadata.
	static bool isSchemaOpened(schema sc);
	static const SchemaMetadata &getSchemaMetadata(schema sc);
	static const TableMetadata &getTableMetadata(schema sc, dbtable tab);
	static const IndexMetadata &getIndexMetadata(schema sc, dbtable tab, dbindex ind);
	static const FieldMetadata &getFieldMetadata(schema sc, dbtable tab, dbfield fld);
	static const CursorMetadata &getCursorMetadata(schema sc, dbtable tab, dbcursor cur);
	static bool lockMetadataSchema();
	static bool freeMetadataSchema();

	static void loadMetadataSchema(const SQLFWConnection &conn);
	static bool disposeMetadataSchema();
	static SQLFWSchema *getMetadataSchema();
	static SchemaMetadata *getMetadataSchemaMetadata();
	static SQLFWConnection *getMetadataSchemaConnection();
	static bool createMetadataConnection();
	static const String &getTableName(schema sc, dbtable tab);

	// Methods that obtain descriptors from names
	static schema getSchemaId(const char *schemaName);
	static dbtable getTableId(schema sc, const char *tableName);
	static dbindex getIndexId(schema sc, dbtable tab, const String &indexName);

	// Methods that test existence of objects.
	static bool schemaMetadataExists(schema sc);
	static bool tableMetadataExists(schema sc, dbtable tab);
	static bool tableExists(schema sc, dbtable tab);
	static bool indexMetadataExists(schema sc, dbtable tab, dbindex ind);
	static bool fieldMetadataExists(schema sc, dbtable tab, dbfield fld);
	static bool cursorMetadataExists(schema sc, dbtable tab, dbcursor cur);

	// Set methods for metadata.
	static schema attachNewSchema(const SQLFWSchema &sc, const char *name, 
					UShort mode, UShort owner, const char *descr, Int nTabs);
	static void setMetadataSchema(SQLFWSchema *schema);
	static schema setCurrentSchema(schema newcurr);

	// Remove methods.
	static bool removeSchemaMetadata(schema sc);
	static bool closeAllSchemas();
	static void deleteCursorsFromTable(TableMetadata *tmd, dbindex ind);

	// Methods that hide the existence of OpenedSchemas
	static Int slotsOccupied();
	static void freeUnusedSlots();
	static const char *getSchemas();
	static schema findNextSchema(schema sch);
	static schema findSchema(const char *name);

	// Schema DDL methods
	static bool createSchema(const String &schemaName, const String &descr);
	static Int dropSchema(schema sc);
	static int lockSchema(schema sc, int lockMode);
	static bool changeSchemaOwner(const String &schemaName, const String &newOwner);

	// Table DDL methods
	static Int createTable(dbtable tab, TableMetadata *tmd, SQLFWSchema *sc);
	static Int dropTable(SQLFWSchema *sc, const String &tabName, bool isTemp);

	// Index DDL methods
	static Int createIndex(const SQLFWIndex &index, SQLFWSchema *sc);
	static Int checkForVectorFieldsInTable(const SQLFWSchema &sc, const IndexMetadata &imd);
	static TableMetadata *createTableForVectorField(const SQLFWSchema &sc, const IndexMetadata &imd, const FieldMetadata &fmd);

	static TableMetadata *createTableForVectorFields(const SQLFWSchema &sc, const IndexMetadata &imd, const FieldArray *fldArray);

	static Int completeIndex(const SQLFWIndex &index, const TableMetadata &tmd, SQLFWSchema *sc);
	static Int dropIndex(const SQLFWIndex &index);
	static bool indexExists(const SQLFWIndex *ind);

	// Miscelanous.
	static bool dbmsTypeHasBeenSet();
	static void setDbmsType(enum DbmsType dbmsType);
	static enum DbmsType getDbmsType();
	static String getDbmsSchemaSep();

	// Debug
	static void dumpCFIXBuffer(schema sc, dbtable tab);
	static void dumpFromCursorCFIXBuffer(schema sc, dbcursor cur);
	static void dumpToCursorCFIXBuffer(schema sc, dbcursor cur);

	static String getClassErrorDescr();
private:

	static SQLFWSchema *metadata_d;	// Metadata for 'metadata'.sc
	static enum DbmsType dbmsType_d;
	static String errorDescr_sd;

	static dbtable createCompleteIndexTemporaryTable(const String &tabName, const TableMetadata &tmd);
	static bool createCompleteIndexPrimaryKey(const String &tabName, const TableMetadata &tmd, dbtable tab);
	static bool createCompleteIndexIndex(const SQLFWIndex &index, const TableMetadata &tmd, dbtable tab);
};

#endif  // SQLDB_H
