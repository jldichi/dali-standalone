/********************************************************************
* Copyright (c) 1991 InterSoft Co.  All Rights Reserved
* THIS IS UNPUBLISHED PROPRIETARY SOURCE CODE OF InterSoft Co.
* The copyright notice above does not evidence any
* actual or intended publication of such source code.
*
* $Id: dsencmd.cc,v 1.2 1997/10/08 19:39:36 eduardoc Exp $
*
* DESCRIPTION: Implementation of sensor class
*********************************************************************/

#include <dsensor.h>
#include <winapp/menu.h>
#include <command.h>
#include <commdef.h>
#include <bascomm.h>
#include <poscomm.h>
#include <drawcomm.h>
#include <grepcomm.h>
#include <filecomm.h>
#include <schcomm.h>
#include <blkcomm.h>
#include <syscomm.h>
#include <debcomm.h>
#include <editor.h>
#include <ifound/str.h>

#define MSG(a)		Editor::editorMsg(a)

void DaliSensor::loadSysMenu()
{
	*sm	<< MenuItem(MSG("MN_ABOUT"),		About::cmdId)
		<< NULL_STRING
		<< MenuItem(MSG("MN_CALCULATOR"),	Calculator::cmdId)
		<< MenuItem(MSG("MN_PRTSCRN"),		PrintScreen::cmdId)
		<< MenuItem(MSG("MN_SHOWCHARS"),	ShowCharSet::cmdId)
		<< MenuItem(MSG("MN_REPAINT"),		RepaintDesktop::cmdId);

	if ( Editor::userMode == "expert" ) {
		*sm << NULL_STRING
			<< MenuItem(MSG("MN_EXECOSCMD"),	ExecOSCommand::cmdId)
			<< MenuItem(MSG("MN_EXECWMCMD"),	ExecWMCommand::cmdId)
			<< MenuItem(MSG("MN_GOTOSHELL"),	GoToShell::cmdId);
	}
}

void DaliSensor::loadFileMenu()
{
	*mf << MenuItem(MSG("MN_NEW"),			NewFile::cmdId)
	   << MenuItem(MSG("MN_OPEN"),			OpenFile::cmdId)
	   << MenuItem(MSG("MN_REOPEN"),	 	ReOpenFile::cmdId)
	   << MenuItem(MSG("MN_SAVE"),			SoftSave::cmdId)
	   << MenuItem(MSG("MN_SAVEAS"),		SaveAsFile::cmdId)
	   << MenuItem(MSG("MN_SAVEALL"),		SaveAll::cmdId)
	   << MenuItem(MSG("MN_INSFILE"),		InsertFile::cmdId);

	if ( Editor::userMode == "expert" ) {
	   *mf << NULL_STRING
		   << MenuItem(MSG("MN_CHDIR"),			ChangeDir::cmdId)
		   << MenuItem(MSG("MN_OPENTAG"),		OpenTag::cmdId)
		   << MenuItem(MSG("MN_FINDTAG"),		FindTag::cmdId)
		   << MenuItem(MSG("MN_OPENERROR"),		GotoFile::cmdId)
		   << NULL_STRING
		   << MenuItem(MSG("MN_OPENWSP"),		LoadWorkSpace::cmdId)
		   << MenuItem(MSG("MN_SAVEWSP"),		SaveWorkSpace::cmdId);
	}

	*mf << NULL_STRING
		<< MenuItem(MSG("MN_EXIT"),			(Int) END_KEY);
}

void DaliSensor::loadEditMenu()
{
	*me	
		<< MenuItem(MSG("MN_UNDO"),			CommHis::cmdId)
		<< NULL_STRING
		<< MenuItem(MSG("MN_BEGBLK"),		BeginBlock::cmdId)
		<< MenuItem(MSG("MN_EXECOSFILTER"),	ExecOSFilter::cmdId)
		<< MenuItem(MSG("MN_CUT"),			CutBlock::cmdId)
		<< MenuItem(MSG("MN_COPY"),			CopyBlock::cmdId)
		<< MenuItem(MSG("MN_PASTE"),		PasteBlock::cmdId)
		<< MenuItem(MSG("MN_CLEAR"),		ClearBlock::cmdId)
		<< MenuItem(MSG("MN_DELETE"),		DeleteBlock::cmdId)
		<< NULL_STRING
		<< MenuItem(MSG("MN_SETATTR"),		SetAttribute::cmdId)
		<< MenuItem(MSG("MN_DELATTR"),		DelAttribute::cmdId)
		<< MenuItem(MSG("MN_SHIFTLEFT"),	ShiftLeft::cmdId)
		<< MenuItem(MSG("MN_SHIFTRIGHT"),	ShiftRight::cmdId);
}

