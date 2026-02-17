/********************************************************************
* Copyright (c) 1991 InterSoft Co.  All Rights Reserved
* THIS IS UNPUBLISHED PROPRIETARY SOURCE CODE OF InterSoft Co.
* The copyright notice above does not evidence any
* actual or intended publication of such source code.
*
* $Id: cmdhist.cc,v 1.1.1.1 1996/04/16 18:52:47 gustavof Exp $
*
* DESCRIPTION: Implementation of CmdHistory class
*********************************************************************/

#include <winapp/cmdhist.h>
#include <winapp/wincmd.h>

CmdHistory::CmdHistory(Int len)
{ 
	idStart();
	maxsize = len;
	floor = 0;
	ind = ERROR;
	offset = LONG_NULL;
	list = (Command **) tr_new char[maxsize * sizeof(Command*)];
	for (Int i=0; i < len; i++)
		list[i] = NULL;
}

CmdHistory::~CmdHistory() 
{ 
	idCheck();
	clear();
	maxsize = 0;
	tr_delete(list);
	list = NULL;
	idStop();
}

void CmdHistory::clear()
{
	idCheck();
	for (Int i=0; i < maxsize; i++) {
		if (list[i]) {
			tr_delete(list[i]);
			list[i] = NULL;
		}
	}
	floor   = 0;
	offset  = LONG_NULL;
	ind     = ERROR;
}

void CmdHistory::insertCommand(Command *c)
{ 
	idCheck();
	if (offset != LONG_NULL && offset != ind)
		ind = offset;
	else {
		if (floor == (ind+1)%maxsize && ind != ERROR)
			floor = (floor+1)%maxsize;
	}

	ind = (ind+1)%maxsize;

	if (list[ind])
		tr_delete(list[ind]);

	list[ind] = c;

	offset = LONG_NULL;
}

Command *CmdHistory::retCommand()
{ 
	idCheck();
	return list[offset == LONG_NULL ? ind : offset];
}

Bool CmdHistory::reAlloc(Int len)
{
	idCheck();
	if (maxsize > len)
		return FALSE;

	if (maxsize == len)
		return TRUE;

	Command **aux = (Command **)tr_new char[len*sizeof(Command*)];

	for (Int j=0, i=floor; i != ind; i = (i+1)%maxsize, j++)
		aux[j] = list[i];

	aux[j] = list[ind];

	maxsize = len;
	floor = 0;
	ind = j;
	offset = LONG_NULL;
	
	tr_delete(list);
	list  = aux;

	return TRUE;
}

void CmdHistory::next()
{ 
	idCheck();
	// no me puedo mover
	if (offset == LONG_NULL)
		return;

	offset = (offset+1) % maxsize;

	// no hay mas comandos 
	if (offset == ind)
		offset = LONG_NULL;
}

void CmdHistory::prev()
{ 
	idCheck();
	// no hay elementos
	if (ind == ERROR)
		return;

	// comienzo a moverme
	if (offset == LONG_NULL) {
		offset = ind-1 >= 0 ? ind-1 : maxsize-1;
		return;
	}

	offset = offset-1 >= 0 ? offset-1 : maxsize-1;
}

Int CmdHistory::currLevel() const
{
	idCheck();
	if (offset == NULL_LONG)
		return levels();

	Int tmp = (offset + 1) % maxsize;
	return tmp - floor + (tmp >= floor ? 0 : maxsize);
}

Int CmdHistory::levels() const
{
	idCheck();
	return ind == ERROR ? 0
						: (ind - floor + 1 + (ind >= floor ? 0 : maxsize));
}

Bool CmdHistory::mayRedo() const
{
	idCheck();
	return currLevel() < levels();
}

Bool CmdHistory::mayUndo() const
{
	idCheck();
	return currLevel() > 0;
}
