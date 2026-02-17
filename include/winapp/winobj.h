/********************************************************************
* Copyright (c) 1991 InterSoft Co.  All Rights Reserved
* THIS IS UNPUBLISHED PROPRIETARY SOURCE CODE OF InterSoft Co.
* The copyright notice above does not evidence any
* actual or intended publication of such source code.
*
* $Id: winobj.h,v 1.1.1.1 1996/04/16 18:51:44 gustavof Exp $
*
* DESCRIPTION: Class definitions for window object type.
*
*********************************************************************/

#ifndef WINOBJ_H
#define WINOBJ_H

#ifdef __GNUC__
#pragma interface
#endif

#include <ifound.h>

#include <ifound/str.h>
#include <ifound/action.h>
#include <winapp/window.h>

// char to indicate next char is the hot key
#define HKSEQ		'~'

// typedef for needed functions
typedef const String (*SFPI)(Int);

#define REJECT           -2
#define REJECT_EXIT      -1
#define ACCEPT            0
#define ACCEPT_EXIT       1

class WinObj {

	useIdCheck(WinObj_id);

protected:

	Window *wi;				// window over which we work

	Int ncol;				// number of columns
	Int nfil;				// number of rows
	Int forig;				// row origin
	Int corig;				// column origin

	String label;			// window's label

	Int border;				// border attribute
	Int nattr;				// content attribute

    BaseAction *action;		// callback action
    Int hotKey_;			// offset of hot key within text

    bool active;			// Tells if the object should be executed
	UnChar lastkey;			// Key pressed to exit the window object

    WinObj(const WinObj &);
    void operator=(const WinObj &) { }

public:

	// Constructors & destructors
	WinObj(Int fo = ERR, Int co = ERR);
	virtual ~WinObj();

	Window *window()						{ idCheck(); return wi;  }

	// Mutators
    void setLabel (String &l)				{ idCheck(); label  = l; }
	void setBorder(Int a)					{ idCheck(); border = a; }
	void setNAttr (Int a)					{ idCheck(); nattr  = a; }
	void setAction(BaseAction *a)			{ idCheck(); action = a; }
    void setActive(bool a = true) 			{ idCheck(); active = a; }

	// Inspectors
    bool isActive() const					{ idCheck(); return active;}
	Int  lastKey() const					{ idCheck(); return lastkey; }

	// Virtual methods
    virtual void front();
	virtual void unDisplay();
	virtual bool display(Int fo=ERR, Int co=ERR);

	// Attention, ALL execute() methods in derived classes must return
	// values with the following convention : 
	//                                        
	//	ACCEPT_EXIT  	:	The user wants to accept the changes involved 
	//						with the window object and exit "correctly" 
	//						any parent window object.(For example a panel)
	//
	//	ACCEPT         	:	The user accepts the changes but wants to stay 
	//						processing the parent window object.
	//
	//	REJECT_EXIT		:	The user rejects the changes and wants to exit  
	//						the parent window object. 
	//
	//	REJECT         	:	The user rejects the changes but wants to stay  
	//						processing the parent window object. 
	//        
	
	virtual Int execute(UnChar c = 0) = 0;
    
    // Virtual inspectors
	virtual bool isDefault() const;
	virtual UnChar hotKey() const;

	virtual void enter();
	virtual void leave();
};

#endif
