//- Copyright Notice
// -----------------------------------------------------------------------
// (C) Copyright 1998 InterSoft S.A.  All Rights Reserved
// THIS IS UNPUBLISHED PROPRIETARY SOURCE CODE OF InterSoft S.A.
// The copyright notice above does not evidence any actual or intended 
// publication of such source code.
//
// $Id: connect.h,v 1.32 2002/03/20 15:09:06 hardaiz Exp $
// -----------------------------------------------------------------------

#ifndef SQLFWCONNECT_H
#define SQLFWCONNECT_H

#include <ideafix.h>

#include <sqldb/fwork/connimpl.h>
#include <sqldb/fwork/smgrimpl.h>
#include <sqldb/fwork/permlist.h>
#include <sqldb/fwork/transact.h>
#ifdef SQLFW_NESTED_TRAN
#include <sqldb/fwork/sqlfwurl.h>
#endif // SQLFW_NESTED_TRAN

class SchemaDDLManagerImpl;
class SQLFWSchema;
class SchemaMetadata;                            
class SQLFWUrl;

#define MAX_OPENED_CONNECTIONS	255

#define URL_SEPARATOR	":"
#define	DBMS_POS	0

/**
 * This class is used to make a connection (link) with a database 
 * contacting a defined schema in the database.
 * The other function of this class is to hide the existence of the 
 * SchemaDDLManagerImpl. Parts of the schema DDL methods will be implemented
 * by the connection instead of the DDLManager, because in that way we are
 * sure we will obtain the same behavior even with different implementations.
 *
 * IMPORTANT: Each connection has a transaction attached to it. There can
 * be one and only one transaction by connection. This does not imply that
 * you cannot have more than one connection with one server.
 * 
 * @version
 * @author Diego Balbi & Eduardo Chiocconi.
 * @see Schema
 * @see ConnectionImpl
 */
class SQLFWConnection {

public:	
	SQLFWConnection(ConnectionImpl *c);
	~SQLFWConnection();     
	
	// Method that opens a connection
	bool open();

	// Schema manipulation methods.
	static SQLFWSchema *openSchema(const SQLFWUrl &URL, short openMode);
	static void closeSchema(Int sch);
	inline const SQLFWUrl &getURL() const;
	inline const String &getSchemaName() const;
	static inline SQLFWConnection *openConnection(SQLFWUrl &url);

	// Transaction methods.
	static bool transOK();
	static bool inTransac();
	static bool didTransaction();
	inline bool inTransaction() const;

	static void setUserBeginTransaction(bool ubt);
	static bool userBeginTransaction();

	static Int beginTransactions();
	static Int endTransactions(bool doSync);
	static bool rollbackTransactions();

	bool beginTransaction();
	bool commitTransaction();
	bool abortTransaction();

	// Constraints' checking
	inline ConnectionOp::Status setConstraintsDeferred();
	inline ConnectionOp::Status setConstraintsImmediate();
	
	// Methods to manipulate nBegTrans_d
	inline Int nBegTrans() const;
	inline void incNBegTrans();
	inline void decNBegTrans();
	inline bool isTransOK() const;
	
	// Methods that will be delivered to the DDLManager
	ConnectionOp::Status createSchema(const String &schemaName,
								const String &descr);
	static ConnectionOp::Status dropSchema(schema sch);
	ConnectionOp::Status lockSchema(const SchemaMetadata &smd,
								enum LockMode lockMode);
	ConnectionOp::Status freeSchema(const String &schemaName);
	ConnectionOp::Status changeSchemaOwner(const String &schName,
								const String &owner);
	ConnectionOp::Status grant(const String &schemaName,
								const PermissionList &perms);
	ConnectionOp::Status revoke(const String &schemaName,
								const PermissionList &perms);

	// Miscelaneous methods
	inline ConnectionImpl* getImpl() const;
	inline SQLFWTransaction *transaction() const;
	inline const String &getErrorDescr();
	static inline const String &getClassErrorDescr();
	static bool deleteOpenedConnections();

	static enum DbmsType getDbmsType(const String &url);
	
	inline bool isOpen();
	inline void lockTableBeganTransaction(bool ltbt);
	inline bool hasLockTableBeganTransaction() const;
	inline void lockSchemaBeganTransaction(bool lsbt);
	inline bool hasLockSchemaBeganTransaction() const;
	static Int  beginTrxByLockSchema();
	static Int  endTrxByLockSchema();
	inline bool isMetadataConnection() const;
	inline void setMetadataConnection();

	// Debug
	void dump() const;

private:
	ConnectionImpl *connImpl_d;
	SQLFWTransaction *trans_d;
	String errorDescr_d;
	SchemaDDLManagerImpl *ddlMgr_d;
	bool inTransaction_d;
	Int	 nBegTrans_d;		// Number of transactions opened in this conn.
	// This member is used to know whether a transaction has been started
	// because LockTable CFIX function was called when we were not in a
	// transaction.
	bool beganTrxByLockTable_d;
	// This member is used to know whether a transaction has been started
	// because LockSchema CFIX function was called when we were not in a
	// transaction.
	bool beganTrxByLockSchema_d;
	bool isMetadataConnection_d; // Used to known whether the connection
								 // is being used to connect metadata schema.

	static String errorDescr_sd;
	static SQLFWConnection *openedConnections_d[MAX_OPENED_CONNECTIONS];

	// Methods that deal with the array of opened connections.
	static SQLFWConnection *open(const SQLFWUrl &URL);
	static SQLFWConnection *find(const String &URL);
    static bool del(const SQLFWUrl &URL);
	static Int findFreeSlot();

	// Getters.
	SchemaDDLManagerImpl& ddlManager();
	inline void setInTransaction(bool sit);

	// Methods that implement DML.
	SQLFWSchema *openSchema_(const String &URL, short openMode);

	// Methods that implement DDL.
	ConnectionOp::Status createUser(const String &user) const;
	static bool addToSchemasTable(const SQLFWConnection &dbaconn,
					 		const String &schemaName, const String &descr,
							const String &user);

	// Forbidden methods.
	SQLFWConnection(const SQLFWConnection &);
	SQLFWConnection &operator=(const SQLFWConnection &);
};

#include <sqldb/fwork/connect.icc>

#endif // SQLFWCONNECT_H
