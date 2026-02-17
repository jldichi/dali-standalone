/********************************************************************
* Copyright (c) 1987 InterSoft Co.  All Rights Reserved
* THIS IS UNPUBLISHED PROPRIETARY SOURCE CODE OF InterSoft Co.
* The copyright notice above does not evidence any
* actual or intended publication of such source code.
*
* $Id: stdecode.c,v 1.1.1.1 1996/04/16 18:52:02 gustavof Exp $
*
* DESCRIPTION:
*	   Characters processing routine.
*
* ROUTINE       |  PURPOSE
*---------------+---------------------------------------------------
*StrDecode		
*********************************************************************/

#include <ideafix.h>

 /*
 * StrDecode:
 *
 * Get 's' string and process the characters in this way :
 *
 *			a) character
 *			b) ^character (control)
 *			c) \b \t \f \n \r \e (ESC) (upper or lower)
 *			d) \DDD (D octal digit)
 *			e) \xDDD (D hex digit)
 *			f) \dDDD (D decimal digit)
 *          g) \\ (backslash)
 *
 * 'dest' is the destination string.
 *
 * Feature (o bug):
 * ----------------
 * If the string has a null value (\000) it's converted to COD_ZERO
 * (defined in osdep.h) because 'dest' is an ascii-zero string 
 * (finishes with 0).
 */

void StrDecode(register UChar *dest, register UChar *s)
{
	register UChar c, i, base, dig;

	while (*s) {

		base = 8;

		switch (*s) {
		case '^':			/* Control characters */
			c = *++s;
			c = (UChar) (toupper((char)c) & 037);
			if (c == '\0')
				c = COD_ZERO;
			break;

		case '\\':			/* Control characters or numbers */
			switch (tolower((char) *++s)) {
			case 'n': c = '\n'; break;
			case 'f': c = '\f'; break;
			case 'r': c = '\r'; break;
			case 't': c = '\t'; break;
			case 'b': c = '\b'; break;
			case 'e': c = '\033'; break;

			case 'x':	/* hex numbers */
			case 'X':
				s++;
				for (c = i = 0; i < 2; s++, i++) {
					if (*s >= '0' && *s <= '9') 
						dig = *s - '0';
					else if (*s >= 'a' && *s <= 'f') 
						dig= *s - 'a' + 10;
					else if (*s >= 'A' && *s <= 'F') 
						dig = *s - 'A' + 10;
					else 
						break;
					c = c * 16 + dig;
				}
				s--;
				if (c == '\0')
					c = COD_ZERO;
				break;
			
			case '\\':
				c = *s;
				break;

			case 'd':	/* Decimal number */
			case 'D':
				s++;
				base = 10;

			case '0':
			case '1':
			case '2':
			case '3':
			case '4':
			case '5':
			case '6':
			case '7':	/* Octal number */
				for (c = i = 0; i < 3 && *s >='0' && *s <='9'; s++, i++)
					c = c * base + *s - '0';
				s--;
				if (c == '\0')
					c = COD_ZERO;
				break;

			default:	/* get the character as it is */
				c = *s;
				break;
			}
			break;

		default:
			c = *s;
		}
		s++;
		*dest++ = c;
	}
	*dest = '\0';
}
