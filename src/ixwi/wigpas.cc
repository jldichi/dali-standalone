/********************************************************************
*
* Copyright (c) 1995 InterSoft Co.  All Rights Reserved
* THIS IS UNPUBLISHED PROPRIETARY SOURCE CODE OF InterSoft Co.
* The copyright notice above does not evidence any
* actual or intended publication of such source code.
*
* $Id: wigpas.cc,v 1.2 2004/09/29 21:36:17 damianm Exp $
*
* DESCRIPTION:
*
*********************************************************************/

#include <string.h>

#include <ifound.h>
#include <ifound/str.h>
#include <cfix.h>
#include <ideafix/priv/gndefs.h>

static inline bool editing(Int ret)
{
	return ret != ERR && ret != K_PROCESS && ret != K_END;
}

/*
	- 'user' y 'password' deben estar alocados con una long. minima de 9.
	- Si user tiene valor se interpreta como valor default, si no
	  lo tiene el valor default es el login del usuario que esta 
	  ejecutando el programa.
*/
extern "C" int WiGetPassword(char *user, char *password)
{
	if (user == NULL || password == NULL)
		return ERR;

	const Int loginLen = 8;
	const Int passwordLen = 8;
	Int ret;

	window wi = WiCreate(SCREEN, 5, 10, 5, 22, ALL_BORDER|DLINE_TYPE,
						 "", A_NORMAL);
	WiMoveTo(0, 2);
	WiPrintf("%s", (char*)_WiErrmsg(USER));
	WiMoveTo(1, 2);
	WiPrintf("%s", (char*)_WiErrmsg(PASSWD));
	WiMoveTo(2, 0);
	WiPrintf("　　　　　　　　　　　");
	WiMoveTo(3, 2);
	char aux[15];
	WiPrintf("%s %s", (char*)_WiErrmsg(CONFIRM)
						  , WiKeyHelp(K_PROCESS, (unsigned char *)aux));
	WiMoveTo(4, 2);
	WiPrintf("%s %s", (char*)_WiErrmsg(IGNORE)
						  , WiKeyHelp(K_END, (unsigned char *)aux));


	char login[loginLen+1],
		 password_[passwordLen+1],
		 tstmask[passwordLen+1],
		 omask[passwordLen+1];

	// Default login
	if (*user == 0)
		strcpy(login, userName(procUserId()));
	else {
		strncpy(login, user, loginLen);
		login[loginLen] = 0;
	}

	// Hidden password
	sprintf(password_, "%ld#x", passwordLen);
	ret = CompileMask(password_, tstmask, omask);
	if (ret == ERR) return ERR;
	*password_ = 0;

	do {

		// Get login
		WiMoveTo(0, 12);
		ret = WiGetField(TY_STRING, (UChar *)login, 0, loginLen, loginLen,
						 0, 0, 0);

		// Get password
		if (editing(ret)) {
			WiMoveTo(1, 12);
			ret = WiGetField(TY_STRING, (UChar *)password_, 0,
							 passwordLen, passwordLen, 0, tstmask, omask);
		}

	} while (editing(ret));

	WiDelete(wi);

	if (ret == K_PROCESS) {
		strcpy (user, login);
		strcpy (password, password_);
		return OK;
	}

	return ERR;
}
