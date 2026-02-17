/********************************************************************
* Copyright (c) 1987 InterSoft Co.  All Rights Reserved
* THIS IS UNPUBLISHED PROPRIETARY SOURCE CODE OF InterSoft Co.
* The copyright notice above does not evidence any
* actual or intended publication of such source code.
*
* $Id: c_crypt.cc,v 1.1.1.1 1996/04/16 18:52:02 gustavof Exp $
*
* DESCRIPTION
* Encryption routine.
*
* ROUTINE         |  PURPOSE
*-----------------+---------------------------------------------------
* InitCrypt(pw,s) | Init the encryption process
* CryptChar(c)	  | Encrypt a character
*********************************************************************/

#include <ideafix.h>

extern "C" void InitCrypt(char *pw, int sflag)
{
	initCrypt(pw, sflag);
}

extern "C" int CryptChar(int i)
{
	return cryptChar(i);
}

extern "C" void CryptString(char *str)
{
	cryptString(str);
}
