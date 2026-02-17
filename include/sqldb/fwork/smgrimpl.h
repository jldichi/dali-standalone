//- Copyright Notice
// -----------------------------------------------------------------------
// (C) Copyright 1998 InterSoft S.A.  All Rights Reserved
// THIS IS UNPUBLISHED PROPRIETARY SOURCE CODE OF InterSoft S.A.
// The copyright notice above does not evidence any actual or intended 
// publication of such source code.
//
// $Id
// -----------------------------------------------------------------------

#ifndef SMGRIMPL_H
#define SMGRIMPL_H

class SQLFWConnection;

#include <sqldb/metadata/mdmisc.h>
#include <sqldb/fwork/connect.h>

class PermissionList;
class SchemaMetadata;

/**
 * This class is used to provide a common interface to the different
 * implementations. That's why most of its methods are pure virtual.
 * A SchemaDDLManager is responsible for executing DDL methods over a DB.
 * These DDL methods deal with schema creation and deletion, and granting
 * and revoking permissions at schema level.
 * The actions that take place during the execution of the operations over the
 * DB will be performed using the 'connImpl_d' member of the class.
 * 
 * @version
 * @author Pablo Rubinstein & Eduardo Chiocconi
 *
 * @see SQLFWConnection
 */
class SchemaDDLManagerImpl {

public:	
	SchemaDDLManagerImpl(const ConnectionImpl &conn) { connImpl_d = &conn; }
	virtual ~SchemaDDLManagerImpl() {}

	/**
	 * This method creates a new user in the DB.
	 * If the user already exists in the DB, then the operation will fail
	 * and SCHEMA_ALREADY_EXIST must be returned.
	 * If the user is not authorized to perform the user creation, then the
	 * operation will fail and SCHEMA_NEEDS_PERMISSION must be returned.
	 * If there is an error not listed above, UNKNOWN_ERROR must be returned.
	 * If none of the above cases takes place, then OKEY is returned.
	 *
	 * @param	user is the user name.
	 *
	 * @return	the status of the operation.
	 */
	virtual SchemaDDLOp::Status createUser(const String &user) = 0;

	/**
	 * This method removes an existing user from the DB.
	 * If the user does not exist in the DB, then the operation will fail
	 * and USER_ALREADY_EXIST must be returned.
	 * If the user is not authorized to perform the user deletion, then the
	 * operation will fail and SCHEMA_NEEDS_PERMISSION must be returned.
	 * If there is an error not listed above, UNKNOWN_ERROR must be returned.
	 * If none of the above cases takes place, then OKEY is returned.
	 *
	 * @param	user is the user name.
	 *
	 * @return	the status of the operation.
	 */
	virtual SchemaDDLOp::Status dropUser(const String &user) = 0;


	/**
	 * This method creates a new schema. It does not open it.
	 * If the schema already exists in the DB, then the operation will fail
	 * and SCHEMA_ALREADY_EXIST must be returned.
	 * If the user is not authorized to perform the schema creation, then the
	 * operation will fail and SCHEMA_NEEDS_PERMISSION must be returned.
	 * If the name of the schema is not a valid one, then SCHEMA_INVALID_NAME
	 * status should be returned and no schema should be created.
	 * If there is an error not listed above, UNKNOWN_ERROR must be returned.
	 * If none of the above cases takes place, then OKEY is returned.
	 *
	 * @param	name is the schema name.
	 *
	 * @return	the status of the operation.
	 */
	virtual SchemaDDLOp::Status createSchema(const String &name) = 0;

	/**
	 * This method drops an existing schema. Must execute the proper sentence
	 * to remove the schema from the DB, but it does not do a thing about the
	 * tables that may remain in the schema.
	 * If the schema does not exists, the operation will fail and
	 * SCHEMA_DOESNT_EXIST will be returned.
	 * If the user does not have permission to drop the schema, the operation
	 * will fail, and SCHEMA_NEEDS_PERMISSION will be returned.
	 * If there is an error not listed above, UNKNOWN_ERROR must be returned.
	 * If none of the above cases takes place, then OKEY is returned.
	 *
	 * @param	name is the schema name.
	 *
	 * @return	the status of the operation.
	 */
	virtual SchemaDDLOp::Status dropSchema(const String &name) = 0;

