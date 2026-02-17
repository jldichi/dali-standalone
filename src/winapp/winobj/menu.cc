/********************************************************************
* Copyright (c) 1991 InterSoft Co.  All Rights Reserved
* THIS IS UNPUBLISHED PROPRIETARY SOURCE CODE OF InterSoft Co.
* The copyright notice above does not evidence any
* actual or intended publication of such source code.
*
* $Id: menu.cc,v 1.1.1.1 1996/04/16 18:52:45 gustavof Exp $
*
* DESCRIPTION: Implementation of base menu class
*********************************************************************/

#ifdef __GNUC__
#pragma implementation
#endif

#include <winapp/menu.h>
#include <ifound/stdc.h>

#ifdef USES_OLD_HELP_MENU_STYLE
#	include <help.h>
#endif

// clase MenuItem

// metodos privados

MenuItem::MenuItem(const MenuItem &mi)
{
	idStart();

#if 0 // because of a gcc bug! (?)
	*this = mi;
#else
	text	= mi.text;
	stext	= mi.stext;
	attr	= mi.attr;
	nhkey	= mi.nhkey;
    lastkey	= mi.lastkey;
	t		= mi.t;    

	switch(t) {
	case MENU:
		menu = mi.menu;
	break;
	case RCOD:
		rcod = mi.rcod;
	break;
	case FUNC:
		func = mi.func;
	break;
	case LINE:
	break;
	default:
		assert(0);
	}
#endif
}          

MenuItem &MenuItem::operator=(const MenuItem &i)
{      
	idCheck();

	if (this != &i) {
		text = i.text;
		stext = i.stext;
		nhkey = i.nhkey;
		t = i.t;
		switch (t) {
		case MENU:
			menu = i.menu;
			break;
		case RCOD:
			rcod = i.rcod;
			break;
		case FUNC:
			func = i.func;
			break;
		case LINE:
			break;
		}
	}
	return *this;
}

void MenuItem::setHotKey(char hk)
{
	idCheck();

	if ((nhkey = text.index(hk)) == ERR)
		nhkey = 0;
	else {
		text.delChar(nhkey);
		if (text.length() == nhkey)
			nhkey = 0;
	}
}

// metodos publicos

MenuItem::MenuItem(const String &s, char hk, Int a)
{
	idStart();

	text = s;
	rcod = ERR;
	if (text == NULL_STRING)
		t = LINE;
	else
		t = RCOD;

	attr = a;

	if (hk != '\0')
		setHotKey(hk);
}

MenuItem::MenuItem(const String &s, Int cod, char hk, Int a)
{
	idStart();

	text = s;
	rcod = cod;
	if (text == NULL_STRING)
		t = LINE;
	else
		t = RCOD;

	attr = a;

	if (hk != '\0')
		setHotKey(hk);
}

MenuItem::MenuItem(const String &s, PoppableMenu &m, char hk, Int a)
{ 
	idStart();

	text = s;
	menu = &m;
	if (text == NULL_STRING)
		t = LINE;
	else
		t = MENU;

	attr = a;

	if (hk != '\0')
		setHotKey(hk);
}

MenuItem::MenuItem(const String &s, LFP f, char hk, Int a)
{ 
	idStart();

	text = s;
	func = f;
	if (text == NULL_STRING)
		t = LINE;
	else
		t = FUNC;

	attr = a;

	if (hk != '\0')
		setHotKey(hk);
}

Int MenuItem::execute()
{
	idCheck();

	switch (t) {
	case MENU:
		menu->execute();
		lastkey = menu->lastKey();
		return menu->currOpt();
	case RCOD:
		return rcod;
	case FUNC:
		return func ? func() : ERR;
	case LINE:
		break;
	}

	return ERR;
}

// fin clase MenuItem

// clase Menu

Menu::Menu(Int fo, Int co)
: WinObj(fo, co)
{
	idCheck();

	border 		= SLINE_BORDER;
	nattr  		= A_NORMAL;
	oattr  		= A_REVERSE;
	hkattr 		= A_BOLD;
	nitems 		= 0;
	curritem 	= previtem = ERR;
	opt    		= ERR;
	dspalways 	= FALSE;
	for (Int i = 0; i < MAX_ITEMS; i++)
		items[i] = NULL;

#ifdef USES_OLD_HELP_MENU_STYLE
	assocHelp = NULL;
#endif		
}

Menu::~Menu()
{
	idCheck();

	for (Int i = 0; i < nitems; i++)
		tr_delete(items[i]);
	nitems = 0;
}

void Menu::setCurrent(Int co, bool stay)
{
	idCheck();

	previtem = co; 
	stayOnOption = stay;
}

