//- Copyright Notice
// -----------------------------------------------------------------------
// (C) Copyright 1998 InterSoft S.A.  All Rights Reserved
// THIS IS UNPUBLISHED PROPRIETARY SOURCE CODE OF InterSoft S.A.
// The copyright notice above does not evidence any actual or intended 
// publication of such source code.
//
// $Id: odbctrim.h,v 1.4 1999/04/23 19:35:51 mariana Exp $
// -----------------------------------------------------------------------

#ifndef ODBCTRIMPL_H
#define ODBCTRIMPL_H

#include <sqldb/fwork/status.h>
#include <ifound/str.h>                

#include <sqldb/fwork/trimpl.h>

/**
 * This class is used to implement an ODBC transaction. It is also the
 * implementation of the abstract class TransactionImpl
 * 
 * @version
 * @author Mariana Tomé
 *
 * @see TransactionImpl
 */                                              
 
class TransactionImpl;
class ODBCConnectionImpl;

class ODBCTransactionImpl : public TransactionImpl {

public:	
    
    ODBCTransactionImpl();
	ODBCTransactionImpl(ODBCConnectionImpl *conn); //puedo agregar un constructor???
	~ODBCTransactionImpl();
    TransactionOp::Status begin();
	TransactionOp::Status commit();
    TransactionOp::Status rollback();
	inline bool inTransaction();
	bool transactionOK();
//	TransactionOp::Status status();
	
	inline ODBCConnectionImpl& conn() const;
	inline void setConnection(ODBCConnectionImpl *conn);

	inline enum LockMode getCurrentLockMode();
	inline enum IsoLevel getCurrentIsoLevel();
	
	TransactionOp::Status setLockMode(enum LockMode lmode);
	TransactionOp::Status setIsolationLevel(enum IsoLevel isol);

	// Debugging method
	void dump() const;

private:

	ODBCConnectionImpl *conn_d ; 
	bool inTransaction_d ; 
	enum LockMode currLockMode_d;	//current locking mode
	ODBCPreparedStmt *pstmtLk_d;	//statement used to set the locking mode
	enum IsoLevel currIsoLevel_d;	//current isolation level
	ODBCPreparedStmt *pstmtIsol_d;	//statement used to set the isolation level
	
	inline ODBCConnectionImpl *connection(); 
	
	ODBCTransactionImpl(const ODBCTransactionImpl &);
	ODBCTransactionImpl &operator=(const ODBCTransactionImpl &);
};         

#include <sqldb/fwork/odbc/odbctrim.icc>

#endif	ODBCTRIMPL_H
