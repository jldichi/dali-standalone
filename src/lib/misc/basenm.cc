/********************************************************************
* Copyright (c) 1994 InterSoft Co.  All Rights Reserved
* THIS IS UNPUBLISHED PROPRIETARY SOURCE CODE OF InterSoft Co.
* The copyright notice above does not evidence any
* actual or intended publication of such source code.
*
* basenm.cc,v 1.1.1.1 1996/04/16 18:51:57 gustavof Exp
*
* DESCRIPTION:
*
* ROUTINE       |  PURPOSE
*---------------+---------------------------------------------------
*baseName		|Return the Base file name of argument.
*********************************************************************/

#include <string.h>

#include <ifound.h>
#include <ifound/stdc.h>

char *baseName(char const *path)
{
	register char *p = (char *) path + strlen(path) - 1;

	while (p >= path && *p != PATH_SLASH_UNIV[0] && *p != PATH_SLASH[0]){
#if 0
		*p = tolower(*p);
		if( *p == '.') *p = '\0'; // Convert PROGRAM.EXE -> PROGRAM
#endif
		p--;
	}
	return (char *) (p + 1);
}
