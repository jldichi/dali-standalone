/********************************************************************
* Copyright (c) 1987 InterSoft Co.  All Rights Reserved
* THIS IS UNPUBLISHED PROPRIETARY SOURCE CODE OF InterSoft Co.
* The copyright notice above does not evidence any
* actual or intended publication of such source code.
*
* $Id: toulcase.cc,v 1.1.1.1 1996/04/16 18:51:57 gustavof Exp $
*
* DESCRIPTION
*
* ROUTINE       |  PURPOSE
*---------------+---------------------------------------------------
* toUpperCase	| Convert a character to UpperCase.
* toLowerCase	| Convert a character to LowerCase.
*********************************************************************/

#include <ifound.h>
#include <ifound/chset.h>

extern "C" int toLowerCase(int x)
{
	return isupper((unsigned char) (x &= 0xFF)) ? _tolower(x) : x;
}

extern "C" int toUpperCase(int x)
{
	return islower((unsigned char) (x &= 0xFF) ) ? _toupper(x) : x;
}




