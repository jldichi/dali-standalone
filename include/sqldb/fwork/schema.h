//- Copyright Notice
// -----------------------------------------------------------------------
// (C) Copyright 1998 InterSoft S.A.  All Rights Reserved
// THIS IS UNPUBLISHED PROPRIETARY SOURCE CODE OF InterSoft S.A.
// The copyright notice above does not evidence any actual or intended 
// publication of such source code.
//
// $Id: schema.h,v 1.44 2002/03/20 15:03:39 hardaiz Exp $
// -----------------------------------------------------------------------

#ifndef SQLFWSCHEMA_H
#define SQLFWSCHEMA_H

#include <sqldb/fwork/table.h>
#include <sqldb/metadata/schemamd.h>
#include <sqldb/metadata/metasch.h>

class Permission;
class FieldArray;
class SQLFWConnection;
class TableDDLManagerImpl;
class InCheck;
class IndexMetadata;
class IndexFieldMetadata;
class SQLFWUrl;

#define	STAMPFIELDS(flags)	((flags&F_CREAT_STAMP)| \
							 (flags&F_MODIF_STAMP)| \
							 (flags&F_PCREAT_STAMP))
#define	CHECKDIGIT(flags)	((flags&F_CDIGIT) ? "D": \
							 (flags&F_CDIGIT_DASH) ? "-": \
							 (flags&F_CDIGIT_SLASH) ? "/": "")
#define	GETCHECKDIGIT(cd)	(strcmp(cd, "") == 0 ? F_OPTIONAL: \
							 (strcmp(cd, "D") == 0) ? F_CDIGIT: \
							 (strcmp(cd, "-") == 0) ? F_CDIGIT_DASH: \
							 (strcmp(cd, "/") == 0) ? F_CDIGIT_SLASH: ERR)

/** 
 * This class is used to manipule schemas. It certainly encapsulates
 * schema metadata and the manipulation of these structures.
 * The basic steps to create a schema are:
 *	- Create a connection with the database where the schema is placed.
 *	- Create the schema object with the created connection.
 *	- Read the metadata associated to the schema.
 *	- Perform Manipulation operations over the schema.
 *
 * All DML operations should be used once the schema object has been created
 * except for createSchema and dropSchema operations.
 * 
 * @version
 * @author Diego Balbi & Eduardo Chiocconi.
 * @see Connection
 * @see ConnectionImpl
 */
class SQLFWSchema {

public:	
	// Constructor & destructor.
	SQLFWSchema(const SQLFWConnection *conn, const String &schName,
				short openMode);
	~SQLFWSchema();

	// Set methods
	inline void setSchemaMetadata(SchemaMetadata* schMD);
	inline void setTableDDLManagerImpl(const TableDDLManagerImpl &tdmimpl);
	inline void invalidateConnection();

	// Get Information methods.
	Int readMetadata();
	inline schema getId() const;
	inline String owner() const;
	inline char *schemaName() const;
	inline Int nTables() const;
	inline Int openMode() const;
	inline SQLFWConnection *getConnection() const;
	static SQLFWSchema *get(schema sch);
	const SQLFWTable &getNewTable(dbtable tab);
	const TableMetadata &readTableMetadata(dbtable tab);
	String findTableName(dbtable tab) const;

	// Methods used to load and store metadata in metadata DB.
	bool storeTableMetadata(const TableMetadata &tmd, enum StoreMode mode);
	bool storeIndexMetadata(const IndexMetadata &imd, const String &tableName, enum StoreMode mode);
	bool storeFieldMetadata(const FieldMetadata &fmd, const String &tableName, enum StoreMode mode);
	bool storeFieldMetadataInCheck(const FieldMetadata &fmd, const String &tableName, enum StoreMode mode);
	bool storeIndexFieldMetadata(const IndexFieldMetadata &fmd, const IndexMetadata &imd, enum StoreMode mode);
	bool storeSchemaPermissions(const Privilege &priv, enum StoreMode mode);
	bool storeTablePermissions(const String &table, const Privilege &priv, enum StoreMode mode);
	bool incrementNumberOfTablesInSchema();
	bool incrementNumberOfIndexesInTable(const String &tabName, int hasUnique = 0);
	bool delAliasTable(dbtable tab);

