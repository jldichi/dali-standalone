//- Copyright Notice
// -----------------------------------------------------------------------
// (C) Copyright 1998 InterSoft S.A.  All Rights Reserved
// THIS IS UNPUBLISHED PROPRIETARY SOURCE CODE OF InterSoft S.A.
// The copyright notice above does not evidence any actual or intended 
// publication of such source code.
//
// $Id: odbcpstm.h,v 1.4 1998/11/27 22:09:21 mariana Exp $
// -----------------------------------------------------------------------

#ifndef ODBCPSTMT_H
#define ODBCPSTMT_H

#include <sqldb/fwork/odbc/odbcstmt.h>
#include <sqldb/fwork/odbc/tableval.h>
#include <sqldb/fwork/odbc/odbcstat.h>

/**
 * This is class ODBCPreparedStmt. It is used to create SQL Statement that
 * do not need preparsing. It is directly executed.
 */
class ODBCPreparedStmt : public ODBCStatement {

public:
	ODBCPreparedStmt(ODBCConnectionImpl *conn, const String &stmt);
	~ODBCPreparedStmt();
	bool execute();
	ODBCResultSet &executeQuery();
	void clearParameters();

	// Seter methods.

	StmtOp::Status setParam(Int col, const CharBuffAnyValue &cbuffany);
	bool isComplete() const;

private:
};

#endif	ODBCPSTMT_H

