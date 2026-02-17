/***********************************************************************
* Copyright (c) 1994 InterSoft Co.  All Rights Reserved
* THIS IS UNPUBLISHED PROPRIETARY SOURCE CODE OF InterSoft Co.
* The copyright notice above does not evidence any
* actual or intended publication of such source code.
*
* $Id: slide.cc,v 1.1.1.1 1996/04/16 18:52:45 gustavof Exp $
*
* DESCRIPTION: Implementation of Slide class
*********************************************************************/

#include <stdio.h>
#include <winapp/slide.h>
#include <ifound/action.h>

Slide::Slide(Int fo, Int co, const String &txt, Int l, 
			const String &format,
			const String &s_str,
			const String &e_str,
			BaseAction *a)
:	TextButton(fo, co, txt, l, a)
{
	idCheck();
	currval 	= 0;	
	startstr 	= s_str;
	endstr 		= e_str;
	nfil		= 2;
	ncol		= l + 20;  // l + expand().length() ?
	fmt			= format;
	granularity = 5;
}

void Slide::setGranularity(const int ng)
{
	idCheck();

	granularity = ng;
}

Int Slide::execute(UnChar c)
{
	idCheck();

	bool end = false;
	bool undisplay = wi->hidden();
	Int markerval = (Int)(corig + currval * len_/100.0);
	display();

	wi->goTo(0, 0);

	Int ret = REJECT;

	for (wi->goTo(0, markerval),
		wi->setAttr(A_REVERSE), 
		(*wi)(0, markerval) << C_BULLET;;) {

		wi->setAttr(A_REVERSE);
		*wi >> lastkey;

		if (lastkey == K_PAGE_DOWN) 
            currval -= granularity;

		if (lastkey == K_PAGE_UP) 
            currval += granularity;
            
		if (lastkey == K_CURS_LEFT || lastkey == K_CURS_DOWN) 
			--currval;
		
		if (lastkey == K_CURS_RIGHT || lastkey == K_CURS_UP) 
			++currval;
		
		if (currval <= 0)
			currval = 0;
		else if (currval >= 100)
				currval = 100;

		draw();
		switch (lastkey) {
			case K_END:
			case K_TAB:
			case K_CTRLX:
			case K_ENTER:
				end = true;
				break;
		}
		if (end) break;
	}

	ret = (action != NULL) 
			? action->execute()
			: (lastkey == K_END) ? REJECT_EXIT : ACCEPT;

	draw(false);

	if (undisplay) 
		unDisplay();

	return lastkey == K_END ? REJECT_EXIT : ret;

}

bool Slide::display(Int fo, Int co)
{
	idCheck();

	if (!(wi->hidden())) {
		wi->front();
		return true;
	}

	TextButton::display(fo, co);

	wi->wrap(false);
	wi->goTo(1, co + len_/2 - text_.length() / 2);
	drawText(A_NORMAL);
	drawLimits();
	draw(false);

	return true;
}


void Slide::drawLimits(void) 
{
	(*wi)(1, corig) << startstr;
	(*wi)(1, corig + len_- endstr.length()) << endstr;
}


void Slide::draw(bool act)
{
 	idCheck();

	if (wi->hidden()) return;
	wi->setAttr(A_NORMAL);	
	Int markerval = (Int)(corig + currval * len_/100.0);
	(*wi)(0, corig) << String(len_ + 1, C_HORIZ);
	if (act == true)	wi->setAttr(A_REVERSE);	

    (*wi)(0, markerval) << (act ? C_BULLET : 
    						(currval == 100 ? C_RIGHT : 
    						(currval == 0 ? C_LEFT: C_CEN)));

	wi->setAttr(A_NORMAL);

	(*wi)(0, corig + len_ + 2) << expand();

	Window::refresh();
}

Int Slide::getPercent(void)
{
	return currval;
}

Int Slide::getValue(void)
{
	
//	Int initval	=	(startstr == INITSTR ? 0  : toInt(startstr));
//	Int endval	=	(startstr == ENDSTR ? 100 : toInt(endstr));

	Int initval	=	(toInt(startstr) ? toInt(startstr) : 0);
	Int endval	=	(toInt(endstr) ? toInt(endstr) : 100);

	return (Int) ((((float)(endval - initval)) / 100) * currval + initval);
}


String Slide::expand(void)
{
	String aux;
	bool format = false;
	Int i = 0;
			
	while (fmt[i]) {
		switch (fmt[i]) {
		case '%':
			if (format) {
				aux += fmt[i];
				format = false;
			}
			else format = true;
			break;
		case 'v':
			if (format) {
				aux += "(";
				aux += toString(getValue(), endstr.length());
				aux += ")";
				format = false;
			}
			break;
		case 'p':
			if (format) {
				aux += "(";
				aux += toString(getPercent(), 3);
				aux += ")";
				format = false;
			}
			break;      
		case 'z':
			if (format) {             
				int vars = 5;                                      
				float d = 1.0 / vars;   
				float a = getPercent() / 100.0;
                
                int var;
				for (var = 0; var < vars; ++i)
					if (a < d*(var+1))
						break;

				aux += "(";
				aux += startstr.field(var,",");
				aux += ")";
				format = false;
			}
			break;
		default:
			format = false;
			aux += fmt[i];
			break;		
		}
		i++;
	}
	return aux;
}

bool CompactSlide::display(Int fo, Int co)
{
	idCheck();

	if (!(wi->hidden())) {
		wi->front();
		return true;
	}

	TextButton::display(fo, co);

	wi->wrap(false);
	wi->goTo(0, co + len_ + expand().length() + 8); // Why a wired 8 ?
	drawText(A_NORMAL);
	draw(false);

	return true;
}
