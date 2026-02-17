/********************************************************************
* Copyright (c) 1991 InterSoft Co.  All Rights Reserved
* THIS IS UNPUBLISHED PROPRIETARY SOURCE CODE OF InterSoft Co.
* The copyright notice above does not evidence any
* actual or intended publication of such source code.
*
* $Id: filecmd.h,v 1.1.1.1 1996/04/16 18:51:43 gustavof Exp $
*
* DESCRIPTION: Definitions for file commands
*********************************************************************/

#ifndef FILECMD_H
#define FILECMD_H

#ifdef __GNUC__
#pragma interface
#endif

#include <winapp/wincmd.h>
#include <winapp/cmdbind.h>

#define	FileNew_EVID(dummy)			COMPOSE(K_META, 'n')
#define	FileOpen_EVID(dummy)		K_ATTENTION
#define	FileLoad_EVID(dummy)		K_ATTENTION
#define	FileReload_EVID(dummy)		COMPOSE(K_META,K_ATTENTION)
#define	FileHardSave_EVID(dummy) 	COMPOSE(K_META, K_APL_HELP)
#define	FileSoftSave_EVID(dummy) 	K_APL_HELP
#define	FileSaveAs_EVID(dummy)		COMPOSE(K_META, 's')
#define	FileSaveAll_EVID(dummy)	 	COMPOSE(K_META, 'a')
#define	FileClose_EVID(dummy)		K_REMOVE
#define	FileCloseAll_EVID(dummy) 	COMPOSE(K_META, K_REMOVE)
#define	FileChgWorkDir_EVID(dummy)	COMPOSE(K_META, 'c')
#define	FileInsert_EVID(dummy)		COMPOSE(K_META, 'i')

void bindFileCmds(CmdBinding *cb);

BeginEmptyDesktopCmd(FileNew)
EndEmptyDesktopCmd(FileNew);

BeginEmptyDesktopCmd(FileOpen)
	UseHistory(fileHst);
	static String filter;
public:
	static void setFilter(const String &f) { filter = f; }
EndEmptyDesktopCmd(FileOpen);

BeginNoEmptyDesktopCmd(FileLoad)
	UseHistory(fileHst);
	static String filter;
public:
	static void setFilter(const String &f) { filter = f; }
EndNoEmptyDesktopCmd(FileLoad);

BeginNoEmptyDesktopCmd(FileReload)
EndNoEmptyDesktopCmd(FileReload);

BeginNoEmptyDesktopCmd(FileHardSave)
EndNoEmptyDesktopCmd(FileHardSave);

BeginNoEmptyDesktopCmd(FileSoftSave)
EndNoEmptyDesktopCmd(FileSoftSave);

BeginNoEmptyDesktopCmd(FileSaveAs)
	UseHistory(fileHst);
	static String filter;
public:
	static void setFilter(const String &f) { filter = f; }
EndNoEmptyDesktopCmd(FileSaveAs);

BeginNoEmptyDesktopCmd(FileInsert)
	UseHistory(fileHst);
	String fileName;
	Int nLines;
	static String filter;
public:
	void clear();
	static void setFilter(const String &f) { filter = f; }
EndNoEmptyDesktopCmd(FileInsert);

BeginNoEmptyDesktopCmd(FileSaveAll)
EndNoEmptyDesktopCmd(FileSaveAll);

BeginNoEmptyDesktopCmd(FileClose)
EndNoEmptyDesktopCmd(FileClose);

BeginNoEmptyDesktopCmd(FileCloseAll)
EndNoEmptyDesktopCmd(FileCloseAll);

BeginEmptyDesktopCmd(FileChgWorkDir)
	String dir;
public:
	void clear();
EndEmptyDesktopCmd(FileChgWorkDir);

#endif
