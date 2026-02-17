/********************************************************************
* Copyright (c) 1991 InterSoft Co.  All Rights Reserved
* THIS IS UNPUBLISHED PROPRIETARY SOURCE CODE OF InterSoft Co.
* The copyright notice above does not evidence any
* actual or intended publication of such source code.
*
*
* $Id: memcmpi.cc,v 1.1.1.1 1996/04/16 18:51:58 gustavof Exp $
*
*
* DESCRIPTION:
*				Compare two areas of memory, no case sense
*				Returns :		< 0 	if a < b
*								0		if a = b
*								> 0		if a > b
*
*********************************************************************/

#include <ifound.h>
#include <ifound/chset.h>

Int memCmpI(const char *a, const char *b, Int s)
{
	Int i = -1, result = 0;
	while (++i < s) 
		if ((result = toUpperCase(a[i]) - toUpperCase(b[i])))
			break;
	return result;
}

// end of memcmpi
