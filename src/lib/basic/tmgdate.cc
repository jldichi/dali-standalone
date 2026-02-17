/********************************************************************
* Copyright (c) 1987 InterSoft Co.  All Rights Reserved
* THIS IS UNPUBLISHED PROPRIETARY SOURCE CODE OF InterSoft Co.
* The copyright notice above does not evidence any
* actual or intended publication of such source code.
*
* $Id: tmgdate.cc,v 1.1.1.1 1996/04/16 18:52:00 gustavof Exp $
*
* DESCRIPTION
* Library routine to return current date.
*
* ROUTINE		|	PURPOSE
*---------------+---------------------------------------------------
* Today			|	Return current date in INTERSOFT format.
* Hour			|	Return current time in INTERSOFT format.
*********************************************************************/

#include <ideafix.h>
#include <cfix.h>
#include <ifound/priv/timedefs.h>
#include <ifound/priv/gfuncs.h>

DATE Today(void)
{
	LDATE ret = _osdate_to_date(_osTime());
	return ret > MAX_DATE ? MAX_DATE
		:  ret < MIN_DATE ? MIN_DATE : (DATE) INT(ret);
}

TIME Hour(void)
{
	return (TIME) (_osTime() % 86400 / 2) + 1;
}
