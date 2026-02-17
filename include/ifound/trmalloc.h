/*
// --------------------------------------------------------------------
// Copyright InterSoft Argentina S.A.
// This is free software; you can redistribute it and/or modify
// it under the terms of the GNU General Public License as published by
// the Free Software Foundation; either version 2, or (at your option)
// any later version.
//
// $Id: trmalloc.h,v 1.2 1996/05/30 15:32:21 eduardoc Exp $
// --------------------------------------------------------------------
*/

#ifndef TRMALLOC_H
#define TRMALLOC_H

#ifdef DEBUG
#	define TRACE_MEMORY		1
#endif

#define HAVE_OP_VEC_NEW		1


#if defined(TRACE_MEMORY) && defined(__cplusplus)

class LeakTrace
	/* Object used to declare static objects to print memory usage
	   as late as possible */
{

	static LeakTrace *first_sd;

public:

	LeakTrace();
	~LeakTrace();

};

#endif /* defined(TRACE_MEMORY) && defined(__cplusplus) */

#ifdef TRACE_MEMORY

#	define idCheck()	_idCheck(__FILE__, __LINE__)

#	define useLightIdCheck(id)								\
															\
public:														\
															\
	enum { TYPE_IDENTIF = id };								\
	unsigned int typeIdentif;								\
															\
	void _idCheck(const char *f, int l) const				\
	{														\
		if (typeIdentif != (unsigned int) TYPE_IDENTIF)		\
			fprintf(stderr,"%s:%d idCheck failed!\n",f,l);	\
	}														\
															\
	void idStart()											\
	{														\
		typeIdentif = TYPE_IDENTIF;							\
		tr_set_mem_id(this, typeIdentif);					\
	}														\
															\
	void idStop()											\
	{														\
		idCheck();											\
		typeIdentif = 0xCACABACA;							\
	}														\
															\
private:													\
															\
	enum {}

	/* The enum is just to allow putting a ';' after the macro */

#else

#	define useLightIdCheck(id)								\
															\
protected:													\
															\
	void idCheck() const { }								\
	void idStart() { }										\
	void idStop() { }										\
															\
private:													\
															\
	enum {}

	/* The enum is just to allow putting a ';' after the macro */

#endif


#ifdef TRACE_MEMORY

#	define useIdCheck(id)									\
															\
public:														\
															\
	enum { TYPE_IDENTIF = id };								\
	unsigned int typeIdentif;								\
															\
	void _idCheck(const char *f, int l)	const				\
	{														\
		if (typeIdentif != (unsigned int) TYPE_IDENTIF)		\
			fprintf(stderr,"%s:%d idCheck failed!\n",f,l);	\
	}														\
															\
	void idStart()											\
	{														\
		typeIdentif = TYPE_IDENTIF;							\
		tr_set_mem_id(this, typeIdentif);					\
	}														\
															\
	void idStop()											\
	{														\
		idCheck();											\
		typeIdentif = 0xCACABACA;							\
	}														\
															\
private:													\
															\
	enum {}

	/* The enum is just to allow putting a ';' after the macro */

#else

#	define useIdCheck(id)									\
															\
	unsigned int typeIdentif;								\
															\
protected:													\
															\
	void idCheck() const { }								\
	void idStart() { }										\
	void idStop() { }										\
															\
private:													\
															\
	enum {}

	/* The enum is just to allow putting a ';' after the macro */

#endif


#ifdef TRACE_MEMORY
	enum {
		tr_unknown,
		tr_element,
		tr_vec
	};

#	ifdef __cplusplus
		extern "C" {
#	endif
		void _tr_mem_used_list(const char *f, int l);
		void _tr_mem_check(const char *f, int l);
		void _tr_push(void *m, const char *f, int l, int iv);
		void _tr_called_from(const char *f, int l, int iv);
		void *_tr_malloc(const char *f, int l, int iv, SizeT sz);
		void tr_set_mem_id(void *m, SizeT l);
		void tr_dont_free(SizeT l);
#	ifdef __cplusplus
		}

		void *operator new(SizeT, const char *f, int l);
#		if HAVE_OP_VEC_NEW
			void *operator new[](SizeT, const char *f, int l);
#		endif

#		define tr_new												\
			new(__FILE__, __LINE__)

#		define tr_delete(m)											\
			do {													\
				_tr_push(m, __FILE__, __LINE__, tr_element);		\
				delete m;											\
			} while (0)

#		define tr_delete_vec(m)										\
			do {													\
				_tr_push(m, __FILE__, __LINE__, tr_vec);			\
				delete [] m;										\
			} while (0)

#	endif

#	define tr_free(ptr)	\
		_tr_called_from(__FILE__, __LINE__, tr_element), free(ptr)

#	define tr_malloc(a) \
		_tr_malloc(__FILE__, __LINE__, tr_element, a)

#	define tr_mem_used_list() \
		_tr_mem_used_list(__FILE__, __LINE__)

#	define tr_mem_check() \
		_tr_mem_check(__FILE__, __LINE__)

#else
#	define tr_new				new
#	define tr_delete(p)			delete p
#	define tr_delete_vec(p)		delete [] p
#	define tr_free(ptr)			free(ptr)
#	define tr_malloc(a)			malloc(a)
#endif

#endif
