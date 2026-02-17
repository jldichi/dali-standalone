/********************************************************************
* Copyright (c) 1991 InterSoft Co.  All Rights Reserved
* THIS IS UNPUBLISHED PROPRIETARY SOURCE CODE OF InterSoft Co.
* The copyright notice above does not evidence any
* actual or intended publication of such source code.
*
* $Id: winmsg.cc,v 1.1.1.1 1996/04/16 18:52:45 gustavof Exp $
*
* DESCRIPTION:
* Connect/Disconnect stdMsgs with a window/stdErr
*********************************************************************/

#include <ifound.h>
#include <local/strfile.h>
#include <winapp/window.h>

static Window *MsgWin;

void connectWinMsg()
{
	MsgWin = tr_new Window;
	stdMsg_address = MsgWin;
}

void disconnectWinMsg()
{
	stdMsg_address = &stdErr;
	tr_delete(MsgWin);
}

// end of winmsg
