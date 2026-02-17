//- Copyright Notice
// -----------------------------------------------------------------------
// (C) Copyright 1998 InterSoft S.A.  All Rights Reserved
// THIS IS UNPUBLISHED PROPRIETARY SOURCE CODE OF InterSoft S.A.
// The copyright notice above does not evidence any actual or intended 
// publication of such source code.
//
// $Id: curimpl.h,v 1.23 2002/05/09 17:00:56 hardaiz Exp $
// -----------------------------------------------------------------------

#ifndef CURIMPL_H
#define CURIMPL_H

#include <ifound.h>

#include <sqldb/fwork/status.h>
#include <sqldb/fwork/fnamelist.h>

#include <sqldb/metadata/mdmisc.h>

#include <ifound/anyvalue.h>

class IndexMetadata;
class IndexValue;
class AnyValue;
class TableImpl;
class SQLFWTableBuffer;
struct s_bufferCFIX;

/**
 * This class MUST be derived by implementers to supply cursor implementation
 * for a table in a schema in a database. For example ODBC has its own way to
 * create cursors.
 * This is just the set of method that the implementer MUST redefine.
 *
 * It is assumed that if a set of cursors are created over the same table,
 * they all will use the same table buffer to leave the fetched registers.
 * It will be the programmer's responsability to use them properly.
 * 
 * @version
 * @author Diego Balbi & Eduardo Chiocconi.
 */
class CursorImpl {

public:	
	// Constructor & Destructor.
	/**
	 * Class constructor. This may not be the only constructor. Some others may
	 * be defined as well. It seems that IndexMetadata is the only data needed 
	 * to create a cursor under any implementation but other data may be needed.
	 * It creates a cursor object but does not open it. Cursor opening is 
	 * performed in the first fetch.
	 *
	 * @param	imd IndexMetadata of the index that will be used to create the
	 *			cursor.
	 *
	 * @see IndexMetadata
	 */
	CursorImpl(const IndexMetadata &imd, const TableImpl &timp, enum LockMode lmode);
	CursorImpl();

	/**
	 * Class destructor. Cursor members may be destroyed.
	 */
	virtual ~CursorImpl();

	// Boundary delimiter methods.
	/**
	 * This method will be used to complete cursor lower(from) limit. Limits are 
	 * not included. If "iv" has less or more values than the Index, or the index
	 * and value types are not compatible, then an error will be returned. Check
	 * CursorOp for error codes. Otherwise OK is returned.
	 * Private set methods can be used to implement this method. (eg. SetFld(...))
	 * 
	 * @param	iv is the IndexValue that contains the content for each one of the 
	 *			fields of the index over which the cursor was created. 
	 * @return	OK if cursor setting could be performed properly.
	 *			!OK if an error took place while setting lower(from) values in 
	 *			cursor.
	 * 
	 * @see	CursorOp
	 * @see	IndexValue
	 */
	virtual CursorOp::Status setFrom(const IndexValue &iv) = 0;

	/**
	 * This method will be used to complete cursor lower(from) limit. Limits are 
	 * included. If "iv" has less or more values than the Index, or the index 
	 * and value types are not compatible then an error will be returned. Check
	 * CursorOp for error codes. Otherwise OK is returned.
	 * Private set methods can be used to implement this method. (eg. SetFld(...))
	 * 
	 * @param	iv IndexValue that contains the content for each one of the fields
	 *			of the index over which the cursor was created. 
	 * @return	OK if the CursorOp
	 *			!OK if an error took place while setting lower(from) values in 
	 *			cursor.
	 * 
	 * @see	CursorOp
	 * @see	IndexValue
	 */
//	virtual CursorOp::Status setFromExclusive(const IndexValue &iv) = 0;

	/**
	 * This method will be used to complete cursor upper(to) limit. Limits are 
	 * not included. If "iv" has less or more values than the Index, or the 
	 * index and value types are not compatible then an error will be returned.
	 * Check CursorOp for error codes. Otherwise OK is returned.
	 * Private set methods can be used to implement this method. (eg. SetFld(...))
	 * 
	 * @param	iv IndexValue that contains the content for each one of the fields
	 *			of the index over which the cursor was created. 
	 * @return	OK if the CursorOp
	 *			!OK if an error took place while setting upper(to) values in 
	 *			cursor.
	 * 
	 * @see	CursorOp
	 * @see	IndexValue
	 */
	virtual CursorOp::Status setTo(const IndexValue &iv) = 0;

