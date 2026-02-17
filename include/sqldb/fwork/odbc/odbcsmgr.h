//- Copyright Notice
// -----------------------------------------------------------------------
// (C) Copyright 1998 InterSoft S.A.  All Rights Reserved
// THIS IS UNPUBLISHED PROPRIETARY SOURCE CODE OF InterSoft S.A.
// The copyright notice above does not evidence any actual or intended 
// publication of such source code.
//
// $Id: odbcsmgr.h,v 1.7 1999/05/19 22:16:50 eduardoc Exp $
// -----------------------------------------------------------------------

#ifndef ODBCSMGRIMPL_H
#define ODBCSMGRIMPL_H

#include <sqldb/fwork/smgrimpl.h>
#include <sqldb/fwork/odbc/odbcconn.h>

/**
 * This class is used to provide a specific implementation for the 'abstract'
 * class SchemaDDLManagerImpl.
 * A ODBCSchemaDDLManager is responsible for executing DDL methods over a DB
 * using ODBC methods.
 * These DDL methods deal with schema creation and deletion, and granting
 * and revoking permissions at schema level.
 * 
 * @version
 * @author Pablo Rubinstein
 *
 * @see	SchemaDDLManagerImpl
 */
class ODBCSchemaDDLManagerImpl : public SchemaDDLManagerImpl {

public:	
	ODBCSchemaDDLManagerImpl(const ConnectionImpl &conn);
	~ODBCSchemaDDLManagerImpl();

	// DDL Operations
	SchemaDDLOp::Status createUser(const String &user);
	SchemaDDLOp::Status dropUser(const String &name);
	SchemaDDLOp::Status createSchema(const String &name);
	SchemaDDLOp::Status dropSchema(const String &name);
	SchemaDDLOp::Status lockSchema(const SchemaMetadata &smd,
									enum LockMode lockMode);
	SchemaDDLOp::Status freeSchema(const SchemaMetadata &smd);
	SchemaDDLOp::Status grant(const String &name,
										const PermissionList &perms);
	SchemaDDLOp::Status revoke(const String &name,
										const PermissionList &perms);
private:
	const ODBCConnectionImpl& connection() const;

	ODBCSchemaDDLManagerImpl(const ODBCSchemaDDLManagerImpl &si);
	ODBCSchemaDDLManagerImpl &operator=(const ODBCSchemaDDLManagerImpl &si);
};

#endif //ODBCSMGRIMPL_H
