/********************************************************************
* Copyright (c) 1991 InterSoft Co.  All Rights Reserved
* THIS IS UNPUBLISHED PROPRIETARY SOURCE CODE OF InterSoft Co.
* The copyright notice above does not evidence any
* actual or intended publication of such source code.
*
* $Id: editor.cc,v 1.8 2002/03/18 21:49:33 hardaiz Exp $
*
* DESCRIPTION: Implementation of Editor class
*********************************************************************/

#include <sys/types.h>
#include <sys/stat.h>

#ifdef __GNUC__
#pragma implementation
#endif

#include <ifound.h>
#include <ifound/stdc.h>
#include <ifound/priv/gfuncs.h>
#include <ifound/msgtable.h>
#include <ifound/barray.h>
#include <ifound/list.h>
#include <ifound/parray.h>
#include <ifound/phashtab.h>

#include <local/strfile.h>
#include <local/osfile.h>
#include <local/dirname.h>
#include <local/dbrowser.h>

#include <errno.h>
#include <fcntl.h>

#ifdef HAVE_UNISTD_H
#	include <unistd.h>
#endif

#include <winapp/strhis.h>
#include <winapp/cell.h>
#include <winapp/label.h>
#include <winapp/mldbox.h>
#include <winapp/msgbox.h>
#include <winapp/waitmsg.h>
#include <winapp/menu.h>
#include <winapp/textview.h>
#include <winapp/window.h>

#include <editor.h>
#include <dview.h>
#include <dtext.h>
#include <command.h>
#include <dsensor.h>
#include <winapp/log.h>
#include <edhelp.h>
#include <datroff.h>
#include <datex.h>
#include <dalirc.h>
#include <debugger.h>

//+++ Config +++
#ifdef HAVE_UNDERSCORE_NAMES
#	define stat _stat
#endif

// Dimension de	los	histories

#define SMALL_HISTORY   5
#define NORMAL_HISTORY 10
#define LARGE_HISTORY  20
#define HUGE_HISTORY   30

//--- Config ---

Int Editor::lhis = 300;
Int Editor::ord  = -1;
Int Editor::mode = 0;

#include <winapp/strlist.h>

// Keywords hash table
declare(PtrHashTab, KeyWord);
implement(PtrHashTab, KeyWord);

// Lista de	Views
declare(PtrList, DaliView);
declare(PtrListCursor, DaliView);
implement(PtrList, DaliView);
implement(PtrListCursor, DaliView);

// Lista de	Match-Pairs
declare(PtrList, MatchPair);
declare(PtrListCursor, MatchPair);
implement(PtrList, MatchPair);
implement(PtrListCursor, MatchPair);

// Lista de	SourceBrowsers
declare(PtrList, SourceBrowserEntry);
declare(PtrListCursor, SourceBrowserEntry);
implement(PtrList, SourceBrowserEntry);
implement(PtrListCursor, SourceBrowserEntry);

// Lista de Indices de Tags
declare(PtrList, TagIndex);			// Indice de tags
implement(PtrList, TagIndex);
declare(PtrListCursor, TagIndex);	// Cursor
implement(PtrListCursor, TagIndex);
declare(PtrArray, TagIndexEntry);	// Entradas en un indice de tags

// Lista de Keywords
static PtrHashTab(KeyWord) *keyWords = NULL;

// Lista de indices de tags del editor
PtrList(TagIndex) *Editor::tagIndexList = NULL;

PtrList(SourceBrowserEntry)	*Editor::sbrowserList	= NULL;
PtrList(MatchPair)			*Editor::matchList		= NULL;
PtrList(DaliView)			*Editor::viewlist		= NULL;
PtrListCursor(DaliView)		*Editor::_cview			= NULL;
List(String)				*Editor::clipboard		= NULL;

// Option Parser
bool						 Editor::op_noDalirc	= false;
bool						 Editor::op_printDir	= false;
String						 Editor::op_loadWsp;
String 						 Editor::op_findTag;
String 						 Editor::op_execOSCmd;
bool				 		 Editor::useCmdLine		= false;
Debugger					*Editor::debugger		= NULL;
History						*Editor::his			= NULL;
Int							 Editor::scratchs		= 0;
MsgTable					*Editor::_editorMsg		= NULL;
DaliSensor					 *Editor::sen			= NULL;
String						 Editor::desktopCwd;
String						 Editor::desktopFiles;
String						 Editor::lastWsp;
time_t				 		 Editor::lastUpdate		= 0;
Window						*Editor::deskTop		= NULL;

#ifdef USES_OLD_HELP_MENU_STYLE
EditorHelp					*Editor::editorHelp		= NULL;
#endif

// .dalirc default values

bool	Editor::autoIndent		= false;
Int		Editor::autoSaveFreq_	= 60;
String	Editor::daliDir;
String	Editor::daliMan;
String	Editor::diffCmd			= "diff";
String	Editor::buildCmd		= "ib";
String	Editor::buildFlags		= "";
String	Editor::daliTags;
bool	Editor::deskTopChar		= false;
String	Editor::dirName;
Int		Editor::footerHeight	= 5;
Int		Editor::functionSensor	= FS_OFF;
Int		Editor::maxViews		= NVIEWS;
bool	Editor::saveDesktop		= false;
Int		Editor::scratchViews	= 4;
String	Editor::searchPath;
String  Editor::userMode		= "expert";
Int		Editor::winsTile		= 2;
bool 	Editor::funcExpBrowser	= false;

// String Histories

StrHistory *Editor::adminComments	= NULL;
StrHistory *Editor::expressions		= NULL;
StrHistory *Editor::osCommands		= NULL;
StrHistory *Editor::wmCommands		= NULL;
StrHistory *Editor::workspaces		= NULL;
StrHistory *Editor::files			= NULL;
StrHistory *Editor::filesInserted	= NULL;
StrHistory *Editor::dirs			= NULL;
StrHistory *Editor::tags			= NULL;
StrHistory *Editor::filters			= NULL;
StrHistory *Editor::finds			= NULL;
StrHistory *Editor::replaces		= NULL;
StrHistory *Editor::helps			= NULL;
StrHistory *Editor::debugProgs		= NULL;
StrHistory *Editor::debugEntries	= NULL;
StrHistory *Editor::projects		= NULL;
StrHistory *Editor::greps   		= NULL;
StrHistory *Editor::grepFiles		= NULL;

//  1	Static functions
static Int cmpKeyWord(const	void *left,	const void *right)
{
	KeyWord *kw1 =	 *(KeyWord **) left;
	KeyWord *kw2 =	 *(KeyWord **) right;

	Int r;
	if	((r	= ::cmp(kw1->kw, kw2->kw)) != 0)
		return r;
	return	::cmp(kw1->ext,	kw2->ext);
}

static Int hashKeyWord(const void *node, Int modulo)
{
	KeyWord *k	= *(KeyWord	**)	node;
	String	tmp	= k->kw	+ k->ext;
	return	hashVal(tmp, modulo);
}

static KeyWord *findKeyWord(const String &k, const String &e)
{
	KeyWord kw(k, e);
	return	keyWords->find(&kw);
}

//Mientras se mejora ScrollMenu	...
static String expandTabs(const String &s)
{
	Int len = s.length();
	String	rs;
	for (Int a	= 0; a < len; a++) 
		if	(s[a] != '\t')
			rs +=	s[a];
		else
			rs +=	String(4,' ');
	return rs;
}

bool Editor::confirmExit()
{
	// This could be a general confirmation function
	if	(saveDesktop && lastWsp != NULL_STRING) {   
		 	MLDBox dbox(libMsg("MESSAGE"), 
		 				editorMsg("SAVEWSP")+NEW_LINE+lastWsp,
						DBOX_YES);

			dbox.execute();
			switch (dbox.getOpt()) {
				case DBOX_YES:
					saveWorkSpace(lastWsp);
					break;	
				case DBOX_NO:
					break;         
				case DBOX_CANCEL:
				default:
					return false;
			}
   	}
	return true;
}

//  2	Main methods

void Editor::execute()
{                             
	UnShort c;
	Int cmd;
	if ( init() ) {
		forever {
			// Test if user wants to exit program      
			if ((cmd = sen->getEvent(c)) ==	ERR ) {
				if ( confirmExit() )	break;
				else continue;
			}

#			ifdef ISDEBUG
				tr_mem_check();
#			endif

			sen->getCmd(cmd)->execute((UnChar)c,1);
		}
}

	clear();
	if (op_printDir)
		stdOut << dirName;
}

