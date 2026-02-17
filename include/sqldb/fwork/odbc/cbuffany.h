//- Copyright Notice
// -----------------------------------------------------------------------
// (C) Copyright 1998 InterSoft S.A.  All Rights Reserved
// THIS IS UNPUBLISHED PROPRIETARY SOURCE CODE OF InterSoft S.A.
// The copyright notice above does not evidence any actual or intended 
// publication of such source code.
//
// $Id: cbuffany.h,v 1.7 1999/04/01 18:37:58 eduardoc Exp $
// -----------------------------------------------------------------------

#ifndef CBUFFANY_H
#define CBUFFANY_H

#include <ifound/anyvalue.h>
#include <ifound/parray.h>
#include <string.h>
                                      
class FieldMetadata;
                                      
/** 
 *	This class encapsulates a char buffer and an AnyValue
 *	and lets the user to translate the AnyValue to characters 
 *	(and viceversa) given the data type.
 */

class CharBuffAnyValue {

public:
	CharBuffAnyValue();
	CharBuffAnyValue(const AnyValue &av);
	~CharBuffAnyValue();

	// Set methods
	void setAnyValue(const AnyValue &av);
	void setBuff(const char* buff);
	inline void setType(Int type);

	// Get methods
	inline const AnyValue* getAnyValue();
	inline char *getBuff() const;
	inline Int getBuffLen() const;
	Int getType() const;
	inline Int getBuffUsedLen() const;
	inline Int *getBuffUsedLenPtr() const;

	void initBuff(Int length);
	void initBuff(const FieldMetadata *fmd);
	void buffToAny();

	inline Int getNDec() const;                                                       
	inline void setNDec(Int dec);

	inline bool isNull() const;
	inline void nullify();
	inline bool isInitialized() const;
	inline FieldMetadata *fieldMetadata() const;

	// Debugging
	void dump() const;

private:
	AnyValue* any_d;
	char* buff_d;
	Int length_d;			// buffer's length.
	Int used_d;				// number of buffer bytes occupied.
 	Int type_d;				// InterSoft type TY_BOOL, TY_DATE, ...
	Int nDec_d; 			// number of decimals (if it's a TY_NUMERIC).
	bool isNull_d;  		// whether the charBuffAnyValue is null or not.
	FieldMetadata *fmd_d;	// Pointer to the field whose value will be
							// stored.

	void fillBuff(const char* buff);
	void anyToBuff();

};

#include <sqldb/fwork/odbc/cbuffany.icc>

#endif //CBUFFANY_H
