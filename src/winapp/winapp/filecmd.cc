/********************************************************************
* Copyright (c) 1991 InterSoft Co.	All Rights Reserved
* THIS IS UNPUBLISHED PROPRIETARY SOURCE CODE OF InterSoft Co.
* The copyright notice above does not evidence any
* actual or intended publication of such source code.
*
* $Id: filecmd.cc,v 1.1.1.1 1996/04/16 18:52:48 gustavof Exp $
*
* DESCRIPTION: Implementation of file commands classes
*********************************************************************/
// para que genere la entrada dentro de la tabla de comandos
#define INITCOMM
#define NEWLINE "\\n"

// System library
#include <sys/types.h>
#include <sys/stat.h>


#include <errno.h>

// Ideafix4 library
#include <config.h>
#include <osfile.h>
#include <strfile.h>
#include <strhis.h>

// WApp library
#include <cell.h>
#include <cmdbind.h>
#include <dirsel.h>
#include <filecmd.h>
#include <filesel.h>
#include <label.h>
#include <menu.h>
#include <mldbox.h>
#include <msgbox.h>
#include <text.h>
#include <view.h>
#include <winapp.h>
#include <waitmsg.h>
#include <wincmd.h>

#define swap(a,b,tt)						\
	do {						 				\
		tt __tmp = a; a = b; b = __tmp;		\
 	} while (0)

declare(PtrList,EditableView);
declare(PtrListCursor,EditableView);

StrHistory *OpenFile::fileHst 	= NULL;
StrHistory *SaveAsFile::fileHst = NULL;
StrHistory *InsertFile::fileHst = NULL;
// @ Static Methods

#define FILESEL_ROWS		17
#define FILESEL_COLS		42
#define FILESEL_ORG_ROW	3
#define FILESEL_ORG_COL	16

static Bool _canSave(EditableView *v)
{
	if ( v->isTextType(READONLY | ISTREAM | ADMIN) ) {
		MsgBox errbox(libMsg("ERROR"), "NOWRITEPERM",
						5,35,6,20);
		errbox.execute();
		return FALSE;
	}

	return TRUE;
}

static Bool _hardSave(EditableView *v)
{
	v->store();
	v->writeStLine();

	return FALSE;
}

static Bool _softSave(EditableView *v)
	// Only save file if it is modified
{
	if ( v->isTextType(READONLY | ISTREAM | ADMIN) || !v->isTextMod() )
		return FALSE;

	return _hardSave(v);
}

void bindFileCmds(CmdBinding *cb)
{
	// Cursor movement
	cb->bind(NewFile::eventId,			NewFile::cmdId);
	cb->bind(OpenFile::eventId,			OpenFile::cmdId);
	cb->bind(ReOpenFile::eventId,		ReOpenFile::cmdId);
	cb->bind(HardSave::eventId,			HardSave::cmdId);
	cb->bind(SoftSave::eventId,			SoftSave::cmdId);
	cb->bind(SaveAsFile::eventId,		SaveAsFile::cmdId);
	cb->bind(SaveAll::eventId,			SaveAll::cmdId);
	cb->bind(CloseFile::eventId,  		CloseFile::cmdId);
	cb->bind(ChangeDir::eventId,  		ChangeDir::cmdId);
	cb->bind(InsertFile::eventId,		InsertFile::cmdId);
}

// File Classes

void NewFile::undo()
{
	idCheck();
}

Bool NewFile::redo()
{
	idCheck();
	appi->addView(new EditableView("noname", FALSE));
	return FALSE;
}

void OpenFile::undo()
{
	idCheck();
}

Bool OpenFile::redo()
{
	idCheck();
	FileSelector menu("OpenFile",
						FILESEL_ROWS,FILESEL_COLS,
						FILESEL_ORG_ROW,FILESEL_ORG_COL,
						fileHst, appi->getDirectory());

	menu.execute();

	if(menu.isAccepted() == TRUE) {
		if (!(menu.perms() & READ_PERM)) {
			MsgBox errbox(libMsg("ERROR"), 
							"NOREADPERM", 5, 35, 6, 20);
			errbox.execute();
		}
		else {
			for (StrCursor c(menu.value(), " "); c; c++) {
				assert(String(c) != NULL_STRING);
				appi->openFile(String(c));
				if (fileHst) 
					fileHst << String(c);
			}
		}
	}
	return FALSE;
}

Bool ReOpenFile::redo()
{
	idCheck();
	EditableView *view = appi->getDesktop()->getCurrEdView();
	
	// Es posible hacer un reOpen de la view activa ?
	if (view->isTextType(ISTREAM | ADMIN))
		return FALSE;

	if (view->isTextMod()) {
		MLDBox dbox(libMsg("MESSAGE"), view->getFileName()+NEWLINE+
				"EXITRDONLY4", DBOX_YES);

		dbox.execute();
		if (dbox.getOpt() != DBOX_YES)
			return FALSE;
	}

	// En que fila-columna estoy
	Int inrow = view->getRow();
	Int atcol = view->getCol();

	view->reload();
	view->goCursor(inrow,atcol,TRUE);
	return FALSE;
}

void ReOpenFile::undo()
{
	idCheck();
}

