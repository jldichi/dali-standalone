/********************************************************************
* Copyright (c) 1991 InterSoft Co.  All Rights Reserved
* THIS IS UNPUBLISHED PROPRIETARY SOURCE CODE OF InterSoft Co.
* The copyright notice above does not evidence any
* actual or intended publication of such source code.
*
*
* $Id: strhash.cc,v 1.1.1.1 1996/04/16 18:51:54 gustavof Exp $
*
*
* DESCRIPTION: Calculate and return de hash value of a string.
*				
*
*********************************************************************/


#include <ifound/itypes.h>

#include <ifound/priv/itypesp.h>


Int hashVal(const String &s, Int modulo)
{
	if (!VALID_PTR(s.p))
		return Int(s.p) % modulo;

	unsigned long ret = 0;
	register char *i, *upto;

	for (i = s.p->s(), upto = i + s.length(); i < upto; i++)
		ret = *i + 31L * ret;

	return (ret % modulo);
}

// end of strhash
