/***********************************************************************
* Copyright (c) 1995 InterSoft Co.  All Rights Reserved
* THIS IS UNPUBLISHED PROPRIETARY SOURCE CODE OF InterSoft Co.
* The copyright notice above does not evidence any
* actual or intended publication of such source code.
*
* $Id: message.h,v 1.1.1.1 1996/04/16 18:51:34 gustavof Exp $
*
* DESCRIPTION
*
*********************************************************************/

#ifndef MESSAGE_H
#define MESSAGE_H

#include <ifound/anyvalue.h>
#include <ifound/str.h>

class Message : public String {

//private:

public:

	explicit Message(const String &s);
	Message (const char *s): String(s) {};
	Message();
		
	Message operator()(const AnyValue &av0, 
					   const AnyValue &av1 = NULL_ANYVALUE) const;
	Message operator()(const AnyValue &av0, const AnyValue &av1,
					   const AnyValue &av2, 
					   const AnyValue &av3 = NULL_ANYVALUE,
					   const AnyValue &av4 = NULL_ANYVALUE) const; 
	Message operator()(const AnyValue &av0, const AnyValue &av1,
					   const AnyValue &av2, const AnyValue &av3,
					   const AnyValue &av4, const AnyValue &av5,
					   const AnyValue &av6 = NULL_ANYVALUE,
					   const AnyValue &av7 = NULL_ANYVALUE,
					   const AnyValue &av8 = NULL_ANYVALUE,
					   const AnyValue &av9 = NULL_ANYVALUE) const;
							 
	void display() const;
	void display(const Message &msg) const;
	void displayError() const;
	void fatalError() const;

	friend Message operator+(const Message &m1, const Message &m2);
};

#endif // MESSAGE_H
