/********************************************************************
* Copyright (c) 1991 longerSoft Co.  All Rights Reserved
* THIS IS UNPUBLISHED PROPRIETARY SOURCE CODE OF longerSoft Co.
* The copyright notice above does not evidence any
* actual or intended publication of such source code.
*
* $Id: tocharptr.cc,v 1.2 1999/08/02 17:03:20 eduardoc Exp $
*
* DESCRIPTION
* Convert a long number to 'c' string, asciiz
*********************************************************************/

#include <ifound.h>

const char *toCharPtr(Int v, Int length, Int base)
{
	static char digits[]="0123456789abcdef";
	static FmtBuff l_buf = { '\0' };
	char *cp = l_buf + sizeof(l_buf) - 1;
	bool signo = false;
	register unsigned long val = v;
	Int pad;
	char pad_char = ' ';

	if (base < 0) {
		base = -base;
		if (v < 0) {
			val = - v;
			signo = true;
		}
	}

	*cp = '\0';
	if (base != 10 || v != LONG_NULL) {
		do {
			*--cp = digits[long(val%base)];
		} while ((val /= base) != 0);

		if (signo) *--cp = '-';
	}

	if ( length < 0 ) {
		length = -length;
		pad_char = '0';
	}

	pad = length - (l_buf + sizeof(l_buf) - 1 - cp);

	while ( pad-- > 0 ) *--cp = pad_char;
	
	return cp;
}

// end of tocharptr
