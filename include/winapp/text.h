/********************************************************************
* Copyright (c) 1993 InterSoft Co.  All Rights Reserved
* THIS IS UNPUBLISHED PROPRIETARY SOURCE CODE OF InterSoft Co.
* The copyright notice above does not evidence any
* actual or intended publication of such source code.
*
* $Id: text.h,v 1.3 2000/02/07 17:47:48 diegon Exp $
*
* DESCRIPTION
* Class definitions for Text type.
*********************************************************************/

#ifndef TEXT_H
#define TEXT_H

#include <ifound.h>
#include <ifound/list.h>
#ifndef DEF_LIST_STRING
#define DEF_LIST_STRING
	declare(List, String);
	declare(ListCursor, String);
#endif

#ifdef __GNUC__
#pragma interface
#endif

#include <winapp/textcur.h>
#include <ifound/str.h>
#include <local/osfile.h>
#include <ifound/itypes.h>

// Tabs
#define FILL_TAB	char(31)
#define NFILLTAB(x)	(Text::tabSize()-((x)%Text::tabSize())-1)

// Atributos
#define BEGIN_ATTR		"\033["
#define ATTR_NORMAL		"\033[0m"
#define SEPAR_ATTR		';'
#define END_ATTR		'm'

enum TextType {
	READONLY	= 0,
	READWRITE	= 1,
	ISTREAM		= 2,
	ADMIN		= 4,
	CSOURCE		= 8,
	IBSOURCE	= 16,
	PRESERVE	= 32,
};	

enum TextPerm {
	PERM_READ	= 0,
	PERM_WRITE	= 1,
	PERM_LOCKED	= 2,
};

class EditableText;
class FileText;
class View;

class Text: public List(String) 
	// Implements methods to treat read-only Text
{                               
	useIdCheck(Text_id);
	
	friend class TextCursor;
	friend class View;
	
protected:	

	TextType		ttype;		// tipo del texto
	bool 			active;		// text activo
	String 			filename;	// nombre del archivo para el constructor
	bool 			existfile;	// existe el archivo corriente
	TextPerm		fileperm;	// permisos del archivo abierto
	OsFile 			file;		// osfile para el archivo

private:

	AttributeZones 	attrzn;		// atributos asociados al texto
	Int  			cview;		// cantidad de views asociadas
	static 			Int tabSize_;

	// Avoid accidental use of these
	    void operator=(const Text&) ;
	    Text(const Text&) ;

public:
    
    // Text constructors
		Text();
		Text(const String &file);
		Text(const String &cmd , bool abortable);
		virtual ~Text();                              
	
	// Access to derived text classes, NULL at this level. (Dynamic Cast)
	    virtual EditableText 	*edText() 	{ return NULL;}
	    virtual FileText 		*fileText() { return NULL;}

    // Text type methods
		bool isType(Int t)		{ return (ttype & t); }
		void changeType(TextType t);			

	// Miscelaneous
	 	Int  nLines()					{ return List(String)::size(); }

	// File management methods
		const 	String &getTextName()		{ return filename; }
		virtual void  	setFile(const String &f, bool loadNewFile = true);
		virtual bool 	load(); 		   	   
		virtual bool 	activate();						// activa el text
		virtual void 	deActivate();					// desactiva el text
				bool	loadAdminFile(const String &f);

	// Tab management methods
		bool 	backTab();		// Borra el anterior
		void 	delFillTabs();	// borra los FILL_TAB's a la derecha del cursor
		void 	rebuildTab(String &replaced);
		void 	reAssignNextTab(Int col, Int n = 1);
		String	encodeTabs(String s);
		String	decodeTabs(String s);
		static 	void tabSize(Int v)				{ tabSize_ = v; }
		static 	Int tabSize()					{ return tabSize_; }

// ex private method
		void delAll() { List(String)::delAll(); }

	// Attribute management
		Int  	getAttr(TextCursor &tc, Int &hasta);
		void 	setAttr(TextCursor &b, TextCursor &e, Int attr);
		void 	delAttr(TextCursor &b, TextCursor &e, Int attr);
		String 	attribToSequence(Int attr);
		Int		sequenceToAttrib(const char	*str, Int &skip);
		String decodeAttrs(TextCursor &tc, Int &curratr, Int cant=ERR);
		void   encodeAttrs(Int filfrom = ERR, Int colfrom = ERR,
						   Int filto = ERR, Int colto = ERR);

	// View related methods (pablog does not like them)
		Int  	nroViews()			 { idCheck(); return cview;}
		void 	incViews()			 { idCheck(); cview++;}
		void 	decViews()			 { idCheck(); cview--;}
};

#endif
