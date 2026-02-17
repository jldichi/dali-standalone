//- Copyright Notice
// -----------------------------------------------------------------------
// (C) Copyright 1998 InterSoft S.A.  All Rights Reserved
// THIS IS UNPUBLISHED PROPRIETARY SOURCE CODE OF InterSoft S.A.
// The copyright notice above does not evidence any actual or intended 
// publication of such source code.
//
// $Id: tabimpl.h,v 1.30 2004/09/16 21:37:42 damianm Exp $
// -----------------------------------------------------------------------

#ifndef TABIMPL_H
#define TABIMPL_H

#define	FULL_KEY	-1

#include <sqldb/metadata/sqldb.h>

#include <sqldb/fwork/status.h>
#include <sqldb/fwork/ifield.h>

class PermissionList;
class Field;
class Constraint;
class IndexFieldList;
class IndexMetadata;
class TableMetadata;
class ConnectionImpl;
class CursorImpl;
class SQLFWIndex;
class FieldNameList;
class SQLFWTableBuffer;

/**
 * This class is used to represent table dependent implementation.
 * 
 * @version
 * @author Eduardo Chiocconi
 *
 * @see TableOp
 * @see SQLFWIndex
 * @see Field
 * @see AnyValue
 * @see Constraint
 * @see FieldList
 * @see ConstraintList
 * @see PermissionList
 * @see IndexFieldList
 * 
 */
class TableImpl {

public:	
	/**
	 * This is the class constructor. It creates a TableImpl object.
	 * This constructor is used when a table exists in database.
	 *
	 * @param	tmd is the Table metadata. This metadata can be usefull for
	 *			table implementation.
	 * @param	conn is the connection used to set and get data to and from
	 *			the database.
	 * 
	 * @see TableOp
	 */
	TableImpl(const TableMetadata &tmd, const ConnectionImpl &conn);

	/**
	 * This is the class destructor.
	 */
	virtual ~TableImpl();

	// Table DDS methods.

	// GRANT
	/**
	 * These are the permission that have to be set (granted) over the table.
	 * This table method represents sql GRANT statement.
	 * 
	 * @param pl is the permission list of the table.
	 * @return OK is permission could be granted properly.
	 *		   !OK if an error took place. Check TableOp for error codes.
	 * 
	 * @see TableOp
	 * @see Permission
	 * @see PermissionList
	 */
	virtual TableOp::Status grant(const PermissionList &pl) = 0;

	// REVOKE
	/**
	 * These are the permission that have to be removed (revoked) over the table.
	 * This table method represents sql REVOKE statement.
	 * 
	 * @param pl is the permission list of the table.
	 * @return OK is permission could be granted properly.
	 *		   !OK if an error took place. Check TableOp for error codes.
	 * 
	 * @see TableOp
	 * @see Permission
	 * @see PermissionList
	 */
	virtual TableOp::Status revoke(const PermissionList &pl) = 0;

	// ALTER ADD
	/**
	 * This method adds a new field to a database. This method represents
	 * sql ALTER TABLE 'T' ADD ... statement.
	 * 
	 * @param f is the field to be added to the end of the table.
	 * @return OK if field could be added properly to the table.
	 *		   !OK if the field could not be added to the table. Check TableOp
	 *		   for error codes.
	 * 
	 * @see Field
	 * @see TableOp
	 */
	virtual TableOp::Status addField(const Field &f) = 0;

	// ALTER DROP
	/**
	 * This method removes an existing field from a table. This method represents
	 * sql ALTER TABLE 'T' DROP ... statement.
	 * 
	 * @param f is the field to be removed from the table.
	 * @return OK if field could be removed properly from table.
	 *		   !OK if the field could not be deleted from table. Check TableOp
	 *		   for error codes.
	 * 
	 * @see Field
	 * @see TableOp
	 */
	virtual TableOp::Status deleteField(const Field &f) = 0;

	// ALTER MODIFY
	/**
	 * This method modifies an existing field in a table. This method represents
	 * sql ALTER TABLE 'T' MODIFY ... statement.
	 * 
	 * @param f is the field to be removed from the table.
	 * @return OK if field could be removed properly from table.
	 *		   !OK if the field could not be deleted from table. Check TableOp
	 *		   for error codes.
	 * 
	 * @see Field
	 * @see TableOp
	 */
	virtual TableOp::Status modifyField(const Field &f) = 0;

