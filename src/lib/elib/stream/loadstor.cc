/********************************************************************
* Copyright (c) 2026 Master_Solutions S.R.L.  All Rights Reserved
* THIS IS UNPUBLISHED PROPRIETARY SOURCE CODE OF Master_Solutions S.R.L.
* The copyright notice above does not evidence any
* actual or intended publication of such source code.
*
* $Id: loadstor.cc,v 1.1.1.1 1996/04/16 18:52:43 gustavof Exp $
*
* DESCRIPTION:	Methods to load and store data types in a
*				CPU-independent representation.
*********************************************************************/
#ifdef __GNUC__
#pragma implementation
#endif

#include <local/istream.h>
// load methods

void load(InputStream &is, short &s)
{
	UnChar b0, b1;

	is >> b0 >> b1;

	s = short(b0 << 8) | short(b1);
}

//BZ 2291
void load(InputStream &is, long &l)
{
	UnChar b0, b1, b2, b3, b4, b5, b6, b7;

	is >> b0 >> b1 >> b2 >> b3 >> b4 >> b5 >> b6 >> b7;
	l = long(b0) << 56 | long(b1) << 48 | long(b2) << 40 | long(b3) << 32 |
	    long(b4) << 24 | long(b5) << 16 | long(b6) << 8 | long(b7);
}

// we assume that floating point numbers are in IEEE-754 format

void load(InputStream &is, double &d)
{
	UnChar *p = (UnChar *) &d;

	for (unsigned int i = 0; i < sizeof(double); i++)
		is >> *p++;
}

// store methods

void store(OutputStream &is, short s)
{
	is << UnChar(s >> 8) << UnChar(s & 0xff);
}

void store(OutputStream &is, long l)
{
	is << UnChar(l >> 56) << UnChar((l >> 48) & 0xff) << 
			UnChar(l >> 40) << UnChar((l >> 32) & 0xff) << 
			UnChar(l >> 24) << UnChar((l >> 16) & 0xff) << 
	 		UnChar((l >> 8) & 0xff) << UnChar(l & 0xff);
}

void store(OutputStream &is, double &d)
{
	UnChar *p = (UnChar *) &d;

	for (unsigned int i = 0; i < sizeof(double); i++)
		is << *p++;
}
