//- Copyright Notice
// -----------------------------------------------------------------------
// (C) Copyright 1998 InterSoft S.A.  All Rights Reserved
// THIS IS UNPUBLISHED PROPRIETARY SOURCE CODE OF InterSoft S.A.
// The copyright notice above does not evidence any actual or intended 
// publication of such source code.
//
// $Id: status.h,v 1.38 2002/03/19 20:46:27 hardaiz Exp $
// -----------------------------------------------------------------------

#ifndef SQLFWSTATUS_H
#define SQLFWSTATUS_H

/**
 * This class contains status values that will be returned
 * when an operation over a schema is performed.
 */
class SchemaOp {

public:	

	enum Status {
		COULD_NOT_LOCK_TABLE = -12,
		COULD_NOT_ADD_TABLE_CONSTRAINT = -11,
		COULD_NOT_DROP_INDEX = -10,
		COULD_NOT_DROP_TABLE = -9,
		COULD_NOT_CREATE_INDEX = -8,
		COULD_NOT_ADD_FIELD_CONSTRAINT = -7,
		COULD_NOT_CREATE_TABLE = -6,
		ABORTED = -5,
		TABLE_LOCKED = -4,
		COULD_NOT_LOCK_SCHEMA = -3,
		SCHEMA_NEVER_LOCKED = -2,
		UNKNOWN_ERROR = -1,
		OKEY = 0,
	};
};

/**
 * This class contains status values that will be returned
 * when an operation over a table is performed.
 */
class TableOp {

public:	

	enum Status {
		NO_INSERT_PERMISSION = -32,
		NO_DELETE_PERMISSION = -31,
		NO_UPDATE_PERMISSION = -30,
		NO_SELECT_PERMISSION = -29,
		RECORD_IS_BEING_REFERENCED_FROM_ANOTHER_TABLE = -28,
		DUP_KEY_IN_UNIQUE_INDEX = -27,
		CONSTRAINT_VIOLATED_IN_INSERT = -26,
		DATA_OVERFLOW = -25,
		RECORD_WITH_NULL_IN_PK = -24,
		INCHECK_CHECK_FAILED = -23,
		CHECK_CHECK_FAILED = -22,
		FOREIGNKEY_CHECK_FAILED = -21,
		NOTNULL_CHECK_FAILED = -20,
		RECORD_IS_LOCKED = -19,
		CFIX_BUFFER_NOT_SET = -18,
		INDEX_OUT_OF_BOUNDS = -17,
		NOT_A_UNIQUE_ROW = -16,
		CONSTRAINT_CHECK_FAILED = -15,
		WRONG_PARAM_NUMBER = -14,
		RECORD_EXISTS = -13,
		NO_RECORD_UPDATED = -12,
		NO_RECORD_DELETED = -11,
		MORE_THAN_A_ROW_UPDATED = -10,
		MORE_THAN_A_ROW_DELETED = -9,
		RECORD_NOT_FOUND = -8,
		PROBLEMS_GETTING_DATA = -7,
		INVALID_BUFFER_SIZE = -6,
		DUPLICATED_RECORD = -5,
		PERMISSION_DENIED = -4,
		INVALID_TYPE_CONVERTION = -3,
		BROKEN_CONNECTION = -2,
		UNKNOWN_ERROR = -1,
		OKEY = 0
	};
};

/**
 * This class contains status values that will be returned
 * when an operation over a cursor is performed.
 */
class CursorOp {

public:	

	enum Status {
		INVALID_DIRECTION = -13,
		NO_SELECT_PERMISSION = -12,
		RECORD_IS_LOCKED = -11,
		CUR_BOF = -10,
		CUR_EOF = -9,
		NO_RECORD_AVAILABLE = -8,
		RECORD_ERROR = -7,  
		TRUNCATED_PARAMS = -6, 
		WRONG_PARAM_NUMBER = -5,
		INVALID_BUFFER_SIZE = -4,
		INVALID_PARAMETER = -3,
		TYPE_CONVERSION_ERROR = -2,
		UNKNOWN_ERROR = -1,
		OKEY = 0
	};
};

/**
 * This class contains status values that will be returned
 * when an operation over an index is performed.
 */
class IndexOp {

public:	

	enum Status {
		OKEY	= 0
	};
};

/**
 * This class contains status values that will be returned
 * when an operation over a transaction is performed.
 */
class TransactionOp {

public:	

	enum Status {
		COULD_NOT_ROLLBACK_TRANSACTION = -2,
		CORRUPTED	= -1,
		OKEY		= 0
	};
};

/**
 * This class contains status values that will be returned
 * when an operation over a connection is performed.
 */
