/********************************************************************
* Copyright (c) 1993 InterSoft Co.  All Rights Reserved
* THIS IS UNPUBLISHED PROPRIETARY SOURCE CODE OF InterSoft Co.
* The copyright notice above does not evidence any
* actual or intended publication of such source code.
*
* $Id: blkcmd.cc,v 1.2 2000/01/31 18:25:32 diegon Exp $
*
* DESCRIPTION: Implementation of block commands
*********************************************************************/
#ifdef __GNUC__
#pragma implementation
#endif

#define INITCOMM
#include <winapp/winapp.h>
#include <winapp/wincmd.h>
#include <winapp/blkcmd.h>
#include <winapp/view.h>
#include <winapp/eview.h>
#include <winapp/text.h>
#include <winapp/menu.h>
#include <local/strfile.h>
#include <winapp/strhis.h>
#include <winapp/msgbox.h>
#include <winapp/cmdbind.h>
#include <stdio.h>

void bindBlockCmds(CmdBinding *cb)
{
//	cb->bind(BlockBeginMode::eventId, 	BlockBeginMode::cmdId);
	cb->bind(BlockBegin::eventId, 		BlockBegin::cmdId);
	cb->bind(BlockEndMode::eventId, 	BlockEndMode::cmdId);
	cb->bind(BlockClear::eventId, 		BlockClear::cmdId);
	cb->bind(BlockDelete::eventId, 		BlockDelete::cmdId);
	cb->bind(BlockCut::eventId, 		BlockCut::cmdId);
	cb->bind(BlockPaste::eventId, 		BlockPaste::cmdId);
	cb->bind(BlockCopy::eventId,		BlockCopy::cmdId);
}

#if 0
bool BlockBeginMode::redo()
{
TRACE
}

bool BlockEndMode::redo()
{
TRACE
}
#endif

bool BlockBegin::redo()
{
	idCheck();

	EditableView *view = appi->getDesktop()->getCurrEdView();
	assert (view != NULL);
	return view->beginBlock();
}

void BlockBegin::undo() 
{ 
	idCheck();

	EditableView *view = appi->getDesktop()->getCurrEdView();
	assert (view != NULL);

	view->delBlock();		
}

bool BlockCopy::redo(TextCursor &tc)
{
	idCheck();

	EditableView *view = appi->getDesktop()->getCurrEdView();

	List(String) oldcb;

	// Save previous list
// 	for (ListCursor(String) lc = view->getClipboard(); lc; lc++)

	for (ListCursor(String) lc(*WinApp::clipboard); lc; lc++)
		oldcb.add(lc.value());

	// Save previous WinApp::clipboard
	theLog.openOutput();
	theLog.put(oldcb);
	theLog.closeOutput();

	// Delete current WinApp::clipboard
	WinApp::clipboard->delAll();

	// Get clipboard
	view->getRegion(*(WinApp::clipboard), tc);

	return true;
}

void BlockCopy::undo()
{
	idCheck();

	EditableView *view = appi->getDesktop()->getCurrEdView();
	assert (view != NULL);

	WinApp::clipboard->delAll();

	List(String) oldcb;

	theLog.openInput();
	theLog.get(oldcb);
	theLog.closeInput();

	// Restore previous list
	for (ListCursor(String) lc(oldcb);lc ; lc++)
		WinApp::clipboard->add(lc.value());
}

bool BlockCut::redo(TextCursor &tc)
{
	idCheck();

	List(String) oldcb;

	EditableView *view = appi->getDesktop()->getCurrEdView();
	assert (view != NULL);

	// Save previous list
	for (ListCursor(String) lc(*WinApp::clipboard);lc ; lc++)
		oldcb.add(lc.value());

	// Save previous WinApp::clipboard
	theLog.openOutput();
	theLog.put(oldcb);
	theLog.closeOutput();

	// Svae previous cursor
	cfil = view->getRow();
	ccol = view->getCol();

	// Delete current WinApp::clipboard
	WinApp::clipboard->delAll();

	// Get block
	view->getRegion(*(WinApp::clipboard), tc);

	Int tcfil = tc.order();
	Int tccol = tc.column();

	// Delete block region
	view->delRegion(tc);
       
	// Set the cursor at the beginning of the block if it is at the end.
	if (cfil > tcfil || (cfil == tcfil && ccol > tccol))
		view->goCursor(tcfil, tccol);

	return true;
}

