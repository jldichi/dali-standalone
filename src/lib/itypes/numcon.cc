/********************************************************************
* Copyright (c) 1991 InterSoft Co.  All Rights Reserved
* THIS IS UNPUBLISHED PROPRIETARY SOURCE CODE OF InterSoft Co.
* The copyright notice above does not evidence any
* actual or intended publication of such source code.
*
* $Id: numcon.cc,v 1.1.1.1 1996/04/16 18:51:55 gustavof Exp $
*
* DESCRIPTION
* save and load num to connection
*********************************************************************/

#include <ifound.h>
#include <ifound/itypes.h>
#include <essentia/connect.h>
#include <ifound/numdefs.h>

Connection &operator<<(Connection &c, const Num &num)
{
	SENDNUM(c, operator<<, num);
	return c;
}

Connection &operator>>(Connection &c, Num &num)
{
	RECEIVENUM(c, operator>>, num, c.counter());
	return c;
}
