/********************************************************************
* Copyright (c) 1994 InterSoft Co.  All Rights Reserved
* THIS IS UNPUBLISHED PROPRIETARY SOURCE CODE OF InterSoft Co.
* The copyright notice above does not evidence any
* actual or intended publication of such source code.
*
* $Id: gridout.h,v 1.1.1.1 1996/04/16 18:51:38 gustavof Exp $
*
* DESCRIPTION
*********************************************************************/

#ifndef GRIDOUTPUT_H
#define GRIDOUTPUT_H

#include <ifound/output.h>

class GridOutput : public BaseOutput {

protected:

	bool list;			// List tabs as \t
	UnShort	topmarg;	// Top margin
	UnShort	leftmarg;	// Left margin

	virtual Int _InitOutput();
	virtual void _LogInFile() = 0;
	virtual Int _GetCmd(bool f) = 0;
	virtual void _Refresh() = 0;
	virtual Int _OutPage(bool more, bool less) = 0;
	
	void _Heading();
	void _BlankLine(Int n);

public:

	GridOutput();
	virtual ~GridOutput();

	void setList();
	virtual Int operator<<(const char *bp);

	GridOutput *getGridOutput()		{ return this; }
};

#endif
