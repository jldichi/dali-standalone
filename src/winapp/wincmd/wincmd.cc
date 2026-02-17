/********************************************************************
* Copyright (c) 1994 InterSoft Co.  All Rights Reserved
* THIS IS UNPUBLISHED PROPRIETARY SOURCE CODE OF InterSoft Co.
* The copyright notice above does not evidence any
* actual or intended publication of such source code.
*
* $Id: wincmd.cc,v 1.3 2004/09/29 21:07:18 damianm Exp $
*
* DESCRIPTION: Implementation of WinCmd class
*********************************************************************/
#ifdef __GNUC__
#pragma implementation
#endif

#include <string.h>

#include <winapp/winapp.h>
#include <winapp/wincmd.h>
#include <winapp/view.h>
#include <winapp/text.h>
#include <ifound/list.h>
#include <stdio.h>

CmdItem *WinCmd::commands=NULL;

WinCmd::WinCmd() 
{
	idStart(); 
}

WinCmd::~WinCmd()  
{
	idCheck();
	idStop(); 
}

Int WinCmd::addCmd(const char *s, WinCmd *c)
{
	Int i;

	if (!commands)
		commands = tr_new CmdItem[MAX_COMMAND];

	for (i=0; i < MAX_COMMAND && commands[i].name; i++)
		if (strcmp(commands[i].name,s) == 0) 
			return i;

	if (i < MAX_COMMAND) {
		//fprintf(stderr,"%d %s\n",i,s);
		commands[i].name = s;
		commands[i].cmd  = c;
		return i;
	}

	return ERR;	
}

WinCmd *WinCmd::getCmd(const Int cmd) // this is a static function
{
	return (cmd >= 0 && cmd < MAX_COMMAND) ? commands[cmd].cmd : NULL;
}

String WinCmd::getName(const Int cmd) // this is a static function
{   
	String tmp;

	if(cmd >= 0 && cmd < MAX_COMMAND) 
		tmp = commands[cmd].name;

	return tmp;
}

ViewCmd::~ViewCmd()  
{
	idCheck();
}

void ViewCmd::execute(char c, WinApp *app)
{
	idCheck();
    assert(app != NULL);
    appi = app;
    
	View *view = appi->getDesktop()->getCurrView();
	assert (view != NULL);

	chr = c;
	clear();

	if (_redo()) ;
//		view->getHistory()->insertCommand(clone());
}

void DesktopCmd::execute(char c, WinApp *app)
{
	idCheck();

    assert(app != NULL);
    appi = app;

	// If there are no views in the desktop, 
	// check if the command can be executed.
    if (!allowEmptyDesktop() && app->getDesktop()->getViews() == 0) {
		app->beep();  
		// toDo: See where we can display an error message.
		//       Podria implentarse un errorHandler() de commandos ?
		return;    	
    } 

	chr = c;
	clear();
	_redo();
}

bool PosCmd::_redo()
{
	idCheck();
	bool ret;
	EditableView *eview = appi->getDesktop()->getCurrEdView();
	View *view = appi->getDesktop()->getCurrView();
	assert(implies(eview == NULL, view != NULL));

	// obtengo los parametros para el undo
	urow = view->getRow();
	ucol = view->getCol();

	if (eview != NULL && eview->hasBlock()) {
		TextCursor block = eview->getBlock();
		eview->delBlock(false);
		ret = redo();
		eview->setBlock(block);
	} else ret = redo();

	view->writeStLine();
	return ret;
}

void PosCmd::_undo()
{
	idCheck();
	EditableView *eview = appi->getDesktop()->getCurrEdView();
	View *view = appi->getDesktop()->getCurrView();
	assert(implies(eview == NULL, view != NULL));

	undo();

	if (eview != NULL && eview->hasBlock()) {
		TextCursor block = eview->getBlock();
		eview->delBlock(false);
		eview->goCursor(urow, ucol);
		eview->setBlock(block);
	} else view->goCursor(urow, ucol);

	view->writeStLine();
}

