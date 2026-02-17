/********************************************************************
* Copyright (c) 1987 InterSoft Co.  All Rights Reserved
* THIS IS UNPUBLISHED PROPRIETARY SOURCE CODE OF InterSoft Co.
* The copyright notice above does not evidence any
* actual or intended publication of such source code.
*
* $Id: c_cmask.cc,v 1.1.1.1 1996/04/16 18:52:02 gustavof Exp $
*
* DESCRIPTION:
*
* ROUTINE       |  PURPOSE
*---------------+---------------------------------------------------
* CompileMask	 Compila una mascara de testeo de string.
*********************************************************************/

#include <ideafix.h>
#include <cfix.h>

extern "C" int CompileMask(char *mask, char *tstmask, char *omask)
{
	return compileMask(mask, tstmask, omask);
}

extern "C" int SizeOfMask(char *mask)
{
	return sizeOfMask(mask);
}
