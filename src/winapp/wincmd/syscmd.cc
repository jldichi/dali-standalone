/********************************************************************
* Copyright (c) 1991 InterSoft Co.  All Rights Reserved
* THIS IS UNPUBLISHED PROPRIETARY SOURCE CODE OF InterSoft Co.
* The copyright notice above does not evidence any
* actual or intended publication of such source code.
*
* $Id: syscmd.cc,v 1.1.1.1 1996/04/16 18:52:48 gustavof Exp $
*
* DESCRIPTION: Implementation of Desktop commands
*********************************************************************/

#ifdef __GNUG__
#	pragma implementation
#endif

#include <ifound.h>

#ifdef ISDEBUG
#include <ifound/iddebug.h>
#endif

#define INITCOMM

#include <winapp/winapp.h>
#include <winapp/wincmd.h>
#include <winapp/syscmd.h>
#include <winapp/window.h>
#include <winapp/cmdbind.h>

void bindSysCmds(CmdBinding *cb)
{
	cb->bind(SysCalculator::eventId, 	SysCalculator::cmdId);
	cb->bind(SysCharSet::eventId, 		SysCharSet::cmdId);
	cb->bind(SysPrintScreen::eventId, 	SysPrintScreen::cmdId);
	cb->bind(SysGotoShell::eventId, 	SysGotoShell::cmdId);
	cb->bind(SysDsktRefresh::eventId, 	SysDsktRefresh::cmdId);
}

void SysCalculator::undo() 
{
	idCheck();
}

bool SysCalculator::redo()
{
	idCheck();

	Window::execService(String("calculador"));
	return false;
}

void SysGotoShell::undo()
{
	idCheck();
}

bool SysGotoShell::redo()
{
	idCheck();

	Window::execService(String("GoToShell"));
	return false;
}

void SysCharSet::undo() 
{
	idCheck();
}


bool SysCharSet::redo()
{
	idCheck();

	Window::execService(String("_print_ichset"));
	return false;
}

void SysPrintScreen::undo()
{
	idCheck();
}

bool SysPrintScreen::redo()
{
	idCheck();

	Window::execService(String("_print_scr"));
	return false;
}

void SysDsktRefresh::undo()
{
	idCheck();
}

bool SysDsktRefresh::redo()
{
	idCheck();

	Window::redraw();
	return false;
}
