//- Copyright Notice
// -----------------------------------------------------------------------
// (C) Copyright 1998 InterSoft S.A.  All Rights Reserved
// THIS IS UNPUBLISHED PROPRIETARY SOURCE CODE OF InterSoft S.A.
// The copyright notice above does not evidence any actual or intended 
// publication of such source code.
//
// $Id: infstbld.h,v 1.21 1999/06/09 14:26:14 eduardoc Exp $
// -----------------------------------------------------------------------

#ifndef	INFSTBLD_H
#define	INFSTBLD_H 

#include <sqldb/fwork/odbc/stmtbldr.h>

/**
 *
 * This class is used to generate SQL-92 statements for an Informix server.
 * 
 * @version
 * @author Pablo Rubinstein
 *
 * @see StatementBuilder
 */

#define INFSCHEMASEP	"@"
#define INFTABLESEP	":"
#define INFFIELDSEP	"."
#define MAX_INDEX_NAME_LENGTH	18

class InformixStatementBuilder : public StatementBuilder {
public: 
	InformixStatementBuilder();
	~InformixStatementBuilder();

	// SELECT FOR ONE RECORD ONLY. THIS_KEY mode.
	String getSelect(const TableMetadata &tmd, const IndexMetadata &imd, enum GRMode grmode, enum LockMode lmode = NO_LOCK) const;
	String getSelectIndexFields(const class IndexMetadata &, enum GRMode, 
					enum LockMode) const;
	String getSelectForUpdate(const TableMetadata &tmd, const IndexMetadata &imd, enum GRMode grmode, enum LockMode lmode = NO_LOCK) const;

	// SELECT FOR CURSORS.
	String getSelectForCursor(const TableMetadata &tmd, const IndexMetadata &imd, bool includeLower, bool includeUpper,  enum LockMode lmode = NO_LOCK) const;
	String getSelectUsingFieldNamesForCursor(const FieldNameList &fnl, const IndexMetadata &imd, bool includeLower, bool includeUpper, LockMode lmode) const;
	String getSelectForCursorForUpdate(const TableMetadata &tmd,const IndexMetadata &imd, bool includeLower, bool includeUpper, enum LockMode lmode) const;

	// UPDATE
	String getUpdate(const TableMetadata &tmd, const IndexMetadata &imd) const;
	String getUpdateKey(const TableMetadata &tmd, const IndexMetadata &imd) const;
	// INSERT
	String getInsert(const TableMetadata &tmd, const IndexMetadata &imd) const;
	// DELETE
	String getDelete(const TableMetadata &tmd, const IndexMetadata &imd) const;

	// GRANT
	String getGrant(const TableMetadata &tmd,
					const String &users = NULL_STRING) const;
	// REVOKE
	String getRevoke(const String &users, const TableMetadata &tmd) const;

	// CREATE TABLE PERSISTENCE
	String getCreateTable(const String schemaName,
					const String tableName,	const FieldList &columns,
					const ConstraintList &constraints) const;
	// CREATE TABLE TEMPORARY
	String getCreateTemporaryTable(const String schemaName,
					const String tableName,	const FieldList &columns,
					const ConstraintList &constraints) const;
	// CREATE INDEX
	String getCreateIndex(const String &schemaName, const String &tableName,
					const String &indexName,
					const IndexFieldList &indFlds,
					IndexConstraint::Constraint iflag) const;
	// CREATE TEMPORARY INDEX
	String getCreateTemporaryIndex(const String &schemaName,
					const String &tableName, const String &indexName,
					const IndexFieldList &indFlds,
					IndexConstraint::Constraint iflag) const;

	// DROP INDEX 
	String getDropIndex(const String &schemaName, const String &indexName) const;
	
	// DROP TABLE
	String getDropTable(const String &schemaName, const String &tableName) const;

	String getCreateSchema(const String &name) const;

	String getLockSchema(const String &schemaName, enum LockMode lmode) const;
	String getFreeSchema(const String &schemaName) const;
	String getLockTable(const String &schemaName, const String &name) const;

	String getLockMode( enum LockMode lmode) const;
	String getIsolationLevel ( enum IsoLevel isol) const;

private: 

	String getWhat(const TableMetadata &tmd, const String &schemaName,
					const String &tableName) const;
	String getWhatIndexFields(const IndexMetadata &imd, 
					const String &tableName) const;
	String getWhatUsingFields(const FieldNameList &fnl, 
					const String &tableName) const;
	String getWhere(const IndexMetadata &imd, const String &schemaName,
					const String &tableName, const String &cmp) const;
	String getWhereForSelect(const IndexMetadata &imd, const String &schemaName,
					const String &tableName, const String &cmp) const;
	String getWhereForCursor(const IndexMetadata &imd,
					const String &schemaname, const String &tableName,
					bool includeLower, bool includeUpper) const;
	String getOrder(const IndexMetadata &imd, const String &schemaName,
					const String &tableName, SelectDir sd) const;
	String getInsertList(const TableMetadata &tmd) const;
	String getUpdateList(const TableMetadata &imd,const String &schemaname, const String &tableName) const;
	String getLock( enum LockMode lmode) const;
	String getFieldConstraint(const String &schName, const String &tabName, const String &fldName, const FieldMetadata *fmd, const FieldConstraint &fc) const;
	String getTableConstraint(const String &schName, const String &tableName, const String &constraintName, const TableConstraint &tc) const;
	String getTemporaryTableConstraints() const;
};

#endif	INFSTBLD_H
