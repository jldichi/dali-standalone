/********************************************************************
* Copyright (c) 1992 InterSoft Co.  All Rights Reserved
* THIS IS UNPUBLISHED PROPRIETARY SOURCE CODE OF InterSoft Co.
* The copyright notice above does not evidence any
* actual or intended publication of such source code.
*
* $Id: connect.cc,v 1.3 2007/03/07 18:41:34 hardaiz Exp $
*
* DESCRIPTION
*********************************************************************/

#ifdef __GNUC__
#pragma implementation
#endif

#include <string.h>
#ifdef _AIX
#include <strings.h>
#endif

#include <essentia/connect.h>
#include <ifound/str.h>
#include <ifound/stdc.h>

#define ALIGNMENT	(sizeof(void *))
#define ISALIGNED(buff)	((long(buff)%ALIGNMENT) == 0)

Connection::Connection()
:	buffer(NULL),
	snd_pdata(NULL),
	rcv_pdata(NULL),
	size(0),
	snd_cnt(0),
	rcv_cnt(0),
	clientId(0),
	counter_(ERR)	
{
}

Connection::~Connection() { }

void Connection::send(register Int l, const char *cp)
{
	Int *p = (Int *) cp;
	l = (l + sizeof(Int) - 1) / sizeof(Int);

	// Should be careful with alignment to sizeof(Int)
	// (sending memory not belonging to the String)
	if (ISALIGNED(cp)) {
		while (l--) 
			pushWord(*p++);
	} else {
		Int aux;
    	while (l--) {
			mCopy(&aux, p, sizeof(Int));
			pushWord(aux);
			p++;
		}
	}
}

Connection &Connection::operator<<(Int i)
{
	pushWord(i);
	return *this;
}

Connection &Connection::operator<<(const String &s)
{
	Int l = s.length();
	*this << l;

	if (l > 0 && l != LONG_MAX) 
		send(l, toCharPtr(s));

	return *this;
}

Connection &Connection::operator<<(const char *cp)
{
	Int l = (cp == HIGH_VALUE ? LONG_MAX : (cp ? strlen(cp) : 0));

	*this << l;

	if (l > 0 && l != LONG_MAX)
		send(l, cp);	

	return *this;
}

void Connection::push(void *buff, Int l)
{
	*this << l;

	if (l > 0)
		send(l, (const char*)buff);
}

void Connection::receive(Int len, char *cp)
{
	register Int *ip = (Int *) cp;
	register Int l = (len + sizeof(Int) - 1) / sizeof(Int);

	// Should be careful with alignment to sizeof(Int)
	// (reading memory not belonging to the String)
	while (l--)
		popWord(*ip++);

	// Make sure we end the string with a '\0'
	cp[len] = '\0';
}

Connection &Connection::operator>>(Int &i)
{
	popWord(i);
	return *this;
}

Connection &Connection::operator>>(String &s)
{
	Int len;
	*this >> len;

	char *p = s.getBuffer(len);

	if (p && p != HIGH_VALUE)
		receive(len, p);

	return *this;
}

Connection &Connection::operator>>(char *&cp)
{
	Int len;
	*this >> len;

	if (len == LONG_MAX) 
		cp = (char *)HIGH_VALUE;
	else if (len <= 0)
		cp = NULL;
	else {
		cp = tr_new char[len + sizeof(Int)];
		receive(len, cp);
	}

	return *this;
}

void Connection::pop(void *buff, Int len)
{
	typedef unsigned int uint;

	if (len > 0) {

		Int *p = (Int *) buff;
		Int l = (len+sizeof(Int)-1) / sizeof(Int);

		// Should be careful with alignment to sizeof(Int)
		// (reading memory not belonging to the buffer)
		if (ISALIGNED(buff)) {

			while (--l)
				popWord(*p++);

			if (len % sizeof(Int) == 0)
				popWord(*p);
			else {
				Int tmp;
				popWord(tmp);
				mCopy((void *) p, &tmp, len % sizeof(Int));
			}

		} else {
			Int tmp;
			while (--l) {
				popWord(tmp);
				((char *) p)[0] = ((char *) &tmp)[0];
				((char *) p)[1] = ((char *) &tmp)[1];
				((char *) p)[2] = ((char *) &tmp)[2];
				((char *) p)[3] = ((char *) &tmp)[3];
				// mCopy(p, &tmp, sizeof(Int));	
				p++;
			}
			popWord(tmp);
			if (len % sizeof(Int) == 0) 
				mCopy(p, &tmp, sizeof(Int));	
			else
				mCopy(p, &tmp, len % sizeof(Int));
		}

	}
}

Connection::operator Int()
{
	return 0L;
}
