/********************************************************************
* Copyright (c) 1991 InterSoft Co.  All Rights Reserved
* THIS IS UNPUBLISHED PROPRIETARY SOURCE CODE OF InterSoft Co.
* The copyright notice above does not evidence any
* actual or intended publication of such source code.
*
* $Id: dbox.h,v 1.1.1.1 1996/04/16 18:51:43 gustavof Exp $
*
* DESCRIPTION: Class definitions for dialog box type.
*
*********************************************************************/

#ifndef DBOX_H
#define DBOX_H

#ifdef __GNUC__
#pragma interface
#endif

#include <ifound.h>

#ifdef __GNUC__
#pragma interface
#endif

#include <winapp/panel.h>

class PullDownMenu;
class Label;
class TextButton;

// valores retornados por el execute
#define	DBOX_YES	0
#define DBOX_NO		1
#define	DBOX_CANCEL	2

class DBox : public Panel
	// Dialog box
{
// private:                      
	Int option;
    DBox(const DBox &);
    void operator=(const DBox &);

public:

	// panel default
	TextButton *yes_;
	TextButton *no_;
	TextButton *cancel_;

	// panel labels
	Label    *msg;

	DBox(String label, String text, Int def = DBOX_YES, Int nf = ERR, 
					   Int nc = ERR, Int fo = ERR, Int co = ERR);
	virtual ~DBox();

	virtual Int execute(UnChar c = 0);
	Int getOpt() const { idCheck(); return isAccepted() ? option : DBOX_CANCEL; }	
	Int yes();
	Int no();
	Int cancel();
};

#endif
