/********************************************************************
* Copyright (c) 1991 InterSoft Co.  All Rights Reserved
* THIS IS UNPUBLISHED PROPRIETARY SOURCE CODE OF InterSoft Co.
* The copyright notice above does not evidence any
* actual or intended publication of such source code.
*
* $Id: panel.h,v 1.2 1998/05/04 16:27:31 mauro Exp $
*
* DESCRIPTION: Class definitions for panel type.
*
*********************************************************************/

#ifndef PANEL_H
#define PANEL_H

#include <ifound.h>

#ifdef __GNUC__
#pragma interface
#endif

#include <winapp/winobj.h>

// Please read commentary in the implementation 
// of the Panel::execute() method 

#define REDO_OBJECT 3

class Panel;
class Label;

class Panel : public WinObj
{
// private:

	WinObj **objects;
	Label  **labels;

	Int nobjs;
	Int nlbls;
	
	Panel(const Panel &);
	
protected:
    
    BaseAction *defaultAction;		// action to be executed if
    								// the panel is accepted, generally 
    								// associated to a default button.
    								
    Boolean accepted_;					// Status of panel on exit
	
	void setDefaultAction(BaseAction *a)	{ idCheck(); defaultAction = a; }
    
public:

	// constructores y destructores

	Panel(String l, Int nf=ERR, Int nc=ERR, Int fo=ERR, Int co=ERR);
	virtual ~Panel();

	// metodo para inicializar el panel

	void InitObjs  (WinObj **wo, Int i);
	void InitLabels(Label  **lb, Int i);

    virtual void 	front();
	virtual bool 	display(Int fil=ERR, Int col=ERR);
	virtual void 	unDisplay();
	virtual Int 	execute(UnChar c = 0);
	virtual bool 	isAccepted() const;
};

inline bool Panel::isAccepted() const 
{                                          
	idCheck();
	assert(!accepted_.isNull());
	return ((bool)accepted_ == true);
}

#endif
