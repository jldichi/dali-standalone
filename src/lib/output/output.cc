/********************************************************************
* Copyright (c) 1994 InterSoft Co.  All Rights Reserved
* THIS IS UNPUBLISHED PROPRIETARY SOURCE CODE OF InterSoft Co.
* The copyright notice above does not evidence any
* actual or intended publication of such source code.
*
* $Id: output.cc,v 1.2 2000/01/31 18:34:21 diegon Exp $
*
* DESCRIPTION
*********************************************************************/

#include <ifound/output.h>
#include <ifound/parray.h>

const Int COLUMNS_REALLOC = 4;

declare(PtrArray, BaseOutputColumn);
declare(PtrArrayCursor, BaseOutputColumn);

#define columns (*columns_)

BaseOutput::BaseOutput()
:	fp(NULL),
	ncols(0),
	currcol(0),
	currline(0),
	currpos(0),
	pwidth(0),
	pheight(0),
	twidth(0),
	offset(0),
	pageno(0),
	items(0),
	lines(NULL),
	hlines(0)
{
	columns_ = tr_new PtrArray(BaseOutputColumn)(0, COLUMNS_REALLOC);
}

BaseOutput::~BaseOutput()
{
	for (PtrArrayCursor(BaseOutputColumn) c(columns); c; ++c)
		if (*c) tr_delete(*c);
	tr_delete(columns_);
}

Int BaseOutput::columnNdec(Int n)
{
	return columns[n]->ndec;
}

UnShort BaseOutput::nCols()
{
	return ncols;
}

UnShort BaseOutput::currCol()
{
	return currcol;
}

UnShort BaseOutput::pageWidth()
{
	return pwidth;
}

Int BaseOutput::columnWidth(int n)
{
	return columns[n]->width;
}

void BaseOutput::setColumn(const String &h, Int minwidth, TypeId t, Int l, 
						  Int nd)
{
	if (h != NULL_STRING) {
		short hl = 1;
		char *aux = NULL;
		if (h != NULL_STRING) 
			aux = (char*)toCharPtr(h);		// (pablog)
		char *p;
		for (p = aux; *p; p++) {
			if (*p == '\n') {
				if (p-aux > minwidth) minwidth = (UnShort) (p-aux);
				aux = p+1;
				hl++;
			}
		}
		if (p-aux > minwidth) minwidth = (UnShort) (p-aux);
		if (hl > hlines) hlines = hl; 
	}
	columns.add(tr_new BaseOutputColumn(h, minwidth, t, l, nd));
	ncols++;
}

TerminalOutput *BaseOutput::getTerminalOutput()
{
	return NULL;
}

StreamOutput *BaseOutput::getStreamOutput()
{
	return NULL;
}

GraphOutput *BaseOutput::getGraphOutput()
{
	return NULL;
}

ReportOutput *BaseOutput::getReportOutput()
{
	return NULL;
}

DelimitedOutput *BaseOutput::getDelimitedOutput()
{
	return NULL;
}

RemoteOutput *BaseOutput::getRemoteOutput()
{
	return NULL;
}

GridOutput *BaseOutput::getGridOutput()
{
	return NULL;
}

CrackerOutput *BaseOutput::getCrackerOutput()
{
	return NULL;
}

Int BaseOutput::end()
{
	return items;
}

Int BaseOutput::operator<<(const String &s)
{
    if (s != NULL_STRING) 
    	return (*this) << toCharPtr(s);
	return (*this) << "";    	
}

