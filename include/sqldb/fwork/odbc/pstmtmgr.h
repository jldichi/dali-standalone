//- Copyright Notice
// -----------------------------------------------------------------------
// (C) Copyright 1998 InterSoft S.A.  All Rights Reserved
// THIS IS UNPUBLISHED PROPRIETARY SOURCE CODE OF InterSoft S.A.
// The copyright notice above does not evidence any actual or intended 
// publication of such source code.
//
// $Id: pstmtmgr.h,v 1.9 1999/06/09 14:30:19 eduardoc Exp $
// -----------------------------------------------------------------------

#ifndef PSTMTMGR_H
#define	PSTMTMGR_H

#include <sqldb/fwork/odbc/odbcpstm.h>
#include <sqldb/fwork/odbc/getrlog.h>

class PreparedStmtManager {

public:
	PreparedStmtManager(Int n);
	~PreparedStmtManager();

	void resizeManager();

	// Geters.
	ODBCPreparedStmt *getODBCSelectPreparedStmt(Int i) const;
	ODBCPreparedStmt *getODBCSelectIndexFieldsPreparedStmt(Int n) const;
	inline ODBCPreparedStmt *getODBCInsertPreparedStmt() const;
	inline ODBCPreparedStmt *getODBCDeletePreparedStmt() const;
	inline ODBCPreparedStmt *getODBCUpdatePreparedStmt() const;
	ODBCPreparedStmt *getODBCUpdateKeyPreparedStmt(dbindex ind);

	// Seters.
	Int setODBCSelectPreparedStmt(Int i, enum GRMode grmode, enum LockMode lmode, ODBCPreparedStmt *ops);
	Int setODBCSelectIndexFieldsPreparedStmt(Int n, enum GRMode grmode, enum LockMode lmode, ODBCPreparedStmt *ops);

	inline void setODBCInsertPreparedStmt(ODBCPreparedStmt *iStmt);
	inline void setODBCDeletePreparedStmt(ODBCPreparedStmt *dStmt);
	inline void setODBCUpdatePreparedStmt(ODBCPreparedStmt *iStmt);
	void setODBCUpdateKeyPreparedStmt(dbindex ind, ODBCPreparedStmt *iStmt);

	inline void setSelectStmtDirty() { dirty_d = true; };
	inline void setSelectIndexFieldsStmtDirty() { dirtyIndFlds_d = true; };

	bool isODBCSelectPreparedStmtReady(Int n, enum GRMode grmode,
										enum LockMode lmode);
	bool isODBCSelectIndexFieldsPreparedStmtReady(Int n, enum GRMode grmode,
										enum LockMode lmode);
	inline bool isSelectStmtDirty() const { return dirty_d; };
	bool isResultSetValid(Int n) const;
	bool isSelectIndexFieldsResultSetValid(Int n) const;
	bool isSelectStmtForward(Int n) const;
	bool isSelectIndexFieldsStmtForward(Int n) const;

	inline bool isODBCInsertPreparedStmtReady() const;
	inline bool isODBCDeletePreparedStmtReady() const;
	inline bool isODBCUpdatePreparedStmtReady() const;
	bool isODBCUpdateKeyPreparedStmtReady(dbindex ind) const;

	// Debug
	void dump() const;

private:
	Int length_d;
	GetRecordAccessLogic **grLogicArray_d;
	// In this array we store select statements with the index fields
	// in the WHAT list.
	GetRecordAccessLogic **grLogicArrayWithKeyFields_d;
	ODBCPreparedStmt *insert_d;
	ODBCPreparedStmt *delete_d;
	ODBCPreparedStmt *update_d;
	ODBCPreparedStmt *updateKey_d;
	dbindex	updateKeyIndexId_d;
	bool dirty_d;
	bool dirtyIndFlds_d;
};

#include <sqldb/fwork/odbc/pstmtmgr.icc>

#endif	PSTMTMGR_H
