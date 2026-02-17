/********************************************************************
* Copyright (c) 1993 InterSoft Co.  All Rights Reserved
* THIS IS UNPUBLISHED PROPRIETARY SOURCE CODE OF InterSoft Co.
* The copyright notice above does not evidence any
* actual or intended publication of such source code.
*
* $Id: textcur.h,v 1.3 2002/03/15 22:10:19 hardaiz Exp $
*
* DESCRIPTION
* Class definitions for TextCursor type.
*********************************************************************/

#ifndef TEXTCUR_H
#define TEXTCUR_H

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

#include <ifound/str.h>
#include <ifound/itypes.h>

// defino mi isspace
#define	isSpace(c)	(isspace(c) || (c) == FILL_TAB)

class Text;
class EditableText;
class EditableTextCursor;
class FileTextCursor;

class TextCursor: public ListCursor(String) 
	// TextCursor methods
{
	useIdCheck(TextCursor_id);

	friend class Text;
	friend class EditableText;

	void reset();
	
protected:	

	Int ncol;				

public:

	// constructores y destructores

	TextCursor();
	TextCursor(Text &txt);
	TextCursor(TextCursor &tc);
	~TextCursor();
	TextCursor &operator=(TextCursor &);
	void operator=(Text &);
	
	// Access to derived textcursor classes, 
	// NULL at this level. (Dynamic Cast)
    virtual EditableTextCursor 	*edTextCur() 	{ return NULL;}
    virtual FileTextCursor 		*fileTextCur() { return NULL;}


	bool 	insChar(char c);		// inserta el caracter
	bool 	delChar();				// borra el caracter
	
	Int 	column() const 			  	{ return ncol; }

	void 	goToFirst();				// primer elemento de la lista
	void 	goToLast();					// ultimo elemento de la lista

	void	goToCol(Int i)			  	{ ncol = i; }
	void	goTo(Int f)			  		{ ListCursor(String)::goTo(f); }
	void	goTo(Int f, Int c);
	void 	goFirstNonSp();
	void	moveCol(Int i, bool cursMove = false);

	// Tab managment 
	bool backTab();						


	// Operations
	const String &value() 	  		{ return ListCursor(String)::value();}
	const String &operator*() 		{ return ListCursor(String)::operator*();}
	const String *operator->()		{ return &operator*(); }

	TextCursor &operator++()	  	{ next(); return *this; } // prefix
	TextCursor &operator++(int)	  	{ next(); return *this; } // postfix
	TextCursor &operator--()	  	{ prev(); return *this; } // prefix
	TextCursor &operator--(int)	  	{ prev(); return *this; } // postfix

	TextCursor &operator+=(Int i) 	{ next(i); return *this; }
	TextCursor &operator-=(Int i) 	{ prev(i); return *this; }

	virtual Int cmp(const BaseListCursor &tc) const;
};

struct AttributeBlock {

	TextCursor start, end;
	Int attribute;

	AttributeBlock() 					{ attribute = 0; }
	AttributeBlock(AttributeBlock &ab)	{ *this = ab; }
	AttributeBlock(TextCursor &tc)		{start = tc; end = tc; attribute = 0;}

	AttributeBlock &operator=(AttributeBlock &ab) { 
		start 		= ab.start; 
		end 		= ab.end; 
		attribute 	= ab.attribute;
		return *this; 
	}
};

class AttributeZones {

	useIdCheck(AttributeZones_id);

// private:

	AttributeBlock **b_arr;
	Int nelem;					// cantidad del elementos del array
	Int dim;					// dimension del array

	// rutinas para el acceso a la block array
	void add(AttributeBlock &ae);
	void del(Int n);
	Int find(const AttributeBlock &ae);
	void reAlloc(Int n);

	TextCursor start, end;	    // Last accessed zone
	Int attribute;				// Attribute of last accessed zone
	bool valid_last;			// signal if start/end are valid
    
    AttributeZones(const AttributeZones &az) ;
     
public :

	AttributeZones();
 	~AttributeZones();
 	AttributeZones &operator=(const AttributeZones &az) ;

	Int getAttribute(TextCursor &tc, Int &lc);
	bool setAttribute(TextCursor s, TextCursor &e, Int attr);
	bool delAttribute(TextCursor s, TextCursor &e, Int attr);
	bool delAttribute(TextCursor &s, Int attr);

	void debug();

};

#endif
