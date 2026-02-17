/********************************************************************
* Copyright (c) 1991 InterSoft Co.  All Rights Reserved
* THIS IS UNPUBLISHED PROPRIETARY SOURCE CODE OF InterSoft Co.
* The copyright notice above does not evidence any
* actual or intended publication of such source code.
*
* $Id: barray.cc,v 1.3 2007/03/07 18:47:05 hardaiz Exp $
*
* DESCRIPTION
* Implementation of bit array class
*********************************************************************/

#include <string.h>
#ifdef _AIX
#include <strings.h>
#endif

#ifdef __GNUC__
#pragma implementation
#endif

#include <ifound.h>
#include <ifound/barray.h>
#include <ifound/stdc.h>

#define BITS_PER_INT (sizeof(Int)*8)

BoolArray::BoolArray(Int s)
{
	if (s < 0)
		s = 0;

	size = s;
	offset = 0;
	if (s) {
		Int n = (size-1) / BITS_PER_INT + 1;
		v = tr_new Int[n];
		mZero(v, n*sizeof(Int));
	} else
		v = NULL;
}


BoolArray::operator int()
{
	if (0 > offset || offset >= size)
		return false;

	return (v ? v[offset/BITS_PER_INT] & 1 << (offset%BITS_PER_INT) :
			false);
}

int BoolArray::operator=(int b)
{
	if (0 > offset || offset >= size || !v)
		return false;

	Int mask = 1 << (offset%BITS_PER_INT);
	if (b)
		v[offset/BITS_PER_INT] |= mask;
	else
		v[offset/BITS_PER_INT] &= ~mask;
	return b;
}

void BoolArray::realloc(Int s)
{
	if (s < 0)
		s = 0;
	Int n = s ? s/BITS_PER_INT + 1 : 0;
	Int m = size ? (size-1)/BITS_PER_INT + 1 : 0;
	size = s;
	if (n == m) return;

	Int *aux;

	if (n) {
		aux = tr_new Int[n];

		if (n < m)
			mCopy(aux, v, n*sizeof(Int));
		else {
			mCopy(aux, v, m*sizeof(Int));
			mZero(aux+m, (n-m)*sizeof(Int));
		}
	}
	else
		aux = NULL;

	if (v)
		tr_delete_vec(v);

	v = aux;
}

BoolArray::BoolArray(const BoolArray& a)
:	v(NULL)
{
	*this = a;
}

BoolArray::~BoolArray()
{
	if (v)
		tr_delete_vec(v);
}

BoolArray &BoolArray::operator=(const BoolArray& a)
{
	if (this == &a)
		return *this;
		
	if (v)
		tr_delete_vec(v);

	size = a.size;
	offset = a.offset;
	
	Int n = size ? (size-1)/BITS_PER_INT + 1 : 0;

	if (n) {
		v = tr_new Int[n];
		mCopy(v, a.v, n*sizeof(Int));
	}
	else
		v = NULL;

	return *this;
}

BoolArray BoolArray::operator&(const BoolArray &b2) const
{
	BoolArray b1(*this);
	Int msize = min(b1.size, b2.size);
	Int n = msize ? (msize-1)/BITS_PER_INT + 1 : 0;
	Int i;
	for (i = 0; i < n; i++)
		b1.v[i] &= b2.v[i];
	return b1;
}

bool BoolArray::any() const
{
	Int n = size ? (size-1)/BITS_PER_INT + 1 : 0;
	Int i;
	for (i = 0; i < n; i++)
		if (v[i] != 0)
			return true;

	return false;
}

void BoolArray::setAll(bool val)
{
	Int filler = 0;

	if (val)
		filler = ~filler;

	Int n = size ? (size-1)/BITS_PER_INT + 1 : 0;
	Int i;
	for (i = 0; i < n; i++)
		v[i] = filler;
}

bool BoolArray::intersects(const BoolArray &b) const
{
	Int msize = min(size, b.size);
	Int n = msize ? (msize-1)/BITS_PER_INT + 1 : 0;
	Int i;
	for (i = 0; i < n; i++)
		if (v[i] & b.v[i])
			return true;

	return false;
}
