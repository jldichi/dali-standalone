/********************************************************************
* Copyright (c) 1991 InterSoft Co.  All Rights Reserved
* THIS IS UNPUBLISHED PROPRIETARY SOURCE CODE OF InterSoft Co.
* The copyright notice above does not evidence any
* actual or intended publication of such source code.
*
* $Id: numcmp.cc,v 1.1.1.1 1996/04/16 18:51:55 gustavof Exp $
*
* DESCRIPTION
* Implementation of Num class
*********************************************************************/

#include <ifound.h>
#include <ifound/numdefs.h>

extern "C" long NumCmp(const NUM *a, const NUM *b)
{
	int result = a->exp - b->exp;
	if (result) return result;

	int i = b->len < a->len ? b->len : a->len;
    const short *pa = a->s;
	const short *pb = b->s;
	while (i--) {
		if ((result = *pa++ - *pb++))
			goto ret;
	}
	result = a->len - b->len;

ret :

	return a->exp > 0 ? result : -result;
}

extern "C" long NumCmpL(const NUM *a, const long b)
{
	NUM tmp;
	LToNum(b, &tmp);
	return NumCmp(a, &tmp);	
}

extern "C" long NumCmpF(const NUM *a, const double b)
{
	NUM tmp;
	FToNum(b, &tmp);
	return NumCmp(a, &tmp);	
}

