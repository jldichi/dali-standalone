/********************************************************************
* Copyright (c) 1991 InterSoft Co.	All Rights Reserved
* THIS IS UNPUBLISHED PROPRIETARY SOURCE CODE OF InterSoft Co.
* The copyright notice above does not evidence any
* actual or intended publication of such source code.
*
* $Id: filecomm.cc,v 1.4 2004/09/29 18:34:04 damianm Exp $
*
* DESCRIPTION: Implementation of filecommands clases
*
*********************************************************************/
#ifdef __GNUC__
#pragma implementation
#endif

// para que genere la entrada dentro de la tabla de comandos
#define INITCOMM

#include <ifound.h>
#include <local/osfile.h>
#include <errno.h>
#include <unistd.h>
#include <ifound/stdc.h>
#include <edfsel.h>
#include <winapp/dirsel.h>
#include <winapp/msgbox.h>
#include <winapp/waitmsg.h>
#include <winapp/mldbox.h>
#include <winapp/label.h>
#include <winapp/cell.h>
#include <winapp/strhis.h>
#include <local/strfile.h>

#include <command.h>
#include <filecomm.h>
#include <editor.h>
#include <dview.h>
#include <dtext.h>
#include <winapp/menu.h>
#include <edlbox.h>

#include <sys/types.h>
#include <sys/stat.h>

#ifdef HAVE_UNDERSCORE_NAMES
#	define stat _stat
#endif

#define swap(a,b,tt)						\
	do {						 				\
		tt __tmp = a; a = b; b = __tmp;		\
 	} while (0)

declare(PtrList,DaliView);
declare(PtrListCursor,DaliView);

// @ Static Methods
static void _openFile(FileSelector &f)
{
	if (!(f.perms() & READ_PERM)) {
		MsgBox errbox(libMsg("ERROR"),
				Editor::editorMsg("NOREADPERM"), 5, 35, 6, 20);
		errbox.execute();
	}
	else {
		for (StrCursor c(f.value(), " "); c; c++) {
			if (c.value() == NULL_STRING)// Should go the assert 
				break;                   // instead of the if(...) break!
				
			// assert(c.value() != NULL_STRING);
			Editor::openFile(c.value());
			*Editor::files << c.value();
		}
		Editor::his->clear();
	}
}

#define EDFILESEL_ROWS		17
#define EDFILESEL_COLS		42
#define EDFILESEL_ORG_ROW	3
#define EDFILESEL_ORG_COL	16

static bool _saveAsFile(DaliView *cview)
{
	// Now that is allowed to save istreams and the debugger view IS
	// a istream_type view avoid this situation.
	// (Check both conditions, user may have a file named DEBUGGER_VIEW !!)
	if	(cview->getFile() == DEBUGGER_VIEW && cview->isIStream()) {
		Editor::beep();
		return false;
	}

	EdFileSelector menu(Editor::editorMsg("SaveAsFile"),
						EDFILESEL_ROWS,EDFILESEL_COLS,
						EDFILESEL_ORG_ROW,EDFILESEL_ORG_COL,
						Editor::files, Editor::dirName);

	menu.execute();
	if (menu.isAccepted() == true) {
		struct stat stbuf;
		String file = menu.value();
		*Editor::files << file;

		if (statFile(file, &stbuf) == 0) {
			MLDBox dbox(libMsg("MESSAGE"),
						Editor::editorMsg("FILEEXISTS"),DBOX_YES);

			dbox.execute();
			if (dbox.getOpt() != DBOX_YES)
				return false;
		}
		else {
			if (errno != ENOENT) {
				MsgBox errbox(libMsg("ERROR"),
						Editor::editorMsg("COULDNTWRITE"),5,32,6,20);
				errbox.execute();
				return false;
			}
		}

		Int mode = menu.perms();
		if (!(mode & WRITE_PERM)) {
			MsgBox errbox(libMsg("ERROR"),Editor::editorMsg("NOWRITEPERM"),
							5, 35, 6, 20);
			errbox.execute();
			return false;
		}

		if (cview->chgFileName(file)) {
			cview->store();
			cview->writeStLine();
		}
		else {
			MsgBox errbox(libMsg("ERROR"), Editor::editorMsg("TEXTBEINGMOD"),
							6, 40, 6, 20);
			errbox.execute();
		}
	}

	return false;
}

static bool _canSave(DaliView *cview)
{
	if ( cview->isRdOnly() || cview->isIStream() || cview->isAdmin() ) {
		MsgBox errbox(libMsg("ERROR"),Editor::editorMsg("NOWRITEPERM"),
						5,35,6,20);
		errbox.execute();
		return false;
	}

	return true;
}

