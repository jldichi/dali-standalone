/********************************************************************
* Copyright (c) 1991 InterSoft Co.  All Rights Reserved
* THIS IS UNPUBLISHED PROPRIETARY SOURCE CODE OF InterSoft Co.
* The copyright notice above does not evidence any
* actual or intended publication of such source code.
*
* $Id: syscomm.cc,v 1.2 1997/11/18 18:41:46 guiller Exp $
*
* DESCRIPTION: Implementation of help command clases
*********************************************************************/

#ifdef __GNUC__
#pragma implementation
#endif

// para que genere la entrada dentro de la tabla de comandos
#define INITCOMM

#include <ifound.h>
#include <winapp/msgbox.h>
#include <winapp/window.h>
#include <winapp/label.h>
#include <winapp/cell.h>
#include <winapp/strhis.h>

#include <editor.h>
#include <dview.h>
#include <command.h>
#include <syscomm.h>
#include <edlbox.h>


declare(PtrList, DaliView);
declare(PtrListCursor, DaliView);

#ifdef USES_OLD_HELP_MENU_STYLE
#include <edhelp.h>

void HelpHelp::undo() 
{ 	
	idCheck();
}

bool HelpHelp::redo()
{
	idCheck();
	Editor::editorHelp->display(Editor::editorMsg("HELPHELP"));
	return false;
}


void HelpIndex::undo()
{
	idCheck();
}

bool HelpIndex::redo()
{
	idCheck();
	Editor::editorHelp->display();
	return false;
}


void Tutorial::undo()
{
	idCheck();
}

bool Tutorial::redo()
{
	idCheck();
	Editor::editorHelp->display(Editor::editorMsg("TUTORIAL"));
	return false;
}
                  
// Open help class

#define OPENHELP_ROWS		3
#define OPENHELP_COLS		64
#define OPENHELP_ORG_ROW  	5
#define OPENHELP_ORG_COL  	8

void OpenHelp::undo()
{
	idCheck();
}

bool OpenHelp::redo()
{
	idCheck();
	String hlp;

	EdLineDialog dlog(Editor::helps, Editor::editorMsg("OpenHelp"), 
		Editor::editorMsg("HELPTOFIND"), OPENHELP_ROWS, 
		OPENHELP_COLS, OPENHELP_ORG_ROW, OPENHELP_ORG_COL);

	if (dlog.execute() != REJECT_EXIT)
		hlp = dlog.value();
	else
		return false;

    *Editor::helps << hlp;

	Help hlpFile("cfixlib");
	if (!hlpFile.display(hlp))
		Editor::showManualEntry(hlp);

	DaliView *cview = cView.value();
	
	if (cview) { 
	 	cview->refresh(); 
		cview->redraw();
	}

	return false;
}
// fin clase OpenHelp

// clase FindHelp

void FindHelp::undo() 
{
	idCheck();
}

bool FindHelp::redo()
{
	idCheck();
	String hlp;
	DaliView *cview = cView.value();

	if ((hlp = cview->getToken()) == NULL_STRING) {
		Editor::beep();
		return false;
	}

	Help hlpFile("cfixlib");
	if (!hlpFile.display(hlp)) {
		Editor::showManualEntry(hlp);
	}		
	 
 	cview->refresh(); 
	cview->redraw();

	return false;
}
#endif

void About::undo()
{
	idCheck();
}

bool About::redo()
{
	idCheck();
	MsgBox aboutBox(Editor::editorMsg("About"),
			String("Dali ") + versionStr(DALI_VERSION) +
			" Copyright InterSoft Co. (c)1991-93",
			7, 60, 5, 10);

	aboutBox.execute();
	return false;
}

void Calculator::undo() 
{
	idCheck();
}

bool Calculator::redo()
{
	idCheck();
	Window::execService(String("calculador"));
	return false;
}

void GoToShell::undo()
{
	idCheck();
}

bool GoToShell::redo()
{
	idCheck();
	Window::execService(String("GoToShell"));
	return false;
}

void ShowCharSet::undo() 
{
	idCheck();
}


bool ShowCharSet::redo()
{
	idCheck();
	Window::execService(String("_print_ichset"));
	return false;
}

void PrintScreen::undo()
{
	idCheck();
}

bool PrintScreen::redo()
{
	idCheck();
	Window::execService(String("_print_scr"));
	return false;
}

#define EXECOSCMD_ROWS		3
#define EXECOSCMD_COLS		60
#define EXECOSCMD_ORG_ROW	5
#define EXECOSCMD_ORG_COL	10

void ExecOSCommand::undo()
{
	idCheck();
}

bool ExecOSCommand::redo()
{
	idCheck();

	String cmd;
	EdLineDialog dlog(Editor::osCommands, 
		Editor::editorMsg("ExecOSCommand"), 
		Editor::editorMsg("SHCOMMAND"), EXECOSCMD_ROWS, 
		EXECOSCMD_COLS, EXECOSCMD_ORG_ROW, EXECOSCMD_ORG_COL);

	dlog.execute();

	cmd = dlog.value();
    *Editor::osCommands << cmd;

	if (dlog.isAccepted() != true)
		return false;

	Editor::execOSCommand(cmd);
	return false;
}

void ExecWMCommand::undo()
{
	idCheck();
}

bool ExecWMCommand::redo()
{
	idCheck();

	String cmd;
	EdLineDialog dlog(Editor::wmCommands, 
		Editor::editorMsg("ExecWMCommand"), 
		Editor::editorMsg("WMCOMMAND"), EXECOSCMD_ROWS, 
		EXECOSCMD_COLS, EXECOSCMD_ORG_ROW, EXECOSCMD_ORG_COL);

	dlog.execute();

	cmd = dlog.value();
    *Editor::wmCommands << cmd;

	if (dlog.isAccepted() != true)
		return false;

	Window::execCommand((char *)toCharPtr(cmd),(char *)toCharPtr(cmd));
	return false;
}


void RepaintDesktop::undo()
{
	idCheck();
}

bool RepaintDesktop::redo()
{
	idCheck();
	Window::redraw();
	return false;
}
// fin clase RepaintDesktop
                  

// fin clase FindHelp
