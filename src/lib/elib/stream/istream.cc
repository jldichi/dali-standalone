/********************************************************************
* Copyright (c) 2026 Master_Solutions S.R.L.  All Rights Reserved
* THIS IS UNPUBLISHED PROPRIETARY SOURCE CODE OF Master_Solutions S.R.L.
* The copyright notice above does not evidence any
* actual or intended publication of such source code.
*
* $Id: istream.cc,v 1.3 2004/09/29 15:28:46 damianm Exp $
*
* DESCRIPTION: IdeaFix Streams
*********************************************************************/
#ifdef __GNUC__
#pragma implementation
#endif

#include <string.h>

#include <ifound.h>
#include <ifound/chset.h>
#include <ifound/itypes.h>
#include <local/istream.h>
#include <ifound/stdc.h>

// Stream methods

Stream::Stream(short n)
{
	size = n;
	if (n)
		ptr = base = tr_new char[n];
	reset();
}

Stream::~Stream()
{
	if (size) tr_delete_vec(base);
}

void Stream::reset()
{
	cnt   	= 0;
	state 	= 0;
	offset	= 0;
}

void Stream::clearError()
{
	state &= ~ (IO_ERROR | END_OF_INPUT); 
}

// Input stream methods

#if 0
InputStream::~InputStream()
{
}
#endif

char InputStream::fill() { return 0; }

InputStream & InputStream::unget(char chr)
{
	if (ptr > base)
		*--ptr = chr;
	else
		state |= IO_ERROR; 
	return *this;
}

InputStream & InputStream::get(String &s, char delim)
{
	FmtBuff aux_buff;
	char *pbuff = aux_buff;

	s = NULL_VALUE;
	while ((*this >> *pbuff).ok() && *pbuff != delim) {
		if (*pbuff != '\r') {
			if (delim == '\0' && !isalnum(*pbuff)) break;
			pbuff++;

			// check aux_buff overflow

			if (pbuff == &aux_buff[sizeof(aux_buff)-1]) {
				*pbuff = 0;
				s	  += aux_buff;		// concat the string
				pbuff  = aux_buff;
			}
		}
	}
	*pbuff = 0;
	s += aux_buff;		// concat the string
	return *this;
}

// OutputStream methods

// toDo:
#if 0
OutputStream::~OutputStream()
{
}
#endif

void OutputStream::flush() {}

OutputStream & OutputStream::operator << (const char *s)
{
	if (s) {
		if (s == HIGH_VALUE) s = HIGH_VALUE_IMAGE;
		while (*s)
			*this << *s++;
	}
	return *this;
}

OutputStream & OutputStream::operator << (const void *s)
{
	return *this << toCharPtr(Int(s), 10, 16);
}

OutputStream & OutputStream::operator << (Int i)
{
	return *this << toCharPtr(i);
}

OutputStream & OutputStream::operator << (const String &s)
{
	return *this << toCharPtr(s);
}

OutputStream & OutputStream::operator << (Date d)
{
	return *this << toCharPtr(d);
}

OutputStream & OutputStream::operator << (Time t)
{
	return *this << toCharPtr(t);
}

// execute function passed as a paramater

OutputStream & OutputStream::operator << (OutputStreamFunc f)
{
	return f(*this);
}

void OutputStream::endMsg()
{ 
	flush(); 
}

void OutputStream::closeMsg()
{ 
	flush();
}

Int  OutputStream::pause()
{
	flush();
	return 0;
}

Int OutputStream::pause(const String &, const String &)
{
	flush();
	return 0;
}

// InputString methods

InputString::~InputString() {}

InputString::InputString(const char *cs)
: InputStream(0)
{
	ptr = base = (char *) cs;
	cnt = strlen(cs);
	strv = NULL;
}

InputString::InputString(String &s)
: InputStream(0)
{
	if (s == HIGH_VALUE)
		str = NULL_STRING;
	else
		str = s;
	ptr = base = (char *) toCharPtr(str);
	cnt = strlen(ptr);
	strv = NULL;
}

InputString::InputString(char *strvec[])
: InputStream(0)
{
	strv = strvec;
	n    = 1;
	ptr = base = strv[0] ? strv[0] : NULL;
	cnt = strlen(ptr);
}

char InputString::fill()
{
	if (strv && n) {
		if (strv[n] == NULL) n = 0;
		else { 
			ptr = base = strv[n++];
			cnt = ptr ? strlen(ptr) : 0;
		}
		return '\n';
	}
	state |= END_OF_INPUT;
	return 0;
}

// end of istream