bool Editor::init()
{
//	inicializacion de la tabla de mensajes
	_editorMsg	= tr_new MsgTable("dali");

	//	inicializacion del help	del	editor
#ifdef USES_OLD_HELP_MENU_STYLE
	editorHelp	= tr_new EditorHelp("dalihlp");
#endif

	bool openpres = true;
	String	presdir;

	//	Read the cwd
	char aux[LONG_CWD];
	getcwd(aux, LONG_CWD -	1);
	dirName = aux;

	//	inicializacion de la tabla de conversion Ideafix-Troff
	TroffSpecialChar::InitTable();

	//	inicializacion de la tabla de conversion Ideafix-TeX
	TeXSpecialChar::InitTable();

	//	Window manager initialization calls
	Window::setRaw(WM_PURERAW);
	//	Create deskTop
	Int deskTopHeight 	= Window::rootWindow()->height() - 2;
	Int deskTopWidth  	= Window::rootWindow()->width();
	deskTop 			= tr_new Window(1, 1,	deskTopHeight, deskTopWidth,
									NO_BORDER,	NULL_STRING, A_NORMAL);

	//	Inicializacion de las listas de	matchpairs y sourcebrowsers
	matchList = tr_new PtrList(MatchPair);
	sbrowserList = tr_new PtrList(SourceBrowserEntry);

	//	Check for .dali subdirectory
	struct	stat st;
	String rcDir = String("HOME").getEnv() + PATH_SLASH + String(DALIDIR);
	daliDir = rcDir + String(PATH_SLASH);

	// If its not there ask if it should be created
	if (statFile(rcDir, &st) || !(st.st_mode & S_IFDIR)) {
		 	MLDBox dbox(libMsg("MESSAGE"), editorMsg("DOTDALINOTFOUND"),
						DBOX_NO);
			dbox.execute();
			if (dbox.getOpt() == DBOX_YES) {
				String d = String("HOME").getEnv() + PATH_SLASH 
							+ String(DALIDIR);
#ifndef __NT__
				if (! mkdir(toCharPtr(d), S_IREAD|S_IWRITE|S_IEXEC)) 
#else
				if (! mkdir(toCharPtr(d))) 
#endif
					stop(2);
			} else stop(2);
	}	
	if	(! op_noDalirc)
		readDaliRc();
	if	(deskTopChar) {
		String deskLine(deskTopWidth, char(C_SHADOW));
		for (Int k = 1; k < deskTopHeight; k++)
			*deskTop << deskLine;
		deskTop->wrap(false);
		*deskTop << deskLine;
		deskTop->wrap(true);
	}
	deskTop->goTo(0, 0);

	//	Set	View`s default window parent
	DaliView::setViewPar(deskTop);

	//	inicializacion de sensor y history
	sen = tr_new DaliSensor();
	his = tr_new History(lhis);

	//	inicializacion de la lista de views
	viewlist  = tr_new PtrList(DaliView);

	//	inicializacion del strlist para	el clipboard
	clipboard = tr_new List(String)(NULL_STRING);

	//	inicializacion del PtrArray	para la	lista de keywords
	keyWords =	tr_new	PtrHashTab(KeyWord)(KW_HT_SIZE,
			FP_CMP(cmpKeyWord), FP_HASH(hashKeyWord));
	readKeyWords();

	//	String Histories
	expressions		= tr_new StrHistory(LARGE_HISTORY);
	adminComments	= tr_new StrHistory(LARGE_HISTORY);
	osCommands		= tr_new StrHistory(LARGE_HISTORY);
	wmCommands		= tr_new StrHistory(LARGE_HISTORY);
	files			= tr_new StrHistory(HUGE_HISTORY);
	filesInserted	= tr_new StrHistory(HUGE_HISTORY);
	workspaces		= tr_new StrHistory(LARGE_HISTORY);
	dirs			= tr_new StrHistory(NORMAL_HISTORY);
	tags			= tr_new StrHistory(NORMAL_HISTORY);
	filters			= tr_new StrHistory(SMALL_HISTORY);
	finds			= tr_new StrHistory(LARGE_HISTORY);
	replaces		= tr_new StrHistory(LARGE_HISTORY);
	helps			= tr_new StrHistory(NORMAL_HISTORY);
	debugProgs		= tr_new StrHistory(NORMAL_HISTORY);
	debugEntries	= tr_new StrHistory(NORMAL_HISTORY);
	projects		= tr_new StrHistory(SMALL_HISTORY);
	greps			= tr_new StrHistory(LARGE_HISTORY);
	grepFiles		= tr_new StrHistory(LARGE_HISTORY);

	String	hfilename =	daliDir	+ String(HISTFILE);
	InputFile hfile(hfilename);

	expressions->load(hfile);
	adminComments->load(hfile);
	osCommands->load(hfile);
	wmCommands->load(hfile);
	files->load(hfile);
	workspaces->load(hfile);
	dirs->load(hfile);
	tags->load(hfile);
	filters->load(hfile);
	finds->load(hfile);
	replaces->load(hfile);
	helps->load(hfile);
	debugProgs->load(hfile);
	debugEntries->load(hfile);
	projects->load(hfile);
	greps->load(hfile);
	grepFiles->load(hfile);
	filesInserted->load(hfile);

	// Initialize preserve directory to users $HOME/preserve.
	presdir = String("HOME").getEnv() + PATH_SLASH + String(PRESERVE_DIR);

	// Tell the view which is its preserve directory.
	if	(statFile(presdir, &st)	|| !(st.st_mode	& S_IFDIR))	{
		openpres = false;
		DaliView::setTempDir("/tmp/dlXXXXXX");
	} else
		DaliView::setTempDir(presdir + PATH_SLASH + "dlXXXXXX");

	//	And	its	message	table
	DaliView::setMessages(*_editorMsg);

	//
	//	If	no arguments passed, check the preserve	directory
	//	for files that	are	not	locked,	and	add	them to	the
	//	view list.
	//
	bool presViews	= false;
	if	(argCnt() >	1) {
		// armo la lista de views
		for (Int i = argCnt() - 1; i > 0;	i--) {
			String fullName = findInPath(argVal(i), searchPath);

			if (! checkPerms(fullName) )
				continue;
				
			viewlist->add(tr_new DaliView(fullName,	false));
		}
	} else {
		presViews = readPreserved();
	}
	
	//	inicializacion del cursor de la	list de	views
	_cview = tr_new	PtrListCursor(DaliView)(*viewlist);
	cView.goToLast();

	//	activacion de las views	correspondientes
	Int cant =	viewlist->size() > maxViews	? maxViews : viewlist->size();

	DaliView *actv	= NULL;
	if	(cant) {
		// activo todas las views (Cascade style)
		Int i	= 0;
		for (cView -=	(cant-1) ; i < cant-1; i++,	++cView)
			cView->activate(FORIG+i,CORIG, FILS-i, COLS);

		// If no wsp is going to be load or this is a preserved-file
		// then make this the current view.
		actv = *cView;

		cView->activate(FORIG+i, CORIG, FILS-i, COLS);
	}

	// If no preserve files	found, then	load workspace and/or tag and/or OScmd
	if	(! presViews) {
		if ( ! op_findTag.isNull() )
			openTag(op_findTag);    
 
 		if ( ! op_execOSCmd.isNull() )
			execOSCommand(op_execOSCmd);    
        
        if (op_findTag.isNull() && op_execOSCmd.isNull()) {
	 		if (op_loadWsp.isNull()) {
				if(saveDesktop && cant == 0) {
					loadWorkSpace(DESKTOPFILE);
					lastWsp	= NULL_STRING;
				}
			}
			else
				loadWorkSpace(op_loadWsp, true);               
		}
	}
	if (actv &&	(op_findTag.isNull() || op_execOSCmd.isNull() || presViews)) {
		if	(cView)
			cView->setCurrent(false);
		actv->setCurrent(true);
		setLastView(actv);
	}

	// Warning that	no preserve	dir	found
	if	(!openpres)	{
		MsgBox warbox(libMsg("MESSAGE"),
			editorMsg("PRESERVENOTFOUND")+" "+presdir, 6, 40, 6, 20);
		(void)warbox.execute();
	}

	return true;
}

void Editor::clear()
{
	//	Delete debugger	object if present ?
	if	(mode &	DEBUG_MODE)
		tr_delete(debugger);

	// Salvo el WorkSpace  
	if	(saveDesktop) 
		saveWorkSpace(DESKTOPFILE);

	//	borro todas	las	listas
	for (cView = *viewlist; cView; ++cView)
		DaliView::deleteView(*cView);

	{
		// Open a scope to ensure PtrListCursors die before the list
		// they run through

		for (PtrListCursor(MatchPair) mpl(*matchList)	; mpl; ++mpl)
			tr_delete(*mpl);

		for (PtrListCursor(SourceBrowserEntry) sbe(*sbrowserList); sbe; ++sbe)
			tr_delete(*sbe);

		if (tagIndexList) {
			for (PtrListCursor(TagIndex) til(*tagIndexList) ; til ; ++til)
				tr_delete(*til);
		} 
	}

	//	limpio el log
	Log().cleanUp();
	String	hfilename =	daliDir	+ String(HISTFILE);

	OutputFile	hfile(hfilename);

	expressions->store(hfile);
	adminComments->store(hfile);
	osCommands->store(hfile);
	wmCommands->store(hfile);
	files->store(hfile);
	workspaces->store(hfile);
	dirs->store(hfile);
	tags->store(hfile);
	filters->store(hfile);
	finds->store(hfile);
	replaces->store(hfile);
	helps->store(hfile);
	debugProgs->store(hfile);
	debugEntries->store(hfile);
	projects->store(hfile);
	greps->store(hfile);
	grepFiles->store(hfile);
	filesInserted->store(hfile);

	// Delete histories
	tr_delete(filesInserted);
	tr_delete(grepFiles);
	tr_delete(greps);
	tr_delete(projects);
	tr_delete(debugProgs);
	tr_delete(debugEntries);
	tr_delete(expressions);
	tr_delete(adminComments);
	tr_delete(osCommands);
	tr_delete(wmCommands);
	tr_delete(files);
	tr_delete(workspaces);
	tr_delete(dirs);
	tr_delete(tags);
	tr_delete(filters);
	tr_delete(finds);
	tr_delete(replaces);
	tr_delete(helps);

	// And now the rest
	tr_delete(his);
	tr_delete(sen);

	// View list cursor and list itself
	tr_delete(_cview);
	tr_delete(viewlist);

	tr_delete(tagIndexList);
	tr_delete(sbrowserList);
	tr_delete(matchList);
	tr_delete(clipboard);
	tr_delete(_editorMsg);

	//	toDo: for (...)	{ tr_delete(...); }
	tr_delete(keyWords);

#ifdef USES_OLD_HELP_MENU_STYLE
		tr_delete(editorHelp);
#endif

}