	/**
	 * This method will be used to complete cursor upper(to) limit. Limits are
	 * included. If "iv" has less or more values than the Index, or the index 
	 * and value types are not compatible then an error will be returned. Check
	 * CursorOp for error codes. Otherwise OK is returned.
	 * Private set methods can be used.
	 * 
	 * @param	iv IndexValue that contains the content for each one of the fields
	 *			of the index over which the cursor was created. 
	 * @return	OK if the CursorOp
	 *			!OK if an error took place while setting lower values in cursor.
	 * 
	 * @see	CursorOp
	 * @see	IndexValue
	 */
//	virtual CursorOp::Status setToExclusive(const IndexValue &iv) = 0;

	// Retrieval methods. If the status of the operation is ERR, then the
	// value of the current register will be undefined.
	/**
	 * This method is in charge of getting the next record in the cursor moving
	 * to the end of the cursor.
	 * Among the problems that can arise calling this method we have:
	 *	- No more records are available.
	 *	- An error took place while creating the cursor and it was not captured.
	 *
	 * @return	OK is the next record could be retrieved without problems.
	 *			!OK if an error took place. Check CursorOp for error codes.
	 * 
	 * @see	CursorOp
	 */
	virtual CursorOp::Status fetchNext() = 0;

	/**
	 * This method is in charge of getting the next record in the cursor moving
	 * to the beginning of the buffer.
	 * Among the problems that can arrise calling this method we have:
	 *	- No more records are available.
	 *	- An error took place while creating the cursor and it was not captured.
	 *
	 * @return	OK is the next record could be retrieved without problems.
	 *			!OK if an error took place. Check CursorOp for error codes.
	 * 
	 * @see	CursorOp
	 */
	virtual CursorOp::Status fetchPrev() = 0;

	// Cursor position methods.
	/**
	 * This method moves the cursor pointer to the beginning of the cursor. The 
	 * pointer is moved to the first record of the cursor.
	 *
	 * @return	OK if the cursor pointer could be moved to the first record of
	 *			the cursor without problems.
	 *			!OK if an error took place. Check CursorOp for error codes.
	 * 
	 * @see	CursorOp
	 */
	virtual CursorOp::Status moveToFirst() = 0;

	/**
	 * This method moves the actual cursor pointer to the end of the cursor. The 
	 * pointer is moved to the last record of the cursor.
	 *
	 * @return	OK if the cursor pointer could be moved to the last record of
	 *			the cursor without problems.
	 *			!OK if an error took place. Check CursorOp for error codes.
	 * 
	 * @see	CursorOp
	 */

	virtual CursorOp::Status moveToLast() = 0;

	/**
	 * This method returns the number of record surrounded by lower(to) and 
	 * upper(from) cursor boundaries.
	 *
	 * @return number of record in the cursor. If an error took place then a
	 *		   negative integer is returned.
	 */
	virtual Int count() = 0;

	// Methods that get parameters from prepare-statements args.
	/**
	 * This method sets the lower(from) boundary content of the i-th index field 
	 * used by the cursor.
	 *
	 * @param	fld position of the index field.
	 * @param	av contains the lower(to) boundary content for the index field 
	 *			defined by "fld" param.
	 * @return	0 if setting operation could be performed properly. Negative
	 *			integer otherwise.
	 * 
	 * @see	AnyValue
	 */
	virtual Int getFldFrom(Int fld, AnyValue &) const = 0;

	/**
	 * This method gets the upper(to) boundary content of the i-th index field 
	 * used by the cursor.
	 *
	 * @param	fld position of the index field.
	 * @param	av contains the upper(to) boundary content for the index field 
	 *			defined by "fld" param.
	 * @return	0 if setting operation could be performed properly. Negative
	 *			integer otherwise.
	 * 
	 * @see	AnyValue
	 */
	virtual Int getFldTo(Int fld, AnyValue &) const = 0;

	// Methods that should set parameters in prepared-statements args.
	/**
	 * This method gets the lower(from) boundary content of the i-th index field 
	 * used by the cursor.
	 *
	 * @param fld position of the index field.
	 * @param av contains the lower(from) boundary content for the index field 
	 *			 defined by "fld" param.
	 * @return 0 if setting operation could be performed properly. Negative
	 *			 integer otherwise.
	 * 
	 * @see AnyValue
	 */
	virtual Int setFldFrom(Int fld, const AnyValue &) = 0;

	/**
	 * This method gets the content upper(to) cursor limit of the i-th index 
	 * field used by the cursor.
	 *
	 * @param fld position of the index field.
	 * @param av contains the upper(to) boundary content for the index field 
	 *			 defined by "fld" param.
	 * @return 0 if setting operation could be performed properly. Negative
	 *			 integer otherwise.
	 * 
	 * @see AnyValue
	 */
	virtual Int setFldTo(Int fld, const AnyValue &) = 0;

