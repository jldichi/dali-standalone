/********************************************************************
* Copyright (c) 1991 InterSoft Co.  All Rights Reserved
* THIS IS UNPUBLISHED PROPRIETARY SOURCE CODE OF InterSoft Co.
* The copyright notice above does not evidence any
* actual or intended publication of such source code.
*
* $Id: list.h,v 1.3 2007/04/20 21:26:04 hardaiz Exp $
*
* DESCRIPTION
*********************************************************************/

#ifndef LIST_H
#define LIST_H

#ifdef __GNUG__
#	pragma interface
#endif

#include <ifound.h>

#include <ifound/template.h>
#include <ifound/str.h>

struct ListNode {
	ListNode *next, *prev;
};

typedef ListNode * (*FP_NODE)(const ListNode *);

// Se agrega para indicar que la clase existe - A partir de GCC 4.x
class BaseListCursor;

class BaseList
{
	friend class BaseListCursor;

// private:

	Int lsize;						// Longitud de la lista
	bool mod;						// se modifico la lista ?
	FP_NODE f_del, f_clone;

protected:

	ListNode *nullnode;				// valor nulo del componente
	BaseListCursor *headcur;		// head de la lista de cursores

	ListNode *head;					// cabeza de la lista 
	ListNode *tail;					// cola de la lista 

	void addList(const BaseList &list);
	void insList(const BaseList &list);

	void addNode(ListNode *node, bool actcur = true);
	void insNode(ListNode *node, bool actcur = true);
	void delNode(ListNode *node, Int fil);
	void setNode(ListNode *node, Int fil);

public:

	BaseList(FP_NODE, FP_NODE);
	BaseList(const BaseList &l);
	~BaseList();

	void delFirst()		{ delNode(head, 0); 	}
	void delLast()		{ delNode(tail, lsize); }
	void delAll();
	Int  size()	const	{ return lsize; 		}
	bool modif() const	{ return mod;			}
	void resetModif() 	{ mod = false;			}
	void setModif() 	{ mod = true;			}
    BaseListCursor *listCursorHead() { return headcur; }
	BaseList &operator=(const BaseList &l);

	ListNode *valNode(ListNode *n);
};

class BaseListCursor
{
	friend class BaseList;

// private:

	Int  nfil;				// numero de fila del cursor
	void insCursor();
	void delCursor();
	void incOrdCursor(Int);

protected:

	ListNode *curr;			// puntero al nodo corriente

public:

	BaseList *list;			// lista a la que esta asociada

#ifdef ISDEBUG
	void trace();
#endif

protected:

	BaseListCursor *nextcur;// lista de cursores

	void addList(BaseList &l);
	void insList(BaseList &l);

	void setNode(ListNode *node);
	void addNode(ListNode *node);
	void insNode(ListNode *node);

	ListNode *currNode();

public:
	
	explicit BaseListCursor(const BaseList &l);
	BaseListCursor(const BaseListCursor &lc);
	BaseListCursor();
	virtual ~BaseListCursor();

	Int order() const			{ return nfil; }
	void next(Int n = 1);		// avanzo el cursor n posiciones
	void prev(Int n = 1)		{ next(-n);}
	void operator++() 			{ next();  } // prefix
	void operator++(int)		{ next();  } // postfix
	void operator--() 			{ next(-1);} // prefix
	void operator--(int) 		{ next(-1);} // postfix
	void operator+=(Int n) 		{ next(n); }
	void operator-=(Int n) 		{ next(-n);}

	void goToFirst();			// voy al primer elemento de la lista
	void goToLast();			// voy al final de la lista
	void goTo(Int n);			// voy al nodo n-esimo

	bool isFirst()				{ return list && curr && curr == list->head; }
	bool isLast()				{ return list && curr && curr == list->tail; }

	int operator! () const		{ return curr == NULL; }
	operator void*() const		{ return curr; }

	void clear();

	void operator=(const BaseListCursor &);
	void operator=(const BaseList &);
	void del();

	virtual Int cmp(const BaseListCursor &blc) const;
	virtual void reset() { nfil = 0; }	

	friend Int  operator- (const BaseListCursor &blc1, 
						   const BaseListCursor &blc2)
		{ return blc1.cmp(blc2); }

	friend bool operator==(const BaseListCursor &blc1, 
						   const BaseListCursor &blc2)
		{ return blc1.cmp(blc2) == 0; }
	friend bool operator!=(const BaseListCursor &blc1, 
						   const BaseListCursor &blc2)
		{ return blc1.cmp(blc2) != 0; }
	friend bool operator> (const BaseListCursor &blc1, 
						   const BaseListCursor &blc2)
		{ return blc1.cmp(blc2) > 0; }
	friend bool operator< (const BaseListCursor &blc1, 
						   const BaseListCursor &blc2)
		{ return blc1.cmp(blc2) < 0; }
	friend bool operator>=(const BaseListCursor &blc1, 
						   const BaseListCursor &blc2)
		{ return blc1.cmp(blc2) >= 0; }
	friend bool operator<=(const BaseListCursor &blc1, 
						   const BaseListCursor &blc2)
		{ return blc1.cmp(blc2) <= 0; }

