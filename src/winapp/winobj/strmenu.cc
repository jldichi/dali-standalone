/********************************************************************
* Copyright (c) 1991 InterSoft Co.  All Rights Reserved
* THIS IS UNPUBLISHED PROPRIETARY SOURCE CODE OF InterSoft Co.
* The copyright notice above does not evidence any
* actual or intended publication of such source code.
*
* $Id: strmenu.cc,v 1.1.1.1 1996/04/16 18:52:45 gustavof Exp $
*
* DESCRIPTION: Implementation of string menu class
*********************************************************************/

#include <ifound.h>
#include <winapp/menu.h>

StringMenu::StringMenu(const String &l, const String *s, Int n,
		Int fo, Int co, Int width)
:	ScrollMenu(l, NULL, 0, 0, fo, co)
{
	idCheck();

	Int maxWidth = Window::rootWindow()->width() - 4;

	dim = n;
	strArray = s;
	int i;
	for (i = 0; i < n && s[i] != NULL_STRING; i++) {
		Int len = s[i].length();
		if (len > ncol ) 
			if (maxWidth - 2 <len) ncol = maxWidth;
			else ncol = len;
	}

	if (width != ERROR)
		ncol = ncol > width ? width : ncol;


	ncol += 2;
	nfil = i < MAX_ITEMS ? i : MAX_ITEMS;
}

void StringMenu::fpRead(Int nro, String &str, String &secStr, Int &attr)
{
	idCheck();
	
	attr = A_NORMAL;

	if (nro >= 0 && nro < dim && strArray[nro] != NULL_VALUE) {
		secStr	= NULL_VALUE;
		str 	= strArray[nro];
	} 
	else {
		str = HIGH_VALUE;
		secStr = HIGH_VALUE;
	}
}

bool StringMenu::display(Int fil, Int col)
{
	idCheck();

	if (!(wi->hidden())) {
		wi->front();
		return true;
	}

	fillItems();
	return PoppableMenu::display(fil, col);
}


ExtStringMenu::ExtStringMenu(const String &menlabel, const String *hisarray, 
							Int hisSize, Int inforows_,
							Int fo, Int co, Int width)
:	StringMenu(menlabel, hisarray, hisSize, fo, co, width)
{
	idCheck();
//debería haber un parametro (gustavof)
    nfil = 5; // This is a test (bruno)
	allowMovedKeys();
	info_ = tr_new InfoPanel(NULL_STRING, inforows_, ncol, fo - inforows_ - 2,
						   co);
	dispinfo_ = false;
}

void ExtStringMenu::fpRead(Int nro, String &str, String &secStr, Int &attr)
{
	idCheck();

	attr = A_NORMAL;

	if (nro >= 0 && nro < dim && strArray[nro] != NULL_VALUE) {
		secStr	= NULL_VALUE;
		str 	= strArray[nro];
		str.prune();
		str = str(0, ncol - 2);
	} 
	else {
		str = HIGH_VALUE;
		secStr = HIGH_VALUE;
	}
}


bool ExtStringMenu::display(Int fil, Int col)
{
	idCheck();
    
	if (!(wi->hidden())) {
		wi->front();
		return true;
	}


	fillItems();

	return PoppableMenu::display(fil, col);
}

void ExtStringMenu::newCurrItem(Int n)
{                      
	idCheck();

	showInfo(n);
}

void ExtStringMenu::showInfo(Int n)
{   
	idCheck();
    assert(info_);

	if (dispinfo_) {    
    	info_->display();
	}
	else
		info_->unDisplay();

	if (n == ERROR)
		n = 0;
	info_->show(strArray[n](0, ncol - 2));
}

 


