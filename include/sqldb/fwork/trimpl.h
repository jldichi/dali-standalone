//- Copyright Notice
// -----------------------------------------------------------------------
// (C) Copyright 1998 InterSoft S.A.  All Rights Reserved
// THIS IS UNPUBLISHED PROPRIETARY SOURCE CODE OF InterSoft S.A.
// The copyright notice above does not evidence any actual or intended 
// publication of such source code.
//
// $Id: trimpl.h,v 1.9 2002/03/20 15:08:41 hardaiz Exp $
// -----------------------------------------------------------------------

#ifndef TRIMPL_H
#define TRIMPL_H

#include <sqldb/fwork/status.h>
#include <ifound/str.h>

/**
 * This class is used to represent implementation dependent transactions.
 * 
 * @version
 * @author Eduardo Chiocconi
 *
 * @see Transaction
 * @see TransactionOp
 */
class TransactionImpl {
public:	

	/**
	 * This is the Implementer Transaction constructor. This may not be the only
	 * constructor. Some others may be defined or parameters added to this one.
	 *
	 * @see TransactionOp
	 */
	TransactionImpl();

	/**
	 * This is the class destructor. It the transaction was opened (begin method
	 * was applied to Transaction object) and was not commited or rollbacked, 
	 * then it has to be commited if no error to place or rollbacked if an error
	 * ocurred.
	 *
	 * @see TransactionOp
	 */
	virtual ~TransactionImpl();

	/**
	 * This method is the one in charge of initiating a transaction. Transactions
	 * can be nested. Actually only one transaction beginning is performed. We
	 * have to keep track of the times "begin" method is called to know correctly
	 * when to commit a transaction.
	 * 
	 * @return OK if no error ocurred while beginning transaction.
	 *		   !OK if an error took place while transaction beginning. Check
	 *		   TransactionOp for error codes.
	 * 
	 * @see TransactionOp
	 */
	virtual TransactionOp::Status begin() = 0;

	/**
	 * This method is in charge of commiting a transaction. The commit action is
	 * performed only if the outer most nested transaction is reached.
	 * 
	 * @return OK if the transaction could be commited properly.
	 *		   !OK if the transaction could not be commited.
	 * 
	 * @see TransactionOp
	 */
	virtual TransactionOp::Status commit() = 0;

	/**
	 * This method is in charge of roll backing a transaction.
	 *
	 * @return OK if the transaction could be rollbacked properly.
	 *		   !OK if the transaction could not be rollbacked.
	 * 
	 * @see TransactionOp
	 */
	virtual TransactionOp::Status rollback() = 0;

	/**
	 * This method decides whether a Transaction object is in transaction or not.
	 * A transaction is "in Transaction" if a begin method was applied to the
	 * Transaction object.
	 *
	 * @return true if the Transaction is in transaction. false otherwise.
	 * 
	 */
	virtual bool inTransaction() = 0;

	/**
	 * This method decides whether an error ocurred while performing actions
	 * over the transaction.
	 * 
	 * @return true if the Transaction is in transaction. false otherwise.
	 * 
	 */
	virtual bool transactionOK() = 0;

	/**
	 * Method that returns the status of the transaction.
	 * 
	 * @return OK if no error took place in transaction.
	 *		   !OK if an error ocurred in transaction. Check TransactionOp for
	 *		   error codes.
	 * 
	 * @see TransactionOp
	 */
	inline TransactionOp::Status status(); 
	
	inline void setStatus(TransactionOp::Status stat);
	inline const String &getErrorDescr() const;

	// Debugging method
	virtual void dump() const {};

protected:
	inline void setErrorDescr(const String &err);

private:
	TransactionOp::Status transState_d;
	String errorDescr_d;

	/**
	 * These two methods are private to the class and should not be made public.
	 * They are just hidden to control object creation.
	 */
	TransactionImpl(const TransactionImpl &);
	TransactionImpl &operator=(const TransactionImpl &);
};    

#include <sqldb/fwork/trimpl.icc>

#endif	// TRIMPL_H
