/********************************************************************
* Copyright (c) 1991 InterSoft Co.  All Rights Reserved
* THIS IS UNPUBLISHED PROPRIETARY SOURCE CODE OF InterSoft Co.
* The copyright notice above does not evidence any
* actual or intended publication of such source code.
*
* $Id: strchr.cc,v 1.2 2004/09/29 17:20:53 damianm Exp $
*
* DESCRIPTION
* Set, Insert, Delete a char from a String
*********************************************************************/

#include <string.h>

#include <ifound/itypes.h>
#include <ifound/stdc.h>

#include <ifound/priv/itypesp.h>

#ifdef realloc
#	undef realloc
#endif

bool String::setChar(char c, Int n)
{
	// fuera de rango
	if (n < 0 || n > UN_SHORT_MAX || 
		p == (struct srep *)HIGH_VALUE) return false;

	// Reallocate if necesary
	realloc(UnShort(n+1));

	if (n >= p->len()) {
		char *q = &p->s()[p->len()];
		while (q < & p->s()[n])
			*q++ = ' ';

		p->s()[p->len() = UnShort(n+1)] = 0;
	}
	p->s()[n] = c; 
	return true;
}

bool String::delChar(Int n)
{
	if (! VALID_PTR(p) || n < 0 || n >= p->len())
		return false;

	if (p->len() == 1) {
		disconnect();
		p = 0;
		return true;
	}
	UnShort plen = p->len();

	if (BYTESALLOC(plen-1) < plen || p->n() > 1) {
		srep *newptr = 	srep::build(plen-1);
		if (n)
			strncpy(newptr->s(), p->s(), (int)n);
		strcpy(newptr->s() + n, p->s() + n + 1); 
		disconnect();
		p = newptr;
	}					
	else {
		memmove(p->s() + n, p->s() + n + 1, (p->len()-n-1));
		//istrcpy(p->s() + n, p->s() + n + 1); 
		p->s()[--p->len()] = 0;
	}
	return true;
}

bool String::insChar(char c, Int n)
{
	if (p == (struct srep *)HIGH_VALUE) return false;

	UnShort plen = UnShort(length());

	// fuera de rango

	if (n < 0 || n >= plen)
		return setChar(c, n);

	// Reallocate if necesary

	realloc(plen+1);
	p->s()[++p->len()] = 0;

	for (Int i = plen; i > n ; i--)
		p->s()[i] = p->s()[i-1];

	p->s()[n] = c; 

	return true;
}
