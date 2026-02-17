//- Copyright Notice
// -----------------------------------------------------------------------
// (C) Copyright 1998 InterSoft S.A.  All Rights Reserved
// THIS IS UNPUBLISHED PROPRIETARY SOURCE CODE OF InterSoft S.A.
// The copyright notice above does not evidence any actual or intended 
// publication of such source code.
//
// $Id: tmgrimpl.h,v 1.22 2002/03/20 17:17:05 hardaiz Exp $
// -----------------------------------------------------------------------

#ifndef TMGRIMPL_H
#define TMGRIMPL_H

#include <sqldb/fwork/schema.h>
#include <sqldb/fwork/connect.h>

/**
 * This class is used to provide a common interface to the different
 * implementation.
 * A TableDDLManager is responsible for executing DDL methods over the DB.
 * 
 * @version
 * @author Pablo Rubinstein & Eduardo Chiocconi
 *
 * @see SQLFWSchema
 * @see FieldList
 * @see SchemaMetadata
 * @see PermissionList
 * @see ConstraintList
 * @see ConnectionImpl
 */
class TableDDLManagerImpl {

public:	
	TableDDLManagerImpl(const SchemaMetadata &smd);
	virtual ~TableDDLManagerImpl();

	// Table DDL methods.
	/**
	 * This method creates a new Table in the Schema specified in the class
	 * constructor. The new table will have the name specified as the first
	 * parameter of the method. The table will be persistent. This means the
	 * table is not temporary.
	 * The fields of this new table will be the ones given in the list
	 * "columns" of Fields passed as the second argument.
	 * You can also specify table constraints such as PrimaryKeys and
	 * ReferentialIntegrity in the list given in the third argument.
	 *
	 * If the table already exists, then the operation will fail and
	 * ALREADY_EXISTS status will be returned.
	 * If the name of the table is not a valid one (eg: is a reserved
	 * word in the DBMS), then the operation will fail and INVALID_TABLE_NAME
	 * status will be returned.
	 * If one of the fields in the columns FieldList has an invalid name,
	 * then the operation will fail and INVALID_FIELD_NAME status should be
	 * returned.
	 * If there is at least one field repetead in the field list supplied
	 * as argument, then the operation will fail and FIELD_ALREADY_EXISTS
	 * status should be returned.
	 * If in the Constraint list, there is a constraint that cannot be
	 * applied to the table, then the operation will fail and 
	 * INVALID_CONSTRAINT status should be returned.
	 * If the user does not have permissions to create the table in the
	 * schema, then the operation will fail and NEEDS_PERMISSION status
	 * will be returned.
	 * If a server error takes place during table creation, then the 
	 * operation should fail and SERVER_GENERAL_ERROR status should be 
	 * returned.
	 * If everything goes fine, then OKEY status is returned.
	 *
	 * @param	tableName is the name of the new table.
	 * @param	columns is the list of fields that will compose the table
	 *			fields.
	 * @param	constraints is the list of constraints applied to the table.
	 *
	 * @return	operation status specified above.
	 */
	virtual TableDDLOp::Status createTable(
								const String &tableName,
								const FieldList &columns,
								const ConstraintList &constraints) = 0;

	/**
	 * This method creates a new Table in the Schema specified in the class
	 * constructor. The new table will have the name specified as the first
	 * parameter of the method. The table will be temporary. This means the
	 * table cannot be seen by other connections or sessions and after the
	 * connection or session is closed, the table can no longer be 
	 * referenced. The fields of this new table will be the ones given in the
	 * list "columns" list of Fields passed as the second argument.
	 * You can also specify table constraints such as PrimaryKeys and
	 * ReferentialIntegrity given in the third argument.
	 *
	 * If the table already exists, then the operation will fail and
	 * ALREADY_EXISTS status will be returned.
	 * If the user does not have permissions to create the table in the
	 * schema, then the operation will fail and NEEDS_PERMISSION status
	 * will be returned.
	 * If everything goes fine, then OKEY status is returned.
	 *
	 * @param	tableName is the name of the new table.
	 * @param	columns is the list of fields that will compose the table
	 *			fields.
	 * @param	constraints is the list of constraints applied to the table.
	 *
	 * @return	operation status specified above.
	 */
	virtual TableDDLOp::Status createTemporaryTable(
								const String &tableName,
								const FieldList &columns,
								const ConstraintList &constraints) = 0;
								
