// --------------------------------------------------------------------
// Copyright (c) 1995 InterSoft Co.  All Rights Reserved
// THIS IS UNPUBLISHED PROPRIETARY SOURCE CODE OF InterSoft Co.
// The copyright notice above does not evidence any actual or intended 
// publication of such source code.
//
// $Id: parray.h,v 1.1.1.1 1996/04/16 18:51:33 gustavof Exp $
//
// Description:
// Interface to generic Pointer Arrays
// --------------------------------------------------------------------

#ifndef	PARRAY_H
#define	PARRAY_H

#include <ifound.h>

#ifdef __GNUC__
#	pragma interface
#endif

#include <ifound/functype.h>
#include <ifound/template.h>

#define ARRAY_REALLOC	(8)

class BasePtrArray {

	// to prevent its uses
	void operator=(const BasePtrArray &);
	BasePtrArray(const BasePtrArray &);

protected:
	Int a_dim;
	Int chunkSize;
	void **c;

	BasePtrArray(Int n, Int nRealloc);
	~BasePtrArray();
	void * &elemTest(Int n);

public:

	Int dim() const { return a_dim; }
	void reAlloc(Int n);
	Int add(const void *element);
	Int del(const void *element);
	Int find(const void *element);
	void ins(Int n, const void *element);
	void del(Int n);
	bool isInRange(Int n) const { return bool(n >= 0 && n < a_dim); }
	bool isValid(Int n) { return bool(isInRange(n) && c[n]); }
	void clear();
	Int nextFree() const;
	void sort(IFPVCPVCP sortFunc);
	int find(const void *el, IFPVCPVCP cmpFunc);

};

class BasePtrArrayScanner {
protected:

	void *const *base;
	void *const *curr;
	void *const *last;

	void scanUp_();
	void scanDown_();

public:
	BasePtrArrayScanner() {};

	void goToFirst()
		{ curr=base; scanUp_(); }
	void goToLast()
		{ curr=last-1; scanDown_(); }
	void goTo(Int offs)
		{ curr=base+offs; scanUp_(); }

	operator bool() const
		{ return (curr < last && curr >= base); }
	bool operator!() const
		{ return !bool(*this); }
	Int index()
		{ return curr-base; }

	void operator+=(Int offs)
		{ curr += offs; scanUp_(); }
	void operator-=(Int offs)
		{ curr -= offs; scanDown_(); }
	void operator++()
		{ curr++; scanUp_(); }
	void operator--()
		{ curr--; scanDown_(); }
/*	void *operator->()
		{ return *curr; }
	void *operator*()
		{ return *curr; }
*/
};

#define PtrArray(CONTENT)		name2(CONTENT,PtrArray)


#define	PtrArraydeclare(CONTENT)									\
																	\
class PtrArray(CONTENT) : public BasePtrArray {						\
																	\
	typedef BasePtrArray inherited;									\
																	\
public :															\
	explicit PtrArray(CONTENT)(Int n = 0, Int chunk = ARRAY_REALLOC)\
			: BasePtrArray(n, chunk) {}								\
																	\
	CONTENT *&operator[](Int n)										\
		{ return (CONTENT * &) elemTest(n); }						\
																	\
	CONTENT *&elem(Int n) const										\
		{ assert(isInRange(n)); return (CONTENT * &) c[n]; }		\
																	\
	Int add(const CONTENT *element)									\
		{ return inherited::add(element); }							\
																	\
	Int del(const CONTENT *element)									\
		{ return inherited::del(element); }							\
																	\
	void del(Int n)													\
		{ inherited::del(n); }										\
																	\
	Int find(const CONTENT *element)								\
		{ return inherited::find(element); }						\
																	\
	void ins(Int n, const CONTENT *element)							\
		{ inherited::ins(n, element); }								\
																	\
	int find(const CONTENT *el, IFPVCPVCP cmpFunc)					\
		{ return inherited::find(el, cmpFunc); }					\
};

#define PtrArrayCursor(CONTENT)		name2(CONTENT,PtrArrayCursor)

struct GoToLast {};

#define PtrArrayCursordeclare(CONTENT)								\
																	\