	// Table DDL methods.
	SchemaOp::Status createTable(const String &tableName, const FieldList &columns, const ConstraintList &constraints, enum TableFlag tflag);
	SchemaOp::Status addTableConstraint(const String &tabName, const TableConstraint &tc);
	SchemaOp::Status addField(const String &tabName, const Field &f);
	SchemaOp::Status deleteField(const String &tabName, const Field &f);
	SchemaOp::Status modifyField(const String &tabName, const Field &f1, const Field &f2);
	SchemaOp::Status addFieldConstraint(const String &tabName, const String &fldName, const FieldMetadata *fmd, const FieldConstraint &fc);
	SchemaOp::Status dropTable(const String &tableName);
	SchemaOp::Status lockTable(const String &tabName, enum LockMode lockmode);
	SchemaOp::Status unlockTable(const String &tabName);
	SchemaOp::Status grant(const String &tabName, const PermissionList &pl);
	SchemaOp::Status revoke(const String &tabName, const PermissionList &pl);
	SchemaOp::Status delTableConstraint(const String &tabName, const Constraint &c);

	// Index DDL methods.
	SchemaOp::Status createIndex(const String &tableName, const IndexMetadata &imd, IndexConstraint::Constraint opt);
	SchemaOp::Status dropIndex(const String &tabName, const String &indexName, bool isUnique, bool tabIsTemporary);
	SchemaOp::Status addIndField(const String &tableName, const SQLFWIndex &i, const IndexField &f);
	SchemaOp::Status deleteIndField(const String &tableName, const SQLFWIndex &i, const IndexField &f);
	SchemaOp::Status modifyIndField(const String &tableName, const SQLFWIndex &i, const IndexField &f);
	bool indexExists(const String &tabName, Int indId);

	bool delSchemaFromSchemaTableMetadata();
	bool delTableFromMetadataSchema(const String &tableName);
	bool delIndexFromMetadataSchema(const String &schemaName,
									const String &tableName, Int indId);
	void delFromTableArray(const String &schName);

	// Report error methods.
	inline const String &getErrorDescr();
	inline static const String &getClassErrorDescr();

	inline SchemaMetadata *schemaMetadata() const;
	static const SchemaMetadata &checkMetadataSchema(const SQLFWConnection *conn);
	static void loadMetadataSchema(const SQLFWConnection &conn);

	inline void setMetadataConnection(SQLFWConnection *mdConn);
	inline void setConnection(SQLFWConnection *conn);
	inline SQLFWConnection *getMetadataConnection() const;
	static SQLFWConnection *createMetadataConnectionForSchema(const String &schema);
	static void removeAllMetadataConnections();
	
	// Debugging
	void dump() const;

private:
	// Members.
	TableDDLManagerImpl *tableDDLManagerImpl_d;
	SchemaMetadata *schemaMD_d;
	SQLFWConnection *conn_d;
	SQLFWConnection *mdConn_d;
	String schemaName_d;
	short openMode_d;
	static MetadataSchemaConnections msc_d;

	// Error members.
	static String errorDescr_sd;
	String errorDescr_d;

	SQLFWSchema(const SQLFWSchema &);
	SQLFWSchema &operator=(const SQLFWSchema &);

	inline void setSchemaName(const String& schName);
	inline const String &getSchemaName() const;
	inline SQLFWConnection *connection() const;
	TableDDLManagerImpl *getTableDDLManager();
	static MetadataSchemaConnections &metadataSchemaConnections();

	Int readSchema();
	Int readTables(schema sch, Int nTabs);

