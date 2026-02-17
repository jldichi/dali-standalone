//- Copyright Notice
// -----------------------------------------------------------------------
// (C) Copyright 1998 InterSoft S.A.  All Rights Reserved
// THIS IS UNPUBLISHED PROPRIETARY SOURCE CODE OF InterSoft S.A.
// The copyright notice above does not evidence any actual or intended 
// publication of such source code.
//
// $Id: implmgr.h,v 1.14 2001/05/04 21:49:39 albertoi Exp $
// -----------------------------------------------------------------------

#ifndef IMPLMGR_H
#define IMPLMGR_H

#include <ifound/str.h>
#include <sqldb/metadata/mdmisc.h>

class ConnectionImpl;
class TableDDLManagerImpl;
class TableImpl;
class CursorImpl;
class QueryCursorImpl;
class TransactionImpl;
class SchemaMetadata;
class TableMetadata;
class IndexMetadata;
class StatementBuilder;
class SchemaDDLManagerImpl;
class DataTypesImpl;                                        
class SQLFWUrl;

/** 
 * This class is used to obtain connections of specific implementation. 
 * It should be able to manage multiple connections to the DB. It must not
 * restringe the amount of opened connections in any way.
 * It is assumed that in a per process basis, there will be a connection for
 * every accesed DB server.
 *
 * Although there is one single interface for all protocols (ODBC, CLI, etc.),
 * there should be as many source files as protocols exist.
 * 
 * @version
 * @author Diego Balbi & Eduardo Chiocconi.
 * @see ConnectionImpl
 * @see TableImpl
 */
class ImplManager {

public:	
	/**
	 * This class-method must return  an object representing a successfully
	 * connection or NULL otherwise, where an error description should be returned
	 * by getErrorDescr().
	 *
	 * @param	URL is the information needed to locate the target.
	 *
	 * @return	the connection.
	 */
	static ConnectionImpl *getConnection(const SQLFWUrl &url);

	/**
	 * Obtains a TableDDLManager to perform DDL operation over a table.
	 * We have to create one for each schema.
	 *
	 * @param	cimpl is the conecction to use.
	 * @param   smd is the schema structure or SchemaMetadata where the DDL
	 *          operations will be performed.
	 *
	 * @return	a table DDL operation executor.
	 */
	static TableDDLManagerImpl *getTableDDLManager(const SchemaMetadata &smd);

	/**
	 * Obtains the table data from the DSN. this method is used to load the 
	 * metadata (ie: to read the metadata.schemas or metadata.tables)
	 *
	 * @param	cimpl is the conecction to use.
	 * @param	tmd contains the information necessary to build the table.
	 *
	 * @return	a table implementation (in this case an ODBC one)
	 */
	static TableImpl *getTable(const ConnectionImpl *cimpl, const TableMetadata &tmd);

	/**
	 * Obtain a cursor (in this case an ODBC cursor) over a given table,
	 *  using a given index.
	 *
	 * @param	tmd is the TableMetadata used to acces the table.
	 * @param	idm is the IndexMetadata used to acces the index.
	 *
	 * @return	the cursor.
	 */
	static CursorImpl *getCursor(const TableMetadata &tmd, const IndexMetadata &ind, enum LockMode lmode);

	/**
	 * Obtain a transaction given the connection used to perform operations.
	 *
	 * @return	the transaction.
	 */
	static TransactionImpl *getTransaction(const ConnectionImpl *ci);

	/**
	 * Obtain the StatementBuilder that will build the SQL queries for you.
	 * This is only needed for ODBC implementation, that's why maybe we
	 * should implement ImplManager as a virtual class.
	 */
	static const StatementBuilder &getStatementBuilder();

	/**
	 * Set the StatementBuilder that will build the SQL queries for you.
	 * This is only needed for ODBC implementation, that's why maybe we
	 * should implement ImplManager as a virtual class.
	 */
	static void setStatementBuilder();

	/**
	 * Determine wether the StatementBuilder has been set or not.
	 */
	static bool hasStatementBuilderBeenSet();

	/**
	 * Obtain a SchemaDDLManagerImpl object that knows how to execute DDL
	 * actions at schema level.
	 */
	static const SchemaDDLManagerImpl &getSchemaDDLManagerImpl(
											const ConnectionImpl &conn);


	/**
	 * Obtains the query cursor from the DSN. 
	 *
	 * @param	cimpl is the conecction to use.
	 * @param	qry is the string for the query to 
	 *
	 * @return	a query Cursor implementation (in this case an ODBC one)
	 */
	//static QueryCursorImpl *getQueryCursor(const ConnectionImpl *cimpl, const String &qry);
	static QueryCursorImpl *getQueryCursor(const ConnectionImpl *cimpl, String qry);

	/**
	 * This method returns the DataTypesImpl for field type creation.
	 */
	static DataTypesImpl *getDataTypes();

	/**
	 * Returns the message of the las error ocurred.
	 *
	 * @return	the error message.
	 */
	static const String &getErrorDescr();

	// This class is not intended to be instantiated (avoid compiling warn).

	/**
	 * Class destructor. 
	 */
	inline ImplManager() { abnormal; }

	~ImplManager();

private:
	static StatementBuilder *stmtbldr_d;
	static String errDescr_d;
	
	// This class is not intended to be instantiated.
	/**
	 * Copy constructor. they should not be called.
	 */
	ImplManager(const ImplManager &);
	ImplManager &operator=(const ImplManager &);
};

#endif //IMPLMGR_H
