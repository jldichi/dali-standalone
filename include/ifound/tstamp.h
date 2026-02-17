/********************************************************************
* Copyright (c) 1991 InterSoft Co.  All Rights Reserved
* THIS IS UNPUBLISHED PROPRIETARY SOURCE CODE OF InterSoft Co.
* The copyright notice above does not evidence any
* actual or intended publication of such source code.
*
* $Id: tstamp.h,v 1.2 1997/01/02 18:49:41 gustavof Exp $
*
* DESCRIPTION
* Class definitions for time stamp type.
*********************************************************************/

#ifndef TSTAMP_H
#define TSTAMP_H

#include <ifound.h>

#ifdef __GNUC__
#pragma interface
#endif

#include <ifound/itypes.h>

class Connection;

class TimeStamp {

// private:

	Time time;
	UnShort nsec;
	UnShort uid;
	
	Int cmp(const TimeStamp &) const;
	
public :

	// constructores y destructores
	TimeStamp();
	TimeStamp(const Time &tm)		{ time = tm; nsec = uid = 0; }

	TimeStamp(const TimeStamp &ts)	{ *this = ts; }
	
	void setUserId(Int i)			{ uid = UnShort(i); }
	Int userId() const				{ return Int(uid); }
	void setNSec(Int i)				{ nsec = UnShort(i); }
	Int nSec() const				{ return nsec; }

	void setTime(const TimeStamp &ts)
	{ time = ts.time; nsec = ts.nsec; }
		
	bool setNow();
	bool setTStamp(bool timeCorruptHandler, const TimeStamp *correctTime = NULL);
	
	TimeStamp &operator=(const TimeStamp &ts);		
	TimeStamp &operator++();
	TimeStamp &operator--();
		
	bool operator>=(const TimeStamp &ts) const
		{ return cmp(ts) >= 0; }
	bool operator<=(const TimeStamp &ts) const
		{ return cmp(ts) <= 0; }
	bool operator>(const TimeStamp &ts) const
		{ return cmp(ts) > 0; }
	bool operator<(const TimeStamp &ts) const
		{ return cmp(ts) < 0; }
	bool operator==(const TimeStamp &ts) const
		{ return cmp(ts) == 0; }
		
	friend Connection &operator<<(Connection &c, const TimeStamp &ts);
	friend Connection &operator>>(Connection &c, TimeStamp &ts);

	void store(char *&buff) const;
	void load(const char *&buff);

	static Int storeLength()
	{ return 2 * sizeof(UnShort) + Time::storeLength(); }

	operator Time() const					{ return time; }

	friend Time toTime(const TimeStamp &ts)	{ return ts.time; 		  }
	friend Date toDate(const TimeStamp &ts)	{ return toDate(ts.time); }

	String descr();
	bool isNull() const;
	TimeStamp &operator-=(Int n);
	TimeStamp &operator+=(Int n);
};

inline bool TimeStamp::isNull() const { return time.isNull(); }
inline TimeStamp &TimeStamp::operator-=(Int n) { time -= n; return *this; }
inline TimeStamp &TimeStamp::operator+=(Int n) { time += n; return *this; }

#endif
