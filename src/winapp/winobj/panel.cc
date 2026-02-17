/********************************************************************
* Copyright (c) 1991 InterSoft Co.  All Rights Reserved
* THIS IS UNPUBLISHED PROPRIETARY SOURCE CODE OF InterSoft Co.
* The copyright notice above does not evidence any
* actual or intended publication of such source code.
*
* $Id: panel.cc,v 1.1.1.1 1996/04/16 18:52:45 gustavof Exp $
*
* DESCRIPTION: Implementation of panel class
*
*********************************************************************/

#ifdef __GNUC__
#pragma implementation
#endif

#include <winapp/panel.h>
#include <winapp/label.h>

// metodos publicos

Panel::Panel(String l, Int nf, Int nc, Int fo, Int co) 
:	WinObj(fo, co)
{
	idCheck();

	nlbls   		= nobjs   = 0;
	label   		= l;
	nfil    		= nf;
	ncol    		= nc;
	objects 		= NULL;
	labels  		= NULL;
    accepted_ 		= BOOL_NULL;
	defaultAction 	= NULL;
}

Panel::~Panel()
{
	idCheck();

	nobjs 	= 0;
	objects = NULL;
}

void Panel::InitObjs(WinObj **wo, Int i) 
{ 
	idCheck();

	objects = wo; 
	nobjs = i; 
}

void Panel::InitLabels(Label **lb, Int i) 
{ 
	idCheck();

	labels = lb; 
	nlbls  = i; 
}

void Panel::front()
{                         
	idCheck();

    wi->front();
	for (Int i = 0; i < nobjs; i++) 
		objects[i]->front();
}

bool Panel::display(Int fil, Int col)
{
	idCheck();

	if (!(wi->hidden())) {
		wi->front();
		return TRUE;
	}

	WinObj::display(fil, col);

	Window *prevDefPar = wi->setDefPar(wi);

	Int i;
    for (i = 0; i < nlbls; i++) 
		labels[i]->display();

	Window::refresh();

	for (i = 0; i < nobjs; i++) 
		objects[i]->display();

	(void)wi->setDefPar(prevDefPar);
		
	return TRUE;
}

void Panel::unDisplay()
{
	idCheck();

	Int i;
    for (i = 0; i < nlbls; i++) 
		labels[i]->unDisplay();
	for (i = 0; i < nobjs; i++)
		objects[i]->unDisplay();

	WinObj::unDisplay();
}

Int Panel::execute(UnChar c)
{        
	// This execute method handles a fifth return value, REDO_OBJECT.
	// Meaning that the last object executed should be executed again.
	// This value can only be set by a WinAction associated with the 
	// WinObj being processed.

	idCheck();

	// Check if the panel is active and if it has objects to execute !
	if( !active || nobjs==0 )
		return REJECT;         

    accepted_ = BOOL_NULL;

	bool undisplay = wi->hidden();

	display();

  	Int key = K_TAB, i = c;
	Int	res = ACCEPT;

	while ( res == ACCEPT || res == REJECT ) {
		// Execute object
		if (objects[i]->isActive()) {
			objects[i]->enter();
			while ((res = objects[i]->execute(c)) == REDO_OBJECT) 
				;
			objects[i]->leave();
				
			c = ' ';  // Blank hotkey for next time 
			key = objects[i]->lastKey();
		}
        
        // Terminate panel execution now ?
        if (res == REJECT_EXIT || res == ACCEPT_EXIT) 
        	break;
        
        // It is not REDO_OBJECT neither REJECT_EXIT/ACCEPT_EXIT, 
        // so it must be ACCEPT/REJECT !!
        assert ( res == ACCEPT || res == REJECT); 
        	
        // Decide where to go next, analizing lastkey pressed.
        // Note that if the current object could not be executed 
        // the previous value of 'key' is taken to keep direction.
        
		switch( key ) { 
			
			// Next object in the ring
			case K_CURS_UP:
			case K_CURS_LEFT:
				i = (--i + nobjs) % nobjs;
				break;

			// Previous object in the ring
			case K_CURS_DOWN:
			case K_CURS_RIGHT:
			case K_TAB:
			case ' ':
			case K_ENTER:
				i = ++i % nobjs;
				break;

			// Object selected via a hotkey 
			case K_CTRLX:
				{
					Window *win = objects[i]->window();
					*win >> c;
					if ( c == K_ENTER ) {
						if (defaultAction != NULL)
							res = defaultAction->execute();
						else	
							res = ACCEPT_EXIT;
						break;
					}
					
					Int j;	
					for (j = 0; j < nobjs &&
							tolower(c) != tolower(objects[j]->hotKey()); j++)
						;
					if (j < nobjs && objects[j]->isActive())
						i = j;
					else
						win->beep();
				}
				break;    
				
 			case K_END:
 				assert(0); // Somebody should have return a REJECTION status !

		 	default: {
		 		Int j;
				for (j = 0; j < nobjs &&
						tolower(key) != tolower(objects[j]->hotKey()); j++)
					;
				if (j < nobjs && objects[j]->isActive()) {
					i = j;
					c = key;
				}					
			}
		} 
	}
    
	if (undisplay) 
		unDisplay();
    
    // If panel status has not been set by an object, then determine it now.
    if (accepted_.isNull())
		accepted_ = (res == ACCEPT_EXIT);

	return (res == ACCEPT || res == ACCEPT_EXIT) ? ACCEPT : REJECT; 

	// toDo: Pensar devolver directamente --> return res; 
}