bool EdPosCmd::_redo()
{
	idCheck();
	bool ret;

	View *view = appi->getDesktop()->getCurrView();
	assert (view != NULL);

	// obtengo los parametros para el undo
	urow = view->getRow();
	ucol = view->getCol();
    
    if (view->edView() && view->edView()->hasBlock()) {
 		EditableTextCursor block(view->edView()->getBlock());
 		view->edView()->delBlock(false);
		ret = redo();
 		view->edView()->setBlock(block);
	}
	else {
		ret = redo();
	}

	view->writeStLine();
	return ret;
}

void EdPosCmd::_undo()
{
	idCheck();
	View *view = appi->getDesktop()->getCurrView();
	assert (view != NULL);

	undo();

    if (view->edView() && view->edView()->hasBlock()) {
		EditableTextCursor block(view->edView()->getBlock());
		view->edView()->delBlock(false);
		view->goCursor(urow, ucol);
		view->edView()->setBlock(block);
	}
	else {
		view->goCursor(urow, ucol);
	} 
	view->writeStLine();
}

bool BlockCmd::_redo()
{
	idCheck();
	EditableView *view = appi->getDesktop()->getCurrEdView();
	assert (view != NULL);

	// si no estoy en modo bloque beep
	if (!view->hasBlock()) {
		// App::beep();
		// toDo: See how we can beep()	
		//       See where we can display an error message.
		//       Podria implentarse un errorHandler() de commandos ?
		return false;
	}

	// obtengo las coordenadas del bloque 
	EditableTextCursor block(view->getBlock());
	urow   = block.order();
	ucol   = block.column();

	// borro el bloque antes
	view->delBlock();

	// ejecuto la funcion virtual
	bool ret = redo(block);
	
	// si la funcion fallo -> restauro el bloque
	if (!ret) 
		view->setBlock(block);
	return ret;
}

void BlockCmd::_undo()
{
	idCheck();
	// ejecuto el undo del comando
	EditableView *view = appi->getDesktop()->getCurrEdView();
	assert (view != NULL);

	undo();

	// restauro el modo bloque
	EditableTextCursor block(*view->getCursor());
	block.goTo(urow, ucol);
	
	view->setBlock(block);
}

bool EditCmd::_redo()
{
	idCheck();
	View *view = appi->getDesktop()->getCurrView();
	assert (view != NULL);

	EditableTextCursor block(view->edView()->getBlock());
	urow = ucol = ERR;

	if (block) { 
		urow = block.order();
		ucol = block.column();
	}

	// toDo: Verificar porque no esta en el if de arriba
	if (block)
		view->edView()->delBlock();

	bool ret = redo();
                
	if (!ret && block)
		view->edView()->setBlock(block);
    
    view->writeStLine();
	return ret;
}

void EditCmd::_undo()
{                              
	idCheck();
	View *view = appi->getDesktop()->getCurrView();
	assert (view != NULL);

	undo();

	if (urow != ERR || ucol != ERR) { 
		EditableTextCursor block(view->edView()->getBlock());
		block.goTo(urow, ucol);
		view->edView()->setBlock(block);
	}                                  
	view->writeStLine();
}

bool EmptyDesktopCmd::_redo()
{
	idCheck();
	/*
	  Aqui podria ir todo lo que fuera generico para comandos de esta clase
	*/
	return redo();
}

void EmptyDesktopCmd::_undo()
{
	idCheck();
	/*
	  Aqui podria ir todo lo que fuera generico para comandos de esta clase
	*/
	undo();
}

NoEmptyDesktopCmd::NoEmptyDesktopCmd() : DesktopCmd() { idCheck(); }
NoEmptyDesktopCmd::~NoEmptyDesktopCmd() { idCheck(); }

bool NoEmptyDesktopCmd::_redo()
{
	idCheck();
	/*
	  Aqui podria ir todo lo que fuera generico para comandos de esta clase
	*/
	return redo();
}

void NoEmptyDesktopCmd::_undo()
{
	idCheck();
	/*
	  Aqui podria ir todo lo que fuera generico para comandos de esta clase
	*/
	undo();
}

