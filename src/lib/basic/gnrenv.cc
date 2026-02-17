/********************************************************************
* Copyright (c) 1987 InterSoft Co.  All Rights Reserved
* THIS IS UNPUBLISHED PROPRIETARY SOURCE CODE OF InterSoft Co.
* The copyright notice above does not evidence any
* actual or intended publication of such source code.
*
* $Id: gnrenv.cc,v 1.1.1.1 1996/04/16 18:52:01 gustavof Exp $
*
* DESCRIPTION:
*
* ROUTINE       |  PURPOSE
*---------------+---------------------------------------------------
*SetReadEnvHandler|Set The read environment error handler.
*def_ReadEnv	|
*********************************************************************/

#include <ideafix.h>
#include <cfix.h>
#include <ideafix/priv/gndefs.h>
#include <ifound/priv/gfuncs.h>

static void renv_default(char *var, char *filename, int lineno)
{
	Error((char *) _GnErrmsg(NOENV), filename, lineno, var);
}

/* Public variables defined in this module                          */
static FPCPCPI renv_handler = renv_default;

extern "C" FPCPCPI SetReadEnvHandler(FPCPCPI f)
{
	FPCPCPI aux = renv_handler;
	renv_handler = f;
	return aux;
}

extern "C" char *def_ReadEnv(register char *x, char *filename, int lineno)
{
	const char *p = readEnvVar(x);
	if (p == NULL) {
		if (renv_handler) 
			(*renv_handler)(x, filename, lineno);
	}
	return (char*)p;
}
