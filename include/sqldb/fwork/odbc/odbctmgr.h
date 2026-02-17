//- Copyright Notice
// -----------------------------------------------------------------------
// (C) Copyright 1998 InterSoft S.A.  All Rights Reserved
// THIS IS UNPUBLISHED PROPRIETARY SOURCE CODE OF InterSoft S.A.
// The copyright notice above does not evidence any actual or intended 
// publication of such source code.
//
// $Id: odbctmgr.h,v 1.15 1999/04/29 17:28:24 eduardoc Exp $
// -----------------------------------------------------------------------

#ifndef ODBCTMGRIMPL_H
#define ODBCTMGRIMPL_H

#include <sqldb/fwork/tmgrimpl.h>


/** This class is used to implement schema operations using ODBC.
 * 
 * @version
 * @author Pablo Rubinstein
 *
 * @see SQLFWSchema
 * @see TableDDLManagerImpl
 */
class ODBCTableDDLManagerImpl : public TableDDLManagerImpl {

public:	
	ODBCTableDDLManagerImpl(const SchemaMetadata &tmd);
	~ODBCTableDDLManagerImpl();

	// DDL Table methods.
	TableDDLOp::Status createTable(const String &tableName,
									const FieldList &columns,
									const ConstraintList &constraints);
	TableDDLOp::Status createTemporaryTable(const String &tableName,
									const FieldList &columns,
									const ConstraintList &constraints);
	TableDDLOp::Status dropTable(const String &tableName);
	TableDDLOp::Status addTableField(const String &tableName,
									const Field &field);
	TableDDLOp::Status addTableConstraint(const String &tableName,
									const TableConstraint &tc);
	TableDDLOp::Status dropTableConstraint(const String &tableName, 
									const String &constraintName);
	TableDDLOp::Status addFieldConstraint(const String &tableName,
									const String &fieldName,
									const FieldMetadata *fmd, 
									const FieldConstraint &tc);
	TableDDLOp::Status createPrimaryKey(
									const String &tableName,
									const String &indexName,
									const IndexFieldList &ifl);
	TableDDLOp::Status createIndex(const String &tableName,
									const String &indexName,
									const IndexFieldList &ifl,
									IndexConstraint::Constraint ic);
	TableDDLOp::Status createTemporaryIndex(
									const String &tableName,
									const String &indexName,
									const IndexFieldList &ifl,
									IndexConstraint::Constraint ic);
	TableDDLOp::Status dropIndex(const String &indexName);
	TableDDLOp::Status addIndexField(const String &tableName,
									const String &indexName,
									const IndexField &indf);
	TableDDLOp::Status completeIndex(const String &tableName,
									const String &indexName);
	TableDDLOp::Status buildIndex(const String &tableName,
									const String &indexName);
	TableDDLOp::Status lockTable(const String &tableName,
									enum LockMode lmode);
	TableDDLOp::Status freeTable(const String &tableName);
	TableDDLOp::Status grant(const String &tableName,
									const PermissionList &pl);
	TableDDLOp::Status revoke(const String &tableName,
									const PermissionList &pl);

	// Debug
	void dump() const;

private:
	bool grantAllPermissionsOverTableToUserCreator(
									const String &schemaName,
									const String &tableName);

	/**
	 * These two methods are private to the class and should not be made public.
	 * They are just hidden to control object creation.
	 */
	ODBCTableDDLManagerImpl(const ODBCTableDDLManagerImpl &);
	ODBCTableDDLManagerImpl &operator=(const ODBCTableDDLManagerImpl &);
};

#endif //ODBCTMGRIMPL_H
