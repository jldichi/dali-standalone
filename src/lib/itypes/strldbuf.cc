/********************************************************************
* Copyright (c) 1991 InterSoft Co.  All Rights Reserved
* THIS IS UNPUBLISHED PROPRIETARY SOURCE CODE OF InterSoft Co.
* The copyright notice above does not evidence any
* actual or intended publication of such source code.
*
*
* $Id: strldbuf.cc,v 1.3 2007/03/07 15:25:31 hardaiz Exp $
*
*
* DESCRIPTION:
*				buffer store/load
*
*********************************************************************/

#include <string.h>
#ifdef _AIX
#include <strings.h>
#endif

#include <ifound/str.h>
#include <ifound/stdc.h>
#include <ifound/priv/itypesp.h>
#include <ifound/priv/gfuncs.h>

// store operators

void String::load(const char *&buff)
{
	if (VALID_PTR(p) && --p->n() == 0)
		tr_delete_vec(p);

	Int l = ldlong(buff);
	buff += sizeof(l);

	if (l == 0)
		p = (struct srep *) LOW_VALUE;
	else if (l == INT_MAX)
		p = (struct srep *) HIGH_VALUE;
	else {
		p = srep::build(UnShort(l));
		mCopy(p->s(), buff, l);
		p->s()[l] = '\0';
		buff += l;
	}
}