void DaliSensor::loadSearchMenu()
{
	*ms <<  MenuItem(MSG("MN_FIND"),		FindText::cmdId)
	   <<  MenuItem(MSG("MN_FINDAGAIN"),	FindAgain::cmdId)
	   <<  MenuItem(MSG("MN_INCFIND"),		IncFind::cmdId)
	   <<  MenuItem(MSG("MN_REPLACE"),		ReplaceText::cmdId)
	   <<  MenuItem(MSG("MN_REPLACEAGAIN"),	ReplaceAgain::cmdId)
	   <<  MenuItem(MSG("MN_GOTOLINE"),		GoToLine::cmdId)
	   <<  MenuItem(MSG("MN_FUNCBROWSER"),	FunctionBrowser::cmdId)
	   <<  MenuItem(MSG("MN_TOKENBROWSER"),	TokenBrowser::cmdId)
	   ;
}

void DaliSensor::loadOptionsMenu()
{
	*mo << MenuItem(MSG("MN_DRAW"),			SetDrawMode::cmdId)
		<< MenuItem(MSG("MN_TROFF"),		SetTroffMode::cmdId)
		<< MenuItem(MSG("MN_TEX"),			SetTeXMode::cmdId)
		<< MenuItem(MSG("MN_COMMODE"),		CommMode::cmdId)
		<< MenuItem(MSG("MN_CTRLCHARS"),	DaliChgCtrolMode::cmdId)
		<< MenuItem(MSG("MN_INSASCII"),		InsAsciiChar::cmdId);
}

void DaliSensor::loadWindowMenu()
{
	*mw <<  MenuItem(MSG("MN_SIZE"),		ResizeFileWin::cmdId)
	   <<  MenuItem(MSG("MN_MOVE"),			MoveFileWin::cmdId)
	   <<  MenuItem(MSG("MN_ZOOM"),			ZoomFile::cmdId)
	   <<  NULL_STRING
	   <<  MenuItem(MSG("MN_NEXT"),			NextFile::cmdId)
	   <<  MenuItem(MSG("MN_PREV"),			PrevFile::cmdId)
	   <<  MenuItem(MSG("MN_CLOSE"),		CloseFile::cmdId)
	   <<  MenuItem(MSG("MN_CLOSEALL"),		CloseAll::cmdId)
	   <<  MenuItem(MSG("MN_SELECT"),		SelectFile::cmdId)
	   <<  NULL_STRING
	   <<  MenuItem(MSG("MN_TILE"),			TileWindows::cmdId)
	   <<  MenuItem(MSG("MN_CASCADE"),		CascadeWindows::cmdId);
}

#ifdef USES_OLD_HELP_MENU_STYLE
void DaliSensor::loadHelpMenu()
{
	*mh <<  MenuItem(MSG("MN_HELPFORHELP"),	HelpHelp::cmdId)
	    <<  MenuItem(MSG("MN_HELPINDEX"),	HelpIndex::cmdId)
	    <<  MenuItem(MSG("MN_TUTORIAL"),	Tutorial::cmdId)
	    <<  MenuItem(MSG("MN_FINDHELP"),	FindHelp::cmdId)
	    <<  MenuItem(MSG("MN_OPENHELP"),	OpenHelp::cmdId);
}
#endif

