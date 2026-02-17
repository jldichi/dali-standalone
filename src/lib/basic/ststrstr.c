/********************************************************************
* Copyright (c) 1987 InterSoft Co.  All Rights Reserved
* THIS IS UNPUBLISHED PROPRIETARY SOURCE CODE OF InterSoft Co.
* The copyright notice above does not evidence any
* actual or intended publication of such source code.
*
* $Id: ststrstr.c,v 1.1.1.1 1996/04/16 18:52:03 gustavof Exp $
*
* DESCRIPTION:
*      String searching routines.
*
* ROUTINE       |  PURPOSE
*---------------+---------------------------------------------------
* strstr		| Return a pointer to the first ocurrence of s2 in s1
* strfld		| Get the n-field delimited by a string
*********************************************************************/

#include <ideafix.h>
#include <ideafix/priv/gndefs.h>


/*
	Do not redefine this function in dos environment cause we have it
	in the library supplied with the compiler M.B. 
*/

#ifndef HAVE_STRSTR

char *strstr(const char *s1, const char *s2)
{
	register char *p, *q;
	do {
		p = (char *) s1;
		q = (char *) s2; 
		while (*p++ == *q++)
			if (*q == '\0')
				return (char *) s1;
	} while (*++s1);
	return NULL;
}

#else
char *strstr(const char *s1, const char *s2);
#endif


char *strfld(char *dest, char *s1, char *s2, int n)
{
	while (n--) {
		s1 = strstr(s1, s2);
		if (s1 == NULL) {
			s1 = "";
			break;
		}
		s1 += strlen(s2);
	}
	strcpy(dest, s1);
	s1 = strstr(dest, s2);
	if (s1)
		*s1 = '\0';
	return dest;
}
