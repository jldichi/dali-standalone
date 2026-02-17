/*
 * TransactionOperation.h
 *
 * Created on: 12 june. 2020
 *      Author: Diogenes Cristaldo
 */


#ifndef _TRANSACTION_OPERATION_H_
#define _TRANSACTION_OPERATION_H_

namespace auxidx{

    const char * const Begin      = "BeginTransaction";
    const char * const End        = "EndTransaction";
    const char * const TransDebug = "DBG_TRANSACTIONS";

    /**
     * Precondicion:
     * Postcondicion: Si @isTransactionProgram es
     * 	true: Se establece como inicio de transaction en el programa del cliente.
     * 	false: Se establece como fin de transaction en el programa del cliente.
     */ 
    void SetProgramTransaction(bool isTransactionProgram);

    /**
     * Precondicion:
     * Postcondicion: devuelve true si hay una transacción en curso en el programa principal
     * de lo contrario devuelve false. 
     */
    bool IsTransactionProgram();

    /**
     * Precondicion: 
     * Postcondicion: Si @isTransactionFramework es
     * 	true: Se establece como inicio de transaction en el Framework.
     * 	false: Se establece como fin de transaction en el Framework.
     */ 
    void SetFrameWorkTransaction(bool isTransactionFramework);

    /**
     * Precondicion:
     * Postcondicion: devuelve true si hay una transacción en curso en el Framework
     * de lo contrario devuelve false. 
     */
    bool IsTransactionFramework();

    /**
     * Precondicion:
     * Postcondicion: devuelve true si hay una transacción de Lockeo en Curso en FW2
     * de lo contrario devuelve false. 
     */
    bool IsTransactionLock();

    /**
     * Precondicion: 
     * Postcondicion: Si @isTransactionLock es
     * 	true: Se establece como inicio de transaction en el Framework con LockTable.
     * 	false: Se establece como fin de transaction en el Framework con FreeTable.
     */ 
    void SetLockTransaction( bool isTransactionLock);

    bool ShowMessage(char *metodo);
}

#endif /*_TRANSACTION_OPERATION_H_*/
