/********************************************************************
* Copyright (c) 1991 InterSoft Co.  All Rights Reserved
* THIS IS UNPUBLISHED PROPRIETARY SOURCE CODE OF InterSoft Co.
* The copyright notice above does not evidence any
* actual or intended publication of such source code.
*
* $Id: strfname.cc,v 1.2 1997/11/18 16:05:45 guiller Exp $
*
* DESCRIPTION
* File name expansion routine.
*********************************************************************/

#include <ifound.h>
#include <ifound/str.h> 
#include <ifound/chset.h>
#include <ifound/priv/itypesp.h>
#include <ifound/priv/gfuncs.h>

String String::expandEnvVars() const
{
	if (!VALID_PTR(p))
		return *this;

	String tmp;
	char *ptr, *aux;
	for (ptr = p->s(); *ptr; ptr++) {
		switch (*ptr) {
		case '\\':
#ifndef __NT__
			tmp += String(1, *++ptr);
#else
			tmp += String(1, *ptr);
#endif
			break;
		case '$':
			aux = ptr;
			while (isalnum(*++aux))
				;
			tmp += String(ptr+1, aux-ptr-1).getEnv();
			ptr = aux-1;
			break;
		case '~':
			aux = ptr;
			while (isalnum(*++aux))
				;
			if (aux == ptr+1)
				tmp += String("HOME").getEnv();
			else {
				String uName = String(ptr + 1, aux - ptr - 1);
				int uid = userId(toCharPtr(uName));
				if (uid != ERR) {
					tmp += userHomeDir(uid);
				}
			}
			ptr = aux-1;
			break;
		default:
			tmp += String(1, *ptr);
			break;
		}
	}
	return tmp;
}
