/********************************************************************
* Copyright (c) 1994 InterSoft Co.  All Rights Reserved
* THIS IS UNPUBLISHED PROPRIETARY SOURCE CODE OF InterSoft Co.
* The copyright notice above does not evidence any
* actual or intended publication of such source code.
*
* $Id: graphout.h,v 1.1.1.1 1996/04/16 18:51:38 gustavof Exp $
*
* DESCRIPTION
*********************************************************************/

#ifndef GRAPHOUTPUT_H
#define GRAPHOUTPUT_H

#include <ifound/output.h>

#ifndef STR_H
#	include <ifound/str.h>
#endif

#include <ideafix.h>
#include <ideafix/rpgraph.h>
#include <ixwi/wi.h>

typedef Int (GraphOutput::*GraphMethod)();

class GraphOutput : public BaseOutput {

	UnShort	dest;		// Output destination
	window wp;			// associated window
	attr_type backgr;	// window background
	UnShort	gty;
	UnShort	glength;
	UnShort	gwidth;
	bool inverted;
	String gtitle;
	String gstitle;
	String glabelx;
	String glabely;
	
	
public:

	GraphOutput(Int Type, const String &gtitle, const String &gstitle, 
				const String &glabelx, const String &glabely, bool direc, 
				bool toprinter, Int grwidth = ERR, Int grlength = ERR,
				Int frwidth = ERR, Int frlength = ERR, Int ncopies = 1);
	~GraphOutput();

	void set(Int gwidth, Int glength, Int fwidth, Int flength);

	Int operator<<(const char *bp);

	Int end();

	GraphOutput *getGraphOutput();
};

#endif