//  3	Methods	that read external files
void Editor::readDaliRc()
	// Reads Dali`s configuration file (at $HOME/.dali/dalirc)
{
	// Read configuration file
	String	rcfile(daliDir + String(DALIRC));

	DaliRc	dalirc(rcfile);

	// Get out of here if no file to read !
	if	(!dalirc) 
		return;
		
	// toDo ? : Skip This the first time ?
	//	Delete MatchTab, SourceBrowser & TagIndexList 
	//	before (re)reading .dalirc
	for (PtrListCursor(MatchPair) mpl(*matchList)	; mpl; ++mpl)
		tr_delete(*mpl);
	matchList->delAll();

	for (PtrListCursor(SourceBrowserEntry)	sbe(*sbrowserList)	; sbe; ++sbe)
		tr_delete(*sbe);
	sbrowserList->delAll();
    
	if (tagIndexList) {
		{	// Scope needed to make PtrListCursor die before deleting 
			// the list it runs through. 
			for (PtrListCursor(TagIndex) til(*tagIndexList) ; til ; ++til)
				tr_delete(*til);
		}  
		tr_delete(tagIndexList);
		tagIndexList = NULL;	// Assign NULL to load again indeces on demand
	}

	daliTags = NULL_STRING;

	// OK, now read the file.
	dalirc.readFile();
}

bool Editor::readPreserved()
	// Reads all the preserved files (at $HOME/preserve)
{
	bool hasSomething = false;
	struct	stat st;
	String presdir = String("HOME").getEnv() + PATH_SLASH + String(PRESERVE_DIR);   

	int fd;

	if	(presdir ==	NULL_STRING) {
		return false;         
	}		

	//	longitud del prefijo para el archivo temp
	RegExp re(PREFIX_REXP);
	assert((!re) == false);
	String	nm;

	for (DirBrowser db(presdir, &re); db; ++db) {

		fd = ERR;
		nm = db.fullName();

		const char *fname = toCharPtr(nm);

		if (!statFile(nm, &st)
				&& st.st_mode&S_IFREG
				&& (fd = _global_open(fname, O_RDONLY, 0666)) != ERR
#ifndef __NT__				
				&& lockFile(fd, false, true, 0L, 0L) != ERR) {
#else  // to do (add physical locking in NT)
				) {
#endif
			if (st.st_size == 0) {
				unlinkFile(nm);
				continue;
			}
			viewlist->add(tr_new DaliView(nm, true));
			hasSomething = true;
		}

		if (fd != ERR)
			_global_close(fd);
	}

	return	hasSomething;
}

void Editor::readKeyWords()
	// 	Reads keywords to highlight by Dali 
	//	At $DATADIR/keywords, superseeded by $HOME/.dali/keywords
{
	//	First look at user`s home
	String	fname =	daliDir	+ String(KEYWORDFILE);

	InputFile file(fname);

	//	If not there, then try at $DATADIR
	if	(!file)	{
		fname	= String("DATADIR").getEnv() + PATH_SLASH + String(KEYWORDFILE);
		file.open(fname);
	}

	//	Read the file
	String	line;
	while (file >>	line) {
		if (!line	|| line[0] == '#') continue;
		String ty =	line.field(0, KW_SEP);
		String exts = line.field(1, KW_SEP);
		String kws = line.field(2, KW_SEP);
		KeyWord *kw;
		String tKw, tExt;
		for (StrCursor ckw(kws, KW_WORD_SEP);	ckw; ckw++)	{
			for (StrCursor cext(exts, KW_EXT_SEP); cext;	cext++)	{
				tKw	= ckw.value();
				tExt = cext.value();
				kw = findKeyWord(tKw, tExt);
				if (kw == NULL)	{
					kw	= tr_new KeyWord(tKw, tExt, toInt(ty));
					keyWords->add(kw);
				} else {
					MsgBox msgbox(libMsg("WARNING"),
							Editor::editorMsg("KWREDEFINED")	+
							" " + tKw + "/" + tExt, 5, 50, 6, 15);
							msgbox.execute();
				}
			}
		}
	}
}

//  4	Methods	that sets values

void Editor::historySize(Int v)
{
	lhis =	v;
	if	(his) his->reAlloc(v);
}

void Editor::tabSize(Int v)
{
	DaliView::tabSize(v);
}

//  5	Informational methods

String Editor::newName()
{
	return	String("noname")+toString(++ord);
}

Int	Editor::nroActViews(bool inc_stream)
{
	Int i = 0;
	for (PtrListCursor(DaliView) vlc(*viewlist); vlc;	++vlc)
		if (vlc->isActive() && (inc_stream ||	!vlc->isIStream()))
			i++;
	return	i;
}

#define MAX_VIEW_NAME_LEN  60

const String Editor::nameView(Int i)
{
	Int cant =	viewlist->size();

	if	(i < 0 || i	>= cant)
		return HIGH_VALUE;

	PtrListCursor(DaliView) vlc(*viewlist);

	vlc.goTo(cant - i - 1);

	String	tmp	= vlc.value()->getFile();

	if	(vlc.value()->isActive())
		tmp =	String(1, C_BULLET)+String(1, ' ')+tmp;
	else
		tmp =	String("  ")+tmp;

	tmp = tmp(0,MAX_VIEW_NAME_LEN);

	return	tmp;
}

String Editor::editorMsg(const char	*s)
{
	return	_editorMsg->find(s);
}

UnChar Editor::getKey()
{
	UnChar	ch;
	return	nroActViews() ?	cView->getKey()	: (*deskTop	>> ch, ch);
}

bool Editor::isFooterActive()
{
	for (PtrListCursor(DaliView) vlc(*viewlist); vlc;	++vlc)
		if (vlc->isFooter() && vlc->isActive())
			return true;
	return	false;
}

String Editor::ibFiles()
{
	PtrListCursor(DaliView) vlc(*viewlist);

	String	filelist;
	for (vlc.goToLast(); vlc; --vlc)
		if (!vlc.value()->isIStream())
			filelist += vlc.value()->getFileName()+" ";

	return	filelist;
}

void Editor::beep()
{
	nroActViews() ? cView->beep() : deskTop->beep();
}

//  6	Workspace methods

void Editor::buildDeskTopLine(DaliView *v, Int iac)
{
	String	sep	= ",";
	
	if(desktopFiles)
		desktopFiles += ":";
		
	desktopFiles += v->getFileName()			+ sep
				+	toString(v->getRow())		+ sep
				+	toString(v->getCol())		+ sep
				+	toString(v->orgCol())		+ sep
				+	toString(v->orgRow())		+ sep
				+	toString(v->orgHeight())	+ sep
				+	toString(v->orgWidth())		+ sep
				+	toString(v->getMarkRow())	+ sep
				+	toString(v->getMarkCol())	+ sep
				+	toString(iac);
}

void Editor::loadWorkSpace(const String	&wspfile, bool additive)
{
	// Read the workspace file
	desktopFiles = NULL_STRING;
	InputFile dsk(daliDir + wspfile + WORKSPACE_EXT);

	if (dsk) {
		String ibProject, line;  

		// Read project and cwd
		dsk >> ibProject;
		dsk >> desktopCwd;

		// Read files
		while (dsk >> line) {
			if (desktopFiles)
				desktopFiles +=	":";
			desktopFiles += line;
		}
		(String("IPROJECT=")+ibProject).putEnv();
	}

	// Cierro todo o cambio	estado de view corriente
	if	( !	additive ) {
		if ( ! closeAll()	)
			return;
	}
	else 
		if (cView)
			cView->setCurrent(false);

	//	Now	change to workspace	directory and open the files
	if	(chdir(toCharPtr(desktopCwd)) != 0)	{
		MsgBox errdt(libMsg("ERR"),
				editorMsg("COULDNTCHDIR"), 5, 40, 6, 20);
		errdt.execute();
	}
	else {
		DirName dn(dirName);
		dn.chDir(desktopCwd);
		dirName = dn.value();
		Int col, lin,	oc,	Or,	oh,	ow,	mo,	mc,	ic;
		for(StrCursor d(desktopFiles,":"); d; ++d) {
			String sd(d.value());
			if (sd.isNull()) continue;
            
	        { // This scope is opened because of a bug in gcc 2.5.7 
				String dName(sd.field(0,","));
				
				if ( checkPerms(dName) ) {

					lin = toInt(sd.field(1,","));
					col = toInt(sd.field(2,","));
					oc  = toInt(sd.field(3,","));
					Or  = toInt(sd.field(4,","));
					oh  = toInt(sd.field(5,","));
					ow  = toInt(sd.field(6,","));
					mo  = toInt(sd.field(7,","));
					mc  = toInt(sd.field(8,","));
					ic  = toInt(sd.field(9,","));  //0=NOT ACTIVE, 1=ACTIVE,	2=CURRENT

					DaliView	*dView	= tr_new DaliView(dName, false);
					viewlist->add(dView);
					if (ic >	0) {
						dView->activate(Or,oc,oh,ow);
						dView->goCursor(lin,col,true);
						dView->setMark(mo,mc);
					}
				}
			}
		}
		cView.goToLast();
		if (cView)
			cView->setCurrent(true);
	}

	Editor::lastWsp = wspfile;
}

