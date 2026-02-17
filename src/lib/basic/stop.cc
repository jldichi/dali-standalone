/********************************************************************
* Copyright (c) 1994 InterSoft Co.  All Rights Reserved
* THIS IS UNPUBLISHED PROPRIETARY SOURCE CODE OF InterSoft Co.
* The copyright notice above does not evidence any
* actual or intended publication of such source code.
*
* $Id: stop.cc,v 1.1.1.1 1996/04/16 18:52:03 gustavof Exp $
*
* DESCRIPTION
* stop() function for programs that are not linked with ixdb or esdb
* libraries.
*********************************************************************/

#include <ifound.h>

void _stop(int ex_vl);

/*
	When compiling with shared libraries (at least in Interactive)
	you can not have two definitions of the same function in
	different libraries.  It is going to be defined in libixdb and
	libesdb.
*/

//#if !defined(SHIDEAIX) && !defined(SHIDEAES) && !defined(SAMENAMESHD)

void stop(int ex_vl)
{
	_stop(ex_vl);
}

//#endif
