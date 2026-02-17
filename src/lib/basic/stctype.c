/********************************************************************
* Copyright (c) 1987 InterSoft Co.  All Rights Reserved
* THIS IS UNPUBLISHED PROPRIETARY SOURCE CODE OF InterSoft Co.
* The copyright notice above does not evidence any
* actual or intended publication of such source code.
*
* $Id: stctype.c,v 1.2 2003/11/05 19:21:57 albertoi Exp $
*
* DESCRIPTION:
*
* ROUTINE       |  PURPOSE
*---------------+---------------------------------------------------
* StrToUpper	| Convert a String to UpperCase.
* StrToLower	| Convert a String to LowerCase.
* StrToAscii	| Convert a String to LowerCase.
*********************************************************************/

#include <ideafix.h>
#include <ideafix/priv/gndefs.h>

void StrToLower(register char *p)
{
	if (p == NULL || p == HIGH_VALUE)
		return;

	while (*p) {
		switch(*p) {
			case 'Ñ': *p = 'ñ'; break;
			case 'Á': *p = 'á'; break;
			case 'É': *p = 'é'; break;
			case 'Í': *p = 'í'; break;
			case 'Ó': *p = 'ó'; break;
			case 'Ú': *p = 'ú'; break;
			default:
				*p = (char) tolower((unsigned char)(*p));
		}
		++p;
	}
}

void StrToUpper(register char *p)
{
	if (p == NULL || p == HIGH_VALUE)
		return;

	while (*p) {
		switch(*p) {
			case 'ñ': *p = 'Ñ'; break;
			case 'á': *p = 'Á'; break;
			case 'é': *p = 'É'; break;
			case 'í': *p = 'Í'; break;
			case 'ó': *p = 'Ó'; break;
			case 'ú': *p = 'Ú'; break;
			default:
				*p = (char) toupper((unsigned char)(*p));
		}
		++p;
	}
}