static bool _hardSave(DaliView *cview)
{
	if ( cview->getFileName().index("noname") == ERR && !cview->isPreserve()) {
		cview->store();
		cview->writeStLine();
	}
	else
		return _saveAsFile(cview);

	return false;
}

static bool _softSave(DaliView *cview)
	// Only save file if it is modified
{
	if ( cview->isRdOnly() ||	cview->isIStream() ||
		 cview->isAdmin()  || !cview->isTextMod())
		return false;

	return _hardSave(cview);
}

// @ File Classes
void OpenFile::undo()
{
	idCheck();
}

bool OpenFile::redo()
{
	idCheck();
	EdFileSelector menu(Editor::editorMsg("OpenFile"),
						EDFILESEL_ROWS,EDFILESEL_COLS,
						EDFILESEL_ORG_ROW,EDFILESEL_ORG_COL,
						Editor::files,
						Editor::dirName, Editor::searchPath);

	menu.setUserFunc( (FPFS) _openFile );

	menu.execute();

	if(menu.isAccepted() == true)
		_openFile(menu);

	return false;
}

bool ReOpenFile::redo()
{
	idCheck();
	DaliView *cview = cView.value();

	// Es posible hacer un reOpen de la view activa ?
	if (cview->isIStream() || cview->isAdmin() || cview->isPreserve())
		return false;

	if (cview->isTextMod()) {
		MLDBox dbox(libMsg("MESSAGE"), cView->getFileName()+NEW_LINE+
				Editor::editorMsg("EXITRDONLY4"), DBOX_YES);
		dbox.execute();
		if (dbox.getOpt() != DBOX_YES)
			return false;
		Editor::his->clear();
	}

	// En que fila-columna estoy
	Int inrow = cview->getRow();
	Int atcol = cview->getCol();

	// Para recordar la marca
	Int mrow = cview->getMarkRow();
	Int mcol = cview->getMarkCol();

	cview->reload();
	Editor::his->clear();
	cview->goCursor(inrow,atcol,true);
	cview->setMark(mrow,mcol);
	return false;
}

void ReOpenFile::undo()
{
	idCheck();
}

void HardSave::undo()
{
	idCheck();
}

bool HardSave::redo()
{
	idCheck();
	DaliView *cview = cView.value();

	if (_canSave(cview))
		return _hardSave(cview);
	return false;
}

void SoftSave::undo()
{
	idCheck();
}

bool SoftSave::redo()
{
	idCheck();
	DaliView *cview = cView.value();

	if (_canSave(cview))
		return _softSave(cview);
	return false;
}

void SaveAsFile::undo()
{
	idCheck();
}

bool SaveAsFile::redo()
{
	idCheck();
	return _saveAsFile(cView.value());
}

void SaveAll::undo()
{
	idCheck();
}

bool SaveAll::redo()
{
	idCheck();
	return Editor::doForAllViews(_softSave);
}

void LoadWorkSpace::undo()
{
	idCheck();
}

bool LoadWorkSpace::redo()
{
	idCheck();
	String fileName;
	Int st = Editor::getCmdLine(Editor::editorMsg("OPENWSP") + ": ",
								fileName,
								Editor::workspaces);
	if ( st != ERR) {
		*Editor::workspaces << fileName;
		Editor::loadWorkSpace(fileName);
		Editor::his->clear();
	}

	return false;
}

void SaveWorkSpace::undo()
{
	idCheck();
}

bool SaveWorkSpace::redo()
{
	idCheck();
	String fileName;
	Int st = Editor::getCmdLine(Editor::editorMsg("SAVEWSP") + ": ",
								fileName,
								Editor::workspaces);
	if ( st != ERR) {
		*Editor::workspaces << fileName;
		 Editor::saveWorkSpace(fileName);
	}
	return false;
}

void NewFile::undo()
{
	idCheck();
}

bool NewFile::redo()
{
	idCheck();
	Editor::newFile();
	return false;
}

void CloseFile::undo()
{
	idCheck();
}

bool CloseFile::redo()
{
	idCheck();
	Editor::closeFile();
	return false;
}

bool CloseAll::redo()
{
	idCheck();
	if ( Editor::closeAll() )
		Editor::his->clear();
	return false;
}

void CloseAll::undo()
{
	idCheck();
}

// @ Navegation Classes
void NextFile::undo()
{
	idCheck();
}

bool NextFile::redo()
{
	idCheck();
	Editor::nextFile();
	return false;
}

void PrevFile::undo()
{
	idCheck();
}

bool PrevFile::redo()
{
	idCheck();
	Editor::prevFile();
	return false;
}

void SelectFile::undo()
{
	idCheck();
}

