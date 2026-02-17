/********************************************************************
* Copyright (c) 1991 InterSoft Co.  All Rights Reserved
* THIS IS UNPUBLISHED PROPRIETARY SOURCE CODE OF InterSoft Co.
* The copyright notice above does not evidence any
* actual or intended publication of such source code.
*
* $Id: poscmd.h,v 1.1.1.1 1996/04/16 18:51:44 gustavof Exp $
*
* DESCRIPTION: Definitions for positional commands
*********************************************************************/

#ifndef POSCMD_H
#define POSCMD_H

#ifdef __GNUC__
#pragma interface
#endif

#include <winapp/wincmd.h>
#define	PosCursDown_EVID(dummy)		K_CURS_DOWN
#define	PosCursUp_EVID(dummy)		K_CURS_UP
#define	PosCursLeft_EVID(dummy)		K_CURS_LEFT
#define	PosCursRight_EVID(dummy)	K_CURS_RIGHT
#define	PosPageDown_EVID(dummy)		K_PAGE_DOWN
#define	PosPageUp_EVID(dummy)		K_PAGE_UP
#define	PosBegOfLine_EVID(dummy)	COMPOSE(K_META, K_CURS_LEFT)
#define	PosEndOfLine_EVID(dummy)	COMPOSE(K_META, K_CURS_RIGHT)
#define	PosBegOfFile_EVID(dummy)	COMPOSE(K_META, K_PAGE_UP)
#define	PosEndOfFile_EVID(dummy)	COMPOSE(K_META, K_PAGE_DOWN)
#define	PosBegOfScr_EVID(dummy)		COMPOSE(K_META, K_CURS_UP)
#define	PosMidOfScr_EVID(dummy)		COMPOSE(K_META, 'm')
#define	PosEndOfScr_EVID(dummy)		COMPOSE(K_META, K_CURS_DOWN)
#define	PosWordRight_EVID(dummy)	COMPOSE(K_CTRLX, K_CURS_RIGHT)
#define	PosWordLeft_EVID(dummy)		COMPOSE(K_CTRLX, K_CURS_LEFT)

void bindPosCmds(CmdBinding *cb);

BeginPosCmd(PosCursDown)
EndPosCmd(PosCursDown);

BeginPosCmd(PosCursUp)
EndPosCmd(PosCursUp);

BeginPosCmd(PosCursLeft)
EndPosCmd(PosCursLeft);

BeginPosCmd(PosPageUp)
EndPosCmd(PosPageUp);

BeginPosCmd(PosPageDown)
EndPosCmd(PosPageDown);

BeginPosCmd(PosCursRight)
EndPosCmd(PosCursRight);

BeginPosCmd(PosWordRight)
EndPosCmd(PosWordRight);

BeginPosCmd(PosWordLeft)
EndPosCmd(PosWordLeft);

BeginPosCmd(PosBegOfLine)
EndPosCmd(PosBegOfLine);

BeginPosCmd(PosEndOfLine)
EndPosCmd(PosEndOfLine);

BeginPosCmd(PosBegOfFile)
EndPosCmd(PosBegOfFile);

BeginPosCmd(PosEndOfFile)
EndPosCmd(PosEndOfFile);

BeginPosCmd(PosBegOfScr)
EndPosCmd(PosBegOfScr);

BeginPosCmd(PosMidOfScr)
EndPosCmd(PosMidOfScr);

BeginPosCmd(PosEndOfScr)
EndPosCmd(PosEndOfScr);

#endif

