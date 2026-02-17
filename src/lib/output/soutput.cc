/********************************************************************
* Copyright (c) 1995 InterSoft Co.  All Rights Reserved
* THIS IS UNPUBLISHED PROPRIETARY SOURCE CODE OF InterSoft Co.
* The copyright notice above does not evidence any
* actual or intended publication of such source code.
*
* $Id: soutput.cc,v 1.3 2010/07/14 14:30:35 cristiand Exp $
*
* DESCRIPTION
*********************************************************************/

#include <ideafix.h>
#include <cfix.h>

#include <ifound/output.h>
#include <ideafix/priv/soutput.h>
#include <cracker/pserconn.h>

CrackerOutput::CrackerOutput(OutputType t, int nf) 
	: out_desc(ERR), 
	  nfils(nf), currfil(0),
	  columnone(false),
	  ty(t)
{
	last_refresh = _global_time(NULL);
}


Int  CrackerOutput::operator << (const char *bp)
{
	prServerConn.buffCmd(PrCmd::outputColumn) << (Int) out_desc << bp;

	Int result = OK;
	if (++currcol == nCols()) {
		currcol = 0;
		if (++currfil == nfils) {
			result = prServerConn.result();
			currfil = 0;
		}	 
	}	
	return  result;

	// Esta parte esta comentada porque producia un error al traer reportes de grandes volumenes de datos
	// por GIF (+ de 4000 legajos del "sue708 l", que hacian 3000 paginas aproximadamente).
	// Creemos que el codigo comentado fue origen de duplicar el comportamiento que sucede por Terminal, pero 
	// por GIF no tiene sentido, ademas de que actualiza la variable result, trayendo problemas.

	//else {
	//Int act_time = _global_time(NULL);
	//
	//if (act_time > last_refresh + RWAIT_TIME) {
	//result = prServerConn.result();
	//last_refresh = act_time;
	//}
	//}
}

