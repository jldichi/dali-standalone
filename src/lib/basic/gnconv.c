/********************************************************************
* Copyright (c) 1987 InterSoft Co.  All Rights Reserved
* THIS IS UNPUBLISHED PROPRIETARY SOURCE CODE OF InterSoft Co.
* The copyright notice above does not evidence any
* actual or intended publication of such source code.
*
*
* $Id: gnconv.c,v 1.1.1.1 1996/04/16 18:52:01 gustavof Exp $
*
*
* DESCRIPTION:
*
* ROUTINE       |  PURPOSE
*---------------+---------------------------------------------------
*SetConvHandler | Set the field conversion handler
*_ConvHandler
*********************************************************************/

#include <ideafix.h>
#include <ideafix/priv/gndefs.h>

/* ++ Prototypes ++ */

static void conv_default(char *function, char *reason);

/* -- Prototypes -- */

static FPCP conv_handler = (FPCP) conv_default;

/* Private Functions */

static void conv_default(char *function, char *reason)
{
	Error(_GnErrmsg(CONV_ERROR), function,reason);
}


FPCP SetConvHandler(FPCP f)
{
	FPCP aux = conv_handler;
	conv_handler = f;
	return aux;
}

void _ConvHandler(char *function, char *reason)
{
	FPCPCP tmp = (FPCPCP) conv_handler;

	if (tmp != NULL)
		(*tmp)(function, reason);
}
