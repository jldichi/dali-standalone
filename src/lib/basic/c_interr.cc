/********************************************************************
* Copyright (c) 1987 InterSoft Co.  All Rights Reserved
* THIS IS UNPUBLISHED PROPRIETARY SOURCE CODE OF InterSoft Co.
* The copyright notice above does not evidence any
* actual or intended publication of such source code.
*
*
* $Id: c_interr.cc,v 1.1.1.1 1996/04/16 18:52:01 gustavof Exp $
*
*
* DESCRIPTION:
*
* ROUTINE       |  PURPOSE
*---------------+---------------------------------------------------
*
*********************************************************************/

#include <ideafix.h>
#include <cfix.h>

extern "C" void SetEnableIntHandler(FPI f)
{
	setEnableIntHandler(f);
}

extern "C" void EnableInterrupts(FPI fp)
{
	enableInterrupts(fp);
}
