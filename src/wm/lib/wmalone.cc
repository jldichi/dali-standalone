/********************************************************************
* Copyright (c) 1987 InterSoft Co.  All Rights Reserved
* THIS IS UNPUBLISHED PROPRIETARY SOURCE CODE OF InterSoft Co.
* The copyright notice above does not evidence any
* actual or intended publication of such source code.
*
*
* $Id: wmalone.cc,v 1.2 2004/09/29 18:35:35 damianm Exp $
*
*
* DESCRIPTION:
*      Descripcion del proposito general del modulo
*	   rutinas para compilar en modo stdalone
*
* ROUTINE       |  PURPOSE
*---------------+---------------------------------------------------
*
*********************************************************************/

#include <ideafix.h>
#include <cfix.h>
#include <ideafix/priv/gndefs.h>
#include <wm.h>
#include <ifound/priv/gfuncs.h>

#include <signal.h>
#include <stdlib.h>
#include <sys/types.h>
#include <unistd.h>

#ifndef HAVE_TTY
#include <disp.h>
#endif

/* ++ Prototypes ++ */

static void quit(void);

/* -- Prototypes -- */

extern bool hab_susp;
#ifdef HAVE_ABORT
bool hab_abort = TRUE;
#else
bool hab_abort = FALSE;
#endif

char * dem_file = NULL;
bool read_demo = FALSE;
int curr_pid;
int rpid;

void _WiCreateProces(void)
{ }

void _WiDeleteProces(void)
{ }

void _WiProcess(void)
{ }

void WiSetFOpen(int i)
{ }

static void quit(void)
{
	OnStop((FP)abort);
	Stop(0); 
}

void WiConnect(char *tty, char *module, char *version)
{
	static bool init = FALSE;

	if (init)
		return;
	init = TRUE;
#ifndef HAVE_TTY
	disp_open();
#endif
	curr_pid = getpid();	
	hab_susp = FALSE;
	w_init();
	WiSettty();


#ifdef HAVE_FULL_SIGNALS
	SigSet(SIGINT, SIG_IGN);
	SigSet(SIGTERM, WiDisconnect);
	SigSet(SIGQUIT, SIG_IGN);
#if HAVE_SIGEMT
	SigSet(SIGEMT, quit);
#endif
#endif
	SetMessager(WiHandler);
	SetEnableIntHandler(WiInterrupts);
	OnStop((FP)WiDisconnect);
	WiSetAplHelp(module, version);
}	

void WiDisconnect(char *tty)
{
	WiDeleteAll();

	vs_setbackgr(A_NORMAL);
	vs_cursor(1);
	vs_moveto(VS_LINES - 1,0);
	vs_refresh();
	vs_erase(VS_LINES - 1, 0, 1, VS_COLS);
	vs_refresh();
	WiResettty();
#ifndef HAVE_TTY
	disp_close();
#endif
}
