/********************************************************************
* Copyright (c) 1991 InterSoft Co.  All Rights Reserved
* THIS IS UNPUBLISHED PROPRIETARY SOURCE CODE OF InterSoft Co.
* The copyright notice above does not evidence any
* actual or intended publication of such source code.
*
* $Id: editor.h,v 1.6 2002/03/18 19:32:17 hardaiz Exp $
*
* DESCRIPTION: Class definitions for editor type.
*********************************************************************/

#ifndef EDITOR_H
#define EDITOR_H

#include <ifound.h>

#ifdef __GNUC__
#pragma interface
#endif

#ifdef ISDEBUG
#	include <ifound/iddebug.h>
#endif

#include <sys/types.h>
extern "C" {
	DLLIMPORT TIMERETTYPE time(TIMEPARAM1TYPE);
}

class String;
class History;
class DaliSensor;
class MsgTable;
class StrCell;

#ifdef USES_OLD_HELP_MENU_STYLE
class EditorHelp; 
#endif

#include <dview.h>
#include <winapp/cell.h>
#include <ifound/oparser.h>

// For doForAllViews method
typedef	bool (*BFPV) (DaliView*);
typedef void (*VFPVP) (DaliView*);

// Dali version * 1000
#define	DALI_VERSION	1508

// dimensiones de las views
#define	FORIG		1
#define CORIG		1
#define FILS	 	(Window::rootWindow()->height()-4)
#define DEB_FILS 	(Window::rootWindow()->height()-10)
#define COLS		(Window::rootWindow()->width()-2)

// Nivel de Function Sensor
#define FS_OFF		0
#define FS_ON		1
#define FS_NOWON    2

// maxima cantidad de views activas
#define NVIEWS	5

// define's para el match

#define NO_MATCH	0
#define FORW_MATCH	1
#define BACK_MATCH	2

class MatchPair {
public:
	String begin;
	String end;
	MatchPair(const String &b, const String &e) : begin(b), end(e) { }
};

class SourceBrowserEntry {

public:

	String extension;
	String expression1;
	String replace1;
	String expression2;
	String replace2;
	String title;

	SourceBrowserEntry(const String &e, const String &x1,
			const String &rep1 = NULL_STRING,
			const String &x2 = NULL_STRING, 
			const String &rep2 = NULL_STRING,
			const String &t = NULL_STRING)
	:	extension(e), expression1(x1), replace1(rep1),
		expression2(x2), replace2(rep2), title(t)
	{ }
};

class KeyWord {

public:

	String kw;
	String ext;
	Int type;

	KeyWord(const String &k, const String &e = NULL_STRING,
			Int t = INT_NULL)
	: kw(k), ext(e), type(t)
	{ }
};

// modos del editor
#define NULL_MODE 	0
#define DRAW_MODE 	1
#define DEBUG_MODE 	2

#define DEBUGGER_VIEW	"Debugger"

// Directory to use for temporary files ($HOME/PRESERVE_DIR)
#define PRESERVE_DIR	"preserve"

// Dali subdir
#define DALIDIR			".dali"

// Configuration file
#define DALIRC			"dalirc"

// History file
#define HISTFILE		"history"

// Keyword file
#define KEYWORDFILE		"keywords"

// Workspace extension
#define WORKSPACE_EXT	".wsp"

// Desktop file
#define DESKTOPFILE		"default"

// Keywords defines
#define KW_SEP			":"
#define KW_EXT_SEP		","
#define KW_WORD_SEP     " "
#define KW_HT_SIZE		2048

// macros para los atributos
#define BEGIN_ATTR		"\033["
#define ATTR_NORMAL		"\033[0m"
#define SEPAR_ATTR		';'
#define END_ATTR		'm'

class StrHistory;
class gtype(PtrArray, TagIndexEntry);
class gtype(PtrList, TagIndex);
class gtype(PtrList, MatchPair);
class gtype(PtrList, SourceBrowserEntry);
class gtype(PtrList, DaliView);
class gtype(PtrListCursor, DaliView);
class gtype(List, String);
class Debugger;

#define cView (*(Editor::_cview))

class Editor {

	friend class dali;

private:

	static Window *deskTop;
	static Int ord;
	static Int mode;
	static MsgTable *_editorMsg;
	static bool useCmdLine;        
	
	// Option Parser
	static bool op_noDalirc;
	static bool op_printDir;
	static String op_loadWsp;
	static String op_findTag;
	static String op_execOSCmd;

	static bool init();			// inicilizacion del editor
	static bool confirmExit();		
	static void clear();			// libera todos los recursos
	
	static bool readPreserved();	// Read preserved files that are not lock
	static void readKeyWords();		// Reads keywords to highlight (Decorate)

	static String newName();		// Genera nombre para un archivo sin nombre

	static bool checkPerms(const String &theFile); // Check permisions of file
	static Int  makeRoom(DaliView *v);			  // Makes room for a tr_new View
 
  	static bool findTag(String dir, String str, String &file,
						String &schStr, bool &back);
  
  	static void openTaggedFile(	const String &fileName,
								const String &searchStr,
								bool back);
  
  	static void buildDeskTopLine(DaliView *v, Int iac);  // Desktop-related
	static void	setLastView(DaliView *v);

    void operator=(const Editor &);         

public:
    Editor(const Editor&) ;

#ifdef USES_OLD_HELP_MENU_STYLE
	static EditorHelp 	*editorHelp;	// help del editor
#endif
    
	static History *his;			// history de los comandos
	static DaliSensor *sen;			// para la lectura de eventos
	static Debugger *debugger;		// Debugger object
	static time_t lastUpdate;		// ultima hora a la que grabe
                       
