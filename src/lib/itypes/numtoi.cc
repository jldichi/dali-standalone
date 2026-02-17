/********************************************************************
* Copyright (c) 1991 InterSoft Co.  All Rights Reserved
* THIS IS UNPUBLISHED PROPRIETARY SOURCE CODE OF InterSoft Co.
* The copyright notice above does not evidence any
* actual or intended publication of such source code.
*
* $Id: numtoi.cc,v 1.1.1.1 1996/04/16 18:51:54 gustavof Exp $
*
* DESCRIPTION
* Num conversion methods
*********************************************************************/

#include <ifound.h>
#include <ifound/itypes.h>

short toShort(const Num &num)
{
	Int aux = toInt(num);
	return aux > SHORT_MAX ? SHORT_MAX
		:  aux < SHORT_MIN ? SHORT_MIN
		:  aux;
}

char toByte(const Num &num)
{
	Int aux = toInt(num);
	return aux > BYTE_MAX ? BYTE_MAX
		:  aux < BYTE_MIN ? BYTE_MIN
		:  aux;
}
