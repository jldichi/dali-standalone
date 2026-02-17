/********************************************************************
* Copyright (c) 1991 InterSoft Co.  All Rights Reserved
* THIS IS UNPUBLISHED PROPRIETARY SOURCE CODE OF InterSoft Co.
* The copyright notice above does not evidence any
* actual or intended publication of such source code.
*
* $Id: debbox.cc,v 1.1.1.1 1996/04/16 18:52:49 gustavof Exp $
*
* DESCRIPTION: Headers of debugger command's menus
*********************************************************************/

#ifdef __GNUC__
#pragma implementation
#endif

#include <editor.h>
#include <winapp/cell.h>
#include <winapp/button.h>
#include <winapp/label.h>
#include <debbox.h>
#include <winapp/winobj.h>

declare(ObjectAction, PDebugBox);
implement(ObjectAction, PDebugBox);

PDebugBox::PDebugBox(String l, Int nf, Int nc, Int fo, Int co)
: wmClient(false), Panel(l, nf, nc, fo, co)
{
	idCheck();

	file     	= tr_new StrCell(50, 22, 0, 1, 11, Editor::debugProgs);
	
	wmclient 	= tr_new CheckBox(3, 3,"Window Manager",false);
	wmclient->setAction(tr_new ObjectAction(PDebugBox)(this, &PDebugBox::ttyStatus));

	tty      	= tr_new StrCell(30, 15, 0, 4, 9);
    tty->setActive(false);
    
	ok_ 	= tr_new DefaultButton(6,  6,libMsg("OK"), 10,
		defaultAction = tr_new ObjectAction(PDebugBox)(this, &PDebugBox::ok));

	cancel_= tr_new PushButton(6, 20,libMsg("CANCEL"), 10,
		tr_new ObjectAction(PDebugBox)(this, &PDebugBox::cancel));

	tfile     	= tr_new Label(Editor::editorMsg("DEBUGFILE"), 1, 3); 
	ttty 	  	= tr_new Label("tty :", 4, 3);
    
	InitObjs((WinObj**) &file, 5);
	InitLabels(&tfile, 2);
}

PDebugBox::~PDebugBox()
{
	idCheck();

	tr_delete(file);
	tr_delete(wmclient);
	tr_delete(tty);
	tr_delete(ok_);
	tr_delete(cancel_);
	
	// delete de los labels
	tr_delete(tfile);
	tr_delete(ttty);
}

Int PDebugBox::ttyStatus()
{
	idCheck();
	
	if (wmclient->value() == false) {
		tty->setActive(false);
		tty->setValue("");
		tty->writeStr();
	}                                     
	else {
		tty->setActive(true);
	}                              
	
	return ACCEPT;
}

Int PDebugBox::ok()
{   
	idCheck();
	  
	fName 		= file->value();
	stty  		= tty->value();
	wmClient 	= wmclient->value();

	return ACCEPT_EXIT;
}

Int PDebugBox::cancel()
{          
	idCheck();
	
	return REJECT_EXIT;
}        




