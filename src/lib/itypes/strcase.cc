/********************************************************************
* Copyright (c) 1991 InterSoft Co.  All Rights Reserved
* THIS IS UNPUBLISHED PROPRIETARY SOURCE CODE OF InterSoft Co.
* The copyright notice above does not evidence any
* actual or intended publication of such source code.
*
* $Id: strcase.cc,v 1.2 1998/09/04 18:39:29 mauro Exp $
*
* DESCRIPTION:
*********************************************************************/

#include <ifound/str.h>
#include <ifound/chset.h>
#include <ifound/priv/itypesp.h>

String String::toUpper() const
{
	String str(*this);

	if (str != NULL_STRING && str != HIGH_VALUE ) {
		str.realloc(str.p->len());

		register char *ps = str.p->s();
		
		for (register int i = 0; i < str.p->len(); i++) {
			*ps = toUpperCase(*ps);
			ps++;
		}
	}
	return str;
}

String String::toLower() const
{
	String str(*this);

	if (str != NULL_STRING && str != HIGH_VALUE ) {
		str.realloc(str.p->len());

		register char *ps = str.p->s();

		for (register int i = 0; i < str.p->len(); i++) {
			*ps = toLowerCase(*ps);
			ps++;
		}
	}
	return str;
}

String String::toAscii() const
{
	String str(*this);

	if (str != NULL_STRING && str != HIGH_VALUE ) {
		str.realloc(str.p->len());

		register char *ps = str.p->s();

		for (register int i = 0; i < str.p->len(); i++) {
			*ps = toascii(*ps);
			ps++;
		}
	}
	return str;
}

String String::fromAscii() const
{
	String str(*this);

	if (str != NULL_STRING && str != HIGH_VALUE ) {
		str.realloc(str.p->len());

		register char *ps = str.p->s();

		for (register int i = 0; i < str.p->len(); i++) {
			*ps = fromascii(*ps);
			ps++;
		}
	}
	return str;
}

// end of strcase