	// Methods to access metadata schema tables. (Hardcoded structures).
	bool storeInCheckFlds(const String &tableName, dbfield fld, const InCheck *ic, enum StoreMode mode);
	bool storeHardwireInFldsMetadata(const String &tableName, dbfield fld, const InCheck *ic, enum StoreMode mode);
	bool storeInTableFldsMetadata(const String &tableName, dbfield fld, const InCheck *ic, enum StoreMode mode);
	void loadSchemaPrivileges(SchemaMetadata *smd);
	void loadTableMembers(dbtable tabid, TableMetadata *tmd, Int nflds);
	void loadFld(Int fld, TableMetadata *tmd, SQLFWTable *tab, FieldMetadata *fmd);
	void loadInCheck(const String &tabName, FieldMetadata *fmd);
	InCheck *loadIntegrity(SQLFWTable *refTab, const String &schName, const String &tabName, const String &indName, const String &refExtent, dbfield fldid, dbindex indid);
	InCheck *loadInList(SQLFWTable *refTab);
	void loadIndices(dbtable tabid, TableMetadata *tmd, Int ninds);
	void loadIndexFlds(TableMetadata *tmd, IndexMetadata *imd, Int tab, SQLFWCursor *indAttrCursor);
	void loadTablePrivileges(TableMetadata *tmd);
	void loadTableReferences(dbtable tab, TableMetadata *tmd);
	void loadVectorFieldTables(const TableMetadata &tmd);

	static const FieldArray &getTableMetadataFields(Int nflds, Int key, FieldDescription *fldStruct);
	static void getTableMetadataIndices(IndexMetadata *imd, Int keyId, Int keyLen, IndexFieldDescription *indFldStruct);
	static SQLFWTable *getSchemasTableMetadata(SQLFWConnection *conn);
	static SQLFWTable *getTablesTableMetadata(SQLFWConnection *conn);
	static SQLFWTable *getTableMembersTableMetadata(SQLFWConnection *conn);
	static SQLFWTable *getIndicesTableMetadata(SQLFWConnection *conn);
	static SQLFWTable *getIndexAttributesTableMetadata(SQLFWConnection *conn);
	static SQLFWTable *getReferencesTableMetadata(SQLFWConnection *conn);
	static SQLFWTable *getReferencesAttributesTableMetadata(SQLFWConnection *conn);
	static SQLFWTable *getEnumsTableMetadata(SQLFWConnection *conn);
	static SQLFWTable *getEnumsAttributesTableMetadata(SQLFWConnection *conn);
	static SQLFWTable *getSchemaPermissionsTableMetadata(SQLFWConnection *conn);
	static SQLFWTable *getTablePermissionsTableMetadata(SQLFWConnection *conn);
	static SQLFWTable *getMetadataTable(SQLFWConnection *conn, const FieldArray &flds, const IndexArray& ia, const char* name, const char *tabDescr, dbtable tabid);

	static bool delFromSchemaTableMetadata(const String &schName, SQLFWConnection *mdConn);
	static bool delFromTablesTableMetadata(const String &schName, const String &tableName, SQLFWConnection *mdConn);
	static bool delFromTabMembersTableMetadata(const String &schName, const String &tableName, Int nfld, SQLFWConnection *mdConn);
	static bool delFromIndicesTableMetadata(const String &schName, const String &tableName, Int nInd, SQLFWConnection *mdConn);
	static bool delFromIndexAttributesTableMetadata(const String &schName, const String &tableName, Int index, Int indFld, SQLFWConnection *mdConn);
	static bool delFromReferencesTableMetadata(const String &schName, const String &tableName, SQLFWConnection *mdConn);
	static bool delFromReferencesAttributesTableMetadata(const String &schName, const String &tableName, SQLFWConnection *mdConn);
	static bool delFromTablePermissionsTableMetadata(const String &schName, const String &tableName, SQLFWConnection *mdConn);

	static SQLFWConnection *openMetadataConnectionUsingUrl(const SQLFWUrl &mdurl);

};

#include <sqldb/fwork/schema.icc>

#endif	// SQLFWSCHEMA_H
