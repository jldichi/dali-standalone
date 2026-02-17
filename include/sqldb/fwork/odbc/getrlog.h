//- Copyright Notice
// -----------------------------------------------------------------------
// (C) Copyright 1998 InterSoft S.A.  All Rights Reserved
// THIS IS UNPUBLISHED PROPRIETARY SOURCE CODE OF InterSoft S.A.
// The copyright notice above does not evidence any actual or intended 
// publication of such source code.
//
// $Id: getrlog.h,v 1.2 1999/02/12 15:28:30 pablor Exp $
// -----------------------------------------------------------------------

#ifndef GETRLOG_H
#define GETRLOG_H

#include <ifound.h>
#include <sqldb/fwork/odbc/odbcpstm.h>
#include <sqldb/metadata/mdmisc.h>

enum GetRecordLookupMode {
	THISKEY = 0,
	NEXTKEY = 1,
	PREVKEY = 2,
	THISKEY_NEXTKEY = 3,
	THISKEY_PREVKEY = 4
};

enum GetRecordDirection {
	NO_DIR = -1,
	FORWARD = 0,
	BACKWARD = 1
};

class GetRecordAccessLogic {

public:
	GetRecordAccessLogic(enum GetRecordLookupMode initialMov, enum LockMode lmode, ODBCPreparedStmt *stmt);
	~GetRecordAccessLogic();

	inline void setLocking(enum LockMode l);
	bool moveWithDirection(enum GetRecordLookupMode movement, enum LockMode lmode);
	void setNewConfiguration(enum GetRecordLookupMode initialMov, enum LockMode lmode, ODBCPreparedStmt *pstmt);
	inline void setValid(bool ready);

	inline bool isValid() const;
	inline bool isForward() const;
	inline ODBCPreparedStmt *getStmt() const;
	inline void setStmt(ODBCPreparedStmt *stmt);
	inline Int nFetches() const;
	inline GetRecordDirection getDir() const;
	inline enum LockMode getLockingMode() const;

	// Debug
	void dump() const;

private:
	GetRecordDirection dir_d;
	Int fetchesBack_d;
	bool valid_d;
	enum LockMode locking_d;
	ODBCPreparedStmt *stmt_d;
};

#include <sqldb/fwork/odbc/getrlog.icc>

#endif GETRLOG_H
