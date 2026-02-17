//- Copyright Notice
// -----------------------------------------------------------------------
// (C) Copyright 1998 InterSoft S.A.  All Rights Reserved
// THIS IS UNPUBLISHED PROPRIETARY SOURCE CODE OF InterSoft S.A.
// The copyright notice above does not evidence any actual or intended 
// publication of such source code.
//
// $Id: odbcstat.h,v 1.13 1999/05/06 19:39:54 eduardoc Exp $
// -----------------------------------------------------------------------

#ifndef ODBCSTATUS_H
#define ODBCSTATUS_H

/**
 * This class contains status values that will be returned
 * when an operation over a ODBCPreparedStmt is performed.
 */
class StmtOp {

public:	

	enum Status {          
		CONSTRAINT_DOES_NOT_EXIST = -18,
		INDEX_ALREADY_EXISTS_WITH_SAME_FIELDS = -17,
		SERVER_GENERAL_ERROR = -16,
		CANNOT_USE_COLUMN = -15,
		DUPLICATED_KEYS = -14,
		NAME_ALREADY_EXISTS = -13,
		NO_PERMISSIONS = -12,
		TABLE_DOES_NOT_EXIST = -11,
		INDEX_ALREADY_EXISTS = -10,
		CANT_WRITE_IN_TABLESPACE = -9,
		FK_OVER_FIELDS_WITH_NON_UNIQUE = -8,
		DATABASE_ALREADY_EXISTS = -7,
		INVALID_COLUMN_NAME = -6,
		RECORD_CHECK_FAILED = -5,
		RECORD_IS_LOCKED = -4,
		NULL_PARAMETER = -3,
		UNKNOWN_ERROR = -2,
		GENERAL_ERROR = -1,
		OKEY		= 0,
	};

};

/**
 * This class contains status values that will be returned
 * when an operation over a ODBCResultSet is performed.
 */
class ResultSetOp {

public:

	enum Status {
		RSERROR		= -2,
		NO_DATA		= -1,
		OKEY		= 0,
	};
};

#endif ODBCSTATUS_H
