/********************************************************************
* Copyright (c) 1987 InterSoft Co.  All Rights Reserved
* THIS IS UNPUBLISHED PROPRIETARY SOURCE CODE OF InterSoft Co.
* The copyright notice above does not evidence any
* actual or intended publication of such source code.
*
*
* $Id: widefpar.cc,v 1.1.1.1 1996/04/16 18:52:16 gustavof Exp $
*
*
* DESCRIPTION:
*
*
* ROUTINE       |  PURPOSE
*---------------+---------------------------------------------------
*
*********************************************************************/

#include <ideafix.h>
#include <cfix.h>
#include <ideafix/priv/gndefs.h>
#include <cracker/pserconn.h>

/* comienza con default parent en SCREEN */
static window defpar = SCREEN;

/* retorna el default parent corriente */
window WiDefPar(void)
{
	if (!crackerClient())
		return defpar;
	return (window) prServerConn(PrCmd::wiDefPar).result();
}

/* setea el default parent con la window pasada como parametro */
void WiSetDefPar(window wi)
{
	if (!crackerClient())
		defpar = wi;
	else {
		prServerConn(PrCmd::wiSetDefPar) << (Int) wi;
		prServerConn.flush();
	}
}
