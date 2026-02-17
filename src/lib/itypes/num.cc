/********************************************************************
* Copyright (c) 1991 InterSoft Co.  All Rights Reserved
* THIS IS UNPUBLISHED PROPRIETARY SOURCE CODE OF InterSoft Co.
* The copyright notice above does not evidence any
* actual or intended publication of such source code.
*
* $Id: num.cc,v 1.1.1.1 1996/04/16 18:51:53 gustavof Exp $
*
* DESCRIPTION
* Implementation of Num class
*********************************************************************/

#include <ifound.h>
#include <ifound/itypes.h>

Num &Num::operator=(const Num &a) 
{
	if (this != &a) {
		exp = a.exp;
		len = a.len;
		for (Int i = 0; i < NUM_SHORTS; i++)
			s[i] = a.s[i];
	}
	return *this;
}