Menu &Menu::addItem(const MenuItem &i)
{
	idCheck();

	if (nitems < MAX_ITEMS) {
		MenuItem *mi = tr_new MenuItem(i);
		items[nitems++] = mi;
	}
	return *this;
}

void Menu::unDisplay()
{
	idCheck();

	WinObj::unDisplay();
	curritem = ERR;
}

void Menu::setSecText(const String &s, Int c)
{
	idCheck();

	for (Int i = 0 ; i < nitems; i++)
		if (items[i]->retCod() == c)
			items[i]->getSecText() = s;
}

void Menu::newCurrItem(Int i)
{
}

#ifdef USES_OLD_HELP_MENU_STYLE
void Menu::setHelp(Help *hlp)
{
	idCheck();

	assocHelp = hlp;
}
#endif

Menu &Menu::operator<<(const MenuItem &i)
{
	idCheck();

	return addItem(i);
}

Menu &Menu::operator<<(const String &s)
{
	idCheck();

	return addItem(s);
}

// fin clase Menu

// clase PoppableMenu

// metodos privados

void PoppableMenu::fillWindow(bool leaving)
{
	idCheck();

	Int columns = wi->width();

	wi->erase();
	wi->goTo(0, 0);

	for (Int i = 0; i < nitems; i++) {

		wi->goTo(i, 0);
		if (items[i]->isLine()) {
			if (i == nitems-1)
				wi->wrap(FALSE);

			wi->goTo(i, -1);
			*wi << C_LEFT;
			wi->goTo(i, columns+2);
			*wi << C_RIGHT;
			wi->goTo(i, 0);
			*wi << String(columns, C_HORIZ);

			if (i == nitems-1)
				wi->wrap(TRUE);
		}
		else {
			*wi << ' ';

			Int len = items[i]->getText().length();
			Int nhk = hilite ? items[i]->getNHKey() : ERR;
			Int lenstext = 0;

			if (items[i]->getSecText() != HIGH_VALUE)
				lenstext = items[i]->getSecText().length();
            
			Int topCol = columns - 2 - lenstext;

			if (i == curritem)
				wi->setAttr(oattr);
			else if (i == previtem && leaving)
				wi->setAttr(hkattr);
			else 
				wi->setAttr(items[i]->getDspAttr());

			// now the option
			for (Int j = 0; j < len && j < topCol; j++) {

				if ((!leaving || i == curritem ||
					 i != previtem) && j == nhk)
					wi->setAttr(hkattr);
                
				*wi << items[i]->getText()[j];

				if ((!leaving || i == curritem || i != previtem) && j == nhk)
					wi->setAttr(i == curritem ? oattr : items[i]->getDspAttr());
			}

			// blanks up to the end
			for (Int s = len + 1; s < topCol; s++)
				*wi << ' ';

			// if submenu print the special symbol indicating this
			if (items[i]->isMenu())
				*wi << SUBMENU;
			else if (len != topCol)
				*wi << ' ';

			// if there is secondary text, print it
			if (lenstext)
				*wi << items[i]->getSecText();

			if (i == curritem || (leaving && i == previtem))
				wi->setAttr(items[i]->getDspAttr());

			if (i != nitems - 1)
				*wi << ' ';
		}
	}
	wi->goTo(curritem, 1);
	wi->flush();

	// let "normal" as default attribute
	// ( needed by ScrollMenu::fillWindow() )
	wi->setAttr(A_NORMAL);
}

void PoppableMenu::calcDim(Int &nfils, Int &ncols)
{
	idCheck();

	bool haysmenu = FALSE;
	nfils = nitems;
	ncols = 0;

	for (Int i = 0 ; i < nitems; i++) {
		if (ncols < items[i]->getText().length()+
					items[i]->getSecText().length())
			ncols = items[i]->getText().length()+
					items[i]->getSecText().length();
		if (items[i]->isMenu()) 
			haysmenu = TRUE;
	}
	ncols += 2;
	if (haysmenu) ncols += 2;
}

bool PoppableMenu::incCurrItem(Int i)
{
	idCheck();

	curritem = (curritem + i) % nitems;

	// now handle we are not falling over a line
	while (items[curritem]->isLine())
		curritem = (curritem + 1) % nitems;

	newCurrItem(curritem);

	return TRUE;
}

bool PoppableMenu::decCurrItem(Int i)
{
	idCheck();

	curritem = (curritem - i + nitems) % nitems;

	// now handle we are not falling over a line
	while (items[curritem]->isLine())
		curritem = (curritem - 1) % nitems;

	newCurrItem(curritem);

	return TRUE;
}

