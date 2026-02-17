/********************************************************************
* Copyright (c) 1993 InterSoft Co.  All Rights Reserved
* THIS IS UNPUBLISHED PROPRIETARY SOURCE CODE OF InterSoft Co.
* The copyright notice above does not evidence any
* actual or intended publication of such source code.
*
* $Id: view.h,v 1.1.1.1 1996/04/16 18:51:44 gustavof Exp $
*
* DESCRIPTION: Class definitions for View type.
*********************************************************************/

#ifndef VIEW_H
#define VIEW_H

#ifdef __GNUC__
#pragma interface
#endif

#include <ifound.h>

#include <winapp/window.h>
#include <ifound/str.h>
#include <ifound/itypes.h>
#include <ifound/msgtable.h>

#include <winapp/text.h>
#include <winapp/textcur.h>

// Tecla para abortar una ventana
#define ABORT_KEY   K_END

#define DEL_CHAR    (mode&CURRENT ? C_D_HORIZ : C_HORIZ)
#define VIEW_BORDER (mode&CURRENT ? DLINE_BORDER : SLINE_BORDER)

#define isSeparator(c) (!(isalnum(c) || c == '_' || c == ':'))

enum ViewMode {
	INSERT	  =	1,
	DSPCTROL  =	2,
	ORGSIZE	  =	4,
	CURRENT	  =	8,
	HELPMODE  =	16,
};                

enum StatusMode {
	ST_OFF	  =	1,
	ST_NORMAL =	2,
	ST_FULL	  =	3,
};

// defines para la linea de status
#define C_MODIF		1
#define D_MODIF		"*"

#define C_MODE		3
#define C_LINECOL	7
#define C_STBLOCK	30

#define C_ADMIN		38
#define D_ADMIN		"Admin"

#define	C_CTROL		50
#define D_CTROL		"Control"

#define	C_HELPMODE	60
#define D_HELPMODE	"Help"

#define D_INS		"Ins"
#define D_OVR		"Ovr"
#define D_STBLOCK	"Block"

struct Decoration 
{                
	Int from;
	Int to;
	Int attr;
	Decoration *next;
	Decoration(Int f, Int l, Int a) { from=f; to=f+l-1; attr=a; next=NULL;}
};
               
class EditableView;               
class FileView;    
class HelpView;
class CmdBinding;

class View : public Window {

protected:

	Text 		*text;		// List of strings being edited
	TextCursor 	*_cursor;	// Cursor of the view
	Int 		mode;		// Current mode of the view
	Int 		stmode;		// Current mode of the statusline
	Int 		ntxt;		// Instance number of the text

	String		label;		// View's label
	Int 		ncols;		// Number of columns of the view
	Int 		nfils;		// Number of rows of the view

	Int 		cfil;		// Row of the cursor
	Int 		ccol;		// Column of the cursor

	Int  		c_orig;		// Upper left origin column
	Int  		f_orig;		// Upper left origin row

	Int 		orgnfil;	// Previous number of rows
	Int 		orgncol;	// Previous number of columns
	Int 		orgfil;		// Previous origin row
	Int 		orgcol;		// Previous origin columns

   	bool 		easyExit_;	// Don't bother analizing before exiting view.
	bool 		dontExit_;	// Don't enable to exit this view.

	const CmdBinding *cmdBinding; // Current command binding 

	static Window *defViewPar;  // Ventana padre de la view

private:
    
	Time exectime;		// Hora en la que se proceso la view 
    bool inProcess;		// Indica si actualmente se esta procesado la dview
    bool processed;		// Indica si ya se leyo el stream 
    
	View *next;						// puntero a la proxima view
	static View *head;				// puntero a la primer view

	static bool fullRedraw;			// Flag que indica si se deben redibujar 
									// las view asociadas al mismo texto

	static MsgTable *msgs;			// Tabla de mensajes de la view

protected:  
	// Metodos que manejan la lista de views
		void insNode();			// inserta una view en la lista
		void delNode();			// borra una view de la lista

    // Metodos para tratar views con el mismo texto
		View *existActText();	// hay otra view con el mismo text y activo
		void addText();			
		void subText();			
 		void updateOtherViews();

    // Devuelven si se debe actualizar la view con respecto al cursor
	   	bool actColOrigin();	
		bool actTopOrigin();	
		bool actBotOrigin();	

	// Metodos que redibujan la view
		void update();
		void redrawLine(TextCursor &, String &ext);
		void actCurrLine(Int val);	// actualiza la linea corriente
		void goToCursor();			// posiciona la view en la pos. del cursor
		void refresh() { Window::refresh(); }

    // Metodos para tratar views con el mismo texto
  		static bool sameFile(const String &f1, const String &f2);

	// Decodifica los tabs y los caracteres especiales
		String decodeTabs(String s);
		String decodeChar(const char c);

	// Rutina para acceder a los mensajes
		static String msg(const String str);

	// Valida y ajusta el rango (from - to)
		void adjustRanges(Int &from, Int &to);

	// se pidio interrumpir el procesamiento de la ventana
		bool isAborted();                         

	// Used if text has not already been constructed by derived classes
		static Text *constructText(const String &f);
		static Text *constructText(const String &cmd, bool abortable);

