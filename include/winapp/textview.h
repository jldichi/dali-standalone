/********************************************************************
* Copyright (c) 1991 InterSoft Co.  All Rights Reserved
* THIS IS UNPUBLISHED PROPRIETARY SOURCE CODE OF InterSoft Co.
* The copyright notice above does not evidence any
* actual or intended publication of such source code.
*
* $Id: textview.h,v 1.1.1.1 1996/04/16 18:51:44 gustavof Exp $
*
* DESCRIPTION: Class definitions for TextView type.
*
*********************************************************************/

#ifndef TEXTVIEW_H
#define TEXTVIEW_H

#ifdef __GNUC__
#pragma interface
#endif

#include <ifound.h>

#include <winapp/winobj.h>
#include <ifound/str.h>

class TextView : public WinObj {
// private:

	// Feeding function
	SFPI fpRead;

	// Current line being displayed (at the bottom)
	Int line;

	// Current column being displayed (at the left)
	Int column;

	// Update scroll bars (not really scroll bars)
	void updateScrBars();

	// Display at text at specified line
	void displayLine(Int i, const String &st);

	TextView(const TextView &);
	
public :

	// Constructors and destructors
	TextView(SFPI fp, const String &lbl, Int fil = ERR,
			Int col = ERR);

	~TextView() { idCheck(); }

	// Execution
	Int  execute(UnChar c = 0);
	bool display(Int fo=ERR, Int co=ERR);
};

#endif