	void print();

};

struct PtrNode : ListNode {
	void *c;
	PtrNode(void *n) { c = n; }
};

class BasePtrList : public BaseList {

	friend class BasePtrListCursor;

// private:

protected:

	void addNode(void *node);
	void insNode(void *node);
	void setNode(void *node, Int fil);

public:

	BasePtrList();
	BasePtrList(const BasePtrList &l) : BaseList(l) {}
	~BasePtrList() {}

	BasePtrList &operator=(const BasePtrList &l)
		{ (void)BaseList::operator=(l); return *this; }
};

class BasePtrListCursor : public BaseListCursor {

	friend class BasePtrList;

protected:

	void setNode(void *node);
	void addNode(void *node);
	void insNode(void *node);

public:
	
	explicit BasePtrListCursor(const BasePtrList &l) : BaseListCursor(l) {}
	BasePtrListCursor(const BasePtrListCursor &lc) : BaseListCursor(lc) {}
	BasePtrListCursor() : BaseListCursor() {}
	virtual ~BasePtrListCursor();

	void operator=(const BasePtrListCursor &lc)
		{ BaseListCursor::operator=(lc); }
	void operator=(const BasePtrList &l)
		{ BaseListCursor::operator=(l); }

};

#define List(CONTENT)		name2(CONTENT,List)
#define PtrList(CONTENT)	name2(CONTENT,PtrList)
#define LNode(CONTENT)		name2(CONTENT,N)

//	we declare this here so we can declare the tag
//	for the Cursor class

#define ListCursor(CONTENT)		name2(CONTENT,ListCursor)
#define PtrListCursor(CONTENT)	name2(CONTENT,PtrListCursor)


#define Listdeclare(CONTENT)										\
																	\
struct LNode(CONTENT) : ListNode {									\
	CONTENT c;														\
	explicit LNode(CONTENT)(const CONTENT &cont);					\
	static void del(LNode(CONTENT) *node);							\
	static LNode(CONTENT) *clone(const LNode(CONTENT) *node);		\
};																	\
																	\
class List(CONTENT) : public BaseList {								\
																	\
	friend class ListCursor(CONTENT);								\
																	\
																	\
public:																\
																	\
	List(CONTENT)();												\
	explicit List(CONTENT)(const CONTENT &nv);						\
	List(CONTENT)(const List(CONTENT) &l);				 			\
																	\
	void add(const CONTENT &cont);									\
	void ins(const CONTENT &cont);									\
	void add(List(CONTENT) &l);										\
	void ins(List(CONTENT) &l);										\
																	\
	void operator+=(List(CONTENT) &l);								\
	List(CONTENT) &operator=(const List(CONTENT) &l); 				\
	List(CONTENT)& operator<<(const CONTENT &cont);					\
	CONTENT first();												\
	CONTENT last();													\
};

#define Listimplement(CONTENT)										\
																	\
LNode(CONTENT)::LNode(CONTENT)(const CONTENT &cont) : c(cont)		\
{ }																	\
																	\
void LNode(CONTENT)::del(LNode(CONTENT) *node)						\
{ delete node; }													\
																	\
LNode(CONTENT) *LNode(CONTENT)::clone(const LNode(CONTENT) *node)	\
{ return new LNode(CONTENT)(node->c); }								\
																	\
List(CONTENT)::List(CONTENT)()										\
:	BaseList((FP_NODE)LNode(CONTENT)::del, 							\
	(FP_NODE)LNode(CONTENT)::clone)									\
{ }																	\
																	\
List(CONTENT)::List(CONTENT)(const CONTENT &nv)						\
:	BaseList((FP_NODE)LNode(CONTENT)::del, 							\
	(FP_NODE)LNode(CONTENT)::clone)									\
{ nullnode = new LNode(CONTENT)(nv); }								\
																	\
List(CONTENT)::List(CONTENT)(const List(CONTENT) &l)						\
:	BaseList(l)														\
{ } 																\
																	\
void List(CONTENT)::add(const CONTENT &cont)						\
{ LNode(CONTENT) *tmp = new LNode(CONTENT)(cont); addNode(tmp);	}	\
																	\
void List(CONTENT)::ins(const CONTENT &cont)						\
{ LNode(CONTENT) *tmp = new LNode(CONTENT)(cont); insNode(tmp);	}	\
																	\
