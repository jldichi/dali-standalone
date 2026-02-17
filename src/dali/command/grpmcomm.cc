/********************************************************************
* Copyright (c) 1991 InterSoft Co.  All Rights Reserved
* THIS IS UNPUBLISHED PROPRIETARY SOURCE CODE OF InterSoft Co.
* The copyright notice above does not evidence any
* actual or intended publication of such source code.
*
* $Id: grpmcomm.cc,v 1.2 1997/11/18 18:39:36 guiller Exp $
*
* DESCRIPTION: Implementation of grep command's panel
*********************************************************************/

#ifdef __GNUC__
#pragma implementation
#endif

#include <ifound.h>

#include <winapp/cell.h>
#include <winapp/label.h>
#include <winapp/button.h>
#include <winapp/winobj.h>
#include <winapp/strhis.h>
#include <winapp/msgbox.h>

#include <grpmcomm.h>
#include <editor.h>

declare(ObjectAction, GrepPanel);
implement(ObjectAction, GrepPanel);

#define GREPPANEL_ROWS		11
#define GREPPANEL_COLS		47

GrepPanel::GrepPanel(String l, Int fo, Int co, const bool &edFiles)
: Panel(l, GREPPANEL_ROWS, GREPPANEL_COLS, fo, co)
{   
	idCheck();
	
	// Panel fields
	expression_		= tr_new StrCell(50, 28, 0,    1, 14, Editor::greps);
	grepFiles_		= tr_new StrCell(50, 28, 0,    2, 14, Editor::grepFiles);
	
	caseIgnored_	= tr_new CheckBox(4,  3,Editor::editorMsg("GREP_CASE"),false);
	excludeExpr_	= tr_new CheckBox(5,  3,Editor::editorMsg("GREP_EXCL"),false);
	printLines_		= tr_new CheckBox(6,  3,Editor::editorMsg("GREP_LINES"),true);
    
	editedFiles_	= tr_new CheckBox(7,  3,Editor::editorMsg("GREP_EDFILES"),false);
	
	if (edFiles)
		editedFiles_->setAction(tr_new ObjectAction(GrepPanel)(this, &GrepPanel::gfStatus));
	else
		editedFiles_->setActive(false);	

	 
 	// OK and CANCEL buttons
	grep_    	= tr_new DefaultButton( 9,  4,"grep", 10,
		defaultAction = tr_new ObjectAction(GrepPanel)(this, &GrepPanel::grep));

	egrep_      = tr_new PushButton( 9, 18,"egrep", 10,
		tr_new ObjectAction(GrepPanel)(this, &GrepPanel::egrep));

	cancel_  	= tr_new PushButton( 9, 32,libMsg("CANCEL"), 10,
		tr_new ObjectAction(GrepPanel)(this, &GrepPanel::cancel));

	// Panel Labels
	lblExp		= tr_new Label(Editor::editorMsg("GREP_EXPR"), 1,  1); 
	lblFiles	= tr_new Label(Editor::editorMsg("GREP_FILES"), 2,  1); 
    
    //
	InitObjs((WinObj**)&expression_, 9);
	InitLabels(&lblExp, 2);
	grepname = NULL_STRING;
}

const String &GrepPanel::getGrepCmd()
{ 
	idCheck();               
	
	// Build flags string

	grepcmd  = grepname + " ";
	
	if (printLines)
		grepcmd += "-n ";    
 
 	if (excludeExpr)
		grepcmd += "-v ";    

	if (caseIgnored)
		grepcmd += "-i ";    

	grepcmd += String("'") + expression + "'";
	
	return grepcmd;
}

Int GrepPanel::gfStatus()
{
	idCheck();
	
	if (editedFiles_->value() == true) {
		grepFiles_->setActive(false);
		grepFiles_->setValue("");
		grepFiles_->writeStr();
	}                                     
	else {
		grepFiles_->setActive(true);
	}                              
	
	return (editedFiles_->lastKey() == K_END) 
			? REJECT_EXIT : ACCEPT;
}

#define GREPCMD  "grep"
#define EGREPCMD "egrep"

Int GrepPanel::grep()
{     
	idCheck();
     
	grepname = GREPCMD;
	
	return setOptions();
}

Int GrepPanel::egrep()
{     
	idCheck();
     
	grepname = EGREPCMD;
	
	return setOptions();
}

Int GrepPanel::setOptions()
{	     
	// Set pattern to search
	expression	= expression_->value();
	grepFiles	= grepFiles_->value();

	// Set flags
	caseIgnored 	= caseIgnored_->value();
	excludeExpr 	= excludeExpr_->value();
	printLines 		= printLines_->value();

	editedFiles		= editedFiles_->value();

	// Test if necessary information has been entered.
	if ((expression == NULL_STRING) || 
		(grepFiles == NULL_STRING && !editedFiles)) 
	{
		MsgBox errbox(libMsg("ERROR"),
				Editor::editorMsg("GREPERR"), 6, 45, 5, 20);
		errbox.execute();
		return REJECT_EXIT;
	}	
	// Set status of panel as 'accepted'
   	accepted_ = (grepFiles != NULL_STRING || editedFiles) 
   				&& (expression != NULL_STRING);

	// Keep in command's strHistories, expression and files
	*Editor::greps << expression;
	*Editor::grepFiles << grepFiles;
	
	return ACCEPT_EXIT;
}

Int GrepPanel::cancel()
{     
	idCheck();

	return REJECT_EXIT;	
}

GrepPanel::~GrepPanel()
{
	idCheck();

	// delete de los winobjs
	tr_delete(expression_);
	tr_delete(grepFiles_);
	tr_delete(caseIgnored_);
	tr_delete(excludeExpr_);
	tr_delete(printLines_);
	tr_delete(editedFiles_);

	tr_delete(grep_);
	tr_delete(egrep_);
	tr_delete(cancel_);

	// delete de los labels
	tr_delete(lblExp);
	tr_delete(lblFiles);
}

