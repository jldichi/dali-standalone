/********************************************************************
* Copyright (c) 1991 InterSoft Co.  All Rights Reserved
* THIS IS UNPUBLISHED PROPRIETARY SOURCE CODE OF InterSoft Co.
* The copyright notice above does not evidence any
* actual or intended publication of such source code.
*
* $Id: numshift.cc,v 1.2 2004/09/29 15:33:29 damianm Exp $
*
* DESCRIPTION
* Number shifting (base 10) methods
*********************************************************************/
#include <strings.h>

#include <ifound.h>
#include <ifound/numdefs.h>
#include <ifound/stdc.h>

static short tenpow[5] = { 1, 10, 100, 1000, 10000 };

extern "C" NUM *NumShift(NUM *a, Int n)
{
	assert(a != NULL && a->len <= NUM_SHORTS);
	if (n == 0 || a->exp == 0 || a->exp == MAX_EXP || a->exp == -MAX_EXP)
		return a;

	short ns = short(n < 0 ? -n : n);
	short exponent = (a->exp < 0 ? -a->exp : a->exp) + short(n) / 4;

	if (exponent <= 0)
		exponent = a->len = 0;
	else if (exponent >= MAX_EXP) {
		a->len = 0;
		exponent = MAX_EXP;
	}
	a->exp = a->exp > 0 ? exponent : -exponent;

	if (a->len == 0 || !(ns = ns % 4)) return a;

	exponent -= BASE_EXP;
	short ret[NUM_SHORTS+1];

	mZero(ret, sizeof(ret));
	mCopy(&ret[1], a->s, a->len * sizeof(short));

	short rem = 0, aux;
	short ndiv = tenpow[4-ns], nmul = tenpow[ns];

	if (n > 0) {
		for (short *sp = &ret[a->len]; sp > ret; sp--) {
			aux = *sp / ndiv;
			*sp = (*sp % ndiv) * nmul + rem;
			rem = aux;
		}

		if (!rem)
			mCopy(a->s, &ret[1], a->len * sizeof(short));
		else {
			ret[0] = rem;
			if (a->len < NUM_SHORTS) a->len++;
			a->exp = a->exp < 0 ? a->exp - 1 : a->exp + 1;
			mCopy(a->s, ret, a->len * sizeof(short));
		}
	} 
	else {
		for (short *sp = &ret[1]; sp <= &ret[a->len]; sp++) {
			aux = *sp % nmul;
			*sp = *sp / nmul + (rem * ndiv);
			rem = aux;
		}

		mCopy(a->s, &ret[1], a->len * sizeof(short));

		if (rem && a->len < NUM_SHORTS)
			a->s[a->len++] = rem * ndiv;
	}

	return NumNorm(a);
}

extern "C" void NumShiftExp(const NUM *a, NUM *b, short places)
{
	short *sp;
	if (a != b) 
		*b = *a;

	if (places == 0)
		return;

	b->exp += (b->exp >0) ? places : -places;

	if (places > NUM_SHORTS-1 || places < - (NUM_SHORTS-1) ) {
		b->len = 0;
		return;
	}

	b->len += places;

	if (places > 0) {
		for (sp = &(b->s[b->len-1]); sp >= &(b->s[places]); sp--)
			sp[0] = sp[-places];
		while (sp >= b->s)
			*sp-- = 0;
	}
	else {
		for (sp = b->s; sp < &(b->s[b->len]); sp++)
			sp[0] = sp[-places];
	}
}


