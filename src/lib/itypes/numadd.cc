/********************************************************************
* Copyright (c) 1991 InterSoft Co.  All Rights Reserved
* THIS IS UNPUBLISHED PROPRIETARY SOURCE CODE OF InterSoft Co.
* The copyright notice above does not evidence any
* actual or intended publication of such source code.
*
* $Id: numadd.cc,v 1.2 1999/07/30 14:06:09 ekonig Exp $
*
* DESCRIPTION
* Num add methods
*********************************************************************/

#include <ifound.h>
#include <ifound/numdefs.h>

extern "C" NUM *NumSub(NUM *a, const NUM *b)
{
	if (b->exp == 0) return a;

	((NUM*)b)->exp = -b->exp;
	(void)NumAdd(a, b);
	((NUM*)b)->exp = -b->exp;
	return a;
}

extern "C" NUM *NumSubL(NUM *a, const long b)
{
	NUM tmp;
	LToNum(b, &tmp);
    return NumSub(a, &tmp);
}              

extern "C" NUM *NumSubF(NUM *a, const double b)
{
	NUM tmp;
	FToNum(b, &tmp);
    return NumSub(a, &tmp);
}

extern "C" NUM *NumAdd(NUM *a, const NUM *b)
{
	if (b->exp == 0) return a;
	if (a->exp == 0) {
		*a = *b;
		return a;
	}

	NUM tmp;
	short substract = 0;

	// Check signs

	short exp_diff = a->exp - b->exp;

	if (b->exp < 0) {
		if (a->exp < 0)
			exp_diff = -exp_diff;
		else {
			exp_diff = a->exp + b->exp;
			substract= 1;
		}
	}
	else if (a->exp < 0) {
		exp_diff = - a->exp - b->exp;
		substract= 1;
	}

	// Check special cases

	if (a->exp == MAX_EXP || a->exp == -MAX_EXP) {
		if (b->exp == -a->exp)
			a->exp = 0;
		return a;
	}
	if (b->exp == MAX_EXP || b->exp == -MAX_EXP) {
		a->exp = b->exp;
		a->len = 0;
		return a;
	}

	// Normalize
	
	if (exp_diff < 0)
		NumShiftExp(a, a, -exp_diff);
	else if (exp_diff > 0) {
		NumShiftExp(b, &tmp, exp_diff);
		b = & tmp;
	}

	register short *sp   = a->s + b->len;
	register const short *sp_b = (b->s + b->len);

	short rem = 0;
	short i = b->len - a->len;

	if (substract) {

		// Substract the numbers

		if (i > 0) {
			a->len = b->len;
			while (i--) {
				*--sp = NUM_BASE - rem - *--sp_b;
				if (*sp_b) rem = 1;
			}
		}
		while (--sp >= a->s) {
			*sp -= *--sp_b + rem;
			if ((rem = *sp < 0))
				*sp += NUM_BASE;
		}
		if (rem) {
			rem = 0;
			a->exp = -a->exp;
			for (sp = &(a->s[a->len-1]); sp >= a->s; --sp) {
				*sp = NUM_BASE - *sp - rem;
				if (*sp < NUM_BASE)
					rem = 1;
				else
					*sp = rem = 0;
			}
		}
	}
	else {

		// Add the numbers

		if (i > 0) {
			a->len = b->len;
			while (i--)
				*--sp = *--sp_b;
		}


		while (--sp >= a->s) {
			*sp += *--sp_b + rem;
			if ((rem = *sp >= NUM_BASE))
				*sp -= NUM_BASE;
		}
		if (rem) {
			NumShiftExp(a, a, 1);
			a->s[0] = rem;
		}
	}
	return NumNorm(a);
}

extern "C" NUM *NumAddL(NUM *a, const long b)
{
	NUM tmp;
	LToNum(b, &tmp);
	return NumAdd(a, &tmp);
}

extern "C" NUM *NumAddF(NUM *a, const double b)
{
	NUM tmp;
	FToNum(b, &tmp);
	return NumAdd(a, &tmp);
}

