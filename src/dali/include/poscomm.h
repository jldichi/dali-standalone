/********************************************************************
* Copyright (c) 1991 InterSoft Co.  All Rights Reserved
* THIS IS UNPUBLISHED PROPRIETARY SOURCE CODE OF InterSoft Co.
* The copyright notice above does not evidence any
* actual or intended publication of such source code.
*
* $Id: poscomm.h,v 1.1.1.1 1996/04/16 18:52:54 gustavof Exp $
*
* DESCRIPTION:	Definitions for poscomm.
*********************************************************************/

#ifndef POSCOMM_H
#define POSCOMM_H

#ifdef __GNUC__
#pragma interface
#endif

#ifdef ISDEBUG
#	include <ifound/iddebug.h>
#endif

// comandos para el movimiento de cursor
BeginPosCommand(CursDown)
EndPosCommand(CursDown);

BeginPosCommand(CursUp)
EndPosCommand(CursUp);

BeginPosCommand(CursLeft)
EndPosCommand(CursLeft);

BeginPosCommand(PageUp)
EndPosCommand(PageUp);

BeginPosCommand(PageDown)
EndPosCommand(PageDown);

BeginPosCommand(CursRight)
EndPosCommand(CursRight);

BeginPosCommand(WordRight)
EndPosCommand(WordRight);

BeginPosCommand(WordLeft)
EndPosCommand(WordLeft);

BeginPosCommand(BegOfLine)
EndPosCommand(BegOfLine);

BeginPosCommand(EndOfLine)
EndPosCommand(EndOfLine);

BeginPosCommand(BegOfFile)
EndPosCommand(BegOfFile);

BeginPosCommand(EndOfFile)
EndPosCommand(EndOfFile);

BeginPosCommand(BegOfScreen)
EndPosCommand(BegOfScreen);

BeginPosCommand(MiddleOfScreen)
EndPosCommand(MiddleOfScreen);

BeginPosCommand(ForwFuncBegin)
EndPosCommand(ForwFuncBegin);

BeginPosCommand(BackFuncBegin)
EndPosCommand(BackFuncBegin);

BeginPosCommand(GoFirstNonSp)
EndPosCommand(GoFirstNonSp);

BeginPosCommand(EndOfScreen)
EndPosCommand(EndOfScreen);

BeginPosCommand(OuterBackMatch)
EndPosCommand(OuterBackMatch);

BeginPosCommand(OuterForwMatch)
EndPosCommand(OuterForwMatch);

BeginPosCommand(MarkPosition)
EndPosCommand(MarkPosition);

BeginPosCommand(GoMarkPosition)
EndPosCommand(GoMarkPosition);

#endif
