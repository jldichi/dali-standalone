/********************************************************************
* Copyright (c) 1991 InterSoft Co.  All Rights Reserved
* THIS IS UNPUBLISHED PROPRIETARY SOURCE CODE OF InterSoft Co.
* The copyright notice above does not evidence any
* actual or intended publication of such source code.
*
* $Id: strstr.cc,v 1.1.1.1 1996/04/16 18:51:58 gustavof Exp $
*
* DESCRIPTION
* Function to locate a string inside of another
*********************************************************************/

#include <ifound.h>

const char *strStr(const char *s1, const char *s2)
{
	register const char *p, *q;
	do {
		p = s1;
		q = s2; 
		while (*p++ == *q++)
			if (*q=='\0') return s1;
	} while (*++s1);
	return NULL;
}

//	Search for string s2 in s1 starting backwards from s3

const char * strStrBackw(const char *s1, const char *s2, const char *s3)
{
	register const char *p, *q;
	while (--s3 >= s1) {
		p = s3;
		q = s2; 
		while (*p++ == *q++)
			if (*q=='\0') return s3;
	}
	return NULL;
}

// end of strstr
