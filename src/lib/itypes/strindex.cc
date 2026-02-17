/********************************************************************
* Copyright (c) 1991 InterSoft Co.  All Rights Reserved
* THIS IS UNPUBLISHED PROPRIETARY SOURCE CODE OF InterSoft Co.
* The copyright notice above does not evidence any
* actual or intended publication of such source code.
*
* $Id: strindex.cc,v 1.1.1.1 1996/04/16 18:51:54 gustavof Exp $
*
* DESCRIPTION
* Implementation of String class
*********************************************************************/

#include <ifound.h>
#include <ifound/str.h>
#include <ifound/priv/itypesp.h>

Int String::index(const String &s, Int pos) const
{
	if (!VALID_PTR(p) || !VALID_PTR(s.p) || pos >= p->len())
		return ERR;
		
	char *aux;
	if ((aux = (char *) strStr(p->s() + pos, s.p->s())) != NULL)
	   	return aux - p->s();
	return ERR;
}