	// ALTER ADD CONSTRAINT
	/**
	 * This method adds a table constraint to an existing table or a table that
	 * is being created. Among table constraints we find Primary keys, Foreign
	 * key, ...
	 *
	 * @param c is the table constraint to be added to the table.
	 * @return OK if table constraint could be added to table.
	 *		   !OK if an error ocurred while adding the constraint. Check TableOp
	 *		   for error codes.
	 * 
	 * @see TableConstraint
	 * @see TableOp
	 */
	virtual TableOp::Status addTableConstraint(const Constraint &c) = 0;

	// ALTER DROP CONSTRAINT
	/**
	 * This method removes a table constraint from an existing table or a table 
	 * that is being created. Among table constraints we find Primary keys, 
	 * Foreign key, ...
	 *
	 * @param c is the table constraint to be removed from table.
	 * @return OK if table constraint could be removed to table.
	 *		   !OK if an error ocurred while adding the constraint. Check TableOp
	 *		   for error codes.
	 * 
	 * @see TableConstraint
	 * @see TableOp
	 */
	virtual TableOp::Status delTableConstraint(const Constraint &c) = 0;
    
    // Table DMS methods.
	/**
	 * Method that perfoms record retrival from database. This method should be
	 * called once the fields of the table have been prepared for table select 
	 * method. This table method represents sql SELECT ... WHERE statement.
	 *
	 * @param	idm is the index to be used to build the statement.
	 * @param	grmode is the acess mode. It can be one of the following
	 *			values: GR_THIS_KEY, GR_NEXT_KEY, GR_PREV_KEY,
	 *					GR_THIS_KEY_NEXT_KEY, GR_THIS_KEY_PREV_KEY.
	 * @param	lmode is the locking mode. It can be one of the 
	 *			following values: LOCK, LOCK_TEST, NO_LOCK. 
	 * 
	 * @return OK if no error took place while performing select action.
	 *		   !OK if an error ocurred. Check TableOp for error codes.
	 * 
	 * @see TableOp
	 */
	virtual TableOp::Status select(const IndexMetadata &idm, enum GRMode grmode,
									enum LockMode lmode) = 0;

	/**
	 * Method that perfoms record retrival from database. This method should be
	 * called once the fields of the table have been prepared for table select 
	 * method. This table method represents sql SELECT ... WHERE statement.
	 * This method is used for performance issues. As it only brings the
	 * fields specified in the first argument of the method.
	 *
	 * @param	fnl is the list of fields to be retrieved by this operation.
	 *			Only these fields should be returned.
	 * @param	grmode is the acess mode. It can be one of the following
	 *			values: GR_THIS_KEY, GR_NEXT_KEY, GR_PREV_KEY,
	 *					GR_THIS_KEY_NEXT_KEY, GR_THIS_KEY_PREV_KEY.
	 * @param	lmode is the locking mode. It can be one of the 
	 *			following values: LOCK, LOCK_TEST, NO_LOCK. 
	 * 
	 * @return OK if no error took place while performing select action.
	 *		   !OK if an error ocurred. Check TableOp for error codes.
	 * 
	 * @see TableOp
	 */
	virtual TableOp::Status selectIndexFields(const IndexMetadata &imd, 
							enum GRMode grmode,	enum LockMode lmode) = 0;

	/**
	 * Method that perfoms record retrival from database. This method should be
	 * called once the fields of the table have been prepared for table select 
	 * method. This table method represents sql SELECT ... WHERE statement.
	 * This method is used for performance issues. As it only brings the
	 * fields specified in the first argument of the method.
	 * This method differs from other select method in the form the WHERE 
	 * clause condition is constructed. The first "n" (nFields parameter) should
	 * be equal to the value specified in the table buffer. The other ones should
	 * respect select retrieval mode. This method is only used by GetRecord
	 * CFIX function with PARTIAL_KEY set in the access mode.
	 *
	 * @param	fnl is the list of fields to be retrieved by this operation.
	 *			Only these fields should be returned.
	 * @param	nFields is the number of firsts fields that must be equal
	 *			to the values in the buffer.
	 * @param	grmode is the acess mode. It can be one of the following
	 *			values: GR_THIS_KEY, GR_NEXT_KEY, GR_PREV_KEY,
	 *					GR_THIS_KEY_NEXT_KEY, GR_THIS_KEY_PREV_KEY.
	 * @param	lmode is the locking mode. It can be one of the 
	 *			following values: LOCK, LOCK_TEST, NO_LOCK. 
	 * 
	 * @return OK if no error took place while performing select action.
	 *		   !OK if an error ocurred. Check TableOp for error codes.
	 * 
	 * @see TableOp
	 */
	virtual TableOp::Status selectPartialFields(const IndexMetadata &imd, 
		Int nFields, enum GRMode grmode, enum LockMode lmode) = 0;

