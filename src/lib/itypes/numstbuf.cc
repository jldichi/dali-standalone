/********************************************************************
* Copyright (c) 1991 InterSoft Co.  All Rights Reserved
* THIS IS UNPUBLISHED PROPRIETARY SOURCE CODE OF InterSoft Co.
* The copyright notice above does not evidence any
* actual or intended publication of such source code.
*
* $Id: numstbuf.cc,v 1.3 2007/03/07 15:27:32 hardaiz Exp $
*
* DESCRIPTION
* Store Num in buffer
*********************************************************************/

#include <string.h>
#ifdef _AIX
#include <strings.h>
#endif

#include <ifound.h>
#include <ifound/itypes.h>
#include <ifound/stdc.h>
#include <ifound/priv/itypesp.h>
#include <ifound/priv/gfuncs.h>

inline int abs(int x) 
{ 
	return x < 0 ? -x : x;
}

// for fixed point Nums...

void Num::store(char *&buff, Int l, Int nd) const
{
	Int tots, nsd;
	intLength(l, nd, tots, nsd);
	buff = _store(buff, tots, nsd);
}

char *Num::_store(char *buff, Int tots, Int nsd) const
{
 	register Int totbytes = tots*sizeof(short);
	register char *p  = buff,
				  *to = p + totbytes;
 	
 	mZero(p, totbytes);

	if (exp == -MAX_EXP)
		stint(ST_MIN_EXP, p);
	else if (exp == 0)
		stint(ST_ZERO, p);
	else if (exp == MAX_EXP)
		stint(ST_MAX_EXP, p);
    else {
  		Int rem = (abs(exp) - BASE_EXP) + nsd;

		if (rem < 0 || rem > tots)
			return NULL;

		// Get in place to store the first non-zero value
		p += sizeof(short) * (tots - rem);

  		if (len < rem) rem = len;

		// Now copy them all
		for (int i = 0; i < rem; i++) {
			stint(s[i], p);
			p += sizeof(short);
		}
		*buff |= ST_CHAR_HIGH;
		if (exp < 0) {
			for (p = buff; p < to; ++p)			
				*p = ~*p;
		}
	}
	return to;
}

// for floating point Nums...

void Num::store(char *&buff) const
{
	stnum((NUM *) this, buff);
	buff += NUM_STORE_LEN;
}
