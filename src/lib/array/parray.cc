/********************************************************************
* Copyright (c) 1991 InterSoft Co.  All Rights Reserved
* THIS IS UNPUBLISHED PROPRIETARY SOURCE CODE OF InterSoft Co.
* The copyright notice above does not evidence any
* actual or intended publication of such source code.
*
* $Id: parray.cc,v 1.2 2004/09/29 15:39:13 damianm Exp $
*
* DESCRIPTION
* Implementation of Pointer array class
*********************************************************************/

#ifdef __GNUC__
#pragma implementation
#endif

#include <stdlib.h>
#include <strings.h>
#include <string.h>

#include <ifound.h>
#include <ifound/parray.h>
#include <ifound/str.h>
#include <ifound/stdc.h>

BasePtrArray::BasePtrArray(Int n, Int chunk)
:	a_dim(0),
	chunkSize(chunk),
	c(NULL)
{
	reAlloc(n);
}

BasePtrArray::~BasePtrArray()
{
	reAlloc(0);
}

void BasePtrArray::reAlloc(Int n)
{
	typedef unsigned int uint;
	typedef void *pvoid;

	// Calculate the least multiple of chunkSize that is >= n
	Int realSize = chunkSize * ((n+chunkSize-1) / chunkSize);
	Int real	 = chunkSize * ((a_dim+chunkSize-1) / chunkSize);

	// Really need to reallocate?
	if (realSize == real) {
		if (n < a_dim)
			mZero(&c[n], (a_dim-n) * sizeof(void *));
	} else {
		void **tmp = NULL;
		Int minimum = a_dim < n ? a_dim : n;

		if (realSize) {
			tmp = tr_new pvoid[realSize];

			// If there is something to copy, do it.
			if (minimum)
				mCopy(tmp, c, minimum * sizeof(void *));

			mZero(&tmp[minimum], (realSize - minimum) * sizeof(void *));
		}
		if (c) tr_delete_vec(c);
		c = tmp;
	}
	a_dim = n;
}

void *&BasePtrArray::elemTest(Int n)
{
	if (n < 0 || n >= a_dim)
		error(libMsg("OUTOFBOUNDS"));
	return c[n];
}

Int BasePtrArray::add(const void *elem)
{
	Int i;

	for (i = 0; i < a_dim && c[i]; i++)
		;

	if (i >= a_dim)
		reAlloc(a_dim + 1);

	c[i] = (void *) elem;
	return i;
}

Int BasePtrArray::nextFree() const
{
	Int i;

	for (i = 0; i < a_dim && c[i]; i++)
		;

	return i;
}

void BasePtrArray::del(Int n)
{
	mMove(&c[n], &c[n+1], (a_dim - n - 1) * sizeof(void *));

	reAlloc(a_dim - 1);
}

void BasePtrArray::ins(Int n, const void *elem)
{
	// need room for one more
	reAlloc(a_dim + 1);

	mMove(&c[n+1], &c[n], (a_dim - n - 1) * sizeof(void *));
	c[n] = (void *) elem;
}

Int BasePtrArray::del(const void *elem)
{
	Int i;
	if ((i = find(elem)) != ERR)
		c[i] = NULL;
	return i;
}

Int BasePtrArray::find(const void *elem)
{
	Int i;

	for (i = 0; i < a_dim && c[i] != elem; i++)
		;
	
	return i < a_dim ? i : ERR;
}

void BasePtrArray::clear()
{
	typedef unsigned int uint;

	if (c)
		mZero(c, a_dim * sizeof(void *));
}

void BasePtrArray::sort(IFPVCPVCP sortFunc)
{
	if (c)
		qsort(c, a_dim, sizeof(void *), sortFunc);
}

int BasePtrArray::find(void const *el, IFPVCPVCP cmpFunc)
{
	return c == NULL ? ERR
			: locate((char const *) &el, (char const *) c,
			  (unsigned) a_dim, (unsigned) sizeof(void *),
			  (IFPCPCP) cmpFunc);
}

void BasePtrArrayScanner::scanUp_()
{
	if (base != NULL)
		while (curr < last && *curr == NULL)
			curr++;
}

void BasePtrArrayScanner::scanDown_()
{
	if (base != NULL)
		while (curr >= base && *curr == NULL)
			curr--;
}
