//- Copyright Notice
// -----------------------------------------------------------------------
// (C) Copyright 1998 InterSoft S.A.  All Rights Reserved
// THIS IS UNPUBLISHED PROPRIETARY SOURCE CODE OF InterSoft S.A.
// The copyright notice above does not evidence any actual or intended 
// publication of such source code.
//
// $Id: indexval.h,v 1.8 2002/03/20 17:16:07 hardaiz Exp $
// -----------------------------------------------------------------------

#ifndef INDEXVAL_H
#define INDEXVAL_H

#include <ifound.h>
#include <ifound/itypes.h>
#include <sqldb/fwork/object.h>
#include <sqldb/fwork/index.h>

class AnyValue;
class gtype(PtrArray, AnyValue); 

/** 
 * This class is used as an interchange structure between SQL-Framework and
 * user implemented classes (*Impl). It contains values for each one of the 
 * fields of the index. From this object we can obtain or set values to 
 * interchange values these two layers.
 * 
 * @version
 * @author Diego Balbi & Eduardo Chiocconi.
 * @see SQLFWIndex
 */
class IndexValue : public Object {

public:	
	IndexValue(const SQLFWIndex &i);
	~IndexValue();

	// Set methods.
	void setIndexValue(Int n, const AnyValue &a);

	// Get methods.
	inline const SQLFWIndex &index() const;
	inline Int nFlds() const;
	const AnyValue &getIndexValue(Int n) const;
	inline bool isComplete() const;
	Object *clone() const;

	void dump() const;
	String convertToString() const;

private:
	const SQLFWIndex *index_d;
	gtype (PtrArray, AnyValue) &arr;

	IndexValue(const IndexValue &);
	IndexValue &operator=(const IndexValue &);
};

#include <sqldb/fwork/indexval.icc>

#endif	// INDEXVAL_H
