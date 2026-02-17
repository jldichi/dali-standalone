/********************************************************************
* Copyright (c) 1991 InterSoft Co.  All Rights Reserved
* THIS IS UNPUBLISHED PROPRIETARY SOURCE CODE OF InterSoft Co.
* The copyright notice above does not evidence any
* actual or intended publication of such source code.
*
* $Id: dsenbind.cc,v 1.2 1997/10/08 19:39:34 eduardoc Exp $
*
* DESCRIPTION: Implementation of sensor class
*********************************************************************/

#include <winapp/menu.h>
#include <ifound/str.h>
#include <dsensor.h>
#include <command.h>
#include <commdef.h>
#include <bascomm.h>
#include <poscomm.h>
#include <drawcomm.h>
#include <filecomm.h>
#include <grepcomm.h>
#include <schcomm.h>
#include <blkcomm.h>
#include <syscomm.h>
#include <debcomm.h>
#include <editor.h>

#define MSG(a)		Editor::editorMsg(a)

void DaliSensor::initCommands()
{
	UnShort i;
	if (!cmds)
		cmds = tr_new _cmds[MAX_COMMAND];

	for (i = 0; i < MAX_COMMAND; i++)
		if (isprint(i))
			bind(i, InsChar::cmdId);

	// **********************************
	// bind de teclas para el modo insert
	// **********************************
    
	// teclas especiales
	bind(K_CTRLX, ALTER_KEY);
	bind(K_META,  ALTER_KEY);

	// teclas del menu
	bind(COMPOSE(K_CTRLX,' '),								MENU_KEY);
	bind(COMPOSE(K_CTRLX,MSG("MNK_EDIT")[0]),				MENU_KEY);
	bind(COMPOSE(K_CTRLX,MSG("MNK_SEARCH")[0]), 			MENU_KEY);
	bind(COMPOSE(K_CTRLX,MSG("MNK_WINDOW")[0]), 			MENU_KEY);
	bind(COMPOSE(K_CTRLX,MSG("MNK_HELP")[0]),				MENU_KEY);

	bind(COMPOSE(K_CTRLX,_tolower(MSG("MNK_EDIT")[0])),		MENU_KEY);
	bind(COMPOSE(K_CTRLX,_tolower(MSG("MNK_SEARCH")[0])),	MENU_KEY);
	bind(COMPOSE(K_CTRLX,_tolower(MSG("MNK_WINDOW")[0])),	MENU_KEY);
	bind(COMPOSE(K_CTRLX,_tolower(MSG("MNK_HELP")[0])),		MENU_KEY);

	if( Editor::userMode != "beginner" ) {
		bind(COMPOSE(K_CTRLX,MSG("MNK_FILE")[0]),				MENU_KEY);
		bind(COMPOSE(K_CTRLX,_tolower(MSG("MNK_FILE")[0])),		MENU_KEY);
	}
	if( Editor::userMode == "expert" ) {
		bind(COMPOSE(K_CTRLX,MSG("MNK_OPTIONS")[0]),			MENU_KEY);
		bind(COMPOSE(K_CTRLX,_tolower(MSG("MNK_TOOL")[0])),		MENU_KEY);
	 	bind(COMPOSE(K_CTRLX,_tolower(MSG("MNK_OPTIONS")[0])),	MENU_KEY);
    }


	// comandos varios
	bind(K_DEL							,DelChar::cmdId);
	bind(K_TAB							,Tab::cmdId);
	bind(K_BACKSPACE					,BackSpace::cmdId);
	bind(K_ENTER						,Enter::cmdId);
	bind(K_INS							,DaliChgInsMode::cmdId);
	bind(K_END							,EndBlockMode::cmdId);
	bind(COMPOSE(K_META, 'b')      		,BraceBlock::cmdId);
	bind(COMPOSE(K_META, 'o')      		,AddNewLine::cmdId);
	bind(COMPOSE(K_META, 'O')      		,InsNewLine::cmdId);

	bind(COMPOSE(K_META, K_BACKSPACE)	,BackTab::cmdId);
	bind(COMPOSE(K_META, 'w')     		,DelWord::cmdId);
	bind(COMPOSE(K_META, 'd')        	,DelLine::cmdId);
	bind(COMPOSE(K_META, 'e')        	,DelEndOfLine::cmdId);
	bind(COMPOSE(K_CTRLX, 'j') 			,JoinLines::cmdId);
	bind(K_PAGE_LEFT   					,OuterBackMatch::cmdId);
	bind(K_PAGE_RIGHT  					,OuterForwMatch::cmdId);

	bind(COMPOSE(K_META, '[')			,BackFuncBegin::cmdId);
	bind(COMPOSE(K_META, ']')			,ForwFuncBegin::cmdId);

	bind(COMPOSE(K_META, 'X')        	,MarkPosition::cmdId);
	bind(COMPOSE(K_META, 'x')        	,GoMarkPosition::cmdId);
	bind(COMPOSE(K_META, 'g')        	,GotoFile::cmdId);
	bind(COMPOSE(K_META, 'y')        	,GotoFile::cmdId);
	

	// movimientos de cursor
	bind(K_CURS_DOWN					,CursDown::cmdId);
	bind(K_CURS_UP						,CursUp::cmdId);
	bind(K_CURS_LEFT					,CursLeft::cmdId);
	bind(K_CURS_RIGHT,					CursRight::cmdId);
	bind(K_PAGE_DOWN,					PageDown::cmdId);
	bind(K_PAGE_UP,						PageUp::cmdId);

	bind(COMPOSE(K_META, K_CURS_LEFT),	BegOfLine::cmdId);
	bind(COMPOSE(K_META, K_CURS_RIGHT),	EndOfLine::cmdId);
	bind(COMPOSE(K_META, K_PAGE_UP),	BegOfFile::cmdId);
	bind(COMPOSE(K_META, K_PAGE_DOWN),	EndOfFile::cmdId);
	bind(COMPOSE(K_META, K_CURS_UP),	BegOfScreen::cmdId);
	bind(COMPOSE(K_META, 'm'),			MiddleOfScreen::cmdId);
	bind(COMPOSE(K_META, K_CURS_DOWN),	EndOfScreen::cmdId);
	bind(COMPOSE(K_CTRLX, '6'),			WordRight::cmdId);
	bind(COMPOSE(K_CTRLX, '4'),			WordLeft::cmdId);
	bind(COMPOSE(K_CTRLX, K_CURS_RIGHT),WordRight::cmdId);
	bind(COMPOSE(K_CTRLX, K_CURS_LEFT),	WordLeft::cmdId);
	bind(COMPOSE(K_META, '.'),			GoFirstNonSp::cmdId);
	bind(COMPOSE(K_CTRLX, '/'),			FunctionBrowser::cmdId);
	bind(COMPOSE(K_CTRLX, '.'),			TokenBrowser::cmdId);
	bind(COMPOSE(K_CTRLX, '>'),			TokenBrowser::cmdId);
                           
	// Menu de System
    if( Editor::userMode == "expert" ) {
		bind(K_SUSPEND,						ExecOSCommand::cmdId);
		bind(COMPOSE(K_META,K_SUSPEND),		ExecWMCommand::cmdId);
		bind(COMPOSE(K_META, 'z'),			GoToShell::cmdId);
	}
	bind(COMPOSE(K_META, 'R'),			RepaintDesktop::cmdId);

	// Menu de File
	if ( Editor::userMode == "expert" ||
	   	 Editor::userMode == "normal" )  {
		bind(K_ATTENTION,					OpenFile::cmdId);
		bind(COMPOSE(K_META,K_ATTENTION),	ReOpenFile::cmdId);
		bind(K_APL_HELP,					SoftSave::cmdId);
		bind(COMPOSE(K_META, K_APL_HELP),	HardSave::cmdId);
		bind(COMPOSE(K_META, 's'),			SaveAsFile::cmdId);
		bind(COMPOSE(K_META, 'i'),			InsertFile::cmdId);

		if( Editor::userMode == "expert" ) {
			bind(COMPOSE(K_META, 'c'),			ChangeDir::cmdId);
			bind(COMPOSE(K_META, 'T'),			OpenTag::cmdId);
			bind(COMPOSE(K_META, 't'),			FindTag::cmdId);

			// Menu Tools
			bind(K_PROCESS,						Build::cmdId);
			bind(COMPOSE(K_META,K_PROCESS),		Generate::cmdId);
		}
    }

	// salida del editor
	bind(COMPOSE(K_CTRLX, 'x'),			END_KEY);
	bind(COMPOSE(K_CTRLX, 'X'),			END_KEY);

	// Menu de Edit
	bind(K_UNDO,						CommHis::cmdId);
	bind(COMPOSE(K_META,K_META),		BeginBlock::cmdId);
	bind(COMPOSE(K_META,'!'),			ExecOSFilter::cmdId);
	bind(COMPOSE(K_META, K_INS),		CopyBlock::cmdId);
	bind(COMPOSE(K_META, K_ENTER),		PasteBlock::cmdId);
	bind(COMPOSE(K_META, K_DEL),		CutBlock::cmdId);
	bind(COMPOSE(K_META, ' '),			ClearBlock::cmdId);
	bind(COMPOSE(K_META, 'D'),			DeleteBlock::cmdId);
	bind(COMPOSE(K_META, '+'),			SetAttribute::cmdId);
	bind(COMPOSE(K_META, '-'),			DelAttribute::cmdId);
	bind(COMPOSE(K_CTRLX, '['),			ShiftLeft::cmdId);
	bind(COMPOSE(K_CTRLX, ']'),			ShiftRight::cmdId);

	// Menu de Search
	bind(COMPOSE(K_META, 'f'),			FindText::cmdId);
	bind(COMPOSE(K_META, 'a'),			FindAgain::cmdId);
	bind(COMPOSE(K_META, 'p'),			FindPrev::cmdId);
	bind(COMPOSE(K_META, 'n'),			FindNext::cmdId);
	bind(COMPOSE(K_CTRLX, 'i'),			IncFind::cmdId);
	bind(COMPOSE(K_META, 'G'),			GrepFiles::cmdId);
	bind(COMPOSE(K_META, 'r'),			ReplaceText::cmdId);
	bind(COMPOSE(K_CTRLX, K_TAB), 		TokenCompletion::cmdId);
	bind(COMPOSE(K_CTRLX, 'r'),			ReplaceAgain::cmdId);
	bind(COMPOSE(K_CTRLX, 'g'),			GoToLine::cmdId);


	if( Editor::userMode == "expert" ) {
		// Menu de Options
		bind(COMPOSE(K_CTRLX, 'G'),			SetDrawMode::cmdId);
		bind(COMPOSE(K_META,  'F'),			SetTroffMode::cmdId);
		bind(COMPOSE(K_META,  'E'),			SetTeXMode::cmdId);
		bind(COMPOSE(K_CTRLX, 'C'),			CommMode::cmdId);
		bind(COMPOSE(K_CTRLX, '#'),			InsAsciiChar::cmdId);
		bind(COMPOSE(K_META,  'l'),			DaliChgCtrolMode::cmdId);
		bind(COMPOSE(K_CTRLX, 'd'),			DbgStart::cmdId);
		bind(COMPOSE(K_CTRLX, 'D'),			DbgStop::cmdId);
    }

	// Menu de Window
	bind(COMPOSE(K_CTRLX, 'M'),			MoveFileWin::cmdId);
	bind(COMPOSE(K_CTRLX, 'Z'),			ResizeFileWin::cmdId);
	bind(COMPOSE(K_CTRLX, 'n'),			NextFile::cmdId);
	bind(COMPOSE(K_CTRLX, 'p'),			PrevFile::cmdId);
	bind(K_REMOVE,						CloseFile::cmdId);
	bind(COMPOSE(K_META,K_REMOVE),		CloseAll::cmdId);
	bind(COMPOSE(K_CTRLX, 'l'),			SelectFile::cmdId);
	bind(COMPOSE(K_CTRLX, 'm'),			TileWindows::cmdId);
	bind(COMPOSE(K_CTRLX, 'c'),			CascadeWindows::cmdId);
	bind(K_IGNORE,						ZoomFile::cmdId);

#ifdef USES_OLD_HELP_MENU_STYLE
	// Menu de Help
	bind(K_HELP,						HelpIndex::cmdId);
	bind(COMPOSE(K_META,  K_HELP),		FindHelp::cmdId);
	bind(COMPOSE(K_META,  'H'),			OpenHelp::cmdId);
#endif

	// ********************************
	// bind de teclas para el modo draw
	// ********************************

	bind(COMPOSE(DRAW_MODE,' '),			SetDrawMode::cmdId);
	bind(COMPOSE(DRAW_MODE,'0'),			SetDrawMode::cmdId);
	bind(COMPOSE(DRAW_MODE,'1'),			SetDrawMode::cmdId);
	bind(COMPOSE(DRAW_MODE,'2'),			SetDrawMode::cmdId);
	bind(COMPOSE(DRAW_MODE,'3'),			SetDrawMode::cmdId);
	bind(COMPOSE(DRAW_MODE,'4'),			SetDrawMode::cmdId);
	bind(COMPOSE(DRAW_MODE,K_CURS_RIGHT),	DrawCursRight::cmdId);
	bind(COMPOSE(DRAW_MODE,K_CURS_LEFT),	DrawCursLeft::cmdId);
	bind(COMPOSE(DRAW_MODE,K_CURS_UP),		DrawCursUp::cmdId);
	bind(COMPOSE(DRAW_MODE,K_CURS_DOWN),	DrawCursDown::cmdId);
	bind(COMPOSE(DRAW_MODE,K_UNDO),			CommHis::cmdId);
	bind(COMPOSE(DRAW_MODE,K_END),			EndDrawMode::cmdId);

	// *********************************
	// bind de teclas para el modo debug
	// *********************************

	// teclas especiales
	bind(COMPOSE(DEBUG_MODE, K_CTRLX),		ALTER_KEY);
	bind(COMPOSE(DEBUG_MODE, K_META),		ALTER_KEY);

	bind(COMPOSE(DEBUG_MODE, 'n'),		DbgNext::cmdId);
	bind(COMPOSE(DEBUG_MODE, 'N'),		DbgNextI::cmdId);
	bind(COMPOSE(DEBUG_MODE, 's'),		DbgStep::cmdId);
	bind(COMPOSE(DEBUG_MODE, 'S'),		DbgStepI::cmdId);
	bind(COMPOSE(DEBUG_MODE, 'r'),		DbgRun::cmdId);
	bind(COMPOSE(DEBUG_MODE, 'c'),		DbgContinue::cmdId);
	bind(COMPOSE(DEBUG_MODE, 'P'),		DbgPrintThis::cmdId);
	bind(COMPOSE(DEBUG_MODE, 'p'),		DbgPrint::cmdId);
	bind(COMPOSE(DEBUG_MODE, 'w'),		DbgWatch::cmdId);
	bind(COMPOSE(DEBUG_MODE, 'd'),		DbgDisplay::cmdId);
	bind(COMPOSE(DEBUG_MODE, 'v'),		DbgPrintVar::cmdId);
	bind(COMPOSE(DEBUG_MODE, 'V'),		DbgPrintVar::cmdId);
	bind(COMPOSE(DEBUG_MODE, 'y'),		DbgTypeVar::cmdId);
	bind(COMPOSE(DEBUG_MODE, 't'),		DbgStack::cmdId);
	bind(COMPOSE(DEBUG_MODE, 'g'),		DbgRegs::cmdId);
	bind(COMPOSE(DEBUG_MODE, 'D'),		DbgDisplays::cmdId);
	bind(COMPOSE(DEBUG_MODE, 'B'),		DbgWatchBreaks::cmdId);
	bind(COMPOSE(DEBUG_MODE, 'W'),		DbgWatchBreaks::cmdId);

	bind(COMPOSE(DEBUG_MODE, K_INS),	DbgInsBreak::cmdId);
	bind(COMPOSE(DEBUG_MODE, K_DEL),	DbgDelBreak::cmdId);

#ifdef USES_OLD_HELP_MENU_STYLE
	bind(COMPOSE(DEBUG_MODE, K_HELP),		HelpIndex::cmdId); 		// F1
#endif

	bind(COMPOSE(DEBUG_MODE, K_APL_HELP),	SoftSave::cmdId); 		// F2
	bind(COMPOSE(DEBUG_MODE, K_ATTENTION),	OpenFile::cmdId); 		// F3
	bind(COMPOSE(DEBUG_MODE, K_SUSPEND),	ExecOSCommand::cmdId); 	// F4
	bind(COMPOSE(DEBUG_MODE, K_PROCESS),	Build::cmdId);  		// F5
	bind(COMPOSE(DEBUG_MODE, K_REMOVE),		CloseFile::cmdId); 		// F6
	bind(COMPOSE(DEBUG_MODE, K_UNDO),		CommHis::cmdId);  		// F7
	bind(COMPOSE(DEBUG_MODE, K_IGNORE),	 	ZoomFile::cmdId); 		// F8
	
	bind(COMPOSE(DEBUG_MODE, ' '),			DbgCmdBox::cmdId);

	bind(COMPOSE(DEBUG_MODE, K_CURS_DOWN)	,CursDown::cmdId);
	bind(COMPOSE(DEBUG_MODE, K_CURS_UP)		,CursUp::cmdId);
	bind(COMPOSE(DEBUG_MODE, K_PAGE_DOWN)	,PageDown::cmdId);
	bind(COMPOSE(DEBUG_MODE, K_CURS_LEFT)	,CursLeft::cmdId);
	bind(COMPOSE(DEBUG_MODE, K_CURS_RIGHT)	,CursRight::cmdId);
	bind(COMPOSE(DEBUG_MODE, K_PAGE_UP)		,PageUp::cmdId);
	bind(COMPOSE(DEBUG_MODE, K_END)			,EndBlockMode::cmdId);

	bind(COMPOSE(DEBUG_MODE,K_PAGE_LEFT)   	,OuterBackMatch::cmdId);
	bind(COMPOSE(DEBUG_MODE,K_PAGE_RIGHT)  	,OuterForwMatch::cmdId);

	initMenuKeys();
}

