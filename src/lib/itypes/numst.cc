/********************************************************************
* Copyright (c) 1991 InterSoft Co.  All Rights Reserved
* THIS IS UNPUBLISHED PROPRIETARY SOURCE CODE OF InterSoft Co.
* The copyright notice above does not evidence any
* actual or intended publication of such source code.
*
* $Id: numst.cc,v 1.2 2004/09/29 17:20:40 damianm Exp $
*
* DESCRIPTION
* Num store methods
*********************************************************************/

#include <string.h>

#include <ifound.h>
#include <ifound/numdefs.h>
#include <ifound/stdc.h>
#include <ifound/priv/gfuncs.h>

extern "C" void stnum(NUM *a, char *p) 
{
	int i;
	short tmp;

	stuint(UnShort(short(a->exp) + MAX_EXP), p);
	p += sizeof(short);

	for (i = 0; i < a->len; i++) {
		tmp = a->s[i];
		if (a->exp < 0)
			tmp = ~tmp;
		stint(tmp, p);
		p += sizeof(*a->s);
	}

	// Make sure to fill with zeroes behind
	mSet(p, a->exp < 0 ? ~char(0) : 0, (NUM_SHORTS-i)*sizeof(short));
}
