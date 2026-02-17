/********************************************************************
* Copyright (c) 1993 InterSoft Co.  All Rights Reserved
* THIS IS UNPUBLISHED PROPRIETARY SOURCE CODE OF InterSoft Co.
* The copyright notice above does not evidence any
* actual or intended publication of such source code.
*
* $Id: adblkcmd.cc,v 1.1.1.1 1996/04/16 18:52:47 gustavof Exp $
*
* DESCRIPTION: Implementation of Advanced block commands.
*********************************************************************/

#define INITCOMM

#include <winapp/wincmd.h>
#include <winapp/adblkcmd.h>
#include <winapp/view.h>
#include <winapp/eview.h>
#include <winapp/text.h>
#include <winapp/menu.h>
#include <local/strfile.h>
#include <winapp/strhis.h>
#include <winapp/msgbox.h>
#include <winapp/cmdbind.h>


void bindAdvancedBlockCmds(CmdBinding *cb)
{
	cb->bind(SetAttribute::eventId,			SetAttribute::cmdId);
	cb->bind(DelAttribute::eventId,			DelAttribute::cmdId);
	cb->bind(ExecOSFilter::eventId, 		ExecOSFilter::cmdId);
	cb->bind(ShiftLeft::eventId,			ShiftLeft::cmdId);
	cb->bind(ShiftRight::eventId,			ShiftRight::cmdId);
}


ixBool ShiftLeft::redo()
{
	idCheck();
	View *view = appi->getDesktop()->getCurrView();
	assert (view != NULL);
	assert(view->edView()); // Must be at least an editable view
	
	TextCursor block;

	rowCur = view->getRow();
	colCur = view->getCol();

	if (view->edView()->hasBlock()) {

		block = view->edView()->getBlock();
		view->edView()->delBlock(FALSE);

		rowBlk = block.order();
		colBlk = block.column();

		if (block < *view->getCursor()) {
			block.goToCol(0);
			if (view->getCol() != 0) {
				(*view->getCursor())++;
				(*view->getCursor()).goToCol(0);
			}
		} else {
			(*view->getCursor()).goToCol(0);
			if (block.column() != 0) {
				block++;
				block.goToCol(0);
			}
		}

		view->edView()->setBlock(block);

	} else
		rowBlk = colBlk = ERROR;

	List(String) lst;
	view->edView()->getRegion(lst, block);

	ixBool sym;
	didSomething = view->edView()->shiftLeft(sym);

	needRestore = FALSE;
	// Do we need to save the list?
	if (view->edView()->hasBlock() && !sym && didSomething) {
		needRestore = TRUE;

		theLog.openOutput();
		theLog.put(lst);
		theLog.closeOutput();
	}

	return TRUE;
}

void ShiftLeft::undo()
{
	idCheck();
	TextCursor block;

	View *view = appi->getDesktop()->getCurrView();
	assert (view != NULL);

	if (rowBlk != ERROR)
		block = view->edView()->getBlock();

	if (didSomething) {
		if (needRestore) {
			view->edView()->delBlock(FALSE);

			List(String) lst;

			theLog.openInput();
			theLog.get(lst);
			theLog.closeInput();

			view->edView()->delRegion(block);
			if (block < (*view->getCursor()))
				(*view->getCursor()) = block;
				
			view->edView()->insertText(lst);
			view->edView()->setBlock(block);
		} else
			(void) view->edView()->shiftRight();
	}

	if (rowBlk != ERROR) {
		view->edView()->delBlock(FALSE);
		TextCursor saveBlk = (*view->getCursor());
		saveBlk.goTo(rowBlk, colBlk);
		(*view->getCursor()).goTo(rowCur, colCur);
		view->edView()->setBlock(saveBlk);
	} else
		(*view->getCursor()).goTo(rowCur, colCur);
}

ixBool ShiftRight::redo()
{
	idCheck();

	TextCursor block;

	View *view = appi->getDesktop()->getCurrView();
	assert (view != NULL);
	rowCur = view->getRow();
	colCur = view->getCol();

	if (view->edView()->hasBlock()) {

		block = view->edView()->getBlock();
		view->edView()->delBlock(FALSE);

		rowBlk = block.order();
		colBlk = block.column();

		if (block < (*view->getCursor())) {
			block.goToCol(0);
			if ((*view->getCursor()).column() != 0) {
				(*view->getCursor())++;
				(*view->getCursor()).goToCol(0);
			}
		} else {
			(*view->getCursor()).goToCol(0);
			if (block.column() != 0) {
				block++;
				block.goToCol(0);
			}
		}
		view->edView()->setBlock(block);

	} else
		rowBlk = colBlk = ERROR;

	return view->shiftRight();
}

