/********************************************************************
* Copyright (c) 1991 InterSoft Co.  All Rights Reserved
* THIS IS UNPUBLISHED PROPRIETARY SOURCE CODE OF InterSoft Co.
* The copyright notice above does not evidence any
* actual or intended publication of such source code.
*
* $Id: filecomm.h,v 1.1.1.1 1996/04/16 18:52:53 gustavof Exp $
*
* DESCRIPTION: Definitions for filecomm.
*********************************************************************/

#ifndef FILECOMM_H
#define FILECOMM_H

#ifdef __GNUC__
#pragma interface
#endif

#ifdef ISDEBUG
#	include <ifound/iddebug.h>
#endif

// comandos para el manejo de archivos

class DaliView;

// DALI-STANDALONE: Removed RCS/SCCS version control commands
// (CheckOutFile, CheckInFile, VersionLog, OpenLastVersion,
//  DiffLastVersion, UndoCheckIn, UndoCheckOut)

BeginControlCommand(FileDiff)
EndControlCommand(FileDiff);

BeginControlCommandNoViews(OpenFile)
EndControlCommandNoViews(OpenFile);

BeginControlCommand(ReOpenFile)
EndControlCommand(ReOpenFile);

BeginControlCommand(SaveAll)
EndControlCommand(SaveAll);

BeginControlCommand(HardSave)
EndControlCommand(HardSave);

BeginControlCommand(SoftSave)
EndControlCommand(SoftSave);

BeginControlCommand(SaveAsFile)
EndControlCommand(SaveAsFile);

BeginControlCommandNoViews(NewFile)
EndControlCommandNoViews(NewFile);

BeginControlCommand(NextFile)
EndControlCommand(NextFile);

BeginControlCommand(PrevFile)
EndControlCommand(PrevFile);

BeginControlCommand(CloseFile)
EndControlCommand(CloseFile);

BeginControlCommand(CloseAll)
EndControlCommand(CloseAll);

BeginControlCommandNoViews(SelectFile)
EndControlCommandNoViews(SelectFile);

BeginControlCommandNoViews(SaveWorkSpace)
EndControlCommandNoViews(SaveWorkSpace);

BeginControlCommandNoViews(LoadWorkSpace)
EndControlCommandNoViews(LoadWorkSpace);

BeginControlCommand(ZoomFile)
EndControlCommand(ZoomFile);

BeginControlCommand(MoveFileWin)
EndControlCommand(MoveFileWin);

BeginControlCommand(ResizeFileWin)
EndControlCommand(ResizeFileWin);

BeginControlCommand(TileWindows)
EndControlCommand(TileWindows);

BeginControlCommand(CascadeWindows)
EndControlCommand(CascadeWindows);

BeginControlCommandNoViews(ChangeDir)
	String dir;
public:
	void clear();
EndControlCommandNoViews(ChangeDir);

BeginAnyCommand(InsertFile)
	String	fileName;
	Int		nLines;
public:
	void clear();
EndAnyCommand(InsertFile);

BeginControlCommandNoViews(SetProject)
	String	oldProject;
public:
	void clear();
EndControlCommandNoViews(SetProject);

BeginControlCommand(Build)
EndControlCommand(Build);

BeginControlCommand(Generate)
EndControlCommand(Generate);

BeginControlCommand(GotoFile)
EndControlCommand(GotoFile);

BeginControlCommandNoViews(CTags)
EndControlCommandNoViews(CTags);

BeginControlCommandNoViews(OpenTag)
EndControlCommandNoViews(OpenTag);

BeginControlCommand(FindTag)
EndControlCommand(FindTag);

BeginControlCommandNoViews(ReadDaliRc)
EndControlCommandNoViews(ReadDaliRc);

#endif
