/********************************************************************
* Copyright (c) 1987 InterSoft Co.  All Rights Reserved
* THIS IS UNPUBLISHED PROPRIETARY SOURCE CODE OF InterSoft Co.
* The copyright notice above does not evidence any
* actual or intended publication of such source code.
*
*
* $Id: c_stop.cc,v 1.1.1.1 1996/04/16 18:52:00 gustavof Exp $
*
*
* DESCRIPTION:
*      Library link routine.
*
* ROUTINE       |  PURPOSE
*---------------+---------------------------------------------------
*Stop			 calls _Stop
*********************************************************************/

#include <ideafix.h>

/* +++ Prototypes +++ */

void stop(int ex_vl);

/* --- Prototypes --- */


/*
	When compiling with shared libraries (at least in Interactive)
	you can not have two definitions of the same function in
	different libraries.  It is going to be defined in libixdb and
	libesdb.
*/
//#if !defined(SHIDEAIX) && !defined(SHIDEAES) && !defined(SAMENAMESHD)
//#if !defined(SHIDEAIX) && !defined(SAMENAMESHD)

extern "C" void Stop(int ex_vl)
{
	stop(ex_vl);
}

//#endif
