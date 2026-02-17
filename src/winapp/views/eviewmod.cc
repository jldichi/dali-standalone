/********************************************************************
* Copyright (c) 1993 InterSoft Co.  All Rights Reserved
* THIS IS UNPUBLISHED PROPRIETARY SOURCE CODE OF InterSoft Co.
* The copyright notice above does not evidence any
* actual or intended publication of such source code.
*
* $Id: eviewmod.cc,v 1.2 2002/03/15 22:23:50 hardaiz Exp $
*
* DESCRIPTION: Implementation of edition methods for EditableView class
*
*********************************************************************/

#include <ifound.h>
#include <ifound/message.h>
#include <winapp/dbox.h>
#include <local/strfile.h>
#include <ifound/stdc.h>
#include <ifound/rexp.h>
#include <winapp/window.h>

#include <winapp/eview.h>
#include <winapp/etext.h>

#include <sys/types.h>
#include <sys/stat.h>

#define cursor (*_cursor)

bool EditableView::insChar(UnChar c)
{
	idCheck();

	if (c == '\t') return tab();

	if (cursor) {

		Int i;
		char auxc = cursor.value()[cursor.column()];
		// me fijo el caracter sobre el que estoy
		// parado y segun eso opero
		switch(auxc) {
		case FILL_TAB:
			edCursor()->delFillTabs();
			cursor.delChar();
			cursor.insChar((char)c);
			break;
		case '\t':
			edCursor()->delFillTabs();

			// si estoy en modo insert
			if (mode&INSERT) {
				// inserto los FILL_TAB
				for (i = 1; i < Text::tabSize(); i++)
					cursor.insChar(FILL_TAB);	
			// si estoy en modo overwrite
			} else cursor.delChar();

			cursor.insChar((char)c);
			break;
		default:

			// estoy en modo insert
			if (mode&INSERT) {
				cursor.insChar((char)c);
				edCursor()->reAssignNextTab(cursor.column()+1);
			// estoy en modo overwrite
			} else {
				cursor.delChar();
				cursor.insChar((char)c);
			}

			break;
		}

	}
	else {
		String s; s.insChar(c, cursor.column());
		cursor.set(s);
	}

	if (cursor.column()-c_orig < ncols-1)
		redrawCurrLine();

	return cursRight(1, true);
}

bool EditableView::delChar()
{
	idCheck();

	if (!cursor) 
		return false;

	Int col = cursor.column();
	char auxc = cursor.value()[col];
	Int i, idxtab, cant;

	// si estoy fuera de la linea hago el join
	if (auxc == '\0') {
		if (col > 0) {
			char auxchar = cursor.value()[col-1];
			if (auxchar == '\0') {
				cursor.moveCol(-1);
				cursor.insChar(' ');
			}
		}
		(void)joinLines();
		return true;
	}

	switch (auxc) {
	case FILL_TAB:
		edCursor()->delFillTabs();

		idxtab = cursor.value().index('\t', col+1);
		cant   = idxtab-col+1;

		// borro los FILL_TABS y el tab
		for (i = 0; i < cant ; i++)
			cursor.delChar();

		// reubico el proximo tab
		edCursor()->reAssignNextTab(col);

		break;
	case '\t':

		// si lo que tengo al lado es <> de FILL_TAB
		// debo convertir los FILL_TAB del este tab en blancos
		// y traer el siguiente tab para aca
		if (cursor.value()[col+1] == FILL_TAB) {
			// ahora borro este tab y los FILL_TAB
			// del proximo tab
			for (i = 0; i < Text::tabSize(); i++)
				cursor.delChar();
		// debo borrar el tab y busco el proximo
		// para reubicarlo
		} else {
			edCursor()->delFillTabs();

			// borro el tab
			cursor.delChar();

			// reubico el proximo tab
			edCursor()->reAssignNextTab(col);
		}

		break;
	default:
		// borro el caracter
		cursor.delChar();

		// reubico el proximo tab
		edCursor()->reAssignNextTab(col);
		break;
	}

	redrawCurrLine();	

	return false;
}

bool EditableView::tab()
{
	idCheck();

	Int moves, cant;

	if (cursor) {
		 Int i;
		 char auxc = cursor.value()[cursor.column()];

		 // me fijo el caracter sobre el que estoy
		 // parado y segun eso opero
		 switch(auxc) {
			case FILL_TAB:
				edCursor()->delFillTabs();

				// inserto los caracteres para el
				// tab que ya estaba
				cant = cursor.column()%Text::tabSize();
				for (i = 0; i < cant; i++)
					cursor.insChar(FILL_TAB);

				// inserto el nuevo tab
				cursor.insChar('\t');

				// inserto los caracteres FILL_TAB
				// para este nuevo tab
				cant = NFILLTAB(cursor.column());
				for (i = 0; i < cant; i++)
					cursor.insChar(FILL_TAB);

				moves = cant + 1;
				break;
			case '\t':
				// inserto los FILL_TAB
				for (i = 1; i < Text::tabSize(); i++)
					cursor.insChar(FILL_TAB);	

				cursor.insChar('\t');
				moves = Text::tabSize();	
				break;
			default:
				// inserto el nuevo tab
				cursor.insChar('\t');

				// inserto los FILL_TAB para el nuevo tab
				cant = NFILLTAB(cursor.column());
				for (i = 0; i < cant; i++)
					cursor.insChar(FILL_TAB);

				moves = cant + 1;

				edCursor()->reAssignNextTab(cursor.column()+moves);

				break;
		 }		
	}	
	else {
		String s;
		Int col = cursor.column();
		s.insChar('\t', col);

		// inserto los FILL_TAB para el nuevo tab
		cant = NFILLTAB(col);
		for (Int i = 0; i < cant; i++)
			s.insChar(FILL_TAB, col);

		cursor.set(s);
		moves = cant + 1;
    }

	if (cursor.column()-c_orig <
		ncols-(Text::tabSize()-(cursor.column()%Text::tabSize())))
		redrawCurrLine();

	return cursRight(moves, true);
}

