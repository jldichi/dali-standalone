/********************************************************************
* Copyright (c) 1993 InterSoft Co.  All Rights Reserved
* THIS IS UNPUBLISHED PROPRIETARY SOURCE CODE OF InterSoft Co.
* The copyright notice above does not evidence any
* actual or intended publication of such source code.
*
* $Id: viewpos.cc,v 1.1.1.1 1996/04/16 18:52:47 gustavof Exp $
*
* DESCRIPTION: Implementation of movement methods for class View
*********************************************************************/

#include <ifound.h>
#include <ifound/message.h>
#include <ifound/stdc.h>
#include <ifound/rexp.h>
#include <winapp/window.h>
#include <winapp/strhis.h>
#include <winapp/view.h>
#include <winapp/text.h>

#include <sys/types.h>
#include <sys/stat.h>

#define cursor (*_cursor)

bool View::cursDown(Int i)
{
	idCheck();

	Int size = text->size();

	if (size == 0) size = 1;

	// si me voy fuera de rango
	if (cursor.order() + i > size+nfils-2) {                                                 
		beep();
		return false;
	}

	actCurrLine(i);

	return false;
}

bool View::cursUp(Int i)
{
	idCheck();

	// me fui de rango
	if (cursor.order()-i < 0) {
		beep();
		return false;
	}

	actCurrLine(-i);

	return false;
}

bool View::cursLeft(Int i, bool wstln)
{
	idCheck();

	if (cursor.column() == 0 && cursor.order() == 0) {
		beep();
		return false;
	}

	cursor.moveCol(-i, true);

	if (!actColOrigin() && !actTopOrigin()) {
		if (wstln)  
		    writeStLine();       
		else
		    goToCursor();
		return false;
	}

	redraw();
	return true;
}

bool View::cursRight(Int i, bool wstln)
{
	idCheck();

	cursor.moveCol(i);

	if (!actColOrigin()) {
		if (wstln) 
			writeStLine();  
		else
			goToCursor();	
		return false;
	}

	redraw();
	return true;
}

bool View::wordRight(Int i)
{
	idCheck();

	Int row, col;

	findNextWord(i, row, col);

	if (row == ERR) {
		beep();
		return false;
	}

	return goCursor(row, col);
}
   
bool View::wordLeft(Int i)
{
	idCheck();

	Int row, col;

	findPrevWord(i, row, col);

	if (row == ERR) {
		beep();
		return false;
	}

	return goCursor(row, col);
}

bool View::goFirstNonSp()
{
	idCheck();

	cursor.goFirstNonSp();

	if (!actColOrigin()) {
		goToCursor();	
		return false;
	}

	redraw();
	return true;
}

void View::goToBegOfFile()
{
	idCheck();

	cursor.goToFirst();
	c_orig = 0;
	f_orig = 0;
    redraw();
	goToCursor();	
}

void View::goToEndOfFile()
{
	idCheck();

	cursor.goToLast();
    f_orig = cursor.order() - nfils + 1;                     
    c_orig = 0;
    redraw();
	goToCursor();	
}

void View::goToEndOfScreen()
{
	idCheck();

	cursor.goTo(f_orig+nfils-1, cursor.column());
	goToCursor();
}

void View::goToMiddleOfScreen()
{
	idCheck();

	cursor.goTo(f_orig+(nfils-1)/2, cursor.column());
	goToCursor();
}

void View::goToBegOfScreen()
{
	idCheck();

	cursor.goTo(f_orig, cursor.column());
	goToCursor();
}

void View::pageDown(Int i)
{
	idCheck();

	Int aux   = cursor.order()+(i*nfils);
	Int size  = text->size();

	if (size == 0) size = 1;

	Int limit = size+nfils-2;

	if (aux <= limit) {
		Int aux1 = f_orig + aux - cursor.order();
		f_orig = aux1 <= size-1 ? aux1 : size-1;
		cursor.goTo(aux);
	}
	else {
		f_orig = size-1;
		cursor.goTo(limit);
		beep();
	}

	redraw();
}

void View::pageUp(Int i)
{
	idCheck();

	Int aux  = cursor.order()-(i*nfils);
		
	if (aux >= 0) {
		Int aux1 = f_orig - cursor.order() + aux;
		f_orig = aux1 >= 0 ? aux1 : 0;
		cursor.goTo(aux);
	}
	else {
		f_orig = 0;
		cursor.goTo(0);
		beep();
	}
		
	redraw();
}

void View::adjustRanges(Int &from, Int &to)
{
	idCheck();

	if (to == ERR)
		to = text->size();

	if (from > to) {
		Int tmp = from;
		from = to;
		to = tmp;
	}

	if (from < 1)
		from = 1;
	else if (from > text->size())
		from = text->size();

	if (to < 1)
		to = 1;
	else if (to > text->size())
		to = text->size();
}

void View::findNextWord(Int i, Int &row, Int &col)
{
	idCheck();

	if (!(cursor)) {
		row = col = ERR;
		return;
	} else {
		if (i < 0) {
			findPrevWord(-i, row, col);
			return;
		}

		TextCursor tc = cursor;

		Int len = tc.value().length();

		Int j;

		while (i-- && tc) {

			// salteo los caracteres <> de blanco
			for (j = tc.column(); j < len && !isSeparator(tc.value()[j]);
					j++);

			// recorro a partir de ahi y salteo lineas si hace falta
			// para encontrar el proximo <> de blanco
			for (;tc; tc++, j = 0) {
				len = tc.value().length();

				for (; j < len && isSeparator(tc.value()[j]); j++)
					;

				if (j < len)
					break;
			}

			// lo encontre
			if (tc)
				col = j;
		}				

		// si no lo encontre entonces lo coloco al final del archivo
		if (!tc) {
			tc--;
			col = tc.value().length();
		}

		row = tc.order();
	}
}

void View::findPrevWord(Int i, Int &row, Int &col)
{
	idCheck();

	if (!(cursor)) {
		row = col = ERR;
		return;
	} else {
		if (i < 0) {
			findNextWord(-i, row, col);
			return;
		}

		TextCursor tc = cursor;

		Int j;

		while (i-- && tc) {

			j = tc.column()-1;

			while (tc) {

				// salteo los caracteres igual a blanco
				for (; j >= 0 && isSeparator(tc.value()[j]); j--);

				if (j >= 0) {
					for (; j >= 0 && !isSeparator(tc.value()[j]); j--);
					j++;
					break;
				}

				tc--;
				if (tc)
					j = tc.value().length()-1;
			}

			// lo encontre
			if (tc)
				col = j;
		}				

		// si no lo encontre entonces lo coloco principio del archivo
		if (!tc) {
			tc++;
			col = 0;
		}

		row = tc.order();
	}
}

// end of view movement
