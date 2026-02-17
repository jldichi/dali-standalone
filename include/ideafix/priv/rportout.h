/********************************************************************
* Copyright (c) 1994 InterSoft Co.  All Rights Reserved
* THIS IS UNPUBLISHED PROPRIETARY SOURCE CODE OF InterSoft Co.
* The copyright notice above does not evidence any
* actual or intended publication of such source code.
*
* $Id: rportout.h,v 1.1.1.1 1996/04/16 18:51:38 gustavof Exp $
*
* DESCRIPTION
*********************************************************************/

#ifndef REPORTOUTPUT_H
#define REPORTOUTPUT_H

#include <ifound/output.h>

#ifndef STR_H
#	include <ifound/str.h>
#endif

#include <cfix.h>

class ReportRep;

class ReportOutput : public BaseOutput {

	ReportRep *rp;			// Report for output to REPORT
	String fmt;
	Int ncopies;

public:

	ReportOutput(const String &rpname, Int ncopies, const String &arg);
	~ReportOutput();

	void setDest(Report::OutputType out_to, char *dest);
	Int operator<<(const char *bp);

	ReportOutput *getReportOutput()		{ return this; }
	
};

#endif