	/**
	 * This method drops an existing table in the schema used in the 
	 * constructor. You can only drop persistent tables.
	 *
	 * If the table does not exists, then the operation will fail and
	 * TABLE_DOESNT_EXIST status will be returned. If the table is 
	 * being accessed, the operation will fail and this method will return
	 * TABLE_IS_BEING_ACCESSED status.
	 * If the user does not have permissions to drop a table, then the
	 * operation will fail and NEEDS_PERMISSION status will be returned.
	 * If everything goes fine, then OKEY status is returned.
	 *
	 * @param	tableName is the name of the table we want to drop or
	 *			remove from DB.
	 *
	 * @return	operation status specified above.
	 */
	virtual TableDDLOp::Status dropTable(const String &tableName) = 0;

	/**
	 * This method adds a field to the table whose name is specified as the
	 * first argument of the method. The field being added is the one given
	 * as the second parameter of the method.
	 *
	 * If the table does not exists, then the operation will fail and
	 * TABLE_DOESNT_EXIST status will be returned.
	 * If the field being passed as argument has been already defined in the
	 * table, then the operation will fail and FIELD_ALREADY_EXISTS status
	 * will be returned.
	 * If the user does not have permissions to add the field in the table
	 * specified by the first argument, then the operation will fail and
	 * NEEDS_PERMISSION status will be returned.
	 * If everything goes fine, then OKEY status is returned.
	 *
	 * @param	tableName is the name of the table to which we want to add the
	 *			field.
	 * @param	field is the field we want to add.
	 *
	 * @return	operation status specified above.
	 */
	virtual TableDDLOp::Status addTableField(
								const String &tableName, 
								const Field &field) = 0;

	/**
	 * This method adds a TableConstraint to the table being specified as
	 * the first argument of the method. The TableConstraint being added is
	 * the one specified as the second argument.
	 *
	 * If the table does not exist, then the operation will fail and
	 * TABLE_DOESNT_EXIST status will be returned.
	 * If the user does not have permissions to add the table constraint, then
	 * the operation will fail and NEEDS_PERMISSION status will be
	 * returned.
	 * If the constraint already exists in the table, then the operation will
	 * fail and CONSTRAINT_ALREADY_EXISTS status will be returned.
	 * If everything goes fine, then OKEY status is returned.
	 * If the referenced and reference fields have different field types, the 
	 * operation should fail and REFERENCED_FIELD_HAS_DIFFERENT_TYPE status
	 * should be returned.
	 *
	 * @param	tableName is the name of the table to which we want to add the
	 *			TableConstraint.
	 * @param	tc is the TableConstraint we want to add.
	 *
	 * @return	operation status specified above.
	 */
	virtual TableDDLOp::Status addTableConstraint(
								const String &tableName, 
								const TableConstraint &tc) = 0;

	/**
	 * This method drops a TableConstraint from the table being specified 
	 * as the first argument of the method. The TableConstraint being 
	 * dropped is the one specified as the second argument.
	 *
	 * If the table does not exist, then the operation will fail and
	 * TABLE_DOESNT_EXIST status will be returned.
	 * If the user does not have permissions to drop the table constraint, 
	 * then the operation will fail and NEEDS_PERMISSION status will be
	 * returned.
	 * If the constraint does not exist in the table, then the operation 
	 * will fail and CONSTRAINT_DOESNT_EXIST status will be returned.
	 * If everything goes fine, then OKEY status is returned.
	 *
	 * @param	tableName is the name of the table to which we want to add the
	 *			TableConstraint.
	 * @param	constName is the name of the constraint to be deleted.
	 *
	 * @return	operation status specified above.
	 */
	virtual TableDDLOp::Status dropTableConstraint(
								const String &tableName, 
								const String &constraintName) = 0;

