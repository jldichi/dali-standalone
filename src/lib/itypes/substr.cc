// --------------------------------------------------------------------
// Copyright (c) 1995 InterSoft Co.  All Rights Reserved
// THIS IS UNPUBLISHED PROPRIETARY SOURCE CODE OF InterSoft Co.
// The copyright notice above does not evidence any actual or intended 
// publication of such source code.
//
// $Id: substr.cc,v 1.2 2004/09/29 16:14:17 damianm Exp $
//
// Description:
// Implementation of SubString class
// --------------------------------------------------------------------

#ifdef __GNUC__
#	pragma implementation
#endif

#include <string.h>

#include <ifound/substr.h>
#include <ifound/itypes.h>
#include <ifound/stdc.h>

#include <ifound/priv/itypesp.h>

// create String from a Substring

String::String(const SubString& ss)
{
	struct srep *aux = ss.p;

	if (!aux->notEmpty())
		p = aux;
	else if (ss.pos >= aux->len() || ss.len <= 0)
		p = NULL;
	else {
		p = srep::build((ss.pos+long(ss.len) > aux->len()) ? 
										aux->len()-ss.pos : ss.len);
		strncpy(p->s(), aux->s() + ss.pos, p->len());
		p->s()[p->len()] = 0;
	}
}

// From another SubString
SubString::SubString(const SubString &s)
{
//	p = NULL;
//	pos = len = 0;
//	*this = String(s);
	p = s.p;
	connect();
	pos = s.pos;
	len = s.len;
}

SubString::SubString(const srep *s, Int posit, Int l)
{
	p = (srep *) s;
	connect();
	pos = UnShort(posit);
	len = UnShort(l);
}

// SubString constructor using a string as separator
SubString::SubString(const srep *s, const String &sep, Int n)
{
	// Test limit conditions
	if (!s->notEmpty() || !sep.p->notEmpty()) {
		pos = len = 0;
		p = NULL;
		return;
	}
	p = (srep *) s;
	connect();
	make(sep.p->s(), sep.p->len(), n);
}

// SubString constructor using a const char * as separator

SubString::SubString(const srep *s, const char *sep, Int n)
{
	// Test limit conditions
	if (!s->notEmpty() || !VALID_PTR(sep)) {
		pos = len = 0;
		p = NULL;
		return;
	}
	p = (srep *) s;
	connect();
	make(sep, strlen(sep), n);
}

void SubString::make(const char *s2, UnShort s2_len, Int n)
{
	const char *s1 = p->s();

	//  if n is negative asume its the nth starting from the end

	if (n < 0) {
		short nfields = 1;
		while ((s1 = strStr(s1,s2)) != NULL) {
			s1 += s2_len;
			++nfields;
		}
		s1 = p->s();
		n += nfields;

		// if less than n return a null at the begining

		if (n < 0) {
			pos = len = 0;
			return;
		}
	}

	// search the nth ocurrence

	while (n--) {
		s1 = strStr(s1, s2);
		if (s1 == NULL) break;
		s1 += s2_len;
	}

	// if less than n return a null at the end

	if (s1 == NULL || !strncmp(s1, s2, s2_len)) {
		pos = p->len();
		len = 0;
	}
	else {
		pos = s1 - p->s();
		s2 = strStr(s1, s2);
		len = s2 ? s2 - s1 : p->len() - pos;
	}
}

void SubString::connect()
{
	if (p->notEmpty())
		p->n()++;
}

void SubString::disconnect()
{
	if (p->notEmpty() && --p->n() == 0)
		p->destroy();
}

// Assignment to a SubString (to change its associated String)
SubString &SubString::operator=(const String &s)
{
	assert(0);

	struct srep *new_srep;

	// Check High Values

	if (p == (struct srep *) HIGH_VALUE)
		return *this;
	if (s.p == (struct srep *) HIGH_VALUE) {
		disconnect();
		p = (struct srep *) HIGH_VALUE;
		return *this;
	}

	// Get length and pointer of rigth value

	char *str2;
	long len2;

	if (s.p == NULL) {
		len2 = 0;
		str2 = "";
	} else {
		len2 = s.p->len();
		str2 = s.p->s();
	}

	// Check if the substring isn't out of the corresponding string

	if (p && pos + long(len) <= p->len()) {
		new_srep = srep::build(UnShort(p->len() + (len2 - len)));
		strncpy(new_srep->s(),			  p->s(),	pos);
		strcpy (new_srep->s() + pos,		  str2);
		strcpy (new_srep->s() + pos + len2, p->s() + pos + len);
	} else {
		UnShort fill_from = 0;
		new_srep = srep::build(UnShort(pos + len2));

		if (p) {
			strncpy(new_srep->s(), p->s(), pos);
			fill_from = p->len();
		}
		while (fill_from < pos)
			new_srep->s()[fill_from++] = ' ';
		strcpy(new_srep->s() + pos, str2);
	}

	// Connect new String

	disconnect();
	p = new_srep;

	return *this;
}

SubString String::operator()(Int pos, Int len)
{
	 return SubString(p, pos, len); 
}

SubString String::operator()(Int pos)
{
	return SubString(p, pos, UN_SHORT_MAX); 
}

const SubString String::operator()(Int pos, Int len) const
{
	return SubString(p, pos, len); 
}

const SubString String::operator()(Int pos) const
{
	return SubString(p, pos, UN_SHORT_MAX); 
}

const SubString String::field(Int n, const char *cs) const
{
	return SubString(p, cs, n); 
}

const SubString String::field(Int n, const String &sep) const
{
	return SubString(p, sep, n); 
}

SubString String::field(Int n, const char *cs)
{
	return SubString(p, cs, n); 
}

SubString String::field(Int n, const String &sep)
{ 
	return SubString(p, sep, n); 
}

