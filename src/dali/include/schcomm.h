/********************************************************************
* Copyright (c) 1991 InterSoft Co.  All Rights Reserved
* THIS IS UNPUBLISHED PROPRIETARY SOURCE CODE OF InterSoft Co.
* The copyright notice above does not evidence any
* actual or intended publication of such source code.
*
* $Id: schcomm.h,v 1.1.1.1 1996/04/16 18:52:54 gustavof Exp $
*
* DESCRIPTION: Definitions for search command
*********************************************************************/

#ifndef SCHCOMM_H
#define SCHCOMM_H

#ifdef __GNUC__
#pragma interface
#endif

#ifdef ISDEBUG
#	include <ifound/iddebug.h>
#endif

#include <ifound/barray.h>
#include <winapp/log.h>

// comandos para find y search de texto

BeginPosCommand(FindText)
	Int 	sFilfrom;
	Int		sFilto;
	Int		sFlgs;
	String	sText;
public:
	void clear();
	void undo();
EndPosCommand(FindText);

BeginPosCommand(FindAgain)
EndPosCommand(FindAgain);

BeginPosCommand(FindNext)
EndPosCommand(FindNext);

BeginPosCommand(FindPrev)
EndPosCommand(FindPrev);

BeginAnyCommand(TokenCompletion)
	String previousText;
	Int offset;
	String newText;
	Int newOffset;
public:
	void clear();
EndAnyCommand(TokenCompletion);

BeginAnyCommand(ReplaceText)
	Log			theLog;
	BoolArray	which;
	Int			row;
	Int			col;
	Int			sFilfrom;
	Int			sFilto;
	Int			sFlgs;
	String		sText;
	String		sNewtext;
public:
	void clear();
EndAnyCommand(ReplaceText);

BeginAnyCommand(ReplaceAgain)
	Int		rRow;
	Int		rCol;
	Int		row;
	Int		col;
	Int		prevLen;
	String	str;
public:
	void	clear();
EndAnyCommand(ReplaceAgain);

BeginPosCommand(GoToLine)
	Int lineNo;			// fila a la que debo ir
public:
	void clear();
EndPosCommand(GoToLine);

BeginPosCommand(IncFind)
	Int 	sFilfrom;
	Int		sFilto;
	Int		sFlgs;
	String	sText;
	Int		row;
	Int		col;
public:
	void clear();
	void undo();
EndPosCommand(IncFind);

BeginPosCommand(FunctionBrowser)
	Int line;
	Int col;
public:
	void clear();
EndPosCommand(FunctionBrowser);

BeginPosCommand(TokenBrowser)
	Int line;
	Int col;
public:
	void clear();
EndPosCommand(TokenBrowser);


#endif
