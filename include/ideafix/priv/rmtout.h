/********************************************************************
* Copyright (c) 1994 InterSoft Co.  All Rights Reserved
* THIS IS UNPUBLISHED PROPRIETARY SOURCE CODE OF InterSoft Co.
* The copyright notice above does not evidence any
* actual or intended publication of such source code.
*
* $Id: rmtout.h,v 1.1.1.1 1996/04/16 18:51:38 gustavof Exp $
*
* DESCRIPTION
*********************************************************************/

#ifndef REMOTEOUTPUT_H
#define REMOTEOUTPUT_H

#include <ideafix/priv/delimout.h>

class String;

class RemoteOutput : public DelimitedOutput {

public:

	RemoteOutput(Int dest, const String &arg, const String &sep);
	~RemoteOutput();

	void outputHeader();

	Int operator<<(const char *bp);

	RemoteOutput *getRemoteOutput()		{ return this; }
};

#endif

