//- Copyright Notice
// -----------------------------------------------------------------------
// (C) Copyright 1998 InterSoft S.A.  All Rights Reserved
// THIS IS UNPUBLISHED PROPRIETARY SOURCE CODE OF InterSoft S.A.
// The copyright notice above does not evidence any actual or intended 
// publication of such source code.
//
// $Id: odbcrset.h,v 1.9 1998/11/17 14:16:42 mariana Exp $
// -----------------------------------------------------------------------

#ifndef ODBCRSET_H
#define ODBCRSET_H

#include <ifound.h>
#include <ifound/str.h> 
#include <sqldb/fwork/odbc/odbcconn.h>

#ifdef iODBC
	#include <sqldb/fwork/odbc/isqlext.h>
#endif iODBC

#ifdef INTERSOLV
	#include <sql.h>
	#include <sqlext.h>
#endif INTERSOLV

#define ROWS_FETCHED	1

#include <sqldb/fwork/odbc/odbcstat.h>

/**
 * This class encapsulates the ODBC Result Set. It is used to obtain
 * information from an ODBC Result Set.
 */

class ODBCResultSet {

public:
	ODBCResultSet(HSTMT hstmt, ODBCConnectionImpl *conn);
	~ODBCResultSet();

	ResultSetOp::Status next();
	ResultSetOp::Status prev(); 
	ResultSetOp::Status first();
	ResultSetOp::Status last();
	Int count();       

	bool moveTo(Int pos);
	Int getCurrentPos();

	//void clearWarnings(); //si hay algun warning se podria guardar
	//Int findColumn(const String &c);
	//String &getColumn(Int i);

	inline const String &getErrorDescr() const;
	static String &getClassErrorDescr();
	void setErrorDescr(const String &errorDsc);
	inline void setError(bool err);
	inline bool getError() const;

	inline bool isBeforeFirst();
	inline bool isAfterLast();

	void dump() const;

private:
	HSTMT hstmt_d;					// statement handler
	Int count_d;		  			// number of rows
	ODBCConnectionImpl *conn_d;		// connection
	String errorDescr_d;			// error description
	bool error_d;					// whether there is an error
	static String errorDescr_sd;	//class error description

	bool beforeFirst_d;	//true if the current position is before the 
						//first value in the result set
	bool afterLast_d; 	//true if the current position is after the
						//last value in the result set
};

#include "odbcrset.icc"

#endif	ODBCRSET_H
