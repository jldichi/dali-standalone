//- Copyright Notice
// -----------------------------------------------------------------------
// (C) Copyright 1998 InterSoft S.A.  All Rights Reserved
// THIS IS UNPUBLISHED PROPRIETARY SOURCE CODE OF InterSoft S.A.
// The copyright notice above does not evidence any actual or intended 
// publication of such source code.
//
// $Id: fieldmd.h,v 1.22 2002/07/31 19:03:52 albertoi Exp $
// -----------------------------------------------------------------------

#ifndef FIELDMD_H
#define	FIELDMD_H

#include <ideafix.h>
#include <ideafix/priv/dbdefs.h>
#include <ideafix/priv/dbexpr.h>
#include <ideafix/priv/dbinchk.h>
#include <essentia/objectid.h>
#include <string.h>

#include <sqldb/metadata/mdmisc.h>

class TableMetadata;
class AliasDictionary;
class SQLFWTable;

/**
 * This class is used to encapsulate field metadata. The underlying 
 * structure protected is s_field structure. An object of this class is
 * created from an allocated pointer to an s_field structure (this pointer
 * may be taken from an allocated array of s_field).
 * 
 * @version
 * @author Eduardo Chiocconi.
 */
class FieldMetadata {

public:
	FieldMetadata(struct s_field *sf, dbfield fid);
	~FieldMetadata();

	// Set methods.
	// Set Dictionary methods.
	inline void setFldId(dbfield id);
	void setFldName(char *name);
	void setFldDescr(const char *descr);
	inline void setFldDim(UShort dim);
	inline void setFldOffset(UShort offset);
	inline void setFldType(type t);
	inline void setFldInternalType(itype it);
	inline void setFldSqlType(sqltype st);
	inline void setFldFlags(field_flags ff);
	inline void setFldLen(UShort flen);
	inline void setFldNDec(UShort nDec);
	void setFldMask(const char *mask);
	void setFldRelCheck(const char *relcheck);
	void setFldInCheck(const char *ic);
	void setFldDefault(const char *def);
	void setFldCompiledMask(const char *cmask);
	void setFldValidateExpr(STRUCT DbExpr *ve);
	void setFldDefaultExpr(STRUCT DbExpr *de);
	void setFldInCheckExpr(STRUCT InCheck *inchk);
	bool store(const String &tableName, enum StoreMode mode);
	bool storeInCheck(const String &tableName, enum StoreMode mode);
	inline void setFieldPos(Int pos);
	inline void setValueForNullValue(const AnyValue &newNullValue);
	void setVectorTable(const SQLFWTable &table);
	void setVectorAliasTable(dbtable tab, const SQLFWTable &table);
	inline void setIsInPrimaryKey(bool isInPrimaryKey);

	// Get methods.	
	inline struct s_field *fieldRep() const;
	inline dbfield getId() const;
	inline char *fieldName() const;
	inline char *fieldDescr() const;
	inline UShort fieldDimension() const;
	inline UShort fieldOffset() const;
	inline type fieldType() const;
	inline itype internalFieldType() const;
	inline sqltype fieldSQLType() const;
	inline field_flags fieldFlags() const;
	inline UShort fieldLen() const;
	inline UShort fieldNDec() const;
	UShort fieldInternalLen();
	inline UChar nDec() const;
	inline char *fieldMask() const;
	inline char *fieldRelCheck() const;
	inline char *fieldInCheck() const;
	inline char *fieldDefault() const;
	inline char *fieldCompiledMask() const;
	inline STRUCT DbExpr *validateCompiledExpr() const;
	inline STRUCT DbExpr *defCompiledExpr() const;
	inline STRUCT InCheck *inCheckCompiledExpr() const;
	inline bool isInPrimaryKey() const;
	inline bool isNotNull() const;
	inline bool hasInTable() const;
	inline dbtable getInTableTableDescriptor() const;
	bool isTimeStampField() const;
	bool isEqual(const FieldMetadata &fmd) const;
	inline Int getFieldPos() const;
	inline AnyValue *getNullValue() const;
	inline bool isVectorField() const;
	inline SQLFWTable &getVectorTable() const;
	SQLFWTable &getVectorAliasTable(dbtable tab) const;
	void removeAliasFromAliasDictionary(dbtable tab) const;

	// Debug
	void dump() const;

	inline int getVectorFieldBegin();
	inline int getVectorFieldEnd();
	inline void setVectorRange(int begin, int end);

private:
	dbfield fldid_d;
	struct s_field *fRep_d;
	bool isInPrimaryKey_d;
	Int internalLen_d;
	Int pos_d;		// position inside table (first field, second field, ...)
	Int rngBegin_d, rngEnd_d;

	AnyValue *nullValue_d;
	SQLFWTable *vecTable_d;
	AliasDictionary *ad_d;

	void initializeField();
	inline AliasDictionary *aliasDictionary() const;
};

#include <sqldb/metadata/fieldmd.icc>

#endif // FIELDMD_H
