/********************************************************************
* Copyright (c) 1991 InterSoft Co.  All Rights Reserved
* THIS IS UNPUBLISHED PROPRIETARY SOURCE CODE OF InterSoft Co.
* The copyright notice above does not evidence any
* actual or intended publication of such source code.
*
* $Id: dirsel.h,v 1.1.1.1 1996/04/16 18:51:43 gustavof Exp $
*
* DESCRIPTION : Class definitions for standard get file type.
*
*********************************************************************/

#ifndef DIRSEL_H
#define DIRSEL_H

#ifdef __GNUC__
#pragma interface
#endif

#include <ifound.h>

#ifdef __GNUC__
#pragma interface
#endif

#include <winapp/panel.h>

class StrCell;
class DirMenu;
class DefaultButton;
class PushButton;

// clase OpenFile

class DirSelector : public Panel {

// private:

	Int getDir();	// Function to obtain directory after menu execution
	String strres;
	DirSelector(const DirSelector &);
		
public:

	StrCell  		*name;
	DirMenu  		*dir;
	DefaultButton 	*ok_;
	PushButton 		*cancel_;

	DirSelector(String label, Int nf = ERR, Int nc = ERR, Int fo = ERR,
			  Int co = ERR, const String &cwd = NULL_STRING);

	virtual ~DirSelector();

	const String &value()	{ idCheck(); return strres; }

	Int ok();
	Int cancel();
};

#endif
