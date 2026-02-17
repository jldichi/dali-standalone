/********************************************************************
* Copyright (c) 1987 InterSoft Co.  All Rights Reserved
* THIS IS UNPUBLISHED PROPRIETARY SOURCE CODE OF InterSoft Co.
* The copyright notice above does not evidence any
* actual or intended publication of such source code.
*
* $Id: wmgets.cc,v 1.1.1.1 1996/04/16 18:50:22 gustavof Exp $
*
* DESCRIPTION
* Window Manager Library module to read a string.
*
* ROUTINE       |  PURPOSE
*---------------+---------------------------------------------------
*_WiGets			 Read a string.
*********************************************************************/

#include <ideafix.h>
#include <cfix.h>

#ifndef IDEAWIN
#	include <wm.h>
#	include <ideafix/priv/wmcom.h>
#endif

/* Private Functions												*/
/*
 * WiGets: lee un string. Termina cuando se ingresa K_ENTER.
 *         Reconoce K_BACKSPACE para borrar. Solo admite
 *         caracteres imprimibles.
 */

#ifdef IDEAWIN
UChar* WiGets(UChar * s)
#else
void _WiGets(void)
#endif
{
	register UChar *p;
	UChar c;
#ifndef IDEAWIN
	UChar *s = wm_par[0].s;
#endif

	p = s;
	while ( (c = WiGetc()) != K_ENTER ) {
		switch (c) {
		case K_BACKSPACE:
			if (p > s){
				--p;
				WiPuts("\b \b");
			}
			else
				WiBeep();
			break;

		default:
			if (isprint(c)) {
				*p++ = c;
				WiPutc((UChar)c);
			}
			else
				WiBeep();
			break;
		}
	}
	*p = 0;
#ifdef IDEAWIN
	return s;
#else
	wm_par[0].s = s;
#endif
}
