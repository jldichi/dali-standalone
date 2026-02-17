/********************************************************************
* Copyright (c) 1991 InterSoft Co.  All Rights Reserved
* THIS IS UNPUBLISHED PROPRIETARY SOURCE CODE OF InterSoft Co.
* The copyright notice above does not evidence any
* actual or intended publication of such source code.
*
* $Id: debbox.h,v 1.1.1.1 1996/04/16 18:52:53 gustavof Exp $
*
* DESCRIPTION: Headers for debugger's panel
*
*********************************************************************/

#include <winapp/panel.h>
#include <ifound/str.h>

#ifdef __GNUC__
#pragma interface
#endif

class StrCell;
class FStrCell;
class Label;
class DefaultButton;
class PushButton;
class CheckBox;

class PDebugBox : public Panel {
// private:

	String 	fName;
	String 	stty;
	bool 	wmClient;
	
	PDebugBox(const PDebugBox&) ;
	void operator=(const PDebugBox&) ;
		
public:

	// winobjs del panel
	StrCell  		*file;
	CheckBox		*wmclient;
	StrCell  		*tty;
	DefaultButton	*ok_;
	PushButton		*cancel_;

	// labels del panel
	Label    *tfile;
	Label    *ttty;

	PDebugBox(String label, Int nf=ERROR, Int nc=ERROR, 
				Int fo=ERROR, Int co=ERROR);
	virtual ~PDebugBox();

	bool  isWmClient()				{ return wmClient; }
	Int   ttyStatus();
	Int   ok();
 	Int   cancel(); 
	const String &value() const		{ return fName; }
	const String &getTTY() const	{ return stty; }
};