bool EditableView::backSpace()
{
	idCheck();

	if (!(cursor)) return cursLeft(1, true);

	//  debo hacer el join de las lineas
	if (cursor.column() == 0) {
		if (f_orig == 0 && cursor.order() == 0) {
			beep();
			return false;
		}
		(cursor)--;
		(void)joinLines();
		return true;
	}

	cursor.moveCol(-1);
	bool m = actColOrigin();
	goToCursor();	
	delChar();
	if (m) {
		View::redraw();
		return false;
	}

	writeStLine();
	return false;
}

bool EditableView::backTab()
{
	idCheck();

	if (cursor) {
		if (edCursor()->backTab()) {
			redrawCurrLine();
			return true;
		}
	}
	return false;
}

Int EditableView::insNewLine(bool deflt)
{
	idCheck();

	bool scr = !cursor.column();

	Int col = edCursor()->insNewLine(deflt);

	c_orig = 0;

	if (!actBotOrigin() && scr && !(mode&DSPCTROL)) {
		insLines();
		goToCursor();	
		writeStLine();
		updateOtherViews();
	}
	else View::redraw();

	return col;
}

bool EditableView::delLine()
{
	idCheck();

	if (cursor) {
		cursor.del();
		View::redraw();
		return true;
	} else {
		beep();
		return false;
	}
}

bool EditableView::insLine(const String &txt)
{
	idCheck();

	bool hadSomething = (cursor) ? true : false;

	cursor.ins(txt);
	insLines();
	if (hadSomething)
		(cursor)--;
	redrawCurrLine();
	return true;
}

void EditableView::setLine(const String &txt)
{
	idCheck();

	cursor.set(txt);
	redrawCurrLine();
}

bool EditableView::joinLines()
{
	idCheck();

	// no pude hacerlo
	if (!edCursor()->joinLines()) return false;

	actColOrigin();
	edCursor()->reAssignNextTab(cursor.column());
	actTopOrigin();
	View::redraw();
	return true;
}

// Returns true if the command deleted a line and false	otherwise.
bool EditableView::delWord()
{
	idCheck();

	if (cursor) {
		Int curcol = cursor.column();
		Int row, col;
		findNextWord(1, row, col);
		// If the word is on the same line, delete the range and that`s it.
		if (row == cursor.order()) {
			col--;
			edCursor()->delRange(curcol, col);
			redrawCurrLine();
			return false;
		} else {
			// If we are not past the EOL, then delete up to EOL.
			if (curcol < cursor.value().length()) {
				delEndOfLine();
				return false;
			} else {
				// If the next word is not on the line 
				// below, delete the whole line.
				TextCursor auxTc = cursor;
				if (row >= cursor.order()+2) {
					auxTc++;
					auxTc.del();
				} else {
					// If we are past the EOL, then add spaces from
					//	EOL to cursor position.
					if (auxTc.column() > cursor.value().length()) {
						auxTc.moveCol(-1);
						auxTc.insChar(' ');
					}
					// Now place (row, col) pointing to the last
					// character to delete.
					col--;
					if (col < 0) {
						row--;
						col = cursor.value().length();
					}
					// Delete from cursor position up to the next word.
					edCursor()->delTo(row, col);
				}
				View::redraw();
				return true;
			}
		}
	} else
		return false;
}

bool EditableView::delEndOfLine()
{
	idCheck();

	if (cursor) {
		edCursor()->delRange(cursor.column(), 
									 cursor.value().length()-1);
		redrawCurrLine();
		return true;
	} else {
		beep();
		return false;
	}
}

void EditableView::getRegion(List(String) &l, TextCursor &tc)
{
	idCheck();

	if (!tc || tc == cursor)
		return;

	if (!cursor || cursor > tc)
		text->edText()->getRegion(l, tc, cursor, false);
	else
		text->edText()->getRegion(l, cursor, tc, true);
		
}
		
void EditableView::delRegion(TextCursor &tc, bool compress)
{
	idCheck();

	// si no hay que marcar me voy
	if (!tc || tc == cursor)
		return;

	bool res;

	if (!cursor || cursor > tc) {
		EditableTextCursor eTextCur(tc); // albertoi
		//res = edText()->delRegion(tc, cursor, compress, true);
		res = edText()->delRegion(eTextCur, cursor, compress, true);
	}	
	else {
		EditableTextCursor eTextCur(cursor); // albertoi
		//	res = edText()->delRegion(cursor, tc, compress, false);
		res = edText()->delRegion(eTextCur, tc, compress, false); 
	}

	if (res) View::redraw();
}

void EditableView::insertText(List(String) &l)
{
	idCheck();

	EditableTextCursor eTextCur(cursor); // albertoi

	// original -> if (!edText()->insertText(l, cursor))
	if (!edText()->insertText(l, eTextCur)) 
		return;

	if (l.size()==1)
		redrawCurrLine();
	else
		View::redraw();
}

		
// end of viewmod
