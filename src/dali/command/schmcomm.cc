/********************************************************************
* Copyright (c) 1991 InterSoft Co.  All Rights Reserved
* THIS IS UNPUBLISHED PROPRIETARY SOURCE CODE OF InterSoft Co.
* The copyright notice above does not evidence any
* actual or intended publication of such source code.
*
* $Id: schmcomm.cc,v 1.1.1.1 1996/04/16 18:52:50 gustavof Exp $
*
* DESCRIPTION: Implementation of search command's menus
*********************************************************************/

#ifdef __GNUC__
#pragma implementation
#endif

#include <ifound.h>

#include <winapp/cell.h>
#include <winapp/label.h>
#include <winapp/button.h>
#include <winapp/winobj.h>

#include <schmcomm.h>
#include <editor.h>

declare(ObjectAction, PFindText);
implement(ObjectAction, PFindText);

declare(ObjectAction, PReplaceText);
implement(ObjectAction, PReplaceText);

declare(ObjectAction, GoToLineDialog);
implement(ObjectAction, GoToLineDialog);

// implementacion del panel para el find

PFindText::PFindText(String l, Int nf, Int nc, Int fo, Int co)
: Panel(l, nf, nc, fo, co)
{
	idCheck();
	
	// Panel fields
	patron   = tr_new StrCell(50, 16, 0, 1, 17, Editor::finds);
	forw     = tr_new CheckBox(3, 3, Editor::editorMsg("REP_FORWARD"),true);
	rfrom    = tr_new FStrCell("nnnnnn", 0, 5, 12);
	rto      = tr_new FStrCell("nnnnnn", 0, 6, 12);
 
 	// OK and CANCEL buttons
	ok_      = tr_new DefaultButton(8,  6,libMsg("OK"), 10,
		 defaultAction = tr_new ObjectAction(PFindText)(this, &PFindText::ok));
	cancel_  = tr_new PushButton(8, 20,libMsg("CANCEL"), 10,
		tr_new ObjectAction(PFindText)(this, &PFindText::cancel));

	// Panel Labels
	ttf   = tr_new Label(Editor::editorMsg("REP_TEXT")		, 1,  1); 
	tfrom = tr_new Label(Editor::editorMsg("REP_FROM")		, 5,  3); 
	tto   = tr_new Label(Editor::editorMsg("REP_TO")		, 6,  3); 
    
    // Initialize window objects
	InitObjs((WinObj**)&patron, 6);
	InitLabels(&ttf, 3);
}

Int PFindText::ok()
{     
	idCheck();

	// Set pattern to search
	re = patron->value();

	// Set flags
	flags = FIND;
	if (forw->value())
		flags |= FORWARD;
	else
		flags |= BACKWARD;
	
	// Set to and from values
	desde = ERR;     
	if (rfrom->value())		desde = toInt(rfrom->value());
   
   	hasta = ERR;
	if (rto->value())		hasta = toInt(rto->value());

	return ACCEPT_EXIT;
}

Int PFindText::cancel()
{     
	idCheck();
    
	return REJECT_EXIT;
}

PFindText::~PFindText()
{
	idCheck();
	
	// delete de los winobjs
	tr_delete(patron);
	tr_delete(forw);
	tr_delete(rfrom);
	tr_delete(rto);
	tr_delete(ok_);
	tr_delete(cancel_);

	// delete de los labels
	tr_delete(ttf);
	tr_delete(tfrom);
	tr_delete(tto);
}

// implementacion del panel para el replace