void DaliSensor::setSysKeys(const String &hkey, const String &skey)
{
	sm->setSecText(skey,			ExecOSCommand::cmdId);
	sm->setSecText(hkey+" "+skey,	ExecWMCommand::cmdId);
	sm->setSecText(hkey+" z",		GoToShell::cmdId);
	sm->setSecText(hkey+" R",		RepaintDesktop::cmdId);
}

void DaliSensor::setFileKeys(const String &attkey, const String &ahlpkey, 
						 const String &hkey, const String &xkey)
{
	mf->setSecText(attkey,			OpenFile::cmdId);
	mf->setSecText(hkey+" "+attkey,	ReOpenFile::cmdId);
	mf->setSecText(ahlpkey,			SoftSave::cmdId);

	mf->setSecText(hkey+" s",		SaveAsFile::cmdId);
	mf->setSecText(hkey+" i",		InsertFile::cmdId);
	mf->setSecText(hkey+" c",		ChangeDir::cmdId);
	mf->setSecText(hkey+" T",		OpenTag::cmdId);
	mf->setSecText(hkey+" t",		FindTag::cmdId);
	mf->setSecText(hkey+" g", 		GotoFile::cmdId);
	mf->setSecText(xkey+" x",		END_KEY);
}

void DaliSensor::setToolKeys(const String &pkey, const String &hkey)
{
	mt->setSecText(pkey, Build::cmdId);
	mt->setSecText(hkey+" "+pkey, Generate::cmdId);
}

