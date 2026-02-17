/********************************************************************
* Copyright (c) 1991 InterSoft Co.	All Rights Reserved
* THIS IS UNPUBLISHED PROPRIETARY SOURCE CODE OF InterSoft Co.
* The copyright notice above does not evidence any
* actual or intended publication of such source code.
*
* $Id: filecmd.cc,v 1.4 2004/09/29 21:07:25 damianm Exp $
*
* DESCRIPTION: Implementation of file commands classes
*********************************************************************/
#ifdef __GNUC__
#pragma implementation
#endif

// para que genere la entrada dentro de la tabla de comandos
#define INITCOMM

#include <errno.h>
#include <unistd.h>

#include <ifound.h>
#include <local/osfile.h>
#include <local/strfile.h>
#include <winapp/strhis.h>

// WApp library
#include <winapp/cell.h>
#include <winapp/cmdbind.h>
#include <winapp/dirsel.h>
#include <winapp/filecmd.h>
#include <winapp/filesel.h>
#include <winapp/label.h>
#include <winapp/menu.h>
#include <winapp/mldbox.h>
#include <winapp/msgbox.h>
#include <winapp/text.h>
#include <winapp/view.h>
#include <winapp/winapp.h>
#include <winapp/waitmsg.h>
#include <winapp/wincmd.h>

// System library
#include <sys/types.h>
#include <sys/stat.h>

declare(PtrList,EditableView);
declare(PtrListCursor,EditableView);

StrHistory *FileOpen::fileHst 	= NULL;
StrHistory *FileLoad::fileHst 	= NULL;
StrHistory *FileSaveAs::fileHst = NULL;
StrHistory *FileInsert::fileHst = NULL;

String FileOpen::filter = "*";
String FileLoad::filter = "*";
String FileInsert::filter = "*";
String FileSaveAs::filter = "*";

// @ Static Methods

#define FILESEL_ROWS		17
#define FILESEL_COLS		42
#define FILESEL_ORG_ROW	3
#define FILESEL_ORG_COL	16

static bool _canSave(EditableView *v)
{
	if ( v->isTextType(READONLY | ISTREAM | ADMIN) ) {
		MsgBox errbox(libMsg("ERR"), "NOWRITEPERM",
						5,35,6,20);
		errbox.execute();
		return false;
	}

	return true;
}

static bool _hardSave(EditableView *v)
{
	v->store();
	v->writeStLine();

	return false;
}

static bool _softSave(EditableView *v)
	// Only save file if it is modified
{
	if ( v->isTextType(READONLY | ISTREAM | ADMIN) || !v->isTextMod() )
		return false;

	return _hardSave(v);
}

void bindFileCmds(CmdBinding *cb)
{
	// Cursor movement
	cb->bind(FileNew::eventId,			FileNew::cmdId);
	cb->bind(FileOpen::eventId,			FileOpen::cmdId);
	cb->bind(FileLoad::eventId,			FileLoad::cmdId);
	cb->bind(FileReload::eventId,		FileReload::cmdId);
	cb->bind(FileHardSave::eventId,		FileHardSave::cmdId);
	cb->bind(FileSoftSave::eventId,		FileSoftSave::cmdId);
	cb->bind(FileSaveAs::eventId,		FileSaveAs::cmdId);
	cb->bind(FileSaveAll::eventId,		FileSaveAll::cmdId);
	cb->bind(FileClose::eventId,  		FileClose::cmdId);
	cb->bind(FileChgWorkDir::eventId,  	FileChgWorkDir::cmdId);
	cb->bind(FileInsert::eventId,		FileInsert::cmdId);
}

// File Classes

void FileNew::undo()
{
	idCheck();
}

bool FileNew::redo()
{
	idCheck();
	appi->addView(tr_new EditableView("noname", false));
	return false;
}

void FileOpen::undo()
{
	idCheck();
}

