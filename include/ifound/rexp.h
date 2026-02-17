/********************************************************************
* Copyright (c) 1991 InterSoft Co.  All Rights Reserved
* THIS IS UNPUBLISHED PROPRIETARY SOURCE CODE OF InterSoft Co.
* The copyright notice above does not evidence any
* actual or intended publication of such source code.
*
* $Id: rexp.h,v 1.2 2000/01/31 18:14:55 diegon Exp $
*
* DESCRIPTION
* Interface to Regular Expression matching class
*********************************************************************/

#ifndef	REGEXP_H
#define	REGEXP_H

#include <ifound.h>

#ifdef __GNUC__
#	pragma interface
#endif

#include <ifound/str.h>

// compile buffer size
#define	C_BUFF_SIZE		1024

// max. no. of brackets
#define	NBRA			9

#define isMetaChar(c)	(c == '[' || c == ']' || c == '*' || c == '.' \
		|| c == '^')


class RegExp {

	useIdCheck(RegExp_id);

// private:

	char *braslist[NBRA];
	char *braelist[NBRA];
	int	nbra;
	char *loc1, *loc2;
	int	circf;
	int low;
	int size;
	char *c_re;				// compiled regular expression
	String re_pattern;		// the regular expression
	String search_s;		// the string to search within
	char *sp;				// pointer to current character
	int state;				// RE state, errors
	Int re_length;

	char *compile(char *ep, char *endbuf, int seof);
	int step(register char *p1, register char *p2);
	int backStep(register char *p1, Int len, register char *p2);
	int advance(register char *lp, register char *ep);
	void getrnge(register char *str);
	char *re_error(int err_code);
	void newrexp(const String &re);

	bool limited;			// limited regular expresion
	bool hasOffset;			// wether the search started at
							// the beginning of a line or not

public:

	explicit RegExp(const char *re, bool l = false);
	explicit RegExp(const String &re, bool l = false);
	RegExp(const RegExp &re, bool l = false);
	RegExp(bool l = false);
	~RegExp();

	const RegExp &operator= (const RegExp &re);
	const RegExp &operator= (const String &s);
	operator void*();
	inline bool operator!() const;
	
	// Returns wether a given string matches with the regexp
	bool match(const String &ss, Int offset = 0);

	// Returns where in the string occurs the regexp
	Int search(const String &ss, Int offset = 0);
	Int backSearch(const String &ss, Int offset = INT_MAX);

	inline const String &pattern() const;
	Int length() const;

	// Determine whether the regexp contains meta characters or not
	bool hasMetaChars() const;

	// Return first character (if regexp starts with one)
	WideChar firstChar() const;

	// Given the string with '\n', replace it with the buffers
	// generated during the last match.
	String expand(const String &str) const;
	inline void fromBeginning(bool v);
	inline bool fromBeginning() const;

private:

	static char compile_buf[C_BUFF_SIZE];

};

void
RegExp::fromBeginning(bool v)
{
	idCheck();
	circf = int(v);
}

bool
RegExp::fromBeginning() const
{
	idCheck();
	return bool(circf);
}

const String &
RegExp::pattern() const
{
	idCheck();
	return re_pattern;
}

bool
RegExp::operator!() const
{
	idCheck();
	return state;
}

class LRegExp : public RegExp {

public:

	inline explicit LRegExp(const String &re);
	inline LRegExp(const LRegExp &re);
	inline LRegExp();
	inline ~LRegExp();

};

LRegExp::LRegExp(const String &re)
:   RegExp((const String &)re, true)
{
	idCheck();
}

LRegExp::LRegExp(const LRegExp &re)
:	RegExp((const RegExp &)re, true)
{
	idCheck();
}

LRegExp::LRegExp()
:   RegExp(true)
{
	idCheck();
}

LRegExp::~LRegExp()
{
	idCheck();
}

String noMetaChars(const String &);

#endif
