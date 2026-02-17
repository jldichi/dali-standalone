/********************************************************************
* Copyright (c) 1991 InterSoft Co.  All Rights Reserved
* THIS IS UNPUBLISHED PROPRIETARY SOURCE CODE OF InterSoft Co.
* The copyright notice above does not evidence any
* actual or intended publication of such source code.
*
* $Id: rbtree.h,v 1.1.1.1 1996/04/16 18:51:33 gustavof Exp $
*
* DESCRIPTION
* Interface to generic Red-Black Trees
*********************************************************************/

#ifndef RBTREE_H
#define RBTREE_H

#include <ifound.h>

#ifdef __GNUC__
#pragma interface
#endif


#include <ifound/brbtree.h>

#define RBTree(CONTENT)				name2(CONTENT,RBTree)
#define RBTreeCursor(CONTENT)		name2(CONTENT,RBTreeCursor)
#define RBNODE(CONTENT)				name2(CONTENT,RBNODE)

#define RBTreedeclare(CONTENT)										\
																	\
struct RBNODE(CONTENT) : public BaseRBNode {						\
	explicit RBNODE(CONTENT)(CONTENT *c) : BaseRBNode(c) { }		\
};																	\
																	\
class RBTree(CONTENT) : public BaseRBTree {							\
																	\
public:																\
																	\
	explicit RBTree(CONTENT)(FP_CMP fcmp)							\
	:	BaseRBTree(fcmp)											\
	{ }																\
																	\
	void add(CONTENT *k)											\
	{ BaseRBTree::add(k); }											\
	void del(CONTENT *k)											\
	{ BaseRBTree::del(k); }											\
	CONTENT *find(CONTENT *k) const									\
	{ return (CONTENT *) BaseRBTree::find(k); }						\
	void list(FPVP pr) const										\
	{ BaseRBTree::list(pr); }										\
																	\
};

#endif // RBTREE_H
