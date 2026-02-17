//- Copyright Notice
// -----------------------------------------------------------------------
// (C) Copyright 1998 InterSoft S.A.  All Rights Reserved
// THIS IS UNPUBLISHED PROPRIETARY SOURCE CODE OF InterSoft S.A.
// The copyright notice above does not evidence any actual or intended 
// publication of such source code.
//
// $Id: transact.h,v 1.8 2002/03/20 15:11:42 hardaiz Exp $
// -----------------------------------------------------------------------

#ifndef SQLFWTRANSACT_H
#define SQLFWTRANSACT_H

#include <sqldb/fwork/status.h>
#include <sqldb/fwork/trimpl.h>

class SQLFWTransaction {

public:	
	// Constructor & Destructor.
	SQLFWTransaction(TransactionImpl *timpl);
	~SQLFWTransaction();

	// DML methods for transactions.
	TransactionOp::Status begin();
	TransactionOp::Status commit();
	TransactionOp::Status rollback();

	// Get Information methods (transaction status).
	inline bool inTransaction() const;
	inline void setInTransaction(bool sit);
	inline bool transactionOK() const;
	static inline bool userBegTrans();
	static inline void userBegTrans(bool b);
	inline TransactionImpl *transactionImpl() const;
	bool correctStatusAsIfItHasntFailed();
	inline const String &getErrorDescr() const;
	
	// Debug
	void dump() const;

private:
	String errorDescr_d;
	TransactionImpl *transImpl_d;
	static bool userBeganTrans_d;		// User has begun a transaction.
	bool inTrans_d;						// Is in transaction.

	inline void setErrorDescr(const String &err);
	SQLFWTransaction(const SQLFWTransaction &);
	SQLFWTransaction &operator=(const SQLFWTransaction &);
};

#include <sqldb/fwork/transact.icc>

#endif // SQLFWTRANSACT_H
