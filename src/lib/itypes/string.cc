/********************************************************************
* Copyright (c) 1991 InterSoft Co.  All Rights Reserved
* THIS IS UNPUBLISHED PROPRIETARY SOURCE CODE OF InterSoft Co.
* The copyright notice above does not evidence any
* actual or intended publication of such source code.
*
* $Id: string.cc,v 1.5 2007/03/07 15:26:43 hardaiz Exp $
*
* DESCRIPTION: Implementation of String class
*
*********************************************************************/

#include <ifound/osdep.h>

#ifdef __GNUC__
#	pragma implementation "str.h"
#endif

#include <string.h>
#ifdef _AIX
#include <strings.h>
#endif

#include <ifound/str.h>
#include <ifound/itypes.h>
#include <ifound/stdc.h>

#include <ifound/priv/itypesp.h>
#include <ifound/priv/gfuncs.h>

#ifdef realloc
#	undef realloc
#endif

// The name 'yasm' stands for "yet another swap macro"
#define	yasm(a,b,tt)							\
	do {						 				\
		tt __tmp = a; a = b; b = __tmp;			\
 	} while (0)

// primitives

void String::connect()
{
	if (p->notEmpty())
		p->n()++;
}

void String::disconnect()
{
	if (p->notEmpty() && --p->n() == 0)
		p->destroy();
}

void String::realloc(UnShort len)
{

	if (p == (struct srep *)HIGH_VALUE) return;

	if (p==NULL && len!=0 || len >= BYTESALLOC(p->len()) || p->n() >1) {
		srep *newptr;
		if (p == NULL) {
			newptr = srep::build(len);
			newptr->len() = 0;
		}
		else {
			newptr = srep::build(max(len, p->len()));
			strcpy(newptr->s(), p->s());
			newptr->len() = p->len();
		}
		disconnect();
		p = newptr;
	}
}


//
// Construc/realloc
//

// From a literal string "xxxx"

String::String(const char *cs, Int len)
{
	if (len == 0 || cs == NULL || (VALID_PTR(cs) && *cs == '\0')) 
		p = NULL;
	else if (len == LONG_MAX || cs == HIGH_VALUE) 
		p = (struct srep *) HIGH_VALUE;
	else {
		int l = strlen(cs);
		if (len == ERR)
			len = l;

		p = srep::build(UnShort(len));
		if (len < l)
			strncpy(p->s(), cs, int(len));
		else {
			strncpy(p->s(), cs, int(l));
			mSet(&p->s()[l], ' ', len - l);
		}
		p->s()[len] = 0;
	}
}

// Constructor for handling binary data 
// The result String might not be null-terminated !!!

String::String(Int len, const char *cs)
{
	if (len == 0 || cs == NULL || !VALID_PTR(cs)) 
		p = NULL;
	else if (len == LONG_MAX || cs == HIGH_VALUE) 
		p = (struct srep *) HIGH_VALUE;
	else {
		p = srep::build(UnShort(len));
		mCopy(p->s(), cs, int(len));
	}
}


// From n-times a character 'x'

String::String(Int l, char c)
{
	if (l <= 0)
		p = NULL;
	else if (l>=UN_SHORT_MAX)
		p = (struct srep *)HIGH_VALUE;
	else {
		p = srep::build(UnShort(l));
		if (c) {
			register Int i = l;
			while (i--) p->s()[i] = c;
		}
		p->s()[l] = 0;
	}
}

// From another String

String::String(const String & x)
{
	if (x.p->notEmpty())
		x.p->n()++;
	p = x.p;
}

String::String(const String &x, Int len)
{
	Int olen = x.length();
	if (x.p->notEmpty() && olen == len) {
		x.p->n()++;
		p = x.p;
	} else {
		p = srep::build(UnShort(len));
		if (len < olen)
			strncpy(p->s(), x.p->s(), int(len));
		else {
			if (olen)
				strncpy(p->s(), x.p->s(), int(olen));
			mSet(&p->s()[olen], ' ', len - olen);
		}
		p->s()[len] = 0;
	}
}

/*
	This function should return 'const char *' and not 'char *' but since
	it is used mostly for C compatibility it is more convenient to avoid
	casting away the const.
*/
char *toCharPtr(const String &s)
{
	const char *ret = s.p->notEmpty() ? s.p->s() : (const char *) s.p;
	if (ret == NULL)
		return "";
	return (char *) ret;
}

// toDo: replace this with newToInt(...) (see below).

inline static Int digitValue(WideChar wc, Int base)
{
	if (isdigit(wc))
		return (wc - '0' >= base) ? ERR : (wc - '0');

	wc = toupper(wc);

	return (wc < 'A' || wc - 'A' + 10 >= base) ? ERR : (wc - 'A' + 10);
}

