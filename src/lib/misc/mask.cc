/********************************************************************
* Copyright (c) 1994 InterSoft Co.  All Rights Reserved
* THIS IS UNPUBLISHED PROPRIETARY SOURCE CODE OF InterSoft Co.
* The copyright notice above does not evidence any
* actual or intended publication of such source code.
*
* $Id: mask.cc,v 1.3 2004/09/29 15:30:07 damianm Exp $
*
* DESCRIPTION:
*
* ROUTINE       |  PURPOSE
*---------------+---------------------------------------------------
* processMask	|formatea un string de acuerdo a una mascara.
*********************************************************************/

#include <string.h>

#include <ifound.h>
#include <ifound/stdc.h>

/*
*   processMask: formatea un string de acuerdo con una mascara de tipo:
*         {r,l}__.__,__/__
*
*			r = rigth justification.
*			l = left  justification.
*			R = rigth justification fill with blanks.
*			L = left  	 "      "    "    "    "  ".
*
*   parametros : 1  char* obuff, string donde guarda el resultado
*                2  char* mask, string que contiene la mascara
*                3  char* ibuff, string a formatear
*/

void processMask(char *obuff, char *mask, char *ibuff)
{
	bool fill, rj;

	fill = rj = false;

	switch (*mask++) {
	case 'L' :
		fill = true;
	case 'l' :
		break;
	case 'R' :
		fill = true;
	case 'r' :
		rj = true;
		break;
	default :
		mask--;
	}

    if (rj) {
		char *ip, *mp;
        ip = ibuff + strlen(ibuff);
		mp = mask;
		while (*mp)
			if (*mp++ != '\\') ++obuff;
        *obuff-- = 0;

        while (mp-- > mask) {
			if (*mp == '\\' && obuff[1] && (ip>=ibuff || fill)) {
				if (mp[1] == '_') {
					if (ip>ibuff || obuff[1] != ' ') ++ip;
					obuff[1] = mp[1];
				}
			}
			else
				*obuff-- = ip == ibuff && !fill ? (char) ' '
						 : *mp != '_' ? *mp
						 : ip == ibuff  ? (char) ' ' : *--ip;
		}
			
    }
    else {
	// original -> while (*mask && (fill || *ibuff)) {
        while (*mask && (fill || (ibuff && *ibuff != '\0'))) { //albertoi
			*obuff++ = (*mask == '\\') ? *++mask
					 : (*mask != '_')  && (*mask != '#') ? *mask
					 : (*ibuff== '\0') || (*ibuff++ == '\1') ? (char) ' '
					 : (*mask=='#') ? (char) '#'
					 : ibuff[-1];
			mask++;
		}
        *obuff = 0;
    }
}
