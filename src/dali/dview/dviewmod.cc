/*********************************************************************
* Copyright (c) 1991 InterSoft Co.  All Rights Reserved
* THIS IS UNPUBLISHED PROPRIETARY SOURCE CODE OF InterSoft Co.
* The copyright notice above does not evidence any
* actual or intended publication of such source code.
*
* $Id: dviewmod.cc,v 1.2 2006/10/27 17:18:22 hardaiz Exp $
*
* DESCRIPTION: Implementation of view class
*********************************************************************/

#include <ifound.h>
#include <ifound/msgtable.h>
#include <dview.h>
#include <dtext.h>
#include <winapp/dbox.h>
#include <local/strfile.h>
#include <ifound/stdc.h>
#include <ifound/rexp.h>
#include <winapp/window.h>
#include <editor.h>

#include <sys/types.h>
#include <sys/stat.h>

#define cursor (*_cursor)
#define block  (*_block)

#include <stdio.h>
bool DaliView::insChar(UnChar c)
{
	idCheck();

	if (c == '\t') 
		return tab();

	if (cursor) {
#if 0
		// If pseudo-autowrap feature is on ...
		if (1 && (cursor.column() > 70) && c != ' ') {
			int t = cursor.value().length() - 1;
			while(t > 0 && cursor.value()[t] != ' ')
				t--;                            

			cursor.goToCol(t);
			
			Int indent = insNewLine();
			fprintf(stderr, "insNewLine '%d'\n", indent);
			
			cursor.delChar();
			cursor.goToCol(cursor.value().length() - 1);

// cursor.moveCol(cursor.value().length() - cursor.column() - indent);
// cursor.goToCol(cursor.value().length());

    		// estoy en modo insert
			if (mode&INSERT) {
				cursor.insChar((char)c);
				cursor.reAssignNextTab(cursor.column()+1);
			// estoy en modo overwrite
			} else {
				cursor.delChar();
				cursor.insChar((char)c);
			}

			redrawCurrLine();
			return cursRight(1, true);
		}
#endif   
		
		Int i;
		char auxc = cursor.value()[cursor.column()];
		// me fijo el caracter sobre el que estoy
		// parado y segun eso opero
		switch(auxc) {
		case FILL_TAB:
			cursor.delFillTabs();
			cursor.delChar();
			cursor.insChar((char)c);
			break;
		case '\t':
			cursor.delFillTabs();

			// si estoy en modo insert
			if (mode&INSERT) {
				// inserto los FILL_TAB
				for (i = 1; i < DaliText::tabSize(); i++)
					cursor.insChar(FILL_TAB);	
			// si estoy en modo overwrite
			} else cursor.delChar();

			cursor.insChar((char)c);
			break;
		default:

			// estoy en modo insert
			if (mode&INSERT) {
				cursor.insChar((char)c);
				cursor.reAssignNextTab(cursor.column()+1);
			// estoy en modo overwrite
			} else {
				cursor.delChar();
				cursor.insChar((char)c);
			}

			break;
		}
	}
	else {
		String s; 
		s.insChar(c, cursor.column());
		cursor.set(s);
	}

	if (cursor.column()-c_orig < ncols-1)
		redrawCurrLine();

	return cursRight(1, true);
}

bool DaliView::delChar()
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
		cursor.delFillTabs();

		idxtab = cursor.value().index('\t', col+1);
		cant   = idxtab-col+1;

		// borro los FILL_TABS y el tab
		for (i = 0; i < cant ; i++)
			cursor.delChar();

		// reubico el proximo tab
		cursor.reAssignNextTab(col);

		break;
	case '\t':

		// si lo que tengo al lado es <> de FILL_TAB
		// debo convertir los FILL_TAB del este tab en blancos
		// y traer el siguiente tab para aca
		if (cursor.value()[col+1] == FILL_TAB) {
			// ahora borro este tab y los FILL_TAB
			// del proximo tab
			for (i = 0; i < DaliText::tabSize(); i++)
				cursor.delChar();
		// debo borrar el tab y busco el proximo
		// para reubicarlo
		} else {
			cursor.delFillTabs();

			// borro el tab
			cursor.delChar();

			// reubico el proximo tab
			cursor.reAssignNextTab(col);
		}

		break;
	default:
		// borro el caracter
		cursor.delChar();

		// reubico el proximo tab
		cursor.reAssignNextTab(col);
		break;
	}

	redrawCurrLine();	

	return false;
}

