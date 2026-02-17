/********************************************************************
* Copyright (c) 1991 InterSoft Co.  All Rights Reserved
* THIS IS UNPUBLISHED PROPRIETARY SOURCE CODE OF InterSoft Co.
* The copyright notice above does not evidence any
* actual or intended publication of such source code.
*
* $Id: blkcomm.cc,v 1.4 2000/07/20 18:10:33 ekonig Exp $
*
* DESCRIPTION: Implementation of block commands class
*********************************************************************/
#ifdef __GNUC__
#pragma implementation
#endif

// para que genere la entrada dentro de la tabla de comandos
#define INITCOMM

#include <command.h>
#include <blkcomm.h>
#include <editor.h>
#include <dview.h>
#include <dtext.h>
#include <winapp/menu.h>
#include <edlbox.h>
#include <local/strfile.h>
#include <winapp/strhis.h>
#include <winapp/msgbox.h>

declare(PtrList,DaliView);
declare(PtrListCursor,DaliView);

// clase CopyBlock

bool CopyBlock::redo(DaliTextCursor &tc)
{
	idCheck();
	List(String) oldcb;

	// salvo la lista anterior
	for (ListCursor(String) lc(*Editor::clipboard); lc; lc++)
		oldcb.add(lc.value());

	// Guardo el clipboard viejo
	theLog.openOutput();
	theLog.put(oldcb);
	theLog.closeOutput();

	// borro el clipboard
	Editor::clipboard->delAll();

	// obtengo el bloque
	cView.value()->getRegion(*(Editor::clipboard), tc);

	return true;
}

void CopyBlock::undo()
{
	idCheck();
	Editor::clipboard->delAll();

	List(String) oldcb;

	theLog.openInput();
	theLog.get(oldcb);
	theLog.closeInput();

	// restauro la lista anterior
	for (ListCursor(String) lc(oldcb);lc ; lc++)
		Editor::clipboard->add(lc.value());
}

// end clase CopyBlock

// clase CutBlock

bool CutBlock::redo(DaliTextCursor &tc)
{
	idCheck();
	List(String) oldcb;

	// salvo la lista anterior
	for (ListCursor(String) lc(*Editor::clipboard);lc ; lc++)
		oldcb.add(lc.value());

	// Guardo el clipboard viejo
	theLog.openOutput();
	theLog.put(oldcb);
	theLog.closeOutput();

	// salvo el cursor
	cfil = cursor().order();
	ccol = cursor().column();

	// borro el clipboard
	Editor::clipboard->delAll();

	// obtengo la view corriente
	DaliView *view = cView.value();

	// obtengo el bloque
	view->getRegion(*(Editor::clipboard), tc);

	Int tcfil = tc.order();
	Int tccol = tc.column();

	// borra zona del bloque	
	view->delRegion(tc);

	// si estoy al final del block lo coloco al principio
	if (cfil > tcfil || (cfil == tcfil && ccol > tccol))
		view->goCursor(tcfil, tccol);

	return true;
}

void CutBlock::undo()
{
	idCheck();
	// tomo la view corriente
	DaliView *view = cView.value();

	// inserto lo que habia en el clipboard
	view->insertText(*(Editor::clipboard));

	// restauro el cursor
	view->goCursor(cfil, ccol);

	// borro el clipboard
	Editor::clipboard->delAll();

	// restauro la lista anterior
	List(String) oldcb;

	theLog.openInput();
	theLog.get(oldcb);
	theLog.closeInput();

	for (ListCursor(String) lc(oldcb); lc; lc++)
		Editor::clipboard->add(lc.value());
}

// end clase CutBlock

// clase ClearBlock

bool ClearBlock::redo(DaliTextCursor &tc)
{
	idCheck();
	DaliView *view = cView.value();
	bool rw = false;

	List(String) oldbk;

	view->getRegion(oldbk, tc);

	theLog.openOutput();
	theLog.put(oldbk);
	theLog.closeOutput();

	tcfil = tc.order();
	tccol = tc.column();

	cfil  = cursor().order();
	ccol  = cursor().column();

	view->delRegion(tc, false);

	if (cursor() > tc)
		rw = view->goCursor(tc.order(), tc.column());
		
	if (!rw)
		view->redraw();

	return true;
}

void ClearBlock::undo()
{
	idCheck();
	DaliView *view = cView.value();

	cursor().goTo(cfil, ccol);

	DaliTextCursor tc = cursor();
	tc.goTo(tcfil, tccol);

	view->delRegion(tc);

	if (cursor().order() > tcfil ||
			(cursor().order() == tcfil && cursor().column() > tccol))
		cursor().goTo(tcfil, tccol);
		
	List(String) oldbk;

	theLog.openInput();
	theLog.get(oldbk);
	theLog.closeInput();

	view->insertText(oldbk);

	if (!view->goCursor(cfil, col))
		view->redraw();
}

