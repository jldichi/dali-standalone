/********************************************************************
* Copyright (c) 1991 InterSoft Co.  All Rights Reserved
* THIS IS UNPUBLISHED PROPRIETARY SOURCE CODE OF InterSoft Co.
* The copyright notice above does not evidence any
* actual or intended publication of such source code.
*
* $Id: numldbuf.cc,v 1.2 2004/09/29 15:33:12 damianm Exp $
*
* DESCRIPTION
* Load Num from buffer
*********************************************************************/

#include <strings.h>
#include <string.h>

#include <ifound.h>
#include <ifound/itypes.h>
#include <ifound/stdc.h>
#include <ifound/priv/gfuncs.h>
#include <ifound/priv/itypesp.h>

void Num::intLength(Int l, Int nd, Int &tots, Int &nsd)
{
	Int nsi = (l - nd + DIGITS_BASE - 1) / DIGITS_BASE;
	nsd     = (nd     + DIGITS_BASE - 1) / DIGITS_BASE;
	tots = nsi + nsd;
}

Int Num::conLength(Int l, Int nd)
{
	Int tots, nsd;
	intLength(l, nd, tots, nsd);
	return ((tots * sizeof(short) - 1)/sizeof(Int)+sizeof(Int))*sizeof(Int);
}

bool Num::sameRep(Int l1, Int d1, Int l2, Int d2)
{
	Int tots1, nsd1, tots2, nsd2;
	intLength(l1, d1, tots1, nsd1);
	intLength(l2, d2, tots2, nsd2);
	return bool(tots1 == tots2 && nsd1 == nsd2);
}

// for fixed point Nums...


void Num::load(const char *&buff, Int l, Int nd)
{
	Int tots, nsd;
	intLength(l, nd, tots, nsd);
	buff = _load(buff, tots, nsd);
}

const char *Num::_load(const char *buff, Int tots, Int nsd)
{
	register const char *p = buff;
	UnShort first = lduint(p);

	if (first == ST_MIN_EXP) {
		len = 0;
		exp = -MAX_EXP;
	} else if (first == ST_MAX_EXP) {
		len = 0;
		exp = MAX_EXP;
	} else {
		bool neg = !(p[0] & ST_CHAR_HIGH);
		mZero(s, sizeof(short) * NUM_SHORTS);

		s[0] = first ^ ST_SHORT_HIGH;
        if (neg) s[0] = ~s[0];

		int i, j;
		for (i = 1; i < tots; i++) {
			p += sizeof(short);
			s[i] = lduint(p);
			if (neg) s[i] = ~s[i];
		}

		// Now normalize the number
		for (i = 0; i < NUM_SHORTS && s[i] == 0; ++i)
			;
		if (i == NUM_SHORTS) {
			len = 0;
			exp = 0;
		} else {
			len = NUM_SHORTS - i;
			for (j = NUM_SHORTS - 1; s[j] == 0; --j)
				--len;
			mMove(s, s + i, len * sizeof(short));
			exp = (signed char) (BASE_EXP + tots - nsd) - i;
			if (neg)
				exp = -exp;
		}
	}
	return buff + tots * sizeof(short);
}

// for floating point Nums...

void Num::load(const char *&buff)
{
	ldnum((NUM *) this, (char *) buff);
	buff += NUM_STORE_LEN;
}

Int	Num::storeLength()
{
	return NUM_STORE_LEN;
}