bool FileOpen::redo()
{
	idCheck();
	FileSelector menu(WinApp::wappDispMsg("FileOpen"),
						FILESEL_ROWS, FILESEL_COLS,
						FILESEL_ORG_ROW, FILESEL_ORG_COL,
						appi->getWorkingDir(), filter, NULL_STRING, fileHst);

	menu.execute();

	if(menu.isAccepted() == true) {
		if (!(menu.perms() & READ_PERM)) {
			MsgBox msgbox(libMsg("ERR"), 
					WinApp::wappErrMsg("NOREADPERM"), 5, 35, 6, 20);
					msgbox.execute();
		}
		else {
			for (StrCursor c(menu.value(), " "); c; c++) {
				assert(String(c) != NULL_STRING);
				appi->openFile(String(c));
				if (fileHst) 
					*fileHst << String(c);
			}
		}
	}
	return false;
}

void FileLoad::undo()
{
	idCheck();
}

bool FileLoad::redo()
// toDo: Consider integrating openFile & loadFile in a unique command.
{
	idCheck();
	FileSelector menu(WinApp::wappDispMsg("FileLoad"),
						FILESEL_ROWS, FILESEL_COLS,
						FILESEL_ORG_ROW, FILESEL_ORG_COL,
						appi->getWorkingDir(), filter, NULL_STRING, fileHst);

	menu.execute();

	if(menu.isAccepted() == true) {
		if (!(menu.perms() & READ_PERM)) {
			MsgBox msgbox(libMsg("ERR"),
					WinApp::wappErrMsg("NOREADPERM"), 5, 35, 6, 20);
					msgbox.execute();
		}
		else {
			EditableView *view = appi->getDesktop()->getCurrEdView();
            view->clear();
			String file;
			for (StrCursor c(menu.value(), " "); c; c++) {
				file = c;
				assert(file != NULL_STRING);
				appi->loadFile(file);
				if (fileHst) 
					*fileHst << file;
			}
            view->chgLabel(file);
		}
	}
	return false;
}

bool FileReload::redo()
{
	idCheck();
	EditableView *view = appi->getDesktop()->getCurrEdView();
	
	// Es posible hacer un reOpen de la view activa ?
	if (view->isTextType(ISTREAM | ADMIN))
		return false;

	if (view->isTextMod()) {
		MLDBox dbox(libMsg("MESSAGE"), 
				view->getLabel() +"\n"+ 
				WinApp::wappDispMsg("EXITRDONLY4"), DBOX_YES);

		dbox.execute();
		if (dbox.getOpt() != DBOX_YES)
			return false;
	}

	// En que fila-columna estoy
	Int inrow = view->getRow();
	Int atcol = view->getCol();

	view->reload();
	view->goCursor(inrow,atcol,true);
	return false;
}

void FileReload::undo()
{
	idCheck();
}

void FileHardSave::undo()
{
	idCheck();
}

bool FileHardSave::redo()
{
	idCheck();
	EditableView *view = appi->getDesktop()->getCurrEdView();

	if (_canSave(view))
		return _hardSave(view);

	return false;
}

void FileSoftSave::undo()
{
	idCheck();
}

bool FileSoftSave::redo()
{
	idCheck();
	EditableView *view = appi->getDesktop()->getCurrEdView();

	if (_canSave(view))
		return _softSave(view);

	return false;
}

void FileSaveAs::undo()
{
	idCheck();
}

bool FileSaveAs::redo()
{
	idCheck();
	EditableView *view = appi->getDesktop()->getCurrEdView();

	if (view->isTextType(ISTREAM)) {
		appi->beep();
		return false;
	}

	FileSelector menu(WinApp::wappDispMsg("FileSaveAs"),
						FILESEL_ROWS, FILESEL_COLS,
						FILESEL_ORG_ROW, FILESEL_ORG_COL,
						appi->getWorkingDir(), filter, NULL_STRING, fileHst);

	menu.execute();
	if (menu.isAccepted() == true) {
#ifdef __VC__
		struct _stat stbuf;
#else
		struct stat stbuf;
#endif		
		String file = menu.value();
		if (fileHst)
			*fileHst << file;

		if (statFile(file, &stbuf) == 0) {
			MLDBox dbox(libMsg("MESSAGE"), WinApp::wappDispMsg("FILEEXISTS"), DBOX_YES);

			dbox.execute();
			if (dbox.getOpt() != DBOX_YES)
				return false;
		}
		else {
			if (errno != ENOENT) {
				MsgBox msgbox(libMsg("ERR"), 
						WinApp::wappErrMsg("COULDNTWRITE"), 5, 32, 6, 20);
						msgbox.execute();
				return false;
			}
		}

		Int mode = menu.perms();
		if (!(mode & WRITE_PERM)) {
			MsgBox msgbox(libMsg("ERR"), 
						WinApp::wappErrMsg("NOWRITEPERM"), 5, 35, 6, 20);
						msgbox.execute();
			return false;
		}

		if (view->chgFileName(file)) {
			view->store();
			view->writeStLine();
		}
		else {
			MsgBox msgbox(libMsg("ERR"), 
				  	WinApp::wappErrMsg("TEXTBEINGMOD"), 6, 40, 6, 20);
					msgbox.execute();
		}
	}

	return false;
}

