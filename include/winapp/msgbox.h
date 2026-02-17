/********************************************************************
* Copyright (c) 1991 InterSoft Co.  All Rights Reserved
* THIS IS UNPUBLISHED PROPRIETARY SOURCE CODE OF InterSoft Co.
* The copyright notice above does not evidence any
* actual or intended publication of such source code.
*
* $Id: msgbox.h,v 1.1.1.1 1996/04/16 18:51:43 gustavof Exp $
*
* DESCRIPTION:	Class definitions for message box type.
*
*********************************************************************/

#ifndef MSGBOX_H
#define MSGBOX_H

#include <ifound.h>

#ifdef __GNUC__
#pragma interface
#endif

#include <winapp/panel.h>

class StrCell;
class Label;
class DefaultButton;

class MsgBox : public Panel
{
//private:
	MsgBox(const MsgBox &);
	
public:

	// objetos del panel
	DefaultButton  *ok_;

	// labels del panel
	Label    *msg;

	MsgBox(String label, String text, Int nf=ERR, Int nc=ERR, 
		   Int fo=ERR, Int co=ERR);
	virtual ~MsgBox();
	Int ok();
};

class CntrBox : public MsgBox
{
//private:
	CntrBox(const CntrBox &);

public:
	CntrBox(String l, String t) 
		   : MsgBox(l, t) 
		   {
				// Place panel centered	   
				ncol	= 40;
			    nfil	= 5;
			    forig	= (Window::rootWindow()->height() - nfil) / 2;
			    corig	= (Window::rootWindow()->width() - ncol)  / 2;
		   }
};
#endif