	/**
	 * This method adds a FieldConstraint to the table being specified as
	 * the first argument of the method to the field specified by the name
	 * given as the second parameter. The FieldConstraint being added is
	 * the one specified as the third argument.
	 *
	 * If the table does not exist, then the operation will fail and
	 * TABLE_DOESNT_EXIST status will be returned.
	 * If the field does not exists in the table, then the operation will 
	 * fail and FIELD_NOT_PRESENT_IN_TABLE status will be returned.
	 * If the user does not have permissions to add the constraint, then
	 * the operation will fail and the NEEDS_PERMISSION status will be
	 * returned.
	 * If the constraint already exists in the table, then the operation will
	 * fail and CONSTRAINT_ALREADY_EXISTS status is returned.
	 * If the constraint is a referential integrity one, and we are trying
	 * to create a foreign key over fields in a table that do not have a
	 * unique constraint or unique index over them, the operation will fail
	 * and FK_OVER_FIELDS_WITH_NON_UNIQUE_CONSTRAINT status should be
	 * returned.
	 * If everything goes fine, then OKEY status is returned.
	 *
	 * @param	tableName is the name of the table where the field constraint
	 *			will be placed.
	 * @param	fieldName is the name of the field to which we want to add
	 *			the field constraint.
	 * @param	fc is the FieldConstraint we want to add.
	 *
	 * @return	operation status specified above.
	 */
	virtual TableDDLOp::Status addFieldConstraint(
								const String &tableName, 
								const String &fieldName,
								const FieldMetadata *fmd,
								const FieldConstraint &fc) = 0;

	/**
	 * This method creates a primary key over an existing table in the schema
	 * provided to the class constructor. The name of the primary key is the
	 * one provided as the second argument of the method. This index will be
	 * composed by the fields specified in the IndexFieldList provided as
	 * the third argument. This index may have a constraint that is specified
	 * in the fourth parameter of the method. This index may be dropped after
	 * creation. This means the primary key is persistent and not temporary.
	 *
	 * If the table does not exist, then the operation will fail and
	 * TABLE_DOESNT_EXIST status will be returned.
	 * If the primary key already exists, then the operation will fail and
	 * INDEX_ALREADY_EXISTS status will be returned.
	 * If the user does not have permissions to add the primary key, then
	 * the operation will fail and the NEEDS_PERMISSION status will be
	 * returned.
	 * If one of the fields specified in the IndexFieldList is not a field
	 * of the table where the primary key is being created, then the 
	 * operation will fail and FIELD_NOT_PRESENT_IN_TABLE status will be 
	 * returned.
	 * If everything goes fine, then OKEY status is returned.
	 *
	 * @param	tableName is the name of the table where the primary key will
	 *			be created.
	 * @param	indexName is the name of the primary key being created.
	 * @param	ifl is the list of IndexFields that will compose the 
	 *			primary key.
	 * @param	ic is the primary key constraint imposed to the primary
	 *			key.
	 *
	 * @return	operation status specified above.
	 */
	virtual TableDDLOp::Status createPrimaryKey(
								const String &tableName,
								const String &indexName,
								const IndexFieldList &ifl) = 0;

	/**
	 * This method creates a new index over an existing table in the schema
	 * provided to the class constructor. The name of the new index is the
	 * one provided as the second argument of the method. This index will be
	 * composed by the fields specified in the IndexFieldList provided as
	 * the third argument. This index may have a constraint that is specified
	 * in the fourth parameter of the method. This index may be dropped after
	 * creation. This means the indexes are persistent and not temporary.
	 *
	 * If the table does not exist, then the operation will fail and
	 * TABLE_DOESNT_EXIST status will be returned.
	 * If the index already exists, then the operation will fail and
	 * INDEX_ALREADY_EXISTS status will be returned.
	 * If the user does not have permissions to add the constraint, then
	 * the operation will fail and the NEEDS_PERMISSION status will be
	 * returned.
	 * If we try to create a unique index over a table that does not 
	 * warranty the unicity over the index fields, the operation must fail 
	 * and UNIQUE_CONSTRAINT_VIOLATED_IN_INDEX status should be returned.
	 * If the name of the index is not a valid word or it is a reserved
	 * word in the underlying DBMS, then the operation will fail and
	 * INVALID_INDEX_NAME status will be returned.
	 * If one of the fields specified in the IndexFieldList is not a field
	 * of the table where the index is being created, then the operation
	 * will fail and FIELD_NOT_PRESENT_IN_TABLE status will be returned.
	 * If everything goes fine, then OKEY status is returned.
	 *
	 * @param	tableName is the name of the table where the index will
	 *			be created.
	 * @param	indexName is the name of the index being created.
	 * @param	ifl is the list of IndexFields that will compose the index key.
	 * @param	ic is the index constraint imposed to the index.
	 *
	 * @return	operation status specified above.
	 */
	virtual TableDDLOp::Status createIndex(
								const String &tableName,
								const String &indexName,
								const IndexFieldList &ifl,
								IndexConstraint::Constraint ic) = 0;

