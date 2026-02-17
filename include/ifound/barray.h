/********************************************************************
* Copyright (c) 1991 InterSoft Co.  All Rights Reserved
* THIS IS UNPUBLISHED PROPRIETARY SOURCE CODE OF InterSoft Co.
* The copyright notice above does not evidence any
* actual or intended publication of such source code.
*
* $Id: barray.h,v 1.1.1.1 1996/04/16 18:51:31 gustavof Exp $
*
* DESCRIPTION
* Array of Boolean class
*********************************************************************/
// This class was documented by Oscar (/usr2/acct/oscar/doc/barray/barray.doc)
#ifndef BARRAY_H
#define BARRAY_H

#include <ifound.h>

#ifdef __GNUC__
#	pragma interface
#endif

class BoolArray {

// private:

	Int *v;
	Int size,
		offset;

public:

	explicit BoolArray(Int n = 0);
	BoolArray(const BoolArray &);
	~BoolArray();

	BoolArray & operator[](Int n) { offset = n; return *this; }

	BoolArray &operator=(const BoolArray &);
	int	operator=(int);
	BoolArray operator&(const BoolArray &) const;
	BoolArray &operator&=(const BoolArray &b)
	{ return (*this = *this & b); }
	bool any() const;
	void setAll(bool);
	bool intersects(const BoolArray &b) const;

	operator int();

	Int dim()	{ return size; }
	void realloc(Int);
};
#endif