// metodos publicos

PoppableMenu::PoppableMenu(const String &l, Int fo, Int co, bool hi)
:	Menu(fo, co),
	hilite(hi),
	leavehilit(FALSE)
{
	idCheck();

	label = l;
	pulldown = FALSE;
	scursor  = FALSE;
	lastkey = 0;
}

PoppableMenu::PoppableMenu(Int fo, Int co, bool hi)
:	Menu(fo, co),
	hilite(hi),
	leavehilit(FALSE)
{
	idCheck();

	label = "";
	pulldown = FALSE;
	scursor  = FALSE;
	lastkey = 0;
}

PoppableMenu::~PoppableMenu()
{
	idCheck();

	unDisplay();
}

bool PoppableMenu::display(Int fo, Int co)
{                    
	idCheck();

	if (! wi->hidden() ) {
		wi->front();
		fillWindow();
		return TRUE;
	}

	if (nitems == 0 && !dspalways)
		return FALSE;

	calcDim(nfil, ncol);
	Menu::display(fo, co); 
	fillWindow();
	Window::refresh();
	return TRUE;
}

Int PoppableMenu::execute(UnChar )
{
	idCheck();

    if (!active)
    	return REJECT_EXIT;
    	
	bool undisplay = wi->hidden();

	curritem = (previtem != ERR ? previtem : 0);

	if (!undisplay)
		fillWindow(leavehilit);
	else {
		if (!display())
			return REJECT_EXIT;
	}
	
	if (previtem == ERR)
		newCurrItem(curritem);

    opt = ERR;
    
	wi->cursor(FALSE);

	for (*wi >> lastkey; lastkey != K_END; *wi >> lastkey) {
		switch(lastkey) {
		case K_META:
			if (!incFind())
				break;

			if ((opt = items[curritem]->execute()) != ERR) 
				goto ret;
			break;
#ifdef USES_OLD_HELP_MENU_STYLE
		case K_HELP: 
			{
				Int retV = ERR;
				if (assocHelp && items[curritem]->isCod() &&
						(retV = items[curritem]->retCod()) != ERR)
					assocHelp->display(retV);
				else
					wi->beep();
			}
			break;

#endif
		case K_PAGE_DOWN:
			if (incCurrItem(nfil))
				fillWindow();
			else 
				wi->beep();
			break;
		case K_PAGE_UP:
			if (decCurrItem(nfil))
				fillWindow();
			else 
				wi->beep();
			break;
		case K_CURS_DOWN:
			if (incCurrItem())
				fillWindow();
			else 
				wi->beep();
			break;
		case K_CURS_UP:
			if (decCurrItem())
				fillWindow();
			else 
				wi->beep();
			break;
		case K_TAB:
		case K_CURS_RIGHT:
		case K_CURS_LEFT:
		case K_CTRLX:
			if (scursor || pulldown)
				 goto ret;
			wi->beep();
			break;
		default:
			{
			Int i; int LK = toUpperCase(lastkey);
			for (i = 0; i < nitems; i++) 
				if (LK == 
						toUpperCase(items[i]->
						getText()[items[i]->getNHKey()]))
					break;	
			if (i == nitems) {
				wi->beep();
				break;
			}
			curritem = i;
			fillWindow();
			// intentionally fall down
            }
		case ' ':
		case K_ENTER:           
#ifndef USES_OLD_HELP_MENU_STYLE		
		case K_HELP:
#endif		
			// if no items -> leave
			if (nitems == 0) {	
				opt = ERR;
				lastkey = K_CURS_RIGHT;
				goto ret;
			} 
			if ((opt = items[curritem]->execute()) != ERR) {
				goto ret;                                     
			}				
			break;
		}
	}

ret:
	wi->cursor(TRUE);

	if (undisplay)
		unDisplay();
	else {                       
		// Preserve the current item for next execution
		if (stayOnOption)
			previtem = curritem;

		// so that fillWindow does not hilite the current option
		curritem = ERR;
		fillWindow(leavehilit);
	}
	
	return (lastkey == K_END) ? REJECT_EXIT : ACCEPT;
}

// fin clase PoppableMenu


// clase PullDownMenu

// metodos privados

