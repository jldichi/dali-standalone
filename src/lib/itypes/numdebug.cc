/********************************************************************
* Copyright (c) 1991 InterSoft Co.  All Rights Reserved
* THIS IS UNPUBLISHED PROPRIETARY SOURCE CODE OF InterSoft Co.
* The copyright notice above does not evidence any
* actual or intended publication of such source code.
*
* $Id: numdebug.cc,v 1.1.1.1 1996/04/16 18:51:55 gustavof Exp $
*
* DESCRIPTION
* Implementation of Num class
*********************************************************************/

#include <ifound.h>
#include <ifound/itypes.h>

void Num::debug(const char *name)
{
	printf("%s = exp %d ", name, exp, len);
	if (len)
		putchar('[');
	for (int i=0; i<len; i++)
		printf("%d%c ", s[i], i<len-1 ? ',' : ']');
	printf("(%d)", len);
	putchar('\n');
}