void DaliSensor::setRunDebuggerKeys(const String &xkey, const String &inskey, 
					 		 	const String &delkey)
{
	mrd->setSecText(xkey+" d", 		DbgStart::cmdId);
	mrd->setSecText(xkey+" D", 		DbgStop::cmdId);
	mrd->setSecText("r",			DbgRun::cmdId);
	mrd->setSecText("c",			DbgContinue::cmdId);
	mrd->setSecText("n",			DbgNext::cmdId);
	mrd->setSecText("N",			DbgNextI::cmdId);
	mrd->setSecText("s",			DbgStep::cmdId);
	mrd->setSecText("S",			DbgStepI::cmdId);
	mrd->setSecText(inskey,			DbgInsBreak::cmdId);
	mrd->setSecText(delkey,			DbgDelBreak::cmdId);
	mrd->setSecText(MSG("SPACE"),	DbgCmdBox::cmdId);
}

void DaliSensor::setDspDebuggerKeys()
{
	mdd->setSecText("P",			DbgPrintThis::cmdId);
	mdd->setSecText("p",			DbgPrint::cmdId);
	mdd->setSecText("w",			DbgWatch::cmdId);
	mdd->setSecText("d",			DbgDisplay::cmdId);
	mdd->setSecText("v",			DbgPrintVar::cmdId);
	mdd->setSecText("y",			DbgTypeVar::cmdId);
	mdd->setSecText(" W/B",			DbgWatchBreaks::cmdId);
	mdd->setSecText("t",			DbgStack::cmdId);
	mdd->setSecText("g",			DbgRegs::cmdId);
	mdd->setSecText("D",			DbgDisplays::cmdId);
}