void Editor::saveWorkSpace(const String	&wspfile)
{
	desktopFiles =	NULL_STRING;
	for (PtrListCursor(DaliView) vlc(*viewlist); vlc;	++vlc)
		if (!vlc->isIStream() && *vlc != *cView)
			buildDeskTopLine(*vlc, vlc->isActive());

	if	(cView && !cView->isAdmin()	&& !cView->isIStream())
		buildDeskTopLine(*cView, 2);

	OutputFile dsko(daliDir + wspfile + WORKSPACE_EXT);

	dsko << String("IPROJECT").getEnv() << NEW_LINE;
	dsko << dirName   << NEW_LINE;
	for (StrCursor	cd(desktopFiles,":"); cd; ++cd)	{
 		String dl(cd.value());
		if (dl.isNull()) continue;
		dsko << dl << NEW_LINE;
	}

	Editor::lastWsp = wspfile;
}

//  7	View list manipulation methods

void Editor::makeCurrentView(DaliView *view)
{
	DaliView *saveView	= *cView;

	setLastView(view);

	//	Make sure the view is activated
	if	(view->isActive()) {
		saveView->setCurrent(false);
		view->setCurrent(true);
	} else	{
		saveView->deActivate();
		view->activate(saveView->orgRow()	 +	1, saveView->orgCol(),
						saveView->orgHeight() -	1, saveView->orgWidth(), true);
	}

	cView->redraw();
	cView->refresh();
}

void Editor::setLastView(DaliView *v)
	//	Reorders Viewlist, putting 'v' as the last view
{
	//	Find 'v' in	the	viewlist
	PtrListCursor(DaliView) vlc(*viewlist);
	for (; vlc && *vlc != v; ++vlc)
		;

	//	Take 'v' and append	it in the list
	cView.goTo(vlc.order());
	cView.del();
	viewlist->add(v);

	cView.goToLast();
}

Int	Editor::makeRoom(DaliView *v)
	//	Makes sure that	there is room for a	tr_new	View
{
	Int cant =	nroActViews();

	if (cant ==	0) {
		cView.add(v);
		return	0;
	}

	DaliView *p	= *cView;

	if(cant == maxViews) {
		// Try to deactivate the oldest active view in the list
		PtrListCursor(DaliView) vlc(*viewlist);

		for	(vlc.goToFirst(); vlc && !vlc->isActive(); ++vlc)
				;

		if	(!(vlc->exit())) {
			MsgBox cantPerform(libMsg("ERR"),
					editorMsg("CANTPERFORM"), 7, 40, 6, 20);
			cantPerform.execute();
			DaliView::deleteView(v);
			return ERR;
		}

		p->setCurrent(false);	// Change	state of current view;
		vlc->deActivate();
		viewlist->add(v);
	}
	else {
		p->setCurrent(false);	// Change	state of current view;
		viewlist->add(v);
	}

	// Point to	the	recently added view
	cView.goToLast();
	return	cant;
}

void Editor::addView(DaliView *view)
{
	if	(view->isFooter()) {
		addFooterView(view);
		return;
	}

	DaliView *prev = *cView;

	//	Is there room for a	tr_new	View ?
	Int cant =	makeRoom(view);

	// No room for another View
	if(cant==ERR)
		return;

	Int fh	 = isFooterActive()	? (footerHeight	+ 2) : 0;

	// First View --> FULL SIZE
	if(nroActViews(false) == 0)	{
		if	(fh)	fh	+= FORIG - 1; // @bruno
		cView->activate(FORIG, CORIG,	FILS - fh ,	COLS, true);
		return;
	}

	Int	c		= prev->Width();
	Int	co		= prev->orgCol();
	Int	fo		= prev->orgRow();
	Int smaller = 0;

	// If current view is a	footer,	examine	the	other views	to set my size
	if	(prev->isFooter()) {
		for (PtrListCursor(DaliView) vlc(*viewlist);	vlc; ++vlc)
			if ( ! vlc->isFooter() && vlc->isActive()	
				&& vlc->orgRow() > smaller)
			{
				smaller = fo = 	vlc->orgRow();
				c  		= 		vlc->Width();
				co 		= 		vlc->orgCol();
			}
	}

	// Has the previous	analysis end with an Error ?
	if (fo == ERR) {
		if	(fh)	fh	+= FORIG - 1; // @bruno
		cView->activate(FORIG, CORIG,	FILS - fh ,	COLS, true);
	}
	else
		cView->activate(fo+1,	co,	deskTop->height() -	(fo+2) - fh, c,	true);

	his->clear();
	return;
}

void Editor::addFooterView(DaliView	*view)
{

	Int	c  = ERR;
	Int	co = ERR;

	// If another view is present take its width and starting column
	// for reference
	if (cView) {
		c 	= cView->Width();
		co 	= cView->orgCol();
	}

	//	Is there room for a	tr_new	View ?
	if	( makeRoom(view) ==	ERR )
		return;

	cView->activate(deskTop->height()-footerHeight	+ 1, co,
					footerHeight, c, true);

	cView->setFooter();

	his->clear();
}

void Editor::delView(DaliView *v)
{
	if	(v == *cView) {
		delCurrView();
		return;
	}

	if	(v->exit())	{
		PtrListCursor(DaliView) vlc(*viewlist);
		for (;	vlc; ++vlc)
			if (vlc.value() == v) break;

		assert(vlc);		// We must have	found this view

 		DaliView::deleteView(v);

		vlc.del();
	}
}

void Editor::delCurrView()
	// Deletes current view, setting another view as current if possible.
{
	if	(cView && cView->exit()) {
		DaliView::deleteView(*cView);
		cView.del();
		cView.goToLast();

		if (cView) {
			// If view is inactive, activate it first.
			if(!cView->isActive()) 
				cView->activate(FORIG, CORIG, FILS, COLS, true);
			cView->setCurrent(true);
		} 
		his->clear();
	}
}

void Editor::addScratchView(DaliView *sview, VFPVP addviewFunc)
	//	Sets current view as a scratch view.
	//	If limit reached then deletes the oldest scratch-view.
{
	sview->setScratch();

	if (scratchs++ >= scratchViews) {
		// Search and delete scratch-view

		PtrListCursor(DaliView) vlc(*viewlist);
		for (;	vlc; ++vlc)
			if (vlc.value()->isScratch())
				break;

		assert( vlc->isScratch() );
		delView(*vlc);
	}

	addviewFunc(sview);
}

void Editor::prevFile()
{
	assert(cView);
	
	if (nroActViews() <	2)
		return;

	DaliView *curr = *cView;
	PtrListCursor(DaliView) vlc = cView;

	cView->setCurrent(false);
	--cView;

	cView->setCurrent(true);

	vlc.del();
	for	(vlc.goToFirst(); vlc && !vlc.value()->isActive(); ++vlc)
			;

	vlc.ins(curr);

	his->clear();
}

void Editor::nextFile()
{
	if (nroActViews() <	2)
		return;

	PtrListCursor(DaliView) vlc = cView;

	cView->setCurrent(false);

	for	(vlc.goToFirst(); vlc && ! vlc.value()->isActive();	++vlc)
			;

	assert(vlc);

	setLastView(vlc.value());
	cView->setCurrent(true);

	his->clear();
}

