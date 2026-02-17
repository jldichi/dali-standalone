/********************************************************************
* Copyright (c) 1987 InterSoft Co.  All Rights Reserved
* THIS IS UNPUBLISHED PROPRIETARY SOURCE CODE OF InterSoft Co.
* The copyright notice above does not evidence any
* actual or intended publication of such source code.
*
* $Id: c_fopenp.cc,v 1.1.1.1 1996/04/16 18:52:02 gustavof Exp $
*
* DESCRIPTION
*
* ROUTINE       |  PURPOSE
*---------------+---------------------------------------------------
*FopenPath		 Mira en el path si puede abrir el archivo pedido.
*********************************************************************/

#include <ideafix.h>
#include <cfix.h>
#include <ifound/str.h>
#include <ifound/priv/gfuncs.h>

extern "C" FILE *FopenPath(char *name, char *ext, char *mode)
{
	String file;
	if (findInPath(name, ext, NULL_STRING, file))
		return fopen(toCharPtr(file), mode);
	return NULL;
}
