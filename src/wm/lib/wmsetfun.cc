/********************************************************************
* Copyright (c) 1987 InterSoft Co.  All Rights Reserved
* THIS IS UNPUBLISHED PROPRIETARY SOURCE CODE OF InterSoft Co.
* The copyright notice above does not evidence any
* actual or intended publication of such source code.
*
*
* $Id: wmsetfun.cc,v 1.4 2004/02/12 19:40:47 diegoa Exp $
*
*
* DESCRIPTION:
*      Descripcion del proposito general del modulo
*
* ROUTINE       |  PURPOSE
*---------------+---------------------------------------------------
*
*********************************************************************/

#include <ideafix.h>
#include <cfix.h>
#include <ideafix/priv/wmcom.h>
#include <wm.h>


static struct _wm_commands *wm_commands;
static int wm_ncmds;

void WmStdAlone(UShort nf)
{
	WiFlush();
	if (nf < (UShort)(wm_ncmds)) (*wm_commands[nf].fp)();
}

void WmSetStdAlone(void)
{
	ExecWm = (FPI) WmStdAlone;

	wm_commands = _WmCommands(&wm_ncmds);

	wm_commands[WM_STOP].fp  		= NULL /* _WiStop */;
	wm_commands[WM_WICREATE].fp  	= _WiCreate;
	wm_commands[WM_WIDELETE].fp		= _WiDelete;
	wm_commands[WM_WIDELETEALL].fp 	= _WiDeleteAll;
	wm_commands[WM_WISWITCHTO].fp 	= _WiSwitchTo;
	wm_commands[WM_WIREFRESH].fp 	= _WiRefresh;
	wm_commands[WM_WIREDRAW].fp 	= _WiRedraw;
	wm_commands[WM_WISETBORDER].fp 	= _WiSetBorder;
	wm_commands[WM_WISTATUSLINE].fp = _WiStatusLine;
	wm_commands[WM_WIGETATTR].fp 	= _WiGetAttr;
	wm_commands[WM_WIORGCOL].fp 	= _WiOrgCol;
	wm_commands[WM_WIORGROW].fp 	= _WiOrgRow;
	wm_commands[WM_WILINE].fp 		= _WiLine;
	wm_commands[WM_WICOL].fp 		= _WiCol;
	wm_commands[WM_WIWIDTH].fp 		= _WiWidth;
	wm_commands[WM_WIHEIGHT].fp		= _WiHeight;
	wm_commands[WM_WIINCHAR].fp 	= _WiInChar;
	wm_commands[WM_WIINATTR].fp 	= _WiInAttr;
	wm_commands[WM_WIGETTAB].fp 	= _WiGetTab;
	wm_commands[WM_WISETTAB].fp 	= _WiSetTab;
	wm_commands[WM_WIPARENT].fp 	= _WiParent;
	wm_commands[WM_WIFLUSH].fp 		= _WiFlush;
	wm_commands[WM_WIGETC].fp 		= _WiGetc;
	wm_commands[WM_WIGETS].fp 		= _WiGets;
	wm_commands[WM_WIKEYHELP].fp 	= _WiKeyHelp;
	wm_commands[WM_WIINTERRUPTS].fp = _WiInterrupts;
	wm_commands[WM_WICURRENT].fp 	= _WiCurrent;
	wm_commands[WM_WICREATEPRO].fp  = _WiCreateProces;
	wm_commands[WM_WIDELETEPRO].fp 	= _WiDeleteProces;
	wm_commands[WM_WISETTTY].fp  	= _WiSettty;
	wm_commands[WM_WIRESETTTY].fp 	= _WiResettty;
	wm_commands[WM_WIGETFIELD].fp 	= _WiGetField;
	wm_commands[WM_WIAPLHELP].fp 	= _WiAplHelp;
	wm_commands[WM_WISETAPLHELP].fp	= _WiSetAplHelp;
	wm_commands[WM_WIPROCESS].fp	= _WiProcess;
	wm_commands[WM_WISETRAW].fp		= _WiSetRaw;
	wm_commands[WM_WIKEYREADY].fp	= _WiKeyReady;
#ifdef HAVE_PROC
#ifndef __NT__
	wm_commands[WM_WISETSERVICES].fp= _WiSetServices;
	wm_commands[WM_WIMOVE].fp		= _WiMove;
	wm_commands[WM_WIRESIZE].fp		= _WiResize;
	wm_commands[WM_WISERVICE].fp	= _WiService;
#endif
#endif


	wm_commands[WM_WIGETSESSION].fp	= _GetSessionWM;
	wm_commands[WM_WISETSESSION].fp	= _SetSessionWM;
}
