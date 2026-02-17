 /********************************************************************
* Copyright (c) 1991 InterSoft Co.  All Rights Reserved
* THIS IS UNPUBLISHED PROPRIETARY SOURCE CODE OF InterSoft Co.
* The copyright notice above does not evidence any
* actual or intended publication of such source code.
*
* $Id: dprefm.cc,v 1.1.1.1 1996/04/16 18:52:52 gustavof Exp $
*
* DESCRIPTION: Implementation of pref command's menus
*
*********************************************************************/

#include <ifound.h>
#include <winapp/cell.h>
#include <winapp/label.h>
#include <dprefm.h>
#include <winapp/button.h>
#include <winapp/winobj.h>

declare(WinObjAction, PrefMenu);
implement(WinObjAction, PrefMenu);

// Preference Menu Implementation

PrefMenu::PrefMenu(String l, Int fo, Int co)
: Panel(l, 13, 70, fo, co)
{
    // Environment
	dc		= tr_new CheckBox(3, 3,"Desktop Background", TRUE);
	cm		= tr_new CheckBox(4, 3,"Command Mode" 		, TRUE);
	sd		= tr_new CheckBox(5, 3,"Save Desktop" 		, TRUE);
    
    // Edit       
	fs		= tr_new CheckBox(6, 3,"Function Sensor"	, TRUE);
	ts		= tr_new FStrCell("nn", 0,  3,50);		
    
    // Security   
	hs		= tr_new FStrCell("nnnn", 0, 4, 50);
	af		= tr_new FStrCell("nnnn", 0, 5, 50);
    
    // Windows    
	fh		= tr_new FStrCell("nn", 0, 6, 50);
	mv		= tr_new FStrCell("nn", 0, 7, 50);
	sv		= tr_new FStrCell("nn", 0, 8, 50);
	wt		= tr_new FStrCell("nn", 0, 9, 50);
    

    // Ok and Cancel buttoms
	ok_       = tr_new DefaultButton(12, 20, "Ok", 10,
		defaultAction = tr_new WinObjAction(PrefMenu)(this, &PrefMenu::ok));

	cancel_   = tr_new PushButton(12, 30, "Cancel", 10,
		tr_new WinObjAction(PrefMenu)(this, &PrefMenu::cancel));


    // Labels

	    // Edit
		lts		= tr_new Label("TabSize : " 				, 3, 30);
	    
	    // Security
		lhs		= tr_new Label("History Size : "			, 4, 30);
		laf		= tr_new Label("Auto Save Frequency : "	, 5, 30);
	                              
	    // Windows
		lfh		= tr_new Label("Footer Height : "		, 6, 30);
		lmv		= tr_new Label("Max Views : "			, 7, 30);
		lsv		= tr_new Label("Scratch Views :"		, 8, 30);
		lwt		= tr_new Label("Window Mosaic : "		, 9, 30);
    
	InitObjs((WinObj**)&dc, 13);
	InitLabels(&lts, 7);
}

PrefMenu::~PrefMenu()
{             
    // Environment
	tr_delete(dc);
	tr_delete(cm);
	tr_delete(sd);
    
    // Edit 
	tr_delete(fs);
	tr_delete(ts);
    
    // Security
	tr_delete(hs);
	tr_delete(af);
    
    // Windows
	tr_delete(fh);
	tr_delete(mv);
	tr_delete(sv);
	tr_delete(wt);
    
    // Ok and Cancel buttoms
	tr_delete(ok_);
	tr_delete(cancel_);
	
    // Edit 
	tr_delete(lts);
    
    // Security
	tr_delete(lhs);
	tr_delete(laf);
    
    // Windows
	tr_delete(lfh);
	tr_delete(lmv);
	tr_delete(lsv);
	tr_delete(lwt);	
}

Int PrefMenu::cancel()
{
	return REJECT_EXIT;
}

Int PrefMenu::ok()
{
    deskTopChar 	= dc->value();
    commandMode 	= cm->value();
    saveDeskTop 	= sd->value();
    functionSensor 	= fs->value();

	tabSize			= toInt(ts->value());
	historySize		= toInt(hs->value());
	autoSaveFreq	= toInt(af->value());
	footerHeight	= toInt(fh->value());
	maxViews		= toInt(mv->value());
	scratchViews	= toInt(sv->value());
	windowsTile		= toInt(wt->value());

	return ACCEPT_EXIT;			
}