	/**
	 * This method locks an existing schema. This means that any table, field
	 * or other object belonging to the schema won't be accesible until
	 * the lock is released.
	 * If the schema has already been locked, then the operation will fail
	 * and SCHEMA_ALREADY_LOCKED will be returned.
	 * If the schema does not exists, then the operation will fail and
	 * SCHEMA_DOESNT_EXIST will be returned.
	 * If lockMode is NO_LOCK, the operation should return OKEY status
	 * without performing any task.
	 * If the user does not have permissions to lock the schema, then the
	 * operation will fail and SCHEMA_NEEDS_PERMISSION will be returned.
	 * If there is an error not listed above, UNKNOWN_ERROR must be returned.
	 * If none of the above cases takes place, then OKEY is returned.
	 *
	 * @param	schemaName is the schema name.
	 * @param	lockMode determines the way the table must be locked.
	 *
	 * @return	the status of the operation.
	 *
	 * @see		LockMode
	 */
	virtual SchemaDDLOp::Status lockSchema(const SchemaMetadata &smd,
											enum LockMode lockMode)	= 0;

	/**
	 * This method frees an existing and locked schema.
	 * If the schema has already been freed, then the operation will fail
	 * and SCHEMA_NEVER_LOCKED will be returned.
	 * If the schema does not exists, then the operation will fail and
	 * SCHEMA_DOESNT_EXIST will be returned.
	 * If the user does not have permissions to unlock the schema, then the
	 * operation will fail and SCHEMA_NEEDS_PERMISSION will be returned.
	 * If there is an error not listed above, UNKNOWN_ERROR must be returned.
	 * If none of the above cases takes place, then OKEY is returned.
	 *
	 * @param	smd contains the information about the schema.
	 *
	 * @return	the status of the operation.
	 */
	virtual SchemaDDLOp::Status freeSchema(const SchemaMetadata &smd) = 0;

	/**
	 * This method is used to grant a permission over the schema.
	 * If the schema does not exists, then the operation will fail and
	 * SCHEMA_DOESNT_EXIST will be returned.
	 * If the user does not have permissions to grant, then the
	 * operation will fail and SCHEMA_NEEDS_PERMISSION will be returned.
	 * If any of the users or groups specified in the grants of the
	 * PermissionList does not exists, then the operation will fail, and
	 * USER_DOESNT_EXIST status will be returned.
	 * If there is an error not listed above, UNKNOWN_ERROR must be returned.
	 * If none of the above cases takes place, then OKEY is returned.
	 *
	 * @param	perms is the list of permissions to grant to the schema.
	 * @param	name is the schema name.
	 *
	 * @return	the status of the operation.
	 */
	virtual SchemaDDLOp::Status grant(const String &name,
									const PermissionList &perms) = 0;

	/**
	 * This method revokes a permission from an existing schema.
	 * If the schema does not exists, then the operation will fail and
	 * SCHEMA_DOESNT_EXIST will be returned.
	 * If the user does not have permissions to revoke, then the
	 * operation will fail and SCHEMA_NEEDS_PERMISSION will be returned.
	 * If any of the users or groups specified in the revokes of the
	 * PermissionList does not exists, then the operation will fail, and
	 * USER_DOESNT_EXIST status will be returned.
	 * If there is an error not listed above, UNKNOWN_ERROR must be returned.
	 * If none of the above cases takes place, then OKEY is returned.
	 *
	 * @param	name is the schema name.
	 * @param	perms is the list of permissions to revoke from the schema.
	 *
	 * @return	the status of the operation.
	 */
	virtual SchemaDDLOp::Status revoke(const String &name,
									const PermissionList &perms) = 0;

	/**
	 * Debug.
	 */
	virtual void dump() const {};

	/**
	 * this method returns the last error that took place in the class
	 * executing one of the method in this class.
	 */
	inline String getErrorDescr() const { return errorDescr_d; }

protected:
	String errorDescr_d;
	const ConnectionImpl *connImpl_d;

	/**
	 * This method sets the last error that took place while calling any
	 * of the methods in this class.
	 */
	inline void setErrorDescr(const String &errDescr) { errorDescr_d = errDescr; }

	/**
	 * Default constructor. Should not be called.
	 */
	SchemaDDLManagerImpl() { assert(0); }

private:
	/**
	 * These two methods are private to the class and should not be made public.
	 * They are just hidden to control object creation.
	 */
	SchemaDDLManagerImpl(const SchemaDDLManagerImpl &si);
	SchemaDDLManagerImpl &operator=(const SchemaDDLManagerImpl &si);
};

#endif //SMGRIMPL_H
