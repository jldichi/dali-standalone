/********************************************************************
* Copyright (c) 1993 InterSoft Co.  All Rights Reserved
* THIS IS UNPUBLISHED PROPRIETARY SOURCE CODE OF InterSoft Co.
* The copyright notice above does not evidence any
* actual or intended publication of such source code.
*
* $Id: poscmd.cc,v 1.1.1.1 1996/04/16 18:52:47 gustavof Exp $
*
* DESCRIPTION: Implementation of class CmdBinding. 
*********************************************************************/

#ifdef __GNUC__
#pragma implementation
#endif

#define INITCOMM

#include <winapp/winapp.h>
#include <winapp/wincmd.h>
#include <winapp/poscmd.h>
#include <winapp/view.h>
#include <winapp/text.h>
#include <winapp/window.h>
#include <ifound/rexp.h>
#include <winapp/strhis.h>
#include <winapp/cmdbind.h>

void bindPosCmds(CmdBinding *cb)
{
	// Cursor movement
	cb->bind(PosCursDown::eventId,		PosCursDown::cmdId);
	cb->bind(PosCursUp::eventId,		PosCursUp::cmdId);
	cb->bind(PosCursLeft::eventId,		PosCursLeft::cmdId);
	cb->bind(PosCursRight::eventId,		PosCursRight::cmdId);
	cb->bind(PosPageDown::eventId,		PosPageDown::cmdId);
	cb->bind(PosPageUp::eventId,		PosPageUp::cmdId);
	cb->bind(PosBegOfLine::eventId,		PosBegOfLine::cmdId);
	cb->bind(PosEndOfLine::eventId,		PosEndOfLine::cmdId);
	cb->bind(PosBegOfFile::eventId,		PosBegOfFile::cmdId);
	cb->bind(PosEndOfFile::eventId,		PosEndOfFile::cmdId);
	cb->bind(PosBegOfScr::eventId,		PosBegOfScr::cmdId);
	cb->bind(PosMidOfScr::eventId,		PosMidOfScr::cmdId);
	cb->bind(PosEndOfScr::eventId,		PosEndOfScr::cmdId);
	cb->bind(PosWordRight::eventId,		PosWordRight::cmdId);
	cb->bind(PosWordLeft::eventId,		PosWordLeft::cmdId);
}   

bool PosCursDown::redo()
{
	idCheck();
	
    View *view= appi->getDesktop()->getCurrView();
	assert (view != NULL);

	view->cursDown();
	return true;
}

bool PosCursUp::redo()
{
	idCheck();

	View *view = appi->getDesktop()->getCurrView();
	assert (view != NULL);
	
	view->cursUp();
	return true;
}

bool PosCursLeft::redo()
{
	idCheck();

	View *view = appi->getDesktop()->getCurrView();
	assert (view != NULL);
	
	view->cursLeft();
	return true;
}

bool PosCursRight::redo()
{
	idCheck();

	View *view = appi->getDesktop()->getCurrView();
	assert (view != NULL);
	
	view->cursRight();
	return true;
}

bool PosPageDown::redo()
{
	idCheck();

	View *view = appi->getDesktop()->getCurrView();
	assert (view != NULL);
	
	view->pageDown();
	return true;
}

bool PosPageUp::redo()
{
	idCheck();

	View *view = appi->getDesktop()->getCurrView();
	assert (view != NULL);
	
	view->pageUp();
	return true;
}

bool PosWordRight::redo()
{
	idCheck();
	
	View *view = appi->getDesktop()->getCurrView();
	assert (view != NULL);

	view->wordRight();
	return true;
}

bool PosWordLeft::redo()
{
	idCheck();

	View *view = appi->getDesktop()->getCurrView();
	assert (view != NULL);
	
	view->wordLeft();
	return true;
}

bool PosBegOfLine::redo()
{
	idCheck();
	
	View *view = appi->getDesktop()->getCurrView();
	assert (view != NULL);

	view->cursLeft(view->getCursor()->column());
	return true;
}

bool PosEndOfLine::redo()
{
	idCheck();   

	View *view = appi->getDesktop()->getCurrView();
	assert (view != NULL);

	view->cursRight(view->getCursor()->value().length() - 
					view->getCursor()->column());
					
	return true;
}

bool PosBegOfFile::redo()
{
	idCheck();

	View *view = appi->getDesktop()->getCurrView();
	assert (view != NULL);

	view->goToBegOfFile();
	return true;
}

bool PosEndOfFile::redo()
{
	idCheck();

	View *view = appi->getDesktop()->getCurrView();
	assert (view != NULL);

	view->goToEndOfFile();
	return true;
}

bool PosBegOfScr::redo()
{
	idCheck();

	View *view = appi->getDesktop()->getCurrView();
	assert (view != NULL);

	view->goToBegOfScreen();
	return true;
}

bool PosEndOfScr::redo()
{
	idCheck();

	View *view = appi->getDesktop()->getCurrView();
	assert (view != NULL);

	view->goToEndOfScreen();
	return true;
}

bool PosMidOfScr::redo()
{
	idCheck();

	View *view = appi->getDesktop()->getCurrView();
	assert (view != NULL);

	view->goToMiddleOfScreen();
	return true;
}