void List(CONTENT)::add(List(CONTENT) &l)							\
{ addList(l); }														\
																	\
void List(CONTENT)::ins(List(CONTENT) &l)							\
{ insList(l); }														\
																	\
void List(CONTENT)::operator+=(List(CONTENT) &l)					\
{ addList(l); }														\
																	\
List(CONTENT) &List(CONTENT)::operator=(const List(CONTENT) &l)			\
{ (void) BaseList::operator=(l); return *this; }					\
																	\
List(CONTENT) &List(CONTENT)::operator<<(const CONTENT &cont)		\
{ addNode(new LNode(CONTENT)(cont)); return *this; }				\
																	\
CONTENT List(CONTENT)::first()										\
{ return ((LNode(CONTENT)*)valNode(head))->c; }						\
																	\
CONTENT List(CONTENT)::last()										\
{ return ((LNode(CONTENT)*)valNode(tail))->c; }

#define ListCursordeclare(CONTENT)									\
																	\
class ListCursor(CONTENT) : public BaseListCursor					\
{																	\
																	\
public:																\
																	\
	ListCursor(CONTENT)();											\
	ListCursor(CONTENT)(const ListCursor(CONTENT) &l);				\
	explicit ListCursor(CONTENT)(const List(CONTENT) &l);			\
	~ListCursor(CONTENT)();											\
																	\
	CONTENT &value();												\
	CONTENT &operator*();											\
	CONTENT *operator->();											\
	void operator=(const ListCursor(CONTENT) &lc);					\
	void operator=(const List(CONTENT) &l);		  					\
																	\
	void add(const CONTENT &cont);									\
	void ins(const CONTENT &cont);									\
	void set(const CONTENT &cont);									\
																	\
	void add(List(CONTENT) &l);										\
	void ins(List(CONTENT) &l);										\
};

#define ListCursorimplement(CONTENT)								\
																	\
ListCursor(CONTENT)::ListCursor(CONTENT)()							\
: BaseListCursor() { }												\
																	\
ListCursor(CONTENT)::ListCursor(CONTENT)(const ListCursor(CONTENT) &l)	\
: BaseListCursor(l) { }												\
																	\
ListCursor(CONTENT)::ListCursor(CONTENT)(const List(CONTENT) &l)	\
: BaseListCursor(l)	{ }												\
																	\
ListCursor(CONTENT)::~ListCursor(CONTENT)()							\
{ }																	\
																	\
CONTENT &ListCursor(CONTENT)::value()								\
{ return ((LNode(CONTENT)*)currNode())->c; }						\
																	\
CONTENT &ListCursor(CONTENT)::operator*()							\
{ return value(); }													\
																	\
CONTENT *ListCursor(CONTENT)::operator->()							\
{ return &value(); }												\
																	\
void ListCursor(CONTENT)::operator=(const ListCursor(CONTENT) &lc)	\
{ BaseListCursor::operator=(lc); }									\
																	\
void ListCursor(CONTENT)::operator=(const List(CONTENT) &l)			\
{ BaseListCursor::operator=(l); }									\
																	\
void ListCursor(CONTENT)::add(const CONTENT &cont)					\
{ LNode(CONTENT) *tmp = new LNode(CONTENT)(cont); addNode(tmp); }	\
																	\
void ListCursor(CONTENT)::ins(const CONTENT &cont)					\
{ LNode(CONTENT) *tmp = new LNode(CONTENT)(cont); insNode(tmp);	}	\
																	\
void ListCursor(CONTENT)::set(const CONTENT &cont)					\
{ LNode(CONTENT) *tmp = new LNode(CONTENT)(cont); setNode(tmp);	}	\
																	\
void ListCursor(CONTENT)::add(List(CONTENT) &l)						\
{ addList(l); }														\
																	\
void ListCursor(CONTENT)::ins(List(CONTENT) &l)						\
{ insList(l); }

#define PtrListdeclare(CONTENT)										\
																	\
struct LNode(CONTENT) : ListNode {									\
	CONTENT *c;														\
};																	\
																	\
class PtrList(CONTENT) : public BasePtrList							\
{																	\
																	\
	friend class PtrListCursor(CONTENT);							\
																	\
public:																\
																	\
	PtrList(CONTENT)();												\
	PtrList(CONTENT)(const PtrList(CONTENT) &l);					\
																	\
	void add(CONTENT *cont);										\
	void ins(CONTENT *cont);										\
	void add(PtrList(CONTENT) &l);									\
	void ins(PtrList(CONTENT) &l);									\
																	\
	void operator+=(PtrList(CONTENT) &l);							\
	PtrList(CONTENT) &operator=(const PtrList(CONTENT) &l);			\
	PtrList(CONTENT)& operator<< (CONTENT &cont);					\
																	\
	CONTENT &first();												\
	CONTENT &last();												\
};