void FileSaveAll::undo()
{
	idCheck();
}

bool FileSaveAll::redo()
{
	idCheck();    
	
	// ====>> Esto deberia hacerse con un metodo de la clase Desktop  <<====
	// return appi->getDesktop()->saveViews();
	
	return false;
}

void FileClose::undo()
{
	idCheck();
}

bool FileClose::redo()
{
	idCheck();
	
	appi->getDesktop()->delCurrent();
	return false;
}

bool FileCloseAll::redo()
{
	idCheck();
	appi->getDesktop()->clearDesktop();

	return false;
}

void FileCloseAll::undo()
{
	idCheck();
}

void FileChgWorkDir::clear()
{
	idCheck();
	dir = NULL_STRING;
}

void FileChgWorkDir::undo()
{
	idCheck();

	if (chdir(toCharPtr(dir)) != 0) {
		MsgBox msgbox(libMsg("ERR"), 
			WinApp::wappErrMsg("COULDNTCHDIR"), 5, 40, 6, 20);
			msgbox.execute();
	}
	dir = appi->getWorkingDir();
}

bool FileChgWorkDir::redo()
{
	idCheck();
	if (dir == NULL_STRING) {
		DirSelector menu(WinApp::wappDispMsg("FileChgWorkDir"), 15, 29, 6, 15, 
						appi->getWorkingDir());

		menu.execute();

		if (menu.isAccepted() == true)
			dir = menu.value();
		else return false;
	}

	if (chdir(toCharPtr(dir)) != 0) {
		MsgBox msgbox(libMsg("ERR"),
				WinApp::wappErrMsg("COULDNTCHDIR"), 5, 40, 6, 20);
				msgbox.execute();
		return false;
	}

	// ====>> Esto deberia hacerse con un metodo de la clase Desktop  <<====
					// Update all view`s name
//					appi->getDesktop()->updateViewNames();

	dir = appi->getWorkingDir();

	return true;
}

void FileInsert::clear()
{
	idCheck();
	fileName = NULL_STRING;
	nLines = 0;
}

void FileInsert::undo()
{
	idCheck();
	EditableView *view = appi->getDesktop()->getCurrEdView();
	if (nLines) {
		EditableTextCursor auxBeg = *(view->edCursor());
		EditableTextCursor auxEnd = auxBeg;
		auxBeg.goToCol(0);
		auxEnd += nLines;
		auxEnd.goToCol(0);
		auxEnd.moveCol(-1);
		auxBeg.delTo(auxEnd.order(), auxEnd.column());
		view->View::redraw(); // I know is not very nice.
	}
}

bool FileInsert::redo()
{
	idCheck();
	EditableView *view = appi->getDesktop()->getCurrEdView();

	if (fileName == NULL_STRING) {
		FileSelector menu(WinApp::wappDispMsg("FileInsert"),
						FILESEL_ROWS, FILESEL_COLS,
						FILESEL_ORG_ROW, FILESEL_ORG_COL,
						appi->getWorkingDir(), filter, NULL_STRING, fileHst);

		menu.execute();
		if(menu.isAccepted() != true)
			return false;

		fileName = menu.value();
		if (fileHst)
			*fileHst << fileName;
	}
	nLines = view->insertFile(fileName);
	if (!nLines) return false;
	return true;
}

