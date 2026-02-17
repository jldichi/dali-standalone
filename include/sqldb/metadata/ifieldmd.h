//- Copyright Notice
// -----------------------------------------------------------------------
// (C) Copyright 1998 InterSoft S.A.  All Rights Reserved
// THIS IS UNPUBLISHED PROPRIETARY SOURCE CODE OF InterSoft S.A.
// The copyright notice above does not evidence any actual or intended 
// publication of such source code.
//
// $Id: ifieldmd.h,v 1.11 2002/03/19 21:20:38 hardaiz Exp $
// -----------------------------------------------------------------------

#ifndef IFIELDMD_H
#define	IFIELDMD_H

#include <ideafix.h>
#include <ideafix/priv/dbdefs.h>
#include <sqldb/metadata/fieldmd.h>

class IndexField;
class SQLFWSchema;
class IndexMetadata;

/**
 * This class is used to encapsulate the an index field.
 * 
 * @version
 * @author Eduardo Chiocconi.
 * @see
 */
class IndexFieldMetadata {

public:
	IndexFieldMetadata(struct s_index_field *sif);
	~IndexFieldMetadata();

	// Set methods.
	inline void setIndFldInternalType(itype it);
	inline void setIndFldFlags(k_field_flags iff);
	inline void setIndFldLen(UChar len);
	inline void setIndFldOffset(UShort offset);
	inline void setIndFldId(dbfield fld);
	inline void setIndexFieldPosition(Int pos);
	inline void changeRep(struct s_index_field *si);
	bool store(const IndexMetadata &imd, enum StoreMode mode);
	SQLFWSchema *getSchema();

	// Get methods.
	inline struct s_index_field *indexFieldRep() const;
	inline itype internalFieldType() const;
	Short fieldType() const;
	inline k_field_flags indexFieldFlags() const;
	inline UChar indexFieldLen() const;
	inline UShort indexFieldOffset() const;
	inline dbfield indexFieldDescriptor() const;
	inline Int indexFieldPositionInTable() const;
	inline Int indexFieldPositionInIndex() const;
	char* fieldName() const;
	inline IndexField *getIndexField() const;
	bool isAscending() const;
	inline bool isNotNull() const;
	inline bool isEmpty() const;
	bool fieldBelongsToPrimaryKey() const;
	FieldMetadata *getFieldMetadata();

	void dump() const;

private:
	struct s_index_field *ifldRep_d;
	IndexField *ifield_d;
	Int indexFieldPosition_d;
	FieldMetadata *fmd_d;

	void initializeIndexField();
};

#include <sqldb/metadata/ifieldmd.icc>

#endif // IFIELDMD_H
