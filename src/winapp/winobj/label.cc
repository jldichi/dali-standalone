/********************************************************************
* Copyright (c) 1991 InterSoft Co.  All Rights Reserved
* THIS IS UNPUBLISHED PROPRIETARY SOURCE CODE OF InterSoft Co.
* The copyright notice above does not evidence any
* actual or intended publication of such source code.
*
* $Id: label.cc,v 1.2 1997/11/18 15:49:47 guiller Exp $
*
* DESCRIPTION: Implementation of wlabel class
*
*********************************************************************/

#ifdef __GNUC__
#pragma implementation
#endif

#include <winapp/label.h>
#include <ifound/str.h>

Label::Label(const String &l, Int fo, Int co, Int width, Int a)
: WinObj(fo, co), forig(fo), corig(co), attr(a), lblwidth(width)
{   
	idCheck();

	setValue(l);	
	border  = NO_BORDER;
}

Label::~Label() 
{ 
	idCheck();
}

void Label::setValue(const String &newlabel)
{
	idCheck();

	Int llen = newlabel.length();
    
	if (lblwidth != ERROR && llen > lblwidth) {
		String aux = newlabel;
		nfil = wordWrap(aux, lblwidth) + 1;
		ncol = lblwidth;
		dato = aux;
	} 
	else { 
		nfil = 1; 
		ncol = (lblwidth==ERROR) ? llen : lblwidth;		
		dato = newlabel;   
	}

}

bool Label::display(Int fo, Int co) 
{ 
	idCheck();

	WinObj::display(fo, co);
	wi->wrap(false);
	draw();
	return true;
}

void Label::draw()
{               
	idCheck();

	if (!wi->hidden()) {
		wi->goTo(0, 0);

		if (attr != ERROR)
			wi->setAttr(attr);
        
	    *wi << dato;  
        wi->eraseEol();
	 
	    //if (lblwidth != ERROR)
	    //    *wi << String(ncol - dato.length() - 2,'_');
        
 		if (attr != ERROR)
			wi->setAttr(A_NORMAL);
	}
}

Int Label::wordWrap(String &line, Int width)
{
	idCheck();

	Int nLines = 1;
	String ll, aux, ww;
	StrCursor theWords(line, " ");
	
	aux = ll = "";	
	ww = theWords.value();
	while (theWords)
	{
		if (ww.length() > width) {
			ll += ww(0, width-1) + "\n";
			ww = ww(width);
			nLines++; 
			continue;
	    }

		while (aux.length() + ww.length() <= width) {
			aux += ww + " "; 
			++theWords;
			ww = theWords.value();		// get next word
		}
		ll += aux + "\n";
		aux = ""; nLines++;
	}
	
	ll.delChar(ll.length());
	line = ll;              
	
	return nLines;
}

