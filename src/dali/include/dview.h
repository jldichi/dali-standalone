/********************************************************************
* Copyright (c) 1991 InterSoft Co.  All Rights Reserved
* THIS IS UNPUBLISHED PROPRIETARY SOURCE CODE OF InterSoft Co.
* The copyright notice above does not evidence any
* actual or intended publication of such source code.
*
* $Id: dview.h,v 1.1.1.1 1996/04/16 18:52:53 gustavof Exp $
*
* DESCRIPTION
* Class definitions for dview type.
*********************************************************************/

#ifndef DALIVIEW_H
#define DALIVIEW_H

#ifdef __GNUC__
#pragma interface
#endif

#ifdef ISDEBUG
#	include <ifound/iddebug.h>
#endif

#include <winapp/window.h>
#include <ifound/list.h>
#include <dtext.h>
#include <ifound/rexp.h>

// defines de la status line

// modo de la linea de status
#define ST_OFF		1
#define ST_NORMAL	2
#define ST_FULL		3

// string para la descripcion de las opciones
#define D_MODIF		"*"
#define D_INS		"INS"
#define D_OVR		"OVR"
#define D_STBLOCK	msg("BLOCK")
#define D_RDONLY	msg("RDONLY")
#define D_LOCKED	msg("LOCKED")
#define D_ADMIN	    msg("ADMIN")
#define D_PRESERVE	"Preserve"
#define D_CTROL		msg("CONTROL")
#define D_TROFF		"TROFF"
#define D_TEX		"TeX"
#define D_DRAW1     msg("DRAW")+" 1";
#define D_DRAW2     msg("DRAW")+" 2";
#define D_DRAW3     msg("DRAW")+" 3";
#define D_DRAWDEL   msg("DRAW")+" "+msg("DRAWDELETE");
#define D_DRAWMOVE  msg("DRAW")+" "+msg("DRAWMOVE");

// desplazamiento de las opciones 
#define C_MODIF		1
#define C_MODE		3
#define C_LINECOL	7
#define C_ABORTMSG  15
#define C_STBLOCK	30
#define C_RDONLY	38
#define C_LOCKED	C_RDONLY
#define C_CTROL		50     
#define C_TROFF		58     
#define C_TEX		58     
#define C_DRAW		64

// modos draw de la dview
#define DRAWMODE1   1
#define DRAWMODE2   2
#define DRAWMODE3   3
#define DRAWDEL     4
#define DRAWMOVE    5

// modo de la dview
#define INSERT		0x01
#define DSPCTROL	0x02
#define ORGSIZE		0x04
#define CURRENT		0x08

// caracter para blanquear la linea de status
#define DEL_CHAR    (mode&CURRENT ? C_D_HORIZ : C_HORIZ)
#define VIEW_BORDER (mode&CURRENT ? DLINE_BORDER : SLINE_BORDER)

// Tecla para abortar una ventana
#define ABORT_KEY   K_END

#define isSeparator(c) (!(isalnum(c) || c == '_' || c == ':'))

class MsgTable;
class List(String);
class DebugBreakSet;
class DebugBreakClear;

//
// Proximamente derivara de EdView
//
class DaliView : private Window 
{
	useIdCheck(DaliView_id);

	friend class Command;
	friend class Debugger;

private:
	
	DaliText *text;				// lista de strings editada
	Int  c_orig;			// origen superior izquierdo
	Int  f_orig;			// origen superior izquierdo
	DaliTextCursor *_cursor;	// cursor de la dview

	Int nfils;				// cantidad de filas de la dview
	Int ncols;				// cantidad de columnas de la dview

	Int orgnfil;			// numero de filas 
	Int orgncol;			// numero de columnas
	Int orgfil;				// fila origen de la dview
	Int orgcol;				// columna origen de la dview

	Int cfil;				// fila del cursor
	Int ccol;				// col. del cursor

	Int ntxt;				// numero de instancia del texto

