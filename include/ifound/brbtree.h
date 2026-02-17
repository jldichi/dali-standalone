/********************************************************************
* Copyright (c) 1991 InterSoft Co.  All Rights Reserved
* THIS IS UNPUBLISHED PROPRIETARY SOURCE CODE OF InterSoft Co.
* The copyright notice above does not evidence any
* actual or intended publication of such source code.
*
* $Id: brbtree.h,v 1.1.1.1 1996/04/16 18:51:31 gustavof Exp $
*
* DESCRIPTION
* Interface to base Red-Black Tree.
*********************************************************************/

#ifndef BRBTREE_H
#define BRBTREE_H

#include <ifound.h>

#ifdef __GNUC__
#pragma interface
#endif

#include <ifound/template.h>

struct BaseRBNode {

	int black;
	BaseRBNode *l, *r;
	void *contents;

	explicit BaseRBNode(void *c, int b = 1, BaseRBNode *ll = NULL,
			BaseRBNode *rr = NULL);

private:

    BaseRBNode(const BaseRBNode &);
    void operator=(const BaseRBNode &);

};

typedef	Int (*FP_CMP)(const void *, const void *);

class BaseRBTree {

	friend class RBTreeCursor;

// private:

	FP_CMP cmpNodes;
	BaseRBNode *head, *z;

	BaseRBNode *rotate(void *v, BaseRBNode *y);
	void split(BaseRBNode *x, BaseRBNode *p, BaseRBNode *g,
			BaseRBNode *gg, void *v);
	void listNode(BaseRBNode *x, FPVP pr) const;
	
	BaseRBTree(const BaseRBTree&) ;
	void operator=(const BaseRBTree&) ;

public:

	explicit BaseRBTree(FP_CMP fcmp);
	virtual ~BaseRBTree();

	void add(void *k);
	void del(void *k);
	void *find(void *k) const;
	void list(FPVP pr) const;
};

#endif // BRBTREE_H
