/********************************************************************
* Copyright (c) 1991 InterSoft Co.  All Rights Reserved
* THIS IS UNPUBLISHED PROPRIETARY SOURCE CODE OF InterSoft Co.
* The copyright notice above does not evidence any
* actual or intended publication of such source code.
*
* $Id: dbox.cc,v 1.1.1.1 1996/04/16 18:52:46 gustavof Exp $
*
* DESCRIPTION: Implementation of dialog box class
*
*********************************************************************/

#ifdef __GNUC__
#pragma implementation
#endif

#include <ifound.h>
#include <winapp/dbox.h>
#include <winapp/menu.h>
#include <winapp/label.h>
#include <winapp/button.h>
#include <winapp/winobj.h>

declare(ObjectAction, DBox);
implement(ObjectAction, DBox);

// dimensiones minimas de un dialog box
#define LEN_PULL	22
#define MIN_FIL		5
#define MIN_COL		LEN_PULL+4

DBox::DBox(String l, String text, Int def, Int nf, Int nc, Int fo, Int co) 
: Panel(l, nf<MIN_FIL ? MIN_FIL : nf, nc<MIN_COL ? MIN_COL : nc, fo, co)
{
	idCheck();

	nf = nf<MIN_FIL ? MIN_FIL : nf;
	nc = nc<MIN_COL ? MIN_COL : nc;

 	// OK and CANCEL buttons
	yes_     = (def == DBOX_YES) 
				? tr_new DefaultButton(8,  5,libMsg("YES"), 10) 
				: tr_new PushButton(8,  5,libMsg("YES"), 10);
				
	yes_->setAction(tr_new ObjectAction(DBox)(this, &DBox::yes));
	
	no_     = (def == DBOX_NO) 
				? tr_new DefaultButton(8, 12,libMsg("NO"), 10) 
				: tr_new PushButton(8, 12,libMsg("NO"), 10);
	no_->setAction(tr_new ObjectAction(DBox)(this, &DBox::no));
	
	yes_     = (def == DBOX_YES) 
				? tr_new DefaultButton(8, 19,libMsg("CANCEL"), 10) 
				: tr_new PushButton(8, 19,libMsg("CANCEL"), 10);       
				
	cancel_->setAction(tr_new ObjectAction(DBox)(this, &DBox::cancel));

	// default del 
	option = def;
	
	// inicializacion de los labels
	msg = tr_new Label(text, 1, 1, nc-2); 

	// inicializacion del panel
	InitObjs  ((WinObj**)&yes_, 3); 
	InitLabels(&msg, 1);
}

DBox::~DBox()
{
	idCheck();

	tr_delete(yes_);
	tr_delete(no_);
	tr_delete(cancel_);
	tr_delete(msg);
}

Int DBox::execute(UnChar)
{         
	return Panel::execute((UnChar)option);
}                          

Int DBox::yes()
{
	idCheck();

	option = DBOX_YES;
	return ACCEPT_EXIT;
}

Int DBox::no()
{
	idCheck();

	option = DBOX_NO;
	return ACCEPT_EXIT;
}

Int DBox::cancel()
{
	idCheck();

	option = DBOX_CANCEL;
	return ACCEPT_EXIT;
}
