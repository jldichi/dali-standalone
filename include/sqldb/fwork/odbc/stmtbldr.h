//- Copyright Notice
// -----------------------------------------------------------------------
// (C) Copyright 1998 InterSoft S.A.  All Rights Reserved
// THIS IS UNPUBLISHED PROPRIETARY SOURCE CODE OF InterSoft S.A.
// The copyright notice above does not evidence any actual or intended 
// publication of such source code.
//
// $Id: stmtbldr.h,v 1.37 1999/06/09 14:31:53 eduardoc Exp $
// -----------------------------------------------------------------------

#ifndef	STMTBUILDER_H
#define	STMTBUILDER_H 

#include <ifound.h>
#include <ifound/str.h>

#include <sqldb/metadata/sqldb.h>

#include <sqldb/fwork/status.h>
#include <sqldb/fwork/constraint.h>

class TableMetadata;
class IndexMetadata;
class SchemaMetadata;
class FieldList;
class IndexFieldList;
class ConstraintList;
class PermissionList;
class IndexFieldArray;
class FieldNameList;

/**
 *
 * This class is used to generate SQL-92 statements.
 * 
 * @version
 * @author Pablo Rubinstein
 *
 * @see SchemaMetadata
 * @see TableMetadata
 * @see IndexMetadata
 */

#define SCHEMASEP	"."
#define TABLESEP	"."
#define FIELDSEP	"."

#define OM_ASCENDING	"asc"
#define OM_DESCENDING	"desc"

enum SelectDir {
	FORWARD_DIR = 0,
	BACKWARD_DIR
};

class StatementBuilder {
public: 
	StatementBuilder();
	virtual ~StatementBuilder();

	// SELECT FOR ONE RECORD ONLY.
	// rmode can be: THIS_KEY, NEXT_KEY, PREV_KEY, (THIS_KEY|NEXT_KEY),
	// (THIS_KEY|PREV_KEY) mode.
	virtual String getSelect(const TableMetadata &tmd, 
									const IndexMetadata &imd, 
									enum GRMode grmode, 
									enum LockMode lmode = NO_LOCK) const;
	virtual String getSelectIndexFields(const IndexMetadata &imd, 
									enum GRMode grmode, 
									enum LockMode lmode = NO_LOCK) const;
	virtual String getSelectForUpdate(const TableMetadata &tmd, 
									const IndexMetadata &imd, 
									enum GRMode grmode, 
									enum LockMode lmode = NO_LOCK) const;

	// SELECT FOR CURSORS.
	virtual String getSelectForCursor(const TableMetadata &tmd, 
									const IndexMetadata &imd, 
									bool includeLower, 
									bool includeUpper, 
									enum LockMode lmode=NO_LOCK) const;

	virtual String getSelectUsingFieldNamesForCursor(
									const FieldNameList &fnl,
									const IndexMetadata &imd, 
									bool includeLower, 
									bool includeUpper,
									LockMode lmode) const;

	virtual String getSelectForCursorForUpdate(const TableMetadata &tmd,
									const IndexMetadata &imd, 
									bool includeLower, 
									bool includeUpper, 
									enum LockMode lmode) const {return NULL_STRING;};

	// UPDATE
	virtual String getUpdate(const TableMetadata &tmd, 
									const IndexMetadata &imd) const;
	virtual String getUpdateKey(const TableMetadata &tmd,
									const IndexMetadata &imd) const;
	// INSERT
	virtual String getInsert(const TableMetadata &tmd, 
									const IndexMetadata &imd) const;
	// DELETE
	virtual String getDelete(const TableMetadata &tmd,
									const IndexMetadata &imd) const;
	// SELECT COUNT(*) ...
	virtual String getSelectCount(const String &schemaName,
									const String &tabName) const;
	// GRANT
	virtual String getGrant(const String &schemaName,
									const PermissionList &perms) const;
	// REVOKE
	virtual String getRevoke(const String &schemaName, 
									const PermissionList &perms) const;
	// CREATE TABLE PERSISTENCE
	virtual String getCreateTable(const String schemaName,
									const String tableName,
									const FieldList &columns,
									const ConstraintList &constraints) const;
	// CREATE TABLE TEMPORARY
	virtual String getCreateTemporaryTable(const String schemaName,
									const String tableName,
									const FieldList &columns,
									const ConstraintList &constraints) const;
	// DROP TABLE
	virtual String getDropTable(const String &schemaName, 
									const String &tableName) const;

