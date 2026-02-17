/********************************************************************
* Copyright (c) 1991 InterSoft Co.  All Rights Reserved
* THIS IS UNPUBLISHED PROPRIETARY SOURCE CODE OF InterSoft Co.
* The copyright notice above does not evidence any
* actual or intended publication of such source code.
*
* $Id: cell.cc,v 1.2 1997/11/18 15:38:37 guiller Exp $
*
* DESCRIPTION: Implementation of cell class
*
*********************************************************************/

#ifdef __GNUC__
#pragma implementation
#endif

#include <winapp/cell.h>
#include <ifound/stdc.h>
#include <winapp/strhis.h>
#include <winapp/window.h>

Cell::Cell(Int o, Int l, Int ol, Int nd, String tm, String om, 
		   Int fo, Int co, StrHistory *hs)
: WinObj(fo, co)
{           
	idCheck();

	// seteo de atributos de WinObj
	border  = NO_BORDER;
	nfil    = 1;
	ncol    = ol;

	tstmask = tm;
	omask   = om;
	opt     = o;
	length  = l;
	olength = ol;
	ndec    = nd;
	
	history = hs;
}

Cell::~Cell() 
{ 
	idCheck();
}

bool Cell::display(Int fo, Int co) 
{ 
	idCheck();

	WinObj::display(fo, co);
	wi->wrap(false);
	return true;
}

void Cell::writeStr()
{
	idCheck();

	if (wi->hidden()) return;

	wi->goTo(0, 0);
	
	// despliego el dato
	if (omask) {
		// this should be done only once
		String mask = String("\1") + omask + "\1" + tstmask;
		String formatted = datum.format(mask);
		String aux(length-formatted.length(), ' ');
		*wi << formatted << aux;
	} else  {
		String aux(length-datum.length(), ' ');
		*wi << datum << aux;
	}    

    wi->eraseEol();
	wi->goTo(0, 0);

	Window::refresh();
}

StrCell::StrCell(Int l, Int ol, Int o, Int fo, Int co, StrHistory *his)
: Cell(o, l, ol, 0, NULL_STRING, NULL_STRING, fo, co, his) { }

StrCell::~StrCell() 
{ 
	idCheck();

	unDisplay();
}

bool StrCell::display(Int fo, Int co)
{
	idCheck();

	if (!(wi->hidden())) {
		wi->front();
		return true;
	}

	(void)Cell::display(fo, co); 

	writeStr(); 
	   
	return true;
}

Int StrCell::execute(UnChar )
{
	idCheck();

    if (!active)
    	return REJECT;

	bool undisplay = wi->hidden();
	
	(void)display();
    	
	while ((lastkey = wi->getField(	TY_STRING, datum, opt, length, olength, 
									ndec, tstmask, omask)) == K_HELP)
	{
	 	if (history) history->execute(datum);
	}

	wi->goTo(0, 0);

	// redespliego el dato para apagar el reverse
	if (omask)  {
		// this should be done only once
		String mask = String("\1") + omask + "\1" + tstmask;
		String formatted = datum.format(mask);
		String aux(length-formatted.length(), ' ');
		*wi << formatted << aux;
	} else  {
		String aux(length-datum.length(), ' ');
		*wi << datum << aux;
	}

	wi->goTo(0, 0);

	wi->flush();

	Int ret = (action != NULL) 
				? action->execute()
				: (lastkey == K_END) ? REJECT_EXIT : ACCEPT;

	if (undisplay) 
		unDisplay();

	return ret;
}

FStrCell::FStrCell(String mask, Int o, Int fo, Int co)
: StrCell(0, 0, o, fo, co)
{ 
	idCheck();

	assert(mask[0] != '\1');
	mask.compileMask();

	omask   = mask.field(1, "\1");
	tstmask = mask.field(2, "\1");

	length = olength = ncol = omask.length() - 1;
}

FStrCell::~FStrCell() 
{ 
	idCheck();
}
