/********************************************************************
* Copyright (c) 1993 InterSoft Co.  All Rights Reserved
* THIS IS UNPUBLISHED PROPRIETARY SOURCE CODE OF InterSoft Co.
* The copyright notice above does not evidence any
* actual or intended publication of such source code.
*
* $Id: etext.cc,v 1.3 2002/03/15 22:20:27 hardaiz Exp $
*
* DESCRIPTION: Implementation of EditableText class
*
*********************************************************************/
#ifdef __GNUC__
#pragma implementation
#endif

#include <winapp/etext.h>
#include <winapp/window.h>
#include <local/strfile.h>
#include <ifound/stdc.h>
#include <errno.h>
#include <sys/stat.h>
#include <local/dirname.h>

// metodos publicos

EditableText::EditableText(String f) 
: Text(f)
{
	idCheck();

	ttype = TextType(ttype|READWRITE);
}

EditableText::EditableText() 
: Text()
{ 
	idCheck();

	ttype = TextType(ttype|READWRITE);
}

EditableText::EditableText(const String &cmd, bool abortable)
: Text(cmd,abortable)
{                                      
	idCheck();

	ttype = TextType(ttype|READWRITE);
}

EditableText::~EditableText()
{ 
	idCheck();
}

void EditableText::deActivate()
{
	idCheck();

	Text::deActivate();
	ttype = READWRITE;
}

bool EditableText::store()
	// Stores file on disk
{
	idCheck();

	if (!active) return false;
    
    String newfile = filename;
    
	{        
		OutputFile outputf(newfile);

		Int curratr = A_NORMAL;

		// albertoi
        EditableTextCursor cursor(*this);

		//for (EditableTextCursor cursor = *this; cursor; cursor++) 
		for (; cursor; cursor++)
			outputf << decodeTabs(decodeAttrs(cursor,curratr)) << "\n";
	}
 
	existfile = true;
	ttype     = READWRITE; 

	resetModif();

	return true;
}

bool EditableText::insertText(List(String) &l, EditableTextCursor &c)
{
	idCheck();

	// no es este texto ERR
	if ((Text*)(c.list) != this)
		return false;

	ListCursor(String) lc(l);	

	// la lista esta vacia
	if (!lc) return false;

	// inserto el string en el cursor
	c.insString(lc.value());

	// si la lista tenia solo un elemento -> listo
	if (l.size() == 1) {
		encodeAttrs(c.order(), c.column(), c.order(), 
					c.column()+lc.value().length());
		return true;
	}

	// guardo las coordenadas del cursor
	Int oldfil = c.order();
	Int oldcol = c.column();

	// coloco el cursor para realizar el insNewLine		
	c.moveCol(lc.value().length());

	// inserto el tr_new line
	c.insNewLine(false);

	// inserto el tr_new line
	// ahora inserto la ultima linea
	lc.goToLast();

	// me fijo en el ultimo valor
	if (lc.value() != NULL_STRING)
		c.insString(lc.value());
	
	// hago el insert de las demas lineas
	for (lc.goTo(1); lc.order() < l.size()-1; lc++) {
		if (!c) {
			c.set(lc.value());
			c++;
		} else c.ins(lc.value());
	}

	// codifico los atributos
	encodeAttrs(oldfil, oldcol, c.order(), 
				c.column()+lc.value().length());

	c.goTo(oldfil, oldcol);

	return true;
}

bool EditableText::getRegion(List(String) &l, TextCursor begin, TextCursor end,
					 bool adding)
{     
	idCheck();

	if (!end) {
		end = *this;
		end.goToLast();
		Int len = end.value().length();
		end.goToCol(len); //end.goToCol(len ? len-1 : 0);
	}                          

	// valido los cursores
	if ((Text*)(begin.list) != this || (Text*)(end.list) != this || 
		begin >= end) return false;

	// atributo en normal para tomar la region
	Int currattr = A_NORMAL;

	// coloco la primer linea
	Int len = begin.value().length();
	if (end.order() == begin.order() && end.column() + adding < len)
		len = end.column() + adding;

	len -= begin.column();

	// veo como inserto la primer linea
	// si hay algo para insertar inserto
	if (len > 0) 
		l.add(decodeAttrs(begin, currattr, len));
	// si no hay pero no es la unica linea -> indica un tr_new line
	else if (len <= 0 && begin.order() != end.order())
		l.add(NULL_STRING);
	 	
	// hay solo una linea -> termino aca
	if (end.order() == begin.order()) return true;

	// copio las demas linea
	for (begin.goTo(begin.order()+1, 0)         ;
		 begin && begin.order() < end.order() ; begin++)
		l.add(decodeAttrs(begin, currattr));

	// coloco la ultima linea
	len = begin.value().length();   

	if (end.column() + adding < len)
		len = end.column() + adding ;

	len -= begin.column();

	if (len <= 0 || begin.column()==end.column()+adding)
		l.add(NULL_STRING);
	else
		l.add(decodeAttrs(begin, currattr, len));

	return true;
}

bool EditableText::delRegion(EditableTextCursor &begin, TextCursor &end, bool compress,
					 bool correc)
{
	idCheck();

	// valido los cursores
	if ((Text*)(begin.list) != this || (Text*)(end.list) != this || 
		begin >= end) return false;

	// no hay que realizar correccion 
	if (!correc) {

		// borro los atributos de la zona
		delAttr(begin, end, A_UNDERLINE|A_REVERSE|A_BLINK|A_BOLD);

		begin.delTo(end.order(), end.column(), false, compress);
	}
	else {
		// estoy dentro del texto
		if (end) {
			// estoy parado en numero de columna <> 0
			if (end.column()) {

				Int len = end.value().length();
				Int col = end.column();

				if (len == 0) len = 1;

				if (col < len)
					end.moveCol(-1);
				else
					end.goToCol(len-1);

				// borro los atributos de la zona
				delAttr(begin,end,A_UNDERLINE|A_REVERSE|A_BLINK|A_BOLD);

				begin.delTo(end.order(), end.column(), false, compress);

				end.goToCol(col);
			}
			// estoy en la columna 0
			else {
				end--;

				Int len = end.value().length();
				if (len == 0) len = 1;

				end.goToCol(len-1);

				// borro los atributos de la zona
				delAttr(begin,end,A_UNDERLINE|A_REVERSE|A_BLINK|A_BOLD);

				begin.delTo(end.order(), end.column(), true, compress);

				end.goTo(end.order()+1, 0);
			}
		}
		// estoy fuera del texto
		else {
			Int fil = end.order();
			Int col = end.column();

			end.goToLast();

			Int len = end.value().length();
			if (len == 0) len = 1;

			end.goToCol(len-1);

			// borro los atributos de la zona
			delAttr(begin, end, A_UNDERLINE|A_REVERSE|A_BLINK|A_BOLD);

			begin.delTo(end.order(), end.column(), true, compress);

			end.goTo(fil, col);
		}
	}

	return true;
}

// end of Editable Text
