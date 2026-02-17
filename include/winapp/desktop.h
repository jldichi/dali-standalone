/********************************************************************
* Copyright (c) 1991 InterSoft Co.  All Rights Reserved
* THIS IS UNPUBLISHED PROPRIETARY SOURCE CODE OF InterSoft Co.
* The copyright notice above does not evidence any
* actual or intended publication of such source code.
*
* $Id: desktop.h,v 1.2 1998/11/19 15:46:45 ernestod Exp $
*
* DESCRIPTION: Class definitions for Desktop class
*********************************************************************/

#ifndef DESKTOP_H
#define DESKTOP_H

#ifdef __GNUC__
#pragma interface
#endif

#ifdef ISDEBUG
#	include <stdio.h>
#endif

#include <winapp/view.h>
#include <winapp/eview.h>


// Dimensiones de las views
#define FORIG		1
#define CORIG		1
#define FILS		(Window::rootWindow()->height() - 4)
#define COLS		(Window::rootWindow()->width() - 2)


class gtype(PtrList, View);
class gtype(PtrListCursor, View);

#define MAX_VIEWS 10
#define MAX_TILE 4

class Desktop {

	useIdCheck(Desktop_id);

private:

	gtype(PtrList,View)				*viewlist_;	// Viewlist head
	gtype(PtrListCursor, View)		*currview_; // Current view
	Int maxViews;								// Maximum views allowed
	Int maxTile;								// Maximum views to tile
	Int actViews;								// Number of active views

	// Makes sure there is room for a new View, disposing the 
	// least accessed view if necessary.
		Int makeRoom(View *v);
		void delView(View *v);

	// Finds a view and sets it as the last view in the list
		void setLastView(View *v);

public:
	// Constructors and Destructor
		Desktop(const Int mv = MAX_VIEWS, const Int mt = MAX_TILE);
		virtual ~Desktop();

	// Methods to add and delete views from the view list
		virtual void add(View *view, Int pfo = ERR, Int pco = ERR, 
							Int pfil = ERR, Int pcol = ERR);

		virtual void del(View *v);
		virtual void delCurrent();
		virtual bool clearDesktop();

	// Methods that run through the viewlist and may reorder it.
		virtual void prev();		// May be operator --
		virtual void next();		// May be operator ++
		virtual void select();

	// Mutators
		void setMaxViews(const Int &mv) { idCheck(); maxViews = mv; }
		void setMaxTile(const Int &mt) { idCheck(); maxTile = mt; }

	// Access to current view
		View *getCurrView();

	// Access to current editable view
		EditableView *getCurrEdView();

	// Finds and makes current a view
		void setAsCurrent(View *view);

	// Inspectors
		Int getActiveViews() const;
		Int getViews() const;
		UnChar getKey()
		{
			idCheck();
			assert(getCurrView() != NULL);
			return getCurrView()->getKey();
		}

	//
		void activate(View *v, Int forg, Int corg, Int fil, Int col, bool curr);
		void deActivate(View *v);

	//
		static Desktop *currentDesktop;
		static String const fpViewNames(Int i);

	// Methods that resize an move views
		virtual void tile() const;
		virtual void cascade() const;
};

#endif
