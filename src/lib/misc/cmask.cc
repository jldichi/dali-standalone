/********************************************************************
* Copyright (c) 1994 InterSoft Co.  All Rights Reserved
* THIS IS UNPUBLISHED PROPRIETARY SOURCE CODE OF InterSoft Co.
* The copyright notice above does not evidence any
* actual or intended publication of such source code.
*
* $Id: cmask.cc,v 1.3 2008/04/22 18:59:23 cristiand Exp $
*
* DESCRIPTION
*
* compileMask	 Compila una mascara de testeo de string.
*********************************************************************/

#include <ifound.h>
#include <ifound/chset.h>
#include <ifound/priv/tstmask.h>

static int processAttribute(char *&mask, char *first, int &ochar, 
							int &TstCharMask)
{
	int n;
	bool is_num = true;

	for (n = 0; isdigit(UnChar(*mask)); mask++)
		if ((n = n*10 + *mask - '0') >= MAX_MASK_LENGTH)
			return ERR; 
			
	ochar = '_';
	TstCharMask = 0;
	while (*mask) {
		switch (*mask++) {
		case '>' :
			TstCharMask |= TST_UPPER;
			continue;
		case '<' :
			TstCharMask |= TST_LOWER;
			continue;
		case 'H' :
			TstCharMask |= TST_REQ;
		case 'h' :
			TstCharMask |= TST_HEXA;
			is_num = false;
			break;
		case 'A' :
			TstCharMask |= TST_REQ;
		case 'a' :
			TstCharMask |= TST_ALPHA;
			is_num = false;
			break;
		case 'N' :
			TstCharMask |= TST_REQ;
		case 'n' :
			TstCharMask |= TST_NUM;
			break;
		case 'X' :
			TstCharMask |= TST_REQ;
		case 'x' :
		case '_' :
			TstCharMask |= TST_ANUM;
			is_num = false;
			break;
		case '#' :
			ochar = '#';
			continue;
		case 'C' :
			TstCharMask |= TST_REQ;
		case 'c' :
			if (*mask || n || is_num == false)
				return ERR;
			TstCharMask |= TST_CHK_DIG;
			ochar = TST_OCHK_DIG;
			if (first != NULL) *first = 'L';
			break;
		case '\\' :
			mask++;
		default :
			ochar = mask[-1];
		}
		break;
	}
	return n == 0 ? 1 : n;
}

/*
*   compileMask(mask, tstmask, omask)
*
*				Compila una mascara de testeo de strings.
*				Retorna en tstmask la mascara de bits de chequeo.
*				Retorna en omask la mascara de formateo de salida.
*				Any of the values can be null, then nothing is copied.
*				If the mask exceed the MAX_MASK_LENGTH return ERR.
*				else return OK;
*
*/
Int compileMask(char *mask, char *tstmask, char *omask)
{
	int n, ochar, TstCharMask;
	short mask_length = 1;
	char *first = omask;

	if (omask) *omask++ = 'l';

	while (*mask) {
		if ((n = processAttribute(mask, first, ochar, TstCharMask)) == ERR)
			return ERR;
		do {
			if (++mask_length >= MAX_MASK_LENGTH) return ERR;  
			if (omask) *omask++ = char(ochar);
			if (TstCharMask && tstmask) *tstmask++ = char(TstCharMask);
		} while (--n > 0);
	}

	

	if (tstmask) *tstmask = '\0';
	if (omask) *omask = '\0';
	return OK;
}

Int sizeOfMask(char *mask)
{
	int n, dummy, mask_length = 0;

	while (*mask) {
		if ((n = processAttribute(mask, NULL, dummy, dummy)) == ERR)
			return ERR;
		mask_length += n;
		if (mask_length >= MAX_MASK_LENGTH)
			return ERR;
	}
	return mask_length;
}
