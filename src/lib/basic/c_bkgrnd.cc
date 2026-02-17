/********************************************************************
* Copyright (c) 1987 InterSoft Co.  All Rights Reserved
* THIS IS UNPUBLISHED PROPRIETARY SOURCE CODE OF InterSoft Co.
* The copyright notice above does not evidence any
* actual or intended publication of such source code.
*
* $Id: c_bkgrnd.cc,v 1.1.1.1 1996/04/16 18:52:02 gustavof Exp $
*
* DESCRIPTION
* Stay in background
*********************************************************************/

#include <ideafix.h>
#include <cfix.h>

extern "C" void Background(void)
{
	background();
}
