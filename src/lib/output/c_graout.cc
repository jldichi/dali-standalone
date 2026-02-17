/********************************************************************
* Copyright (c) 1994 InterSoft Co.  All Rights Reserved
* THIS IS UNPUBLISHED PROPRIETARY SOURCE CODE OF InterSoft Co.
* The copyright notice above does not evidence any
* actual or intended publication of such source code.
*
* $Id: c_graout.cc,v 1.1.1.1 1996/04/16 18:52:07 gustavof Exp $
*
* DESCRIPTION
*********************************************************************/

#include <ideafix.h>
#include <cfix.h>

#include <ideafix/priv/graphout.h>
#include <ideafix/priv/soutput.h>

#include <cracker/pserconn.h>

extern "C"  Output OpenGraphOutput(int Type, UChar *gt,
									UChar *gst, UChar *glx,
									UChar *gly, bool direc, 
									bool toprinter)
{
	if (!crackerClient()) 
		return tr_new GraphOutput(Type, (char*)gt, (char*)gst, 
							    (char*)glx, (char*)gly, direc, toprinter);
	else {
	    prServerConn(PrCmd::openGraphOutput) << Int(Type) << (char*)gt
		    	<< (char*)gst << (char*)glx << (char*)gly 
		    	<< Int(direc) << Int(toprinter);
	    CrackerOutput *co = tr_new CrackerOutput(output_graph);
		co->out_desc = prServerConn.result();    	
		return co;
	}
}

extern "C" void SetGraphOutput(Output out, int gw, int gl, 
							   int fwidth, int flength)
{
	CrackerOutput *co = out->getCrackerOutput();
	if (co) {
	    prServerConn(PrCmd::setGraphOutput) << (Int) (co->out_desc )
	    			<< Int(gw) << Int(gl) << Int(fwidth) << Int(flength);
		prServerConn.flush();
	} 
	else {
		GraphOutput *goutput = out->getGraphOutput();
		assert(goutput != NULL);
		goutput->set(gw, gl, fwidth, flength);
	}
}

extern "C" bool IsGraphOutput(Output out)
{
	CrackerOutput *co = out->getCrackerOutput();
	if (co) 
		return (co->ty == output_graph);
	else 
		return out->getGraphOutput() != NULL;
}
