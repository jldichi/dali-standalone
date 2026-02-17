/********************************************************************
* Copyright	(c)	1994 InterSoft Co.	All	Rights Reserved
* THIS IS UNPUBLISHED PROPRIETARY SOURCE CODE OF InterSoft Co.
* The copyright	notice above does not evidence any
* actual or	intended publication of	such source	code.
*
* $Id: desktop.cc,v 1.3 2000/01/31 18:25:30 diegon Exp $
*
* DESCRIPTION: Implementation of Desktop class
*********************************************************************/

#ifdef __GNUG__
#	pragma implementation
#endif

#include <ifound.h>

#ifdef ISDEBUG
#	include <ifound/iddebug.h>
#	include <stdio.h>
#endif

#include <winapp/desktop.h>
#include <winapp/menu.h>
#include <winapp/msgbox.h>

declare(PtrList, View);
implement(PtrList, View);

declare(PtrListCursor, View);
implement(PtrListCursor, View);

#define currview (*currview_)
#define viewlist (*viewlist_)

Desktop::Desktop(const Int mv, const Int wt)
{ 
	idStart();  
	
	maxViews = mv; 
	maxTile = wt;
	actViews = 0;
		
	//	inicializacion de la lista de views
	viewlist_  = tr_new PtrList(View);
	
	//	inicializacion del cursor de la	list de	views
	currview_ = tr_new PtrListCursor(View)(viewlist);
	currview.goToLast();
}

Desktop::~Desktop()
{                       
	idCheck();

	// Borro todas las listas
	for (currview = viewlist; currview; ++currview) {
		delView(*currview);
	}

	// View list cursor and list itself
	tr_delete(currview_);
	tr_delete(viewlist_);

	idStop();
}

View *Desktop::getCurrView() 
{ 
	idCheck(); 
	assert(*currview);
	return *currview; 
}   


EditableView *Desktop::getCurrEdView() 
{                     
	idCheck(); 
	assert(*currview); 

// It's OK to return NULL, see wincmd.C:109 (PosCmd::_redo())
//	assert(currview->edView() != NULL); 

	return currview->edView(); 
}   

void Desktop::activate(View *v, Int forg, Int corg, Int fil, 
					   Int col, bool curr)
{
	assert(v != NULL);
	v->activate(forg, corg, fil, col, curr);
	actViews++;
	assert(actViews <= maxViews);
}

void Desktop::deActivate(View *v)
{
	assert(v != NULL);
	v->deActivate();
	actViews--;	  		// Shouldn't be call before deleting the view
    assert (actViews >= 0);
}

Int Desktop::getActiveViews() const
{ 
	idCheck(); 

	return actViews; 
}

Int Desktop::getViews() const
{ 
	idCheck(); 

	return viewlist.size(); 
}

void Desktop::setAsCurrent(View *view)
{
	idCheck();

	View *saveView = *currview;

	setLastView(view);

	// Make sure the view is activated
	if (view->isActive()) {
		saveView->setCurrent(false);
		view->setCurrent(true);
	} 
	else {
		deActivate(saveView);
		activate(view, saveView->orgRow()	 +	1, saveView->orgCol(),
						saveView->orgHeight() -	1, saveView->orgWidth(), true);
	}

	currview->redraw();
	// currview->refresh(); // Bruno: toDo
}

void Desktop::setLastView(View *v)
	// Reorders Viewlist, putting 'v' as the last view
{
	idCheck();

	// Find 'v' in the viewlist
	PtrListCursor(View) vlc(viewlist);
	for (; vlc && *vlc != v; ++vlc)
		;

	// Take 'v' and append it in the list
	currview.goTo(vlc.order());
	currview.del();
	viewlist.add(v);

	currview.goToLast();
}

Int	Desktop::makeRoom(View *v)
	// Makes sure that there is room for a tr_new View
{
	idCheck();

	if (actViews == 0) {
		currview.add(v);
		return 0;
	}

	View *p = *currview;

	if (actViews == maxViews) {
		// Try to deactivate the oldest active view in the list
		PtrListCursor(View) vlc(viewlist);

		for (vlc.goToFirst(); vlc && !vlc->isActive(); ++vlc)
			;

		if (!(vlc->exit())) {
			delView(v);
			return ERR;
		}

		p->setCurrent(false);	// Change state of current view;
		deActivate(*vlc);
		viewlist.add(v);
	}
	else {
		p->setCurrent(false);	// Change state of current view;
		viewlist.add(v);
	}

	// Point to the recently added view
	currview.goToLast();
	return actViews;
}