void Editor::selectFile()
{

	String label= editorMsg("SELECT");
	Int ncol	= label.length();
	Int res		= 0, cant, maxSize;

	PtrListCursor(DaliView) vlc(*viewlist);
	for (; vlc ; ++vlc,	res++)
		if (vlc.value()->getFile().length() > ncol)
			ncol = vlc.value()->getFile().length();

	maxSize  = viewlist->size();

	ScrollMenu	msv(label+" ("+toString(maxSize)+")", nameView, 
					res > 10 ?	10 : res, ncol + 4,	7, 25);

	msv.execute();

	switch	(res = msv.currOpt()) {
		case 0:      	// The user selected the active view
		case ERR:     // or canceled the operation.
			break; 
			
		default:

			// Adjust 'res' because views are shown reversed
			 res = maxSize - res - 1;

			// no hay view corriente
			if ((cant = nroActViews()) == 0)	{
				cView.goTo(res);

				cView->activate(FORIG, CORIG, FILS,	COLS, true);
				setLastView(*cView);
                
			} else {
                
                assert(cView.order() != res); // Should be catch in the switch
                
				//if (cView.order() == res) break;

				vlc	= cView;
				vlc.goTo(res);

				DaliView *vprev	= *cView;  // This will	be the previous	view
				DaliView *vlast	= *vlc;	   // This will	be the last	view

				if (vlast->isActive()) {
					vprev->setCurrent(false);
					setLastView(vlast);
					vlast->setCurrent(true);
				}
				else {

					Int f, fo, c, co;

					//	Must deactivate	a view first

					if (cant == maxViews) {

						// Try to deactivate the first active view in the list
						for	(cView.goToFirst();	cView && !cView->isActive(); cView++ )
							;

						if (!(cView->exit())) {
							MsgBox cantPerform(libMsg("ERR"),
									editorMsg("CANTPERFORM"), 7, 40, 6, 20);
							cantPerform.execute();
							break;             
						}							

						// Deactivate it!
						cView->deActivate();

				 		f =	vlast->orgHeight();
				 		fo=	vlast->orgRow();
						c = vlast->orgWidth();
						co= vlast->orgCol();
					}
					else {
						f = vprev->orgHeight() - 1;
						fo= vprev->orgRow() + 1;
						c = vprev->orgWidth();
						co= vprev->orgCol();
					}
					vprev->setCurrent(false);
					setLastView(vlast);
					vlast->activate(fo, co, f,	c, true);
				}
			}
		    his->clear();
			break;
	}

	// It must be the last view in the list

	cView.goToLast();
}

// Diff command implementation

#define DIFF_ADD	1
#define DIFF_DEL	2
#define DIFF_CHG	3

#define DIFF_HISTORY_OFFS	60

class Difference {

// private:

	Int command_;
	Int file1from_;
	Int file1to_;
	Int file2from_;
	Int file2to_;

public:

	Difference(Int c, Int f1from, Int f1to, Int f2from, Int f2to)
	:	command_(c), file1from_(f1from), file1to_(f1to),
		file2from_(f2from), file2to_(f2to)
	{ }

	Int command() const { return command_; }
	Int file1from() const { return file1from_; }
	Int file1to() const { return file1to_; }
	Int file2from() const { return file2from_; }
	Int file2to() const { return file2to_; }
};

declare(PtrArray, Difference);
implement(PtrArray, Difference);

static void splitLines(const String &str, Int &from, Int &to)
{
	Int pos = str.index(",");
	if (pos == ERR) {
		from = to = toInt(str);
	} else {
		from = toInt(str(0, pos));
		to = toInt(str(pos + 1));
	}
}

static void splitHeader(const String &str, Int &cmd, Int &file1from,
		Int &file1to, Int &file2from, Int &file2to)
{
	Int i;
	Int pos = ERR;
	for (i = 0; i < str.length(); ++i) {
		switch (str[i]) {
		case 'a':
		case 'd':
		case 'c':
			pos = i;
			cmd = str[i] == 'a' ? DIFF_ADD
				: str[i] == 'd' ? DIFF_DEL
				: DIFF_CHG;
			break;
		}
	}
	assert(pos != ERR);
	String str1 = str(0, pos);
	String str2 = str(pos + 1);
	splitLines(str1, file1from, file1to);
	if (cmd == DIFF_ADD) {
		++file1from;
		++file1to;
	}
	splitLines(str2, file2from, file2to);
	if (cmd == DIFF_DEL) {
		++file2from;
		++file2to;
	}
}

static void findDiffs(const String &file1, const String &file2,
		PtrArray(Difference) &diffs)
{
	String str;
	Int f1from, f1to, f2from, f2to, dcmd;
	String cmd = Editor::diffCmd;
	cmd += " ";
	cmd += file1;
	cmd += " ";
	cmd += file2;
	InputPipe ip(cmd);
	while (ip) {
		ip >> str;
		if (isdigit(str[0])) {
			splitHeader(str, dcmd, f1from, f1to, f2from, f2to);
			diffs.add(tr_new Difference(dcmd, f1from, f1to, f2from, f2to));
		}
	}
}

bool Editor::diff()
{
	if (nroActViews() < 2) {
		MsgBox msgbox(libMsg("MESSAGE"), editorMsg("NEED2VIEWS"),
				5, 50, 6, 15); 
		msgbox.execute();
		return false;
	}

	bool ret = true;

	PtrListCursor(DaliView) vlc(*viewlist);

	// find the topmost two views
	vlc.goToLast();
	DaliView *views[2];
	Int fils = (FILS + 2) / 2;
	Int rfils = (FILS + 2) % 2;
	Int i;
	for (i = 0; vlc && i < 2; --vlc)
		if (vlc.value()->isActive()) {
			views[i] = vlc.value();
			++i;
		}

	// do we need temporary files?
	String fileNames[2];
	for (i = 0; i < 2; ++i) {
		if (views[i]->isTextMod() || views[i]->isIStream() ||
				views[i]->isAdmin() || views[i]->isPreserve()) {
			fileNames[i] = tempnam(NULL, NULL);
			views[i]->store(fileNames[i]);
		}
	}

	String fname1 = fileNames[0];
	if (fname1.isNull())
		fname1 = views[0]->getFileName();

	String fname2 = fileNames[1];
	if (fname2.isNull())
		fname2 = views[1]->getFileName();

	// put them side by side
	views[0]->chgOrgSize(FORIG, CORIG, fils - 2, COLS);
	views[1]->chgOrgSize(FORIG + fils, CORIG, fils + rfils - 2, COLS);

	// obtain differences between files
	PtrArray(Difference) diffs;
	findDiffs(fname1, fname2, diffs);

	Int pos = 0;

	if (diffs.dim() == 0) {

		// no differences where found

		MsgBox msgbox(libMsg("MESSAGE"), editorMsg("NODIFFS"),
				5, 50, 6, 15);
		msgbox.execute();

		ret = false;

	} else {

		// ok, let the user browse through the differences
		UnChar key;
		bool err;
		Difference *differ;
		Window win((Window::rootWindow()->height()), 0, 1, 
				(Window::rootWindow()->width()), NO_BORDER, 
				NULL_STRING, A_BOLD);

		win.goTo(0, 0);
		Int nlines1 = ERR;
		Int nlines2 = ERR;
		String hlp = editorMsg("DIFFHLP");
		String diffAdd = editorMsg("DIFF_ADD");
		String del = editorMsg("DIFF_DEL");
		String chg = editorMsg("DIFF_CHG");
		String line = editorMsg("LINE");
		String lines = editorMsg("LINES");
		String into = editorMsg("INTO");

		win << hlp;

		do {
			assert(diffs.isInRange(pos));
			differ = diffs.elem(pos);

			win.goTo(0, hlp.length() + 1);
			win.eraseEol();
			nlines2 = ERR;
			switch (differ->command()) {
			case DIFF_ADD:
				win << diffAdd;
				nlines1 = differ->file2to() - differ->file2from() + 1;
				break;
			case DIFF_DEL:
				win << del;
				nlines1 = differ->file1to() - differ->file1from() + 1;
				break;
			case DIFF_CHG:
				win << chg;
				nlines1 = differ->file1to() - differ->file1from() + 1;
				nlines2 = differ->file2to() - differ->file2from() + 1;
				break;
			default:
				assert(0);
				break;
			}
			win << " " << nlines1 << " ";
			win << (nlines1 > 1 ? lines : line);
			if (nlines2 != ERR) {
				win	<< " " << into << " " << nlines2 << " "
					<< (nlines2 > 1 ? lines : line);
			}
			win.goTo(0, DIFF_HISTORY_OFFS);
			win << "(" << pos + 1 << "/" << diffs.dim() << ")";
			win.flush();

			views[1]->goCursor(differ->file2from() - 1, 0, true);
			DaliTextCursor tcTo2 = *(views[1]->getText());
			tcTo2.goTo(differ->file2to() - 1, 0);
			tcTo2.goToCol(tcTo2.value().length());
			views[1]->setAttribute(tcTo2, A_STANDOUT);
			views[1]->redraw();

			views[0]->goCursor(differ->file1from() - 1, 0, true);
			DaliTextCursor tcTo1 = *(views[0]->getText());
			tcTo1.goTo(differ->file1to() - 1, 0);
			tcTo1.goToCol(tcTo1.value().length());
			views[0]->setAttribute(tcTo1, A_STANDOUT);
			views[0]->redraw();

			Window::refresh();

			do {
				err = false;
				key = getKey();
				switch (key) {
				case K_CURS_UP:
					if (pos == 0) {
						err = true;
						beep();
					} else
						--pos;
					break;
				case K_CURS_DOWN:
					if (pos == diffs.dim() - 1) {
						err = true;
						beep();
					} else
						++pos;
					break;
				case K_PAGE_UP:
					if (pos == 0) {
						err = true;
						beep();
					} else {
						pos -= 10;
						if (pos < 0)
							pos = 0;
					}
					break;
				case K_PAGE_DOWN:
					if (pos == diffs.dim() - 1) {
						err = true;
						beep();
					} else {
						pos += 10;
						if (pos >= diffs.dim())
							pos = diffs.dim() - 1;
					}
					break;
				}
			} while (err);
			views[0]->delAttribute(tcTo1, A_STANDOUT);
			views[1]->delAttribute(tcTo2, A_STANDOUT);
		} while (key != K_END);

		views[0]->redraw();
		views[1]->redraw();

		Window::refresh();
	}

	// free difference structure
	for (pos = 0; pos < diffs.dim(); ++pos)
		tr_delete(diffs.elem(pos));

	// remove temporary files if any
	for (i = 0; i < 2; ++i) {
		if (!fileNames[i].isNull())
			unlinkFile(fileNames[i]);
	}

	return ret;
}

