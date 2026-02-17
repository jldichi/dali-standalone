/********************************************************************
* Copyright (c) 1991 InterSoft Co.  All Rights Reserved
* THIS IS UNPUBLISHED PROPRIETARY SOURCE CODE OF InterSoft Co.
* The copyright notice above does not evidence any
* actual or intended publication of such source code.
*
* $Id: commdef.cc,v 1.2 2000/01/31 15:37:43 diegon Exp $
*
* DESCRIPTION: Implementation of command class
*********************************************************************/

#ifdef __GNUC__
#pragma implementation
#endif

// para que genere la entrada dentro de la tabla de comandos
#define INITCOMM

#include <command.h>
#include <commdef.h>
#include <editor.h>
#include <dview.h>
#include <dtext.h>
#include <dsensor.h>
#include <local/strfile.h>
#include <winapp/strhis.h>

declare(PtrList,DaliView);
declare(PtrListCursor,DaliView);

// clase History

void CommHis::undo()
{ 
	idCheck();
}

bool CommHis::redo()
{
	idCheck();
	Int cid;
	Command *theCmd = NULL;
	UnChar c;                  
    
	String stUndo = Editor::editorMsg("UNDO")+"["+
					toString(Editor::his->currLevel())+"/"+
					toString(Editor::his->levels())+"]";
	cView.value()->writeStLine(stUndo);

	while ((c = Editor::getKey()) != K_UNDO && c != K_END) {
		switch(c) {
		case K_CURS_UP:
			if (Editor::his->mayUndo()) { 
				(Editor::his->retCommand())->_undo();
				Editor::his->prev();	
			}
			else Editor::beep();
			break;
		case K_PAGE_UP:
			if (Editor::his->mayUndo()) {
				cid = (theCmd = Editor::his->retCommand())->id();
				do {
					theCmd->_undo();
					Editor::his->prev();
				} while (Editor::his->mayUndo() &&
						(theCmd=Editor::his->retCommand())->id() == cid);
			} else
				Editor::beep();
			break;
		case K_CURS_DOWN:
			if (Editor::his->mayRedo()) {
				Editor::his->next();
				(Editor::his->retCommand())->_redo();
			} 
			else Editor::beep();
			break;
		case K_PAGE_DOWN:
			if (Editor::his->mayRedo()) {
				Editor::his->next();
				cid = (theCmd=Editor::his->retCommand())->id();
				do {
					theCmd->_redo();
				} while (Editor::his->mayRedo() && (Editor::his->next(),
						(theCmd=Editor::his->retCommand())->id()) == cid);

				if (theCmd->id() != cid)
					Editor::his->prev();
			} else
				Editor::beep();
			break;
		default:
			Editor::beep();
			break;
		}                                                                          
		stUndo = Editor::editorMsg("UNDO")+"["+
				 toString(Editor::his->currLevel())+"/"+
				 toString(Editor::his->levels())+"]";
/*
		if ((theCmd=Editor::his->retCommand()) != NULL)                                
			stUndo += " ("+Editor::sen->getCmdName(theCmd->id())+")";
*/			
    	cView.value()->writeStLine(stUndo);
	}   
	cView.value()->writeStLine();
	return false;
}

// end clase History

// clase ShiftLeft

bool ShiftLeft::redo()
{
	idCheck();
	DaliView *view = cView.value();
	DaliTextCursor block;

	rowCur = cursor().order();
	colCur = cursor().column();

	if (view->hasBlock()) {

		block = view->getBlock();
		view->delBlock(false);

		rowBlk = block.order();
		colBlk = block.column();

		if (block < cursor()) {
			block.goToCol(0);
			if (cursor().column() != 0) {
				cursor()++;
				cursor().goToCol(0);
			}
		} else {
			cursor().goToCol(0);
			if (block.column() != 0) {
				block++;
				block.goToCol(0);
			}
		}

		view->setBlock(block);

	} else
		rowBlk = colBlk = ERR;

	List(String) lst;
	view->getRegion(lst, block);

	bool sym;
	didSomething = view->shiftLeft(sym);

	needRestore = false;
	// Do we need to save the list?
	if (view->hasBlock() && !sym && didSomething) {
		needRestore = true;

		theLog.openOutput();
		theLog.put(lst);
		theLog.closeOutput();
	}

	return true;
}

void ShiftLeft::undo()
{
	idCheck();
	DaliView *view = cView.value();
	DaliTextCursor block;

	if (rowBlk != ERR)
		block = view->getBlock();

	if (didSomething) {
		if (needRestore) {
			view->delBlock(false);

			List(String) lst;

			theLog.openInput();
			theLog.get(lst);
			theLog.closeInput();

			view->delRegion(block);
			if (block < cursor())
				cursor() = block;
			view->insertText(lst);
			view->setBlock(block);
		} else
			(void) view->shiftRight();
	}

	if (rowBlk != ERR) {
		view->delBlock(false);
		DaliTextCursor saveBlk = cursor();
		saveBlk.goTo(rowBlk, colBlk);
		cursor().goTo(rowCur, colCur);
		view->setBlock(saveBlk);
	} else
		cursor().goTo(rowCur, colCur);
}

// end clase ShiftLeft

// clase ShiftRight

bool ShiftRight::redo()
{
	idCheck();
	DaliView *view = cView.value();
	DaliTextCursor block;

	rowCur = cursor().order();
	colCur = cursor().column();

	if (view->hasBlock()) {

		block = view->getBlock();
		view->delBlock(false);

		rowBlk = block.order();
		colBlk = block.column();

		if (block < cursor()) {
			block.goToCol(0);
			if (cursor().column() != 0) {
				cursor()++;
				cursor().goToCol(0);
			}
		} else {
			cursor().goToCol(0);
			if (block.column() != 0) {
				block++;
				block.goToCol(0);
			}
		}
		view->setBlock(block);

	} else
		rowBlk = colBlk = ERR;

	return view->shiftRight();
}

