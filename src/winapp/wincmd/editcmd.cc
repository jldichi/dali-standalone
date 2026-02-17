/********************************************************************
* Copyright (c) 1993 InterSoft Co.  All Rights Reserved
* THIS IS UNPUBLISHED PROPRIETARY SOURCE CODE OF InterSoft Co.
* The copyright notice above does not evidence any
* actual or intended publication of such source code.
*
* $Id: editcmd.cc,v 1.1.1.1 1996/04/16 18:52:47 gustavof Exp $
*
* DESCRIPTION: Implementation of edition commands
*********************************************************************/
#ifdef __GNUC__
#pragma implementation
#endif

#define INITCOMM
#include <winapp/winapp.h>
#include <winapp/wincmd.h>
#include <winapp/editcmd.h>
#include <winapp/view.h>
#include <winapp/text.h>
#include <winapp/menu.h>
#include <ifound/itypes.h>
#include <winapp/window.h>
#include <winapp/cmdbind.h>

void bindEditCmds(CmdBinding *cb)
{
	UnShort i;

	for (i = 0; i < MAX_COMMAND; i++) {
		if (isprint(i)) {
			cb->bind(i, EditInsChar::cmdId);
		}			                   
	}		

	cb->bind(EditEnter::eventId,			EditEnter::cmdId);
	cb->bind(EditAddNewLine::eventId,		EditAddNewLine::cmdId);
	cb->bind(EditInsNewLine::eventId,		EditInsNewLine::cmdId);
	cb->bind(EditJoinLines::eventId,		EditJoinLines::cmdId);
	cb->bind(EditInsChar::eventId,			EditInsChar::cmdId);
	cb->bind(EditDelChar::eventId,			EditDelChar::cmdId);
	cb->bind(EditBackSpace::eventId,		EditBackSpace::cmdId);
	cb->bind(EditDelLine::eventId,			EditDelLine::cmdId);
	cb->bind(EditDelWord::eventId,			EditDelWord::cmdId);
	cb->bind(EditDelEndOfLine::eventId,		EditDelEndOfLine::cmdId);
	cb->bind(EditInsAsciiChar::eventId,		EditInsAsciiChar::cmdId);
	cb->bind(EditTab::eventId,				EditTab::cmdId);
	cb->bind(EditBackTab::eventId,			EditBackTab::cmdId);

	cb->bind(ChgInsMode::eventId,				ChgInsMode::cmdId);
	cb->bind(ChgCtrolMode::eventId,				ChgCtrolMode::cmdId);
}   


void doInsChar(EditableView *cview, UnChar chr, bool &insertMode, Int &line,
				Int &col, UnChar &character, String &replaced)
{    
	assert(cview);

  	EditableTextCursor *tc;
  	tc = cview->edCursor();

	Int theCol = cview->getCol();

	insertMode = cview->inInsMode();
	character = tc->value()[theCol];

	/*
		If we are inserting in the middle of a tab, save whatever
		we had to our left.
	*/

	if (tc->value()[theCol-1] == FILL_TAB) {
		Int toSaveLeft = Text::tabSize()-NFILLTAB(theCol)-1;
		if (toSaveLeft > 0)
			replaced = tc->value()(theCol-toSaveLeft, toSaveLeft);
		else
			replaced = NULL_STRING;
	} else
		replaced = NULL_STRING;
	/*
		We must consider the case when characters (spaces or newlines)
		will be automatically inserted by the insChar method.

		Are we beyond the last line?
	*/
	if (cview->getRow() > cview->sizeOfText()-1) {
		/*
			Save text's last line & column
		*/
		line = cview->sizeOfText()-1;
		if (line == -1) {
			line = 0;
			col = 0;
		} else {
			EditableTextCursor tcAux = *tc;
			tcAux.goTo(line);
			col = tcAux.value().length();
		}
	} else {
		/*
			Indicate we were not beyond last line
		*/
		line = ERR;
		if (cview->getCol() > tc->value().length())
			col = tc->value().length();
		else
			col = ERR;
	}
		
	cview->insChar(chr);
}

void undoInsChar(EditableView *cview, bool &insertMode,
		Int &line, Int &col, UnChar &character, String &replaced)
{
	assert(cview);

  	EditableTextCursor *tc = cview->edCursor();

	if (line == ERR) {
		if (col == ERR) {
			tc->moveCol(-1);
			tc->delChar();
			/*
				If we are in overwrite mode and the character
				overwritten is not a FILL_TAB, restore it.
			*/
			if (!insertMode && character != FILL_TAB)
				tc->insChar(character);

			Int theCol = cview->getCol();
			/*
				If something might have changed to our left (FILL_TAB's
				converted to blanks), restore it.
			*/
			if (replaced != NULL_STRING)
				tc->rebuildTab(replaced);

			tc->reAssignNextTab(theCol);
			cview->redrawCurrLine();
		} else {
			Int saveCol = cview->getCol() - 1;
			EditableTextCursor tcAux = *tc;
			tcAux.delRange(col, saveCol);
			tc->goToCol(saveCol);
			cview->redrawCurrLine();
		}
	} else {
		Int	r = cview->getRow();
		Int	c = cview->getCol();

		EditableTextCursor tcAux = *tc;

		tcAux.goTo(line, col);
		tcAux.delTo(cview->getRow(), cview->getCol());//Por que no usar r y c?
		tc->goTo(r, c);
		tc->moveCol(-1);
		cview->redrawCurrLine();
	}
}

