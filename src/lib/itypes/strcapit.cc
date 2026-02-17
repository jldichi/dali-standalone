/********************************************************************
* Copyright (c) 1991 InterSoft Co.  All Rights Reserved
* THIS IS UNPUBLISHED PROPRIETARY SOURCE CODE OF InterSoft Co.
* The copyright notice above does not evidence any
* actual or intended publication of such source code.
*
*
* $Id: strcapit.cc,v 1.2 1998/04/24 16:50:23 guiller Exp $
*
*
* DESCRIPTION:
*				
*
*********************************************************************/

#ifdef __VC__
#	include <math.h>
#endif

#include <ifound/chset.h>
#include <ifound/str.h>

String String::capitalized() const
{
	if (*this == NULL_STRING)
		return NULL_STRING;

	if (*this == HIGH_VALUE)
		return HIGH_VALUE;

	String tmp(1, toUpperCase((*this)[0]));
	if (length() > 1)
		tmp += String((*this)(1)).toLower();
	return tmp;
}

// end of strcapit