void HardSave::undo()
{
	idCheck();
}

Bool HardSave::redo()
{
	idCheck();
	EditableView *view = appi->getDesktop()->getCurrEdView();

	if (_canSave(view))
		return _hardSave(view);

	return FALSE;
}

void SoftSave::undo()
{
	idCheck();
}

Bool SoftSave::redo()
{
	idCheck();
	EditableView *view = appi->getDesktop()->getCurrEdView();

	if (_canSave(view))
		return _softSave(view);

	return FALSE;
}

void SaveAsFile::undo()
{
	idCheck();
}

Bool SaveAsFile::redo()
{
	idCheck();
	EditableView *view = appi->getDesktop()->getCurrEdView();

	if (view->isTextType(ISTREAM)) {
		appi->beep();
		return FALSE;
	}

	FileSelector menu("SaveAsFile",
						FILESEL_ROWS, FILESEL_COLS,
						FILESEL_ORG_ROW, FILESEL_ORG_COL,
						fileHst, appi->dirName);

	menu.execute();
	if (menu.isAccepted() == TRUE) {
		struct stat stbuf;
		String file = menu.value();
		if (fileHst)
			fileHst << file;

		if (statFile(file, &stbuf) == 0) {
			MLDBox dbox(libMsg("MESSAGE"),
						"FILEEXISTS",DBOX_YES);

			dbox.execute();
			if (dbox.getOpt() != DBOX_YES)
				return FALSE;
		}
		else {
			if (errno != ENOENT) {
				MsgBox errbox(libMsg("ERROR"),
						"COULDNTWRITE", 5, 32, 6, 20);
				errbox.execute();
				return FALSE;
			}
		}

		Int mode = menu.perms();
		if (!(mode & WRITE_PERM)) {
			MsgBox errbox(libMsg("ERROR"), "NOWRITEPERM",
							5, 35, 6, 20);
			errbox.execute();
			return FALSE;
		}

		if (view->chgFileName(file)) {
			view->store();
			view->writeStLine();
		}
		else {
			MsgBox errbox(libMsg("ERROR"), "TEXTBEINGMOD",
							6, 40, 6, 20);
			errbox.execute();
		}
	}

	return FALSE;

}

void SaveAll::undo()
{
	idCheck();
}

Bool SaveAll::redo()
{
	idCheck();    
	
	// ====>> Esto deberia hacerse con un metodo de la clase Desktop  <<====
	return appi->getDesktop()->saveViews();
}

void CloseFile::undo()
{
	idCheck();
}

Bool CloseFile::redo()
{
	idCheck();
	
	appi->getDesktop()->delCurrent();
	return FALSE;
}

Bool CloseAll::redo()
{
	idCheck();
	appi->closeAll();

	return FALSE;
}

void CloseAll::undo()
{
	idCheck();
}

void ChangeDir::clear()
{
	idCheck();
	dir = NULL_STRING;
}

void ChangeDir::undo()
{
	idCheck();

	if (chdir(toCharPtr(dir)) != 0) {
		MsgBox errbox(libMsg("ERROR"), 
						"COULDNTCHDIR", 5, 40, 6, 20);
		errbox.execute();
	}

	swap(appi->dirName, dir, String);
}

Bool ChangeDir::redo()
{
	idCheck();
	if (dir == NULL_STRING) {
		DirSelector menu("ChangeDir", 15, 29, 6, 15,
						 appi->dirName);

		menu.execute();

		if (menu.isAccepted() == TRUE)
			dir = menu.value();
		else
			return FALSE;
	}

	if (chdir(toCharPtr(dir)) != 0) {
		MsgBox errbox(libMsg("ERROR"),
				"COULDNTCHDIR", 5, 40, 6, 20);
		errbox.execute();
		return FALSE;
	}

	// ====>> Esto deberia hacerse con un metodo de la clase Desktop  <<====
					// Update all view`s name
					appi->getDesktop()->updateViewNames();

	swap(appi->getDirectory(), dir, String);

	return TRUE;
}

void InsertFile::clear()
{
	idCheck();
	fileName = NULL_STRING;
	nLines = 0;
}

void InsertFile::undo()
{
	idCheck();
	EditableView *view = appi->getDesktop()->getCurrEdView();
	if (nLines) {
		EditableTextCursor auxBeg = view->getCursor(), auxEnd = auxBeg;
		auxBeg.goToCol(0);
		auxEnd += nLines;
		auxEnd.goToCol(0);
		auxEnd.moveCol(-1);
		auxBeg.delTo(auxEnd.order(), auxEnd.column());
		view->redraw();
	}
}

Bool InsertFile::redo()
{

	idCheck();
	EditableView *view = appi->getDesktop()->getCurrEdView();

	if (fileName == NULL_STRING) {
		FileSelector menu("InsertFile",
							FILESEL_ROWS,FILESEL_COLS,
							FILESEL_ORG_ROW,FILESEL_ORG_COL,
							fileHst, appi->dirName);
		menu.execute();
		if(menu.isAccepted() != TRUE)
			return FALSE;

		fileName = menu.value();
		*fileHst << fileName;
	}
	nLines = view->insertFile(fileName);
	if (!nLines) return FALSE;
	return TRUE;
}

