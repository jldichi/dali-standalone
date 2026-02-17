/********************************************************************
* Copyright (c) 1987 InterSoft Co.  All Rights Reserved
* THIS IS UNPUBLISHED PROPRIETARY SOURCE CODE OF InterSoft Co.
* The copyright notice above does not evidence any
* actual or intended publication of such source code.
*
* pserconn.h,v 1.1.1.1 1996/04/16 18:51:50 gustavof Exp
*
* DESCRIPTION:
*********************************************************************/

#ifndef PSERCONN_H
#define PSERCONN_H

#ifdef __GNUC__
#pragma interface
#endif

#include <ifound/priv/pconfig.h>
#include <essentia/netconn.h>
#include <cracker/prcmd.h>

class PrServerConnection : public NetConnection {

	Int currcmd_;		// keep current command
	Int lastcmd_;		// keep last command
	bool buffcmd_;		// Is this cmd buffered ?

public:

	PrServerConnection(Int sd = 0, ConvType ct = NO_CONVERT);
	PrServerConnection(const String &, bool);

	Int currCmd()	{ return currcmd_; }
	Int lastCmd()	{ return lastcmd_; }
	operator bool() { return status() == OK; }
	PrServerConnection& buffCmd(Int cmd);
	PrServerConnection &operator()(Int cmd);

	virtual void flush();

	// Because of a gcc fucking bug.
	Connection &operator<<(Int i)
	{ return NetConnection::operator<<(i); }
	Connection &operator<<(const char *s)
	{ return NetConnection::operator<<(s); }
	Connection &operator<<(const String &s)
	{ return NetConnection::operator<<(s); }
	Connection &operator>>(Int &i)
	{ return NetConnection::operator>>(i); }
	Connection &operator>>(String &s)
	{ return NetConnection::operator>>(s); }
	Connection &operator>>(char *&cp)
	{ return NetConnection::operator>>(cp); }

};

#define prServerConn (*(_processConfig()->conn))

extern "C" bool crackerClient();

#endif
	
