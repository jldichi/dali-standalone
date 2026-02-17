//- Copyright Notice
// -----------------------------------------------------------------------
// (C) Copyright 1998 InterSoft S.A.  All Rights Reserved
// THIS IS UNPUBLISHED PROPRIETARY SOURCE CODE OF InterSoft S.A.
// The copyright notice above does not evidence any actual or intended 
// publication of such source code.
//
// $Id: sqlfwurl.h,v 1.9 2006/08/14 20:33:59 cristiand Exp $
// -----------------------------------------------------------------------

#ifndef SQLFWURL_H
#define SQLFWURL_H

#include <ifound/str.h>

#define URL_SEPARATOR	":"
#define INF_URL_SEPARATOR ";"
#define INF_FLD_SEPARATOR "="

#define DBMS_POS	0
#define SERVER_POS	1
#define SCHEMA_POS	2
#define USER_POS	3
#define PASSWD_POS	4

#define SERVER_LENGTH	16
#define UID_LENGTH		16
#define PASSWD_LENGTH	16

#define INFORMIX_DBMS_STR			"Informix"
#define ORACLE_DBMS_STR				"Oracle7"
#define POSTGRE_DBMS_STR			"Postgre"

#define NATIVE_INFORMIX_DBMS_STR	"NativeInformix"
#define NATIVE_ORACLE_DBMS_STR		"NativeOracle"
#define NATIVE_POSTGRE_DBMS_STR		"NativePostgre"

/**
 * @version
 * @author Pablo Rubinstein
 * @see ODBCConnectionImpl
 */
class SQLFWUrl {

public:	
	SQLFWUrl(const String& url);
	SQLFWUrl(const SQLFWUrl& url);
	~SQLFWUrl();

	inline const String &getOriginalUrl() const;
	inline const String &getDBMS() const;
	inline const String &getServer() const;
	inline const String &getSchema() const;
	inline const String &getUser() const;
	inline const String &getPasswd() const;

	inline void setUrlSchema(const String &sc);
	inline bool isInformixUrl() const;

	String convertToString() const;
	bool operator==(const SQLFWUrl& url) const;
	bool operator!=(const SQLFWUrl& url) const;
	// Debug
	void dump() const;

private:
	String originalUrl_d;
	String dbms_d;
	String server_d;
	String schema_d;
	String user_d;
	String passwd_d;

	// private methods
	bool parseURL(const String& url);
};

#include <sqldb/fwork/sqlfwurl.icc>

#endif // SQLFWURL_H
