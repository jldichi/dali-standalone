//- Copyright Notice
// -----------------------------------------------------------------------
// (C) Copyright 1998 InterSoft S.A.  All Rights Reserved
// THIS IS UNPUBLISHED PROPRIETARY SOURCE CODE OF InterSoft S.A.
// The copyright notice above does not evidence any actual or intended 
// publication of such source code.
//
// $Id: odbctimp.h,v 1.32 1999/07/17 19:40:09 eduardoc Exp $
// -----------------------------------------------------------------------

#ifndef	ODBCTIMP_H
#define	ODBCTIMP_H

#include <sqldb/fwork/odbc/odbcstat.h>

#include <sqldb/fwork/status.h>
#include <sqldb/fwork/tabimpl.h>
#include <sqldb/fwork/odbc/tableval.h>
#include <sqldb/fwork/odbc/odbcconn.h>
#include <sqldb/fwork/odbc/pstmtmgr.h>

#include <sqldb/fwork/odbc/odbcmisc.h>

#ifdef iODBC
	#include <sqldb/fwork/odbc/isqlext.h>
#endif iODBC

#ifdef INTERSOLV
	#include <sql.h>
	#include <sqlext.h>
#endif INTERSOLV

class AnyValue; 
class IndexFieldArray;

/**
 *
 * This class is used to implement an ODBC table. It is also the implementation
 * of the abstract class TableImpl.
 * It has a TableValue field to store the values passed to the SQL driver. The
 * cfix interface is expected to ask for the data held in the tablevalue.
 * 
 * @version
 * @author Pablo Rubinstein
 * 
 * @see
 *
 */
class ODBCTableImpl : public TableImpl {
	friend class ODBCCursorImpl;

public:
	ODBCTableImpl(const TableMetadata &tmd, const ODBCConnectionImpl &conn);
	~ODBCTableImpl();	

	// Table DMS methods.
	TableOp::Status select(const IndexMetadata &imd, enum GRMode grmode,
							enum LockMode lmode = NO_LOCK);
	TableOp::Status selectIndexFields(const IndexMetadata &imd, 
							enum GRMode grmode, enum LockMode lmode);
	TableOp::Status selectPartialFields(const IndexMetadata &imd, 
					Int nFields, enum GRMode grmode, enum LockMode lmode);
	TableOp::Status insert(const IndexMetadata &imd);
	TableOp::Status insertTesting(const IndexMetadata &imd);
	TableOp::Status update(const IndexMetadata &imd);
	TableOp::Status del(const IndexMetadata &imd);
	TableOp::Status deleteAll();

	// Table DDS methods.
	// GRANT
	TableOp::Status grant(const PermissionList &pl);
	// REVOKE
	TableOp::Status revoke(const PermissionList &pl);
	// ALTER ADD
	TableOp::Status addField(const Field &f);
	// ALTER DROP
	TableOp::Status deleteField(const Field &f);
	// ALTER MODIFY
	TableOp::Status modifyField(const Field &f);
	// ALTER ADD CONSTRAINT
	TableOp::Status addTableConstraint(const Constraint &c);
	// ALTER DROP CONSTRAINT
	TableOp::Status delTableConstraint(const Constraint &c);

	Int getFld(Int fld, AnyValue &v);
	Int setFld(Int i, const AnyValue &av);

	const String &getErrorDescr() const;
	void setErrorDescr(const String &e);
	Int getNRecords();

	ODBCPreparedStmt *getSelectPreparedStatement(const IndexMetadata &idm,
					 		enum GRMode grmode, enum LockMode lmode);
	ODBCPreparedStmt *ODBCTableImpl::getSelectIndexFieldsPreparedStatement(
							const IndexMetadata& imd, enum GRMode grmode, 
							enum LockMode lmode);
	ODBCPreparedStmt *getSelectForUpdatePreparedStatement(const IndexMetadata &imd,
			 				enum GRMode grmode, enum LockMode lmode);
 	ODBCPreparedStmt *getInsertPreparedStatement();
	ODBCPreparedStmt *getDeletePreparedStatement(const IndexMetadata &idm);
	ODBCPreparedStmt *getUpdatePreparedStatement();
	ODBCPreparedStmt *getUpdateKeyPreparedStatement(const IndexMetadata &imd);

	bool isResultSetValid(const IndexMetadata &imd);
	bool isSelectIndexFieldsResultSetValid(const IndexMetadata &imd);
	bool isSelectStmtForward(const IndexMetadata &imd);
	bool isSelectIndexFieldsStmtForward(const IndexMetadata &imd);
	void setSelectStmtDirty(void);
	void setSelectIndexFieldsStmtDirty(void);
	bool addIndexToPreparedStmtManager();
  
	// Debug
	void dump() const;
	void dumpBuffers() const;

	void clearTableBuffer(); 

private:
	TableValue *tabVal_d;		// used to store the data.
	PreparedStmtManager *psm_d; //used to manage prepared statements for indices
	String errorDescr_d;		// last error message

	inline TableValue *tabVal() const;
	inline PreparedStmtManager *statementMgr() const;
	void bindStmtToTableValue(ODBCPreparedStmt *pstmt);
	void bindStmtToTableValueForIndexFields(ODBCPreparedStmt *pstmt, 
							const IndexMetadata &imd);
	TableOp::Status setStmtParameters(ODBCPreparedStmt *pstmt,
					 		const IndexFieldArray &ifa,
					 		enum GRMode grmode);

	// Internal methods to process locking modes.
	TableOp::Status updateKey(const IndexMetadata &imd);
	ODBCPreparedStmt *getUpdatePreparedStatement(const IndexMetadata &imd);
	bool selectKeyForUpdate(const IndexMetadata &imd);
	void copyOnlyIndexFieldsToImplementationBuffer(const IndexMetadata &imd);

	ODBCTableImpl(const ODBCTableImpl &);
	ODBCTableImpl &operator=(const ODBCTableImpl &);
};

#include <sqldb/fwork/odbc/odbctimp.icc>

#endif	ODBCTIMP_H
