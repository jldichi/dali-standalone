//- Copyright Notice
// -----------------------------------------------------------------------
// (C) Copyright 1998 InterSoft S.A.  All Rights Reserved
// THIS IS UNPUBLISHED PROPRIETARY SOURCE CODE OF InterSoft S.A.
// The copyright notice above does not evidence any actual or intended 
// publication of such source code.
//
// $Id: cursor.h,v 1.29 2002/03/19 21:14:40 hardaiz Exp $
// -----------------------------------------------------------------------

#ifndef SQLFWCURSOR_H
#define SQLFWCURSOR_H

#include <sqldb/fwork/object.h>
#include <sqldb/fwork/status.h>
#include <sqldb/fwork/fnamelist.h>
#include <sqldb/metadata/indexmd.h>
#include <ifound.h>
#include <cfix.h>
#include <sqldb/fwork/condbuil.h>

#undef DATE 

#define	IDEA_SCROLL_CURSOR	"IDEA_SCROLL_CURSOR"
class IndexMetadata;    
class TableMetadata;
class SQLFWTable;
class CursorImpl;

/**
 * This class is used to control a cursor over a table. It is used to 
 * traverse a table delimiting the range of records to be scaned.
 * 
 * @version
 * @author Diego Balbi & Eduardo Chiocconi.
 * @see CursorImpl
 */
class SQLFWCursor : public Object {

public:	
	// Constructor & Destructor.
	SQLFWCursor(const SQLFWTable &t, dbindex ind, Int lmode);
	~SQLFWCursor();

	// SQLFWCursor DML methods.
	CursorOp::Status fetchNext();
	CursorOp::Status fetchPrev();
	CursorOp::Status moveToFirst();
	CursorOp::Status moveToLast();
	Int count();
	bool hasMoreNextRows() const;
	bool hasMorePrevRows() const;

	// Get Information methods.
	inline const SQLFWTable &table() const;
	inline dbcursor cursorId() const;
	inline dbindex indexId() const;
	inline dbtable tableId() const;
	schema schemaId() const;
	inline const CursorMetadata &cursorMetadata() const;

	// Interface only used to
	void setFromAnyFld(Int fld, const AnyValue &av);
	void setFromFld(Int fld, char *buff);
	void setFromBFld(Int fld, char b);
	void setFromIFld(Int fld, Short i);
	void setFromLFld(Int fld, Int l);
	void setFromFFld(Int fld, double f);
	void setFromDFld(Int fld, DATE d);
	void setFromTFld(Int fld, TIME t);

	void setToAnyFld(Int fld, const AnyValue &av);
	void setToFld(Int fld, char *buff);
	void setToBFld(Int fld, char b);
	void setToIFld(Int fld, Short i);
	void setToLFld(Int fld, Int l);
	void setToFFld(Int fld, double f);
	void setToDFld(Int fld, DATE d);
	void setToTFld(Int fld, TIME t);

	inline void resetBoundaries();

	void setIncludeLowerBound(bool b);
	void setIncludeUpperBound(bool b);

	// Debug
	void dump() const;
	String convertToString() const;
	Object *clone() const;

	// To get the old cursor buffer 
	inline char *getOldCursorBuffer();

	inline void setErrorDescr(const String &err);
	inline String getErrorDescr() const;

private:
	dbcursor cursor_d;			// cursor descriptor.
	CursorMetadata *cursorMD_d;	// cursor metadata.
	IndexMetadata *indexMD_d;	// Index used by cursor to traverse the table.
	SQLFWTable *table_d; 		// Table used to store values in its buffer.
	CursorImpl *cursorImpl_d; 	// uses its functionality.
	bool firstTime_d;			// flag to know if the cursor has been used
	bool hasCalledMoveToFirst_d;// Whether MovetoFirst has been called and no
								// fetch operation has been performed.
	bool hasCalledMoveToLast_d; // Whether MovetoLast has been called and no
								// fetch operation has been performed.
	SQLFWCursor(const SQLFWCursor &);
	SQLFWCursor &operator=(const SQLFWCursor &);

	inline const IndexMetadata &indexMetadata() const;
	inline CursorImpl &cursorImpl() const;
	inline bool isFirstTime() const;
	inline bool hasFirstBeenCalled() const;
	inline bool hasLastBeenCalled() const;
	inline bool hasToIncludeLowerBound() const;
	inline bool hasToIncludeUpperBound() const;

	// Set range methods. They are called when we fetch records for the
	// first time.
	void setFrom();
	void setTo();

	// Method that really completes and creates a cursor metadata.
	CursorMetadata *completeCursorCreation(const TableMetadata &tmd,
											Int lmode);
	void resetCursor();
	void clearRanges(struct s_dbcur *cursor);
	void resetValues(struct s_dbcur *cursor, bool forw);
								
	// These methods get data from the cursor CFIX buffer
	AnyValue *getFromAnyFld(Int i) const;
	char *getFromFld(Int fld) const;
	char getFromBFld(Int fld) const;
	Short getFromIFld(Int fld) const;
	Int	getFromLFld(Int fld) const;
	double getFromFFld(Int fld) const;
	char* getFromDFld(Int fld) const;
	char* getFromTFld(Int fld) const;
	NUM getFromNFld(Int fld) const;

	AnyValue *getToAnyFld(Int i) const;
	char *getToFld(Int fld) const;
	char getToBFld(Int fld) const;
	Short getToIFld(Int fld) const;
	Int getToLFld(Int fld) const;
	double getToFFld(Int fld) const;
	char* getToDFld(Int fld) const;
	char* getToTFld(Int fld) const;
	NUM getToNFld(Int fld) const;

	bool isFromFldNull(Int fld, bool isAscending) const;
	bool isToFldNull(Int fld, bool isAscending) const;

	// Buffer interchange stuff.
	void copyToCFIXTableBuffer();
	// Vector buffer stuff.
	CursorOp::Status getValuesFromVectorFieldTables();

	FieldNameList *getCursorFields() const;

	// Object to get a condition
	ConditionBuilder *condBuilder_d;

	// Old buffer to be used when the cursor is created with the flag
	// IO_CONTROL_BREAK;
	char 			 *oldBuffer_d;

	// To get errors in cursors
	String errorDescr_d;
};

#include <sqldb/fwork/cursor.icc>

#endif // SQLFWCURSOR_H