void DaliSensor::loadToolMenu()
{
	// DALI-STANDALONE: Removed RCS/SCCS version control menu items
	*mt << MenuItem(MSG("MN_RUNDEBUGGER"),	*mrd)
		<< MenuItem(MSG("MN_DSPDEBUGGER"),	*mdd)
		<< NULL_STRING
		<< MenuItem(MSG("MN_SETPROJECT"),	SetProject::cmdId)
		<< MenuItem(MSG("MN_GENERATE"),		Generate::cmdId)
		<< MenuItem(MSG("MN_BUILD"),		Build::cmdId)
		<< NULL_STRING
		<< MenuItem(MSG("MN_GREPFILES"),	GrepFiles::cmdId)
		<< MenuItem(MSG("MN_FILEDIFF"),		FileDiff::cmdId)
		<< MenuItem(MSG("MN_CTAGS"),		CTags::cmdId)
		<< MenuItem(MSG("MN_PREFER"),		ReadDaliRc::cmdId)
		;
}

void DaliSensor::loadRunDebuggerMenu()
{
	*mrd << MenuItem(MSG("MN_DBGSTART"),	DbgStart::cmdId)
		 << MenuItem(MSG("MN_DBGSTOP"),		DbgStop::cmdId)
		 << NULL_STRING
		 << MenuItem(MSG("MN_DBGRUN"),		DbgRun::cmdId)
		 << MenuItem(MSG("MN_DBGCONTINUE"),	DbgContinue::cmdId)
		 << MenuItem(MSG("MN_DBGNEXT"),		DbgNext::cmdId)
		 << MenuItem(MSG("MN_DBGNEXTI"),	DbgNextI::cmdId)
		 << MenuItem(MSG("MN_DBGSTEP"),		DbgStep::cmdId)
		 << MenuItem(MSG("MN_DBGSTEPI"),	DbgStepI::cmdId)
		 << NULL_STRING
		 << MenuItem(MSG("MN_DBGINSBREAK"),	DbgInsBreak::cmdId)
		 << MenuItem(MSG("MN_DBGDELBREAK"),	DbgDelBreak::cmdId)
		 << MenuItem(MSG("MN_DBGCMDBOX"),	DbgCmdBox::cmdId);
}

void DaliSensor::loadDspDebuggerMenu()
{
	*mdd << MenuItem(MSG("MN_DBGPRINT"),		DbgPrint::cmdId)
		 << MenuItem(MSG("MN_DBGPRINTTHIS"),	DbgPrintThis::cmdId)
		 << MenuItem(MSG("MN_DBGPRINTVAR"),		DbgPrintVar::cmdId)
		 << MenuItem(MSG("MN_DBGTYPEVAR"),		DbgTypeVar::cmdId)
		 << MenuItem(MSG("MN_DBGWATCH"),		DbgWatch::cmdId)
		 << MenuItem(MSG("MN_DBGDISPLAY"),		DbgDisplay::cmdId)
		 << NULL_STRING
		 << MenuItem(MSG("MN_DBGSTACK"),		DbgStack::cmdId)
		 << MenuItem(MSG("MN_DBGREGS"),			DbgRegs::cmdId)
		 << MenuItem(MSG("MN_DBGWATCHBREAKS"),	DbgWatchBreaks::cmdId)
		 << MenuItem(MSG("MN_DBGDISPLAYS"),		DbgDisplays::cmdId);
}

void DaliSensor::loadMenu()
{

#ifdef USES_OLD_HELP_MENU_STYLE
	menu->setRight(true);
#endif

	*menu	<< MenuItem(" ",					*sm);

	if ( Editor::userMode == "expert" || Editor::userMode == "normal" ) {
		*menu	<< MenuItem(MSG("MN_FILE"),		*mf) ;
	}

	*menu	<< MenuItem(MSG("MN_EDIT"),			*me)
			<< MenuItem(MSG("MN_SEARCH"),		*ms);

	if ( Editor::userMode == "expert" ) {
		*menu	<< MenuItem(MSG("MN_OPTION"),		*mo)
				<< MenuItem(MSG("MN_TOOL"),			*mt);
	}

	*menu	<< MenuItem(MSG("MN_WINDOW"),		*mw)

#ifndef USES_OLD_HELP_MENU_STYLE
			;
#else						
			<< MenuItem(MSG("MN_HELP"),			*mh);
#endif
}

