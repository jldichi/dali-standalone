/********************************************************************
* Copyright (c) 1991 InterSoft Co.  All Rights Reserved
* THIS IS UNPUBLISHED PROPRIETARY SOURCE CODE OF InterSoft Co.
* The copyright notice above does not evidence any
* actual or intended publication of such source code.
*
* $Id: syscmd.h,v 1.1.1.1 1996/04/16 18:51:44 gustavof Exp $
*
* DESCRIPTION: Definitions for standard System commands.
*********************************************************************/

#ifndef SYSCMD_H
#define SYSCMD_H

#ifdef __GNUC__
#pragma interface
#endif

#include <winapp/wincmd.h>
#include <winapp/cmdbind.h>

#define SysCalculator_EVID(cid)     MENUEVENT(cid)
#define SysCharSet_EVID(cid)       	MENUEVENT(cid)
#define SysPrintScreen_EVID(cid)    MENUEVENT(cid)

#define SysGotoShell_EVID(dummy)    COMPOSE(K_META, 'z')
#define SysDsktRefresh_EVID(dummy)  COMPOSE(K_META, 'R')

void bindSysCmds(CmdBinding *cb);

BeginEmptyDesktopCmd(SysCalculator)
EndEmptyDesktopCmd(SysCalculator);

BeginEmptyDesktopCmd(SysPrintScreen)
EndEmptyDesktopCmd(SysPrintScreen);

BeginEmptyDesktopCmd(SysCharSet)
EndEmptyDesktopCmd(SysCharSet);

BeginEmptyDesktopCmd(SysGotoShell)
EndEmptyDesktopCmd(SysGotoShell);

BeginEmptyDesktopCmd(SysDsktRefresh)
EndEmptyDesktopCmd(SysDsktRefresh);

#endif

