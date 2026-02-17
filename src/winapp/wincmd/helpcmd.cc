/********************************************************************
* Copyright (c) 1991 InterSoft Co.  All Rights Reserved
* THIS IS UNPUBLISHED PROPRIETARY SOURCE CODE OF InterSoft Co.
* The copyright notice above does not evidence any
* actual or intended publication of such source code.
*
* $Id: helpcmd.cc,v 1.1.1.1 1996/04/16 18:52:47 gustavof Exp $
*
* DESCRIPTION: Implementation of help commands
*********************************************************************/
#ifdef __GNUC__
#pragma implementation
#endif

#define INITCOMM

// IFound Library
#include <ifound.h>

// WinApp library
#include <winapp/wincmd.h>
#include <winapp/helpcmd.h>
#include <winapp/hlpview.h>
#include <winapp/winapp.h>
#include <winapp/menu.h>
          
void HelpHelp::undo() 
{ 	
	idCheck();
}

bool HelpHelp::redo()
{
	idCheck();
	appi->addHelpView("HelpHelp");
	return false;
}

void HelpIndex::undo()
{
	idCheck();
}

#include <ifound/parray.h>   

struct HelpItem {
	String key;
	String desc;
	HelpItem(const String &k, const String &d ) : key(k), desc(d) { }
};

declare(PtrArray, HelpItem);

static PtrArray(HelpItem) *fhitems = NULL;

#define MAX_HEIGHT	12
#define MAX_WIDTH	76

static const String fpReadLine(Int n)
{
	if (!fhitems->isInRange(n)) return HIGH_VALUE;
	
	return fhitems->elem(n)->desc;
}

bool HelpIndex::redo()
{
	idCheck();

 	// It is assumed that the Help Index cannot change once it is created. 
 	if (fhitems == NULL) {
		fhitems = tr_new PtrArray(HelpItem);
		// Run through commands and build help entries.
	 	for (Int cid = 0; cid < MAX_COMMAND; ++cid) {
	 		String cname = WinCmd::getName(cid);
	 		if (cname.isNull()) break;
		 	fhitems->add(tr_new HelpItem(cname,appi->getHlpLabel(WinCmd::getName(cid))));
		}
	}
 		
	assert(fhitems != NULL); 		
 		
 	Int n=fhitems->dim();

	if (n > 0) {
		ScrollMenu sm(WinApp::wappDispMsg("HelpIndex") +
					" ("+toString(n)+")", fpReadLine, 
					n > MAX_HEIGHT ? MAX_HEIGHT : n, MAX_WIDTH + 2);
		Int el;
		if ( sm.execute() != REJECT_EXIT && (el=sm.currOpt()) != ERROR)
			appi->addHelpView(fhitems->elem(el)->key);
	}
	return false;
}


void HelpOnCmd::undo() 
{ 	
	idCheck();
}

bool HelpOnCmd::redo()
{
	idCheck();

	String key;
	View *v = appi->getDesktop()->getCurrView();
	
	if ((key = v->getToken()) == NULL_STRING)
		appi->beep();
	else
		appi->addHelpView(key);
		
	if (v) {
		v->redraw();
		// v->refresh(); // toDo: bruno
	}
	return false;

	appi->addHelpView(key);

	return false;
}

bool XRefNext::redo()
{ 
    HelpView *hv = appi->getDesktop()->getCurrView()->helpView();

    assert(hv);
    
    hv->goToNext();
	return false;
}   

void XRefNext::undo()
{
}   

bool XRefPrev::redo()
{
    HelpView *hv = appi->getDesktop()->getCurrView()->helpView();
    
    assert(hv);

    hv->goToPrev();
	return false;
}   

void XRefPrev::undo()
{
	
}   

bool XRefGo::redo()
{
    HelpView *hv = appi->getDesktop()->getCurrView()->helpView();
    
    assert(hv);

	String xref = hv->getXRefKey();
    
    if (xref != NULL_STRING)
		appi->addHelpView(xref);

	return false;
}   

void XRefGo::undo()
{
	
}   