void Editor::tileWindows()
	// Implements the tile (or mosaic) feature, if footer windows 
	// are present they are resized and repositioned as well.
{
	Int nav   = nroActViews();
	if (winsTile < nav)
		nav = winsTile;

	Int resto = (nav == 2)	? 0	: nav %	2;
	Int slots = (nav == 2)	? 2	: (winsTile	< nav ?	winsTile : nav ) / 2;
	Int fils  = (FILS + 2)	/ (slots + resto);
	Int cols  = (COLS + 2)	/ 2;
	Int rfils = (FILS + 2)	% (slots + resto);
	Int rcols = (COLS + 2)	% 2;

	PtrListCursor(DaliView) vlc(*viewlist);

	// Caso especial para optimizar el scroll
	if	(nav ==	2) {
		vlc.goToLast();
		for (Int n = 0; vlc && n < 2; --vlc)
			if (vlc.value()->isActive()) {
				vlc.value()->chgOrgSize(
					FORIG + (n	* fils), CORIG,
				 		n == 1 ? fils+rfils-2 : fils-2, COLS);
				n++;
			}
	}
	else {
		vlc.goToLast();

		// Ya que el numero	de ventanas	a ubicar es	impar,
		// dejo	la coriente	para lo	ultimo.
  		if (nav	& 1)  
			--vlc;

		Int i	= 0;  
		for (i =	0; i < slots; i++)
			for	(Int n = 0;	vlc	&& n < 2; --vlc)
				if	(vlc.value()->isActive()) {
					vlc.value()->chgOrgSize(
						FORIG + (i *	fils),
						CORIG + (n *	cols),
						!resto && i == slots-1 ?	fils+rfils-2 : fils-2,
						!resto && i == slots-1 ?	cols+rcols-2 : cols-2
					);
					n++;
				}

		if (resto)
			cView->chgOrgSize(FORIG + (i	* fils)	, CORIG,
								fils + rfils - 2 , COLS);
	}
}

void Editor::cascadeWindows()
	// Implements the cascade feature, 
	// takes care of footer windows if present. 
{
	PtrListCursor(DaliView) vlc(*viewlist);

	//	busco la ultima	view activa
	for (vlc.goToLast(); vlc && !vlc.value()->isActive(); --vlc)
		;

	assert(vlc);

	DaliView::setFullRedraw(false);

	if	(getMode() & DEBUG_MODE) 
		debugger->redrawView();

	Int i 	= 0;
	Int fh	= isFooterActive() ? footerHeight +	2 :	0;
	for (vlc.goToFirst(); vlc ; ++vlc)	{
		DaliView *v =	vlc.value();
		if (v->isActive())
			if (v->isFooter())
				vlc.value()->chgOrgSize(deskTop->height()-footerHeight + 1,
										CORIG,
										footerHeight,
										COLS);
			else	{
				vlc.value()->chgOrgSize(FORIG+i, CORIG,	FILS - i -	fh,	COLS);
				i++;
			}

	}

	DaliView::setFullRedraw();
}

bool Editor::doForAllViews(BFPV	func)
	//	Recorre	la lista de	views y	ejecuta	la funcion recibida	p/cada view
{
	if (viewlist->size() > 0) {
		 DaliView *prev	= *cView;

		PtrListCursor(DaliView) vlc(*viewlist);

		for (vlc.goToFirst();	vlc; ++vlc)
			func(*vlc);

		assert(prev != NULL);
		makeCurrentView(prev);
	}

	return	false;
}

bool Editor::exit()
{            
	// If a debugging session is active ask user to confirm exit action.
	if	(mode &	DEBUG_MODE) {
		MLDBox stopDebugger(libMsg("MESSAGE"), 
								editorMsg("STOPDEBUGGER"), DBOX_YES);
		stopDebugger.execute();
		if (stopDebugger.getOpt() != DBOX_YES) 
			return false;
		else {
		    tr_delete(debugger); // Destroy debugger object                            
			setMode(NULL_MODE); // Restore editor mode
		}                                             
	}		

	bool res =	true;
	for (PtrListCursor(DaliView) vlc(*viewlist); vlc && res; ++vlc)
			res = vlc.value()->exit(false);

	if	(res) return true;

	return	false;
}

//  8	Match pairs
void Editor::addMatchPair(const	String &b, const String	&e)
{
	matchList->add(tr_new	MatchPair(b,e));
}

bool Editor::isInMatchList(const String	&b)
{
	for (PtrListCursor(MatchPair) mpl(*matchList); mpl; ++mpl) {
		if (b	== mpl->begin || b == mpl->end)
			return true;
	}
	return	false;
}

Int	Editor::retMatch(const String &s, String &r)
{
	PtrListCursor(MatchPair) mpl(*matchList);
	for (; mpl; ++mpl) {
		if (s	== mpl->begin) {
			r = mpl->end;
			return FORW_MATCH;
		}
	}

	for (mpl =	*matchList;	mpl; ++mpl)	{
		if (s	== mpl->end) {
			r = mpl->begin;
			return BACK_MATCH;
		}
	}

	r = NULL_STRING;
	return	NO_MATCH;
}

//  9	Source Browser

void Editor::addSourceBrowserEntry(const String	&e,	const String &x1,
		const	String &r1,	const String &x2, const	String &r2,
		const	String &t)
{
	sbrowserList->add(tr_new SourceBrowserEntry(e, x1, r1, x2, r2, t));
}

bool Editor::getSourceBrowserExp(const String &ext,	String &exp1,
		String &rep1,	String &exp2, String &rep2,	String &title)
{
	for (PtrListCursor(SourceBrowserEntry)	sbe(*sbrowserList); sbe; ++sbe) {
		RegExp filePttr(sbe->extension);
		assert((!filePttr) == false);
		
		if ( (filePttr.hasMetaChars() && filePttr.match(ext) )
			|| ext.extName() == sbe->extension )
		{
			exp1	= sbe->expression1;
			rep1	= sbe->replace1;
			exp2	= sbe->expression2;
			rep2	= sbe->replace2;
			title 	= sbe->title;
			return true;
		}
	}
	return	false;
}

#define MAX_GROUPS	17

//  10 File treatment methods
bool Editor::checkPerms(const String &theFile)
	// Check if the file is accessible 
{
	// Implementation commentary:
	//		Opening a file is too expensive in the use of resources,
	//		too many stats and too many checks involved. Specially when 
	//		this is executed within the FileSelector or after it.
	//		Portions of the following code are identical to a similar
	//		method in the FileSelector class.
	//


	Int errorStatus = OK; 	// This is just for debugging, can be removed 
							// in any moment if wanted.
							
	struct stat	st;
	static GidT grpList[MAX_GROUPS];
	static int ngrps = ERR;

	if (ngrps == ERR) {
		grpList[0] = getegid();
		ngrps = 1;

#ifdef HAVE_GETGROUPS
		int ng;
		if ((ng = getgroups(MAX_GROUPS-1, &grpList[1])) != ERR)
			ngrps += ng;
#endif
	}

	// Start the job !!
	if(statFile(theFile, &st)) {

		// No such file or directory
		if (errno == ENOENT) {  
			// Check creation permisions on required directory 
			String theDir = theFile.pathName();

			// If no directory specified, take
			// Dali`s current working directory 
			if (theDir.isNull())
				theDir = dirName;

			// OK, now do the stat
			if (!statFile(theDir, &st)) {
				if ((st.st_mode & S_IFDIR)) {
					// The directory is there, but do we have permision 
					// to create the file in it ?
					if (!canWrite(st.st_mode, st.st_uid, st.st_gid, geteuid(), grpList, ngrps))
						errorStatus = 10;
                    // ^ (are both checks needed ?) v
					if (!canSearch(st.st_mode, st.st_uid, st.st_gid, geteuid(), grpList, ngrps))
						errorStatus = 11;
				}
				else {
					// Not even a directory !!
					errorStatus = 12;
				} 
			}
			else {
				// Some error stating the directory !
				errorStatus = 13;
			}
		} 
		else {
			// Some other error  // errno != ENOENT
			errorStatus = 14;
		} 
	}
	else {
		// The entry exists, but is it a directory ?
		if	((st.st_mode & S_IFDIR)) {
			errorStatus = 20;
		}
		else {
			// Finally we must have the appropiate permisions 
			// to access the file
			if (!canRead(st.st_mode, st.st_uid, st.st_gid, geteuid(), grpList, ngrps))
				errorStatus = 21;
		}
	}


	// Analize final error condition. 
	if (errorStatus != OK) {
		MsgBox errbox(libMsg("ERR"), String(" (")+toString(errorStatus)+")." 
						+ editorMsg("CANTACCESSFILE")
						+ " "+theFile, 
						5, 50, 6, 15
					 );
		errbox.execute();
		return false;
	}

	return true;
}

