/********************************************************************
* Copyright (c) 1991 InterSoft Co.  All Rights Reserved
* THIS IS UNPUBLISHED PROPRIETARY SOURCE CODE OF InterSoft Co.
* The copyright notice above does not evidence any
* actual or intended publication of such source code.
*
*
* $Id: datenum.cc,v 1.1.1.1 1996/04/16 18:51:55 gustavof Exp $
*
*
* DESCRIPTION:
*
*
*********************************************************************/

#include <ifound/itypes.h>
#include <ifound/priv/timedefs.h>

Date::Date(const Num &d, const Num &m, const Num &y)
{
	idate = _dmy_to_date(toInt(d), toInt(m), toInt(y));
}
