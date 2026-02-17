/********************************************************************
* Copyright (c) 1991 InterSoft Co.  All Rights Reserved
* THIS IS UNPUBLISHED PROPRIETARY SOURCE CODE OF InterSoft Co.
* The copyright notice above does not evidence any
* actual or intended publication of such source code.
*
* $Id: strbase.cc,v 1.1.1.1 1996/04/16 18:51:54 gustavof Exp $
*
* DESCRIPTION: Base name.
*********************************************************************/

#include <ifound/str.h> 
#include <ifound/priv/itypesp.h>

String String::baseName() const
{
	String ret = field(-1, PATH_SLASH_UNIV);
	ret = ret.field(0, ".");
	return ret;
}

String String::baseFullName() const
{
	String ret = field(-1, PATH_SLASH_UNIV);
	return ret;
}

String String::pathName() const
{
	if (!VALID_PTR(p))
		return NULL_STRING;

	UnShort aux = p->len();
	while (aux && p->s()[aux--] != PATH_SLASH_UNIV[0])
		;

	if (!aux && p->s()[aux+1] != PATH_SLASH_UNIV[0])
		return NULL_STRING;

	return String(p->s(), aux+1);
}

String String::extName() const
{
	if (String(field(-1, PATH_SLASH_UNIV)).index('.') != ERR) 
		return field(-1, "."); 
	else  
		return NULL_STRING;
}
