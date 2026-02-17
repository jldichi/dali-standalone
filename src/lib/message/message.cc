/********************************************************************
* Copyright (c) 1995 InterSoft Co.  All Rights Reserved
* THIS IS UNPUBLISHED PROPRIETARY SOURCE CODE OF InterSoft Co.
* The copyright notice above does not evidence any
* actual or intended publication of such source code.
*
* $Id: message.cc,v 1.1.1.1 1996/04/16 18:52:32 gustavof Exp $
*
* DESCRIPTION
*
*********************************************************************/

#include <ifound/message.h>
#include <ifound/gfuncs.h>

#define MAX_ARGS	10

Message::Message(const String &s)
:	String(s)
{
}

Message::Message()
:	String()
{
}

Message Message::operator()(const AnyValue &av0, const AnyValue &av1) const
{
	String args[MAX_ARGS];
	
	args[0] = toString(av0);
	args[1] = toString(av1);

	return Message(replaceArgs(args));
}

Message Message::operator()(const AnyValue &av0, const AnyValue &av1,
						    const AnyValue &av2, const AnyValue &av3,
						    const AnyValue &av4) const
{
	String args[MAX_ARGS];
	
	args[0] = toString(av0);
	args[1] = toString(av1);
	args[2]	= toString(av2);
	args[3] = toString(av3);
	args[4] = toString(av4);
	
	return Message(replaceArgs(args));
}						 

Message Message::operator()(const AnyValue &av0, const AnyValue &av1,
						    const AnyValue &av2, const AnyValue &av3,
						    const AnyValue &av4, const AnyValue &av5,
						    const AnyValue &av6, const AnyValue &av7,
						    const AnyValue &av8, const AnyValue &av9) const
{
	String args[MAX_ARGS];

	args[0] = toString(av0);
	args[1] = toString(av1);
	args[2]	= toString(av2);
	args[3] = toString(av3);
	args[4] = toString(av4);
	args[5] = toString(av5);
	args[6] = toString(av6);
	args[7]	= toString(av7);
	args[8] = toString(av8);
	args[9] = toString(av9);
	
	return Message(replaceArgs(args));
}						 

void Message::display() const
{
	::display(NULL_STRING, *this);
}

void Message::display(const Message &msg) const
{
	::display(msg, *this);
}

void Message::displayError() const
{
	::displayError(*this);
}

void Message::fatalError() const
{
	::fatalError(*this);
}

Message operator+(const Message &m1, const Message &m2)
{
	String aux = m1;
	aux += m2;
	return Message(aux);
}