	/**
	 * Method that perfoms record insertion in a table. This method should be
	 * called once the fields of the table have been prepared for table insert 
	 * method. This table method represents sql INSERT INTO ... WHERE ...
	 * statement.
	 * If the record already exist, then the operation will corrupt the
	 * transaction and the status RECORD_EXISTS will be returned.
	 * If the operation could be completed successfully (the insertion
	 * could be performed without errors) the method has to return OKEY.
	 * If the record violates at least one table constraint or a field
	 * constraint, the operation should fail and we should return the
	 * status CONSTRAINT_VIOLATED_IN_INSERT.
	 *
	 * @param idm is the index to be used to build the statement.
	 * 
	 * @return	one of the status described above.
	 * 
	 * @see TableOp
	 */
	virtual TableOp::Status insert(const IndexMetadata &idm) = 0;

	/**
	 * Method that perfoms record insertion in a table. This method should be
	 * called once the fields of the table have been prepared for table insert 
	 * method. This table method represents sql INSERT INTO ... WHERE ... 
	 * statement.
	 * The difference with insert sql statement is that if the insertion fails,
	 * the transaction is not corrupted.
	 *
	 * @param idm is the index to be used to build the statement.
	 * 
	 * @return OK if no error took place while performing insert action.
	 *		   !OK if an error ocurred. Check TableOp for error codes.
	 * 
	 * @see TableOp
	 */
	virtual TableOp::Status insertTesting(const IndexMetadata &idm) = 0;

	/**
	 * Method that perfoms record updates in a table. This method should be
	 * called once the fields of the table have been prepared for table update
	 * method. This table method represents sql UPDATE ... statement.
	 * If we are trying to update a record that is being locked, then
	 * the operation should fail and RECORD_IS_LOCKED status should be
	 * returned.
	 * 
	 * @param idm is the index to be used to build the statement.
	 * 
	 * @return OK if no error took place while performing update action.
	 *		   !OK if an error ocurred. Check TableOp for error codes.
	 * 
	 * @see TableOp
	 */
	virtual TableOp::Status update(const IndexMetadata &idm) = 0;

	/**
	 * Method that perfoms record deletion from a table. This method should be
	 * called once the fields of the table have been prepared for table deletion
	 * method. This table method represents sql DELETE FROM ... WHERE statement.
	 * If we are trying to delete a record that is being locked, then
	 * the operation should fail and RECORD_IS_LOCKED status should be
	 * returned.
	 * If the record is being referenced from another table, the record will
	 * not be deleted and the operation will fail. We should return 
	 * RECORD_IS_BEING_REFERENCED_FROM_ANOTHER_TABLE.
	 * 
	 * @param idm is the index to be used to build the statement.
	 * 
	 * @return OK if no error took place while performing delete action.
	 *		   !OK if an error ocurred. Check TableOp for error codes.
	 * 
	 * @see TableOp
	 */
	virtual TableOp::Status del(const IndexMetadata &idm) = 0;

	/**
	 * Method that deletes all records from a database.
	 * method. This table method represents sql DELETE FROM ... statement.
	 * Note that we do not specify the index because we will probably use the PK.
	 * 
	 * @return OK if no error took place while performing delete action.
	 *		   !OK if an error ocurred. Check TableOp for error codes.
	 * 
	 * @see TableOp
	 */
	virtual TableOp::Status deleteAll() = 0;