    // Avoid accidental use
		void operator=(const View&) { assert(0); }
		View(const View&) { assert(0); }

public:

	// Constructor en base a un archivo
		View(const String &file, Text *t = NULL, TextCursor *tc = NULL);

	// Constructor en base a un InputPipe
		View(const String &cmd, bool abortable, 
			Text *t = NULL, TextCursor *tc = NULL);
				
		virtual ~View();

	// Access to EditableText && EditableTextCursor	
     	virtual EditableText		*edText() { idCheck(); return NULL; } 
		virtual EditableTextCursor	*edCursor() { idCheck(); return NULL; } 

	// Access to EditableView && FileView
		virtual EditableView *edView() { idCheck(); return NULL; }
		virtual FileView	*fileView() { idCheck(); return NULL; }
		virtual HelpView	*helpView() { idCheck(); return NULL; }
		
	// Movimiento de cursor
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

	// Metodos de busqueda	
		void findNextWord(Int n, Int &row, Int &col);
		void findPrevWord(Int n, Int &row, Int &col);
		void findTokenBounds(Int &from, Int &to) const;
		String getToken();
		void highlightToken(Int from, Int to);
    
    // Metodos para el handler de keywords
    	virtual Int keyWordAttrib(const	String &p, const String	&ext);
		virtual Decoration *getDecoration(const char *pc, String &ext);

	// Devuelve si se puede salir de la ventana
		virtual bool exit(bool igntxt = true);
    
    // Devuelve el tipo del texto
	bool isTextType(TextType type);
	bool isTextType(int type);

	// Public static methods
	   	// Setea el padre de la ventana
			static void setViewPar(Window *par);

		// Setea la tabla de mensajes a utilizar
			static void setMessages(MsgTable &m)	{ msgs = &m; }

		// Setea el tamaño del TAB
		    static void tabSize(Int v);
	    
		// Devuelve la view con el nombre dado si existe, NULL si no existe
			static View *existText(const String &name);

		// Metodo que redibuja la view
			static void setFullRedraw(const bool &b = true);

	    // Method to delete a view, takes care of multiple instances of text
			static void deleteView(View *v);
	
	// Atributos de la view
		Int orgCol() 		{ idCheck();  return orgcol;  }
		Int orgRow() 		{ idCheck();  return orgfil;  }
		Int orgHeight() 	{ idCheck();  return orgnfil; }
		Int orgWidth() 		{ idCheck();  return orgncol; }     
		
		// Cual es la diferencia con los metodos de arriba ??
		Int getHeight() 	{ idCheck();  return nfils; }       
		Int getWidth() 		{ idCheck();  return ncols; }
		Int getRow() const	{ idCheck();  return _cursor->order(); }
		Int getCol() const	{ idCheck();  return _cursor->column(); }
		void easyExit(bool tv = true) { idCheck(); easyExit_ = tv; }
		void dontExit(bool tv = true) { idCheck(); dontExit_ = tv; }
    
    // Methods that return the cursors of the view.
    	TextCursor *getCursor() { idCheck(); assert(_cursor); return _cursor;}
    	Text *getText()   { idCheck(); assert(text); return text;}

	// View's label
		bool chgLabel(const String &vname);
		const String getLabel(const bool addTxtInstance = false);

	// Text's name (filename, command, etc.)
		const String getTextName();

	// Modo de la view
		bool	isCurrent() 		 { idCheck();  return mode&CURRENT; }
		bool	inOrgSize() 		 { idCheck();  return mode&ORGSIZE; }
		void	chgCtrolMode();
		void	chgHelpMode();

    // Metodos que modifican las dimensiones y la ubicacion de la ventana
		bool resize();
		bool move();
		void setOrgSize();
		void chgOrgSize(Int fo, Int co, Int cr, Int cc);
		void setSize(Int fo, Int co, Int cr, Int cc);

	// Metodos para activar y desactivar la view
		virtual void activate(Int forg, Int corg, 
							  Int fil, Int col, bool curr = false);
		virtual void deActivate();
		bool isActive() { idCheck(); return !(Window::hidden()); }

    // Metodo para colocar corriente una view
		bool setCurrent(bool f);

	// Metodos para (re)leer una view de un archivo
		bool load(String f);
		bool reload();

	// Redibujan la view
		void redraw();			// Toda la view
		void redrawCurrLine();	// Solo la linea corriente
		
		void appendLine(const String &text);

		// Escribe la linea de status
		virtual void writeStLine(const String &s = NULL_STRING); 
		virtual void _writeStLine(); 

	// Atributos del texto correspondiente a la view
		Int  sizeOfText();
        void clear();
        
	// Ingreso de caracter
		UnChar getKey(void);

		View &operator >> (UnChar &c)	
			{ idCheck(); Window::operator>>(c); return *this; }

		void beep()	
			{ idCheck(); Window::beep(); }

	// Command binding treatment
		void setCmdBinding( const CmdBinding *cbind)	
			{ idCheck(); cmdBinding = cbind; }		

		const CmdBinding *getCmdBinding(void) const
			{ idCheck(); return cmdBinding; }
};

#endif