bool DaliView::tab()
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
				cursor.delFillTabs();

				// inserto los caracteres para el
				// tab que ya estaba
				cant = cursor.column()%DaliText::tabSize();
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
				for (i = 1; i < DaliText::tabSize(); i++)
					cursor.insChar(FILL_TAB);	

				cursor.insChar('\t');
				moves = DaliText::tabSize();	
				break;
			default:
				// inserto el nuevo tab
				cursor.insChar('\t');

				// inserto los FILL_TAB para el nuevo tab
				cant = NFILLTAB(cursor.column());
				for (i = 0; i < cant; i++)
					cursor.insChar(FILL_TAB);

				moves = cant + 1;

				cursor.reAssignNextTab(cursor.column()+moves);

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
		ncols-(DaliText::tabSize()-(cursor.column()%DaliText::tabSize())))
		redrawCurrLine();

	return cursRight(moves, true);
}

bool DaliView::backSpace()
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
		redraw();
		return false;
	}

	writeStLine();
	return false;
}

bool DaliView::backTab()
{
	idCheck();
	if (cursor) {
		if (cursor.backTab()) {
			redrawCurrLine();
			return true;
		}
	}
	return false;
}

bool DaliView::shiftRightLine(DaliTextCursor &tc)
{
	idCheck();
	if (tc.value().length() != 0) {
		goCursor(tc.order(), 0);
		tab();
		// goFirstNonSp();
		return true;
	} else
		return false;
}

bool DaliView::shiftRight()
{
	idCheck();
	Int from, to;

	// estoy en modo bloque
	if (block) {
		if (block < cursor) {
			from = block.order();
			to = cursor.order();

			// Correct when last position is at first character of
			// a line
			if (cursor.column() == 0)
				to--;
		} else {
			from = cursor.order();
			to = block.order();

			// Correct when last position is at first character of
			// a line
			if (block.column() == 0)
				to--;
		}
	} else
		if (cursor)
			from = to = cursor.order();
		else
			return false;

	bool result = true;
	Int cCol = cursor.column(), clin = cursor.order();

	DaliTextCursor tc = cursor;
	for (tc.goTo(from); tc.order() <= to; tc++)
		if (!shiftRightLine(tc))
			result = false;

	goCursor(clin, cCol);

	return result;
}

bool DaliView::shiftLeftLine(DaliTextCursor &tc, bool &blank)
{
	idCheck();
	blank = false;
	goCursor(tc.order(), 0);

	if (tc.value().length() == 0)
		return true;

	if (isSpace(tc.getChar())) {
		if (tc.getChar() == ' ')
			blank = true;
		delChar();
		// goFirstNonSp();
		return true;
	}

	return false;
}

bool DaliView::shiftLeft(bool &symetric)
{
	idCheck();
	Int from, to;

	// estoy en modo bloque
	if (block) {
		if (block < cursor) {
			from = block.order();
			to = cursor.order();

			// Correct when last position is at first character of
			// a line
			if (cursor.column() == 0)
				to--;
		} else {
			from = cursor.order();
			to = block.order();

			// Correct when last position is at first character of
			// a line
			if (block.column() == 0)
				to--;
		}
	} else
		if (cursor)
			from = to = cursor.order();
		else
			return false;

	Int cCol = cursor.column(), clin = cursor.order();

	symetric = true;
	bool didSomething = false, blank;

	DaliTextCursor tc = cursor;
	for (tc.goTo(from); tc.order() <= to; tc++)
		if (shiftLeftLine(tc, blank)) {
			didSomething = true;

			// If a blank was erased, the operation will not be symetric
			// (with shiftRight)
			if (blank)
				symetric = false;
		} else
			symetric = false;

	goCursor(clin, cCol);

	return didSomething;
}

Int DaliView::insNewLine(bool deflt)
{
	idCheck();
	bool scr = !cursor.column();

	Int col = cursor.insNewLine(deflt);

	c_orig = 0;

	if (!actBotOrigin() && scr && !(mode&DSPCTROL)) {
		insLines();
		goToCursor();	
		writeStLine();
		updateOtherViews();
	}
	else redraw();

	return col;
}

bool DaliView::delLine()
{
	idCheck();
	if (cursor) {
		cursor.del();
		redraw();
		return true;
	} else {
		beep();
		return false;
	}
}

void DaliView::appendLine(const String &txt)
{
	idCheck();
	text->add(txt);
	if (!goCursor(text->size(), 0))
		redraw();
	refresh();
}

bool DaliView::insLine(const String &txt)
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

void DaliView::setLine(const String &txt)
{
	idCheck();
	cursor.set(txt);
	redrawCurrLine();
}