class ConnectionOp {

public:	

	enum Status {
		SET_CONSTRAINTS_IMMEDIATE_FAILED = -21,
		SET_CONSTRAINTS_DEFERRED_FAILED = -20,
		PERMISSION_DENIED = -19,
		USER_ALREADY_EXISTS = -18,
		SCHEMA_NEVER_LOCKED = -17,
		SCHEMA_IS_LOCKED = -16,
		SCHEMA_NOT_FOUND = -15,
		INV_USER_OR_PASSWD = -14,
		INVALID_USER = -13,
		WRONG_URL = -12,
		TRANSACTIONS_OPENED = -11,
		CONNECTION_NOT_OPENED = -10,
		COULD_NOT_DISCONNECT = -9,
		TIMEOUT = -8,
		INVALID_BUFFER_SIZE = -7,
		MAX_CONN_REACHED = -6,
		ACCESS_DENIED = -5,
		BROKEN_CONNECTION = -4,
		CONNECTION_ALREADY_OPENED = -3,
		COULD_NOT_CONNECT = -2,
		UNKNOWN_ERROR = -1,
		OKEY = 0
	};
};

/**
 * This class contains status values that will be returned
 * when an DDL operation is performed with schemas.
 */
class SchemaDDLOp {

public:	

	enum Status {
		// Informix specific error codes.
		NO_DBSPACE_IN_DBMS = -200,	// sqlcode = -255

		// Oracle specific error codes.

		// Gerenal error codes.
		SCHEMA_INVALID_NAME = -10,
		SCHEMA_ALREADY_EXISTS = -9,
		SCHEMA_IS_LOCKED = -8,
		USER_DOESNT_EXIST = -7,
		SCHEMA_NEVER_LOCKED = -6,
		SCHEMA_ALREADY_LOCKED = -5,
		SCHEMA_DOESNT_EXISTS = -4,
		SCHEMA_NEEDS_PERMISSION = -3,
		GENERAL_ERROR = -2,
		UNKNOWN_ERROR = -1,
		OKEY = 0
	};
};

/**
 * This class contains status values that will be returned
 * when an DDL operation is performed with a table.
 */
class TableDDLOp {

public:	

	enum Status {
		// Informix specific error codes.
		NO_MORE_LOCKS_IN_DBMS = -200,	// sqlcode = -312

		// Oracle specific error codes.

		// Gerenal error codes.
		DEFAULT_VALUE_FOR_PK_IS_NULL = -31,
		LONG_TRANSACTION_ABORTED = -30,
		NOT_IN_TRANSACTION = -29,
		REFERENCED_FIELD_HAS_DIFFERENT_TYPE = -28,
		TABLE_IS_BEING_ACCESSED = -27,
		INDEX_ALREADY_EXISTS = -26,
		SERVER_GENERAL_ERROR = -25,
		CANNOT_USE_COLUMN = -24,
		UNIQUE_CONSTRAINT_VIOLATED_IN_INDEX = -23,
		CONSTRAINT_DOESNT_EXIST = -22,
		INDEX_NAME_ALREADY_EXISTS = -21,
		FK_OVER_FIELDS_WITH_NON_UNIQUE_CONSTRAINT = -20,
		INVALID_INDEX = -19,
		INVALID_CONSTRAINT = -18,
		INVALID_FIELD_NAME = -17,
		INVALID_TABLE_NAME = -16,
		ALREADY_EXISTS = -15,
		REVOKES_FAILED = -14,
		GRANTS_FAILED = -13,
		LOCK_NEVER_ACQUIRED = -12,
		TABLE_LOCKED = -11,
		INDEX_FIELD_ALREADY_EXISTS = -10,
		INDEX_DOESNT_EXIST = -9,
		FIELD_NOT_PRESENT_IN_TABLE = -8,
		CONSTRAINT_ALREADY_EXISTS = -7,
		INDEX_ALREADY_EXISTS_WITH_SAME_FIELDS = -6,
		FIELD_ALREADY_EXISTS = -5,
		TABLE_DOESNT_EXIST = -4,
		NEEDS_PERMISSION = -3,
		GENERAL_ERROR = -2,
		UNKNOWN_ERROR = -1,
		OKEY = 0
	};
};

/**
 * This class contains Index Constraints that will be used by createIndex
 * DDL methods.
 */
class IndexConstraint {

public:	

	enum Constraint {
		INDEX_NORMAL = 0,
		INDEX_TEMPORARY = 1,
		INDEX_UNIQUE_NORMAL = 2,
		INDEX_UNIQUE_TEMPORARY = 3
	};
};

#endif // SQLFWSTATUS_H
