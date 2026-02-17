/********************************************************************
* Copyright (c) 1991 InterSoft Co.  All Rights Reserved
* THIS IS UNPUBLISHED PROPRIETARY SOURCE CODE OF InterSoft Co.
* The copyright notice above does not evidence any
* actual or intended publication of such source code.
*
* $Id: winobj.cc,v 1.1.1.1 1996/04/16 18:52:46 gustavof Exp $
*
* DESCRIPTION
* Implementation of window object class
*********************************************************************/

#ifdef __GNUC__
#pragma implementation
#endif

#include <winapp/winobj.h>

WinObj::WinObj(Int fo, Int co)
{
	idStart();
	
	wi 		= tr_new Window;
	ncol 	= nfil = ERR;
	label 	= "";
	border 	= SLINE_BORDER;
	nattr 	= A_NORMAL;
	forig 	= fo;
	corig 	= co;
	action 	= NULL;
	hotKey_ = ERR;
	active	= true;
}

WinObj::~WinObj() 
{ 
	idCheck();

	tr_delete(wi);
	wi = NULL;

	idStop();
}

void WinObj::unDisplay()
{
	idCheck();

	wi->unDisplay();
}

void WinObj::front()
{   
	idCheck();

    assert( ! wi->hidden() );     
	wi->front();
}

bool WinObj::display(Int fo, Int co)
{
	idCheck();

	wi->display(fo!=ERR ? fo : forig != ERR ? forig : 
								 Window::rootWindow()->line()+2,
				co!=ERR ? co : corig != ERR ? corig :
								 Window::rootWindow()->column()+1,
				nfil, ncol, border, label, nattr);
	ncol = wi->width();
	nfil = wi->height();
	return true;
}

UnChar WinObj::hotKey() const
{
	idCheck();

	return hotKey_;
}

bool WinObj::isDefault() const
{
	idCheck();

	return false;
}

void WinObj::enter()
{
}

void WinObj::leave()
{
}
