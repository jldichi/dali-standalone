/********************************************************************
* Copyright (c) 1994 InterSoft Co.  All Rights Reserved
* THIS IS UNPUBLISHED PROPRIETARY SOURCE CODE OF InterSoft Co.
* The copyright notice above does not evidence any
* actual or intended publication of such source code.
*
* $Id: c_output.cc,v 1.2 1996/04/29 15:15:01 gab Exp $
*
* DESCRIPTION
*********************************************************************/

#include <ideafix.h>
#include <cfix.h>

#include <ideafix/priv/termout.h>
#include <ideafix/priv/strout.h>
#include <ideafix/priv/rportout.h>
#include <ideafix/priv/delimout.h>
#include <ideafix/priv/rmtout.h>
#include <ideafix/priv/soutput.h>

#include <ifound/str.h>
#include <cracker/pserconn.h>

static Int convDestination(int dst)
{
	switch (dst) {
	case RP_IO_FILE:
		return OUT_FILE;
	case RP_IO_PIPE:
		return OUT_PIPE;
	case RP_IO_DEFAULT:
	// case RP_IO_PRINTER:
	default:
		return OUT_PRINTER;
	}
}

static Report::OutputType convOutputType(rp_output out)
{
	switch (out) {
	case RP_IO_DEFAULT:
		return Report::io_default;
//	case RP_IO_PRINTER:
//		return Report::io_printer;
	case RP_IO_FILE:
		return Report::io_file;
	case RP_IO_PIPE:
			return Report::io_pipe;
	case RP_IO_TERM:
		return Report::io_term;
	case RP_IO_REPORT:
		return Report::io_report;
	case RP_IO_GRAPH:
		return Report::io_graph;
	default:
		assert(0);
		return Report::OutputType(ERR);
	}
}

extern "C" Output OpenTermOutput(int f_org, int c_org, int height, int width, 
							  	  attr_type backgr, IFP getcmd)
{
	if (!crackerClient()) 
		return tr_new TerminalOutput(f_org, c_org, height, width, backgr, 
								   getcmd);
	else {
	    prServerConn(PrCmd::openTermOutput) << (Int) f_org << (Int) c_org
	    	<< (Int) height << (Int) width << (Int) backgr;
		CrackerOutput *ret = tr_new CrackerOutput(output_term,height);
		ret->out_desc = prServerConn.result();    	
		return ret;
	}
}

extern "C" Output OpenRpOutput(char *rpname, int ncopies, char *arg)
{
	return tr_new ReportOutput(rpname, ncopies, arg);
}


extern "C" Output OpenDelimOutput(int dst, char *arg, char *sep, 
								   int ncopies)
{
	if (!crackerClient()) 
		return  tr_new DelimitedOutput(convDestination(dst), arg, sep, ncopies);
	else {
	    CrackerOutput *ret = tr_new CrackerOutput(output_delim);
	    prServerConn(PrCmd::openDelimOutput) << (Int) dst << arg << sep
	    	<< (Int) ncopies;
		ret->out_desc = prServerConn.result();    	
		return ret;
	}
}

extern "C" Output OpenFmtOutput(int dst, UChar *arg, UChar *heading, 
								 UChar *footing, int fwidth, int flen, 
								 int topm, int botm, int leftm, int ncopies)
{
	if (!crackerClient()) 
		return tr_new StreamOutput(convDestination(dst), (char*)arg, 
								(char*)heading,	(char*)footing,	fwidth, flen,
								topm, botm, leftm, ncopies);
	 else {
	    prServerConn(PrCmd::openFmtOutput) << (Int) dst << (char*) arg 
	    	<< (char*) heading << (char*) footing << (Int) fwidth 
	    	<< (Int) flen << (Int) topm	<< (Int) botm << (Int) leftm 
	    	<< (Int) ncopies;
		CrackerOutput *ret = tr_new CrackerOutput(output_formatted, flen);
		ret->out_desc = prServerConn.result();    	
		return ret;
    }
}

extern "C" Output OpenRemoteOutput(int dst, char *arg, char *sep)
{
	if (!crackerClient()) 
		return  tr_new RemoteOutput(convDestination(dst), arg, sep);
	else {
		CrackerOutput *ret = tr_new CrackerOutput(output_remote);
	    prServerConn(PrCmd::openRemoteOutput) << (Int) dst << arg << sep;
		ret->out_desc = prServerConn.result();    	
		return ret;
	}
}

extern "C" int SetOutputColumn(Output out, char *head, int minwidth, 
							   type t, ...)
{
	CrackerOutput *co = out->getCrackerOutput();
	if (!co) {
		Int length = 0, ndec = 0;
		va_list ap;
		va_start(ap, t);
		if (t != TY_STRING) {
			length = va_arg(ap, int);
			ndec   = va_arg(ap, int);
		}
		va_end(ap);
		out->setColumn(head, minwidth, t, length, ndec);
		return OK;
	} else {
	    prServerConn.buffCmd(PrCmd::setOutputColumn) << (Int) co->out_desc 
	    	<< head << (Int) minwidth << (Int) t;

		Int length = 0, ndec = 0;
		va_list ap;
		va_start(ap, t);
		if (t != TY_STRING) {
			length = va_arg(ap, int);
			ndec   = va_arg(ap, int);
			prServerConn << length << ndec;
		}
		va_end(ap);                       
		
		out->setColumn(head, minwidth, t, length, ndec);
		return OK;
	}
}

extern "C" int ColumnWidth(Output out, int n)
{
	return int(out->columnWidth(n));
}

extern "C" void SetList(Output out)
{
	CrackerOutput *co = out->getCrackerOutput();
	if (co) {
	    prServerConn(PrCmd::setList) << (Int) co->out_desc;
		prServerConn.flush();    	
	} else {
		GridOutput *goutput = out->getGridOutput();
		assert(goutput != NULL);
		goutput->setList();
	}
}

extern "C" int OutputColumn(Output out, char *bp)
{
	return (int)((*out) << bp);
}

extern "C" int CloseOutput(Output out)
{
	CrackerOutput *co = out->getCrackerOutput();
	if (co) {
	    prServerConn(PrCmd::closeOutput) << (Int) co->out_desc;
		tr_delete(co);
		Int res = prServerConn.result();	
		return res;
    } else {
		int ret = int(out->end());
		tr_delete(out);
		return ret;
	}
}

extern "C" void SetRpOutput(Output out, rp_output out_to, char *dst)
{
	CrackerOutput *co = out->getCrackerOutput();
	if (co) {
	    prServerConn(PrCmd::setRpOutput) << (Int) co->out_desc << 
	    	(Int) out_to << dst;
		prServerConn.flush();
	} else {
		ReportOutput *routput = out->getReportOutput();
		assert(routput != NULL);
		routput->setDest(convOutputType(out_to), dst);
	}
}

extern "C" int ColumnNdec(Output out, int n)
{
	return int(out->columnNdec(n));
}

extern "C" UShort NColsOutput(Output out)
{

	return out->nCols();
}

extern "C" UShort CurrColOutput(Output out)
{
	return out->currCol();
}

extern "C" UShort PageWidthOutput(Output out)
{
	CrackerOutput *co = out->getCrackerOutput();
	if (!co)
		return out->pageWidth();
	else {
	    prServerConn(PrCmd::pageWidthOutput) << (Int) co->out_desc;
		return (UShort) prServerConn.result();	
	}
}

extern "C" bool IsNullOutput(Output out)
{
	return out == NULL;		
}

extern "C" void SetNullOutput(Output out)
{
	out = NULL;
}

