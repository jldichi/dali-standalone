/********************************************************************
* Copyright (c) 1993 InterSoft Co.	All Rights Reserved
* THIS IS UNPUBLISHED PROPRIETARY SOURCE CODE OF InterSoft Co.
* The copyright notice above does not evidence any
* actual or intended publication of such source code.
*
* MODULE: $Id: wappdbg.h,v 1.1.1.1 1996/04/16 18:51:44 gustavof Exp $
*
* DESCRIPTION: Header definitions to implement identity check for
*			   WinApp library classes using idCheck()
*********************************************************************/

#error 0

#ifndef WAPPDBG_H
#define WAPPDBG_H

#ifdef ISDEBUG

// CLASSES
// #define	ClassName		_D_Class_Name
//

#define AttributeZones		_D_AttributeZones
#define Desktop				_D_Desktop
#define Log 				_D_Log
#define LogItem 			_D_LogItem
#define MenuItem			_D_MenuItem
#define StrHistory			_D_StrHistory
#define Text		 		_D_Text
#define TextCursor			_D_TextCursor
#define WinApp				_D_WinApp
#define WinCmd				_D_WinCmd
#define WinObj				_D_WinObj
#define WinObjAct			_D_WinObjAct
#define Window				_D_Window

#define ATTRIBUTEZONES_ID	0x1762a112
#define DESKTOP_ID			0x1872a4ba
#define LOGITEM_ID			0x2a2b3c4d
#define LOG_ID				0xa6321544
#define MENUITEM_ID			0xE872FE82
#define STRHISTORY_ID		0x1AC123AB
#define TEXTCURSOR_ID		0xa12ff12d
#define TEXT_ID 			0xa876122d
#define WINAPP_ID			0x29826134
#define WINCMD_ID			0xabd34134
#define WINDOW_ID			0x2187dab4
#define WINOBJ_ID			0xBC3120B6

#endif	// ISDEBUG
#endif	// WAPPDBG_H

