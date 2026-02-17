/********************************************************************
* Copyright (c) 1991 InterSoft Co.  All Rights Reserved
* THIS IS UNPUBLISHED PROPRIETARY SOURCE CODE OF InterSoft Co.
* The copyright notice above does not evidence any
* actual or intended publication of such source code.
*
* $Id: istream.h,v 1.1.1.1 1996/04/16 18:51:49 gustavof Exp $
*
* DESCRIPTION
* Stream class interface
*********************************************************************/

#ifndef ISTREAM_H
#define ISTREAM_H

#ifdef __GNUC__
#pragma interface
#endif

#include <ifound.h>
#include <ifound/itypes.h>

enum {
	END_OF_INPUT	= 0020,
	IO_ERROR		= 0040,
	DEF_BUFSIZE		= 1024
};


class Stream {

// private:

	Stream(Stream &);		// don't let the user use these methods
	void operator=(Stream &);

protected:

	short	size;			// buffer size
	short	cnt;			// how many chars have been written/read
	char	*ptr;			// ptr to next char to write/read
	char	*base;			// ptr to start of buffer
	Int		offset;			// offset from start of file
	char	state;			// state of the stream

	void reset();			// reset all attributes
	
public:
	
	explicit Stream(short n);
	virtual ~Stream();

	void clearError();

	bool ok() const 
		{ return (state&(IO_ERROR | END_OF_INPUT)) ? false : true; }
	bool operator! () const 
		{ return !ok();}
#if HAVE_OP_CONST_VOID_PTR_BUG
	operator void*() const
#else
	operator const void*() const
#endif
		{ return ok() ? (void *) this : NULL; }
	void addState(char st) 
		{ state |= st; }
};

class InputStream : public Stream {

// private:

	InputStream(const InputStream &);
	
public:

	explicit InputStream(short n) : Stream(n) {}
	// toDo: ~InputStream();
	InputStream & operator >> (char & c)
		{ c = --cnt < 0 ? fill() : *ptr++; return *this; }
	InputStream & operator >> (UnChar & c)
		{ c = UnChar(--cnt < 0 ? fill() : *ptr++); return *this; }
	InputStream & operator >> (long &l)
		{ load(*this, l); return *this; }

	InputStream & unget(char);

	InputStream & get(String &s, char delim=0);
	InputStream & operator >> (String &s) { return get(s,'\n'); }
	InputStream & operator >> (Num&);

	friend void load(InputStream &is, char &c) { is >> c; }
	friend void load(InputStream &is, UnChar &c) { is >> c; }
	friend void load(InputStream &is, short &s);
	friend void load(InputStream &is, long &l);
	friend void load(InputStream &is, double &d);

	int eof() { return state&END_OF_INPUT; }
	int withError() { return state&IO_ERROR; }
	virtual char fill();

};

class OutputStream : public Stream {

typedef OutputStream &(*OutputStreamFunc)(OutputStream &);

// private:

	OutputStream(const OutputStream &);
	
public:

	explicit OutputStream(short n) : Stream(n) {}
	// toDo: virtual ~OutputStream();
	OutputStream & operator << (char c)
		{ if (--cnt<0) flush(); *ptr++= c; return *this; }
	OutputStream & operator << (UnChar c)
		{ if (--cnt<0) flush(); *ptr++= char(c); return *this; }
	OutputStream & operator << (Int i);
#ifdef HAVE_BOOL
	OutputStream & operator << (bool i)
	{
		return *this << (i ? "true" : "false");
	}
#endif
	OutputStream & operator << (int i) { return *this << Int (i); }
	OutputStream & operator << (const char *s);
	OutputStream & operator << (const void *s);
	OutputStream & operator << (const Num &num);
	OutputStream & operator << (const String &s);
	OutputStream & operator << (Date d);
	OutputStream & operator << (Time t);
	OutputStream & operator << (OutputStreamFunc f);

	friend void store(OutputStream &is, char c) { is << c; }
	friend void store(OutputStream &is, UnChar c) { is << c; }
	friend void store(OutputStream &is, short s);
	friend void store(OutputStream &is, long l);
	friend void store(OutputStream &is, double &d);

	virtual void flush();

	virtual void endMsg();
	virtual void closeMsg();
	virtual Int pause();
	virtual Int pause(const String &o1, const String &o2);

};

// function prototypes to pass to << operator

OutputStream &flush(OutputStream &s);
OutputStream &endl(OutputStream &s);
OutputStream &endMsg(OutputStream &s);

class InputString : public InputStream
{

// private:

	InputString(const InputString &);
	
	String str;
	char **strv;
	short n;

public:

	explicit InputString(String &s);
	explicit InputString(const char *cs);
	explicit InputString(char *strvec[]);
	virtual ~InputString();
	virtual char fill();

};

extern OutputStream *stdMsg_address;

#define	stdMsg	(*stdMsg_address)

#endif