bool SelectFile::redo()
{
	idCheck();
	Editor::selectFile();
	return false;
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
	if (nLines) {
		DaliTextCursor	auxBeg = cursor(), auxEnd = auxBeg;
		auxBeg.goToCol(0);
		auxEnd += nLines;
		auxEnd.goToCol(0);
		auxEnd.moveCol(-1);
		auxBeg.delTo(auxEnd.order(), auxEnd.column());
		cView.value()->redraw();
	}
}

bool InsertFile::redo()
{
	idCheck();
	if (fileName == NULL_STRING) {
		EdFileSelector menu(Editor::editorMsg("InsertFile"),
							EDFILESEL_ROWS, EDFILESEL_COLS,
							EDFILESEL_ORG_ROW, EDFILESEL_ORG_COL,
							Editor::filesInserted,
							Editor::dirName,
							Editor::searchPath
							);
		menu.execute();
		if(menu.isAccepted() != true)
			return false;

		 fileName = menu.value();
		*Editor::filesInserted << fileName;
	}
	nLines = cView.value()->insertFile(fileName);
	if (!nLines) return false;
	return true;
}

static bool _updateViewNames(DaliView *cview)
	// update the view`s name with the correct path and name
{
	// No need to change istreams, and would conflict with chgFileName method
	if (cview->isIStream())
		return false;

	// Do it !
	cview->addPath();
	return false;
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
						Editor::editorMsg("COULDNTCHDIR"), 5, 40, 6, 20);
		errbox.execute();
	}
	swap(Editor::dirName, dir, String);
}

bool ChangeDir::redo()
{
	idCheck();
	if (dir == NULL_STRING) {
		DirSelector menu(Editor::editorMsg("ChangeDir"), 15, 29, 6, 15,
						 Editor::dirName);

		menu.execute();

		if (menu.isAccepted() == true)
			dir = menu.value();
		else
			return false;
	}

	if (chdir(toCharPtr(dir)) != 0) {
		MsgBox errbox(libMsg("ERROR"),
				Editor::editorMsg("COULDNTCHDIR"), 5, 40, 6, 20);
		errbox.execute();
		return false;
	}

	// Update all view`s name
	Editor::doForAllViews(_updateViewNames);

	swap(Editor::dirName, dir, String);

	return true;
}

void GotoFile::undo()
{
	idCheck();
}

bool GotoFile::redo()
{
	idCheck();
	DaliView *cview = cView.value();
	String errFile;
	Int	errLine;

	if ((errFile = cview->parseFileToken(errLine)) == NULL_STRING)
		Editor::beep();
	else  {
		Editor::gotoFile(errFile,errLine,chr == 'y');
		Editor::his->clear();
	}
	return false;
}

// clase CTags

bool CTags::redo()
{
	idCheck();
	// Build tags for each instance in DaliTags
	for (StrCursor c(Editor::daliTags,":"); c ; c++) {
		String pipeline,errmsg;
		MLDBox dbox(libMsg("MESSAGE"),
					Editor::editorMsg("BUILDTAG")+NEW_LINE+c.value(),
					DBOX_YES);
		dbox.execute();
		if (dbox.getOpt() != DBOX_YES)
			continue;
        
        { // This scope is opened because of a bug in gcc 2.5.7 
			InputPipe buildtag(String("bldtags ")+c.value());
			{ // message SCOPE
				WaitMsg pleaseWait(Editor::editorMsg("ONEMOMENT"), c.value());
				pleaseWait.execute();
				errmsg = NULL_STRING;
				while(buildtag >> pipeline)
					errmsg += pipeline;
			}

			if(buildtag.withError()) {
				MsgBox errbox(libMsg("ERROR")+" "+c.value(),errmsg,
						 9, 35, 5, 20);
				errbox.execute();
			}
		}

	}
	return false;
}

void CTags::undo()
{
	idCheck();
}

#define OPENTAG_ROWS		3
#define OPENTAG_COLS		64
#define OPENTAG_ORG_ROW		5
#define OPENTAG_ORG_COL		8

void OpenTag::undo()
{
	idCheck();
}

bool OpenTag::redo()
{
	idCheck();
	String tag;
	DaliView *cview = cView.value();

	EdLineDialog dlog(Editor::tags, "OpenTag",
		Editor::editorMsg("TAGTOFIND"), OPENTAG_ROWS,
		OPENTAG_COLS, OPENTAG_ORG_ROW, OPENTAG_ORG_COL);

	dlog.execute();
	if (dlog.isAccepted() != true)
		return false;

	tag = dlog.value();

	*Editor::tags << tag;

	if (Editor::openTag(tag))
		Editor::his->clear();

	if(cview) {
		cview->redraw();
		cview->refresh();
	}
	return false;
}

void FindTag::undo()
{
	idCheck();
}

