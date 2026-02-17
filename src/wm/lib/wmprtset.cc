/********************************************************************
* Copyright (c) 1987 InterSoft Co.  All Rights Reserved
* THIS IS UNPUBLISHED PROPRIETARY SOURCE CODE OF InterSoft Co.
* The copyright notice above does not evidence any
* actual or intended publication of such source code.
*
*
* $Id: wmprtset.cc,v 1.1.1.1 1996/04/16 18:50:23 gustavof Exp $
*
*
* DESCRIPTION:
*      Descripcion del proposito general del modulo
*
* ROUTINE       |  PURPOSE
*---------------+---------------------------------------------------
*_print_ichset()	|Print the idea character set.
*********************************************************************/

#include <ideafix.h>
#include <cfix.h>

#include <ifound/str.h>

#include <wm.h>

#ifdef NO_SERVICES
void _print_ichset() { }
#else

/* ++ Prototypes ++ */

static char *getcline(char *fmt, int n);

/* -- Prototypes -- */


void _print_ichset(void)
{
	char fmt[32];

	static String selBase, decBase, hexBase, octBase;
	
	if (selBase == NULL_STRING) {
		selBase = appMsg("SELBASE");
		decBase = appMsg("DECBASE");
		hexBase = appMsg("HEXBASE");
		octBase = appMsg("OCTBASE");
	}
	
	switch (PopUpLMenu(6, 20, (char*)toCharPtr(selBase), 
							  (char*)toCharPtr(decBase), 
							  (char*)toCharPtr(hexBase),
							  (char*)toCharPtr(octBase), (char*)NULL)) {
	case 1:
		(void) strcpy(fmt, " %02x %c %c"); break;
	case 2:
		(void) strcpy(fmt, " %03o %c %c"); break;
	case ERROR:
		return;
	default:
		(void) strcpy(fmt, " %03d %c %c"); break;
	}
	WiHelp(12, 50, _WmErrmsg("CHSET"), getcline, fmt);
}

static char *getcline(char *fmt, int n)
{
	static char _far linea[100];
	register int i,num,c; register char *l;

	if(n < 1 || n > 38)
		return NULL;

	for(i=0, l=linea, num=6*(n-1)+' '; i < 6; i++, num++) {
		c = num;

		if(num > 255) return linea;
		
		if(num >=127 && num <= 159)
			c = ' ';

		(void) sprintf(l, fmt, num, c, C_VERT);
		l+=strlen(l);
	}

	return linea;
}
#endif /* NO_SERVICES */
