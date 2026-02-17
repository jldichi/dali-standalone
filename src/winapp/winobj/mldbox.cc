/********************************************************************
* Copyright (c) 1991 InterSoft Co.  All Rights Reserved
* THIS IS UNPUBLISHED PROPRIETARY SOURCE CODE OF InterSoft Co.
* The copyright notice above does not evidence any
* actual or intended publication of such source code.
*
* $Id: mldbox.cc,v 1.2 1997/11/18 15:54:41 guiller Exp $         
*
* DESCRIPTION: Implementation of dialog box class
*
*********************************************************************/

#ifdef __GNUC__
#pragma implementation
#endif

#include <ifound.h>
#include <winapp/mldbox.h>
#include <winapp/menu.h>
#include <winapp/label.h>
#include <winapp/button.h>
#include <winapp/winobj.h>

declare(ObjectAction, MLDBox);
implement(ObjectAction, MLDBox);

// dimensiones minimas de un dialog box
#define MIN_FIL			5
#define MIN_COL			40   
#define MAX_WIDTH		Window::rootWindow()->width()
#define BUTTON_SPACE 	12

MLDBox::MLDBox(String l, String text, Int def) 
: Panel(l, MIN_FIL, MIN_COL, 1, 1)
{
	// inicializacion de los labels
	Int la	= 0;
	ncol  	= max(l.length() + 2,MIN_COL);
	nfil	= MIN_FIL;
	
	for (StrCursor wd(text,"\n"); wd && la < MAX_LABELS; wd++, la++) { 
		 Int tw = wd.value().length() + 2;
         if (ncol < tw )
         	 ncol = min(tw,MAX_WIDTH);
    }                       
    
	la=0;
	for (StrCursor sc(text,"\n"); sc && la < MAX_LABELS; sc++, la++) 
		msg[la] = tr_new Label(sc.value()(0,ncol-2), la+1, 1, ncol);           

	// Place panel centered	
    ncol    += 2;
    nfil	= la + 4;
    forig	= (Window::rootWindow()->height() - nfil) / 2;
    corig	= (Window::rootWindow()->width() - ncol)  / 2;

	// default del 
    option = def;

 	// OK and CANCEL buttons
 	int freecols = (ncol - (BUTTON_SPACE * 3)) / 2;
 	
	yes_	= (def == DBOX_YES) 
		? tr_new DefaultButton(nfil-2, freecols ,libMsg("YES"), 10) 
		: tr_new PushButton   (nfil-2, freecols ,libMsg("YES"), 10);
				
	yes_->setAction(tr_new ObjectAction(MLDBox)(this, &MLDBox::yes));
	
	no_		= (def == DBOX_NO) 
		? tr_new DefaultButton(nfil-2, freecols + BUTTON_SPACE,libMsg("NO"), 10) 
		: tr_new PushButton   (nfil-2, freecols + BUTTON_SPACE,libMsg("NO"), 10);

	no_->setAction(tr_new ObjectAction(MLDBox)(this, &MLDBox::no));
	
	cancel_  = (def == DBOX_CANCEL) 
		? tr_new DefaultButton(nfil - 2, freecols + (BUTTON_SPACE * 2),libMsg("CANCEL"), 10) 
		: tr_new PushButton   (nfil - 2, freecols + (BUTTON_SPACE * 2),libMsg("CANCEL"), 10);       
				
	cancel_->setAction(tr_new ObjectAction(MLDBox)(this, &MLDBox::cancel));

    
	// inicializacion del panel
	InitLabels(&msg[0], nLabels = la);
	InitObjs  ((WinObj**)&yes_, 3); 
}

MLDBox::~MLDBox()
{
	tr_delete(yes_);
	tr_delete(no_);
	tr_delete(cancel_);
	
	for (Int a = 0; a < nLabels ; a++)
		tr_delete(msg[a]);
}

Int MLDBox::yes()
{
	idCheck();

	option = DBOX_YES;
	return ACCEPT_EXIT;
}

Int MLDBox::no()
{
	idCheck();

	option = DBOX_NO;
	return ACCEPT_EXIT;
}

Int MLDBox::execute(UnChar)
{         
	return Panel::execute((UnChar)option);
}                          

Int MLDBox::cancel()
{
	idCheck();

	option = DBOX_CANCEL;
	return ACCEPT_EXIT;
}

