/********************************************************************
* Copyright (c) 1991 InterSoft Co.  All Rights Reserved
* THIS IS UNPUBLISHED PROPRIETARY SOURCE CODE OF InterSoft Co.
* The copyright notice above does not evidence any
* actual or intended publication of such source code.
*
* $Id: numshft.cc,v 1.1.1.1 1996/04/16 18:51:54 gustavof Exp $
*
* DESCRIPTION:
* Numeric shift (base 10)
*********************************************************************/

#include <ifound.h>
#include <ifound/itypes.h>

Num Num::shift(int n) const
{
	Num num = *this;
	NumShift(&num, n);
	return num;
}
