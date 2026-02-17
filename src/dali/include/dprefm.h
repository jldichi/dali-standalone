/********************************************************************
* Copyright (c) 1991 InterSoft Co.  All Rights Reserved
* THIS IS UNPUBLISHED PROPRIETARY SOURCE CODE OF InterSoft Co.
* The copyright notice above does not evidence any
* actual or intended publication of such source code.
*
* $Id: dprefm.h,v 1.1.1.1 1996/04/16 18:52:54 gustavof Exp $
*
* DESCRIPTION: Headers of pref command's menus
*
*********************************************************************/

#include <panel.h>
#include <str.h>

class StrCell;
class FStrCell;
class Label;
class PushButton;
class DefaultButton;
class CheckBox;

// panel para la captura de datos para el find
class PrefMenu : public Panel
{

private:    
    // Environment
	bool 	deskTopChar;
	bool 	commandMode;
	bool 	saveDeskTop;
    
    // Edit 
	bool 	autoIndent;
	bool 	functionSensor;
	Int 	tabSize;
    
    // Security
	Int 	historySize;
	Int 	autoSaveFreq;
    
    // Windows
	Int 	footerHeight;
	Int 	maxViews;
	Int 	scratchViews;
	Int 	windowsTile;

    PrefMenu(const PrefMenu&) ;
    void operator=(const PrefMenu&) ;
       
public:
	CheckBox		*dc;
	CheckBox		*cm;
	CheckBox		*sd;
    
    // Edit 
	CheckBox		*fs;
	FStrCell		*ts;
    
    // Security
	FStrCell 	*hs;
	FStrCell 	*af;
    
    // Windows
	FStrCell 	*fh;
	FStrCell 	*mv;
	FStrCell 	*sv;
	FStrCell 	*wt;
    
    // Ok and Cancel buttoms
	DefaultButton	 *ok_;
	PushButton		 *cancel_;
    
    
    // Labels
	Label 		 *ldc;
	Label 		 *lcm;
	Label 		 *lsd;
    
    // Edit 
	Label 		 *lai;
	Label 		 *lvm;
	Label 		 *lfs;
	Label 		 *lts;
    
    // Security
	Label 		 *lhs;
	Label 		 *laf;
    
    // Windows
	Label 		 *lfh;
	Label 		 *lmv;
	Label 		 *lsv;
	Label 		 *lwt;
    
	PrefMenu(String label, Int fo=ERROR, Int co=ERROR);
	virtual ~PrefMenu();

	// resultados del panel
	bool 	getDeskTopChar()  		{ return deskTopChar ;}
	bool 	getCommandMode()  		{ return commandMode  ;}
	bool 	getSaveDesktop()  		{ return saveDeskTop  ;}
    
    // Edit 
	bool 	getAutoIndent()  		{ return autoIndent  ;}
	bool 	getFunctionSensor()  	{ return functionSensor ;}
	Int 	getTabSize()  			{ return tabSize  ;}
    
    // Security
	Int 	getHistorySize()  		{ return historySize ;}
	Int 	getAutoSaveFreq()  		{ return autoSaveFreq  ;}
    
    // Windows
	Int 	getFooterHeight()  		{ return footerHeight  ;}
	Int 	getMaxViews()  			{ return maxViews  ;}
	Int 	getScratchViews()  		{ return scratchViews  ;}
	Int 	getWindowsTile()  		{ return windowsTile  ;}

	// WinActions
	Int ok();
	Int cancel();
};

