/********************************************************************
* Copyright (c) 1991 InterSoft Co.  All Rights Reserved
* THIS IS UNPUBLISHED PROPRIETARY SOURCE CODE OF InterSoft Co.
* The copyright notice above does not evidence any
* actual or intended publication of such source code.
*
* $Id: commdef.h,v 1.1.1.1 1996/04/16 18:52:53 gustavof Exp $
*
* DESCRIPTION: Definitions for commdef.
*********************************************************************/

#ifndef COMMDEF_H
#define COMMDEF_H

#ifdef __GNUC__
#pragma interface
#endif

#ifdef ISDEBUG
#	include <ifound/iddebug.h>
#endif

#include <ifound/str.h>

#include <winapp/log.h>

// history

BeginControlCommand(CommHis)
EndControlCommand(CommHis);

BeginAnyCommand(JoinLines)
	Int cfil, ccol; 	// coordenadas del cursor
EndAnyCommand(JoinLines);

BeginAnyCommand(InsNewLine)
	Int indent;
	Int cfil, ccol; 	// coordenadas del cursor
EndAnyCommand(InsNewLine);

BeginAnyCommand(AddNewLine)
	Int indent;
	Int cfil, ccol; 	// coordenadas del cursor
EndAnyCommand(AddNewLine);

BeginAnyCommand(Tab)
EndAnyCommand(Tab);

BeginControlCommand(ShiftRight)
	Int		rowBlk;
	Int		colBlk;
	Int		rowCur;
	Int		colCur;
EndControlCommand(ShiftRight);

BeginControlCommand(ShiftLeft)
	Int		rowBlk;
	Int		colBlk;
	Int		rowCur;
	Int		colCur;
	bool	needRestore;
	bool	didSomething;
	Log		theLog;
EndControlCommand(ShiftLeft);

// comandos varios

BeginControlCommand(DaliChgInsMode)	
EndControlCommand(DaliChgInsMode);

BeginControlCommand(DaliChgCtrolMode)	
EndControlCommand(DaliChgCtrolMode);

BeginControlCommand(SetTroffMode)	
EndControlCommand(SetTroffMode);

BeginControlCommand(SetTeXMode)	
EndControlCommand(SetTeXMode);

BeginAnyCommand(PasteBlock)	
	Int hfil;			// fila hasta donde tengo que borrar
	Int hcol;			// columna hasta donde tengo que borrar
EndAnyCommand(PasteBlock);

BeginAnyCommand(BackTab)	
EndAnyCommand(BackTab);

BeginControlCommand(BeginBlock)	
EndControlCommand(BeginBlock);

BeginControlCommandNoViews(CommMode)
EndControlCommandNoViews(CommMode);

#endif
