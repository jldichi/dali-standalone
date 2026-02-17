/********************************************************************
* Copyright (c) 1987 InterSoft Co.  All Rights Reserved
* THIS IS UNPUBLISHED PROPRIETARY SOURCE CODE OF InterSoft Co.
* The copyright notice above does not evidence any
* actual or intended publication of such source code.
*
* $Id: stmask.cc,v 1.2 2004/09/29 16:42:00 damianm Exp $
*
* DESCRIPTION:
*      Rutina para formateo de string de acuerdo a una mascara.
*
* ROUTINE       |  PURPOSE
*---------------+---------------------------------------------------
* StrMask		|Formatea un string de acuerdo a una mascara.
*               |Incluye el tratamiento del digito verificador.
*********************************************************************/

#include <stdlib.h>
#include <string.h>

#include <ideafix.h>
#include <cfix.h>
#include <ideafix/priv/gndefs.h>
#include <ifound/priv/tstmask.h>

extern "C" void StrMask(register char *obuff, register char *mask, 
			 register char *ibuff)
{
	register char *p;

	/* If the last char is for check digit, temporary delete it ! */
	if ((p = strrchr(mask, TST_OCHK_DIG)) != NULL)
		*p = '\0';

	/* Format without check digit */
	processMask(obuff,mask,ibuff);

	/* If check digit append it*/
	if (p != NULL && *ibuff) {
		while (*obuff)
			obuff++;

		*obuff++ = (char) (CheckDigit(atol(ibuff)) + '0');
		*obuff = '\0';
	}

	/* Rebuild the original mask */
	if (p) *p = TST_OCHK_DIG;

	return;
}
