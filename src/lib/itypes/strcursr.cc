/********************************************************************
* Copyright (c) 1991 InterSoft Co.  All Rights Reserved
* THIS IS UNPUBLISHED PROPRIETARY SOURCE CODE OF InterSoft Co.
* The copyright notice above does not evidence any
* actual or intended publication of such source code.
*
* $Id: strcursr.cc,v 1.2 1998/05/19 21:15:52 eduardoc Exp $
*
* DESCRIPTION
* A String iterator
*********************************************************************/

#include <ifound/itypes.h>

#include <ifound/priv/itypesp.h>


StrCursor::StrCursor(const String &s, const String &t)
: str(s),
  tok(t)
{
	pos = (VALID_PTR(tok.p)) ? 0 : -1;
	len = -1;
}

StrCursor::StrCursor(const String &s)
: str(s),
  tok("\t")
{
	pos = (VALID_PTR(tok.p)) ? 0 : -1;
	len = -1;
}


void StrCursor::reStart()
{
	pos = (VALID_PTR(tok.p)) ? 0 : -1;
	len = -1;
}

StrCursor::operator void* ()
{
	return ok() ? (void*)this : NULL;
}               

bool StrCursor::ok()
{
	return pos == -1 ? false : true;
}


StrCursor::operator String ()
{
	return value();
}

String StrCursor::value()
{
	if (pos == -1) return NULL_STRING;
	if (!VALID_PTR(str.p)) return str;

	const char *s_end = strStr(str.p->s() + pos, tok.p->s());
	len = s_end ? s_end - str.p->s() - pos : str.p->len() - pos;

	if (len) return str(pos, len);
	return NULL_STRING;
}

StrCursor& StrCursor::operator ++ ()
{
	if (!VALID_PTR(str.p)) pos = -1;
	if (pos == -1) return *this;

	if (len == -1) {
		const char *s_end = strStr(str.p->s() + pos, tok.p->s());
		len = s_end ? s_end - str.p->s() - pos : str.p->len() - pos;
	}

	pos += len + tok.p->len();

	if (pos > str.p->len()) pos = -1;

	//	len must always be recomputed because, it is possible
	//	to call more this operator more than once consecutively
	//	without calling the String() operator in between, se the len
	//	variable would be in an inconsistent value.
	len = -1;

	return *this;
}

StrCursor& StrCursor::operator ++ (int)
{
	if (!VALID_PTR(str.p)) pos = -1;
	if (pos == -1) return *this;

	if (len == -1) {
		const char *s_end = strStr(str.p->s() + pos, tok.p->s());
		len = s_end ? s_end - str.p->s() - pos : str.p->len() - pos;
	}

	pos += len + tok.p->len();

	if (pos > str.p->len()) pos = -1;

	//	len must always be recomputed because, it is possible
	//	to call more this operator more than once consecutively
	//	without calling the String() operator in between, se the len
	//	variable would be in an inconsistent value.
	len = -1;

	return *this;
}

StrCursor& StrCursor::operator -- ()
{
	if (!VALID_PTR(str.p) || pos < tok.p->len()) pos = -1;
	if (pos == -1) return *this;

	const char *p = 
		strStrBackw(str.p->s(), tok.p->s(), 
								str.p->s() + pos - tok.p->len());

	pos = p ? p - str.p->s() + tok.p->len() : 0;

	return *this;
}

StrCursor& StrCursor::operator -- (int)
{
	if (!VALID_PTR(str.p) || pos < tok.p->len()) pos = -1;
	if (pos == -1) return *this;

	const char *p = 
		strStrBackw(str.p->s(), tok.p->s(), 
								str.p->s() + pos - tok.p->len());

	pos = p ? p - str.p->s() + tok.p->len() : 0;

	return *this;
}

// end of strcursr
