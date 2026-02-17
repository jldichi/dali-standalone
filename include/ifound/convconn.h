/****************************************************************
* Copyright (c) 1992 InterSoft Co.  All Rights Reserved
* THIS IS UNPUBLISHED PROPRIETARY SOURCE CODE OF InterSoft Co.
* The copyright notice above does not evidence any
* actual or intended publication of such source code.
*
* $Id: convconn.h,v 1.1.1.1 1996/04/16 18:51:31 gustavof Exp $
*
* DESCRIPTION:
*
*
*********************************************************************/

#ifndef CONVCONN_H
#define CONVCONN_H

#include <essentia/dbmcom.h>

inline void invswap(char *a, char *b)
{ 
	char c = *a; 
	*a = *b; 
	*b = c; 
}

/*
 * devuelve un short con el valor obtenido de invertir los bytes
 * de num.
 */
inline short invS(short n)
{
	invswap((char *)&n, ((char *) &n) + 1);
	return n;
}

/*
 * devuelve un long con el valor obtenido de invertir los bytes
 * de num. O sea, 1234 -> 4321.
 */
inline Int invI(Int n)
{
	invswap((char *) &n, ((char *) &n) + 3);
	invswap(((char *) &n) + 1, ((char *) &n) + 2);
	return n;
}

/*
 * invierte los bytes de los distintos miembros del header.
 * Hay que usarla SIEMPRE antes de enviar un paquete y despues
 * de recibir otro.
*/
inline void invH(MsgBuff hd)
{
	hd->h.clientId = invI(hd->h.clientId);
	hd->h.objectId = invI(hd->h.objectId);
	hd->h.serverOp = invS(hd->h.serverOp);
	hd->h.flags_ = invS(hd->h.flags_);
}

#endif
