/********************************************************************
* Copyright (c) 1993 InterSoft Co.  All Rights Reserved
* THIS IS UNPUBLISHED PROPRIETARY SOURCE CODE OF InterSoft Co.
* The copyright notice above does not evidence any
* actual or intended publication of such source code.
*
* $Id: dsktcmd.cc,v 1.1.1.1 1996/04/16 18:52:48 gustavof Exp $
*
* DESCRIPTION: Implementation of Desktop commands
*********************************************************************/
#ifdef __GNUC__
#pragma implementation
#endif

#define INITCOMM

#include <winapp/winapp.h>
#include <winapp/desktop.h>
#include <winapp/cmdbind.h>
#include <winapp/wincmd.h>
#include <winapp/dsktcmd.h>

void bindDesktopCmds(CmdBinding *cb)
{
	cb->bind(ViewMove::eventId		, ViewMove::cmdId);
	cb->bind(ViewSize::eventId		, ViewSize::cmdId);
	cb->bind(ViewZoom::eventId		, ViewZoom::cmdId);
	cb->bind(DsktNext::eventId		, DsktNext::cmdId);
	cb->bind(DsktPrev::eventId		, DsktPrev::cmdId);
	cb->bind(DsktSelect::eventId	, DsktSelect::cmdId);
	cb->bind(DsktMosaic::eventId	, DsktMosaic::cmdId);
	cb->bind(DsktCascade::eventId	, DsktCascade::cmdId);
}

void ViewZoom::undo()
{
	idCheck();
}

bool ViewZoom::redo()
{
	idCheck();
	View *view = appi->getDesktop()->getCurrView();
	assert (view != NULL);

	if (view->inOrgSize())
		view->setSize(FORIG, CORIG, FILS, COLS);
	else
		view->setOrgSize();

	return false;
}

void ViewSize::undo()
{
	idCheck();
}

bool ViewSize::redo()
{
	idCheck();
	View *view = appi->getDesktop()->getCurrView();
	assert (view != NULL);

	if (view->resize())
		view->redraw();

	return false;
}

void ViewMove::undo()
{
	idCheck();
}

bool ViewMove::redo()
{
	idCheck();
	View *view = appi->getDesktop()->getCurrView();
	assert (view != NULL);

	if (view->move())
		view->redraw();

	return false;
}


void DsktNext::undo()
{
	idCheck();
}

bool DsktNext::redo()
{
	idCheck();
	appi->getDesktop()->next();
	return false;
}
 
void DsktPrev::undo()
{
	idCheck();
}

bool DsktPrev::redo()
{
	idCheck();
	
	appi->getDesktop()->prev();
	return false;
}

void DsktSelect::undo()
{
	idCheck();
}

bool DsktSelect::redo()
{
	idCheck();
	
	appi->getDesktop()->select();
	return false;
}

void DsktMosaic::undo()
{
	idCheck();
}

bool DsktMosaic::redo()
{
	idCheck();
	
	appi->getDesktop()->tile();
	return false;
}

void DsktCascade::undo()
{
	idCheck();
}

bool DsktCascade::redo()
{
	idCheck();
	
	appi->getDesktop()->cascade();
	return false;
}