bool EditInsChar::redo()
{
	idCheck();

	EditableView *view = appi->getDesktop()->getCurrEdView();

	doInsChar(view, chr, insertMode, line, col, character, replaced);
	return true;
}

void EditInsChar::undo()
{
	idCheck();

	EditableView *view = appi->getDesktop()->getCurrEdView();
	undoInsChar(view, insertMode, line, col, character, replaced);
}



bool EditInsAsciiChar::redo()
{
	idCheck();      
#if 0	
	if (isCleared) {
		ScrollMenu sm(fpReadLine, 15, 19);
		if (sm.execute() == REJECT_EXIT)
			return false;
		
		chr = UnChar(sm.currOpt());
		isCleared = false;
	}

	doInsChar(chr, cursor(), insertMode, line, col, character, replaced);
#endif
	return true;
}

void EditInsAsciiChar::clear()
{
	idCheck();
}


void EditInsAsciiChar::undo()
{
	idCheck();

//	undoInsChar(Cursor(), insertMode, line, col, character,
//			replaced);
}




bool EditDelChar::redo()
{
	idCheck();

	EditableView *view = appi->getDesktop()->getCurrEdView();

	EditableTextCursor *tc = view->edCursor();
	Int theCol = tc->column();
	theChar = tc->value()[theCol];
	if (theChar == '\t' || theChar == FILL_TAB) {
		Int toSaveLeft = Text::tabSize()-NFILLTAB(theCol)-1;
		if (toSaveLeft > 0)
			replaced = tc->value()(theCol-toSaveLeft, toSaveLeft);
		else
			replaced = NULL_STRING;
	} else
		replaced = NULL_STRING;

	return !view->delChar() && !theChar ? false : true;
}

void EditDelChar::undo()
{
	idCheck();

	EditableView *view = appi->getDesktop()->getCurrEdView();

	// debo volver atras el join
	if (theChar == '\0') {
		Int c = view->getCol();
		Int f = view->getRow();
		view->edCursor()->insNewLine(false);
		view->edCursor()->goTo(f, c);
		view->View::redraw();
	}		
	else {
		EditableTextCursor *tc = view->edCursor();
		if (replaced != NULL_STRING)
			tc->rebuildTab(replaced);
		if (theChar == FILL_TAB)
			tc->insChar('\t');
		else
			tc->insChar(theChar);
		tc->reAssignNextTab(tc->column(),2);
		view->redrawCurrLine();
	}
}

bool EditDelLine::redo()
{
	idCheck();

	EditableView *view = appi->getDesktop()->getCurrEdView();
	
	text = view->edCursor()->value();
	offset = view->getCol();

	bool retValue = view->delLine();

	if (retValue != false)
		view->goCursor(view->getRow(), 0);

	return retValue;
}

void EditDelLine::undo()
{
	idCheck();

	EditableView *view = appi->getDesktop()->getCurrEdView();
	
	(void) view->insLine(text);
	view->goCursor(view->getRow(), offset);
}

bool EditDelWord::redo()
{
	idCheck();

	EditableView *view = appi->getDesktop()->getCurrEdView();
	
	EditableTextCursor auxTc = *view->edCursor();

	offset = auxTc.column();
	text1 = auxTc.value();
	auxTc++;
	text2 = auxTc.value();
	twoLines = view->delWord();

	return true;
}

void EditDelWord::undo()
{
	idCheck();

	EditableView *view = appi->getDesktop()->getCurrEdView();

// bien??
	EditableTextCursor auxTc = *view->edCursor();

	auxTc.set(text1);
	if (twoLines)
		auxTc.add(text2);


	view->goCursor(view->getRow(), offset);
	view->View::redraw();
}

bool EditDelEndOfLine::redo()
{
	idCheck();

	EditableView *view = appi->getDesktop()->getCurrEdView();

	EditableTextCursor* tc = (view->edCursor());

	Int column = tc->column();
	text = tc->value()(column, tc->value().length() - column);

	return view->delEndOfLine();
}

void EditDelEndOfLine::undo()
{
	idCheck();

	EditableView *view = appi->getDesktop()->getCurrEdView();
	
	view->setLine(view->edCursor()->value() + text);
}

