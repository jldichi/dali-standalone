/********************************************************************
* Copyright (c) 1991 InterSoft Co.  All Rights Reserved
* THIS IS UNPUBLISHED PROPRIETARY SOURCE CODE OF InterSoft Co.
* The copyright notice above does not evidence any
* actual or intended publication of such source code.
*
* $Id: strhis.h,v 1.1.1.1 1996/04/16 18:51:44 gustavof Exp $
*
* DESCRIPTION: Class definition for a String History facility
*********************************************************************/

#ifndef STRHIS_H
#define STRHIS_H

#ifdef __GNUC__
#pragma interface
#endif

#include <ifound.h>

class OutputStream;
class InputStream;
class String;

class StrHistory {

	useIdCheck(StrHistory_id);
	
protected:

	String *history;
	Int hisSize;
	Int fullslots_;
public:

	StrHistory(Int hs = 5);
	virtual ~StrHistory();
	virtual void execute(String &datum);
	void operator<<(const String &datum);
	void store(OutputStream &) const;
	void store(char *&p) const;
	void load(InputStream &);
	void load(const char *&p);
	Int fullslots() { return fullslots_; }
};	

class ExtStrHistory : public StrHistory {
	Int inforows_;
	Int currpos_;
public:                                 
//10 5
	ExtStrHistory(Int n1 = 30, Int n2 = 10);
	virtual ~ExtStrHistory() { idCheck(); }
	void execute(String &datum);
	void operator<<(const String &datum);
	const String &operator -- (void);
	const String &operator ++ (void);
	Int	currPos();
};

inline ExtStrHistory::ExtStrHistory(Int n1, Int n2)
: StrHistory(n1) 
{ 
	idCheck(); 
	inforows_ = n2; 
	currpos_ = ERR;
} 

inline Int ExtStrHistory::currPos()
{ 
	idCheck(); 
	return currpos_ + 1; 
}

#endif