void BlockCut::undo()
{
	idCheck();

	EditableView *view = appi->getDesktop()->getCurrEdView();
	assert (view != NULL);

	// Insert whatever was in the WinApp::clipboard
	view->insertText(*(WinApp::clipboard));

	// Restore cursor
	view->goCursor(cfil, ccol);

	// Delete WinApp::clipboard
	WinApp::clipboard->delAll();

	// Restore previous list
	List(String) oldcb;

	theLog.openInput();
	theLog.get(oldcb);
	theLog.closeInput();

	for (ListCursor(String) lc(oldcb); lc; lc++)
		WinApp::clipboard->add(lc.value());
}

bool BlockClear::redo(TextCursor &tc)
{
	idCheck();

	bool rw = false;

	List(String) oldbk;

	EditableView *view = appi->getDesktop()->getCurrEdView();
	assert (view != NULL);

	view->getRegion(oldbk, tc);

	theLog.openOutput();
	theLog.put(oldbk);
	theLog.closeOutput();

	tcfil = tc.order();
	tccol = tc.column();

	cfil  = view->edCursor()->order();
	ccol  = view->edCursor()->column();

	view->delRegion(tc, false);

	if (*view->edCursor() > tc)
		rw = view->goCursor(tc.order(), tc.column());
		
	if (!rw)
		view->View::redraw();

	return true;
}

void BlockClear::undo()
{
	idCheck();

	EditableView *view = appi->getDesktop()->getCurrEdView();
	assert (view != NULL);

	view->edCursor()->goTo(cfil, ccol);

	TextCursor tc = *view->edCursor();
	tc.goTo(tcfil, tccol);

	view->delRegion(tc);

	if (view->edCursor()->order() > tcfil ||
			(view->edCursor()->order() == tcfil && view->edCursor()->column() > tccol))
		view->edCursor()->goTo(tcfil, tccol);
		
	List(String) oldbk;

	theLog.openInput();
	theLog.get(oldbk);
	theLog.closeInput();

	view->insertText(oldbk);

	if (!view->goCursor(cfil, ccol))
		view->View::redraw();
}

bool BlockDelete::redo(TextCursor &tc)
{
	idCheck();

	bool rw = false;

	List(String) oldbk;

	EditableView *view = appi->getDesktop()->getCurrEdView();
	assert (view != NULL);

	view->getRegion(oldbk, tc);

	theLog.openOutput();
	theLog.put(oldbk);
	theLog.closeOutput();

	cfil  = view->edCursor()->order();
	ccol  = view->edCursor()->column();

	view->delRegion(tc);

	if (*view->edCursor() > tc) 
		rw = view->goCursor(tc.order(), tc.column());
		
	if (!rw)
		view->View::redraw();

	return true;
}

void BlockDelete::undo()
{
	idCheck();

	List(String) oldbk;

	theLog.openInput();
	theLog.get(oldbk);
	theLog.closeInput();

	EditableView *view = appi->getDesktop()->getCurrEdView();
	assert (view != NULL);

	view->insertText(oldbk);

	if (!view->goCursor(cfil, ccol))
		view->View::redraw();
}

bool BlockPaste::redo()
{
	idCheck();

	EditableView *view = appi->getDesktop()->getCurrEdView();
	assert (view != NULL);

	List(String) *cb = WinApp::clipboard;

	Int sz = cb->size();

	if (sz == 0)	
		return false;

	ListCursor(String) lc(*cb);
	lc.goToLast();

	view->insertText(*cb);

	// hasta la fila que debo borrar
	hfil = view->edCursor()->order() + sz - 1;
	hcol = (sz == 1 ? view->edCursor()->column() : 0) + lc.value().length() - 1;

	return true;
}

void BlockPaste::undo() 
{ 
	idCheck();

	EditableView *view = appi->getDesktop()->getCurrEdView();
	assert (view != NULL);

	view->edCursor()->delTo(hfil, hcol, false);
	view->View::redraw();
}

bool BlockEndMode::redo(TextCursor &)
{
	idCheck();

	EditableView *view = appi->getDesktop()->getCurrEdView();
	assert (view != NULL);

	view->writeStLine();
	return true;
}

void BlockEndMode::undo()
{ 
    idCheck();
}




