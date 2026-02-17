/********************************************************************
* Copyright (c) 1987 InterSoft Co.  All Rights Reserved
* THIS IS UNPUBLISHED PROPRIETARY SOURCE CODE OF InterSoft Co.
* The copyright notice above does not evidence any
* actual or intended publication of such source code.
*
*
* $Id: strmatch.c,v 1.1.1.1 1996/04/16 18:52:03 gustavof Exp $
*
*
* DESCRIPTION:
*
* ROUTINE       |  PURPOSE
*---------------+---------------------------------------------------
*RexpMatch       Match a very simple reg. expression
*
*********************************************************************/

#include <ideafix.h>
#include <ideafix/priv/gndefs.h>

/*
 * 'a' contains a regular expression with '*' and '?' and '^' and '('.
 * 'b' is the candidate to match.
 */
#define char_eq(c1,c2) ((flags&REXP_NOCASE)?\
				toupper(toascii((int)c1))==toupper(toascii((int)c2)):\
							c1==c2)

bool RexpMatch(UChar *a, UChar *b, int flags)
{
	UChar *astera = NULL, *asterb = NULL, *p;

	if (flags & REXP_EMBEDDED) {
		if (*a == '^') ++a;
		else {
			astera = a;
			asterb = b;
		}
		p = a + strlen((char*)a)-1;
		if (*p == '$') {
			flags &= ~REXP_EMBEDDED;
			*p = '\0';
		}
	}

	forever {
		while (*a) {
			if (*a == '*') {
				astera = ++a;
				asterb = b;
			}
			if (*a == '?' || *a == '.' || char_eq(*a, *b) ) {
				if ((*a == '?' || *a == '.') && !*b)
					return FALSE;

				a++;
				b++;
			}
			else if (astera) {
				if (*b == 0) return FALSE;
				a = astera;
				b = ++asterb;
			}
			else return FALSE;
		}
		if (*b == 0 || (flags&REXP_EMBEDDED) || (astera && *astera==0)) 
			return TRUE;
		if (astera==NULL) return FALSE;
		a = astera;
		b = ++asterb;
	}
}