bool EditBackSpace::redo()
{
	idCheck();
 
	EditableView *view = appi->getDesktop()->getCurrEdView();

 	EditableTextCursor *tc = view->edCursor();

	Int theCol = tc->column()-1;
	theChar = tc->value()[theCol];

	if (theChar == '\t' || theChar == FILL_TAB) {
		Int toSaveLeft = Text::tabSize()-NFILLTAB(theCol)-1;
		if (toSaveLeft > 0)
			replaced = tc->value()(theCol-toSaveLeft, toSaveLeft);
		else
			replaced = NULL_STRING;
	} else
		replaced = NULL_STRING;

	isJoin = view->backSpace();

	return true;
}

void EditBackSpace::undo()
{
	idCheck();

	EditableView *view = appi->getDesktop()->getCurrEdView();

	// debo hacer Enter
	if (isJoin) {
		view->insNewLine(false);
		return;
	}

	EditableTextCursor *tc = view->edCursor();

	if (theChar == '\t' || theChar == FILL_TAB || theChar == '\0') {
		if (replaced != NULL_STRING)
			tc->rebuildTab(replaced);
		if (theChar != '\0')
			tc->insChar('\t');
		tc->reAssignNextTab(tc->column(), 2);
		tc->moveCol(1);
		view->redrawCurrLine();
	} else
		view->insChar(theChar);
}

bool EditEnter::redo()
{
	idCheck();

	EditableView *view = appi->getDesktop()->getCurrEdView();

	indent = view->insNewLine();

	return true;
}

void EditEnter::undo()
{
	idCheck();

	EditableView *view = appi->getDesktop()->getCurrEdView();

	view->edCursor()->goToCol(0);
	view->edCursor()->delRange(0, indent-1);

	view->backSpace();
}

bool EditTab::redo()
{
	idCheck();

	EditableView *view = appi->getDesktop()->getCurrEdView();

    if(view->inInsMode()) 
		view->tab();
    else 
	    view->cursRight(Text::tabSize());

	return true;
}

void EditTab::undo()
{
	idCheck();

	EditableView *view = appi->getDesktop()->getCurrEdView();

    if(view->inInsMode()) 
		view->backTab();
    else 
	    view->cursLeft(Text::tabSize());
}

bool ChgInsMode::redo()
{
	idCheck();

	EditableView *view = appi->getDesktop()->getCurrEdView();

	view->chgInsMode(true);
	return true;
}

void ChgInsMode::undo() 
{
	idCheck();

	EditableView *view = appi->getDesktop()->getCurrEdView();

	view->chgInsMode(false);
}

bool EditAddNewLine::redo()
{
	idCheck();

	EditableView *view = appi->getDesktop()->getCurrEdView();

	urow = view->getRow();
	ucol = view->getCol();

	Int offset = view->edCursor()->value().length() - view->getCol();
	view->cursRight(offset);

	indent = view->insNewLine();

	// nota: Aqui se reseteaba el modo del editor a NULL_MODE	

	return true;
}

void EditAddNewLine::undo()
{
	idCheck();

	EditableView *view = appi->getDesktop()->getCurrEdView();

	view->edCursor()->goToCol(0);
	view->edCursor()->delRange(0, indent-1);
	view->backSpace();
	view->goCursor(urow, ucol);
}

bool EditInsNewLine::redo()
{
	idCheck();

	EditableView *view = appi->getDesktop()->getCurrEdView();

	urow = view->getRow();
	ucol = view->getCol();

	view->goFirstNonSp();
	indent = view->insNewLine();
	view->cursUp();

	// nota: Aqui se reseteaba el modo del editor a NULL_MODE	

	return true;
}

void EditInsNewLine::undo()
{
	idCheck();

	EditableView *view = appi->getDesktop()->getCurrEdView();

	view->edCursor()->goToCol(0);
	view->edCursor()->delRange(0, indent-1);

	view->backSpace();
	view->goCursor(urow, ucol);
}

bool EditJoinLines::redo()
{
	idCheck();

	EditableView *view = appi->getDesktop()->getCurrEdView();

	urow = view->getRow();
	ucol = view->getCol();

	return view->joinLines();
}                    

void EditJoinLines::undo()
{
	idCheck();

	EditableView *view = appi->getDesktop()->getCurrEdView();

	view->insNewLine(false);
	view->goCursor(urow, ucol);
}

bool ChgCtrolMode::redo()
{
	idCheck();

	EditableView *view = appi->getDesktop()->getCurrEdView();

	view->chgCtrolMode();

	return true;
}

void ChgCtrolMode::undo() 
{ 
	idCheck();

//	EditableView *view = appi->getDesktop()->getCurrEdView();

//	view->chgCtrolMode(); ???
}

bool EditBackTab::redo()
{
	idCheck();

	EditableView *view = appi->getDesktop()->getCurrEdView();

	return view->backTab();
}

void EditBackTab::undo()
{
	idCheck();

	EditableView *view = appi->getDesktop()->getCurrEdView();

	view->tab();
}


