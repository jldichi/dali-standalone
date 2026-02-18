/********************************************************************
* Copyright (c) 1991 InterSoft Co.  All Rights Reserved
* THIS IS UNPUBLISHED PROPRIETARY SOURCE CODE OF InterSoft Co.
* The copyright notice above does not evidence any
* actual or intended publication of such source code.
*
* $Id: bascomm.cc,v 1.1.1.1 1996/04/16 18:52:50 gustavof Exp $
*
* DESCRIPTION: Implementation of command class
*********************************************************************/
#ifdef __GNUG__
#	pragma implementation
#endif

// para que genere la entrada dentro de la tabla de comandos
#define INITCOMM

#include <command.h>
#include <bascomm.h>
#include <editor.h>
#include <dview.h>
#include <dtext.h>
#include <winapp/menu.h>
#include <dsensor.h>
#include <ifound/itypes.h>
#include <winapp/window.h>

declare(PtrList,DaliView);
declare(PtrListCursor,DaliView);

// clase InsChar e InsAsciiChar

void doInsChar(UnChar chr, DaliTextCursor &tc, bool &insertMode,
		Int &line, Int &col, UnChar &character, String &replaced)
{           
	DaliView *cview = cView.value();
	Int theCol = tc.column();

	insertMode = cview->inInsMode();
	character = tc.value()[theCol];
	/*
		If we are inserting in the middle of a tab, save whatever
		we had to our left.
	*/
	if (tc.value()[theCol-1] == FILL_TAB) {
		Int toSaveLeft = DaliText::tabSize()-NFILLTAB(theCol)-1;
		if (toSaveLeft > 0)
			replaced = tc.value()(theCol-toSaveLeft, toSaveLeft);
		else
			replaced = NULL_STRING;
	} else
		replaced = NULL_STRING;
	/*
		We must consider the case when characters (spaces or newlines)
		will be automatically inserted by the insChar method.

		Are we beyond the last line?
	*/
	if (tc.order() > cview->sizeOfText()-1) {
		/*
			Save text's last line & column
		*/
		line = cview->sizeOfText()-1;
		if (line == -1) {
			line = 0;
			col = 0;
		} else {
			DaliTextCursor tcAux = tc;
			tcAux.goTo(line);
			col = tcAux.value().length();
		}
	} else {
		/*
			Indicate we were not beyond last line
		*/
		line = ERR;
		if (tc.column() > tc.value().length())
			col = tc.value().length();
		else
			col = ERR;
	}
		
	cview->insChar(chr);
}

void undoInsChar(DaliTextCursor &tc, bool &insertMode,
		Int &line, Int &col, UnChar &character, String &replaced)
{
	DaliView *cview = cView.value();
	if (line == ERR) {
		if (col == ERR) {
			tc.moveCol(-1);
			tc.delChar();
			/*
				If we are in overwrite mode and the character
				overwritten is not a FILL_TAB, restore it.
			*/
			if (!insertMode && character != FILL_TAB)
				tc.insChar(character);

			Int theCol = tc.column();
			/*
				If something might have changed to our left (FILL_TAB's
				converted to blanks), restore it.
			*/
			if (replaced != NULL_STRING)
				tc.rebuildTab(replaced);

			tc.reAssignNextTab(theCol);
			cview->redrawCurrLine();
		} else {
			Int saveCol = tc.column()-1;
			DaliTextCursor tcAux = tc;
			tcAux.delRange(col, saveCol);
			tc.goToCol(saveCol);
			cview->redrawCurrLine();
		}
	} else {
		Int	r = tc.order(),
			c = tc.column();

		DaliTextCursor tcAux = tc;
		tcAux.goTo(line, col);
		tcAux.delTo(tc.order(), tc.column());
		tc.goTo(r, c);
		tc.moveCol(-1);
		cview->redrawCurrLine();
	}
}

bool InsChar::redo()
{
	idCheck();
	doInsChar(chr, cursor(), insertMode, line, col, character,
			replaced);
	return true;
}

void InsChar::undo()
{
	idCheck();
	undoInsChar(cursor(), insertMode, line, col, character,
			replaced);
}

static const String fpReadLine(Int n)
{
	if (n < 0 || n > 255) return HIGH_VALUE;
	
    // Esto quedo asi despues del port a IRIX (Silicon Graphics)
    // Era muy complicado para el compilador
	String temp;
	temp = toString(n,-3,-10);
	temp += " (";
	temp += toString(n,-2,16);
	temp += ") : '";
	temp += String(1,n < ' '||(n>125 && n<160) ? C_SHADOW : char(n));
	temp += "'";
	return temp; 
}

void InsAsciiChar::clear()
{
	idCheck();
	isCleared = true;
}

bool InsAsciiChar::redo()
{
	idCheck();      
	
	if (isCleared) {
		ScrollMenu sm(fpReadLine, 15, 19);
		if (sm.execute() == REJECT_EXIT)
			return false;
		
		chr = UnChar(sm.currOpt());
		isCleared = false;
	}

	doInsChar(chr, cursor(), insertMode, line, col, character, replaced);

	return true;
}

void InsAsciiChar::undo()
{
	idCheck();
	undoInsChar(cursor(), insertMode, line, col, character,
			replaced);
}

// end clase InsChar

// clase DelChar

bool DelChar::redo()
{
	idCheck();
	DaliTextCursor *tc = &cursor();
	Int theCol = tc->column();
	theChar = tc->value()[theCol];
	if (theChar == '\t' || theChar == FILL_TAB) {
		Int toSaveLeft = DaliText::tabSize()-NFILLTAB(theCol)-1;
		if (toSaveLeft > 0)
			replaced = tc->value()(theCol-toSaveLeft, toSaveLeft);
		else
			replaced = NULL_STRING;
	} else
		replaced = NULL_STRING;
	bool res = cView.value()->delChar();
	return !res && !theChar ? false : true;
}

