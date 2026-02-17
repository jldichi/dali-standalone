/********************************************************************
* Copyright (c) 1991 InterSoft Co.  All Rights Reserved
* THIS IS UNPUBLISHED PROPRIETARY SOURCE CODE OF InterSoft Co.
* The copyright notice above does not evidence any
* actual or intended publication of such source code.
*
* $Id: numfmt.cc,v 1.4 1997/01/06 20:48:17 valeria Exp $
*
* DESCRIPTION
* Implementation of Num class string formatting method
*********************************************************************/

#include <ifound.h>
#include <ifound/itypes.h>
#include <ifound/stdc.h>

String Num::format(const String &mask) const
{
	return toString(*this).format(mask);
}

// This method was adapted by pablog to be used on the report library 
// (I guess it's the only part of the Ideafix that uses this routine).
// Previous version of this function can be found by using the version log
// command (dali).
String Num::format(Int ndig, Int ndec, bool separ, Int sign, 
				   char filler) const
{
	assert(sign == SIGN_IGNORE || sign == SIGN_LEADING);
	short nint	= short(ndig-ndec);
	short sep	= separ ? (nint - 1) / 3 : 0;
	short totlen= short(ndig + (ndec > 0) + sep);

	// Check limit conditions
	if (exp == -MAX_EXP)
		return String(totlen, ' ');
	if (exp == MAX_EXP)
		return String(totlen, '*');

	bool needSign = (sign == SIGN_LEADING && exp < 0);

	// Get the total digits, ints, and decimals
	Int totint, totdec;
	totDigits(totint, totdec);

	// Verify if the number fits in the given ``ndig'' size.
	if (nint < totint)
		return String(totlen + sign==SIGN_LEADING, '*');
		
	static FmtBuff l_buf = { '\0' };
	register char *p = l_buf;

	// Calculate leading blanks
	// a blank for each non-printed digit
	short totblank = short(nint - (totint ? totint : 1));
	// one more blank if the number is positive and I have to print the sign
	totblank += (sign == SIGN_LEADING && !needSign);

	if (sign != SIGN_LEADING && exp <0) {
		// try to print the sign if the number is negative and the
		// sign is not drawn in the report image
		if (totblank == 0)
			// there's not enough space
			return String(totlen, '*');
		else  {
			needSign = true;
			totblank--;
			// one blank less because of the sign
		}
	}

	if (separ)
		totblank +=	 sep - (totint ? short(totint-1)/3 : 0);
	while (totblank--) 
		*p++ = filler;

	// Put if necessary the leading sign.
	if (needSign)
		*p++ = '-';

	// Calculate where does the first comma go.
	sep = separ ? short(totint-1) % 3 + 1: -1;

	// Start building the number (integers).
	if ( !totint )
		*p++ = '0';
	else {
		while ( totint ) {
			if ( !sep-- ) {
				*p++ = thousandSep();
				sep = 2;
			}
			*p++ = char(nthDig(totint--) + '0');
		} 
	}

	if (ndec) {
		*p++ = decPoint();
		for (register short i = 0; i < ndec; i++)
			if (totdec > i) 
				*p++ = char(nthDig(-i) + '0');
			else
				*p++ = '0';
	}
	*p = '\0';
	return String(l_buf);
}
