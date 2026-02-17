/********************************************************************
* Copyright (c) 1991 InterSoft Co.  All Rights Reserved
* THIS IS UNPUBLISHED PROPRIETARY SOURCE CODE OF InterSoft Co.
* The copyright notice above does not evidence any
* actual or intended publication of such source code.
*
* $Id: waitmsg.cc,v 1.3 2006/02/17 17:38:22 spark Exp $
*
* DESCRIPTION: Implementation of waitmsg class
*
*********************************************************************/

#ifdef __GNUC__
#pragma implementation
#endif

#include <winapp/waitmsg.h>

WaitMsg::WaitMsg(const String &m, const String &l, UnChar c) 
:	WinObj(ERR, ERR),
 	message(m),
 	abortchar(c)
{  
	idCheck();

 	label	= l;
	border  = SLINE_BORDER;
	nattr   = A_NORMAL;
	ncol    = max(message.length(),label.length()) + 4;
	nfil	= 3;
	forig	= (Window::rootWindow()->height()-3)/2;
	corig	= (Window::rootWindow()->width()-ncol-8)/2;
}

WaitMsg::~WaitMsg() 
{
	idCheck(); 
}

Int WaitMsg::execute(UnChar)
{
	idCheck();

	if (wi->hidden()) {
		display();
		wi->setAttr(A_BLINK);
		*wi << "\n  " << message;
		Window::refresh();
	}

	return ACCEPT;
}
	
bool WaitMsg::isAborted()
{
	idCheck();

	assert(!wi->hidden());

    if (abortchar == (int)0)
    	return false;
    	
	if (wi->keyReady()) {
		UnChar c;
		*wi >> c;
		return c == abortchar;	
	}		

	return false;
}