void ShiftRight::undo()
{
	idCheck();
	ixBool dummy;

	View *view = appi->getDesktop()->getCurrView();
	assert (view != NULL);

	(void) view->edView()->shiftLeft(dummy);

	if (rowBlk != ERROR) {
		view->edView()->delBlock(FALSE);
		TextCursor saveBlk = (*view->getCursor());
		(*view->getCursor()).goTo(rowCur, colCur);
		saveBlk.goTo(rowBlk, colBlk);
		view->edView()->setBlock(saveBlk);
	} else
		(*view->getCursor()).goTo(rowCur, colCur);
}

ixBool ExecOSFilter::redo(TextCursor &tc)
{
	idCheck();         
	assert(0);
	// no tengo comando -> lo leo
	if (cmd == NULL_STRING) {
		AppLineDialog dlog(App::filters, 
			App::appMsg("ExecOSCommand"), 
			App::appMsg("COMMAND"), EXECOSFIL_ROWS, 
			EXECOSFIL_COLS, EXECOSFIL_ORG_ROW, EXECOSFIL_ORG_COL);
	  
		// tomo la linea de comandos
		dlog.execute();
		if(dlog.isAccepted() != TRUE) 
			return FALSE;
		
		cmd = dlog.value();
		*App::filters << cmd;
	}

	View *view = appi->getDesktop()->getCurrView(); //poner antes ??
	assert (view != NULL);	// salvo el cursor
	cfil = view->getRow();
	ccol = view->getCol();

	// obtengo el bloque
	List(String) oldbk;
	view->getRegion(oldbk, tc);

	// lo guardo en el log
	theLog.openOutput();
	theLog.put(oldbk);
	theLog.closeOutput();

	// ahora abro el iopipe
	InOutPipe iop(cmd);

	// mando al pipe el bloque
	for (ListCursor(String) lc = oldbk; lc; lc++) 
		iop << Text::decodeTabs(lc.value()) << "\n";		
    
	iop.close();

	String aux;
	oldbk.delAll();

	// tomo el resultado del pipe
	while (iop >> aux)
		oldbk.add(Text::encodeTabs(aux)); 

	Int tcfil = tc.order();
	Int tccol = tc.column();

	// borro la region de la view
	view->delRegion(tc);

	// el resultdo es vacio
	if (oldbk.size() == 0) {

		hfil = hcol = INT_MIN;

		// reacomodo el cursor
		if (view->getRow() > tcfil 
			|| view->getRow() == tcfil 
			&& cursor().column() > tccol)
			view->goCursor(tcfil, tccol);

		return TRUE;
	}

	// inserto el newline
	oldbk.add(NULL_STRING);

	// reacomodo el cursor
	if (cursor().order() > tcfil || cursor().order() == tcfil &&
									cursor().column() > tccol)
		view->goCursor(tcfil, tccol);
		
	// datos para el undo
	Int sz = oldbk.size();
	lc.goToLast();

	// hasta la fila que debo borrar
	hfil = cursor().order()+sz-1;
	hcol = (sz == 1 ? cursor().column() : 0) + lc.value().length() - 1;

	// inserto el resultado en la view
	view->insertText(oldbk);
	return TRUE;
}

ixBool SetAttribute::redo(TextCursor &tc)
{
	idCheck();

	if (atrib == ERROR) {
		AttributeMenu am("Menu de atributos"); // toDo: La clase AttributeMenu
        Int n;
		
		if((am.execute() != REJECT_EXIT ) 
			&& (n = am.currOpt()) == ERROR)
	 		return FALSE;

		atrib = attrs[n].attr;
        
		tcfil = tc.order();
		tccol = tc.column();
	}

	View *view = appi->getDesktop()->getCurrView();
	assert (view != NULL);
	view->setAttribute(tc, atrib);
	view->redraw();

	return TRUE;
}

void SetAttribute::undo()
{
	idCheck();

	View *view = appi->getDesktop()->getCurrView();
	assert (view != NULL);
	TextCursor tc = cursor();
	tc.goTo(tcfil, tccol);
	view->delAttribute(tc, atrib);
}

ixBool DelAttribute::redo(TextCursor &tc)
{
	idCheck();

	View *view = appi->getDesktop()->getCurrView();
	assert (view != NULL);
//	View *view = cView.value();

	if (atrib == ERROR) {
		AttributeMenu am("Menu de atributos"); // toDo: La clase AttributeMenu
        Int n;
		
		if((am.execute() != REJECT_EXIT ) 
			&& (n = am.currOpt()) == ERROR)
	 		return FALSE;

		atrib = attrs[n].attr;

		tcfil = tc.order();
		tccol = tc.column();
	}

	view->delAttribute(tc, atrib);
	view->redraw();

	return TRUE;
}

void DelAttribute::undo()
{
	idCheck();

	View *view = appi->getDesktop()->getCurrView();
	assert (view != NULL);

	TextCursor tc = cursor();
	tc.goTo(tcfil , tccol);
	view->setAttribute(tc, atrib);
}