Int toInt(const String &s, Int base)
{
	if (!s.p->notEmpty())
		return ((const char *)s.p == LOW_VALUE) ? INT_NULL : INT_MAX;

	char *p = s.p->s();

	while (isspace(*p))
		++p;

	if (*p == '\0')
		return 0;

	bool neg = false;
	if (*p == '-') {
		neg = true;
		++p;
	}

	if (base == 0) {
		if (p[0] != '0')
			base = 10;
		else {
			++p;
			if (toupper(p[0]) != 'X')
				base = 8;
			else {
				base = 16;
				++p;
			}
		}
	}

	Int res = 0;
	Int value;

	while ((value = digitValue(*p, base)) != ERR) {
		res = res * base + value;
		++p;
	}

	return neg ? -res : res;
}

String toString(Int v, Int len, Int base)
{
	return String(toCharPtr(v, len, base), ERR);
}

static int addChar(int c, char *arg)
{
	String &s = *(String *) arg;
	s += char(c);
	return 0;
}

String
doFormat(String fmt, ...)
{
	va_list ap;
	va_start(ap, fmt);
	String s;
	// ugh !!! (pablog)
	doFormat(addChar, (char *) &s, toCharPtr(fmt), ap);
	va_end(ap);
	return s;
}

String
doFormat(const char *fmt, ...)
{
	va_list ap;
	va_start(ap, fmt);
	String s;
	// ugh !!! (pablog)
	doFormat(addChar, (char *) &s, fmt, ap);
	va_end(ap);
	return s;
}

String toString(String fmt, ...)
{
	va_list ap;
	va_start(ap, fmt);
	String s;
	// ugh !!! (pablog)
	doFormat(addChar, (char *) &s, toCharPtr(fmt), ap);
	va_end(ap);
	return s;
}

String toString(const char *fmt, ...)
{
	va_list ap;
	va_start(ap, fmt);
	String s;
	// ugh !!! (pablog)
	doFormat(addChar, (char *) &s, fmt, ap);
	va_end(ap);
	return s;
}

String toStr(const char *fmt, va_list ap)
{
	String s;
	doFormat(addChar, (char *) &s, fmt, ap);
	return s;
}

String toStr(const String &fmt, va_list ap)
{
	String s;
	doFormat(addChar, (char *) &s, toCharPtr(fmt), ap);
	return s;
}

Int String::length() const
{
	return p == NULL ? 0
		:  p == (struct srep *)HIGH_VALUE ? LONG_MAX
		:  p->len();
}

// comparison routines 

Int cmp(const String &a, const char *cstr)
{
	const char *cs = (VALID_PTR(cstr) && *cstr == '\0') ? NULL : cstr;

	if ((const char *) a.p == cs)
		return 0;

	if (a.p->notEmpty()) {
		if (VALID_PTR(cs)) return strcmp(a.p->s(), cs);
		return cs == HIGH_VALUE ? -1 : 1;
	}
	return a.p == (srep *) HIGH_VALUE ? 1 : -1;
}

Int cmp(const String &a, const String &b)
{
	if (a.p == b.p) return 0;

	if (a.p->notEmpty()) {
		if (b.p->notEmpty())
			return strcmp(a.p->s(), b.p->s());
		return b.p == (srep *) HIGH_VALUE ? -1 : 1;
	}
	return a.p == (srep *) HIGH_VALUE ? 1 : -1;
}


// Assigment Operators

// From a literal string "xxxx"

String & String::operator=(const char *cs)
{
	if (p->notEmpty() && --p->n() == 0)
		p->destroy();

	if (cs == NULL || (VALID_PTR(cs) && *cs == '\0')) p = NULL;
	else if (cs == HIGH_VALUE) p = (struct srep *)HIGH_VALUE;
	else {
		p = srep::build(strlen(cs));
		strcpy(p->s(), cs);
	}
	return *this;
}

// From a single character

String & String::operator=(const char c)
{
	if (p->notEmpty() && --p->n() == 0)
		p->destroy();

	p = srep::build(1);
	p->s()[0] = c;
	p->s()[1] = '\0';

	return *this;
}

// From another String

String & String::operator=(const String &x)
{
	if (this != &x) {
		if (x.p->notEmpty())
			x.p->n()++;
		if (p->notEmpty() && --p->n() == 0)
			p->destroy();
		p = x.p;
	}
	return *this;
}

// Concatenation operators

