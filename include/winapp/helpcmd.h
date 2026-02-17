/********************************************************************
* Copyright (c) 1994 InterSoft Co.  All Rights Reserved
* THIS IS UNPUBLISHED PROPRIETARY SOURCE CODE OF InterSoft Co.
* The copyright notice above does not evidence any
* actual or intended publication of such source code.
*
* $Id: helpcmd.h,v 1.1.1.1 1996/04/16 18:51:44 gustavof Exp $
*
* DESCRIPTION: Definitions for help commands.
*********************************************************************/

#ifndef HELPCMD_H
#define HELPCMD_H

#ifdef __GNUC__
#pragma interface
#endif

// WinApp includes
#include <winapp/cmdbind.h>
#include <winapp/wincmd.h>

// IFound includes
#include <ifound/str.h>

#define	HelpIndex_EVID(cid)		MENUEVENT(cid)
#define	HelpHelp_EVID(cid)		MENUEVENT(cid)
#define HelpOnCmd_EVID(dummy)	COMPOSE(K_META, K_HELP)
#define XRefNext_EVID(dummy)	K_TAB
#define XRefPrev_EVID(dummy)	COMPOSE(K_META, K_TAB)
#define XRefGo_EVID(dummy)		K_ENTER

BeginEmptyDesktopCmd(HelpIndex)
EndEmptyDesktopCmd(HelpIndex);

BeginEmptyDesktopCmd(HelpHelp)
EndEmptyDesktopCmd(HelpHelp);

BeginNoEmptyDesktopCmd(HelpOnCmd)
EndNoEmptyDesktopCmd(HelpOnCmd);

BeginNoEmptyDesktopCmd(XRefNext)
EndNoEmptyDesktopCmd(XRefNext);

BeginNoEmptyDesktopCmd(XRefPrev)
EndNoEmptyDesktopCmd(XRefPrev);

BeginNoEmptyDesktopCmd(XRefGo)
EndNoEmptyDesktopCmd(XRefGo);

#endif