// end clase ClearBlock

// clase DeleteBlock

bool DeleteBlock::redo(DaliTextCursor &tc)
{
	idCheck();
	DaliView *view = cView.value();
	bool rw = false;

	List(String) oldbk;

	view->getRegion(oldbk, tc);

	theLog.openOutput();
	theLog.put(oldbk);
	theLog.closeOutput();

	cfil  = cursor().order();
	ccol  = cursor().column();

	view->delRegion(tc);

	if (cursor() > tc) 
		rw = view->goCursor(tc.order(), tc.column());
		
	if (!rw)
		view->redraw();

	return true;
}

void DeleteBlock::undo()
{
	idCheck();
	DaliView *view = cView.value();

	List(String) oldbk;

	theLog.openInput();
	theLog.get(oldbk);
	theLog.closeInput();

	view->insertText(oldbk);

	if (!view->goCursor(cfil, ccol))
		view->redraw();
}

// end clase DeleteBlock

// clase SetAttribute

bool SetAttribute::redo(DaliTextCursor &tc)
{
	idCheck();
	// Can't set attributes in debug_mode
	if (Editor::getMode() & DEBUG_MODE)
   	 	return true;

	DaliView *view = cView.value();

	if (atrib == ERR) {

		if ((atrib = Editor::attribMenu()) == ERR)
			return false;

		tcfil = tc.order();
		tccol = tc.column();
	}

	view->setAttribute(tc, atrib);
	view->redraw();

	return true;
}

void SetAttribute::undo()
{
	idCheck();
	DaliTextCursor tc = cursor();
	tc.goTo(tcfil, tccol);
	cView.value()->delAttribute(tc, atrib);
}

// end clase SetAttribute

// clase DelAttribute

bool DelAttribute::redo(DaliTextCursor &tc)
{
	idCheck();
	DaliView *view = cView.value();

	if (atrib == ERR) {

		if ((atrib = Editor::attribMenu()) == ERR)
			return false;

		tcfil = tc.order();
		tccol = tc.column();
	}

	view->delAttribute(tc, atrib);
	view->redraw();

	return true;
}

void DelAttribute::undo()
{
	idCheck();
	DaliTextCursor tc = cursor();
	tc.goTo(tcfil , tccol);
	cView.value()->setAttribute(tc, atrib);
}

// end clase DelAttribute

// clase ExecOSFilter

#define EXECOSFIL_ROWS		3
#define EXECOSFIL_COLS		60
#define EXECOSFIL_ORG_ROW	5
#define EXECOSFIL_ORG_COL	10

bool ExecOSFilter::redo(DaliTextCursor &tc)
{
	idCheck();

	// no tengo comando -> lo leo
	if (cmd == NULL_STRING) {
		EdLineDialog dlog(Editor::filters, 
			Editor::editorMsg("ExecOSCommand"), 
			Editor::editorMsg("COMMAND"), EXECOSFIL_ROWS, 
			EXECOSFIL_COLS, EXECOSFIL_ORG_ROW, EXECOSFIL_ORG_COL);
	  
		// tomo la linea de comandos
		dlog.execute();
		if(dlog.isAccepted() != true) 
			return false;
		
		cmd = dlog.value();
		*Editor::filters << cmd;
	}

	DaliView *view = cView.value();

	// salvo el cursor
	cfil = cursor().order();
	ccol = cursor().column();

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
	ListCursor(String) lc(oldbk);
	for (; lc; lc++) 
		iop << DaliText::decodeTabs(lc.value()) << NEW_LINE;		
    
	iop.close();

	String aux;
	oldbk.delAll();

	// tomo el resultado del pipe
	while (iop >> aux)
		oldbk.add(DaliText::encodeTabs(aux)); 

	Int tcfil = tc.order();
	Int tccol = tc.column();

	// borro la region de la view
	view->delRegion(tc);

	// el resultdo es vacio
	if (oldbk.size() == 0) {

		hfil = hcol = INT_MIN;

		// reacomodo el cursor
		if (cursor().order() > tcfil || cursor().order() == tcfil &&
										cursor().column() > tccol)
			view->goCursor(tcfil, tccol);

		return true;
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

	return true;
}

void ExecOSFilter::undo()
{
	idCheck();
	// borro la que habia insertado
	if (hfil != INT_MIN && hcol != INT_MIN)
		cursor().delTo(hfil, hcol, false);

	DaliView *view = cView.value();

	List(String) oldbk;

	theLog.openInput();
	theLog.get(oldbk);
	theLog.closeInput();

	view->insertText(oldbk);

	// restauro el cursor
	view->goCursor(cfil, ccol);
}

// end clase ExecOSFilter

// class EndBlockMode

bool EndBlockMode::redo(DaliTextCursor &)
{
	idCheck();
	cView.value()->writeStLine();
	return true;
}

void EndBlockMode::undo() { }

// end clase EndBlockMod


