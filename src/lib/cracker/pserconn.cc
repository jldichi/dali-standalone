/********************************************************************
* Copyright (c) 1995 InterSoft Co.  All Rights Reserved
* THIS IS UNPUBLISHED PROPRIETARY SOURCE CODE OF InterSoft Co.
* The copyright notice above does not evidence any
* actual or intended publication of such source code.
*
* pserconn.cc,v 1.1.1.1 1996/04/16 18:52:26 gustavof Exp
*
* DESCRIPTION:
*********************************************************************/

#ifdef __GNUC__
#pragma implementation
#endif
#include <ifound.h>
#include <ifound/stdc.h>

#include <cracker/pserconn.h>

#ifndef IDEAWIN
PrServerConnection::PrServerConnection(Int sd, ConvType ct)
	: NetConnection(sd,ct),
	  currcmd_(ERR),
	  lastcmd_(ERR),
	  buffcmd_(false)
{
}

PrServerConnection::PrServerConnection(const String &s, bool localCli)
	: NetConnection(s, localCli),
	  currcmd_(ERR),
	  lastcmd_(ERR),
	  buffcmd_(false)
{
}

void PrServerConnection::flush()
{
	if (buffcmd_) {
		(*this) << EO_FRAME;
		buffcmd_ = false;
	}
	NetConnection::flush();
	lastcmd_ = currcmd_;    // keep last command
	currcmd_ = ERR;			// clear current command
}

PrServerConnection& PrServerConnection::operator()(Int cmd)
{
	if (currcmd_ != ERR && currcmd_ != cmd) {
		if (buffcmd_) {
			(*this) << EO_COMMAND;
			buffcmd_ = false;
		}
		flush();
	}
	currcmd_ = cmd;
	NetConnection::operator()(cmd);
	return *this;
}

PrServerConnection& PrServerConnection::buffCmd(Int cmd)
{
	if (currcmd_ != cmd)
		(*this)(cmd);
	(*this) << MOREDATA;
	buffcmd_ = true;
	return *this;
}

#else
PrServerConnection& PrServerConnection::operator()(Int cmd)
{
	assert(0);
	return *this;
}

PrServerConnection& PrServerConnection::buffCmd(Int cmd)
{
	assert(0);
	return *this;
}

#endif
