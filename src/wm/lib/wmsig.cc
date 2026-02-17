/********************************************************************
* Copyright (c) 1987 InterSoft Co.  All Rights Reserved
* THIS IS UNPUBLISHED PROPRIETARY SOURCE CODE OF InterSoft Co.
* The copyright notice above does not evidence any
* actual or intended publication of such source code.
*
*
* $Id: wmsig.cc,v 1.1.1.1 1996/04/16 18:50:23 gustavof Exp $
*
*
* DESCRIPTION:
*      Signal manager 
*
* ROUTINE       |  PURPOSE
*---------------+---------------------------------------------------
*_send_signal	Manejo de senales 
*
*
*********************************************************************/

#include <ideafix.h>
#include <cfix.h>

#include <ideafix/priv/gndefs.h>
#include <ideafix/priv/wmcom.h>

#include <signal.h>

#ifndef HAVE_FULL_SIGNALS
static FPI handterm = Stop;
#endif

bool signal_sent;		/* indica si el wm envio un signal a un proc.*/

void _send_signal(int sig)
{
#ifdef HAVE_FULL_SIGNALS
	if (curr_pid < 2) return;
	kill(curr_pid,sig);
#else
	if (handterm) (*handterm)(0);
#endif
	signal_sent = TRUE;
}