bool FindTag::redo()
{
	idCheck();
	String tag;
	DaliView *cview = cView.value();

	if ((tag = cview->getToken()) == NULL_STRING)
		Editor::beep();
	else
		if (Editor::openTag(tag))
			Editor::his->clear();
	if (cview) {
		cview->redraw();
		cview->refresh();
	}
	return false;
}

// fin clase FindTag

// @ Window Classes

void ZoomFile::undo()
{
	idCheck();
}

bool ZoomFile::redo()
{
	idCheck();
	DaliView *cview = cView.value();

	if (cview->inOrgSize())
		cview->setSize(FORIG, CORIG, FILS, COLS);
	else
		cview->setOrgSize();

	return false;
}

void ResizeFileWin::undo()
{
	idCheck();
}

bool ResizeFileWin::redo()
{
	idCheck();
	DaliView *cview = cView.value();

	if (cview->resize())
		cview->redraw();

	return false;
}

void MoveFileWin::undo()
{
	idCheck();
}

bool MoveFileWin::redo()
{
	idCheck();
	DaliView *cview = cView.value();

	if (cview->move())
		cview->redraw();

	return false;
}

void TileWindows::undo()
{
	idCheck();
}

bool TileWindows::redo()
{
	idCheck();
	Editor::tileWindows();
	return false;
}

void CascadeWindows::undo()
{
	idCheck();
}

bool CascadeWindows::redo()
{
	idCheck();
	Editor::cascadeWindows();
	return false;
}

// @ Utilities
void Build::undo()
{
	idCheck();
}

bool Build::redo()
{
	idCheck();
	DaliView *cview = cView.value();

	if( cview->isAdmin() || cview->isPreserve() || cview->isIStream()) {
		Editor::beep();
		return false;
	}

	// Save all
	Editor::doForAllViews(_softSave);

	// Execute ibuild
	Editor::execOSCommand(Editor::buildCmd + " " + Editor::buildFlags + " " + cview->getFileName());

	return false;
}

void Generate::undo()
{
	idCheck();
}

bool Generate::redo()
{
	idCheck();
	struct stat stbuf;

	String ibfile = cView.value()->getFileName().baseName() + ".ib";

	if (statFile(ibfile, &stbuf) == 0) {
		MLDBox dbox(libMsg("MESSAGE"),
					Editor::editorMsg("FILEEXISTS"),
					DBOX_YES);
		dbox.execute();
		if (dbox.getOpt() != DBOX_YES)
			return false;
	}

	String cmdline = String("genib ") + cView.value()->getFileName().baseName() +
			" " +  Editor::ibFiles() + " > " + ibfile;

	String pipeline,errmsg;
	InputPipe genib(cmdline);
	{ // message SCOPE
		WaitMsg pleaseWait(Editor::editorMsg("ONEMOMENT"),
							Editor::editorMsg("GENERATE"));
		pleaseWait.execute();

		while(genib >> pipeline)
			errmsg += pipeline;
	}

	// Si no hubo error hago reload del archivo
	if (genib.withError()) {
		MsgBox errbox(libMsg("ERROR"),errmsg, 9, 35, 5, 20);
		errbox.execute();
	}
	else
		Editor::openFile(ibfile);

	return false;
}

void FileDiff::undo()
{
	idCheck();
}

bool FileDiff::redo()
{
	idCheck();
	Editor::diff();
	return false;
}

void SetProject::clear()
{
	idCheck();
	oldProject = NULL_STRING;
}

void SetProject::undo()
{
	idCheck();
	String op = String("IPROJECT=") + oldProject;
	op.putEnv();
}

bool SetProject::redo()
{
	idCheck();

	String newProject;
	if (oldProject == NULL_STRING) {
		EdLineDialog dlog(Editor::projects, "Set Project",
			Editor::editorMsg("SetProject"), OPENTAG_ROWS,
			OPENTAG_COLS, OPENTAG_ORG_ROW, OPENTAG_ORG_COL);

		dlog.execute();
		if (dlog.isAccepted() != true)
			return false;

		oldProject = newProject = dlog.value();
		*Editor::projects << newProject;
	}

	newProject = String("IPROJECT=") + oldProject;
	newProject.expandEnvVars().putEnv();

	return true;
}

// DALI-STANDALONE: Removed RCS/SCCS version control classes
// (VersionLog, CheckOutFile, CheckInFile, UndoCheckOut, UndoCheckIn,
//  OpenLastVersion, DiffLastVersion)

// @ clase ReadDaliRc
bool ReadDaliRc::redo()
{
	idCheck();
	Editor::readDaliRc();
	return false;
}

void ReadDaliRc::undo()
{
	idCheck();
}

// end clase ReadDaliRc
