/********************************************************************
* Copyright (c) 1994 InterSoft Co.  All Rights Reserved
* THIS IS UNPUBLISHED PROPRIETARY SOURCE CODE OF InterSoft Co.
* The copyright notice above does not evidence any
* actual or intended publication of such source code.
*
* $Id: namespac.h,v 1.3 2002/03/15 20:55:15 hardaiz Exp $
*
*
*********************************************************************/

#ifndef NAMESPAC_H
#define NAMESPAC_H

#include <local/typeinfo.h>

#define NameSpace(CONTENT) name2(CONTENT, NameSpace)

#define NameSpacedeclare(CONTENT)						\
														\
class NameSpace(CONTENT) {								\
														\
private:												\
														\
	static NameSpace(CONTENT) *first;					\
	NameSpace(CONTENT) *next;							\
														\
protected:												\
														\
	NameSpace(CONTENT)();                               \
														\
public:													\
														\
	virtual CONTENT *generate() = 0;					\
	virtual TypeInfo &type() = 0;						\
														\
	static void init() { TypeInfo::init(); }			\
	static void clean();								\
	static Int nofClasses();							\
	static NameSpace(CONTENT) *nameSpace(const String &);\
														\
};

#define NameSpaceimplement(CONTENT)							\
															\
NameSpace(CONTENT) *NameSpace(CONTENT)::first = NULL;		\
															\
NameSpace(CONTENT)::NameSpace(CONTENT)()					\
{															\
	next = first; first = this;								\
}															\
															\
NameSpace(CONTENT) *NameSpace(CONTENT)::nameSpace(const String &cname)\
{															\
	NameSpace(CONTENT) *current;		  					\
															\
	String cn = "NS";	\
 cn+=cname;									\
															\
	for (current = first;									\
		 current && cn != (current->type()).name();			\
		 current = current->next)							\
		 ;													\
															\
	return current;											\
}															\
															\
void NameSpace(CONTENT)::clean()							\
{															\
	NameSpace(CONTENT) *d = first, *n;						\
															\
	while (d) {												\
		n = d->next;										\
		delete d;											\
		d = n;												\
	}														\
}															\
															\
Int NameSpace(CONTENT)::nofClasses()						\
{															\
	Int n = 0;							  					\
	NameSpace(CONTENT) *current;		  					\
															\
	for (current = first;									\
		 current;											\
		 current = current->next, n++)						\
		 ;													\
															\
	return n;												\
}

#define CatalogClass(cname, ty)								\
															\
declare(NameSpace, ty)										\
															\
class paste(NS, cname) : public NameSpace(ty) {				\
															\
public:														\
															\
	paste(NS,cname)();										\
	ty *generate()		{ return new cname(); }				\
	static void chain() { (void) new paste(NS, cname)(); }	\
	TypeInfo &type();										\
															\
};															\
															\
paste(NS, cname)::paste(NS, cname)() { }					\
															\
DefineClass(paste(NS, cname), paste(NS, cname)::chain);

#endif // NAMESPAC_H