	/**
	 * This method creates a new index over an existing table in the schema
	 * provided to the class constructor. The name of the new index is the
	 * one provided as the second argument of the method. This index will be
	 * composed by the fields specified in the IndexFieldList provided as
	 * the third argument. This index may have a constraint that is specified
	 * in the fourth parameter of the method. This index is temporary and is
	 * not persistent. This index cannot be seen from other sessions or
	 * connections. Once the connection or session is closed, the index will
	 * no longer be visible to any user and should be dropped from the DBMS.
	 * This kind of index cannot be dropped.
	 *
	 * If the table does not exist, then the operation will fail and
	 * TABLE_DOESNT_EXIST status will be returned.
	 * If the index already exists, then the operation will fail and
	 * INDEX_ALREADY_EXISTS status will be returned.
	 * If the user does not have permissions to add the constraint, then
	 * the operation will fail and the NEEDS_PERMISSION status will be
	 * returned.
	 * If the name of the index is not a valid word or it is a reserved
	 * word in the underlying DBMS, then the operation will fail and
	 * INVALID_INDEX_NAME status will be returned.
	 * If one of the fields specified in the IndexFieldList is not a field
	 * of the table where the index is being created, then the operation
	 * will fail and FIELD_NOT_PRESENT_IN_TABLE status will be returned.
	 * If everything goes fine, then OKEY status is returned.
	 *
	 * @param	tableName is the name of the table where the index will
	 *			be created.
	 * @param	indexName is the name of the index being created.
	 * @param	ifl is the list of IndexFields that will compose the index.
	 * @param	ic is the index constraint imposed to the index.
	 *
	 * @return	operation status specified above.
	 */
	virtual TableDDLOp::Status createTemporaryIndex(
								const String &tableName,
								const String &indexName,
								const IndexFieldList &ifl,
								IndexConstraint::Constraint ic) = 0;

	/**
	 * This method drops an existing persistent index. This index must be
	 * an index in the "tableName" table. The index belongs to the schema
	 * used in the class constructor.
	 *
	 * If the tableName does not exists, then the operation will fail and
	 * TABLE_DOESNT_EXIST status will be returned.
	 * If the index does not exist, then the operation will fail and
	 * INDEX_DOESNT_EXISTS status will be returned.
	 * If the user does not have permissions to add the constraint, then
	 * the operation will fail and the NEEDS_PERMISSION status will be
	 * returned.
	 * If everything goes fine, then OKEY status is returned.
	 *
	 * @param	tableName is the name of the table to which the index belongs.
	 * @param	indexName is the name of the index we want to drop.
	 *
	 * @return	operation status specified above.
	 */
	virtual TableDDLOp::Status dropIndex(const String &indexName) = 0;

	/**
	 * This method locks the table specified by the name supplied as the
	 * first parameter of the method. The locking mode is an exclusive one.
	 * The table can only be unlocked or freed by the same user that 
	 * acquired the lock. After the table lock is acquired, no other user
	 * or session will be available to lock any records inside the table or
	 * lock the table itself.
	 *
	 * If the tableName does not exist, then the operation will fail and
	 * TABLE_DOESNT_EXIST status will be returned.
	 * If the table has already been locked and the locking parameter is
	 * LOCK_TEST, then we have to return TABLE_LOCKED.
	 * If the table has already been locked and the locking parameter is
	 * LOCK, then we have to wait until the lock is release and we can
	 * get it. After that we have to return OKEY.
	 * If the lock could be acquired without problems, then we have to return
	 * OKEY.
	 *
	 * @param	tableName is the name of the table we want to lock.
	 * @param	lmode is the locking mode used to lock the table.
	 *
	 * @return	operation status specified above.
	 */
	virtual TableDDLOp::Status lockTable(
								const String &tableName, 
								enum LockMode lmode) = 0;

