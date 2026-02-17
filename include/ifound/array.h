/********************************************************************
* Copyright (c) 1991 InterSoft Co.  All Rights Reserved
* THIS IS UNPUBLISHED PROPRIETARY SOURCE CODE OF InterSoft Co.
* The copyright notice above does not evidence any
* actual or intended publication of such source code.
*
* $Id: array.h,v 1.1.1.1 1996/04/16 18:51:31 gustavof Exp $
*
* DESCRIPTION
* Interface to generic Arrays an Sorted Arrays
*********************************************************************/
//This file was documented by Oscar (/usr2/acct/oscar/doc/array/array.doc)
#ifndef	ARRAY_H
#define	ARRAY_H

#include <ifound.h>

#ifdef __GNUC__
#pragma interface
#endif

#include <ifound/template.h>

#define Array(CONTENT)		name2(CONTENT, Array)


#define	Arraydeclare(CONTENT)										\
																	\
class Array(CONTENT) {												\
																	\
	  Array(CONTENT)(const Array(CONTENT) &);		                \
	  void operator=(const Array(CONTENT) &);						\
																	\
protected:															\
																	\
	CONTENT *cont;													\
	Int a_dim, a_base, a_limit;										\
																	\
public:																\
																	\
	explicit Array(CONTENT)(Int n)									\
	{ cont = new CONTENT[a_dim = n]; a_base = a_limit = 0; }		\
	Array(CONTENT)(Int n, Int b)									\
	{ cont = new CONTENT[a_dim = n]; a_base = b; a_limit = 	0; }	\
	~Array(CONTENT)() { delete [] cont; }							\
																	\
	Int dim() { return a_dim; }										\
	Int base() { return a_base; }									\
	Int limit() { return a_limit; }									\
																	\
	CONTENT &operator[](Int n)										\
	{																\
		if (n < a_base || n >= a_base + a_dim)						\
			error(libMsg("OUTOFBOUNDS"));							\
		if (n - a_base > a_limit) a_limit = n - a_base;				\
		return cont[n - a_base];									\
	}																\
																	\
	CONTENT &elem(Int n) { return cont[n - a_base]; }				\
																	\
	const CONTENT &elem(Int n) const { return cont[n - a_base]; }	\
																	\
	void reAlloc(Int n)												\
	{																\
		CONTENT *n_cont = new CONTENT[n];							\
		for (Int j = 0; j < ((a_dim < n) ? a_dim : n); j++)			\
			n_cont[j] = cont[j];									\
		delete [] cont;												\
		a_dim = n;													\
		cont = n_cont;												\
	}																\
																	\
};																	\

/*
	In the reAlloc function, we can NOT use the memcpy function
	and we must assign the string to increment the reference
	count, so the strings are not destroyed when we delete cont,
	also when we new n_cont CONTENTS constructor is called, so if
	we memcpy something over it, and the constructor has malloced
	dynamic memory (Ex: Strings), it is forever lost in 
	hyper-space.
*/



#define SortedArray(CONTENT)		name2(CONTENT, SortedArray)

#define	SortedArraydeclare(CONTENT)									\
																	\
Arraydeclare(CONTENT);												\
																	\
class SortedArray(CONTENT) : public Array(CONTENT) {				\
																	\
    SortedArray(CONTENT)(const SortedArray(CONTENT)&) ;		        \
	void operator=(const SortedArray(CONTENT)&) ;				    \
																	\
	friend Int f_cmp(const char *l, const char *r)					\
	{ return cmp(*(CONTENT*)l, *(CONTENT*)r); }						\
																	\
public:																\
																	\
	explicit SortedArray(CONTENT)(Int n) : Array(CONTENT)(n) {}		\
	~SortedArray(CONTENT)() { }										\
																	\
	const CONTENT &operator[](Int n)								\
	{ return (const CONTENT&)Array(CONTENT)::operator[](n); }		\
																	\
	CONTENT &elem(Int n) { return Array(CONTENT)::elem(n); }		\
																	\
	void add(const CONTENT &ae)										\
	{																\
		Int j = locate((const char*)&ae, (const char*)cont,			\
			(unsigned)a_limit, sizeof(CONTENT), (IFPCCPCCP)f_cmp);	\
		if (j < 0) {												\
			j = ~j;													\
			if (++a_limit >= a_dim) 								\
				Array(CONTENT)::reAlloc(a_dim + 5);					\
			for (Int k = a_limit; k > j; k--)						\
				cont[k] = cont[k - 1];								\
		}															\
		cont[j] = ae;												\
	}																\
																	\
	void del(Int n)													\
	{																\
		if (n < 0 || n >= a_limit)									\
			error(libMsg("OUTOFBOUNDS"));							\
		a_limit--;													\
		for (Int j = n; j < a_limit; j++)							\
			cont[j] = cont[j + 1];									\
	}																\
																	\
	Int find(const CONTENT &ae)										\
	{																\
		return locate((const char*)&ae, (const char*)cont,			\
			(unsigned)a_limit, sizeof(CONTENT), (IFPCCPCCP)f_cmp);	\
	}																\
																	\
};																	\

#endif // ARRAY_H
