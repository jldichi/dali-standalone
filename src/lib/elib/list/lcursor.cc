/********************************************************************
* Copyright (c) 1991 InterSoft Co.  All Rights Reserved
* THIS IS UNPUBLISHED PROPRIETARY SOURCE CODE OF InterSoft Co.
* The copyright notice above does not evidence any
* actual or intended publication of such source code.
*
* $Id: lcursor.cc,v 1.1.1.1 1996/04/16 18:52:43 gustavof Exp $
*
* DESCRIPTION: Implementation of BaseListCursor class
*********************************************************************/

#include <ifound/list.h>

// metodos privados

void BaseListCursor::insCursor()
{
	if (list) {
		nextcur = list->headcur;
		list->headcur = this;
	}
}

void BaseListCursor::delCursor()
{
	if (list == NULL) return;
	for (BaseListCursor **aux=&list->headcur; *aux;aux=&(*aux)->nextcur)
		if (*aux == this) {
			if (this == list->headcur)
				list->headcur = nextcur;
			*aux = nextcur;
			break;
		}
}

// metodos publicos

BaseListCursor::BaseListCursor(const BaseList &l)
{ 
	list  = (BaseList *) &l; 
	curr  = l.head;
	nfil  = 0;
	insCursor();
}	

BaseListCursor::BaseListCursor()
{ 
	list = NULL;
	curr = NULL; 
	nfil  = 0;
}	

BaseListCursor::BaseListCursor(const BaseListCursor &lc)
{ 
	list = lc.list;
	curr = lc.curr;
	nfil = lc.nfil;
	insCursor();
}	

BaseListCursor::~BaseListCursor()
{
	delCursor();
}

void BaseListCursor::operator=(const BaseListCursor &c)
{
	if (&c != this) {
		delCursor();
		list = c.list;
		curr = c.curr;
		nfil = c.nfil;
		insCursor();
	}
}

void BaseListCursor::operator=(const BaseList &l)
{
	delCursor();
	list  = (BaseList *) &l; 
	curr  = l.head;
	nfil  = 0;
	insCursor();
}

void BaseListCursor::clear()
{ 
	delCursor();
	list = NULL;
	curr = NULL; 
	nfil  = 0;
}	

void BaseListCursor::next(Int n)
{
	// si estoy fuera de la lista voy con goTo
	if (list==NULL || curr==NULL) {
		goTo(nfil+n);
		return;
	}

	Int i;
	if (n < 0)  {
		for (i = -n; curr!=list->head && i; curr=curr->prev, --i)
			;
	}
	else {
		for (i = n; curr!=list->tail && i; curr=curr->next, --i)
			;
	}
	
	nfil += n;

	if (i!=0)			// estoy afuera de la lista
		curr = NULL;
}

void BaseListCursor::goTo(Int n)
{
	nfil = n;

	// goto fuera de rango
	if (list == NULL || n < 0 || n >= list->lsize) {
		curr = NULL;
		return;
	}
	for (curr = list->head; n ; --n, curr = curr->next)
		;
}

void BaseListCursor::goToFirst()
{
	if (list==NULL) return;
	curr = list->head;
	nfil = 0;
}

void BaseListCursor::goToLast()
{
	if (list==NULL) return;
	curr = list->tail;
	nfil = list->lsize-1;
}

void BaseListCursor::del()
{
	if (list == NULL || list->head && curr == NULL) 
		return;
	list->delNode(curr, nfil);
}

void BaseListCursor::incOrdCursor(Int f)
{
	// debo reacomodar los nfil de los cursores correspondientes
	for (BaseListCursor *aux = list->headcur; aux; aux = aux->nextcur)
		 if (aux->nfil >= f) aux->nfil++;				
}

void BaseListCursor::insNode(ListNode *it)
{
	// cursor no asignado a lista
	if (list == NULL) return;

	// estoy dentro de la lista
	if (*this) {
		if (list->head == curr) {
			list->insNode(it);
			return;
		}

		it->next = curr;
		it->prev = curr->prev;
		curr->prev->next = it;
		curr->prev = it;
		list->lsize++;
		list->mod = true;
		incOrdCursor(nfil);
	}
	// estoy fuera de la lista
	else setNode(it);
}

void BaseListCursor::addNode(ListNode *it)
{
	// cursor no asignado a lista
	if (list == NULL) return;

	// estoy dentro de la lista
	if (*this) {
		if (list->tail == curr) {
			list->addNode(it);
			return;
		}

		it->prev = curr;
		it->next = curr->next;
		curr->next->prev = it;
		curr->next = it;
		list->lsize++;
		list->mod = true;
		incOrdCursor(nfil+1);
	}
	// estoy fuera de la lista
	else setNode(it);
}

void BaseListCursor::addList(BaseList &l)
{
	BaseListCursor cur(l);

	cur.goToLast();

	for (; cur; cur--) 
		addNode(list->f_clone(cur.curr));
}

void BaseListCursor::insList(BaseList &l)
{
	BaseListCursor cur(l);

	for (;cur; cur++) 
		insNode(list->f_clone(cur.curr));
}

void BaseListCursor::setNode(ListNode *node)
{
	// cursor no asignado a lista
	if (list == NULL) return;

	list->setNode(node, nfil);
}

Int BaseListCursor::cmp(const BaseListCursor &blc) const
{ 
	return nfil - blc.nfil; 
}

ListNode *BaseListCursor::currNode() 
{ 
	return list->valNode(curr);
}

BasePtrListCursor::~BasePtrListCursor()
{
}

void BasePtrListCursor::addNode(void *node)
{
	BaseListCursor::addNode(tr_new PtrNode(node));
}

void BasePtrListCursor::insNode(void *node)
{
	BaseListCursor::insNode(tr_new PtrNode(node));
}

void BasePtrListCursor::setNode(void *node)
{
	BaseListCursor::setNode(tr_new PtrNode(node));
}