#include <stdio.h>

bool Editor::openFile(const	String &f)
	// Opens and adds a tr_new file to the editor.
{
	String theFile = findInPath(f ,searchPath);

	if ( checkPerms(theFile) ) {
		WaitMsg pleaseWait(editorMsg("ONEMOMENT"));
		pleaseWait.execute();
		addView(tr_new DaliView( theFile, false));
		return true;
	}

	return false;
}

void Editor::newFile()
{
	addView(tr_new DaliView(newName(), false));
}

void Editor::closeFile()
{
	if	(cView->getFile() == DEBUGGER_VIEW)
		beep();
	else
		delCurrView();
}

bool Editor::closeAll()
{
	//	Close all views	before loading tr_new WorkSpace
	if(! (exit())) {
		MsgBox cantPerform(libMsg("ERR"),
				editorMsg("CANTPERFORM"), 7, 40, 6, 20);
		cantPerform.execute();

		return false;  
	}		

	for (cView	= *viewlist	; cView; ++cView)
		DaliView::deleteView(*cView);

	viewlist->delAll();

	return	true;
}

//  11	OS Commands
Int	Editor::execOSCommand(const	String &cmd)
{
	if (cmd == NULL_STRING) {
		return ERR;
	}		

	addScratchView(tr_new DaliView(true,cmd), addFooterView);

	return 0;
}

Int	Editor::execAdminCommand(const String &cmd,const String &viewname,bool viewOnError)
{             
    // Imposible to continue without a command to execute
	if	(cmd ==	NULL_STRING)
		return ERR;

	//	If viewname	not	supplied, use cmd as view`s	name
 	String viewName; 
 	if	(viewname != NULL_STRING)
 		viewName = viewname;

    // Build pipe, create view and added it to Editor`s view list
	InputPipe aux(cmd);
	addView(tr_new DaliView(aux,viewName));

    // If an error ocurred delete view if required.
	if	(aux.withError() && !viewOnError) {
		delCurrView();                     
		return 0;
	}

	// Get tag now, we may need it later ...
	RegExp tag(noMetaChars(cView->getText()->value()));
    
	String hostFunction; 
    cView->getFunction(hostFunction, false, false);

    // Now try to find tag.
	cView->goToBegOfFile();	              
	
	// Check if we have a valid RegExp ...
	RegExp hostFunctionRX(hostFunction);
	if((!hostFunctionRX) == true) {
		if (cView->findForwText(
					hostFunctionRX, 1, cView->sizeOfText(), true) == ERR) 
		{
			cView->goToBegOfFile();
			return 0;
	    }                         
	}	    

	// If we cannot find the host function, we will have to rely only in
	// the tag expression. 
    if ((!tag) == false)
		cView->findForwText(tag, 1, cView->sizeOfText(), true);

	return	0; // status ? ERR : OK;
}

void Editor::gotoFile(const	String &fileName,const Int errline,	const bool stay)
{            
	DaliView *view = *cView, *orig = *cView;

	// If file not already open, open it
	
	String tfile = findInPath(fileName, searchPath);
				// Note: findInPath() is executed again in openFile(), but
				//      it is not so expensive because the filename will be  
				//      already absolute. Bruno.

	if ((view = DaliView::existText(tfile)) == NULL) {
		if (!openFile(tfile))
			return;
		view = *cView;
	}
	else
		makeCurrentView(view);

	view->goCursor(errline, 0, true);
	view->goFirstNonSp();

	// This is to implement a sort of source-tracing feature. (HOME-y)
	if (stay) {
		view->highlightToken(0,INT_MAX);
		makeCurrentView(orig);
		orig->cursDown();
		orig->redraw();
		orig->refresh();
	}
}

//  12	 Tags
bool Editor::findTag(String	fname, String str, String &file,
		String &schStr, bool &back)
{
	String	aux;
	InputFile inFile(fname);

	if	(!inFile)
		return false;

	String	tmpStr;
	while (inFile >> aux) {
		tmpStr = aux(0, str.length());
		if (str == tmpStr) {
			String sep =	"\t";
			file = aux.field(1, sep);
			schStr = aux.field(2, sep);
			if (schStr.length() > 2)	{
				if (schStr[0] == '?')
					back =	true;
				else
					back =	false;
				schStr = schStr(1, schStr.length()-2);
				return true;
			}
			return false;
		}

		if (str < tmpStr)
			return false;
	}

	return false;
}

struct TagMatch {
	String str;
	String file;
	String expr;
	bool back;
	TagMatch(const String &s, const String &f, const String &e, bool bk)
	: str(s), file(f), expr(e), back(bk) { }
};

declare(PtrArray, TagMatch);

static PtrArray(TagMatch) tlist;
static String lastTagMatch = NULL_STRING;

#define	MAX_HEIGHT	12
#define	MAX_WIDTH	70

static const String	fpReadLine(Int n)
{
	if	(!tlist.isInRange(n)) 
		return HIGH_VALUE;
		
	String f = String(tlist.elem(n)->file.baseFullName(),14);
	f += String(1, C_VERT);
	f += " ";
	f += expandTabs(tlist.elem(n)->str);
    return String(f,MAX_WIDTH-6);
}

static int cmpTagMatch(void	const *a, void const *b)
{
	TagMatch const	*ta	= *(TagMatch const **) a;
	TagMatch const	*tb	= *(TagMatch const **) b;

	if	(ta	== NULL) {
		if (tb ==	NULL)
			return 0;
		else
			return -1;
	} else	if (tb == NULL)
		return 1;

	return	int(::cmp(ta->str, tb->str));
}

static int cmpTagIndexEntry(void const *a, void const *b)
{
	TagIndexEntry const	*ta	= *(TagIndexEntry const **) a;
	TagIndexEntry const	*tb	= *(TagIndexEntry const **) b;

	if	(ta	== NULL) {
		if (tb ==	NULL)
			return 0;
		else
			return -1;
	} else	if (tb == NULL)
		return 1;

	return	int(::cmp(ta->tag, tb->tag));
}

#define TAGIDX ".idx"

TagIndex::TagIndex(const String &file)
{
	idStart();     
	tagIndex = tr_new PtrArray(TagIndexEntry);
	tagfile  = file;
	InputFile indexFile(tagfile+String(TAGIDX));
	if (indexFile) {
		String indexEntry;
		if (indexFile)	
			while (indexFile >> indexEntry) {
				tagIndex->add(tr_new TagIndexEntry(
											indexEntry.field(0,"\t"),
											toInt(indexEntry.field(1,"\t")))
							 );               
			}							
	}
}             
	
TagIndex::~TagIndex() 
{
	idCheck();

	Int n = tagIndex->dim();
	for (Int d = 0; d <	n; d++)
		tr_delete(tagIndex->elem(d));
	
	tr_delete(tagIndex);		// bruno: Added without testing it.
	idStop();
}          
	
Int TagIndex::locateEntry(const String &tag)
{   
	idCheck();
	TagIndexEntry t(tag,ERR);
	
	Int result = tagIndex->find(&t,cmpTagIndexEntry);
    
    if (result == ERR)
    	return 0;                 

    result = result < 0 ? (~result)-1 : result;

    assert(result >= 0);
	
	return tagIndex->elem(result)->offset;
}               

#define	NAME_SEP	"\t"

Int Editor::loadTagIndexes(const String &t)
{         
	tagIndexList = tr_new PtrList(TagIndex);
	for (StrCursor tc(t,":"); tc; ++tc) 
		tagIndexList->add(tr_new TagIndex(tc.value()+"/tags"));
		
	return 0;
}

