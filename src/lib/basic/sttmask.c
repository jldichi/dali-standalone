/********************************************************************
* Copyright (c) 1987 InterSoft Co.  All Rights Reserved
* THIS IS UNPUBLISHED PROPRIETARY SOURCE CODE OF InterSoft Co.
* The copyright notice above does not evidence any
* actual or intended publication of such source code.
*
* $Id: sttmask.c,v 1.3 2006/02/17 15:59:25 spark Exp $
*
* DESCRIPTION:
*
* ROUTINE       |  PURPOSE
*---------------+---------------------------------------------------
*TstChrMask		 Check if character agree with the bit mask.
*********************************************************************/

#include <stdlib.h>
#include <ideafix.h>
#include <ideafix/priv/gndefs.h>
#include <ifound/priv/tstmask.h>

/* 
* Check if chr agree with the bit mask 'mask'.
* Do the necesaries conversions.
* If not agree return 0.
* Else return the char converted
* If the bit mask is TST_CHK_DIG,
* check against the third argument.
*/

UChar TstChrMask(UChar chr, char mask, char *value)
{
	if (chr == ' ' && !(mask & TST_REQ))
		return chr;
	if (iskey(chr))  {
		return (mask & TST_REQ) ? (UChar) 0 : chr;
	}
	if (mask & TST_UPPER) chr = (UChar) toupper((int)chr);
	else if (mask & TST_LOWER) chr = (UChar) tolower((int)chr);

	if (mask & TST_ANUM) return chr;
	if (mask & TST_NUM	 && isdigit(chr)) return chr;
	if (mask & TST_ALPHA && (isalpha(chr) || chr == ' ')) return chr;
	if (mask & TST_HEXA  && isxdigit(chr))  return chr;
#ifndef IDEADLL /* I've troubles with dll */
	if (mask & TST_CHK_DIG  && CheckDigit(atol(value))==(chr-'0'))  
		return chr;
#endif		
	return (UChar) 0;
}

/*
* Check if a given mask is numeric
*/

bool MaskIsNum(register char *mask)
{
	while (*mask) 
		if ((*mask++ & ~ TST_REQ) != TST_NUM) return FALSE;
	return TRUE;
}
