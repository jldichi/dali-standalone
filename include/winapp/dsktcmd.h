/********************************************************************
* Copyright (c) 1991 InterSoft Co.  All Rights Reserved
* THIS IS UNPUBLISHED PROPRIETARY SOURCE CODE OF InterSoft Co.
* The copyright notice above does not evidence any
* actual or intended publication of such source code.
*
* $Id: dsktcmd.h,v 1.1.1.1 1996/04/16 18:51:43 gustavof Exp $
*
* DESCRIPTION: Definitions for desktop commands
*********************************************************************/

#ifndef DSKTCMD_H
#define DSKTCMD_H

#ifdef __GNUC__
#pragma interface
#endif

#include <winapp/wincmd.h>
#include <winapp/cmdbind.h>

#define ViewMove_EVID(dummy)		COMPOSE(K_CTRLX, 'M')
#define ViewSize_EVID(dummy)		COMPOSE(K_CTRLX, 'Z')
#define ViewZoom_EVID(dummy)		K_IGNORE
#define DsktNext_EVID(dummy)		COMPOSE(K_CTRLX, 'n')
#define DsktPrev_EVID(dummy)		COMPOSE(K_CTRLX, 'p')
#define DsktSelect_EVID(dummy)		COMPOSE(K_CTRLX, 'l')
#define DsktMosaic_EVID(dummy)		COMPOSE(K_CTRLX, 'm')
#define DsktCascade_EVID(dummy)		COMPOSE(K_CTRLX, 'c')

void bindDesktopCmds(CmdBinding *cb);

BeginNoEmptyDesktopCmd(ViewZoom)
EndNoEmptyDesktopCmd(ViewZoom);

BeginNoEmptyDesktopCmd(ViewMove)
EndNoEmptyDesktopCmd(ViewMove);

BeginNoEmptyDesktopCmd(ViewSize)
EndNoEmptyDesktopCmd(ViewSize);

BeginNoEmptyDesktopCmd(DsktMosaic)
EndNoEmptyDesktopCmd(DsktMosaic);

BeginNoEmptyDesktopCmd(DsktCascade)
EndNoEmptyDesktopCmd(DsktCascade);

BeginNoEmptyDesktopCmd(DsktNext)
EndNoEmptyDesktopCmd(DsktNext);

BeginNoEmptyDesktopCmd(DsktPrev)
EndNoEmptyDesktopCmd(DsktPrev);

BeginEmptyDesktopCmd(DsktSelect)
EndEmptyDesktopCmd(DsktSelect);

#endif
