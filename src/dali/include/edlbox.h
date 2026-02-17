/********************************************************************
* Copyright (c) 1991 InterSoft Co.  All Rights Reserved
* THIS IS UNPUBLISHED PROPRIETARY SOURCE CODE OF InterSoft Co.
* The copyright notice above does not evidence any
* actual or intended publication of such source code.
*
* $Id: edlbox.h,v 1.1.1.1 1996/04/16 18:52:53 gustavof Exp $
*
* DESCRIPTION: Definitions for Editor Line Dialog
*********************************************************************/

#ifndef EDLINEBOX_H
#define EDLINEBOX_H

#ifdef __GNUC__
#pragma interface
#endif

#include <winapp/cell.h>
#include <winapp/panel.h>

class String;
class StrHistory;
class Label;

class EdLineDialog : public Panel
{
private:

	String		*cmd;
	StrHistory	*strHis;
	
	EdLineDialog(const EdLineDialog&) ;
	void operator=(const EdLineDialog&) ;
	
public:

	// objetos del panel
	StrCell		*cmdFld;
	Label		*staticText;

	EdLineDialog(StrHistory *his, String title, 
    	String stText, Int rows, Int cols, Int srow, Int scol);
	virtual ~EdLineDialog();
	const String &value()	{ return *cmd; }
	void setValue(const String &val)	{ cmdFld->setValue(val); }
	Int ok();
	virtual Int execute(UnChar c = 0);
};

#endif