bool DaliView::joinLines()
{
	idCheck();
	// no pude hacerlo
	if (!cursor.joinLines()) return false;

	actColOrigin();
	cursor.reAssignNextTab(cursor.column());
	actTopOrigin();
	redraw();
	return true;
}

/*
	Returns true if the command deleted a line and false
	otherwise.
*/
bool DaliView::delWord()
{
	idCheck();
	if (cursor) {
		Int curcol = cursor.column();
		Int row, col;
		findNextWord(1, row, col);
		/*
			If the word is on the same line, delete the range and
			that's it.
		*/
		if (row == cursor.order()) {
			col--;
			cursor.delRange(curcol, col);
			redrawCurrLine();
			return false;
		} else {
			/*
				If we are not past the EOL, then delete up to EOL.
			*/
			if (curcol < cursor.value().length()) {
				delEndOfLine();
				return false;
			} else {
				/*
					If the next word is not on the line below, delete
					the whole line.
				*/
				DaliTextCursor auxTc = cursor;
				if (row >= cursor.order()+2) {
					auxTc++;
					auxTc.del();
				} else {
					/*
						If we are past the EOL, then add spaces from
						EOL to cursor position.
					*/
					if (auxTc.column() > cursor.value().length()) {
						auxTc.moveCol(-1);
						auxTc.insChar(' ');
					}
					/*
						Now place (row, col) pointing to the last
						character to delete.
					*/
					col--;
					if (col < 0) {
						row--;
						col = cursor.value().length();
					}
					/*
						Delete from cursor position up to the next word.
					*/
					cursor.delTo(row, col);
				}
				redraw();
				return true;
			}
		}
	} else
		return false;
}

bool DaliView::delEndOfLine()
{
	idCheck();
	if (cursor) {
		cursor.delRange(cursor.column(), cursor.value().length()-1);
		redrawCurrLine();
		return true;
	} else {
		beep();
		return false;
	}
}

Int DaliView::replForwText(RegExp& re,String& newtext, String& oldtext,
						Int filfrom, Int filto, bool &conf,
						bool& didIt, bool dontDoIt)
{
	idCheck();
	UnChar c;
	Int res = findForwText(re, filfrom, filto, false);

	oldtext = NULL_STRING;
	didIt = false;

	if (res == ERR) return ERR;

	if (conf) {       
		DaliTextCursor tcAux = cursor;
		tcAux.moveCol(res-1);
		text->setAttr(cursor, tcAux, A_STANDOUT);
		redraw();

		*this >> c;

		text->delAttr(cursor, tcAux, A_STANDOUT);
		redrawCurrLine();

		switch (c) {
		case ' ':  
			conf = false;
			break;
		case K_ENTER:
			break;
		case K_CURS_DOWN:
			cursor.moveCol(res == 0 ? 1 : res);
			redraw();
			return res;
		case K_END:
			return ERR-1;
		default:
			Editor::beep();
			return OK;
		}
	}

	if (res)
		oldtext = cursor.value()(cursor.column(), res);
	else
		oldtext = NULL_STRING;

	if (dontDoIt)
		cursor.moveCol(oldtext.length());
	else {
		didIt = true;
		newtext = re.expand(newtext);
		chgStrCursor(res, newtext);
		cursor.moveCol(newtext.length());
	}

	redraw();
	return res;
}

void DaliView::getRegion(List(String) &l, DaliTextCursor &tc)
{
	idCheck();
	if (!tc || tc == cursor)
		return;

	DaliTextCursor aux = tc;
	DaliTextCursor aux1 = cursor;

	if (!cursor || cursor > tc)
	{
		// Bug del cut & paste en un bloque. BZ 2446
		//text->getRegion(l, tc, cursor, false);
		text->getRegion(l, aux, aux1, false);
	}
	else
	{
		// BZ 2446
		//text->getRegion(l, cursor, tc, true);
		text->getRegion(l, aux1, aux, true);
	}
		
}

void DaliView::insertText(List(String) &l)
{
	idCheck();
	if (!text->insertText(l, cursor))
		return;

	if (l.size()==1)
		redrawCurrLine();
	else
		redraw();
}
		
void DaliView::delRegion(DaliTextCursor &tc, bool compress)
{
	idCheck();
	// si no hay que marcar me voy
	if (!tc || tc == cursor)
		return;

	bool res;

	if (!cursor || cursor > tc)
		res = text->delRegion(tc, cursor, compress, true);
	else
		res = text->delRegion(cursor, tc, compress, false);

	if (res) redraw();
}

// end of viewmod
