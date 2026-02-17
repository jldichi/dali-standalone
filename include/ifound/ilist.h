/********************************************************************
* Copyright (c) 1991 InterSoft Co.  All Rights Reserved
* THIS IS UNPUBLISHED PROPRIETARY SOURCE CODE OF InterSoft Co.
* The copyright notice above does not evidence any
* actual or intended publication of such source code.
*
* $Id: ilist.h,v 1.2 1999/08/06 00:31:44 eduardoc Exp $
*
* DESCRIPTION
*********************************************************************/

#ifndef ILIST_H
#define ILIST_H
                
#include <ifound.h>

#ifdef __GNUC__
#pragma interface
#endif

#include <ifound/template.h>
#include <ifound/str.h>

class IListNode {
    
    friend class IListBase;
    
// private:

    IListNode(const IListNode &);
	void operator=(const IListNode &);
	IListNode *next;

public:

	IListNode()	{ next = NULL; }
	IListNode(IListNode* p)	{ next = p; }
	IListNode *getNext();
};


class IListBase  {

	friend class IListBaseIter;

// private:

	IListNode *last;	

  	IListBase(const IListBase &);
	void operator=(const IListBase &);

protected:

	Int number;

public:

	IListBase() { last = NULL; number = 0; };
    void insert(IListNode *p);
	void append(IListNode *p);
	IListNode *first() { return (last) ? last->getNext() : NULL; };
	IListNode *get();
	IListNode *next(IListNode *p) { return p->getNext(); };
};	

class IListBaseIter {

// private:

	IListNode *ce;
	IListBase *cl;
    IListBaseIter(const IListBaseIter &);
    void operator=(const IListBaseIter &);

public:
	
	explicit IListBaseIter(IListBase &l);
	IListNode *operator()();
};

//#define ILNode(CONTENT)	name2(CONTENT,ILNode)
#define IList(CONTENT)		name2(CONTENT,IList)
#define IListIter(CONTENT) 	name2(CONTENT,IListIter)


#define IListdeclare(CONTENT)										\
																	\
class IList(CONTENT) : public IListBase	 {							\
																	\
     friend class IListIter(CONTENT);								\
																	\
public:				                                    			\
																	\
	IList(CONTENT)();												\
																	\
	void insert(CONTENT *cont);	 									\
	void append(CONTENT *cont);	 								    \
																	\
	CONTENT *first();	 											\
	CONTENT *next(CONTENT *p);						 				\
	CONTENT *get();			                               			\
};

#define IListimplement(CONTENT)									\
																\
IList(CONTENT)::IList(CONTENT)() : IListBase() {};				\
																\
void IList(CONTENT)::insert(CONTENT *cont)	 					\
{	                                                            \
	IListBase::insert((IListNode *) cont);						\
}																\
																\
void IList(CONTENT)::append(CONTENT *cont)	 					\
{																\
	IListBase::append((IListNode *) cont);						\
}																\
																\
CONTENT *IList(CONTENT)::first()	 							\
{                                                               \
	return (CONTENT *) IListBase::first(); 			 			\
}                                                               \
																\
CONTENT *IList(CONTENT)::next(CONTENT *p)						\
{                                                               \
	return (CONTENT *) IListBase::next(p); 			 			\
}																\
																\
CONTENT *IList(CONTENT)::get()			                        \
{                                                               \
	return (CONTENT *) IListBase::get(); 			 		  	\
}


#define IListIterdeclare(CONTENT)									\
																	\
class IListIter(CONTENT) : public IListBaseIter	{					\
																	\
public:																\
																	\
	IListIter(CONTENT)(IList(CONTENT) &l);							\
																	\
	CONTENT *operator()();                                          \
};

#define IListIterimplement(CONTENT)									\
																	\
IListIter(CONTENT)::IListIter(CONTENT)(IList(CONTENT) &l) 			\
: IListBaseIter(l) {}    											\
																	\
CONTENT *IListIter(CONTENT)::operator()()                           \
{                                                               	\
	return (CONTENT *) IListBaseIter::operator()(); 	        	\
}

/*
void insert(const CONTENT &cont);
void append(const CONTENT &cont);
void IList(CONTENT)::insert(const CONTENT &cont)
{
	IListBase::insert((IListNode *) cont.clone());
}
void IList(CONTENT)::append(const CONTENT &cont)
{
	IListBase::append((IListNode *) cont.clone());
}
*/

#endif
