/********************************************************************
* Copyright (c) 1987 InterSoft Co.  All Rights Reserved
* THIS IS UNPUBLISHED PROPRIETARY SOURCE CODE OF InterSoft Co.
* The copyright notice above does not evidence any
* actual or intended publication of such source code.
*
* $Id: gncheck.c,v 1.2 2006/02/17 15:55:10 spark Exp $
*
* DESCRIPTION:
*
* ROUTINE       |  PURPOSE
*---------------+---------------------------------------------------
*SetCheckFactor |Set the check factor for CheckDigit
*CheckDigit		|Return the Check digit
*********************************************************************/

#include <stdlib.h>
#include <ideafix.h>
#include <ideafix/priv/gndefs.h>

#if IDEAWIN
#	include <mswi/ideadll.h>
#	define ponderator	pDLL->ponderator
#else
static long ponderator = ERR;
#endif

void SetCheckFactor(long n)
{
#if IDEAWIN
	sDLL *pDLL = GetDLLData();
	if (!pDLL)
		return;
#endif
	ponderator = n;
}

/*

	The routine checks for the existence of the environment
	variable 'CHECKDIGIT', this variable should be set according to
	the following specifications:

		CHECKDIGIT=ponderator:module:accum


	Where the variables mean:

		ponderator:	the digits in this number are multiplied by the
					corresponding digit in the number to be evaluated.

		module:		the multiplied digits are added up and then
					the result of obtaining this module is returned.
		
		accum:		If this variable is 1, the result of the
					multiplication that was mentioned above is added
					up, if this variable is 0, the modulo (10) of
					each intermediate operation is taken.
					If this variable is 2 (each digit is added up
					independently).


	If the variable does not exist, it is equivalent to:

		CHECKDIGIT=1397139713:10:0


	For negative numbers, the result is ERR.
	The other case in which ERR is returned, is when the resulting
	number is 10 or greater (only possible when the modulo number is
	over 10).

*/
int CheckDigit(register long n)
{
    register int j, k;
	register long m;
	static int accum = 0;
	static int mod = 10;
	int temp;

#if IDEAWIN
	sDLL *pDLL = GetDLLData();
	if (!pDLL)
		return ERR;
#endif

	if (n < 0)
		return ERR;

	if (ponderator == ERR) {
		FPCPCPI oldHandler = SetReadEnvHandler(NULLFP);
		char *p = ReadEnv("CHECKDIGIT");
		(void) SetReadEnvHandler(oldHandler);

		if (p == NULL || p[0] == '\0')
			ponderator = 1397139713;
		else {
			ponderator = atol(p);
			p = strchr(p, ':');
			if (p != NULL) {
				mod = atoi(p+1);
				p = strchr(p+1, ':');
				if (p != NULL)
					accum = atoi(p+1);
			}
		}
	}

	m = ponderator;

	switch (accum) {
	/*
		Do not accumulate (each digit's remainder)
	*/
	case 0:
		for (j = 0; n > 0; n /= 10, m /= 10)
			j += INT(n % 10) * INT(m % 10) % 10;
		break;
	/*
		Accumulate (each digit's remainder)
	*/
	case 1:
		for (j = 0; n > 0; n /= 10, m /= 10)
			j += INT(n % 10) * INT(m % 10);
		break;
	/*
		Add up each digit independently when there is remainder
	*/
	default:
		for (j = 0; n > 0; n /= 10, m /= 10)
			for (k = INT(n % 10) * INT(m % 10); k > 0; k /= 10)
				j += INT(k % 10);
		break;
	}

    if ((temp = (mod - j % mod) % mod) >= 10)
		return ERR;
	else
		return temp;
}
