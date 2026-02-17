/********************************************************************
* Copyright (c) 1991 InterSoft Co.  All Rights Reserved
* THIS IS UNPUBLISHED PROPRIETARY SOURCE CODE OF InterSoft Co.
* The copyright notice above does not evidence any
* actual or intended publication of such source code.
*
*
* $Id: hashcsr.cc,v 1.1.1.1 1996/04/16 18:51:57 gustavof Exp $
*
*
* DESCRIPTION:
*				
*
*********************************************************************/


#include <ifound/hashtab.h>
#include <ifound/itypes.h>


BaseHashTabCursor::BaseHashTabCursor(BaseHashTab *bht)
{
	ht = bht;
	first();	// position self in the first node
}

BaseHashTabCursor::operator void * ()
{
	return (slot == -1)? NULL : this;
}

void BaseHashTabCursor::first()
{
	slot = -1;
	currNode = nextNode = NULL;
	next();		// advance to the first node
}

void BaseHashTabCursor::next()
{
	while (nextNode == NULL && ++slot < ht->modulo)
		nextNode = ht->tab[slot];

	if (nextNode) {
		currNode = nextNode;
		nextNode = nextNode->next;
	}
	else slot = -1;
}

// end of hashcsr
