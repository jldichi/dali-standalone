/********************************************************************
* Copyright (c) 1991 InterSoft Co.  All Rights Reserved
* THIS IS UNPUBLISHED PROPRIETARY SOURCE CODE OF InterSoft Co.
* The copyright notice above does not evidence any
* actual or intended publication of such source code.
*
*
* $Id: strstbuf.cc,v 1.3 2007/03/07 15:24:57 hardaiz Exp $
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

void String::store(char *&buff) const
{
	Int l;
	if (p == (struct srep *) HIGH_VALUE)
		l = INT_MAX;
	else if (p == (struct srep *) LOW_VALUE)
		l = 0;
	else
		l = p->len();
	stlong(l, buff);
	buff += sizeof(l);
	if (l == 0 || l == INT_MAX)
		return;
	mCopy(buff, p->s(), l);
	buff += l;
}