	Int mode;				// modo en el que esta la dview
	Int stmode;				// modo de la linea de status
    Int drawMode;			// mode de dibujo en que se encuentra la dview
    
	DaliTextCursor *_block;		// marca el pricipio de bloque
	DaliTextCursor *_mark;		// posicion marcada 
    
    bool footer;            // Indica si la dview es tipo footer
    bool scratch;           // Indica si la dview es tipo scratch
    bool inProcess;			// Indica si actualmente se esta procesado la dview
    bool processed;			// Indica si ya se leyo el stream 
    Time exectime;			// Hora en la que cargo el dview (IStream)
    static bool fullRedraw;	// Indica si se debe hacer redraw de todas 
    						// las views asociadas al mismo texto
    
   	bool actColOrigin();	// act. la col origen c/respecto a c_cursor
	bool actTopOrigin();	// act. la row origen c/respecto a cursor
	bool actBotOrigin();	// act. la row origen c/respecto a cursor

	void findNextWord(Int n, Int &row, Int &col);
							// posicion de la n-esima proxima palabra
	void findPrevWord(Int n, Int &row, Int &col);
							// posicion de la n-esima proxima palabra

	void subText();			// corrije el nombre de las views con = text
	void addText();			// corrije el nombre de las views con = text

	// metodos para saber si estoy sobre un texto ya editado
	static bool sameFile(const String &f1, const String &f2);

	DaliView *existActText();	// hay otra view con el mismo text y activo

	void actCurrLine(Int val);	// actualiza la linea corriente

	void goToCursor();		// posiciona la view en la pos. del cursor

	// cambia el contiendo del cursor de longitud len por newtext
	void chgStrCursor(Int len, const String newtext); 

	// reemplaza los caracteres de control dependiendo del 
	// modo de la view (DSPCTROL o NO)
	String decodeTabs(String s);
	String decodeChar(const char c);
	void   redrawLine(DaliTextCursor &, String &ext);
    
    void   clearUpperLine();      		  // Clears the area where the
    							  		  // function name is displayed

	// realiza el redraw de la view
	void update();
	
	// redibuja las otras views asociadas el texto
	void updateOtherViews();

	// Tabla de mensajes de la view
	static MsgTable *msgs;
	
	// Rutina para acceder a los mensajes
	static String msg(const String str);

	// lista de view
	static DaliView *head;		// puntero a la primer view
	DaliView *next;				// puntero a la proxima view
	void insNode();			// inserta una view en la lista
	void delNode();			// borra una view de la lista

	// activa y desactiva el bloque
	void blockOn (bool rw = true);
	void blockOff(bool rw = true);

	// determines the bounds of the token under the cursor
	void findTokenBounds(Int &from, Int &to) const;

	// Makes the from and to ranges valid
	void adjustRanges(Int &from, Int &to);

	static Window *defViewPar;
	
	void operator=(const DaliView&);
              DaliView(const DaliView&); 

public:

	// constructores 
	DaliView(String f, bool isTemp);
	DaliView(InputStream &is, const String &cmd);
	DaliView(const bool proc, const String &cmd);
			
	~DaliView();
    
	// destructor que borra los archivos temporarios del texto
	static void deleteView(DaliView *v);
	
	// atributos de la view
	Int orgCol() 		{ idCheck(); return orgcol;  }
	Int orgRow() 		{ idCheck(); return orgfil;  }
	Int orgHeight() 	{ idCheck(); return orgnfil; }
	Int orgWidth() 		{ idCheck(); return orgncol; }

	Int Height() 		{ idCheck(); return nfils; } 
	Int Width() 		{ idCheck(); return ncols; }

	// determina si existe una view con el nombre dado
	static DaliView *existText(const String &name);

	// resize de la view
	void setOrgSize();
	void chgOrgSize(Int fo, Int co, Int cr, Int cc);
	void setSize(Int fo, Int co, Int cr, Int cc);