void Desktop::add(View *view, Int pfo, Int pco, Int pfil, Int pcol)
{
	idCheck();

	View *pv = *currview;

	// Is there room for a tr_new View?	
	if (makeRoom(view) == ERR)
		return;

	if (pv == NULL) 
		activate(*currview, pfo == ERR ? FORIG : pfo, 
							pco == ERR ? CORIG : pco, 
							pfil== ERR ? FILS  : pfil, 
							pcol== ERR ? COLS  : pcol, 
							true);
    else
		activate(*currview, pfo == ERR ? pv->orgRow()+1			: pfo, 
							pco == ERR ? pv->orgCol()				: pco, 
							pfil== ERR ? FILS - (pv->orgRow()+2)	: pfil, 
							pcol== ERR ? pv->getWidth()			: pcol, 
							true);

	return;
}

void Desktop::delView(View *v)
{             
	idCheck();

	View::deleteView(*currview);
	actViews--;
	assert (actViews >= 0);
}   

void Desktop::del(View *v)
	// This method deletes a view "politely"
{
	idCheck();

	if (v == *currview) {
		delCurrent();
		return;
	}

	if (v->exit()) {
		PtrListCursor(View) vlc(viewlist);
		for ( ; vlc; ++vlc)
			if (vlc.value() == v) break;

		assert(vlc);		// We must have	found this view

		// No more needed, done in delView !			
				/* Keep the number of active views updated
					if (v->isActive())
						deActivate(v); 
				*/
				
		delView(v);

		vlc.del();
	}
}

void Desktop::delCurrent()
	// Deletes current view, setting another view as current if possible.
{
	idCheck();
	if (*currview && currview->exit()) {
		delView(*currview);
		currview.del();
		currview.goToLast();

		if (*currview) {
			// If view is inactive, activate it first.
			if (!currview->isActive())
				activate(*currview, FORIG, CORIG, FILS, COLS, true);
			currview->setCurrent(true);
		} 
	}
}

bool Desktop::clearDesktop()
{
	bool canExit = true; // Can exit all views ?
	
	for (PtrListCursor(View) vlc(viewlist); vlc && canExit; ++vlc)
		canExit = vlc.value()->exit(false);

	if(! canExit) {
		MsgBox cantPerform(libMsg("ERR"), "CANTPERFORM", 7,40,6,20);
		cantPerform.execute();
		return false;  
	}		

	for (currview = viewlist; currview; ++currview)
		delView(*currview);

	viewlist_->delAll();

	return	true;
}

void Desktop::prev()
{
	idCheck();

	assert(currview);
	
	if (getActiveViews() <	2)
		return;

	View *curr = *currview;
	PtrListCursor(View) vlc = currview;

	currview->setCurrent(false);
	--currview;

	currview->setCurrent(true);

	vlc.del();
	for	(vlc.goToFirst(); vlc && !vlc.value()->isActive(); ++vlc)
			;

	vlc.ins(curr);
}

void Desktop::next()
{
	idCheck();

	if (getActiveViews() <	2)
		return;

	PtrListCursor(View) vlc = currview;

	currview->setCurrent(false);

	for	(vlc.goToFirst(); vlc && ! vlc.value()->isActive();	++vlc)
		;

	assert(vlc);

	setLastView(vlc.value());
	currview->setCurrent(true);
}

#define MAX_VIEW_NAME_LEN  60
Desktop *Desktop::currentDesktop = NULL;

String const Desktop::fpViewNames(Int i)
{
	assert(currentDesktop != NULL);

	Int cant = currentDesktop->viewlist_->size();

	if	(i < 0 || i	>= cant)
		return HIGH_VALUE;

	PtrListCursor(View) vlc(*(currentDesktop->viewlist_));

	vlc.goTo(cant - i - 1);

	String tmp = vlc.value()->getLabel(true);

	if	(vlc.value()->isActive())
		tmp = String(1, C_BULLET)+String(1, ' ')+tmp;
	else
		tmp = String("  ")+tmp;

	tmp = tmp(0, MAX_VIEW_NAME_LEN);

	return tmp;
}


