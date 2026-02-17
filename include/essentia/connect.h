/********************************************************************
* Copyright (c) 1992 InterSoft Co.  All Rights Reserved
* THIS IS UNPUBLISHED PROPRIETARY SOURCE CODE OF InterSoft Co.
* The copyright notice above does not evidence any
* actual or intended publication of such source code.
*
* $Id: connect.h,v 1.1.1.1 1996/04/16 18:51:47 gustavof Exp $
*
* DESCRIPTION
* Class definitions for general Connection type.
*********************************************************************/

#ifndef CONNECT_H
#define CONNECT_H

#include <ifound.h>

#ifdef __GNUC__
#pragma interface
#endif

#include <essentia/msghdr.h>

class String;

class Connection {

// private:

    Connection(const Connection&);
    void operator=(const Connection&);
    
protected:

	MsgBuff buffer; // Message buffer
	Int *snd_pdata;	// Pointer to next Int in snd buffer
	Int *rcv_pdata;	// Pointer to next Int in rcv buffer
	Int size;		// Size of buffer (in number of Int's)
					// (does not consider the header)
	Int snd_cnt;	// Number of Int's left to reach snd buffer size
	Int rcv_cnt;	// Number of Int's left to reach rcv buffer size
	Int clientId;	// client id
	Int counter_;	// Repetition counter of last parameter

	virtual void _flush() = 0;
	virtual Int _fill() = 0;
	virtual void flushBuff()	{ _flush(); }
	virtual void fillBuff()		{ (void)_fill(); }

	void pushWord(Int i)
	{
		if (--snd_cnt < 0)
			flushBuff();
		*snd_pdata++ = i;
	}

	void popWord(Int &i)
	{
		if (--rcv_cnt < 0)
			fillBuff();
		i = *rcv_pdata++;
	}

	void receive(Int len, char *cp);
	void send(register Int len, const char *cp);

public:

	Connection();
	virtual ~Connection();

	virtual operator Int();

	// push parameters routines
	virtual Connection &operator<<(Int);
	virtual Connection &operator<<(const String &);
	virtual Connection &operator<<(const char *);
	virtual void push(void *, Int);

	// pop parameters routines
	virtual Connection &operator>>(Int &i);
	virtual Connection &operator>>(String &s);
	virtual Connection &operator>>(char *&cp);
	virtual void pop(void *, Int);

	// counter routines
	void sendCounter(Int rep)
		{ 
			if (rep > 1)
				(*this) << Int(SPECIAL_VALUE) << rep;
		}
	void recFirstInt(Int &i)
		{ 	
			(*this) >> i;
			if (i != SPECIAL_VALUE)
	    		counter_ = ERR;
			else
				(*this) >> counter_ >> i;
		}

	Int &counter()
		{ 
			return counter_; 
		}

	// miscellaneous routines
	Int getCnt()				{ return snd_cnt * sizeof(Int); }
	Int retValue() const		{ return buffer->h.retValue; }
	void setRetValue(Int rv)	{ buffer->h.retValue = rv;  }
};

#endif // CONNECT_H
