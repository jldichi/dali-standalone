//- Copyright Notice
// -----------------------------------------------------------------------
// (C) Copyright 1998 InterSoft S.A.  All Rights Reserved
// THIS IS UNPUBLISHED PROPRIETARY SOURCE CODE OF InterSoft S.A.
// The copyright notice above does not evidence any actual or intended 
// publication of such source code.
//
// $Id: paramval.h,v 1.3 2002/09/24 17:03:53 albertoi Exp $
// -----------------------------------------------------------------------

#ifndef PARAMVAL_H
#define PARAMVAL_H

#include <ifound.h>
#include <ifound/itypes.h>
#include <sqldb/fwork/object.h>

class AnyValue;
class gtype(PtrArray, AnyValue); 
class SQLQueryMetadata;

/** 
 * This class is used as an interchange structure between SQL-Framework and
 * user implemented classes (*Impl). It contains values for each one of the 
 * fields of the param. From this object we can obtain or set values to 
 * interchange values these two layers.
 * 
 * @version
 * @author Diego Balbi & Eduardo Chiocconi.
 * @see SQLFWParam
 */
class ParamValue : public Object {

public:	
	ParamValue(const SQLQueryMetadata &i);
	~ParamValue();

	// Set methods.
	void setParamValue(Int n, const AnyValue *a);

	// Get methods.
	inline const SQLQueryMetadata &param() const;
	inline Int nFlds() const;
	const AnyValue &getParamValue(Int n) const;
	inline bool isComplete() const;
	Object *clone() const;

	void dump() const;
	String convertToString() const;

private:
	const SQLQueryMetadata *param_d;
	gtype (PtrArray, AnyValue) &arr;

	ParamValue(const ParamValue &);
	ParamValue &operator=(const ParamValue &);
};

#include <sqldb/fwork/paramval.icc>

#endif	// PARAMVAL_H
