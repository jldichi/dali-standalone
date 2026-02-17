/********************************************************************
* ORGLAND Ltd. IDEAFIX for MS Windows.
* Copyright (c) 1994 InterSoft Co.  All Rights Reserved
* THIS IS UNPUBLISHED PROPRIETARY SOURCE CODE OF InterSoft Co.
* The copyright notice above does not evidence any
* actual or intended publication of such source code.
*
* $Id: iwbars.h,v 1.2 1996/08/07 16:14:22 gab Exp $
*
* DESCRIPTION   : Extended name of module (one string)
*
* LAST CHANGE   : DATE
*
*********************************************************************/

#ifndef IWBARS_H
#define IWBARS_H

#include <iwwidefs.h>

/* Toolbar part ------------------------------------ */

#define		NOBAR	  (-1)
#define		TOPBAR		0
#define		LEFTBAR		1
#define		POPUPBAR	2

#define		BT_DOWN		0L
#define		BT_UP		1L
#define		BT_ACTION	2L

#define STRTBL				100

extern int BarType;
extern int	__nToolSize;

extern int WINAPI InitToolBar(HWND hWndPrnt, int nBarType, LPSTR resName);
extern void PASCAL MoveToolBar(int nX, int nY);
extern void WINAPI HideToolButton(int iCmdId, BOOL state);


extern int __nStatusHeight;
/* Dialog ------------------------------------------ */

extern "C" BOOL CALLBACK DesktopDlgProc(HWND hDlg, UINT msg, WPARAM wParam, LPARAM lParam);

#endif
