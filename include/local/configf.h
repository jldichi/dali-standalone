/********************************************************************
*
* Copyright (c) 1995 InterSoft Co.  All Rights Reserved
* THIS IS UNPUBLISHED PROPRIETARY SOURCE CODE OF InterSoft Co.
* The copyright notice above does not evidence any
* actual or intended publication of such source code.
*
* $Id: configf.h,v 1.1.1.1 1996/04/16 18:51:50 gustavof Exp $
*
* DESCRIPTION:
*
*********************************************************************/
#ifndef CONFIGF_H
#define CONFIGF_H
#ifdef __GNUC__
#pragma interface
#endif

#include <ifound/str.h>
#include <ifound/anyvalue.h>

struct Token {
	String   name;
	AnyValue value;
	explicit Token(const String &n) : name(n) {}
};

declare(PtrArray, Token);
declare(PtrArrayCursor, Token);

class Configuration {

private:

	String fileName;
	gtype(PtrArray, Token) *tokens;

	static int cmptok(Token **s1, Token **s2);
	void setTokens(va_list);
	void setValue(const String &, const String &);	

public:	

	// Variable args should be "const char*" and last one should be null !!!
	explicit Configuration(const char *, ...);
	~Configuration();

	void set();
	const AnyValue &valueOf(const String &) const;
};

#endif //CONFIGF_H

