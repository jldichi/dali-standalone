//- Copyright Notice
// -----------------------------------------------------------------------
// (C) Copyright 1998 InterSoft S.A.  All Rights Reserved
// THIS IS UNPUBLISHED PROPRIETARY SOURCE CODE OF InterSoft S.A.
// The copyright notice above does not evidence any actual or intended 
// publication of such source code.
//
// $Id: tableval.h,v 1.10 1999/06/02 21:10:08 eduardoc Exp $
// -----------------------------------------------------------------------

#ifndef TABLEVAL_H
#define TABLEVAL_H

#include <ifound.h>
#include <ifound/itypes.h>
#include <sqldb/fwork/object.h>
#include <ifound/anyvalue.h>
#include <ifound/parray.h>
#include <sqldb/fwork/odbc/cbuffany.h>

class gtype(PtrArray, CharBuffAnyValue); 
class FieldMetadata;

/** 
 * This class is used as an interchange structure between SQL-Framework and
 * user implemented classes (*Impl). It contains values for each one of the 
 * fields of the index. From this object we can obtain or set values to 
 * interchange values these two layers.
 * 
 * @version
 * @author Diego Balbi & Eduardo Chiocconi.
 * @see Index
 */
class TableValue {

public:	
	TableValue(Int nFlds);
	~TableValue();

	// Set methods.
	Int setTableValue(Int n, const AnyValue &a);
	inline void setCharBuff(Int i, const char *buff);
	inline void setType(Int i, Int type);
	inline void setNDec(Int i, Int dec);

	// Get methods.
	const CharBuffAnyValue &getCharBuffAnyValue(Int i) const;
	const AnyValue &getTableValue(Int n) const;
	inline char *getCharBuff(Int i) const;
	inline Int getType(Int i) const;
	inline Int getCharBuffLen(Int i) const;
	inline Int getCharBuffUsedLen(Int i) const;
	inline Int *getCharBuffUsedLenPtr(Int i);
	bool isComplete() const;
	Int nFlds() const;
	inline Int getNDec(Int i);

	void initCharBuff(Int i, Int length);
	void initCharBuff(Int i, const FieldMetadata *fmd);
	void buffToAny(); // translate every element
	void buffToAny(Int i); // translate i-th element
	
	void clear();

	// Debugging
	void dump() const;
	void dumpBuffers() const;

private:
	gtype (PtrArray, CharBuffAnyValue) *arr;
	Int ocuppied_d;	// slots used;

	CharBuffAnyValue &getCharBuffAnyValueAt(Int i) const;
		
	TableValue(const TableValue &tv);
	TableValue &operator=(const TableValue &tv);
};

#include <sqldb/fwork/odbc/tableval.icc>

#endif //TABLEVAL_H
