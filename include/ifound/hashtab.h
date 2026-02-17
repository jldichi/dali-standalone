/********************************************************************
* Copyright (c) 1991 InterSoft Co.  All Rights Reserved
* THIS IS UNPUBLISHED PROPRIETARY SOURCE CODE OF InterSoft Co.
* The copyright notice above does not evidence any
* actual or intended publication of such source code.
*
*
* $Id: hashtab.h,v 1.1.1.1 1996/04/16 18:51:32 gustavof Exp $
*
*
* DESCRIPTION
*********************************************************************/

#ifndef HASHTAB_H
#define HASHTAB_H

#include <ifound.h>

#ifdef __GNUC__
#pragma interface
#endif

#include <ifound/bhashtab.h>

#define HashTab(KEY,CONTENT)	name3(KEY,CONTENT,HashTab)
#define N(KEY,CONTENT)			name3(KEY,CONTENT,N)

//	we declare this here so we can declare the tag
//	for the Cursor class

#define HashTabCursor(KEY,CONTENT)	name3(KEY,CONTENT,HashTabCursor)


#define HashTabdeclare2(KEY,CONTENT)									\
																		\
struct N(KEY,CONTENT) : HashNode {										\
																		\
	KEY k;																\
	CONTENT c;															\
																		\
	N(KEY,CONTENT)(const KEY &key, const CONTENT &cont);				\
	explicit N(KEY,CONTENT)(const KEY &key);							\
	static Int cmp(const void *left, const void *right);				\
	static Int hash(const void *node, Int modulo);						\
	static void del(N(KEY,CONTENT) *node);				 				\
};																		\
																		\
class HashTabCursor(KEY,CONTENT);										\
																		\
class HashTab(KEY,CONTENT) : public BaseHashTab {						\
																		\
	friend class HashTabCursor(KEY,CONTENT);							\
																		\
	HashTab(KEY,CONTENT)(const HashTab(KEY,CONTENT) &);					\
																		\
public:																	\
																		\
	explicit HashTab(KEY,CONTENT)(int m);								\
	CONTENT &add(const KEY &k, const CONTENT &cont);					\
	void del(const KEY &k);												\
	CONTENT *find(const KEY &k) const; 									\
																		\
};

#define HashTabimplement2(KEY,CONTENT)				  					\
																		\
N(KEY,CONTENT)::N(KEY,CONTENT)(const KEY &key, const CONTENT &cont)		\
: k(key), c(cont)														\
{ }																		\
												  						\
N(KEY,CONTENT)::N(KEY,CONTENT)(const KEY &key)							\
: k(key)																\
{ }																		\
												  						\
Int N(KEY,CONTENT)::cmp(const void *left, const void *right)			\
{ return ::cmp(*(KEY*)left, *(KEY*)right); }				 			\
												  						\
Int N(KEY,CONTENT)::hash(const void *node, Int modulo)					\
{ return hashVal(* (KEY *) node, modulo); }								\
												  						\
void N(KEY,CONTENT)::del(N(KEY,CONTENT) *node)							\
{ delete node; }										 				\
												  						\
HashTab(KEY,CONTENT)::HashTab(KEY,CONTENT)(int m)						\
:	BaseHashTab(m)														\
{											  		  					\
	f_cmp = (FP_CMP)N(KEY,CONTENT)::cmp;	  							\
	f_hash = (FP_HASH)N(KEY,CONTENT)::hash;	  							\
	f_del = (FP_DEL)N(KEY,CONTENT)::del;	  			 				\
}											  							\
												  						\
CONTENT &HashTab(KEY,CONTENT)::add(const KEY &k, const CONTENT &cont)	\
{ return ((N(KEY,CONTENT)*) addNode(new N(KEY,CONTENT)(k,cont)))->c; }	\
												  						\
void HashTab(KEY,CONTENT)::del(const KEY &k)							\
{ delNode(N(KEY,CONTENT)(k)); }											\
												  						\
CONTENT *HashTab(KEY,CONTENT)::find(const KEY &k) const 				\
{																		\
	N(KEY,CONTENT) *p;													\
	return (p = (N(KEY,CONTENT)*)findNode((void*)&k))	? &(p->c)		\
														: NULL;			\
}                                                              

#define HashTabCursordeclare2(KEY,CONTENT)							\
																	\
class HashTabCursor(KEY,CONTENT) : public BaseHashTabCursor {		\
																	\
																	\
	HashTabCursor(KEY,CONTENT)(const HashTabCursor(KEY,CONTENT) &); \
																	\
public:																\
																	\
	explicit HashTabCursor(KEY,CONTENT)(BaseHashTab *ht);			\
	const KEY &key();												\
	CONTENT &value();												\
};

#define HashTabCursorimplement2(KEY,CONTENT)						\
																	\
HashTabCursor(KEY,CONTENT)::HashTabCursor(KEY,CONTENT)(BaseHashTab *__h)\
: BaseHashTabCursor(__h) { }										\
																	\
const KEY &HashTabCursor(KEY,CONTENT)::key()						\
{ return ((N(KEY,CONTENT)*)currNode)->k; }							\
																	\
CONTENT &HashTabCursor(KEY,CONTENT)::value()						\
{ return ((N(KEY,CONTENT)*)currNode)->c; }

#endif // HASHTAB_H
