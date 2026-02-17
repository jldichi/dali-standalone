/********************************************************************
* Copyright (c) 1991 InterSoft Co.  All Rights Reserved
* THIS IS UNPUBLISHED PROPRIETARY SOURCE CODE OF InterSoft Co.
* The copyright notice above does not evidence any
* actual or intended publication of such source code.
*
* $Id: msgbox.cc,v 1.1.1.1 1996/04/16 18:52:45 gustavof Exp $
*
* DESCRIPTION: Implementation of message box class
*
*********************************************************************/

#ifdef __GNUC__
#pragma implementation
#endif

#include <ifound.h>
#include <winapp/msgbox.h>
#include <winapp/label.h>
#include <winapp/cell.h>

#include <winapp/button.h>
#include <winapp/winobj.h>

declare(ObjectAction, MsgBox);
implement(ObjectAction, MsgBox);

// dimensiones minimas de un message box
#define LEN_FIELD	10
#define MIN_FIL		5
#define MIN_COL		10	

MsgBox::MsgBox(String l, String text, Int nf, Int nc, Int fo, Int co) 
: Panel(l, nf<MIN_FIL ? MIN_FIL : nf, nc<MIN_COL ? MIN_COL : nc, fo, co)
{
	idCheck();

	nf = nf<MIN_FIL ? MIN_FIL : nf;
	nc = nc<MIN_COL ? MIN_COL : nc;

	ok_ = tr_new DefaultButton(nf - 2,(nc-LEN_FIELD) / 2 ,libMsg("OK"), 10); 
 	ok_->setAction(defaultAction = tr_new ObjectAction(MsgBox)(this, &MsgBox::ok)); 

	// inicializacion de los labels
	msg = tr_new Label(text, 1, 1, nc-2); 

	// inicializacion del panel
	InitObjs  ((WinObj**)&ok_, 1); 
	InitLabels(&msg, 1);

}

MsgBox::~MsgBox()
{
	idCheck();

	tr_delete(ok_);
	tr_delete(msg);
}

Int MsgBox::ok()
{
	idCheck();

	return ACCEPT_EXIT;
}