	/**
 	 * This method gets the value of the i-th table field that was read during 
	 * a call to fetchXXX().
	 *
	 * NOTE: If this method is called BEFORE any call to fetchXXX(), the value
	 * 		 returned is uncertain.
	 *
	 * @param	fld is the index of the field in the table.
	 * @param	av is the AnyValue where the value must be returned.
	 *
	 * @return	0 if the operation was successful. Negative integer otherwise.
 	 */
	virtual Int getFld(Int fld, AnyValue &av) const = 0;

	/**
 	 * This method returns true when there are more records to fetch when
 	 * fetching with fetchPrev.
	 *
	 * @return	true if there are more records to fetch.
	 *			false otherwise.
 	 */
	virtual bool hasMorePrevRows() = 0;

	/**
 	 * This method returns true when there are more records to fetch when
 	 * fetching with fetchNext.
	 *
	 * @return	true if there are more records to fetch.
	 *			false otherwise.
 	 */
	virtual bool hasMoreNextRows() = 0;

	/**
	 * This method returns the locking mode that was set when creating the 
	 * cursor.
	 */
	virtual enum LockMode getLockMode() = 0;

	/**
	 * Set a value for the lower bound flag.
	 */
	void setIncludeLowerBound(bool b) { includeLower_d = b; }

	/**
	 * Set a value for the upper bound flag.
	 */
	void setIncludeUpperBound(bool b) { includeUpper_d = b; }

	/**
	 * Return if the lower bound must be included or not.
	 */
	bool hasToIncludeLowerBound() const;

	/**
	 * Return if the upper bound must be included or not.
	 */
	bool hasToIncludeUpperBound() const;

	/**
	 * This method is used to dump the content of a CursorImpl.
	 */
	virtual void dump() const;

	/**
	 * This method is used to set the fields to be retrieved by a cursor.
	 */
	inline bool setCursorFields(const FieldNameList *fnl)
				{ fieldNameList_d = (FieldNameList *)fnl;
return true; };

	/**
	 * This method returns true when all fields in the table have to be
	 * returned. If only specified fields are going to be retrieved, this
	 * method will return false.
	 */
	inline bool hasToRetrieveAllFields() const
					{ return (fieldNameList() == NULL); };

	/**
	 * This method returns the list of fields to be retrieved by the
	 * cursor.
	 */
	inline FieldNameList &getCursorFields() const
					{ return *(fieldNameList()); };

	inline FieldNameList *fieldNameList() const 
					{ return fieldNameList_d; };

	/**
	 * This method is in charge of setting the where-condition to be used when
	 * creating the cursor.
	 */
	void	setCondition(String condition);

	/**
	 * This method returns the where-condition to be used when creating the
	 * cursor.
	 */
	String	getCondition();

#ifdef	SQLFW_PERFORMANCE
	/**
	 * This method is in charge of returning the string representation of the
	 * AnyValue.
	 */
	String	getValueRep(AnyValue &value);

	/**
	 * This method creates the cursor, it is called when it is the first time
	 * that the method SQLFWCursor::fetchNext/fetchPrev is invoked.
	 */
	virtual CursorOp::Status createCursor(int direction, bool isScroll) = 0;

	/**
	 * This method fetches the first row of the resultset.
	 */
	virtual CursorOp::Status fetchFirst()   = 0;

	/**
	 * This method fetches the last row of the resultset.
	 */
	virtual CursorOp::Status fetchLast()    = 0;
#endif	// SQLFW_PERFORMANCE

	/**
	 * This method returns the error set in the implementation classes.
	 */
	virtual const String &getErrorDescr()   = 0;

	void setTableBuffer(SQLFWTableBuffer *tabBuf);

	/**
	 * These methods return the numbers of fields corresponding to the equal
	 * part of the WHERE condition, its lower bound and its upper bound
	 */
	inline	int	getNumEqual()	{ return numEqual_d; }
	inline	int	getNumFrom()	{ return numFrom_d;  }
	inline	int	getNumTo()		{ return numTo_d;    }

	/**
	 * These methods set the numbers of fields corresponding to the equal
	 * part of the WHERE condition, its lower bound and its upper bound
	 */
	inline	void	setNumEqual(int numEqual)	{ numEqual_d = numEqual; }
	inline	void	setNumFrom(int numFrom)		{ numFrom_d  = numFrom;  }
	inline	void	setNumTo(int numTo)			{ numTo_d    = numTo;    }

protected:
	SQLFWTableBuffer *tableBuffer_d;

private:
	bool includeLower_d;
	bool includeUpper_d;
	FieldNameList *fieldNameList_d;
	String	condition_d;
	const TableImpl	*tabImpl_d;

	int	numEqual_d;
	int	numFrom_d;
	int	numTo_d;

	/**
	 * These two methods are private to the class and should not be made public.
	 * They are just hidden to control object creation.
	 */
	CursorImpl(const CursorImpl &);
	CursorImpl &operator=(const CursorImpl &);

};

#endif // CURIMPL_H
