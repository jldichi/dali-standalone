/********************************************************************
* Copyright (c) 1994 InterSoft Co.  All Rights Reserved
* THIS IS UNPUBLISHED PROPRIETARY SOURCE CODE OF InterSoft Co.
* The copyright notice above does not evidence any
* actual or intended publication of such source code.
*
* $Id: strout.h,v 1.1.1.1 1996/04/16 18:51:38 gustavof Exp $
*
* DESCRIPTION
*********************************************************************/

#ifndef STREAMOUTPUT_H
#define STREAMOUTPUT_H

#include <ideafix/priv/gridout.h>

#ifndef STR_H
#	include <ifound/str.h>
#endif

class StreamOutput : public GridOutput {

	UnShort	dest;		// Output destination
	String heading;
	String footing;
	
	void _LogInFile();
	Int _GetCmd(bool f);
	void _Refresh();
	Int _OutPage(bool more, bool less);

	Int _CalcWidth(Int offset, bool cont);
	void _OutTitle(bool heading);
	Int _CountLf(char *s);

public:

	StreamOutput(Int dest, const String &arg, const String &heading, 
				 const String &footing, Int fwidth, Int flen, Int topm, 
				 Int botm, Int leftm, Int ncopies);
	~StreamOutput();

	StreamOutput *getStreamOutput()	 { return this; }
};

#endif
