/********************************************************************
* Copyright (c) 1987 InterSoft Co.  All Rights Reserved
* THIS IS UNPUBLISHED PROPRIETARY SOURCE CODE OF InterSoft Co.
* The copyright notice above does not evidence any
* actual or intended publication of such source code.
*
* $Id: c_onstop.cc,v 1.1.1.1 1996/04/16 18:52:03 gustavof Exp $
*
* DESCRIPTION:
* Manejo de las terminaciones de los programas tanto
* usr como stdalone.
*
* ROUTINE       |  PURPOSE
*---------------+---------------------------------------------------
* _Stop			 Execute all functions loaded with OnStop()
*********************************************************************/

#include <ideafix.h>
#include <cfix.h>

extern "C" void OnStop(FP fun)
{
	onStop(fun);
}
