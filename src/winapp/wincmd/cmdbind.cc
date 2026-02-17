/********************************************************************
* Copyright (c) 1993 InterSoft Co.  All Rights Reserved
* THIS IS UNPUBLISHED PROPRIETARY SOURCE CODE OF InterSoft Co.
* The copyright notice above does not evidence any
* actual or intended publication of such source code.
*
* $Id: cmdbind.cc,v 1.2 2004/04/30 20:50:56 gsposito Exp $
*
* DESCRIPTION: Implementation of class CmdBinding. 
*********************************************************************/

#include <stdio.h>
#include <ifound/stdc.h>
#include <ifound/str.h>
#include <ifound/hashtab.h>

#include <local/strfile.h>

#include <winapp/cmdbind.h>

// rutinas para la hashtab

Int cmp(UnShort us1, UnShort us2);
Int hashVal(UnShort us, Int mc);

declare2(HashTab,Int,Int);
implement2(HashTab,Int,Int);

#define HASHT(t)	((HashTab(Int,Int)*)(t))

Int cmp(UnShort us1, UnShort us2)
{
	return us1 - us2;
}

Int hashVal(UnShort us, Int mc)
{
	return us % mc;
}

// metodos privados 

CmdBinding::CmdBinding()
{
	tcmds =	tr_new HashTab(Int,Int)(MAX_COMMAND);
}

CmdBinding::~CmdBinding()
{
	tr_delete(HASHT(tcmds));
}

CmdId CmdBinding::getCmdId(EventId eid) const
{ 
	CmdId *i = HASHT(tcmds)->find(eid);
	return i ? *i : ERROR;
}

void CmdBinding::bind(EventId evid, CmdId cmdid)
{
	HASHT(tcmds)->add(evid, cmdid);
}

void CmdBinding::bindMenu(const String &key)
{
	bind(COMPOSE(K_CTRLX,key[0]),   MENU_KEY);
	bind(COMPOSE(K_CTRLX,tolower(key[0])),   MENU_KEY);
}


