//- Copyright Notice
// -----------------------------------------------------------------------
// (C) Copyright 1998 InterSoft S.A.  All Rights Reserved
// THIS IS UNPUBLISHED PROPRIETARY SOURCE CODE OF InterSoft S.A.
// The copyright notice above does not evidence any actual or intended 
// publication of such source code.
//
// $Id: odbcstmt.h,v 1.16 1999/07/01 11:01:11 eduardoc Exp $
// -----------------------------------------------------------------------

#ifndef ODBCSTMT_H
#define	ODBCSTMT_H

#include <ifound.h>
#include <ifound/str.h>

#include <sqldb/fwork/odbc/odbcrset.h>
#include <sqldb/fwork/odbc/cbuffany.h>

#ifdef iODBC
	#include <sqldb/fwork/odbc/isqlext.h>
#endif iODBC

#ifdef INTERSOLV
	#include <sql.h>
	#include <sqlext.h>
#endif INTERSOLV


class ODBCConnectionImpl;

/**
 * This is class ODBCStatement. It is used to create SQL Statement that do
 * not need preparsing. It is directly executed.
 */
class ODBCStatement {

public:
	ODBCStatement(ODBCConnectionImpl *conn, const String &stmt);
	virtual ~ODBCStatement();

	virtual bool execute();
	virtual ODBCResultSet &executeQuery();
	const ODBCResultSet &getResultSet() const;
	bool bindCol(Int col, const CharBuffAnyValue &cbuffany);
    
	inline HSTMT getStmtHandler() const;
	inline const String &getErrorDescr() const;
	inline const String &getClassErrorDescr();
	inline String &query() const;
	inline ODBCResultSet *resultSet() const;
	inline StmtOp::Status getStatus() { return currStat_d; };
	inline void setStatus(StmtOp::Status status) { currStat_d = status; };
	inline bool isVerboseOn() const;

	// Debug
	void dump() const;                  

protected:
	inline ODBCConnectionImpl *connection() const;
	void setResultSet(ODBCResultSet *resSet);
	void setErrorDescr(const String &errorDsc);
	bool checkODBCError();

private:
	ODBCConnectionImpl *conn_d;
	String stmt_d;  
	HSTMT hstmt_d;
	ODBCResultSet *resSet_d;
	String errorDescr_d;   //error description
	static String errorDescr_sd; //class error description
	StmtOp::Status currStat_d; //status 
	bool verbose_d;
};

#include <sqldb/fwork/odbc/odbcstmt.icc>

#endif	ODBCSTMT_H