void DaliSensor::setEditKeys(const String &ukey, const String &hkey, 
						 const String &enterkey, const String &inskey, 
						 const String &delkey, const String &xkey)
{
	me->setSecText(ukey,			CommHis::cmdId);
	me->setSecText(hkey+hkey,		BeginBlock::cmdId);
	me->setSecText(hkey+" !",		ExecOSFilter::cmdId);
	me->setSecText(hkey+inskey,		CopyBlock::cmdId);
	me->setSecText(hkey+enterkey,	PasteBlock::cmdId);
	me->setSecText(hkey+delkey,		CutBlock::cmdId);
	me->setSecText(hkey+" ' '",		ClearBlock::cmdId);
	me->setSecText(hkey+" D",		DeleteBlock::cmdId);
	me->setSecText(hkey+" +",		SetAttribute::cmdId);
	me->setSecText(hkey+" -",		DelAttribute::cmdId);
	me->setSecText(xkey+" [",		ShiftLeft::cmdId);
	me->setSecText(xkey+" ]",		ShiftRight::cmdId);
}

void DaliSensor::setSearchKeys(const String &hkey, const String &xkey)
{
//	ms->setSecText(hkey+" f", FindText::cmdId);
	ms->setSecText(hkey+" n", FindAgain::cmdId);
	ms->setSecText(xkey+" i", IncFind::cmdId);
	ms->setSecText(hkey+" r", ReplaceText::cmdId);
	ms->setSecText(xkey+" r", ReplaceAgain::cmdId);
	ms->setSecText(hkey+" G", GrepFiles::cmdId);
	ms->setSecText(xkey+" g", GoToLine::cmdId);
	ms->setSecText(xkey+" /", FunctionBrowser::cmdId);
	ms->setSecText(xkey+" . "+xkey+" >", TokenBrowser::cmdId);
}