	// CREATE INDEX PERSISTENCE
	virtual String getCreateIndex(const String &schemaName,
									const String &tableName, 
									const String &indexName,
									const IndexFieldList &indFlds,
									IndexConstraint::Constraint iflag) const;
	// CREATE INDEX TEMPORARY
	virtual String getCreateTemporaryIndex(const String &schemaName,
									const String &tableName, 
									const String &indexName,
									const IndexFieldList &indFlds,
									IndexConstraint::Constraint iflag) const;
	// DROP INDEX
	virtual String getDropIndex(const String &schemaName,
									const String &indexName) const;
	virtual String getCreateUser(const String &username) const;
	virtual String getDropUser(const String&username) const;
	virtual String getCreateSchema(const String &schemaName) const;
	virtual String getDropSchema(const String &schemaName) const;

	virtual String getLockSchema(const String &schemaName, 
									enum LockMode lmode) const;
	virtual String getFreeSchema(const String &name) const;
	virtual String getLockTable(const String &schemaName, 
									const String &name) const;
	virtual String getFreeTable(const String &schemaName, 
									const String &name) const;
	
	virtual String getLockMode(enum LockMode lmode) const;
	virtual String getIsolationLevel(enum IsoLevel isol) const;

	// CONSTRAINT METHODS.
	virtual String getFieldConstraint(const String &schName, 
									const String &tabName, 
									const String &fldName, 
									const FieldMetadata *fmd, 
									const FieldConstraint &fc) const;
	virtual String getTableConstraint(const String &schName, 
									const String &tableName, 
									const String &constraintName, 
									const TableConstraint &tc) const;
	virtual String getTableDropConstraint(const String &schName,
									const String &tabName, 
									const String &constraintName) const;

protected: 
	virtual String getWhat(const TableMetadata &tmd, const String &tableName) const;
	virtual String getWhatIndexFields(const IndexMetadata &imd, const String &tableName) const;
	virtual String getWhatUsingFields(const FieldNameList &fnl, const String &tableName) const;

	virtual String getWhere(const IndexMetadata &imd, const String &tableName,
							const String &cmp) const;
	virtual String getWhereForSelect(const IndexMetadata &imd, const String &tableName,
							const String &cmp) const;
	virtual String getWhereForCursor(const IndexMetadata &imd, const String &tableName,
							bool includeLower, bool includeUpper) const;
	virtual String getOrder(const IndexMetadata &imd, const String &tableName, enum SelectDir sd) const;
	virtual String getInsertList(const TableMetadata &tmd) const;
	virtual String getUpdateList(const TableMetadata &tmd) const;
	virtual String getUpdateKeyList(const IndexMetadata &imd) const;
	virtual String getFieldsDefinition(const FieldList &columns) const;
	virtual String getIndexFieldsColumns(const IndexFieldList &indFlds) const;
	virtual String getIndexFieldsColumnsWithoutConstraints(const IndexFieldList &indFlds) const;
	virtual String getConstraints(const ConstraintList &constraints) const;
	virtual String getLock( enum LockMode lmode) const;
	virtual String getTemporaryTableConstraints() const;
	virtual String getNotNullFilter(const String &tabName, const IndexFieldArray &ifarray) const;
	virtual String getTableFieldList(const TableMetadata &tmd) const;
};

#endif	STMTBUILDER_H
