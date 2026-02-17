/********************************************************************
* Copyright (c) 1991 InterSoft Co.  All Rights Reserved
* THIS IS UNPUBLISHED PROPRIETARY SOURCE CODE OF InterSoft Co.
* The copyright notice above does not evidence any
* actual or intended publication of such source code.
*
* $Id: numld.cc,v 1.2 1996/10/22 15:55:37 eduardoc Exp $
*
* DESCRIPTION
* Num load methods
*********************************************************************/

#include <ifound.h>
#include <ifound/numdefs.h>
#include <ifound/priv/gfuncs.h>

extern "C" NUM *ldnum(NUM *a, char *p)
{
	int i = 0, aux = ldint(p) - MAX_EXP;
	short tmp;

	a->exp = (signed char)aux;
	p += sizeof(short);
	if (a->exp == 0x0 || a->exp == MAX_EXP || a->exp == -MAX_EXP)
		a->len = 0;
	else {
		int last = 0;
		for (i = 0; i < NUM_SHORTS; i++) {
			tmp = ldint(p);
			if (a->exp < 0)
				tmp = ~tmp;
			if ((a->s[i] = tmp))
				last = i;
			p += sizeof(a->s[i]);
		}
		a->len = last+1;
	}
	return a;
}