void DelChar::undo()
{
	idCheck();
	// debo volver atras el join
	if (theChar == '\0') {
		Int c = cursor().column();
		Int f = cursor().order();
		cursor().insNewLine(false);
		cursor().goTo(f, c);
		cView.value()->redraw();
	}		
	else {
		DaliView *cview = cView.value();
		DaliTextCursor *tc = &cursor();
		if (replaced != NULL_STRING)
			tc->rebuildTab(replaced);
		if (theChar == FILL_TAB)
			tc->insChar('\t');
		else
			tc->insChar(theChar);
		tc->reAssignNextTab(tc->column(),2);
		cview->redrawCurrLine();
	}
}

// end clase DelChar

// clase DelLine

bool DelLine::redo()
{
	idCheck();
	DaliView *cview = cView.value();

	text = cursor().value();
	offset = cursor().column();
	bool retValue = cview->delLine();
	if (retValue != false)
		cview->goCursor(cursor().order(), 0);
	return retValue;
}

void DelLine::undo()
{
	idCheck();
	DaliView *cview = cView.value();
	(void) cview->insLine(text);
	cview->goCursor(cursor().order(), offset);
}

// end clase DelLine

// clase DelWord

bool DelWord::redo()
{
	idCheck();
	DaliTextCursor auxTc = cursor();
	offset = auxTc.column();
	text1 = auxTc.value();
	auxTc++;
	text2 = auxTc.value();
	twoLines = cView.value()->delWord();
	return true;
}

void DelWord::undo()
{
	idCheck();
	DaliView *cview = cView.value();

	DaliTextCursor auxTc = cursor();
	auxTc.set(text1);
	if (twoLines)
		auxTc.add(text2);
	cview->goCursor(cursor().order(), offset);
	cview->redraw();
}

// end clase DelWord

// clase DelEndOfLine

bool DelEndOfLine::redo()
{
	idCheck();
	DaliTextCursor* tc = &(cursor());
	Int column = tc->column();
	text = tc->value()(column, tc->value().length() - column);
	return cView.value()->delEndOfLine();
}

void DelEndOfLine::undo()
{
	idCheck();
	cView.value()->setLine(cursor().value() + text);
}

// end clase DelEndOfLine

// clase DelEndOfLineWithIns

bool DelEndOfLineWithIns::redo()
{
	idCheck();
	DaliTextCursor* tc = &(cursor());
	Int column = tc->column();
	text = tc->value()(column, tc->value().length() - column);
	if (cView.value()->delEndOfLine()) {
		Editor::setMode(NULL_MODE);
		return true;
	}
	return false;
}

void DelEndOfLineWithIns::undo()
{
	idCheck();
	cView.value()->setLine(cursor().value() + text);
}

// end clase DelEndOfLineWithIns

// clase BackSpace

bool BackSpace::redo()
{
	idCheck();
	DaliTextCursor *tc = &cursor();
	Int theCol = tc->column()-1;
	theChar = tc->value()[theCol];
	if (theChar == '\t' || theChar == FILL_TAB) {
		Int toSaveLeft = DaliText::tabSize()-NFILLTAB(theCol)-1;
		if (toSaveLeft > 0)
			replaced = tc->value()(theCol-toSaveLeft, toSaveLeft);
		else
			replaced = NULL_STRING;
	} else
		replaced = NULL_STRING;

	isJoin = cView.value()->backSpace();
	return true;
}

void BackSpace::undo()
{
	idCheck();
	// debo hacer Enter
	if (isJoin) {
		cView.value()->insNewLine(false);
		return;
	}

	DaliTextCursor *tc = &cursor();

	if (theChar == '\t' || theChar == FILL_TAB || theChar == '\0') {
		if (replaced != NULL_STRING)
			tc->rebuildTab(replaced);
		if (theChar != '\0')
			tc->insChar('\t');
		tc->reAssignNextTab(tc->column(), 2);
		tc->moveCol(1);
		cView.value()->redrawCurrLine();
	} else
		cView.value()->insChar(theChar);
}
// end clase BackSpace

// clase Enter

bool Enter::redo()
{
	idCheck();
	indent = cView.value()->insNewLine();
	return true;
}

void Enter::undo()
{
	idCheck();
	cursor().goToCol(0);
	cursor().delRange(0, indent-1);
	cView.value()->backSpace();
}

// end clase Enter

#include <stdio.h>
bool BraceBlock::redo()
{              
	idCheck();
    Int indent;
    Int cline 	= cursor().order();
    offset 		= cursor().column(); 
	text 		= cursor().value();
    
    // Search for correct column to insert '{'
 	Int i;
 	for (i = text.length() - 1; i > 0 && isSpace(text[i]); i--)
 		;

 	if (i>0) {
 	    cursor().goToCol(i+1);
 		cView->insChar(' ');
 	}	
 	else
 		cursor().goToCol(offset);	
   
	// The opening-brace		              
	cView->insChar('{');
	
    // Insert TWO New Lines
	indent 	= cView->insNewLine();
	indent 	= cView->insNewLine();
	
	// The closing-brace
	cView->insChar('}');

	cursor().goTo(cline+1,indent);

	// Place a Tab		
	cView->tab();		
	
	return true;
}                     

void BraceBlock::undo()
{     
	idCheck();
	cursor().del();
  	cursor().del();  
    cursor()--;
  	cursor().set(text);
 	cursor().goToCol(offset);
 	cView->redraw();
}
