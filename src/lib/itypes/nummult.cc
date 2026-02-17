/********************************************************************
* Copyright (c) 1991 InterSoft Co.  All Rights Reserved
* THIS IS UNPUBLISHED PROPRIETARY SOURCE CODE OF InterSoft Co.
* The copyright notice above does not evidence any
* actual or intended publication of such source code.
*
* $Id: nummult.cc,v 1.2 1999/07/30 14:06:10 ekonig Exp $
*
* DESCRIPTION
* Num product
*********************************************************************/

#include <ifound.h>
#include <ifound/numdefs.h>

extern "C" NUM *NumMul(NUM *a, const NUM *b)
{
	if (a->exp == 0) 
		return a;

	if (b->exp == 0) {
		*a = *b;
		return a;
	}

	// Calculate new exp

	int exponent = a->exp > 0 ? (b->exp > 0 ?   a->exp + b->exp - BASE_EXP
										: -(a->exp - b->exp - BASE_EXP))
						   : (b->exp < 0 ? -(a->exp + b->exp + BASE_EXP)
										:   a->exp - b->exp + BASE_EXP);
	if (exponent >= MAX_EXP) {
		a->exp = MAX_EXP;
		a->len = 0;
		return a;
	}
	else if (exponent <= -MAX_EXP) {
		a->exp = -MAX_EXP;
		a->len = 0;
		return a;
	}
	a->exp = exponent;

	short ret[NUM_SHORTS*2];
	register short *sp_r;
	for (sp_r = ret; sp_r < &ret[NUM_SHORTS*2]; sp_r++) 
		*sp_r = 0;

	short *last_ret = &ret[NUM_SHORTS*2-1];
	const short *sp_b   = &(b->s[b->len-1]);

	for (; sp_b >= b->s; sp_b--) {
		sp_r  = last_ret--;
		if (*sp_b == 0) continue;

		register long rem = 0;
		register short * sp_a = &(a->s[a->len-1]);

		while (sp_a >= a->s) {
			rem    += *sp_r + (long) *sp_b * *sp_a--;
			*sp_r-- = short(rem % NUM_BASE);
			rem /= NUM_BASE;
		}
		if (rem)
			*sp_r += short(rem % NUM_BASE);
	}

	if ( ! *sp_r ) {
		sp_r++;
		if (a->exp>0) 
			a->exp--; 
		else 
			a->exp++;
	}

	// Round the number.
	if ( sp_r < &ret[NUM_SHORTS] && sp_r[NUM_SHORTS] >= NUM_BASE/2) {
		last_ret = & sp_r[NUM_SHORTS-1];
		while (++*last_ret >= NUM_BASE) *last_ret-- = 0;
		if (last_ret < sp_r) sp_r = last_ret;
	}
	a->len = 0;
	while (sp_r < &ret[NUM_SHORTS*2] && a->len < NUM_SHORTS)
		a->s[a->len++] = *sp_r++;

	return NumNorm(a);
}

extern "C" NUM *NumMulL(NUM *a, const long b)
{
	NUM tmp;                  
	LToNum(b, &tmp);
	return NumMul(a, &tmp);
}

extern "C" NUM *NumMulF(NUM *a, const double b)
{
	NUM tmp;
	FToNum(b, &tmp);
	return NumMul(a, &tmp);
}
