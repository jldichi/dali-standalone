/********************************************************************
* Copyright (c) 1993 InterSoft Co.  All Rights Reserved
* THIS IS UNPUBLISHED PROPRIETARY SOURCE CODE OF InterSoft Co.
* The copyright notice above does not evidence any
* actual or intended publication of such source code.
*
* $Id: etextcur.cc,v 1.3 2006/02/17 17:37:07 spark Exp $
*
* DESCRIPTION: Implementation of EditableTextCursor class
*
*********************************************************************/

#ifdef __GNUC__
#pragma implementation
#endif

#include <string.h>

#include <ifound.h>
#include <winapp/etext.h>
#include <winapp/etextcur.h>
#include <winapp/window.h>
#include <ifound/stdc.h>

EditableTextCursor::EditableTextCursor() 
: TextCursor()
{ 
	idCheck();
}

EditableTextCursor::EditableTextCursor(Text &txt) 
: TextCursor(txt)
{ 
	idCheck();
}

EditableTextCursor::EditableTextCursor(TextCursor &tc) 
: TextCursor(tc)
{ 
	idCheck();
}

EditableTextCursor::~EditableTextCursor()
{     
	idCheck();

	ncol = 0; 
}

void EditableTextCursor::del()
{
	idCheck();

	Int f = order(), col = ncol, len;

	// borro el atributo de la linea que voy a borrar
	EditableTextCursor end = *this;
	if ((len = value().length())==0)
		len = 1;
	end.goToCol(len-1);
	ncol = 0;
	((Text*)list)->delAttr(*this, end, 
						   A_UNDERLINE|A_REVERSE|A_BLINK|A_BOLD);

	ListCursor(String)::del();

	ncol = col;

	if (f != order())
		goTo(f);
}

bool EditableTextCursor::delRange(Int from, Int to, bool compress)
{
	idCheck();

	bool res = false;
	Int count = to - from + 1;

	if (count > 0) {
		if (to >= value().length() - 1)
			compress = true;

		ncol = from;

		delFillTabs();

		if (compress)
			while (count-- && (*this)) {
				if (delChar())
					res = true;
			}
		else
			while (count-- && (*this)) {
				if (setChar(' '))
					res = true;
				ncol++;
			}

		reAssignNextTab(ncol);

	}

	return res;
}

bool EditableTextCursor::delTo(Int row, Int col, 
								bool joinLastEOL, bool compress)
{
	idCheck();

	/*
		If row, col is before current cursor position
		we swap them.
	*/
	if ((row < order()) || (row == order() && col < ncol)) {
		Int tmp = row;
		row = order();
		this->goTo(tmp);
		tmp = col;
		col = ncol;
		ncol = tmp;
	}

	Int lastPosOrig = value().length()-1;

	/*
		If we are at the same line, delete whatever we have to.
	*/
	if (order() == row) {
		if (ncol == 0 && col >= lastPosOrig && joinLastEOL && compress)
			del();
		else {
			delRange(ncol, col, compress);
			/*
				If the last character is to be deleted, join with the
				next line.
				Do this only if the compress option is used, and
				the joinLastEOL is turned on.
			*/
			if (col >= lastPosOrig && compress && joinLastEOL)
				joinLines();
		}
	} else {
		EditableTextCursor tc = *this;
		/*
			Obtain the length of the last line to delete.
		*/
		tc.goTo(row);
		Int lastPosDest = tc.value().length()-1;
		/*
			Now point to the first line to delete completely.
		*/
		tc.goTo(order());
		if (ncol != 0) {
			if (ncol <= lastPosOrig)
				delRange(ncol, lastPosOrig, compress);
			tc++;
		}
		/*
			If the last line is not to be deleted completely, delete
			whatever corresponds and decrement the last line to delete.
			When joinLastEOL is turned off this should also be done.
		*/
		if (col < lastPosDest || !joinLastEOL) {
			EditableTextCursor tc1 = *this;
			tc1.goTo(row);
			tc1.delRange(0, col, compress);
			row--;
		}
		/*
			Delete all the complete lines between tc.order() and
			row.
		*/
		Int count = row - tc.order() + 1;
		while (count--)
			if (compress)
				tc.del();
			else {
				tc.set(NULL_STRING);
				tc++;
			}

		/*
			If the first line to delete was deleted
			completely, join the two lines that remain.
		*/
		if (ncol > 0 && compress)
			joinLines();
	}
	return true;
}

bool EditableTextCursor::joinLines()
{
	idCheck();

	EditableTextCursor cur = *this;
	cur++;

	// si no hay nada debajo listo
	if (!cur) return false;

	ncol = value().length(); 
	set(value()+cur.value());

	// ahora debo actualizar los cursores correspondientes
	for (EditableTextCursor *aux = (EditableTextCursor*)(((EditableText*)list)->headcur); 
					aux; aux = (EditableTextCursor*)(aux->nextcur)) {
		if (aux != &cur && aux->order() == order()+1) {
			(*aux)--;
			aux->ncol += ncol;
		}
	}

	cur.del();

	return true;
}

