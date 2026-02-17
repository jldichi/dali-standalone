/*
// --------------------------------------------------------------------
// Copyright (c) 1995 InterSoft Co.  All Rights Reserved
// THIS IS UNPUBLISHED PROPRIETARY SOURCE CODE OF InterSoft Co.
// The copyright notice above does not evidence any actual or intended 
// publication of such source code.
//
// $Id: leaktrce.h,v 1.1.1.1 1996/04/16 18:51:34 gustavof Exp $
//
// Description:
// --------------------------------------------------------------------
*/

#ifndef LEAKTRCE_H
#define LEAKTRCE_H

#include <ifound/osdep.h>

#ifdef __cplusplus
class LeakTrace
	// Object used to declare static objects to print memory usage
	// as late as possible
{

	static LeakTrace *first_sd;

public:

	LeakTrace();
	~LeakTrace();

};

#endif

#ifdef ISDEBUG
	enum {
		ix_unknown,
		ix_element,
		ix_vec
	};

#	ifdef __cplusplus
		extern "C" {
#	endif
		void malloc_print_used_list();
		void malloc_push(void *m, const char *f, int l, int iv);
		void malloc_called_from(const char *f, int l, int iv);
		void *malloc_cf(const char *f, int l, int iv, SizeT sz);
		void malloc_set_id(void *m, SizeT l);
		void malloc_dont_free(SizeT l);
		void malloc_check();
#	ifdef __cplusplus
		}

		void *operator new(SizeT, const char *f, int l);
#		if HAVE_OP_VEC_NEW
			void *operator new[](SizeT, const char *f, int l);
#		endif

#		define ix_new												\
			new(__FILE__, __LINE__)

#		define ix_delete(m)											\
			do {													\
				malloc_push(m, __FILE__, __LINE__, ix_element);		\
				delete m;											\
			} while (0)

#		define ix_delete_vec(m)										\
			do {													\
				malloc_push(m, __FILE__, __LINE__, ix_vec);			\
				delete [] m;										\
			} while (0)

#	endif

#	define ix_free(ptr)	\
		malloc_called_from(__FILE__, __LINE__, ix_element), free(ptr)

#	define ix_malloc(a) \
		malloc_cf(__FILE__, __LINE__, ix_element, a)

#else
#	define ix_new				new
#	define ix_delete(p)			delete p
#	define ix_delete_vec(p)		delete [] p
#	define ix_free(ptr)			free(ptr)
#	define ix_malloc(a)			malloc(a)
#endif

#endif