	/**
	 * This method is used when we want to release a lock previously 
	 * acquiered with lockTable method.
	 *
	 * If the tableName does not exist, then the operation will fail and
	 * TABLE_DOESNT_EXIST status will be returned.
	 * If the lock could be free without problems, then OKEY is returned.
	 * If the lock was never adquiered, then LOCK_NEVER_ACQUIRED must be
	 * returned.
	 *
	 * @param	tableName is the name of the table we want to unlock or free.
	 *
	 * @return	operation status specified above.
	 */
	virtual TableDDLOp::Status freeTable(const String &tableName) = 0;

	/**
	 * This method is used to grant permissions over the table specified in
	 * the first parameter. The permissions to be granted over the table will
	 * be the ones given in the PermissionList supplied in the second 
	 * parameter.
	 *
	 * If the tableName does not exist, then the operation will fail and
	 * TABLE_DOESNT_EXIST status will be returned.
	 * If the user does not have permission to grant permissions, then the
	 * operation will fail and NEEDS_PERMISSION status will be returned.
	 * If any of the permissions in the grant list fails, then the operation
	 * will fail and GRANTS_FAILED status will be returned. The same will
	 * take place if one of the permissions to grant is not a 
	 * GrantTablePermission.
	 * If all grant permissions could be applied properly and without 
	 * problems, we have to return OKEY.
	 *
	 * @param	tableName is the name of the table over which we will grant
	 *			permissions.
	 * @param	pl is the list of grant permissions to be granted over the
	 *			table.
	 *
	 * @return	operation status specified above.
	 */
	virtual TableDDLOp::Status grant(const String &tableName,
								const PermissionList &pl) = 0;

	/**
	 * This method is used to revoke permissions from the table specified in
	 * the first parameter. The permissions to be revoked from the table will
	 * be the ones given in the PermissionList supplied in the second 
	 * parameter.
	 *
	 * If the tableName does not exist, then the operation will fail and
	 * TABLE_DOESNT_EXIST status will be returned.
	 * If the user does not have permission to revoke permissions, then the
	 * operation will fail and NEEDS_PERMISSION status will be returned.
	 * If any of the permissions in the revoke list fails, then the operation
	 * will fail and REVOKES_FAILED status will be returned. The same will
	 * take place if one of the permission if not a RevokeTablePermission.
	 * If all grant permissions could be applied properly and without 
	 * problems, we have to return OKEY.
	 *
	 * @param	tableName is the name of the table over which we will revoke
	 *			table permissions.
	 * @param	pl is the list of revoke permissions to be revoked from the
	 *			table.
	 *
	 * @return	operation status specified above.
	 */
	virtual TableDDLOp::Status revoke(const String &tableName,
								const PermissionList &pl) = 0;

	/**
	 * this method returns the last error that took place in the class
	 * executing one of the method in this class.
	 */
	inline String getErrorDescr() const { return errorDescr_d; }

protected:
	String errorDescr_d;
	SchemaMetadata *schemaMD_d;
	
	/**
	 * This method sets the last error that took place while calling any
	 * of the methods in this class.
	 */
	inline void setErrorDescr(const String &errDescr) { errorDescr_d = errDescr; }

	/**
	 * This method is used to get the SchemaMetadata attached to the schema
	 * implementation. This SchemaMetadata has been attached when the
	 * object is created.
	 *
	 * @return	SchemaMetadata associated with this schema implementation.
	 */
	inline const SchemaMetadata &schemaMetadata() const { return *schemaMD_d; }

	/**
	 * This method is used to obtain the connectionImpl that must be used
	 * to execute the operations needed by the DDLManager.
	 *
	 * @return	the connectionImpl that must be used.
	 */
	inline const ConnectionImpl &connection() const {
		assert(schemaMD_d);
		const SQLFWSchema *sc = schemaMD_d->getSchema();
		assert(sc);
		const SQLFWConnection *conn = sc->getConnection();
		assert(conn);
		const ConnectionImpl *connImpl = conn->getImpl();
		assert(connImpl);
		return *connImpl;
	}

	/**
	 * This is the default constructor and should not be used.
	 */
	TableDDLManagerImpl();

private:

	/**
	 * These two methods are private to the class and should not be made public.
	 * They are just hidden to control object creation.
	 */
	TableDDLManagerImpl(const TableDDLManagerImpl &si);
	TableDDLManagerImpl &operator=(const TableDDLManagerImpl &si);
};

#endif	// TMGRIMPL_H
