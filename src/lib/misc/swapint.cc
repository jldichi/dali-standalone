/********************************************************************
* Copyright (c) 1991 InterSoft Co.  All Rights Reserved
* THIS IS UNPUBLISHED PROPRIETARY SOURCE CODE OF InterSoft Co.
* The copyright notice above does not evidence any
* actual or intended publication of such source code.
*
* $Id: swapint.cc,v 1.1.1.1 1996/04/16 18:51:57 gustavof Exp $
*
* DESCRIPTION
* Swap two Ints
*********************************************************************/

#include <ifound.h>

void swap(Int &a, Int &b)
{
	Int tmp = a;
	a = b;
	b = tmp;
}
