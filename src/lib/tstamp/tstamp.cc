/********************************************************************
* Copyright (c) 1992 InterSoft Co.  All Rights Reserved
* THIS IS UNPUBLISHED PROPRIETARY SOURCE CODE OF InterSoft Co.
* The copyright notice above does not evidence any
* actual or intended publication of such source code.
*
* $Id: tstamp.cc,v 1.4 1998/05/19 21:22:09 eduardoc Exp $
*
* DESCRIPTION
*********************************************************************/

#ifdef __GNUC__
#pragma implementation
#endif

#include <ifound.h>
#include <ifound/priv/gfuncs.h>
#include <ifound/tstamp.h>
#include <stdio.h>

// it's no more inline because there are problems with static vars in inlines

TimeStamp &lastStamp()
{
	static TimeStamp *lStamp = NULL;
	
	if (lStamp == NULL)
		lStamp = tr_new TimeStamp;
	return *lStamp;
}

Int TimeStamp::cmp(const TimeStamp &ts) const
{
	Int res;
	if ((res = ::cmp(time, ts.time)))
		return res;
	return nsec - ts.nsec;
}
                      
TimeStamp &TimeStamp::operator=(const TimeStamp &ts)
{  
  if (this != &ts) {
       time = ts.time;
       nsec = ts.nsec;
       uid  = ts.uid;
  }
  return *this;

}                      

TimeStamp::TimeStamp()
:	time(),
	nsec(0),
	uid(0)
{
}

TimeStamp &TimeStamp::operator++()
{
	if (nsec == UN_SHORT_MAX) {
		lastStamp().time = ++time;
		lastStamp().nsec = nsec = 0;
	} else {
		lastStamp().nsec = ++nsec;
		lastStamp().time = time;
	}
	return *this;
}

TimeStamp &TimeStamp::operator--()
{
	if (nsec > 0) {
		--nsec;
	} else {
		nsec = UN_SHORT_MAX;
		--time;		
	}
	return *this;
}

bool TimeStamp::setNow()
{
	time = TODAY;
	if (lastStamp().time == time) {
		nsec = ++lastStamp().nsec;
		return false;
	}
	lastStamp().time = time;
	lastStamp().nsec = nsec = 0;
	return true;
}


// used in essentia to handle operating system time changes
#define CORRUPTTIMEALLOWED	5400

bool TimeStamp::setTStamp(bool timeCorruptHandler, const TimeStamp *correctTime)
{
	static bool troubles = false;
	if (timeCorruptHandler && isNull()) {
		assert(correctTime != NULL);
		time = correctTime->time;
		lastStamp() = *correctTime;
	}

	Time now = TODAY;
	
	if (lastStamp().time == now) {
		nsec = ++lastStamp().nsec;
		return false;
	}

	if (!timeCorruptHandler &&
			(lastStamp().time - now).seconds() > CORRUPTTIMEALLOWED) {
		error("Time inconsistency detected !! Time (%S) should be around (%S)\n",
				&(toString(now)), &(toString(lastStamp().time)));
	} else if (timeCorruptHandler && lastStamp().time > now) {
		if (!troubles) {
			warning("Time inconsistency detected !!. Time (%S) should be around (%S)\n",
					&(toString(now)), &(toString(lastStamp().time)));
			troubles = true;
		} 
		
		if (nsec == UN_SHORT_MAX) {
			++time;
			nsec = 0;
		}
		++nsec;
		return false;
	}

	if (troubles) {
		warning("Time inconsistency solved (%S).\n",
			&(toString(now)));
    	troubles = false;
	}

	time = now;
	nsec = 0;
	return true;
}

void TimeStamp::store(char *&buff) const
{
	time.store(buff);
	stuint(nsec, buff);
	buff += sizeof(nsec);
	stuint(uid, buff);
	buff += sizeof(uid);
}

void TimeStamp::load(const char *&buff)
{
	time = Time::load(buff);
	nsec = lduint(buff);
	buff += sizeof(nsec);
	uid = lduint(buff);
	buff += sizeof(uid);
}

String TimeStamp::descr()
{
	String tmp = "(Time: ";
	tmp += time.format("DD/MM/YY HH:MI:SS");
	tmp += ", seqn: ";
	tmp += toString(Int(nsec));
	tmp += ", uid: ";
	tmp += toString(Int(uid));
	tmp += ")";
	return tmp;
}
