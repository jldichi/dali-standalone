/********************************************************************
* Copyright (c) 1991 InterSoft Co.  All Rights Reserved
* THIS IS UNPUBLISHED PROPRIETARY SOURCE CODE OF InterSoft Co.
* The copyright notice above does not evidence any
* actual or intended publication of such source code.
*
* $Id: hlpview.cc,v 1.1.1.1 1996/04/16 18:52:46 gustavof Exp $
*
* DESCRIPTION: Implementation of the help view
*********************************************************************/

#ifdef __GNUC__
#	pragma implementation
#endif

#ifdef ISDEBUG
#	include <ifound/iddebug.h>
#endif

// WinApp library
#include <winapp/eview.h>
#include <winapp/hlpview.h>
#include <winapp/text.h>
#include <winapp/etext.h>
#include <winapp/textcur.h>
#include <winapp/winapp.h>
#include <ifound/str.h>

declare(PtrArray, CrossReference);

HelpView::HelpView(const String &label, const String &contents, 
					const String &key, WinApp *app)
: EditableView(label)
{         
	idCheck();         
	xrefs = tr_new PtrArray(CrossReference);
    
	text->activate();  // toDo: Hace falta esto ?

	String attrb = String(BEGIN_ATTR) + "1m[";
	String attre = String("] ") + ATTR_NORMAL;
	Int attrslen = attrb.length() + attre.length() - 3;
	
    // Process help contents
    Int xrfil = 0; 
	for (StrCursor sc(contents,"\n"); sc ; ++sc, ++xrfil) {
		String aux = sc.value();
		Int xrcolb;
		Int xrcole;
		Int attrcorr = 0;
		while ((xrcolb = aux.index('@')) != ERR &&
			   (xrcole = aux.index('@', xrcolb+1)) != ERR) 
		{
			String keyWord = aux(xrcolb+1, xrcole-xrcolb-1);
			String descr = app->getHlpLabel(keyWord);
			if (descr == HIGH_VALUE) descr = keyWord; // .. or std err msg ?
			aux = aux(0, xrcolb) + attrb + descr + attre + aux(xrcole+1);
			xrefs->add(tr_new CrossReference(keyWord, 
									xrfil, xrcolb-attrcorr, 
									xrfil, xrcolb-attrcorr+descr.length()+1));
			attrcorr += attrslen;
		}
		text->add(aux);
	}
	text->encodeAttrs();
	current = ERR;
	easyExit_ = TRUE;
	idCheck();
}

HelpView::~HelpView()
{                 
	idCheck();

	Int n = xrefs->dim();

	for (Int i = 0; i < n; ++i)
		tr_delete(xrefs->elem(i));

	tr_delete(xrefs);		 
}

void HelpView::goToNext()
{                          
	idCheck();

	// Check if there are cross references for this helpview    
    Int xrdim = xrefs->dim();
	if (xrdim == 0)
		return;
		
	// Cycle to the first xref if were at the last one
 	current = (current == ERR) || (current > (xrdim - 2)) ? 0 : current + 1;
	
	goToXRef(current);
}   

void HelpView::goToPrev()
{
	idCheck();

	// Check if there are cross references for this helpview    
    Int xrdim = xrefs->dim();
	if (xrdim == 0)
		return;

	// Cycle to the last xref if were at the first one
	current = current < 1 ? xrdim - 1 : current - 1;

	goToXRef(current);
}

void HelpView::goToXRef(Int el)
{
	idCheck();
	
	goCursor(xrefs->elem(el)->srow, xrefs->elem(el)->scol);
}

String HelpView::getXRefKey()
{                             
	// Check if there are cross references for this helpview    
    Int xrdim = xrefs->dim();
	if (xrdim == 0)
		return NULL_STRING;

	// Check for xref at cursor position
	Int xrrow = _cursor->order();
	Int xrcol = _cursor->column();

	Int n = xrefs->dim();

	for (Int i = 0; i < n; ++i) {                 
		// Check if we are ON it !
		if (xrrow >= xrefs->elem(i)->srow && xrrow <= xrefs->elem(i)->erow && 
			xrcol >= xrefs->elem(i)->scol && xrcol <= xrefs->elem(i)->ecol)
			return xrefs->elem(i)->key;
            
		if (xrefs->elem(i)->srow > xrrow || 
			(xrrow == xrefs->elem(i)->srow && xrefs->elem(i)->ecol > xrcol)) 
		{
			goToXRef(current = i);                   
			return NULL_STRING;
		}
	}
	return NULL_STRING;
}
