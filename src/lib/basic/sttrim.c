/********************************************************************
* Copyright (c) 1987 InterSoft Co.  All Rights Reserved
* THIS IS UNPUBLISHED PROPRIETARY SOURCE CODE OF InterSoft Co.
* The copyright notice above does not evidence any
* actual or intended publication of such source code.
*
*
* $Id: sttrim.c,v 1.1.1.1 1996/04/16 18:52:03 gustavof Exp $
*
*
* DESCRIPTION:
*
* ROUTINE       |  PURPOSE
*---------------+---------------------------------------------------
*StrTrim		 Delete initial and trailing spaces in a string.
*********************************************************************/

#include <ideafix.h>
#include <ideafix/priv/gndefs.h>

/*
 * StrTrim: Delete initial and trailing spaces in a string. 
 * 
 */
char *StrTrim(register char *s)
{
	register char *s1;

	/* Skip initial spaces */

	while (isspace((UChar) *s)) ++s; 

	/* Skip trailing Spaces */

	if (*s) {
		s1 = s + strlen(s) - 1;
		while (isspace((UChar) *s1)) --s1; 
		s1[1] = '\0';
	}
	return s;
}