	// metodos para activar y desactivar la view
	void activate(Int forg, Int corg, Int fil, Int col, bool curr = false);
	void deActivate();
	bool isActive() 				{ idCheck(); return !(Window::hidden()); }

    // Metodos para setear el footer y obtener el estado actual
    void setFooter(bool f = true) 	{ idCheck(); footer = f; }
    bool isFooter()      			{ idCheck(); return footer; }

    void setScratch(bool f = true) 	{ idCheck(); scratch = f; }
    bool isScratch()      			{ idCheck(); return scratch; }
    
    // metodo para colocar corriente una view
	bool setCurrent(bool f);

	// metodos para load y store
	bool load(String f);
	bool reload();
	bool store();
	void store(const String &fname);

	// metodo para guardar temporalmente el archivo
	void rememberFile();

	// redibujan
	static void setFullRedraw(const bool b = true);
	void redraw();
	void redrawCurrLine();	// Solo la linea corriente

	// inserta file
	Int insertFile(const String &fname);

	// modo de la view
	bool  inInsMode() 		 { idCheck(); return mode&INSERT; }
	bool  inCtrolMode() 	 { idCheck(); return mode&DSPCTROL; }
	bool  inOrgSize() 		 { idCheck(); return mode&ORGSIZE; }
	void  chgInsMode(bool redrw = true);
	void  chgCtrolMode();
	bool  isCurrent() 		 { idCheck(); return mode&CURRENT; }
    
    // Metodos para setear y obtener el mode de dibujo de la view
    // NOTA: Ahora se utilizan solo en el Metodo writeStLine
    void  setDrawMode(Int m) { idCheck(); drawMode = m; }
	Int   getDrawMode() 	 { idCheck(); return drawMode; }

    
	// atributos del texto correspondiente a la view
	Int  sizeOfText();
	bool isTextMod();
	bool isTroff();
	bool isTeX();

	// This member function is used by the debugger (temporary?)
	void setDisplayMode()		 {idCheck(); text->setDisplayMode(); } 

	// metodos que cambian el contenido del DaliText
	bool insChar(UnChar c);
	bool backSpace();
	bool backTab();
	bool tab();
	bool delLine();
	bool delWord();
	bool delEndOfLine();
	void appendLine(const String &text);
	bool insLine(const String &text);
	void setLine(const String &text);
	bool delChar();
	bool joinLines();
	Int  insNewLine(bool deflt = true);
	bool shiftRight();
	bool shiftRightLine(DaliTextCursor &tc);
	bool shiftLeft(bool &symetric);
	bool shiftLeftLine(DaliTextCursor &tc, bool &blank);
	void insertText(List(String) &l);
	void delRegion(DaliTextCursor &tc, bool compress = true);
	void getRegion(List(String) &l, DaliTextCursor &tc);
	void setTroff();
	void setTeX();
	void setAdmin();
	static void tabSize(Int v);

	/*
		Parameters:

		re:			regular expression to use for the search
		newtext:	text to replace with, it is expanded to the
					actually replaced string (considering \1, etc.)
		oldtext:	text that was found and replaced
		filfrom:	start row
		filto:		end row
		conf:		wether it should ask for confirmation
		dontDoIt:	don't replace, even if conf was true
		didIt:		returns true if the replace was done
	*/
	Int replForwText(RegExp& re, String& newtext, String& oldtext,
							Int filfrom, Int filto, bool &conf,
							bool& didIt, bool dontDoIt = false);

	void tokenCompletion();

	// ingreso de caracter
	UnChar getKey();