void DaliSensor::setOptionsKeys(const String &hkey, const String &xkey)
{
	mo->setSecText(xkey+" G", SetDrawMode::cmdId);
	mo->setSecText(hkey+" F", SetTroffMode::cmdId);
	mo->setSecText(hkey+" E", SetTeXMode::cmdId);
	mo->setSecText(xkey+" C", CommMode::cmdId);
	mo->setSecText(xkey+" #", InsAsciiChar::cmdId);
	mo->setSecText(hkey+" l", DaliChgCtrolMode::cmdId);
}

void DaliSensor::setWindowKeys(	const String &xkey, 	const String &ignkey, 
							const String &rmvkey,	const String &hkey)
{
	mw->setSecText(xkey+" Z",		ResizeFileWin::cmdId);
	mw->setSecText(xkey+" M",		MoveFileWin::cmdId);
	mw->setSecText(xkey+" n",		NextFile::cmdId);
	mw->setSecText(xkey+" p",		PrevFile::cmdId);
	mw->setSecText(rmvkey,			CloseFile::cmdId);
	mw->setSecText(hkey+" "+rmvkey,	CloseAll::cmdId);
	mw->setSecText(xkey+" l",		SelectFile::cmdId);
	mw->setSecText(ignkey, 			ZoomFile::cmdId);
	mw->setSecText(xkey+" m",		TileWindows::cmdId);
	mw->setSecText(xkey+" c",		CascadeWindows::cmdId);
}

#ifdef USES_OLD_HELP_MENU_STYLE
void DaliSensor::setHelpKeys(const String &hlpkey, const String &xkey, 
						 const String &hkey)
{
	mh->setSecText(hlpkey,		HelpIndex::cmdId);
	mh->setSecText(hkey+hlpkey,	FindHelp::cmdId);
	mh->setSecText(hkey+" H",	OpenHelp::cmdId);
}
#endif
