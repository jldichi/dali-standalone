/********************************************************************
* Copyright (c) 1991 InterSoft Co.  All Rights Reserved
* THIS IS UNPUBLISHED PROPRIETARY SOURCE CODE OF InterSoft Co.
* The copyright notice above does not evidence any
* actual or intended publication of such source code.
*
*
* $Id: ilist.cc,v 1.2 1999/08/06 00:54:24 eduardoc Exp $
*
*
* DESCRIPTION:
*				Implementation of IList class
*
*********************************************************************/

#include <ifound/ilist.h>

void IListBase::insert(IListNode *p)
{
	if (last)
		p->next = last->next;
	else
		last = p;
	last->next = p;
	number++;
}

void IListBase::append(IListNode *p)
{
	if (last) {
		p->next = last->next;
		last = last->next = p;
	} else
		last = p->next = p;
	number++;
}

IListNode *IListBase::get()
{
	if (last==NULL) return NULL;
	IListNode *tmp = last->next;
	if (tmp == last)
		last = NULL;
	else
		last->next = tmp->next;
	number--;
	return tmp;
}

IListNode *IListNode::getNext()
{
	return next;
}

// Clase base para los Iterators.

IListBaseIter::IListBaseIter(IListBase &l)
{ 
	cl = &l; 
	ce = cl->last; 
}


IListNode* IListBaseIter::operator()()
{
	IListNode* ret = ce ? (ce = ce->getNext()) : NULL;
	if (ce == cl->last) ce = NULL;
	return ret;
}