String String::operator+=(const String &s2)
{
	// Check if any of the strings is null or HIGH_VALUE

	if (p == NULL || s2.p == (struct srep *)HIGH_VALUE) {
		// Separated in 2 because of GNU compiler
		*this = s2;
		return *this;
	}
	if (s2.p == NULL || p == (struct srep *)HIGH_VALUE)
		return *this;

	// Create a tr_new srep and copy the two strings over it

	realloc(p->len() + s2.p->len());
	strcpy(p->s() + p->len(), s2.p->s());
	p->len() += s2.p->len();
	return *this;
}

String String::operator+=(const char c)
{
	// Check if any of the strings is null or HIGH_VALUE

	if (p == NULL) {
		// Separated in 2 becuase of GNU compiler
		*this = c;
		return *this;
	}

	if (p == (struct srep *)HIGH_VALUE)
		return *this;

	// Create a tr_new srep and append the character

	UnShort new_len = p->len() + 1;
	realloc(new_len);
	p->s()[p->len()] = c;
	p->s()[p->len()+1] = '\0';
	p->len() = new_len;
	return *this;
}

String String::operator+=(const char *s2)
{
	// Check if any of the strings is null or HIGH_VALUE

	if (p  == NULL || s2 == HIGH_VALUE) {
		// Separated in 2 becuase of GNU compiler
		*this = s2;
		return *this;
	}
	if (s2 == NULL || p == (struct srep *)HIGH_VALUE)
		return *this;

	// Create a tr_new srep and copy the two strings over it

	UnShort new_len = p->len() + strlen(s2);
	realloc(new_len);
	strcpy(p->s() + p->len(), s2);
	p->len() = new_len;
	return *this;
}

String operator+(const String &s1, const String &s2)
{
	String sTemp = s1;
	// Separated in 2 becuase of GNU compiler
	sTemp += s2;
	return sTemp;
}

String operator+(const String &s1, const char *cs)
{
	String sTemp = s1;
	// Separated in 2 becuase of GNU compiler
	sTemp += cs;
	return sTemp;
}

unsigned char String::operator[](Int n) const
{
	return p->notEmpty() && n >= 0 && n < p->len() ? p->s()[n] : 0;
}

Int String::count(char c, Int pos) const
{
	if (!p->notEmpty() || pos >= p->len())
		return ERR;

	Int n = 0;
	for (char *ptr = p->s() + pos; *ptr != '\0'; ptr++) {
		if (*ptr == c)
			n++;
	}
	return n;
}

Int String::index(char c, Int pos) const
{
	if (!p->notEmpty() || pos >= p->len())
		return ERR;

	char *res = strchr(p->s() + pos, c);		

	return res ? res - (p->s()) : ERR;
}

Int String::backIndex(char c, Int pos) const
{
	if (!p->notEmpty() || pos < 0)
		return ERR;

	if (pos > p->len() - 1)
		pos = p->len() - 1;

	char save = p->s()[pos+1];
	p->s()[pos+1] = '\0';

	char *res = strrchr(p->s(), c);		

	p->s()[pos+1] = save;

	return res ? res - (p->s()) : ERR;
}

char *String::getBuffer(Int len)
{
	bool no_buffer = !p->notEmpty() || --p->n() > 0;

	if (no_buffer ||
		len==LONG_MAX || len<=0 || 
						 BYTESALLOC(len)!=BYTESALLOC(p->len()))
	{
		if (!no_buffer)
			p->destroy();
		if (len == LONG_MAX) 
			return (char *) (p = (struct srep *)HIGH_VALUE);
		if (len == 0)
			return (char *) (p = NULL);
		p = srep::build(UnShort(len));
	}
	p->len() = UnShort(len);
	p->n()   = 1;
	return p->s();
}

const char *debugString(long s)
{
	String *st = (String *) s;
	const char *cptr = toCharPtr(*st);
	if (cptr == NULL)
		return "";
	if (cptr == HIGH_VALUE)
		return HIGH_VALUE_IMAGE;
	return cptr;
}

// toDo: put it inline
#ifdef HAVE_BOOL
String::operator bool() const
#else
String::operator const void *() const
#endif
{ 
	if (p > (srep*) HIGH_VALUE)
#ifdef HAVE_BOOL
		return true;
	return false;
#else
		return p;
	return 0;
#endif
}

bool String::operator!() const
{ 
	return p <= (srep *) HIGH_VALUE;
}


bool String::isNull() const
{ 
	return bool(p == NULL);
}

String& String::reverse()
{
	if (!p->notEmpty())
		return *this;

    int first = 0, last = p->len() - 1;		
	while (first++ < last--)
		yasm(p->s()[first], p->s()[last], char);
		
	return *this;		
}
