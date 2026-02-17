/********************************************************************
* Copyright (c) 1991 InterSoft Co.  All Rights Reserved
* THIS IS UNPUBLISHED PROPRIETARY SOURCE CODE OF InterSoft Co.
* The copyright notice above does not evidence any
* actual or intended publication of such source code.
*
* $Id: command.cc,v 1.2 2005/06/16 22:07:51 albertoi Exp $
*
* DESCRIPTION:	Implementation of command class
*********************************************************************/

#ifdef __GNUC__
#pragma implementation
#pragma implementation "action.h"
#endif

#ifdef ISDEBUG
#include <ifound/iddebug.h>
#endif

#include <command.h>
#include <editor.h>
#include <dview.h>
#include <dtext.h>
#include <dsensor.h>
#include <ifound/list.h>

declare(PtrList,DaliView);
declare(PtrListCursor,DaliView);

// clase Command

Command::Command() 
{
	idStart(); 
}

Command::~Command()  
{
	idCheck();
	idStop(); 
}

void Command::execute(char c, Int n)
{
	idCheck();
	// Check to see if the command can be done
	if (Editor::nroActViews() == 0 && !allowNoViews()) {
		Editor::beep();
		return;
	}

	chr = c; rep = n;

	// Is the autosave option turned on?
	if (Editor::autoSaveFreq()) {

		// Has the timer been initialized?
		if (Editor::lastUpdate) {
			time_t tm = time(NULL);

			// Should we save?
			if (tm - Editor::lastUpdate > (Editor::autoSaveFreq())) {
				if (Editor::nroActViews()) {
					DaliView *cview = cView.value();
					if (cview->isTextMod())
						cview->rememberFile();
					Editor::lastUpdate = tm;
				}					
		   	}
		} else
			// Initialize the timer
			Editor::lastUpdate = time(NULL);
	}

	clear();

	if (_redo())
		Editor::his->insertCommand(clone());
}

Int Command::makeCommand(const char *s)
{
	idCheck();
	return Editor::sen->makeCommand(s, this);
}

DaliTextCursor &Command::cursor()
{
	idCheck();
	return *(cView.value()->_cursor);
}

bool Command::allowNoViews()
{
	idCheck();
	return false;                                                             
}	

// fin clase Command

// clase PosCommand

bool PosCommand::_redo()
{
	idCheck();
	bool ret;

	// obtengo los parametros para el undo
	cfil = cursor().order();
	ccol = cursor().column();

	DaliView *view = cView.value();

	if (!view->hasBlock())
		ret = redo();
	else {

		DaliTextCursor block(view->getBlock());
		//DaliTextCursor block = view->getBlock();
		view->delBlock(false);
		ret = redo();
		view->setBlock(block);

	}
	view->writeStLine();
	return ret;
}

void PosCommand::_undo()
{
	idCheck();
	DaliView *view = cView.value();

	undo();

	if (!view->hasBlock()) 
		view->goCursor(cfil, ccol);
	else {
		DaliTextCursor block(view->getBlock());
		//DaliTextCursor block = view->getBlock();
		view->delBlock(false);
		view->goCursor(cfil, ccol);
		view->setBlock(block);

	} 
	view->writeStLine();
}

// fin clase PosCommand

// clase BlockCommand

bool BlockCommand::_redo()
{
	idCheck();
	DaliView *view = cView.value();

	// si no estoy en modo bloque beep
	if (!view->hasBlock()) {
		Editor::beep();
		return false;
	}

	// obtengo las coordenadas del bloque 
	//DaliTextCursor block = view->getBlock();
	DaliTextCursor block(view->getBlock());
	fil   = block.order();
	col   = block.column();

	// borro el bloque antes
	view->delBlock();

	// ejecuto la funcion virtual
	bool ret = redo(block);
	
	// si la funcion fallo -> restauro el bloque
	if (!ret) 
		view->setBlock(block);
	return ret;
}

void BlockCommand::_undo()
{
	idCheck();
	// ejecuto el undo del comando
	undo();

	// restauro el modo bloque
	DaliTextCursor block = cursor();
	block.goTo(fil, col);
	
	cView.value()->setBlock(block);
}

// fin clase BlockCommand

// clase AnyCommand

bool AnyCommand::_redo()
{
	idCheck();
	DaliView *view = cView.value();

	DaliTextCursor block(view->getBlock());
	//DaliTextCursor block = view->getBlock();
	fil = col = ERR;

	if (block) { 
		fil = block.order();
		col = block.column();
	}

	if (block)
		view->delBlock();

	bool ret = redo();

	if (!ret && block)
		view->setBlock(block);
    
    view->writeStLine();
	return ret;
}

void AnyCommand::_undo()
{                              
	idCheck();
	DaliView *view = cView.value();
	undo();

	if (fil != ERR || col != ERR) { 
		DaliTextCursor block = cursor();
		block.goTo(fil, col);
		cView.value()->setBlock(block);
	}                                  
	view->writeStLine();
}

// fin clase AnyCommand

// clase ControlCommand

void ControlCommand::_undo()
{
	idCheck();
	undo();
}

bool ControlCommand::_redo()
{
	idCheck();
	return redo();
}

// fin clase ControlCommand


// clase ControlCommandNoViews

bool ControlCommandNoViews::allowNoViews()
{
	idCheck();
	return true;
}

// fin clase ControlCommandNoViews