	static gtype(List, String) *clipboard; 
	static gtype(PtrList,DaliView) *viewlist;
	static gtype(PtrList, MatchPair) *matchList;
	static gtype(PtrList, SourceBrowserEntry) *sbrowserList;
  	static gtype(PtrList, TagIndex) *tagIndexList;
	static gtype(PtrListCursor, DaliView) *_cview;  

	// String Histories
	static StrHistory *greps;				// For grep Command
	static StrHistory *grepFiles;			// For grep Command
	static StrHistory *expressions;			// For Token/Expression Browser                       	                              
	static StrHistory *adminComments;		// For Check In command                           
	static StrHistory *osCommands;
	static StrHistory *wmCommands;
	static StrHistory *files;
	static StrHistory *filesInserted;
	static StrHistory *workspaces;
	static StrHistory *dirs;
    static StrHistory *tags;
    static StrHistory *filters;
    static StrHistory *finds;
    static StrHistory *replaces;
    static StrHistory *helps;
    static StrHistory *debugProgs;
    static StrHistory *debugEntries;
    static StrHistory *projects;

    static String desktopFiles;
    static String desktopCwd;

	static UnChar getKey();
	static void beep();

	static void readDaliRc();		// Reads dali's configuration file

	// Metodos para agregar y borrar views de la lista de views
	static void addView(DaliView *v);
	static void addFooterView(DaliView *v);
	static void addScratchView(DaliView *sview,VFPVP addviewFunc);
	static void delView(DaliView *v);
	static void delCurrView();

    // Metodos que recorren/ordenan la lista de Views
	static void prevFile();
	static void nextFile();
	static void selectFile();
	static void tileWindows();
	static void cascadeWindows();
    static void makeCurrentView(DaliView *view);// Finds and makes current a view

	// Metodos que agregan/sacan views de la lista de Views 
	static bool openFile(const String &f);
	static void newFile();
	static void closeFile();
    static bool closeAll();
	static void loadWorkSpace(const String &wspfile, bool additive = false);
    static void saveWorkSpace(const String &wspfile);
	static void gotoFile(const String &fileName,const Int errline,
						 const bool stay = false);
	static bool openTag(const String &str);
	static Int execOSCommand(const String &cmd);
	// DALI-STANDALONE: Removed execAdminCommand (RCS/SCCS only)
    
	static bool doForAllViews(BFPV func);

	// Loads tag indexes 
	static Int loadTagIndexes(const String &t);
	
	// manejo de los modos del editor
	static void setMode(Int m);
	static Int getMode() { return mode; }
	
	static bool showManualEntry(const String &str);

	// Numero de views activas   
	static Int nroActViews(bool inc_stream = true);	

	// Is there any footer-type view in the view list? 
	static bool isFooterActive(); 

	static void execute();
	static const String nameView(Int i);		
	static String ibFiles();		
	static String editorMsg(const char *);
	static bool exit();
	static bool diff();

	// Metodos para tratar la lista de matches
	static Int  retMatch(const String &s, String &r);
	static void addMatchPair(const String &b, const String &e);
	static bool isInMatchList(const String &b);

	// Metodos para tratar la lista de source browser entries
	static void addSourceBrowserEntry(	const String &e, const String &x1,
										const String &r1 = NULL_STRING,
										const String &x2 = NULL_STRING,
										const String &r2 = NULL_STRING,
										const String &t  = NULL_STRING);
										
	static bool getSourceBrowserExp(const String &ext, String &exp1,
									String &rep1, String &exp2, 
									String &rep2, String &title);

  	// .dalirc options
	static String dirName;      
	static String daliDir; // Where should Dali find & put his stuff
	static String daliTags;
	static String daliMan;
	static String diffCmd;
	static String buildCmd;
	static String buildFlags;
	static String searchPath;
	static String lastWsp;
	static String userMode;    // expert - normal - beginner
	static bool deskTopChar;
	static bool saveDesktop;
	static bool autoIndent;
	static Int maxViews;
	static Int scratchViews;   // Maximo numero de views auto-removibles
	static Int scratchs;       // Cantidad actual de scratchViews
	static Int winsTile;
	static Int footerHeight;
	static Int autoSaveFreq_;
	static Int lhis;
    static Int functionSensor;
    static bool funcExpBrowser;

	static void autoSaveFreq(Int v) { autoSaveFreq_ = v; }
	static Int autoSaveFreq() { return autoSaveFreq_; }
	static void historySize(Int v);
	static void tabSize(Int v);

	// Command line
	static bool isCmdLine()	{ return useCmdLine; }
	static void setCmdLine(bool b) { useCmdLine = b; }
	static Int getCmdLine(const String &lb, String &value, 
							StrHistory *his = NULL);

    // Metodos para manejar los atributos
	static Int attribMenu();
	static Int findAttrib(const String &str);
	static String attribToSequence(Int attr);
	static Int sequenceToAttrib(const char *str, Int &skip);
	static Int keyWordAttrib(const String &s, const String &ext);
};

class TagIndexEntry  
{  
public:
	String tag;
	Int	offset;
	TagIndexEntry(const String &t, const Int &o) : tag(t), offset(o) { }
};

class TagIndex
	// Indice de tags
{
	useIdCheck(TagIndex_id);
private:
	gtype(PtrArray,TagIndexEntry) *tagIndex;
	String tagfile;
public:
	TagIndex(const String &tfile); 
	~TagIndex();
	Int locateEntry(const String &tag);
	String tagFile() { idCheck(); return tagfile; }
};

#endif
