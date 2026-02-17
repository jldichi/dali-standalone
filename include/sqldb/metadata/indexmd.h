//- Copyright Notice
// -----------------------------------------------------------------------
// (C) Copyright 1998 InterSoft S.A.  All Rights Reserved
// THIS IS UNPUBLISHED PROPRIETARY SOURCE CODE OF InterSoft S.A.
// The copyright notice above does not evidence any actual or intended 
// publication of such source code.
//
// $Id: indexmd.h,v 1.20 2002/03/19 21:23:37 hardaiz Exp $
// -----------------------------------------------------------------------

#ifndef INDEXMD_H
#define	INDEXMD_H

#include <ideafix.h>
#include <ideafix/priv/dbdefs.h>

#include <sqldb/metadata/ifarray.h>

#include <sqldb/fwork/ifieldlist.h>
#include <sqldb/fwork/status.h>                       

class IndexField;
class TableMetadata;

#define INVALID_NODE_LENGTH		MAX_SHORT

/**
 * This class is used to store the information related to an index.
 * 
 * @author Eduardo Chiocconi.
 */
class IndexMetadata {

public:
	IndexMetadata(struct s_index* st, dbindex indid = ERR);
	~IndexMetadata();

	// Set methods.
	inline void setIndId(dbindex indid);
	inline void setIndName(const char *name);
	inline void setIndFlags(key_flags kf);
	inline void setIndLen(UShort len);
	inline void setIndNodelen(UShort len);
	inline void setIndNFlds(UShort nFlds);
	inline void setIndexTable(const TableMetadata &tmd);
	inline void setIndexFlds(const IndexFieldArray &ifa);
	void setIndex(const SQLFWIndex &index);
	inline void changeRep(struct s_index *st);
	inline void setComplete();
	inline void setIsPrimaryKey();
	inline void setHasBeenCreatedWithCompleteIndex();
	inline void setTableDescrOfIndexCreatedWithCI(dbtable tab);
	bool store(enum StoreMode mode);
	inline void setFrameWorkName(const String &name);
	inline void setHasUniquePhysicalIndex(bool hpi);

	// Get methods.
	inline dbindex getId() const;
	inline struct s_index *indexRep() const;
	inline char *nameOfIndex() const;
	inline key_flags indexFlags() const;
	inline UShort indexLen() const;
	inline UShort nFields() const;
	inline struct s_table *indexOfTable() const;
	inline const TableMetadata &getTableMetadata() const;
	inline const IndexFieldArray &indexFields() const;
	SQLFWIndex *getIndex() const;
	inline const IndexFieldMetadata &getIndexFieldMD(Int n) const;
	inline const IndexField &getIndexField(Int n) const;
	FieldMetadata &getFieldMetadataOfIndexField(Int n) const;
	inline bool isComplete() const;
	inline bool isTemporary() const;
	inline bool isUnique() const;
	inline bool isPrimaryKey() const;
	inline bool hasIndexBeenCreatedWithCompleteIndex() const;
	inline dbtable getTableDescrOfIndexCreatedWithCI() const;
	inline String tableNameOfIndex() const;
	SQLFWSchema *getSchema() const;
	inline String frameWorkName() const;
	inline bool getHasUniquePhysicalIndex() const;
	IndexConstraint::Constraint getIndexConstraint() const;
	bool fieldExists(const String &indFldName) const;
	inline void setHasANotNullFieldInUniqueIndex();
	inline bool hasANotNullFieldInUniqueIndex() const;
	inline bool canCreateIndexInUnderlyingDBMS() const;

	static String buildIndexName(const String &tabName, const char *indName);

	IndexFieldList *toIndexFieldList() const;
	void dump() const;

	// Methods to build, set and get the where-condition to filter records when
	// the index has fields having not null as an attribute
	void buildIndexCondition();
	inline bool isIndexConditionSet() const;
	inline String getIndexCondition() const;

private:
	struct s_index *indRep_d;
	IndexFieldArray *indexFieldArray_d;
	dbindex indId_d;
	bool isComplete_d;		// indicates whether CompleteIndex() has been called 
	bool isPrimaryKey_d;	// Whether the index is a primary key or not.
	bool hasBeenCreatedWithCompleteIndex_d;	// Indicate if the index has
							// been created with CompleteIndex CFIX function.
	String frameWorkName_d;
	Int complIndTabDescr_d;	// Table description
	bool hasUniquePhysicalIndex_d;
							// This member is used to know whether this
							// index has a related physical index in the
							// underlying DBMS. It can happen that there
							// is already an index, indexing the columns
							// selected by this index and CREATE INDEX sql
  							// stmt fails when executed.
	bool hasANotNullField_d;// This field is set when the index is being
							// created and one of the index fields has a not
							// null clause. In this case, the index may have
							// less records than the index, so we should store
							// it in metadata schema but not launch the create
							// index SQL statement over the schema.
	String indexCondition_d;	// Where condition to filter records using the
								// index
	IndexMetadata(const IndexMetadata &imd);	
	void initializeSIndex();
};

#include <sqldb/metadata/indexmd.icc>

#endif	// INDEXMD_H
