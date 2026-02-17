/********************************************************************
* Copyright (c) 1991 InterSoft Co.  All Rights Reserved
* THIS IS UNPUBLISHED PROPRIETARY SOURCE CODE OF InterSoft Co.
* The copyright notice above does not evidence any
* actual or intended publication of such source code.
*
* $Id: edlbox.cc,v 1.1.1.1 1996/04/16 18:52:49 gustavof Exp $
*
* DESCRIPTION: Implementation of debug panel
*********************************************************************/

#ifdef __GNUC__
#pragma implementation
#endif

#include <edlbox.h>
#include <ifound/str.h>
#include <editor.h>
#include <winapp/label.h>
#include <winapp/cell.h>
#include <winapp/winobj.h>

declare(PtrList,DaliView);
declare(PtrListCursor,DaliView);

declare(ObjectAction, EdLineDialog);
implement(ObjectAction, EdLineDialog);
                     
EdLineDialog::EdLineDialog(StrHistory *his, String title, 
    String stText, Int rows, Int cols, Int srow, Int scol)
    : strHis(his), Panel(title, rows, cols, srow, scol)
{   
	idCheck();
	                                    
	staticText = tr_new Label(stText, 1, 2); 

	Int stLen = stText.length();                      
	
	cmdFld = tr_new StrCell(128, 43, 0, 1, stLen+4, strHis);
	cmdFld->setAction(
	  defaultAction = tr_new ObjectAction(EdLineDialog)(this, &EdLineDialog::ok)
	);

	// inicializacion del panel
	InitObjs((WinObj**) &cmdFld, 1);
	InitLabels(&staticText, 1); 

	// creo el string para retornar el commando
	cmd = tr_new String;
}

EdLineDialog::~EdLineDialog()
{
	idCheck();
	
	tr_delete(cmdFld);
	tr_delete(staticText);
	tr_delete(cmd);
}

Int EdLineDialog::execute(UnChar c)
{       
	idCheck();

	if (!active) 
		return REJECT;
		
	// Check if the editor is in Command mode
	const String &lbl = staticText->value();
	if (Editor::isCmdLine()) {
	    Int ch = Editor::getCmdLine(lbl, *cmd, strHis);
	    if (ch != K_TAB)    
		    return (accepted_ = (ch != ERROR && *cmd != NULL_STRING))
		    		 ? ACCEPT : REJECT ;
		Window::refresh();
	}

	return Panel::execute(c);
}

Int EdLineDialog::ok()
{
	idCheck();

	if (cmdFld->lastKey() == K_ENTER && cmdFld->value() != NULL_STRING) {
		 *cmd = cmdFld->value();
		 return ACCEPT_EXIT;
    }  
    
	return REJECT_EXIT;		
}