PReplaceText::PReplaceText(String l, Int nf, Int nc, Int fo, Int co)
: Panel(l, nf, nc, fo, co)
{   
	idCheck();
	
	patron   = tr_new StrCell(50, 16, 0, 1, 17, Editor::finds);
	newtext  = tr_new StrCell(50, 16, 0, 2, 17, Editor::replaces);
	all 	 = tr_new CheckBox(4, 3,Editor::editorMsg("REP_ALL"),true);
	confirm  = tr_new CheckBox(5, 3,Editor::editorMsg("REP_CONFIRM"),true);
	rfrom    = tr_new FStrCell("nnnnnn", 0, 4, 28);
	rto      = tr_new FStrCell("nnnnnn", 0, 5, 28);

	ok_      = tr_new DefaultButton( 8,  6,libMsg("OK"), 10,
    	defaultAction = tr_new ObjectAction(PReplaceText)(this, &PReplaceText::ok));
	cancel_  = tr_new PushButton( 8, 20,libMsg("CANCEL"), 10,
		tr_new ObjectAction(PReplaceText)(this, &PReplaceText::cancel));

	ttf   = tr_new Label(Editor::editorMsg("REP_TEXT")		, 1,  2); 
	tnt   = tr_new Label(Editor::editorMsg("REP_NEWTEXT")	, 2,  2); 
	tfrom = tr_new Label(Editor::editorMsg("REP_FROM")		, 4, 19); 
	tto   = tr_new Label(Editor::editorMsg("REP_TO")		, 5, 19); 

	InitObjs((WinObj**)&patron, 8);
	InitLabels(&ttf, 4);
}

PReplaceText::~PReplaceText()
{
	idCheck();

	// delete de los winobjs
	tr_delete(patron);
	tr_delete(newtext);
	tr_delete(all);
	tr_delete(confirm);
	tr_delete(rfrom);
	tr_delete(rto);
	tr_delete(ok_);
	tr_delete(cancel_);

	// delete de los labels
	tr_delete(ttf);
	tr_delete(tnt);
	tr_delete(tfrom);
	tr_delete(tto);
}

Int PReplaceText::ok()
{     
	idCheck();

	// Set pattern to search and the text to replace it
	re    = patron->value();
	newt  = newtext->value();

	// Set flags
	flags = REPLACE;
	if (all->value())		flags |= ALL;
	if (confirm->value())	flags |= CONFIRM;

	// Set to and from values
	desde = ERR;
	if (rfrom->value())		desde = toInt(rfrom->value());

	hasta = ERR;
	if (rto->value())		hasta = toInt(rto->value());

	return ACCEPT_EXIT;
}

Int PReplaceText::cancel()
{     
	idCheck();
	return REJECT_EXIT;
}

// implementacion del panel para Go to line number
#define GOTOLINE_ROWS		3
#define GOTOLINE_COLS		29
#define GOTOLINE_ORG_ROW	5
#define GOTOLINE_ORG_COL	20

GoToLineDialog::GoToLineDialog() 
: Panel(Editor::editorMsg("GoToLine"),
		GOTOLINE_ROWS   , GOTOLINE_COLS, 
		GOTOLINE_ORG_ROW, GOTOLINE_ORG_COL)
{
	idCheck();
	
	// inicializacion de los labels
	staticText = tr_new Label(Editor::editorMsg("GOTOLINENO"), 1, 2); 

	// inicializacion de los objetos
	lineNoFld = tr_new FStrCell("nnnnnn", 0, 1, 21);
	lineNoFld->setAction(defaultAction = tr_new 
					ObjectAction(GoToLineDialog)(this, &GoToLineDialog::ok));
	
	// inicializacion del panel
	InitObjs((WinObj**) &lineNoFld, 1); 
	InitLabels(&staticText, 1);
}

GoToLineDialog::~GoToLineDialog()
{
	idCheck();

	tr_delete(lineNoFld);
	tr_delete(staticText);
}

Int GoToLineDialog::ok()
{
	idCheck();
    
    if (lineNoFld->lastKey() == K_ENTER && lineNoFld->value() != NULL_STRING) {
			lineNo = toInt(lineNoFld->value()); 
			return ACCEPT_EXIT;
	}
	
	return REJECT_EXIT;		
}