bool Editor::openTag(const String &str)
{
	String		className,         // Fields read from tagfile
				fileName, 
				schStr;
	String  	theTag	= str,     // The tag to be found
				classFilter,       // Include only methods of this class
				tmpStr,            // Temp && aux vars
				tStr,
				aux;
	String		sep(NAME_SEP);
	bool 		back;
	bool 		ret =	false;
	InputFile 	inFile;
	Int 		n, el;

	if (str != lastTagMatch) {
		// Delete previous tagMatchList
		n = tlist.dim();
		for (el =	0; el <	n; el++)
			tr_delete(tlist.elem(el));
		tlist.reAlloc(0);

		{
			WaitMsg pleaseWait(editorMsg("ONEMOMENT"),"Searching Tags ...");
			pleaseWait.execute();

			if (tagIndexList == NULL)
				loadTagIndexes(daliTags);

			// Split class and method
			if (str.index("::") != ERR) {
				theTag 		= str.field(1, "::");
				classFilter = str.field(0, "::");
			} 
			else theTag = str;	

			RegExp	rexp(theTag);
			if (!rexp) {
				MsgBox invRegExp(libMsg("ERR"), 
						editorMsg("INVREGEXP"), 5, 32, 6, 20);
				(void) invRegExp.execute();
				return false;
			}

			bool hasmeta =  rexp.hasMetaChars();

			for (PtrListCursor(TagIndex) ti(*tagIndexList); ti; ++ti) {
				Int start = ERR; 

				if(!hasmeta) {
					 start = ti->locateEntry(theTag);

					// Tag is not in this tagfile
					if (start == ERR) 
							continue;
				}

				inFile.open(ti->tagFile());    

				// Some error with tagfile, skip it
				if (!inFile)
					continue;

				if(!hasmeta)
					inFile.seek(start);

				while (inFile >> aux) {
					tmpStr = aux.field(0, sep);
					if (!hasmeta && tmpStr > theTag) 
						break;

					if (theTag == tmpStr || (hasmeta && rexp.match(tmpStr)) ) {
						
						// Not wanted
						className =	aux.field(1, sep);
						if (classFilter != NULL_STRING && 
							classFilter	!= className) 
								continue;

						fileName =	aux.field(2, sep);

						// Count tabs to get SearchString position accurately
						Int n1	= aux.index('\t');		  assert(n1 != ERR);
						Int n2	= aux.index('\t', n1+1);  assert(n2 != ERR);
						n1		= aux.index('\t', n2+1);  assert(n1 != ERR);
						schStr	= aux(n1+1,	INT_MAX);
						if	(schStr.length() > 2) {
							back = (schStr[0]	== '?');
							tStr = schStr	= schStr(1,	schStr.length()-2);

							if ((n = tStr.index('('))	!= ERR) {
								if (tStr[0] == '^') {
									tStr = tStr(1, INT_MAX);
									n--;
								}

								if (tStr[tStr.length()-1] ==	'$')
									tStr = tStr(0, tStr.length()-1);

								tStr	= tmpStr + tStr(n, INT_MAX);

								while (tStr[0] == '\t')
									tStr.delChar(0);
							}	else
								tStr	= tmpStr;

							if (!className.isNull())
								tStr = className+"::"+tStr;

						 	tlist.add(tr_new TagMatch(tStr, fileName, 
						 							schStr, back));

						}
					}
				}
			}
			lastTagMatch = str;
		} // END of please	wait Message scope
    }
    
	if	((n	= tlist.dim()) == 0) {
		MsgBox errbox(libMsg("MESSAGE"),
				editorMsg("TAGNOTFOUND"), 5, 40, 6, 20);
		errbox.execute();
	} else	{
		// Hay mas de un match, armo menu
		el = 0;
		if (n > 1) {
			ScrollMenu sm(  String("Tag Match List ")+"("+toString(tlist.dim())+")",
							fpReadLine			,
					 		min(MAX_HEIGHT, n)	,
					 		MAX_WIDTH			,
					 		ERR, 3)			;
					 		
			tlist.sort(cmpTagMatch);
			sm.execute();
			el = sm.currOpt(); // ERR if menu is rejected.
		}
		if (el !=	ERR) {
			TagMatch	*m = tlist.elem(el);
			assert(m != NULL);
			openTaggedFile(m->file, m->expr, m->back);
			ret = true;
		}
	}
	return	ret;
}

void Editor::openTaggedFile(const String &fileName,	const String &searchStr,
		bool back)
{
	//	if file	not	already	open, open it
	DaliView *view;
	String schStr = searchStr;

	if ((view = DaliView::existText(fileName))	== NULL) {
		if (!openFile(fileName))
			return;
		view = *cView;
	}
	else
		makeCurrentView(view);

	// Put filltabs
	schStr = schStr(0,1) + noMetaChars(DaliText::encodeTabs(schStr(1)));

#if 0
	//	Escape meta	chars so they are not treated that way
	Int i = 0;
	while (i <	schStr.length()) {
		if (i	> 0	&& i < schStr.length() - 1 &&
				isMetaChar(schStr[i])) {
			if (i > 0)
				schStr = schStr(0, i)+"\\"+schStr(i, INT_MAX);
			else
				schStr = String("\\")+schStr;
			i++;
		}
		i++;
	}
#endif

	RegExp re(schStr);
	assert((!re) == false);

	if (back) {
		view->goToEndOfFile();
		view->findBackText(re, 1, view->sizeOfText(), false);
	}
	else {
		view->goToBegOfFile();
		view->findForwText(re, 1, view->sizeOfText(), false);
	}
}

//  13	 Manual

static List(String)	sl;

const String fpReadMan(Int n)
{
	ListCursor(String)	tc(sl);
	tc.goTo(n-1);

	if	(tc)
		return tc.value();

	return	HIGH_VALUE;
}

bool Editor::showManualEntry(const String &str)
{
	if	(daliMan != NULL_STRING) {
		String strAux;
		sl.delAll();

		InputPipe aux(daliMan+" "+str);
		while(aux) {
			aux >> strAux;
			sl.add(strAux);
		}
		if (!aux.withError())	{
			TextView tv(fpReadMan, str);
			(void) tv.execute();
			return true;
		}
		return false;
	} else
		return false;
}

//  14	 Command Line

Int Editor::getCmdLine(const String	&lb, String	&value,	StrHistory *history)
{
	//	Create & exec Label
	Label cmdLabel(lb,Window::rootWindow()->height(), 1);
	cmdLabel.display(ERR, ERR);

	//	Create & exec StrCell
	Int length	= lb.length() +	1;
	Int width	= 80 - length;
	StrCell cmdLine(160, width, 0,Window::rootWindow()->height(), 
						length +	3, history);
	
	Int ch = ERR;
	
	if(cmdLine.execute() == ACCEPT) {
		ch = cmdLine.lastKey();
		value = cmdLine.value(); 
	}
	
	return	ch;
}

//  15	 Keywords && Screen	attributes

Int	Editor::keyWordAttrib(const	String &p, const String	&ext)
{
	KeyWord *k	= findKeyWord(p, ext);
	return	k ?	k->type	: ERR;
}

struct ScreenAttribute {
	char *str;
	Int attr;
	char *seq;
};

static ScreenAttribute attrs[] = {
	{ "Bold",			A_BOLD,			"1" },
	{ "Underline",		A_UNDERLINE,	"4" },
	{ "Blink",			A_BLINK,		"5"	},
	{ "Reverse",		A_REVERSE,		"7" },
	{ "Red",			A_RED,			"31" },
	{ "Green",			A_GREEN,		"32" },
	{ "Blue",			A_BLUE,			"34"	},
	{ "Yellow",			A_YELLOW,		"33" },
	{ "Cyan",			A_CYAN,			"36"	},
	{ "Magenta",		A_MAGENTA,		"35"	},
	{ "White",			A_WHITE,		"37" },
	{ "RedBg",			A_RED_BG,		"41;1"	},
	{ "GreenBg",		A_GREEN_BG,		"42" },
	{ "BlueBg",			A_BLUE_BG,		"44"	},
	{ "YellowBg",		A_YELLOW_BG,	"43" },
	{ "CyanBg",			A_CYAN_BG,		"46;1" },
	{ "MagentaBg",		A_MAGENTA_BG,	"45"	},
	{ "WhiteBg",		A_WHITE_BG,		"47" }
};

#define	NATTRS	Int(sizeof(attrs)/sizeof(attrs[0]))

Int Editor::findAttrib(const String	&str)
{
	Int i;
	for (i	= 0; i < NATTRS; i++) {
		if (str == attrs[i].str)
			return attrs[i].attr;
	}
	return	ERR;
}

static const String	fpAttr(Int n)
{
	if	(n < 0 || n	>= NATTRS) return HIGH_VALUE;

	return	attrs[n].str;
}

Int	Editor::attribMenu()
{
	Int n = ERR;
	ScrollMenu	sm(fpAttr, 6, 19);
	if	((sm.execute() != REJECT_EXIT ) && (n = sm.currOpt()) == ERR)
		return ERR;
	return	attrs[n].attr;
}

Int	Editor::sequenceToAttrib(const char	*str, Int &skip)
{
	const char	*backstr = str;
	int l,	i;
	Int cattr = 0;

	//	avanzo para	buscar los atributos
	str +=	sizeof(BEGIN_ATTR)-1;

	forever {
		for (i = 0; i	< NATTRS; i++) {
			if (strncmp(str,	attrs[i].seq, l	= strlen(attrs[i].seq))	== 0) {
				cattr |= attrs[i].attr;
				str	+= l;
				break;
			}
		}
		if (i	== NATTRS) break;
		if (*str == SEPAR_ATTR)
			str++;
		else
			break;
	}

	skip =	str	- backstr;

	//	era	una	secuencia valida
	if	(*str == END_ATTR) {
		skip++;
		return cattr;
	}

	return	ERR;
}

#define	COLOR(a)	((a) &	0x00FF0000)

String Editor::attribToSequence(Int	attr)
{
	String	strseq = BEGIN_ATTR;
	bool hasSomething = false;
	Int i;

	for (i	= 0; i < NATTRS; i++) {

		if (COLOR(attrs[i].attr))	{
			if (COLOR(attr) != COLOR(attrs[i].attr))
				continue;
		}	else if	(!(attr	& attrs[i].attr))
			continue;

		if (hasSomething)	strseq += SEPAR_ATTR;
		hasSomething = true;
		strseq +=	attrs[i].seq;
	}

	if	(strseq	!= BEGIN_ATTR) {
		strseq += END_ATTR;
		return strseq;
	}

	return	NULL_STRING;
}

void Editor::setMode(Int m)
{
	mode =	(m & DRAW_MODE) ? DRAW_MODE :
			(m & DEBUG_MODE)? DEBUG_MODE:
			NULL_MODE;

	if (cView)
		cView->writeStLine();
}


