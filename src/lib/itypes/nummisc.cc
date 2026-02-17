/********************************************************************
* Copyright (c) 1991 InterSoft Co.  All Rights Reserved
* THIS IS UNPUBLISHED PROPRIETARY SOURCE CODE OF InterSoft Co.
* The copyright notice above does not evidence any
* actual or intended publication of such source code.
*
* $Id: nummisc.cc,v 1.2 1998/04/24 16:50:22 guiller Exp $
*
* DESCRIPTION
* Implementation of Num class
*********************************************************************/

#include <ifound.h>
#include <ifound/numdefs.h>

static short tenpow[5] = { 1, 10, 100, 1000, 10000 };
inline short abs(short x) { return x < 0 ? -x : x; }

extern "C" NUM *NumNorm(NUM *a)
{
	// Eliminate trailing zeroes

	while (a->len && !a->s[a->len-1])
		--(a->len);

	if (a->len == 0)
		a->exp = 0;
	else {
		// Eliminate leading zeroes
		short i;
		for (i = 0; i < a->len && !(a->s[i]); ++i)
			;
		NumShiftExp(a, a, -i);
	}
	if (a->exp <= -MAX_EXP || a->exp >= MAX_EXP) {
		a->exp = a->exp < 0 ? -MAX_EXP : MAX_EXP;
		a->len = 0;
	}
	return a;
}

extern "C" Int NumNthDig(const NUM *a, Int n) 
{
	short exponent = (a->exp < 0 ? -a->exp : a->exp) - BASE_EXP;
	short pos = exponent - short(n <= 0 ? n / 4 : (n - 1) / 4 + 1);

	if ( pos < 0 || pos > a->len-1 ) return 0;

	n %= 4;
	short tp = tenpow[ short(n-1 < 0 ? 3+n : n-1) ];

	return ((a->s[pos] / tp) % 10);
}

extern "C" void NumTrunc(NUM *a, Int ndec, short up)
{
	short ind = abs((short)a->exp) - BASE_EXP + short(ndec/4) - (ndec <= 0);

	if (ind >= a->len)
		return;

	if (ind < 0) {
		LToNum(up ? (a->exp > 0 ? 1L : -1L) : 0L, a);
		return;
	}

	short nd = ndec > 0 ? 4 - short(ndec % 4) : short(-ndec % 4);
	short i = a->s[ind] / tenpow[nd] * tenpow[nd];
	if (i == a->s[ind] && a->len-1 <= ind)
		up = 0;
	a->s[ind] = i;
	a->len = ind + 1;

	if (up) {
		NUM aux;
		LToNum(1L, &aux);	
		NumShift(&aux, -ndec);
		if (a->exp < 0)
			(void)DoNumNeg(&aux);
		NumAdd(a, &aux);
	}

	NumNorm(a);
}

extern "C" void NumRound(NUM *a, Int ndec) 
{
	NumTrunc(a, ndec, NumNthDig(a, -ndec) >= 5);
}

