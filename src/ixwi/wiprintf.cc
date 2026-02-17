/********************************************************************
* Copyright (c) 1987 InterSoft Co.  All Rights Reserved
* THIS IS UNPUBLISHED PROPRIETARY SOURCE CODE OF InterSoft Co.
* The copyright notice above does not evidence any
* actual or intended publication of such source code.
*
* $Id: wiprintf.cc,v 1.1.1.1 1996/04/16 18:52:16 gustavof Exp $
*
* DESCRIPTION
* Window function Library.
*
* ROUTINE       |  PURPOSE
*---------------+---------------------------------------------------
*WiPrintf		 printf on a window.
*WiPuts			 Print a string on a window.
*********************************************************************/

#include <ideafix.h>
#include <cfix.h>
#include <ideafix/priv/gndefs.h>
#include <ifound/priv/gfuncs.h>

/*
 * W_PRINTF: printf en una window.
 */

extern "C" void WiPrintf(char *fmt, ...)
{
	va_list ap;

	va_start(ap, fmt);
	doFormat((IFPICP)WiPutc, NULL, (const char *)fmt, ap);
	va_end(ap);
}

/* Impresion de un string */

extern "C" void WiPuts(char *s)
{
	WiPrintf("%s", s);
}
