/********************************************************************
* Copyright (c) 1991 InterSoft Co.  All Rights Reserved
* THIS IS UNPUBLISHED PROPRIETARY SOURCE CODE OF InterSoft Co.
* The copyright notice above does not evidence any
* actual or intended publication of such source code.
*
* $Id: wicod.cc,v 1.1.1.1 1996/04/16 18:52:45 gustavof Exp $
*
* DESCRIPTION
* Implementation of window manager class
*********************************************************************/

#include <ifound.h>
#include <ifound/priv/gfuncs.h>
#include <winapp/winmgr.h>
#include <ifound/stdc.h>

#ifndef HAVE_IPC
extern union _WmPar {
	short		i;
	long		l;
	double		f;
	UnChar		*s;
	UnChar		u;
} *wm_par;

#endif


void WinMgr::push(short i)
{
#ifndef HAVE_IPC
	wm_par[paramNum++].i = i;
#else
	stint(i, index);
	index += sizeof(short);
#endif
}

void WinMgr::push(UnChar c)
{
#ifndef HAVE_IPC
	wm_par[paramNum++].u = c;
#else
	*index++ = c; 
#endif
}

void WinMgr::push(long l)
{
#ifndef HAVE_IPC
	wm_par[paramNum++].l = l;
#else
	stlong(l, index);
	index += sizeof(long);
#endif
}

void WinMgr::push(UnChar *s)
{
#ifndef HAVE_IPC
	if (s == NULL)
		*index = 0;
	else
		strcpy((char *) index, (char *) s);

	wm_par[paramNum++].s = (UnChar *) index;
	index += strlen((char *) index) + 1;
#else
	if (s == NULL)
		*index++ = 0;
	else { 
		strcpy(index, (char*)s); 
		index += strlen(index)+1;
	}
#endif
}

void WinMgr::push(String s)
{
#ifndef HAVE_IPC
	if (s == NULL_STRING || s == HIGH_VALUE)
		*index = 0;
	else
		strcpy((char *) index, toCharPtr(s));

	wm_par[paramNum++].s = (UnChar *) index;
	index += s.length() + 1;
#else
	if (s == NULL_STRING)
		*index++ = 0;
	else { 
		strcpy(index, (char*)toCharPtr(s)); 
		index += strlen(index)+1;
	}
#endif
}

void WinMgr::pop(short &i)
{
#ifndef HAVE_IPC
	i = wm_par[paramNum++].i;
#else
	i = lduint(index);
	index += sizeof(short);
#endif
}

void WinMgr::pop(UnChar &c)
{
#ifndef HAVE_IPC
	c = wm_par[paramNum++].u;
#else
	c = *index++;
#endif
}

void WinMgr::pop(long &l)
{
#ifndef HAVE_IPC
	l = wm_par[paramNum++].l;
#else
	l = ldlong(index);
	index += sizeof(long);
#endif
}

void WinMgr::pop(UnChar *s)
{
#ifndef HAVE_IPC
	strcpy((char *) s, (char *) wm_par[paramNum++].s);
#else
	strcpy((char*)s, index);
	index += strlen((char *) index) + 1;	
#endif
}

void WinMgr::pop(String &s)
{
#ifndef HAVE_IPC
	s = (char *) wm_par[paramNum++].s;
#else
	s = index;
	index += strlen(index) + 1;	
#endif
}