void Desktop::select()
{
	idCheck();

	Int res = 0, maxSize = viewlist.size();
	String label = toString("Select (") + toString(maxSize) +  ")";
	Int ncol = label.length() + 1;
	Int cant;

	PtrListCursor(View) vlc(viewlist);
	for (; vlc; ++vlc, res++)
		if (vlc.value()->getLabel(true).length() > ncol)
			ncol = vlc.value()->getLabel(true).length();

	// We know it is not very neat ...
	currentDesktop = this;

	ScrollMenu msv(label, fpViewNames, res > 10 ? 10 : res, ncol+4, 7, 25);
	msv.execute();

	if ((res = msv.currOpt()) != ERR) {
		// Adjust 'res' because views are shown reversed
		 res = maxSize - res - 1;

		// No current view
		if ((cant = getActiveViews()) == 0) {
			currview.goTo(res);
			activate(*currview, FORIG, CORIG, FILS, COLS, true);
			setLastView(*currview);
		} 
		else {
			if (currview.order() != res) {

				vlc	= currview;
				vlc.goTo(res);

				View *vprev	= *currview;	// This will be the previous view
				View *vlast	= *vlc;			// This will be the last view

				if (vlast->isActive()) {
					vprev->setCurrent(false);
					setLastView(vlast);
					vlast->setCurrent(true);
				}
				else {
					Int f, fo, c, co;

					// Must deactivate a view first
					if (cant == maxViews) {
						// Try to deactivate the first active view in the list
						for	( 
								currview.goToFirst();
								currview && !currview->isActive();
								currview++ 
							);

						if (!currview->exit())	
							return;

						// Deactivate it!
						deActivate(*currview);
						f  = vlast->orgHeight();
						fo = vlast->orgRow();
						c  = vlast->orgWidth();
						co = vlast->orgCol();
					}
					else {
						f  = vprev->orgHeight() - 1;
						fo = vprev->orgRow() + 1;
						c  = vprev->orgWidth();
						co = vprev->orgCol();
					}

					vprev->setCurrent(false);
					setLastView(vlast);
					activate(vlast, fo, co, f, c, true);
				}
			}
		}
	}
	// It must be the last view in the list
	currview.goToLast();
}

void Desktop::tile() const
	// Implements the tile (or mosaic) feature.
{
	idCheck();

	Int nav = getActiveViews();

	if (maxTile < nav)
		nav = maxTile;

	Int resto = (nav == 2)	? 0	: nav %	2;
	Int slots = (nav == 2)	? 2	: (maxTile	< nav ?	maxTile : nav ) / 2;
	Int fils  = (FILS + 2)	/ (slots + resto);
	Int cols  = (COLS + 2)	/ 2;
	Int rfils = (FILS + 2)	% (slots + resto);
	Int rcols = (COLS + 2)	% 2;

	PtrListCursor(View) vlc(viewlist);

	//	caso especial para optimizar el	scroll
	if	(nav ==	2) {
		vlc.goToLast();
		for (Int n = 0; vlc && n < 2;	--vlc)
		if (vlc.value()->isActive()) {
			vlc.value()->chgOrgSize(FORIG + (n	* fils), CORIG,
									n == 1 ? fils+rfils-2 : fils-2, COLS);
			n++;
		}
	}
	else {
		vlc.goToLast();

		// Ya que el numero	de ventanas	a ubicar es	impar,
		// dejo	la corriente para lo ultimo.
		if (nav	& 1)  
			--vlc;
        
        Int i;
		for (i = 0; i < slots; i++)
			for	(Int n = 0;	vlc	&& n < 2; --vlc)
				if (vlc.value()->isActive()) {
					vlc.value()->chgOrgSize(FORIG + (i *	fils),
											CORIG + (n *	cols),
											!resto && i == slots-1 
												? fils+rfils-2 : fils-2,
											!resto && i == slots-1 
												? cols+rcols-2 : cols-2
										   );
					n++;
				}

		if (resto)
			currview->chgOrgSize(FORIG + (i * fils), CORIG, 
							fils + rfils - 2, COLS);
	}
}

void Desktop::cascade() const
	// Implements the cascade feature
{                  
	idCheck();

	PtrListCursor(View) vlc(viewlist);

	//	busco la ultima	view activa
	for (vlc.goToLast(); vlc && !vlc.value()->isActive(); --vlc)
		;

	assert(vlc);
    
    View::setFullRedraw(false);
    
	Int i 	= 0;
	for (vlc.goToFirst(); vlc ; ++vlc)	{
		View *v =	vlc.value();
		if (v->isActive()) {
			vlc.value()->chgOrgSize(FORIG+i, CORIG,	FILS - i, COLS);
			i++;
		}
	}     
	        
	View::setFullRedraw();
}


