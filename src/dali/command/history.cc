/********************************************************************
* Copyright (c) 1991 InterSoft Co.  All Rights Reserved
* THIS IS UNPUBLISHED PROPRIETARY SOURCE CODE OF InterSoft Co.
* The copyright notice above does not evidence any
* actual or intended publication of such source code.
*
* $Id: history.cc,v 1.1.1.1 1996/04/16 18:52:50 gustavof Exp $
*
* DESCRIPTION: Implementation of history class
*********************************************************************/

#include <command.h>

History::History(Int len)
{ 
	idStart();
	maxsize = len;
	floor = 0;
	ind = ERROR;
	offset = INT_NULL;
	list = (Command **) tr_new char[maxsize * sizeof(Command*)];
	for (Int i=0; i < len; i++)
		list[i] = NULL;
}

History::~History() 
{ 
	idCheck();
	clear();
	maxsize = 0;
	tr_delete_vec(list);
	list = NULL;
	idStop();
}

void History::clear()
{
	idCheck();
	for (Int i=0; i < maxsize; i++) {
		if (list[i]) {
			tr_delete(list[i]);
			list[i] = NULL;
		}
	}
	floor   = 0;
	offset  = INT_NULL;
	ind     = ERROR;
}

void History::insertCommand(Command *c)
{ 
	idCheck();
	if (offset != INT_NULL && offset != ind)
		ind = offset;
	else {
		if (floor == (ind+1)%maxsize && ind != ERROR)
			floor = (floor+1)%maxsize;
	}

	ind = (ind+1)%maxsize;

	if (list[ind])
		tr_delete(list[ind]);

	list[ind] = c;

	offset = INT_NULL;
}

Command *History::retCommand()
{ 
	idCheck();
	return list[offset == INT_NULL ? ind : offset];
}

bool History::reAlloc(Int len)
{
	idCheck();
	if (maxsize > len)
		return false;

	if (maxsize == len)
		return true;

	Command **aux = (Command **)tr_new char[len*sizeof(Command*)];

	Int j, i;
	for (j=0, i=floor; i != ind; i = (i+1)%maxsize, j++)
		aux[j] = list[i];

	aux[j] = list[ind];

	maxsize = len;
	floor = 0;
	ind = j;
	offset = INT_NULL;
	
	tr_delete(list);
	list  = aux;

	return true;
}

void History::next()
{ 
	idCheck();
	// no me puedo mover
	if (offset == INT_NULL)
		return;

	offset = (offset+1) % maxsize;

	// no hay mas comandos 
	if (offset == ind)
		offset = INT_NULL;
}

void History::prev()
{ 
	idCheck();
	// no hay elementos
	if (ind == ERROR)
		return;

	// comienzo a moverme
	if (offset == INT_NULL) {
		offset = ind-1 >= 0 ? ind-1 : maxsize-1;
		return;
	}

	offset = offset-1 >= 0 ? offset-1 : maxsize-1;
}

Int History::currLevel() const
{
	idCheck();
	if (offset == INT_NULL)
		return levels();

	Int tmp = (offset + 1) % maxsize;
	return tmp - floor + (tmp >= floor ? 0 : maxsize);
}

Int History::levels() const
{
	idCheck();
	return ind == ERROR ? 0
						: (ind - floor + 1 + (ind >= floor ? 0 : maxsize));
}

bool History::mayRedo() const
{
	idCheck();
	return currLevel() < levels();
}

bool History::mayUndo() const
{
	idCheck();
	return currLevel() > 0;
}
