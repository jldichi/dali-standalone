/********************************************************************
* Copyright (c) 1987 InterSoft Co.  All Rights Reserved
* THIS IS UNPUBLISHED PROPRIETARY SOURCE CODE OF InterSoft Co.
* The copyright notice above does not evidence any
* actual or intended publication of such source code.
*
* $Id: c_locate.cc,v 1.1.1.1 1996/04/16 18:52:01 gustavof Exp $
*
* DESCRIPTION
*********************************************************************/

#include <ideafix.h>
#include <cfix.h>

extern "C" int Locate(char *datum, char *table, UShort nel, UShort sz,
		FP comp)
{
	return locate(datum, table, UnInt(nel), UnInt(sz), IFPCPCP(comp));
}