void EditableTextCursor::delFillTabs()
{
	idCheck();

	Int backcol = ncol-1;
	Int auxcol  = backcol;
	// busco el principio de la sequencia de FILL_TAB
	for (;auxcol>=0 && value()[auxcol]==FILL_TAB; auxcol--);
	auxcol++;		
	// cambio los FILL_TAB anteriores al cursor
	// por blancos
	while (auxcol <= backcol) {
		ncol = auxcol;
		delChar();
		insChar(' ');
		auxcol++;
	}
	// restauro la columna 
	ncol = backcol+1;
}

void EditableTextCursor::reAssignNextTab(Int col, Int n)
{                  
	idCheck();

	// busco el proximo tab si hay
	Int idxtab, idxftab;
	Int nextt = col;
	do {
		// busco el proximo tab si hay
		idxtab  = value().index('\t', nextt);
		idxftab = value().index(FILL_TAB, nextt);
		// existe otro tab 
		if (idxtab != ERROR || idxftab != ERROR) {

			Int ccol = ncol;
			Int cant, i;

			// antes del tab hay un FILL_TAB
			if (idxftab != ERROR && idxftab < idxtab) {
				cant = NFILLTAB(idxftab);
				ncol = idxftab;

				// si debo borrar FILL_TAB's
				if (idxtab-idxftab > cant) {
					for (i = idxtab-idxftab-cant; i; i--)
						delChar();	
				// debo insertar FILL_TAB's
				} else if (idxtab-idxftab < cant) {
					for (i = cant-(idxtab-idxftab); i; i--)
						insChar(FILL_TAB);
				}
				ncol = ccol;
			// no hay FILL_TAB antes de tab
			} else {
				ncol = idxtab;
				
				// inserto los FILL_TAB que hagan falta
				for (i = NFILLTAB(idxtab); i ; i--)
					insChar(FILL_TAB);
				ncol = ccol;
			}
		}
		n--;
		if (n > 0)
			nextt = value().index('\t', nextt)+1;
	} while (n > 0 && (idxtab != ERROR || idxftab != ERROR));
}

bool EditableTextCursor::setChar(char c)
{
	idCheck();

	if (curr)
		return ListCursor(String)::value().setChar(c, ncol);
	else {
		String aux;
		if (!(aux.setChar(c, ncol)))
			return false;
		set(aux);
		return true;
	}
}

char EditableTextCursor::getChar()
{
	idCheck();

	if (curr) return value()[ncol];
	return (char)NULL;
}

bool EditableTextCursor::backTab()
{
	idCheck();

	if (value()[ncol-1] != '\t')
		return false;
	else {
		do {
			ncol--;
			delChar();
		} while (value()[ncol-1] == FILL_TAB);
		return true;
	}
}

void EditableTextCursor::rebuildTab(String &replaced)
{
	idCheck();

	Int lenRep = replaced.length();
	moveCol(-lenRep);
	Int i;
	for (i = 0; i < lenRep; i++) {
		(void) setChar(replaced[i]);
		moveCol(1);
	}
}

void EditableTextCursor::insString(const String &s)
{
	idCheck();

	// no hay nada que insertar
	if (s.length() == 0) return;

	const char *aux = toCharPtr(s);
	Int len = strlen(aux);

	// si no hay nada inserto un string vacio
	if (!(*this)) set(NULL_STRING);

	// borro los FILL_TAB a la izquierda
	delFillTabs();

	// inserto los caracteres
	for (Int i = len-1; i >= 0; i--)
		insChar(aux[i]);

	// reubico el primer tab que insert
	reAssignNextTab(ncol);
	
	// reubico el primer de lo que habia
	reAssignNextTab(ncol+len);
}

Int EditableTextCursor::insNewLine(bool indent)
{
	idCheck();

	// estoy en un linea valida
	if (*this) {
		Int len = value().length();

		// si estoy fuera del string
		if (len < ncol) {
			moveCol(-1);
			insChar(' ');
			moveCol(1);
			add(NULL_STRING);
		// estoy dentro del string
		} else {
			// convierto en blancos los FILL_TAB 
			// anteriores al cursor
			delFillTabs();

			if (!ncol) {
				ins(NULL_STRING);
				(*this)--;
			} else {
				String aux = value();
				set(aux(0, ncol));
				len -= ncol;
				if (len>0)
					add(aux(ncol, len));
				else
					add(NULL_STRING);
			}
		}

		// ahora busco los blancos para indentar
		Int newcol;
		if (indent) {
			String s = findIndentSpace();
			(*this)++;
			set(s+value());
			(*this)--;
			newcol = s.length();
		} 
		else newcol = 0;

		// ahora debo actualizar los cursores correspondientes
		for (EditableTextCursor *aux = (EditableTextCursor*)(((EditableText*)list)->headcur); 
						aux; aux = (EditableTextCursor*)(aux->nextcur)) {
			if (aux != this && aux->order() == order() &&
				aux->ncol >= ncol) {
				(*aux)++;
				aux->ncol = newcol + aux->ncol - ncol;
			}
		}

		// actualizo columna
		ncol = newcol;

	// estoy en una linea vacia
	} else {
		ins(NULL_STRING);
		ncol = 0;
	}

	// actualizo fila
	(*this)++;

	// reordeno los tabs
	reAssignNextTab(ncol);

	return ncol;
}

String EditableTextCursor::findIndentSpace()
{
	idCheck();

	String res = NULL_STRING;

	Int len , i;

	for(len=value().length(), i=0; i < len && isSpace(value()[i]); i++)
		;
	
	if (i) res = value()(0, i);

	return res;
}


