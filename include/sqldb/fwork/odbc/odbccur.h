//- Copyright Notice
// -----------------------------------------------------------------------
// (C) Copyright 1998 InterSoft S.A.  All Rights Reserved
// THIS IS UNPUBLISHED PROPRIETARY SOURCE CODE OF InterSoft S.A.
// The copyright notice above does not evidence any actual or intended 
// publication of such source code.
//
// $Id: odbccur.h,v 1.23 1999/06/07 20:50:13 eduardoc Exp $
// -----------------------------------------------------------------------

#ifndef ODBCCUR_H
#define ODBCCUR_H

#include <sqldb/fwork/curimpl.h>
#include <sqldb/fwork/odbc/odbcrset.h>
#include <sqldb/fwork/odbc/odbcpstm.h>
#include <sqldb/fwork/odbc/odbcstat.h>

#include <sqldb/metadata/tablemd.h>

class ODBCTableImpl;

/**
 * Enumeration used to indicate in which direction a cursor is being
 * traversed.
 */
enum Direction {
	ASCENDING_DIR = 0,
	DESCENDING_DIR = 1,
	NO_DIRECTION = 2
};

/**
 * @version
 * @author Diego Balbi & Eduardo Chiocconi.
 */
class ODBCCursorImpl : public CursorImpl {

public:	
	// Constructor & Destructor.
	ODBCCursorImpl(const IndexMetadata &imd, const ODBCTableImpl& timp, enum LockMode lmode);
	~ODBCCursorImpl();

	// Boundary delimiter methods.
	CursorOp::Status setFrom(const IndexValue &iv);
	CursorOp::Status setTo(const IndexValue &iv);

	// Retrival methods.
	CursorOp::Status fetchNext();
	CursorOp::Status fetchPrev();

	// Cursor position methods.
	CursorOp::Status moveToFirst();
	CursorOp::Status moveToLast();

	Int count();

	// Methods that get parameters from prepared-statements args.
	Int getFldFrom(Int fld, AnyValue &) const;
	Int getFldTo(Int fld, AnyValue &) const;

	// Method that returns a fetched value for a field.
	Int getFld(Int Fld, AnyValue &av) const;
	inline const String &getErrorDescr() const;
	inline const String &getClassErrorDescr();

	bool hasMorePrevRows();
	bool hasMoreNextRows();

	inline enum LockMode getLockMode();

	// Debug method.
	void dump() const;

protected:
	void setErrorDescr(const String &errorDsc);

private:
	const ODBCTableImpl *tabImpl_d;	// reference to the table
	const IndexMetadata *indexMD_d; // reference to the index
	ODBCPreparedStmt *pstmt_d;		// reference to the related prepared statement
	bool firstTime_d;				// if it's the first time a fetch method has been called.
	TableValue *fromInputValues_d;	// 'from' buffer
	TableValue *toInputValues_d;	// 'to' buffer
	String errorDescr_d;			// error description
	static String errorDescr_sd;	// class error description
	bool last_d;					// performed last operation and succeded
	bool first_d;					// performed first operation and succeded
	enum LockMode lmode_d;			// locking mode specified when creating the cursor
	ODBCPreparedStmt *pstmtlk_d;// reference to the prepared statement needed when 
								// working with Informix in order to lock the rows
	enum Direction direction_d; 	// direction in which the cursor is being traversed
	bool isEOC_d;					// the cursor is pointing to the end of data
	bool isBOC_d;                   // the cursor is pointing to the beginning of data

	inline const ODBCTableImpl *tableImpl() const;
	inline const IndexMetadata *indexMetadata() const;
	inline ODBCPreparedStmt *getCursorPStmt() const;
	inline bool isFirstTime() const;
	inline TableValue *getFromInputValues() const;
	inline TableValue *getToInputValues() const;          
	inline bool isLastLastOp() const;
	inline bool isFirstLastOp() const;
	inline enum Direction direction() const;
	inline bool isEOC() const;
	inline bool isBOC() const;
	
	// Methods that should set parameters in prepare-statements args.
	Int setFldFrom(Int fld, const AnyValue &);
	Int setFldFrom(Int col, Int fld, const AnyValue &);

	Int setFldTo(Int fld, const AnyValue &);
	Int setFldTo(Int col, Int fld, const AnyValue &);

	ODBCCursorImpl(const ODBCCursorImpl &);
	ODBCCursorImpl &operator=(const ODBCCursorImpl &);

	// Method used to set the cursor values
	CursorOp::Status init();
	void getCursorPreparedStatement();
	void bindCursorColumns(const TableMetadata &tmd);
};

#include <sqldb/fwork/odbc/odbccur.icc>

#endif ODBCCUR_H