#define PtrListimplement(CONTENT)									\
																	\
PtrList(CONTENT)::PtrList(CONTENT)()								\
: BasePtrList() { }													\
																	\
PtrList(CONTENT)::PtrList(CONTENT)(const PtrList(CONTENT) &l)		\
: BasePtrList(l) { } 												\
																	\
void PtrList(CONTENT)::add(CONTENT *cont)							\
{ addNode(cont); }													\
																	\
void PtrList(CONTENT)::ins(CONTENT *cont)							\
{ insNode(cont); }													\
																	\
void PtrList(CONTENT)::add(PtrList(CONTENT) &l)						\
{ addList(l); }														\
																	\
void PtrList(CONTENT)::ins(PtrList(CONTENT) &l)						\
{ insList(l); }														\
																	\
void PtrList(CONTENT)::operator+=(PtrList(CONTENT) &l)				\
{ addList(l); }														\
																	\
PtrList(CONTENT) &PtrList(CONTENT)::operator=(const PtrList(CONTENT) &l)	\
{ BasePtrList::operator=(l); return *this; }						\
																	\
PtrList(CONTENT) &PtrList(CONTENT)::operator<<(CONTENT &cont)		\
{ addNode(&cont); return *this; }									\
																	\
CONTENT &PtrList(CONTENT)::first()									\
{ return *((LNode(CONTENT)*)valNode(head))->c; }					\
																	\
CONTENT &PtrList(CONTENT)::last()									\
{ return *((LNode(CONTENT)*)valNode(tail))->c; }

#define PtrListCursordeclare(CONTENT)								\
																	\
class PtrListCursor(CONTENT) : public BasePtrListCursor	{			\
																	\
public:																\
																	\
	PtrListCursor(CONTENT)();										\
	PtrListCursor(CONTENT)(const PtrListCursor(CONTENT) &lc);		\
	explicit PtrListCursor(CONTENT)(const PtrList(CONTENT) &l);		\
	~PtrListCursor(CONTENT)();					 					\
																	\
	CONTENT *value();												\
	CONTENT *operator*();											\
	CONTENT *operator->();											\
																	\
	void operator=(const PtrListCursor(CONTENT) &lc);				\
	void operator=(const PtrList(CONTENT) &l); 						\
																	\
	void add(CONTENT *cont);										\
	void ins(CONTENT *cont);										\
	void set(CONTENT *cont);										\
																	\
	void add(PtrList(CONTENT) &l);									\
	void ins(PtrList(CONTENT) &l);									\
};

#define PtrListCursorimplement(CONTENT)								\
																	\
PtrListCursor(CONTENT)::PtrListCursor(CONTENT)()					\
: BasePtrListCursor() { }											\
																	\
PtrListCursor(CONTENT)::PtrListCursor(CONTENT)(const PtrListCursor(CONTENT)&lc)\
: BasePtrListCursor(lc) { }											\
																	\
PtrListCursor(CONTENT)::PtrListCursor(CONTENT)(const PtrList(CONTENT) &l) \
: BasePtrListCursor(l) { }											\
																	\
PtrListCursor(CONTENT)::~PtrListCursor(CONTENT)()					\
{ }																	\
																	\
CONTENT *PtrListCursor(CONTENT)::value()							\
{ return ((LNode(CONTENT)*)currNode())->c; }						\
																	\
CONTENT *PtrListCursor(CONTENT)::operator*()						\
{ return value(); }													\
																	\
CONTENT *PtrListCursor(CONTENT)::operator->()						\
{ return value(); }													\
																	\
void PtrListCursor(CONTENT)::operator=(const PtrListCursor(CONTENT) &lc) 	\
{ BasePtrListCursor::operator=(lc); }								\
																	\
void PtrListCursor(CONTENT)::operator=(const PtrList(CONTENT) &l) 		\
{ BasePtrListCursor::operator=(l); }								\
																	\
void PtrListCursor(CONTENT)::add(CONTENT *cont)						\
{ addNode(cont); }													\
																	\
void PtrListCursor(CONTENT)::ins(CONTENT *cont)						\
{ insNode(cont); }													\
																	\
void PtrListCursor(CONTENT)::set(CONTENT *cont)						\
{ setNode(cont); }													\
																	\
void PtrListCursor(CONTENT)::add(PtrList(CONTENT) &l)				\
{ addList(l); }														\
																	\
void PtrListCursor(CONTENT)::ins(PtrList(CONTENT) &l)				\
{ insList(l); }

#endif // LIST_H
