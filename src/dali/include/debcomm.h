/********************************************************************
* Copyright (c) 1991 InterSoft Co.  All Rights Reserved
* THIS IS UNPUBLISHED PROPRIETARY SOURCE CODE OF InterSoft Co.
* The copyright notice above does not evidence any
* actual or intended publication of such source code.
*
* $Id: debcomm.h,v 1.1.1.1 1996/04/16 18:52:53 gustavof Exp $
*
* DESCRIPTION:	Definitions for commdef.
*********************************************************************/

#ifndef DEBCOMM_H
#define DEBCOMM_H

#ifdef __GNUC__
#pragma interface
#endif

#ifdef ISDEBUG
#	include <ifound/iddebug.h>
#endif
           
#include <dview.h>           
#include <local/strfile.h>    

//
// comandos para Debug
//

BeginControlCommandNoViews(DbgStart)
EndControlCommandNoViews(DbgStart);

BeginControlCommandNoViews(DbgStop)
EndControlCommandNoViews(DbgStop);

BeginControlCommandNoViews(DbgNext)
EndControlCommandNoViews(DbgNext);

BeginControlCommandNoViews(DbgNextI)
EndControlCommandNoViews(DbgNextI);

BeginControlCommandNoViews(DbgStep)
EndControlCommandNoViews(DbgStep);

BeginControlCommandNoViews(DbgStepI)
EndControlCommandNoViews(DbgStepI);

BeginControlCommandNoViews(DbgRun)
EndControlCommandNoViews(DbgRun);

BeginControlCommandNoViews(DbgContinue)
EndControlCommandNoViews(DbgContinue);

BeginControlCommandNoViews(DbgPrintThis)
EndControlCommandNoViews(DbgPrintThis);

BeginControlCommandNoViews(DbgPrint)
EndControlCommandNoViews(DbgPrint);

BeginControlCommandNoViews(DbgWatch)
EndControlCommandNoViews(DbgWatch);

BeginControlCommandNoViews(DbgDisplay)
EndControlCommandNoViews(DbgDisplay);

BeginControlCommandNoViews(DbgCmdBox)
EndControlCommandNoViews(DbgCmdBox);

BeginControlCommandNoViews(DbgInsBreak)
EndControlCommandNoViews(DbgInsBreak);

BeginControlCommandNoViews(DbgDelBreak)
EndControlCommandNoViews(DbgDelBreak);

BeginControlCommandNoViews(DbgPrintVar)
EndControlCommandNoViews(DbgPrintVar);

BeginControlCommandNoViews(DbgTypeVar)
EndControlCommandNoViews(DbgTypeVar);

BeginControlCommandNoViews(DbgStack)
EndControlCommandNoViews(DbgStack);

BeginControlCommandNoViews(DbgRegs)
	static String* fmtRegs(const String &streg);
EndControlCommandNoViews(DbgRegs);

BeginControlCommandNoViews(DbgDisplays)
	static String* fmtDisplays(const String &streg);
EndControlCommandNoViews(DbgDisplays);

BeginControlCommandNoViews(DbgWatchBreaks)
	static String* fmtWatchBreaks(const String &streg);
EndControlCommandNoViews(DbgWatchBreaks);

#endif