	// movimiento de cursor
	bool goCursor (Int fil, Int col = 0, bool center = false);
	bool cursDown (Int i=1);
	bool cursUp   (Int i=1);
	bool cursLeft (Int i=1, bool wstln = false);
	bool cursRight(Int i=1, bool wstln = false);
	void pageDown (Int i=1);
	void pageUp   (Int i=1);
	bool wordRight(Int i=1);
	bool wordLeft (Int i=1);
	bool goFirstNonSp();
	void goToBegOfFile();
	void goToEndOfFile();
	void goToBegOfScreen();
	void goToMiddleOfScreen();
	void goToEndOfScreen();
	void goToForwFuncBegin();
	void goToBackFuncBegin();
   //bool functionBrowser(Int &line, Int &col);
	bool tokenBrowser(Int &line, Int &col, bool findFunc, String &target1,
						const String &rep1 		= NULL_STRING,
						const String &target2 	= NULL_STRING,
						const String &rep2 		= NULL_STRING,
						const String &title 	= NULL_STRING);


	// Match de caracteres % del vi
	bool forwMatch(const String &b, const String &e, Int stack = 1);
	bool backMatch(const String &e, const String &b, Int stack = 1);
    
	// busqueda y sustitucion
	Int findBackText(RegExp& re, Int filfrom, Int filto, bool addcol);
					  
	Int findForwText(RegExp& re, Int filfrom, Int filto, 
					 bool addcol, bool usecursor = true);

	String incSearch();                   
	
	// Function name that holds cursor  
    bool getFunction(String &function, 
    				bool optimize = true, 
    				bool stripType = true,
    				Int startLine = ERR); 
                            
	// retorna el nombre del archivo editado (con [n] si esta repetido)
	const String getFile();

	// retorna el nombre del archivo editado (sin [n])
	const String getFileName();

	// setea el nombre del archido editado
	bool chgFileName(const String &fileName); // Used in saveAsFile
	bool chgViewName(const String &fileName); // Used in changeDir
	void addPath();							  // Adds path to filename


	bool exit(bool igntxt = true);

	// cfront 2.0 doesn't like this two
		void beep() {idCheck(); Window::beep(); }
		void refresh() {idCheck(); Window::refresh(); }

	DaliView &operator >> (UnChar &c)
		{idCheck(); Window::operator>>(c); return *this; }

	// escribe la linea de status
	void writeStLine(const String &s = NULL_STRING);
    
	// Si es o no read only
	bool isRdOnly();

	// Si esta lockeado
	bool isLocked();

	// es un stream
	bool isIStream();                         

	// es un Admin
	bool isAdmin();                         

	// es Preserve
	bool isPreserve();                         

	// se pidio interrumpir el procesamiento de la ventana
	bool isAborted();                         

	const String &lockOwner();

	// para setear el directorio a usar para preservar
	static void setTempDir(const String &s);

	// para setear la tabla de mensajes a utilizar
	static void setMessages(MsgTable &m) { msgs = &m; }

	// para tomar el directorio preserve
	static const String &getTempDir();

	// rutinas para el manejo de atributos
	void setAttribute(DaliTextCursor &tc, Int attr);
	void delAttribute(DaliTextCursor &tc, Int attr);

	// rutinas para el manejo de bloques
	bool beginBlock();
	void setBlock(DaliTextCursor &tc);
	DaliTextCursor &getBlock();
	void delBlock(bool rw=true);
	bool hasBlock();

	bool resize();
	bool move();
	String getToken();
	void highlightToken(Int from, Int to);
    
    // texto
    DaliTextCursor *getText() { idCheck(); return _cursor; }
    
    // Analiza contexto, devuelve archivo, y si es posible no. de linea
	String parseFileToken(Int &line); 
	
	// Cursor position
	Int getRow() const	{idCheck(); return _cursor->order(); }
	Int getCol() const	{idCheck(); return _cursor->column(); }
	
	// Marca en el texto
    void setMark(const Int mo, const Int mc);
    void setMark();

    void goMark(); 
	Int getMarkRow() const	{idCheck(); return _mark->order(); }
	Int getMarkCol() const	{idCheck(); return _mark->column(); }

	static void setViewPar(Window *par);

};

#endif
