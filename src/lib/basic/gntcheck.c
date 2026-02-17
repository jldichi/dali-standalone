/********************************************************************
* Copyright (c) 1987 InterSoft Co.  All Rights Reserved
* THIS IS UNPUBLISHED PROPRIETARY SOURCE CODE OF InterSoft Co.
* The copyright notice above does not evidence any
* actual or intended publication of such source code.
*
*
* $Id: gntcheck.c,v 1.1.1.1 1996/04/16 18:52:02 gustavof Exp $
*
*
* DESCRIPTION:
*
* ROUTINE       |  PURPOSE
*---------------+---------------------------------------------------
*TestCheckDigit	|R:The value of the string if it verifies,if not :ERR
*
*
*********************************************************************/

#include <ideafix.h>

/*
*	Dado un string que contiene un numero con su ponderador 
*   retorna el mismo convertido a long sin check-digit o
*   ERR si el check-digit no coincide
*/

long TestCheckDigit(register char *s)
{
	register long aux;

	if (s[0]==0) return ERR;
	aux = 0;
	while (s[1]) aux = aux * 10 + *s++ -'0';
	return CheckDigit(aux) == (s[0]-'0') ? aux : ERR;
}
