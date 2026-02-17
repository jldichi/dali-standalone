/********************************************************************
* Copyright (c) 1991 InterSoft Co.  All Rights Reserved
* THIS IS UNPUBLISHED PROPRIETARY SOURCE CODE OF InterSoft Co.
* The copyright notice above does not evidence any
* actual or intended publication of such source code.
*
* $Id: phash.cc,v 1.1.1.1 1996/04/16 18:51:57 gustavof Exp $
*
* DESCRIPTION
*
*********************************************************************/

#include <ifound/osdep.h>

#ifdef __GNUC__
#pragma implementation "phashtab.h"
#endif

#include <ifound/phashtab.h>


PtrHashNode::PtrHashNode(void * cont)
{
	c = cont;
}

void PtrHashNode::del(PtrHashNode *node)
{
	tr_delete(node);
}

BasePtrHashTab::BasePtrHashTab(int m) : BaseHashTab(m)
{
	f_del = (FP_DEL)PtrHashNode::del;
}

void BasePtrHashTab::add(void *cont)
{
	addNode(tr_new PtrHashNode(cont));
}

void BasePtrHashTab::del(void *cont)
{
	delNode(PtrHashNode(cont));
}

void *BasePtrHashTab::pfind(void *cont) const
{
	PtrHashNode *p;
	p = (PtrHashNode *) findNode(cont);
	return p ? p->c : NULL;
}

#ifdef DEBUG

#include <stdio.h>
void BasePtrHashTab::list() const
{
	int	i,
		j;
	static int runs = 0;

	HashNode *pcurr;
	void *elem;

	fprintf(stderr, "-------------- Pass #%d --------------\n", runs++);
	for (i = 0; i < modulo; i++) {
		j = 0;
		pcurr = buckets()[i];
		while (pcurr) {
			elem = ((PtrHashNode *) pcurr)->c;
fprintf(stderr, "bucket %d, node %d = %ld\n", i, j++, long(elem));
			pcurr = pcurr->next;
		}
	}
	fprintf(stderr, "--------------------------------------\n");
	fflush(stderr);
}
#endif
