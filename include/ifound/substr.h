/***********************************************************************
* Copyright (c) 1991 InterSoft Co.  All Rights Reserved
* THIS IS UNPUBLISHED PROPRIETARY SOURCE CODE OF InterSoft Co.
* The copyright notice above does not evidence any
* actual or intended publication of such source code.
*
* $Id: substr.h,v 1.2 2006/02/15 18:05:38 hardaiz Exp $
*
* DESCRIPTION
* Class definitions for the SubString data type
*********************************************************************/

#ifndef	SUBSTR_H
#define	SUBSTR_H

#include <ifound.h>

#ifdef __GNUC__
#pragma interface
#endif

struct srep;
class String;

// SubString class
                    
// Oscar is documenting this class (/usr2/acct/oscar/doc/clase1.doc)                    
class SubString {

	friend class String;

// private :

	srep *p;			// String representation this is a SubString of
	UnShort pos;		// subString Initial pos
	UnShort len;		// subString length 

	SubString(const srep *s, Int p, Int l);
	
	SubString(const srep *s, const String &sep, Int n);
	SubString(const srep *s, const char *cs, Int n);
	void make(const char *s2, UnShort s2_len, Int n);
	void connect();
	void disconnect();

public :
	SubString(const SubString &s);
	SubString &operator=(const String &);
	~SubString() { disconnect(); }

};
#endif

