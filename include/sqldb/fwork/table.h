//- Copyright Notice
// -----------------------------------------------------------------------
// (C) Copyright 1998 InterSoft S.A.  All Rights Reserved
// THIS IS UNPUBLISHED PROPRIETARY SOURCE CODE OF InterSoft S.A.
// The copyright notice above does not evidence any actual or intended 
// publication of such source code.
//
// $Id: table.h,v 1.57 2002/05/08 20:10:57 hardaiz Exp $
// -----------------------------------------------------------------------

#ifndef SQLDBTABLE_H
#define SQLDBTABLE_H

#undef DATE

#include <sqldb/fwork/tabimpl.h>

class TableMetadata;
class FieldList;
class ConstraintList;
class TableOp;
class Status;
class FieldArray;
class IndexArray;
class CursorArray;
class SQLFWCursor;
class SQLFWConnection;

#ifdef DELETE
#undef DELETE
#endif

class ABMOp {
public:
	enum Op {
		INSERT = 0,
		DELETE = 1,
		UPDATE = 2,
	};
};


struct t_cache_fw {
long cache_size;
char *cachebuff;
char *currpos;
bool full;
};

/**
 * This class is used to represent SQL Tables.
 * 
 * @version
 * @author Diego Balbi & Eduardo Chiocconi
 *
 * @see TableImpl
 * @see Field
 * @see Constraint
 * @see TableOp
 * @see SQLFWIndex
 * @see SQLFWCursor
 * @see TableMetadata
 * @see PermissionList
 * @see FieldArray
 * @see IndexArray
 * @see CursorArray
 * @see IndexFieldList
 */
class SQLFWTable {

public:	
	// Constructors & Destructors.
	SQLFWTable(dbtable tab, TableImpl *tabImpl);
	~SQLFWTable();

	// Table DML methods.
	TableOp::Status select(const SQLFWIndex &index, enum GRMode grmode,
							enum LockMode lmode = NO_LOCK);	// without cursor.
	TableOp::Status selectIndexFields(const SQLFWIndex &ind, enum GRMode grmode,
							enum LockMode lmode = NO_LOCK);
	TableOp::Status selectPartialFields(const SQLFWIndex &ind, Int nFields,
							enum GRMode grmode,

#ifdef __VC__
	enum LockMode lmode);
#else
	enum LockMode lmode = NO_LOCK);
#endif

	TableOp::Status selectPartialIndexFields(const SQLFWIndex &ind, Int nFields,
							enum GRMode grmode,
#ifdef __VC__
	enum LockMode lmode);
#else
	enum LockMode lmode = NO_LOCK);
#endif
	TableOp::Status insert();					// without cursor.
	// This method is added because is different the insertion in a temporary table
	TableOp::Status insertIntoTemporaryTable(); // without cursor.
	TableOp::Status insertTesting();			// without cursors.
	TableOp::Status update();					// without cursors.
	TableOp::Status updateTmpTable(SQLFWTable *fromtab);
	TableOp::Status exists(enum GRMode grmode, enum LockMode lmode = NO_LOCK);		// without cursors.
	TableOp::Status existIntoTmpTable(SQLFWTable *tmptable, enum GRMode grmode, enum LockMode lmode = NO_LOCK);
	TableOp::Status del();						// without cursors.
	TableOp::Status deleteAll();
	Int getNRecords() const;
	void cleanBuffer();

	// Get Information methods.
	inline dbtable getId() const;
	inline const char *tabName() const;
	const char *schName() const;
	inline Int nFields() const;
	inline Int nIndexes() const;
	inline const FieldArray &fields() const;
	inline const IndexArray &indexes() const;
	inline const CursorArray &cursors() const;
	inline const SQLFWIndex &getPrimaryKey() const;
	inline const SQLFWIndex &getIndex(dbindex indId) const;
	inline const TableMetadata &getTableMetadata() const;
	SQLFWConnection *getConnection() const;
	inline TableMetadata *tableMetadata() const;
	dbindex findIndex(const String &indexName) const;

	// Table buffer manipulation methods.
	bool copyEqualFieldContents(const SQLFWTable *fromTab);
	bool copyEqualFieldContentsToImplementationBuffer(const SQLFWTable *fromTab);

   	// Set field buffer.
	Int setAnyFld(Int fld, const AnyValue &av, Int row = 0,
					bool convertDecimal = true);
	Int setFld(Int fld, const char *buff, Int row = 0);
	Int setBFld(Int fld, char b, Int row = 0);
	Int setIFld(Int fld, Short i, Int row = 0);
	Int setLFld(Int fld, Int l, Int row = 0);
	Int setFFld(Int fld, double f, Int row = 0);
	inline Int setDFld(Int fld, DATE d, Int row = 0);
	inline Int setTFld(Int fld, TIME t, Int row = 0);
	Int setNFld(Int fld, NUM n, Int row= 0);

	// Get field buffer.
	AnyValue *getAnyFld(Int fld, Int row = 0) const;
	char *getFld(Int fld, Int row = 0) const;
	char getBFld(Int fld, Int row = 0) const;
	Short getIFld(Int fld, Int row = 0) const;
	Int getLFld(Int fld, Int row = 0) const;
	double getFFld(Int fld, Int row = 0) const;
	char* getDFld(Int fld, Int row = 0) const;
	char* getTFld(Int fld, Int row = 0) const;
	NUM getNFld(Int fld, Int row = 0) const;

