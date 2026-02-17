/********************************************************************
* Copyright (c) 1991 InterSoft Co.  All Rights Reserved
* THIS IS UNPUBLISHED PROPRIETARY SOURCE CODE OF InterSoft Co.
* The copyright notice above does not evidence any
* actual or intended publication of such source code.
*
* $Id: phashtab.h,v 1.2 2006/02/14 21:29:03 hardaiz Exp $
*
* DESCRIPTION
*********************************************************************/
//Oscar is documenting this file (/usr2/acct/oscar/doc/phashtab/phashtab.doc)
#ifndef PHASHTAB_H
#define PHASHTAB_H

#include <ifound.h>

#ifdef __GNUC__
#pragma interface
#endif


#include <ifound/bhashtab.h>

struct PtrHashNode : HashNode {
	void *c;
	explicit PtrHashNode(void *cont);
	static void del(PtrHashNode *node);
	
//private:	

    PtrHashNode(const PtrHashNode &);
    void operator=(const PtrHashNode &);
};

class BasePtrHashTab : public BaseHashTab {
  
// private:             

    BasePtrHashTab(const BasePtrHashTab &);
    void operator=(const BasePtrHashTab &); 

protected:

	explicit BasePtrHashTab(int m);
	void *pfind(void *cont) const;

public:

	void add(void *cont);
	void del(void *cont);
#ifdef ISDEBUG
	void list() const;
#endif
};

#define PtrHashTab(CONTENT)	name2(CONTENT,PtrHashTab)
#define PTRN(CONTENT)		name2(CONTENT,PTRN)

#define PtrHashTabdeclare(CONTENT)										\
																		\
struct PTRN(CONTENT) : PtrHashNode {									\
	PTRN(CONTENT)(CONTENT *c);											\
};																		\
																		\
class PtrHashTab(CONTENT) : public BasePtrHashTab {						\
																		\
																		\
	PtrHashTab(CONTENT)(const PtrHashTab(CONTENT)&) ;					\
	void operator=(const PtrHashTab(CONTENT)&) ;						\
														 				\
public:																	\
														 				\
	PtrHashTab(CONTENT)(int m, FP_CMP f1, FP_HASH f2);					\
	CONTENT *find(CONTENT *k) const;									\
																		\
};

#define PtrHashTabimplement(CONTENT)									\
																		\
PTRN(CONTENT)::PTRN(CONTENT)(CONTENT *elem)								\
: PtrHashNode(elem) { }													\
																		\
PtrHashTab(CONTENT)::PtrHashTab(CONTENT)(int m, FP_CMP f1, FP_HASH f2)	\
: BasePtrHashTab(m)														\
{ f_cmp = f1; f_hash = f2; }											\
																		\
CONTENT *PtrHashTab(CONTENT)::find(CONTENT *k) const					\
{ return (CONTENT *) pfind(&k); }

#endif // PHASHTAB_H
