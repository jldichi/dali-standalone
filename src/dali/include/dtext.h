/********************************************************************
* Copyright (c) 1991 InterSoft Co.  All Rights Reserved
* THIS IS UNPUBLISHED PROPRIETARY SOURCE CODE OF InterSoft Co.
* The copyright notice above does not evidence any
* actual or intended publication of such source code.
*
* $Id: dtext.h,v 1.5 2007/04/20 21:03:02 hardaiz Exp $
*
* DESCRIPTION: Class definitions for DaliText class.
*********************************************************************/

#ifndef DTEXT_H
#define DTEXT_H

#ifdef __GNUC__
#pragma interface
#endif

#include <ifound/list.h>
#include <local/osfile.h>
#include <local/istream.h>
#include <ifound/str.h>

#ifdef ISDEBUG
#	include <ifound/iddebug.h>
#endif

#include <winapp/strlist.h>

// manejo de tabs

// caracter para relleno de tab
#define FILL_TAB	char(31)

// cantidad de FILL_TABs a insertar
#define NFILLTAB(x)	(DaliText::tabSize()-((x)%DaliText::tabSize())-1)

// defino mi isspace
#define	isSpace(c)	(isspace(c) || (c) == FILL_TAB)

// permiso del archivo abierto
#define TXT_READ	0x01
#define TXT_WRITE	0x02
#define TXT_LOCKED	0x04

// prefijo para los archivos temporarios
#define PREFIX_TMP		"dali"
#define PREFIX_REXP		"dl.*"

// tipo de texto
#define T_NORMAL	(1 << 0)
#define T_PRESERVE	(1 << 1)
#define T_ISTREAM	(1 << 2)
#define T_TROFF		(1 << 3)
#define T_ADMIN		(1 << 4)
#define T_TEX		(1 << 5)

//Le indico que la clase existe - A partir de GCC 4.x
class DaliText;

class DaliTextCursor: public ListCursor(String) 
{
    useIdCheck(DaliTextCursor_id);

	friend class DaliText;

private:

	Int ncol;				// numero de columna del cursor

	// busca los espacios para hacer el autoindent
	String findIndentSpace();

public:

	// constructores y destructores

	DaliTextCursor();
	DaliTextCursor(DaliText &txt);
	DaliTextCursor(DaliTextCursor &tc);
	~DaliTextCursor();

	bool setChar(char c);		// asigna el caracter c al cursor
	bool insChar(char c);		// inserta el caracter
	bool delChar();				// borra el caracter
	char getChar();				// toma el caracter de la pos del cursor
	
	void insString(const String &s);	// inserta un string

	void del();

	bool delRange(Int from, Int to, bool compress = true);
								// Borra el rango indicado de la
								// linea corriente.

	/*
		Deletes up to specified row & col.
		joinLastEOL	:	wether the last line should be joined
						or not.
		compress	:	wether the area should be removed or
						just blanked.
	*/
	bool delTo(Int row, Int col, bool joinLastEOL = true,
			bool compress = true);

	Int column() const 			  {idCheck(); return ncol;		  }

	void goToFirst();			// voy al primer elemento de la lista
	void goToLast();			// voy al final de la lista


	void goToCol(Int i)			  {idCheck(); ncol = i;			  		 }
	void goTo(Int f)			  {idCheck(); ListCursor(String)::goTo(f); }
	void goTo(Int f, Int c);
	void moveCol(Int i, bool cursMove = false);

	// Tab managment 

	bool backTab();		// Borra el anterior
	void delFillTabs();	// borra los FILL_TAB's a la derecha del cursor

	// reemplaza espacios por FILL_TAB's a la izquierda
	// e inserta a la derecha (para reconstruir un tab)
	void rebuildTab(String &replaced);

	void reAssignNextTab(Int col, Int n = 1);// reubica el proximo tab
											 // empezando de col

	// inserta un tr_new line en el cursor
	Int insNewLine(bool indent = true);

	// realiza el join de la linea 
	bool joinLines();

	// va el primer no blanco
	void goFirstNonSp();

	// para no permitir que acceda directarmete al string
	const String &value() 	  
		{idCheck(); return ListCursor(String)::value(); }
	const String &operator*() 
		{idCheck(); return ListCursor(String)::operator*(); }
	const String *operator->()
		{idCheck(); return &operator*(); }

	DaliTextCursor &operator++()	  {idCheck(); next(); return *this; }
	DaliTextCursor &operator++(int)	  {idCheck(); next(); return *this; }
	DaliTextCursor &operator--()	  {idCheck(); prev(); return *this; }
	DaliTextCursor &operator--(int)	  {idCheck(); prev(); return *this; }

	DaliTextCursor &operator+=(Int i) {idCheck(); next(i); return *this; }
	DaliTextCursor &operator-=(Int i) {idCheck(); prev(i); return *this; }

	DaliTextCursor &operator=(DaliTextCursor &);
	void operator=(DaliText &);

	virtual Int cmp(const BaseListCursor &tc) const;
};

struct DaliAttributeBlock {

	DaliTextCursor start, end;
	Int attribute;

	DaliAttributeBlock() { attribute = 0; }
	DaliAttributeBlock(DaliAttributeBlock &ab) { *this = ab; }
	DaliAttributeBlock(DaliTextCursor &tc) { start = tc; end = tc; attribute = 0; }

	DaliAttributeBlock &operator=(DaliAttributeBlock &ab) 
		{ 
			start = ab.start; 
			end = ab.end; 
			attribute = ab.attribute;
			return *this; 
		}
};

class DaliAttributeZones 
{	
	useIdCheck(DaliAttributeZones_id);

private:

	DaliAttributeBlock **b_arr;
	Int nelem;					// cantidad del elementos del array
	Int dim;					// dimension del array

	// rutinas para el acceso a la block array
	void add(DaliAttributeBlock &ae);
	void del(Int n);
	Int find(const DaliAttributeBlock &ae);
	void reAlloc(Int n);

	DaliTextCursor start, end;	    // Last accessed zone
	Int attribute;				// Attribute of last accessed zone
	bool valid_last;			// signal if start/end are valid
    
    DaliAttributeZones(const DaliAttributeZones &az) ;
	DaliAttributeZones &operator=(const DaliAttributeZones &az) ;
     
public :

	DaliAttributeZones();
 	~DaliAttributeZones();

	Int getAttribute(DaliTextCursor &tc, Int &lc);
	bool setAttribute(DaliTextCursor s, DaliTextCursor &e, Int attr);
	bool delAttribute(DaliTextCursor s, DaliTextCursor &e, Int attr);
	bool delAttribute(DaliTextCursor &s, Int attr);

	void debug();

};

class DaliText: public List(String)
{ 
	useIdCheck(DaliText_id);

	friend class DaliTextCursor;
	friend class DaliView;

private:
	DaliAttributeZones attrzn;	// atributos asociados al texto

	Int  ttype;				// tipo del texto

	OsFile file;			// osfile para el archivo
	String filename;		// nombre del archivo para el constructor

	OsFile tmpfile;			// osfile para el archivo tmp
	String tmpfilename;		// mombre del archivo tmp

	bool active;			// text activo

	Int  cview;				// cantidad de views asociadas

	Int fileperm;			// open file permissions
	String lockowner;		// owner of the lock (when locked)
	bool existfile;			// existe el archivo corriente

	static String tempDir;	// 'preserve' directory

	bool rmFiles();			// libera tmp y file
	void mkFiles();			// crea el tmp y file

	static Int tabSize_;

	// devuelve el nombre de un archivo temporario
	static const String tempFile(); 

	// oculta el clear
	void delAll() 			{idCheck(); List(String)::delAll(); }

	// decodifica los atributos para grabarlo en un archivo
	String decodeAttrs(DaliTextCursor &tc, Int &curratr, Int cant=ERROR);
	void   encodeAttrs(Int filfrom = ERROR, Int colfrom = ERROR,
					   Int filto = ERROR, Int colto = ERROR);

	// Transforma las sequencias Troff en Ideafix
	String decodeTroffSeq(String ss);
	void encodeTroffSeq(Int rowb = ERROR, Int rowe = ERROR);

	// Transforma las sequencias TeX en Ideafix
	String decodeTeXSeq(String ss);
	void encodeTeXSeq(Int rowb = ERROR, Int rowe = ERROR);

    void operator=(const DaliText&) ;
              DaliText(const DaliText&) ;

public: 

	DaliText(String f, bool isTemp);
	DaliText(InputStream &is, const String &cmd);
	DaliText(const String &cmd);
	DaliText();
	~DaliText();
    
	/*
		storeTmp declarada public para poder ser accedida desde afuera
		para ejecutar el auto save, pero no deberia ser usada
		libremente!!
	*/
	// write in temporary file
	bool storeTmp();

	// write to associated file
	bool store();

	// write to specified file
	void store(const String &file);

	bool load(); 		   		// public for View::reload()

	bool activate();			// activa el text
	void deActivate();			// desactiva el text

	// codifica los tabs del string s utilizando FILL_TAB
	static String encodeTabs(String s);
	static String decodeTabs(String s);

	void setTeX();			// set troff mode
	void setTroff();			// set troff mode
	void setAdmin();			// set Admin mode
	void setIStream();			// set istream mode

	bool isTeX()				{ idCheck(); return ttype&T_TEX; }
	bool isTroff()				{ idCheck(); return ttype&T_TROFF; }
	bool isPreserve()			{ idCheck(); return ttype&T_PRESERVE; }
	bool isIStream()			{ idCheck(); return ttype&T_ISTREAM; }
	bool isAdmin()				{ idCheck(); return ttype&T_ADMIN; }

	// This member function is used by the debugger (temporary?)
	void setDisplayMode()		{ idCheck(); ttype = T_ISTREAM; fileperm = TXT_READ; }

	// metodos asociados a views. Estos no me gustan (pablog)
	Int nroViews()				{ idCheck(); return cview; }
	void incViews()				{ idCheck(); cview++; }
	void decViews()				{ idCheck(); cview--; }

	// manejo del archivo fuente
	void  setFile(const String &f, bool load_new_file = true);
	void  addPath();
	const String &getFile();
	Int   getFilePerm()			{ idCheck(); return fileperm; }
	const String &lockOwner();

	// manejo de bloques
	bool insertText(List(String) &l, DaliTextCursor &c);
	bool getRegion(List(String) &l, DaliTextCursor &begin, DaliTextCursor &end,
				   bool add);
	bool delRegion(DaliTextCursor &begin, DaliTextCursor &end, bool compress,
				   bool correc);

	// manejo de atributos
	Int  getAttr(DaliTextCursor &tc, Int &hasta);
	void setAttr(DaliTextCursor &b, DaliTextCursor &e, Int attr);
	void delAttr(DaliTextCursor &b, DaliTextCursor &e, Int attr);

	// Cantidad de lineas en el texto
	Int nLines()	{idCheck(); return List(String)::size(); }

	// directorio para archivos temporarios
	static void setTempDir(const String &s)	{ tempDir = s; }
	static const String &getTempDir()		{ return tempDir; }

	// Tab size
	static void tabSize(Int v)				{ tabSize_ = v; }
	static Int tabSize()					{ return tabSize_; }
};

#endif
