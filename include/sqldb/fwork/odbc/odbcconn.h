//- Copyright Notice
// -----------------------------------------------------------------------
// (C) Copyright 1998 InterSoft S.A.  All Rights Reserved
// THIS IS UNPUBLISHED PROPRIETARY SOURCE CODE OF InterSoft S.A.
// The copyright notice above does not evidence any actual or intended 
// publication of such source code.
//
// $Id: odbcconn.h,v 1.15 1999/05/03 21:53:14 mariana Exp $
// -----------------------------------------------------------------------

#ifndef ODBCCONN_H
#define ODBCCONN_H

#ifdef iODBC
	#include <sqldb/fwork/odbc/isqlext.h>
#endif iODBC

#ifdef INTERSOLV
	#ifdef _CTYPE_DEFINED //chset.h conflicts
		#undef _N
		#undef _S
		#undef _P
		#undef _C
	#endif
	#ifdef __NT__
		#include <mswi/winidea.h>
		#undef NO_DATA
		#define DATE dummyDATE
	#endif __NT__
	#include <sql.h>
	#include <sqlext.h>
	#ifdef _CTYPE_DEFINED
		#define _N	 04
		#define _S	010
		#define _P	020
		#define _C	040
	#endif
#endif INTERSOLV

#include <sqldb/fwork/connimpl.h>
#include <sqldb/fwork/sqlfwurl.h>

class ODBCPreparedStmt;
class ODBCStatement;
class ODBCTransactionImpl;            

/**
 * This class implements the abtract class ConnectionImpl. It knows how to 
 * communicate with an ODBC driver.
 * It is in charge of calling the ODBC API functions.
 * 
 * @version
 * @author Pablo Rubinstein
 * @see ConnectionImpl
 */
class ODBCConnectionImpl : public ConnectionImpl {

public:	
	ODBCConnectionImpl(const SQLFWUrl &url);
	~ODBCConnectionImpl();

	ConnectionOp::Status open();
	ConnectionOp::Status close();
	ODBCPreparedStmt *getPreparedStatement(const String &stmt) const;
	ODBCStatement *getStatement(const String &stmt) const;

	inline const String &getErrorDescr() const;
	static String &getClassErrorDescr();

	const String &getSchemaName() const;
	inline const HDBC getHdbc() const;
	inline const HENV getHenv() const;

	// Debug
	void dump() const;

private:
	HENV henv_d;			// used to allocate the environment
	HDBC hdbc_d;  			// used to allocate the connection to the ODBC driver
	String errorDescr_d;	// last error message
	bool isOpen_d;			// open flag

	// private methods
	ODBCConnectionImpl(const ODBCConnectionImpl &ci);
	ODBCConnectionImpl &operator=(const ODBCConnectionImpl &ci);
	inline bool isOpen() const;
	void ConnTranslate(const String& oper, RETCODE ret);
	void setErrorDescr(const String &errmsg);
};

#include <sqldb/fwork/odbc/odbcconn.icc>

#endif ODBCCONN_H