/* toDo: definir
	Int setBLOB(Int fld, ??) = 0;
	Int getBLOB(Int fld, ??) = 0;

	Int setBLOB(Int fld, Int row, ??) = 0;
	Int getBLOB(Int fld, Int row, ??) = 0;
*/

	// Cursor methods.
	/**
	 * Cursor returned, when fetched, will set data in buffer corresponding
	 * to Table instance that created it.
	 */
	// CREATE CURSOR
	const SQLFWCursor &createCursor(const SQLFWIndex& ind, Int lockMode);
	bool deleteCursor(dbcursor cursorId);
	bool deleteIndex(dbindex indexId);

	// Error methods.
	inline void setErrorDescr(const String &err);
	inline String getErrorDescr() const;
	String getClassErrorDescr() const;
	
	// Debug
	void dump() const;
	void dumpTableBuffer() const;
	String getKeyValues() const;
	inline TableImpl *tableImpl() const;

	// Vector field stuff.
	bool selectFromVectorTables(const class SQLFWIndex &);
	bool selectFromVectorTables2(const class SQLFWIndex &);
	bool hasAVectorField() const;


	bool haveTableCache();
	int setTableCache(int size);
	void addToTableCache();
	int removeFromTableCache(dbindex idx);
	int  getFromTableCache(dbindex idx);
protected:

private:
	dbtable tabId_d;
	TableImpl *tabImpl_d;
	TableMetadata *tabMD_d;
	static String errorDescr_sd;
	String errorDescr_d;
	struct t_cache_fw *cache_d;

	int recordLen();
	char *buffer();

	void setImplementationBufferComplete(enum StoreMode mode); // Parameter added
	Int  setTemporaryImplementationBuffer(const SQLFWIndex &index, SQLFWTable *tmptable);	
	Int  setImplementationBuffer(const SQLFWIndex &index);
	void getFromImplementationBuffer();
	void getIndexFieldsFromImplementationBuffer(const IndexMetadata &imd);
	TableOp::Status selectTemporaryIndexTable(const IndexMetadata *indmd, enum GRMode grmode, enum LockMode lmode);
	TableOp::Status selectPartialTemporaryIndexTable(const IndexMetadata *indmd, Int nFields, enum GRMode grmode, enum LockMode lmode);
	TableOp::Status selectIndexTemporaryIndexTable(const IndexMetadata *indmd, enum GRMode grmode, enum LockMode lmode);
	void copyFieldContent(const FieldMetadata *fmdFromTab, const FieldMetadata *fmdToTab, const SQLFWTable *fromTab);
	void copyFieldContentToImplementationBuffer(const FieldMetadata *fmdFromTab, const FieldMetadata *fmdToTab, const SQLFWTable *fromTab);
	void updateTemporaryTables();

	void resolveTimeStamps(tab_flags tflags, field_flags fflags);
	AnyValue *getMinInsteadOfNull(dbfield fld) const;
	AnyValue getCFIXNullForField(const FieldMetadata &fmd);
	bool isAlias() const;

	// Vector field stuff.
	bool ABMInVectorTables(const SQLFWIndex &pk, ABMOp::Op op);
	bool ABMInVectorTables2(const SQLFWIndex &pk, ABMOp::Op op);
	bool getVectorTableFieldValues(SQLFWTable *vecTab, const SQLFWIndex &pk, const FieldMetadata &fmd, int n);
	bool getVectorTableFieldValues2(SQLFWTable *vecTab, const SQLFWIndex &pk, const FieldMetadata &fmd, int begRng, int endRng);
	void setVectorTableFieldsOfPrimaryKey(const SQLFWIndex &pk, SQLFWTable *vecTab, int n);
	void setVectorTableValues(const SQLFWIndex &pk, SQLFWTable *vecTab, const FieldMetadata &fmd, int n);
	void setVectorTableValues2(const SQLFWIndex &pk, SQLFWTable *vecTab, const FieldMetadata &fmd);

	SQLFWTable(const SQLFWTable &);
	SQLFWTable &operator=(const SQLFWTable &);
};

#include <sqldb/fwork/table.icc>

#endif // SQLDBTABLE_H
