/********************************************************************
* Copyright (c) 1991 InterSoft Co.  All Rights Reserved
* THIS IS UNPUBLISHED PROPRIETARY SOURCE CODE OF InterSoft Co.
* The copyright notice above does not evidence any
* actual or intended publication of such source code.
*
* $Id: list.cc,v 1.1.1.1 1996/04/16 18:52:42 gustavof Exp $
*
* DESCRIPTION:
* Implementation of list class
*********************************************************************/
#ifdef __GNUC__
#pragma implementation
#endif

#include <ifound.h>
#include <ifound/list.h>

BaseList::BaseList(FP_NODE fpd, FP_NODE fpc) 
{ 
	head = tail = NULL; 
	headcur = NULL; 
	lsize = 0; 
	mod = false;
	nullnode = NULL;
	f_del = fpd;
	f_clone = fpc;
}

BaseList::BaseList(const BaseList &l)
{
	head = tail = NULL; 
	headcur = NULL; 
	lsize = 0; 
	f_del = l.f_del;
	f_clone = l.f_clone;
	nullnode = l.nullnode ? f_clone(l.nullnode) : NULL;
	addList(l);
	mod = false;
}

BaseList::~BaseList()
{ 
	delAll(); 
	if (nullnode)
		f_del(nullnode);
}

void BaseList::delAll()
{
	ListNode *aux, *aux1;
	int i;

	for(i=0, aux=head; i < lsize; i++) {
		aux1 = aux->next;
		f_del(aux);
		aux = aux1;
	}

	for (BaseListCursor *aux2=headcur; aux2; aux2=aux2->nextcur) {
		aux2->curr = NULL;
		aux2->reset();
	}

	head = tail = NULL; 
	lsize = 0; 
	mod = true;
}

void BaseList::insNode(ListNode *it, bool actcur)
{
	if (head == NULL)
		it->next = it->prev = head = tail = it;
	else {
		it->next = head;
		it->prev = tail;
		head->prev = tail->next = it;
		head = it;

		// actualizo la posicion de los cursores
		if (actcur) {
			for (BaseListCursor *aux=headcur; aux; aux=aux->nextcur)
				if (aux->nfil >= 0) aux->nfil++;
		}
	}
	lsize++;
	mod = true;
}

void BaseList::addNode(ListNode *it, bool actcur)
{
	if (tail == NULL)
		it->next = it->prev = head = tail = it;
	else {
		it->next = head;
		it->prev = tail;
		head->prev = tail->next = it;
		tail = it;

		// actualizo la posicion de los cursores
		if (actcur) {
			for (BaseListCursor *aux=headcur; aux; aux=aux->nextcur)
				if (!(*aux) && aux->nfil >= 0) aux->nfil++;
		}
	}
	lsize++;
	mod = true;
}

void BaseList::addList(const BaseList &l)
{
	BaseListCursor cur(l);

	for (; cur; cur++)
		addNode(f_clone(cur.curr));
}

void BaseList::insList(const BaseList &l)
{
	BaseListCursor cur(l);

	for (cur.goToLast(); cur; cur--)
		insNode(f_clone(cur.curr));
}

void BaseList::delNode(ListNode *it, Int fil)
{
	// la lista esta vacia
	if (head == NULL) return;

	// es el ultimo elemento debo limpiar al estructura
	if (lsize == 1) {
		delAll();
		return;
	}

	ListNode *new_node = it->next;
	it->next->prev = it->prev;
	it->prev->next = it->next;
	if (it == head)
		head = it->next;
	else if (it == tail) {
		tail = it->prev;
		new_node = tail;
	}
	lsize--;
	mod = true;

	// actualizo los cursores
	for (BaseListCursor *aux=headcur; aux; aux=aux->nextcur) {
		if (aux->curr == it) {
			aux->curr = new_node;
			if (new_node == tail) 
				aux->nfil = lsize-1;
		}
		else if (aux->nfil > fil)
			aux->nfil--;	
	}
	f_del(it);
}

void BaseList::setNode(ListNode *it, Int fil)
{
	mod = true;

	// estoy dentro de la lista
	if (lsize && fil < lsize && fil >= 0) {
		Int i; ListNode *aux;
		for(i=0, aux=head; i < fil; i++)
			aux = aux->next;
		*it = *aux;		// Copio los punteros sobre el nuevo nodo
		it->prev->next = it->next->prev = it;
		if (head == aux) head = it;
		if (tail == aux) tail = it;

		// actualizo los cursores
		for (BaseListCursor *aux1=headcur; aux1; aux1=aux1->nextcur)
			if (aux1->curr == aux) aux1->curr = it;

		f_del(aux);		// Borro el nodo anterior
		return;
	}

	// estoy fuera de la lista

	if (fil > 0) { 	// hacia abajo

		// hago append de las lineas de mas
		for (Int i=lsize; i < fil; i++)
			addNode(f_clone(valNode(nullnode)), false);	

		// hago append de la linea
		addNode(it, false);

	} else { 			// hacia arriba fil 

		// inserto las lineas de mas
		for (Int i = fil; i < -1; i++)
			insNode(f_clone(valNode(nullnode)), false);	

		// inserto la linea
		insNode(it, false);

		// ahora debo reasignar aquellos cursores que estaban
		// fuera de la lista y ahora quedaron apuntando a un
		// nodo valido
		for (BaseListCursor *aux=headcur; aux; aux=aux->nextcur) {
			 if (*aux) aux->nfil += -(fil);
			 if (!(*aux) && aux->nfil < 0 && aux->nfil >= fil)
					aux->goTo(aux->nfil-fil);
		}
	}

	// ahora debo reasignar aquellos cursores que estaban
	// fuera de la lista y ahora quedaron apuntando a un
	// nodo valido
	for (BaseListCursor *aux = headcur; aux; aux = aux->nextcur) {
		 if (!(*aux) && aux->nfil < lsize && aux->nfil >= 0)
			aux->goTo(aux->nfil);
	}
}

BaseList &BaseList::operator=(const BaseList &l)
{
	if (this != &l) {
		delAll();
		addList(l);
	}
	return *this;
}

ListNode *BaseList::valNode(ListNode *n) 
{ 
	if (n) return n;
	if (nullnode) return nullnode;
	error("BaseList: NULLNODE required and not established!");
	return 0;	// Shouldn't reach...
}

// class BasePtrList

static void delPtrNode(PtrNode *node)
{
	tr_delete(node);
}

static PtrNode *clonePtrNode(PtrNode *node)
{
	return tr_new PtrNode(node->c);
}

BasePtrList::BasePtrList() : BaseList((FP_NODE)delPtrNode,
									   (FP_NODE)clonePtrNode)
{ 
	nullnode = tr_new PtrNode(NULL);
}

void BasePtrList::addNode(void *node)
{
	BaseList::addNode(tr_new PtrNode(node));
}

void BasePtrList::insNode(void *node)
{
	BaseList::insNode(tr_new PtrNode(node));
}

void BasePtrList::setNode(void *node, Int fil)
{
	BaseList::setNode(tr_new PtrNode(node), fil);
}
