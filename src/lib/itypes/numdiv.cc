/********************************************************************
* Copyright (c) 1991 InterSoft Co.  All Rights Reserved
* THIS IS UNPUBLISHED PROPRIETARY SOURCE CODE OF InterSoft Co.
* The copyright notice above does not evidence any
* actual or intended publication of such source code.
*
* $Id: numdiv.cc,v 1.3 2004/09/29 15:32:56 damianm Exp $
*
* DESCRIPTION
* Num division methods
*********************************************************************/

#include <strings.h>

#include <ifound.h>
#include <ifound/numdefs.h>
#include <ifound/stdc.h>

#define BASEL long(NUM_BASE)

extern "C" NUM *NumDiv(NUM *a, const NUM *b)
{
	if (a->exp == 0)
		return a;

	if (b->exp == 0) {
		a->exp = a->exp > 0 ? MAX_EXP : -MAX_EXP;
		a->len = 0;
		return a;
	}

	if (a->exp == MAX_EXP || a->exp == -MAX_EXP) {
		if (b->exp == a->exp)
			LToNum(1, a);
		else if (b->exp == -a->exp)
			LToNum(-1, a);
		else if (b->exp < 0)
			a->exp = -a->exp;
		return a;
	}

	if (b->exp == MAX_EXP || b->exp == -MAX_EXP) {
		a->exp = 0;
		a->len = 0;
		return a;
	}

	// Calculate new exp (Aply the rule of signs)

	int x; // Abs value of exponent
	int exponent= a->exp > 0 ? (b->exp > 0 ?  (x =  a->exp-b->exp +BASE_EXP)
									   : -(x =  a->exp+b->exp +BASE_EXP))
						  : (b->exp < 0 ?  (x = -a->exp+b->exp +BASE_EXP)
									   : -(x = -a->exp-b->exp +BASE_EXP));
	
	// dividendo
	short u[NUM_SHORTS*2];
	mZero(u, sizeof(u));

	// divisor
	short v[NUM_SHORTS];
	mZero(v, sizeof(v));

	// Cociente
	short q[NUM_SHORTS+3];
	mZero(q, sizeof(q));

	short *sp_u, *sp_v;
	short *sp_q;
	register short *uj, *vk;

	// Normalizar (convertir v de forma que v[0] >= NUM_BASE/2)

	short d = NUM_BASE / (b->s[0] + 1);
	if (d == 1) {
		mCopy(&u[1], a->s,   a->len   * sizeof(short));
		mCopy(v,     b->s,   b->len * sizeof(short));
	}
	else {
		long rem = 0;
		sp_u = &u[a->len];
		sp_q = &(a->s[a->len-1]);
		while (sp_u > u) {
			*sp_u-- = short((rem += *sp_q-- * (long) d) % NUM_BASE);
			rem /= NUM_BASE;
		}
		u[0] = short(rem);

		rem = 0;
		sp_v = &  v[b->len-1];
		sp_q = (short*)(&b->s[b->len-1]);
		while (sp_v >= v) {
			*sp_v-- = short((rem += *sp_q-- * (long) d) % NUM_BASE);
			rem /= NUM_BASE;
		}
	}

	// do the division

	sp_v = v;
	for (sp_u = u,sp_q= &q[1]; sp_q < &q[NUM_SHORTS+3]; sp_q++,sp_u++) {

		// Calculate estimated quotient

		register long e_q = (sp_u[0] == sp_v[0]) ? NUM_BASE - 1
						  : (sp_u[0] * BASEL + sp_u[1]) / sp_v[0];

		// Check if e_q is too big

		while ( sp_v[1] * e_q > sp_u[2] +
		 ((sp_u[0]*BASEL + sp_u[1]) - e_q*sp_v[0])*BASEL)
				e_q--;

		register long rem = 0;
		while (e_q) {

			// u -= v * e_q;

			for (vk = &v[b->len-1], uj = &sp_u[b->len]; uj > sp_u; uj--) {
				*uj -= short((rem += e_q * *vk--) % NUM_BASE);
				rem /= NUM_BASE;
				if (*uj < 0) {
					*uj += NUM_BASE;
					rem++;
				}
			}
			if (*sp_u >= rem) break;

			// u += v * e_q;

			rem = 0;
			for (vk = &v[b->len-1], uj = &sp_u[b->len]; uj > sp_u; uj--) {
				*uj = short((rem += *uj + e_q * *vk--) % NUM_BASE);
				rem /= NUM_BASE;
			}	
			rem = 0;
			e_q--;
		}
		*sp_u -= short(rem);
		*sp_q =  short(e_q);
	}

	//  I still have to round the number.

	// point sp_q to first non zero

	for (sp_q = q; sp_q < &q[NUM_SHORTS+3] && *sp_q == 0; sp_q++)
		;


	a->len = &q[NUM_SHORTS+3] - sp_q;
	if (a->len > NUM_SHORTS)
		a->len = NUM_SHORTS;

	// sp_u to last significant digit

	sp_u = sp_q + a->len;

	// Round

	if ( *sp_u >= NUM_BASE/2 ) {
		++(*--sp_u);
		while ( *sp_u >= NUM_BASE ) {
			*sp_u = 0;
			++(*--sp_u);
		}
	}

	// Adjust Exponent and Check underflow 

	int adj = (sp_q - q) - 2;
	if (x-adj <= 0)
		a->exp = a->len = 0;
	else {
		a->exp = exponent - (exponent > 0 ? adj : -adj);
		mCopy(a->s, sp_q, a->len * sizeof(short));
		while (a->len && !(a->s[a->len-1]))
			--(a->len);
	}
	return a;
}

extern "C" NUM *NumDivL(NUM *a, const long b)
{
	NUM tmp;                               
	LToNum(b, &tmp);
	return NumDiv(a, &tmp);
}

extern "C" NUM *NumDivF(NUM *a, const double b)
{
	NUM tmp;                               
	FToNum(b, &tmp);
	return NumDiv(a, &tmp);
}