	// Needed to perform a lock operation over a new key we are generating in an insert.
	/**
	 * Method that perfoms record existence testing at database. This method should be
	 * called once the fields of the table have been prepared for table select 
	 * method. This table method represents sql SELECT ... WHERE statement.
	 * This method will be called to determine if we have to do an insert or an update.
	 * We need to be sure that 2 processes accesing the same key will stay at this method
     * locked until first of them finish transactions, to avoid insert problems
	 * We will provide a default implementation as a select with THIS_KEY and IO_LOCK, but
	 * probably we need to redefine this at drivers level.
	 *
	 * @param	idm is the index to be used to build the statement.
	 * 
	 * @return OK if there is a record for this key
	 *		   !OK otherwise.
	 * 
	 * @see TableOp
	 */
	virtual TableOp::Status  exists(const IndexMetadata &idm); 

	/**
	 * Method that returns the number of records in a table. It represents sql
	 * SELECT COUNT(*) ... statement.
	 * 
	 * @return Positive integer if no error took place. A negative value 
	 *		   otherwise.
	 */
	virtual Int getNRecords() = 0;
    
    // Get Information methods.
	/**
	 * Method that returns CFIX table descriptor over which the TableImpl oject
	 * was created.
	 * 
	 * @return descriptor of the table over which the TableImpl object was 
	 *		   created.
	 * 
	 * @see dbtable
	 */
	virtual dbtable getId() const;

	/**
	 * Method that returns the TableMetadata associated to the TableImpl.
	 * 
	 * @return the TableMetadata object associated when TableImpl was created.
	 * 
	 * @see TableMetadata
	 */
	inline const TableMetadata &tableMetadata() const
	{
		assert(tableMD_d);
		return *tableMD_d;
	}

	// Table buffer manipulation methods.
	// Set single & vector field buffer.
	/**
	 * This method sets the field buffer for a table field.
	 * 
	 * @param fld field position in table.
	 * @param v is the value that we want to assign to the field "fld".
	 * @return 0 if field buffer setting could be performed properly. Negative
	 *		   integer otherwise.
	 * 
	 * @see AnyValue
	 */
	virtual Int setFld(Int fld, const AnyValue &v) = 0;

	virtual Int setFld(Int fld,int pos, const AnyValue &v) = 0;

	// Get single & vector field buffer.
	/**
	 * This method gets the field buffer for a table field.
	 * 
	 * @param fld field position in table.
	 * @param v will hold the value in field "fld" buffer.
	 * @return 0 if field buffer retrival could be performed properly. Negative
	 *		   integer otherwise.
	 * 
	 * @see AnyValue
	 */
	virtual Int getFld(Int fld, AnyValue &v) = 0;

	virtual Int getFld(Int fld,int pos, AnyValue &v) = 0;

/* toDo: define
	virtual Int setBLOB(Int fld, ??) = 0;
	virtual Int getBLOB(Int fld, ??) = 0;

*/

	/**
	 * This method initializes the buffer of the Implementation Table.
	 * All fields must be cleaned up.
	 */                              
	virtual void clearTableBuffer() = 0; 
	 
	/**
	 * Method that returns an error string if an error took place and NULL_STRING
	 * if no error was produced.
	 * 
	 * @return NULL_STRING if no error took place and a non NULL_STRING string
	 *		   otherwise.
	 */
	virtual const String &getErrorDescr() const = 0;

	// Debug	
	virtual void dump() const = 0;
	virtual void dumpBuffers() const { warning("TableImpl:::dumpBuffers"); };
	virtual inline void setSelectStmtDirty(void) {};

	/**
	* sets CFIX buffer to output operations
	*/
	void setTableBuffer(SQLFWTableBuffer *tabBuf);

protected:
	TableMetadata *tableMD_d;

	/**
	 * This method is used to get the connection associated to this table
	 * implementation.
	 *
	 * @return    the Connection used for the table implementation.
	 *
	 * @see ConnectionImpl
	 */
	const ConnectionImpl &connection() const;

	/**
	 * This method is used to get the TableMetadata attached to the table
	 * implementation.
	 *
	 * @return	TableMetadata associated with this table implementation.
	 * 
	 * @see ConnectionImpl
	 */
	inline const TableMetadata &getTableMetadata() const { assert(tableMD_d); return *tableMD_d; }

	/**
	 * This is the default constructor and should not be used.
	 */
	TableImpl();

protected:

	/**
	 * These two methods are private to the class and should not be made public.
	 * They are just hidden to control object creation.
	 */
	TableImpl(const TableImpl &);
	TableImpl &operator=(const TableImpl &);

	SQLFWTableBuffer *tableBuffer_d;
};

#endif //TABIMPL_H

