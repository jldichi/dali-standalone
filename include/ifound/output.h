/********************************************************************
* Copyright (c) 1994 InterSoft Co.  All Rights Reserved
* THIS IS UNPUBLISHED PROPRIETARY SOURCE CODE OF InterSoft Co.
* The copyright notice above does not evidence any
* actual or intended publication of such source code.
*
* $Id: output.h,v 1.2 1998/05/14 14:53:32 eduardoc Exp $
*
* DESCRIPTION
*********************************************************************/

#ifndef OUTPUT_H
#define OUTPUT_H

#ifndef BUFSIZ
#	include <stdio.h>
#endif

#include <ifound.h>
#include <ifound/str.h>

#define OUT_PIPE 		(UnShort) 0
#define OUT_FILE		(UnShort) 1
#define OUT_PRINTER		(UnShort) 2
#define OUT_TERMINAL	(UnShort) 3
#define OUT_DEFAULT		(UnShort) 4

struct BaseOutputColumn {
	String head;			// Headers
	UnShort width;			// Width of each column
	UnShort ty;				// Type of each column
	UnShort length;
	short ndec;
	BaseOutputColumn(const String &h, UnShort w, UnShort t, UnShort l, 
					 short nd) 
					 : head(h), width(w), ty(t), length(l), ndec(nd) 
	{ }
};

class gtype(PtrArray, BaseOutputColumn);

class TerminalOutput;
class StreamOutput;
class GraphOutput;
class ReportOutput;
class DelimitedOutput;
class RemoteOutput;
class GridOutput;
class CrackerOutput;
class String;


class BaseOutput {

protected:

	FILE *fp;						// PIPE, FILE, PRINTER or back paging file
	UnShort	ncols;					// Number of output columns
	UnShort	currcol;				// Current output column
	UnShort	currline;				// Current output line number
	UnShort	currpos;				// Current output pos. in line
	UnShort	pwidth;					// Page width
	UnShort	pheight;				// Page height
	UnShort	twidth;					// Total page width
	UnShort	offset;					// Current offset
	UnShort	pageno;					// Current pageno
	UnShort	items;					// Number of items listed
	UnChar **lines;					// Image of a whole output page
	short hlines;					// Number of lines of the header

	gtype(PtrArray, BaseOutputColumn) *columns_;
#ifdef	IDEAWIN
	friend class IWReport;
#endif

public:	

	BaseOutput();
	virtual ~BaseOutput();
	
	Int columnNdec(Int n);
	UnShort nCols();
	UnShort currCol();
	UnShort pageWidth();
	Int columnWidth(int n);
	void setColumn(const String &head, Int minwidth, TypeId t, Int length = 0, 
				   Int ndec = 0);

	virtual Int operator<<(const char *bp) = 0;
	Int	operator<<(const String &s);

 	virtual Int end();
	virtual TerminalOutput *getTerminalOutput();
	virtual StreamOutput *getStreamOutput();
	virtual GraphOutput *getGraphOutput();
	virtual ReportOutput *getReportOutput();
	virtual DelimitedOutput *getDelimitedOutput();
	virtual RemoteOutput *getRemoteOutput();
	virtual GridOutput *getGridOutput();
	virtual CrackerOutput *getCrackerOutput();

};

#endif