void ShiftRight::undo()
{
	idCheck();
	DaliView *view = cView.value();

	bool dummy;
	(void) view->shiftLeft(dummy);

	if (rowBlk != ERR) {
		view->delBlock(false);
		DaliTextCursor saveBlk = cursor();
		cursor().goTo(rowCur, colCur);
		saveBlk.goTo(rowBlk, colBlk);
		view->setBlock(saveBlk);
	} else
		cursor().goTo(rowCur, colCur);
}

// end clase ShiftRight

// clase Tab

bool Tab::redo()
{
	idCheck();

    if(cView.value()->inInsMode()) 
		cView.value()->tab();
    else 
	    cView.value()->cursRight(DaliText::tabSize());

	return true;
}

void Tab::undo()
{
	idCheck();
    if(cView.value()->inInsMode()) 
		cView.value()->backTab();
    else 
	    cView.value()->cursLeft(DaliText::tabSize());

}

// fin clase Tab

// clase ChgInsMode

bool DaliChgInsMode::redo()
{
	idCheck();
	cView.value()->chgInsMode();
	return true;
}

void DaliChgInsMode::undo() 
{
	idCheck();
	cView.value()->chgInsMode();
}

// fin clase DaliChgInsMode

// clase DaliChgCtrolMode

bool DaliChgCtrolMode::redo()
{
	idCheck();
	cView.value()->chgCtrolMode();
	return true;
}

void DaliChgCtrolMode::undo() 
{ 
	idCheck();
	cView.value()->chgCtrolMode();
}

// fin clase DaliChgCtrolMode

// clase SetTroffMode

bool SetTroffMode::redo()
{
	idCheck();

	if (cView.value()->isTeX())
		cView.value()->setTeX(); // This unsets TeX mode

	cView.value()->setTroff();
	return true;
}

void SetTroffMode::undo() 
{ 
	idCheck();
	cView.value()->setTroff();
}

// clase SetTeXMode

bool SetTeXMode::redo()
{
	idCheck();

	if (cView.value()->isTroff())
		cView.value()->setTroff();  // This unsets Troff mode
		
	cView.value()->setTeX();
	return true;
}

void SetTeXMode::undo() 
{ 
	idCheck();
	cView.value()->setTeX();
}

// fin clase SetTeXMode

// clase BeginBlock

bool BeginBlock::redo()
{
	idCheck();
	return cView.value()->beginBlock();
}

void BeginBlock::undo() 
{ 
	idCheck();
	cView.value()->delBlock();		
}

// fin clase BeginBlock

// clase PasteBlock

bool PasteBlock::redo()
{
	idCheck();
	List(String) *cb = Editor::clipboard;

	Int sz = cb->size();

	if (sz == 0)	
		return false;

	ListCursor(String) lc(*cb);
	lc.goToLast();

	cView.value()->insertText(*cb);

	// hasta la fila que debo borrar
	hfil = cursor().order()+sz-1;

	hcol = (sz == 1 ? cursor().column() : 0) + lc.value().length() - 1;

	return true;
}

void PasteBlock::undo() 
{ 
	idCheck();
	cursor().delTo(hfil, hcol, false);
	cView.value()->redraw();
}

// end clase PasteBlock

// clase BackTab

bool BackTab::redo()
{
	idCheck();
	return cView.value()->backTab();
}

void BackTab::undo()
{
	idCheck();
	cView.value()->tab();
}

// fin clase BackTab

// clase InsNewLine

bool InsNewLine::redo()
{
	idCheck();
	cfil = cursor().order();
	ccol = cursor().column();

	DaliView *view = cView.value();
	view->goFirstNonSp();
	indent = view->insNewLine();
	view->cursUp();

	Editor::setMode(NULL_MODE);

	return true;
}

void InsNewLine::undo()
{
	idCheck();
	cursor().goToCol(0);
	cursor().delRange(0, indent-1);

	DaliView *view = cView.value();
	view->backSpace();
	view->goCursor(cfil, ccol);
}

// end clase InsNewLine

// clase AddNewLine

bool AddNewLine::redo()
{
	idCheck();
	DaliView *view = cView.value();

	cfil = cursor().order();
	ccol = cursor().column();

	Int offset = cursor().value().length() - cursor().column();
	view->cursRight(offset);

	indent = view->insNewLine();

	Editor::setMode(NULL_MODE);

	return true;
}

void AddNewLine::undo()
{
	idCheck();
	DaliView *view = cView.value();
	cursor().goToCol(0);
	cursor().delRange(0, indent-1);
	view->backSpace();
	view->goCursor(cfil, ccol);
}

// end clase AddNewLine

// clase JoinLines

bool JoinLines::redo()
{
	idCheck();
	cfil = cursor().order();
	ccol = cursor().column();
	return cView.value()->joinLines();
}

void JoinLines::undo()
{
	idCheck();
	cView.value()->insNewLine(false);
	cView.value()->goCursor(cfil, ccol);
}

// end clase JoinLines

bool CommMode::redo()
{                           
	idCheck();
	Editor::setCmdLine( !Editor::isCmdLine() );
	return true;
}

void CommMode::undo()
{
	idCheck();
	Editor::setCmdLine( !Editor::isCmdLine() );
}