void PullDownMenu::fillWindow()
{
	idCheck();

	Int col = 0, rcol = 0;
	wi->erase();
	wi->goTo(0, 0);

	for (Int i = 0; i < nitems; i++) {

		*wi << ' '; rcol++;

		Int len = items[i]->getText().length();
		
		// justifico a derecha el ultimo item
		if (i == nitems-1 && right) {
			Int wth = wi->width()-1;	// no cuento la ultima col.
			for (Int n = rcol; n < wth-len; n++) {
				*wi << ' '; rcol++;
			}
		}

		if (i == curritem) {
			col = rcol;
			wi->setAttr(oattr);
		}

		Int nhk = items[i]->getNHKey();

		// ahora va la opcion
		for (Int j = 0; j < len; j++) {
			if (j == nhk) wi->setAttr(hkattr);
			*wi << items[i]->getText()[j];
			if (j == nhk) wi->setAttr(i==curritem ? oattr : nattr);
		}

		// me debo posicionar en col

		if (i == curritem) wi->setAttr(nattr);

		rcol += len;
	}
	wi->goTo(0, col);
	wi->flush();

}

// metodos publicos

PullDownMenu::PullDownMenu(Int fo, Int co, Int nf, Int nc) 
: Menu(fo, co)
{
	idCheck();

	right  = FALSE;
	border = NO_BORDER;
	nattr  = A_REVERSE;
	oattr  = A_NORMAL;
	nfil   = nf > MAX_ITEMS ? MAX_ITEMS : nf;
	ncol   = nc;
}

PullDownMenu::~PullDownMenu()
{
	idCheck();

	unDisplay();
}

Menu &PullDownMenu::addItem(const MenuItem &i)
{
	idCheck();

	if (nitems < MAX_ITEMS) {
		items[nitems] = tr_new MenuItem(i);
		if (items[nitems]->isMenu()) {
			assert(items[nitems]->subMenu());
			items[nitems]->subMenu()->setPullDown(TRUE);
		}
		nitems++;
	}
	return *this;
}

bool PullDownMenu::display(Int fo, Int co)
{
	idCheck();

	if (!(wi->hidden())) {
		wi->front();
		return TRUE;
	}

	if (nitems == 0 && !dspalways)
		return FALSE;

	Menu::display(fo, co);
	fillWindow();
	Window::refresh();
	return TRUE;
}

Int PullDownMenu::execute(UnChar entrykey)
{
	idCheck();

	if (!active)
		return REJECT_EXIT;
		
	opt = ERR;
	bool undisplay = wi->hidden();

	curritem = (previtem != ERR ? previtem : 0);

	if (!undisplay) 
		fillWindow();

	if (!display())
		return REJECT_EXIT;

	UnChar c;
	wi->cursor(FALSE);
	for (c = entrykey ? entrykey : wi->getKey(); c != K_END; c=wi->getKey()) {
		switch(c) {
#ifdef USES_OLD_HELP_MENU_STYLE
		case K_HELP: 
			{
				Int retV = ERR;

				if (assocHelp && items[curritem]->isCod() &&
						(retV = items[curritem]->retCod()) != ERR)
					assocHelp->display(retV);
				else
					wi->beep();
			}
			break;
#endif
		case K_CURS_RIGHT:
			curritem = ++curritem % nitems;
			fillWindow();
			break;
		case K_CURS_LEFT:
			curritem = (--curritem + nitems) % nitems;
			fillWindow();
			break;
		default:
			{
			Int i; int C = toUpperCase(c);
			for (i = 0; i < nitems; i++) 
				if (C == toUpperCase(items[i]->getText()[items[i]->getNHKey()]))
					break;	
			if (i == nitems) {
				wi->beep();
				break;
			}
			curritem = i;
			fillWindow();                 
			if (items[curritem]->isCod() && entrykey)
				break;
			// intentionally fall down
			}
		case K_CURS_DOWN:
		case K_ENTER:
#ifndef USES_OLD_HELP_MENU_STYLE
		case K_HELP:                                            
#endif		
			forever {
				opt = items[curritem]->execute();
				lastkey = items[curritem]->lastKey();
				if (opt != ERR) {
					goto ret;                                     
				}					
				PoppableMenu *m = items[curritem]->subMenu();
				if (m == NULL) break;
				if (m->lastKey() == K_END) {
					// Pretend user ended this menu also
					c = K_END;
					goto ret;
				}
				curritem = (m->lastKey() == K_CURS_RIGHT) ?
					++curritem % nitems : (--curritem+nitems) % nitems;
				fillWindow();
			}
			break;
		}
		entrykey = 0;
	}
ret:
	wi->cursor(TRUE);

	if (undisplay) 
		unDisplay();
	else {
		// Preserve the current item for next execution
		if (stayOnOption) 
			previtem = curritem; 
		
		// so that fillWindow does not hilite the current option
		curritem = ERR;
		fillWindow();
	}      
	
	return (c == K_END) ? REJECT_EXIT : ACCEPT;
}

// fin clase PullDownMenu