class PtrArrayCursor(CONTENT) {										\
																	\
	CONTENT **base;													\
	CONTENT **curr;													\
	CONTENT **last;													\
																	\
public:																\
	explicit PtrArrayCursor(CONTENT)(PtrArray(CONTENT) &pa)			\
	{ assign(pa); }													\
	explicit PtrArrayCursor(CONTENT)(PtrArray(CONTENT) &pa, 		\
			const GoToLast&)										\
	{ assign(pa); goToLast(); }										\
	PtrArrayCursor(CONTENT)(PtrArray(CONTENT) &pa, Int from) 		\
	{ assign(pa,from); }											\
	PtrArrayCursor(CONTENT)(PtrArray(CONTENT) &pa, Int from,		\
			const GoToLast&)								 		\
	{ assign(pa,from); goToLast(); }								\
	PtrArrayCursor(CONTENT)(PtrArray(CONTENT) &pa, Int from, Int to)\
	{ assign(pa,from,to); }											\
	PtrArrayCursor(CONTENT)(PtrArray(CONTENT) &pa, Int from, Int to,\
			const GoToLast&)										\
	{ assign(pa,from,to); goToLast(); }								\
	PtrArrayCursor(CONTENT)()										\
	{ reset(); }													\
	PtrArrayCursor(CONTENT) &operator=(PtrArray(CONTENT) &pa)		\
	{ assign(pa); return *this; }									\
	void assign(PtrArray(CONTENT) &pa)								\
	{																\
		if (pa.dim() == 0)											\
			reset();												\
		else {														\
			base = curr = &pa.elem(0);								\
			last = base + pa.dim();									\
		}															\
	}																\
	void assign(PtrArray(CONTENT) &pa, Int from)					\
	{																\
		if (pa.dim() == 0)											\
			reset();												\
		else {														\
			base = &pa.elem(0);										\
			last = base + pa.dim();									\
			curr = base += from;									\
		}															\
	}																\
	void assign(PtrArray(CONTENT) &pa, Int from, Int to)			\
	{																\
		if (pa.dim() == 0)											\
			reset();												\
		else {														\
			base = &pa.elem(0);										\
			last = base + (to != INT_MAX ? to + 1 : pa.dim());		\
			curr = base += from;									\
		}															\
	}																\
	void reset()													\
	{ base = curr = last = NULL;}									\
	void goToFirst()												\
	{ curr = base; }												\
	void goToLast()													\
	{ if (base) curr = last - 1; }									\
	void goTo(Int offs)												\
	{ if (base) curr = base + offs; }								\
	Int index() const												\
	{ return curr - base; }											\
	operator bool()	const											\
	{ return (curr < last && curr >= base); }						\
	bool operator!() const											\
	{ return !bool(*this); }										\
	PtrArrayCursor(CONTENT) &operator+=(Int offs)					\
	{ if (base) curr += offs; return *this; }						\
	PtrArrayCursor(CONTENT) &operator-=(Int offs)					\
	{ if (base) curr -= offs; return *this; }						\
	PtrArrayCursor(CONTENT) &operator++()							\
	{ if (base) curr++; return *this; }								\
	PtrArrayCursor(CONTENT) &operator--()							\
	{ if (base) curr--; return *this; }								\
	CONTENT *operator->()											\
	{ return *curr; }												\
	CONTENT *&operator*()											\
	{ return *curr; }												\
};


#define PtrArrayScanner(CONTENT)     name2(CONTENT,PtrArrayScanner)

#define PtrArrayScannerdeclare(CONTENT)								\
																	\
class PtrArrayScanner(CONTENT) : public BasePtrArrayScanner {		\
public:																\
	PtrArrayScanner(CONTENT)()										\
		{ base = curr = last = NULL; }								\
	PtrArrayScanner(CONTENT) &operator=(PtrArray(CONTENT) &pa)		\
		{															\
			if ( pa.dim()==0 )										\
				base = curr = last = NULL;							\
			else													\
			{														\
				curr = base = (void*const*)&pa.elem(0);				\
				last = base + pa.dim();								\
				scanUp_();											\
			}														\
			return *this;											\
		}															\
	PtrArrayScanner(CONTENT)(PtrArray(CONTENT) &pa) 				\
		{															\
			if ( pa.dim()==0 )										\
				base = curr = last = NULL;							\
			else													\
			{														\
				curr = base = (void*const*)&pa.elem(0);				\
				last = base + pa.dim();								\
				scanUp_();											\
			}														\
		}															\
	PtrArrayScanner(CONTENT)(PtrArray(CONTENT) &pa, 				\
						Int from, Int to = INT_MAX)					\
		{															\
			if ( pa.dim()==0 )										\
				base = curr = last = NULL;							\
			else													\
			{														\
				base = (void*const*)&pa.elem(0);					\
				last = base + (to != INT_MAX ? to + 1 : pa.dim());	\
				curr = (base += from);								\
				scanUp_();											\
			}														\
		}															\
	CONTENT *operator->()											\
	{ return (CONTENT*)*curr; }										\
	CONTENT *operator*()											\
	{ return (CONTENT*)*curr; }										\
};

// Empty implementations
#define	PtrArrayimplement(CONTENT)
#define	PtrArrayCursorimplement(CONTENT)
#define	PtrArrayScannerimplement(CONTENT)

#endif // PARRAY_H
