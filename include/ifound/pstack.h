/********************************************************************
* Copyright (c) 1991 InterSoft Co.  All Rights Reserved
* THIS IS UNPUBLISHED PROPRIETARY SOURCE CODE OF InterSoft Co.
* The copyright notice above does not evidence any
* actual or intended publication of such source code.
*
* pstack.h,v 1.1.1.1 1996/04/16 18:51:34 gustavof Exp
*
* DESCRIPTION
* Interface to generic Pointer Stacks
*********************************************************************/

#ifndef	PSTACK_H
#define	PSTACK_H

#include <ifound/parray.h>

#ifdef __GNUC__
#pragma interface
#endif

#define PtrStack(CONTENT)		name2(CONTENT,PtrStack)

#define	PtrStackdeclare(CONTENT)									\
																	\
class PtrStack(CONTENT) : public BasePtrArray {						\
																	\
	int sp;															\
																	\
public:																\
																	\
	explicit PtrStack(CONTENT)(Int n = 0, Int chunk = ARRAY_REALLOC)\
			: BasePtrArray(n, chunk), sp(0) {}						\
																	\
	void clear();													\
	bool pop();														\
	bool push(CONTENT *elem);										\
	CONTENT *top() const;											\
	bool isEmpty() const;											\
																	\
};																	\
																	\
inline void PtrStack(CONTENT)::clear()								\
{																	\
	sp = 0;															\
}																	\
																	\
inline bool PtrStack(CONTENT)::pop()								\
{																	\
	return (sp > 0) ? sp--, true : false;							\
}																	\
																	\
inline bool PtrStack(CONTENT)::push(CONTENT *elem)				\
{																	\
	if (!isInRange(sp)) reAlloc(chunkSize);							\
																	\
	c[sp++] = elem;													\
	return true;													\
}																	\
																	\
inline CONTENT* PtrStack(CONTENT)::top() const						\
{																	\
	assert( sp > 0 );												\
	return (CONTENT*) c[sp - 1]; 									\
}																	\
																	\
inline bool PtrStack(CONTENT)::isEmpty() const						\
{																	\
	return (sp == 0);												\
}

#define	PtrStackimplement(CONTENT)

#endif
