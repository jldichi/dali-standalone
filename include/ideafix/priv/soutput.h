/********************************************************************
* Copyright (c) 1995 InterSoft Co.  All Rights Reserved
* THIS IS UNPUBLISHED PROPRIETARY SOURCE CODE OF InterSoft Co.
* The copyright notice above does not evidence any
* actual or intended publication of such source code.
*
* $Id: soutput.h,v 1.2 1996/04/29 14:15:59 gab Exp $
*
* DESCRIPTION
*********************************************************************/

#ifndef SOUTPUT_H
#define SOUTPUT_h

enum OutputType {
	output_term,
	output_reprt,
	output_delim,
	output_formatted,
	output_remote,
	output_graph,
};

#define RWAIT_TIME 5

class CrackerOutput : public BaseOutput{
public:
	Int  out_desc;				// output descriptor
	Int  nfils, currfil;
	Int  last_refresh;
	bool columnone;	
	OutputType ty;
	Int operator<<(const char *bp);
	
	CrackerOutput *getCrackerOutput() {return this;}
	CrackerOutput(OutputType t, int nf = 24);
	~CrackerOutput() {};
};


#endif
