/********************************************************************
* Copyright (c) 1993 InterSoft Co.	All Rights Reserved
* THIS IS UNPUBLISHED PROPRIETARY SOURCE CODE OF InterSoft Co.
* The copyright notice above does not evidence any
* actual or intended publication of such source code.
*
* MODULE: $Id: libdebug.h,v 1.1.1.1 1996/04/16 18:51:34 gustavof Exp $
*
* DESCRIPTION: Header definitions to implement identity check for
*			   library classes using idCheck()
*********************************************************************/

#ifndef LIBDEBUG_H
#define LIBDEBUG_H

#ifdef ISDEBUG

// CLASSES
// #define	ClassName		_D_Class_Name
//


#define AttributeZones		_D_AttributeZones
#define DataBase			_D_DateBase
#define DataBaseImplem		_D_DataBaseImplem
#define DbCursor			_D_DbCursor
#define DbCursorImplem		_D_DbCursorImplem
#define DbIndex				_D_DbIndex
#define DbIndexImplem		_D_DbIndexImplem
#define DbIndexPart			_D_DbIndexPart
#define DbMemberType		_D_DbMemberType
#define DbObject			_D_DbObject
#define DbSet				_D_DbSet
#define DbSetImplem			_D_DbSetImplem
#define DbTypeDescr			_D_DbTypeDescr
#define Desktop				_D_Desktop
#define FmFieldRep			_D_FmFieldRep
#define FormRep				_D_FormRep
#define Log 				_D_Log
#define LogItem 			_D_LogItem
#define MenuItem			_D_MenuItem
#define RegExp				_D_RegExp
#define ReportRep			_D_ReportRep
#define Resource			_D_Resource
#define RpAccumulator		_D_RpAccumulator
#define RpFieldRep			_D_RpFieldRep
#define RpZoneObject		_D_RpZoneObject
#define RpZoneObject		_D_RpZoneObject
#define RpZoneRep			_D_RpZoneRep
#define StrHistory			_D_StrHistory
#define Text		 		_D_Text
#define TextCursor			_D_TextCursor
#define VirtualFile			_D_VirtualFile
#define WinApp				_D_WinApp
#define WinCmd				_D_WinCmd
#define WinObj				_D_WinObj
#define WinObjAct			_D_WinObjAct
#define Window				_D_Window
#define srep				_D_srep

// IDENTIFIERS
// #define ClassName	ClassId
//

#define BASEACTION_ID	  	0xac18172f
#define ATTRIBUTEZONES_ID	0x1762a112
#define DATABASEIMPLEM_ID	0x098fefff
#define DATABASE_ID			0x57575a2f
#define DBMCURSORIMPLEM_ID	0x1acb931e
#define DBMCURSOR_ID		0x49aec099
#define DBMINDEXIMPLEM_ID	0x00034fcc
#define DBMINDEXPART_ID		0x8ef0a5bb
#define DBMINDEX_ID			0x7a7e7f77
#define DBMMEMBERTYPE_ID	0x0900e4fa
#define DBMOBJECT_ID		0x34980afe
#define DBMSETIMPLEM_ID		0x98bf8a95
#define DBMSET_ID			0x3fff039b
#define DBMTYPEDESCR_ID		0xe90f7e02
#define DESKTOP_ID			0x1872a4ba
#define FMFIELDREP_ID		0x77123aec
#define FORMREP_ID			0x87f66e11
#define LOGITEM_ID			0x2a2b3c4d
#define LOG_ID				0xa6321544
#define MENUITEM_ID			0xE872FE82
#define REGEXP_ID			0x87a23d7c
#define REPORTREP_ID		0x87aef900
#define RESOURCE_ID			0x18723187
#define RPACCUMULATOR_ID	0xA987EF9A
#define RPFIELDREP_ID		0xa809f017
#define RPZONEOBJECT_ID		0xea7903fa
#define RPZONEREP_ID		0xefb0909a
#define SREP_ID				0x7a4b5c6d
#define STRHISTORY_ID		0x1AC123AB
#define TEXTCURSOR_ID		0xa12ff12d
#define TEXT_ID 			0xa876122d
#define VIRTUALFILE_ID		0x798aaaff
#define WINAPP_ID			0x29826134
#define WINCMD_ID			0xabd34134
#define WINDOW_ID			0x2187dab4
#define WINOBJ_ID			0xBC3120B6
#endif	// ISDEBUG
#endif	// LIBDEBUG_H



