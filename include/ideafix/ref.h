/********************************************************************
* Copyright (c) 1995 InterSoft Co.  All Rights Reserved
* THIS IS UNPUBLISHED PROPRIETARY SOURCE CODE OF InterSoft Co.
* The copyright notice above does not evidence any
* actual or intended publication of such source code.
*
* $Id: ref.h,v 1.7 2007/03/26 21:33:04 hardaiz Exp $
*
* DESCRIPTION
*********************************************************************/

#ifndef REF_H
#define REF_H

class Persistent_Object;

class Ref_Any {

public:

	Ref_Any();
	Ref_Any(const Ref_Any &);
	Ref_Any(Persistent_Object *);
	~Ref_Any();
	
	Ref_Any &operator=(const Ref_Any &);
	Ref_Any &operator=(Persistent_Object *);

	void clear();
	int is_null();
	void delete_object();
	
	friend int operator==(const Ref_Any &, const Ref_Any &);
	friend int operator==(const Ref_Any &, const Persistent_Object *);
	friend int operator==(const Persistent_Object *, const Ref_Any &);
	friend int operator!=(const Ref_Any &, const Ref_Any &);
	friend int operator!=(const Ref_Any &, const Persistent_Object *);
	friend int operator!=(const Persistent_Object *, const Ref_Any &);
};

template <class T> class Ref {

public:

	Ref();
	Ref(T *fromPtr);
	Ref(const Ref<T> &);
	Ref(const Ref_Any &);
	~Ref();
	
	operator Ref_Any() const;
	Ref<T> &operator=(T *);
	Ref<T> &operator=(const Ref<T> &);
	Ref<T> &operator=(const Ref_Any &);

	void clear();

	T *operator->() const;		// dereference the reference
	T &operator*() const;

	T *ptr() const;
	int is_null() const;
	void delete_object();		// delete referred object from memory
								// and the database
		
	// do these Refs and pointers refer to the same objects ?
	//friend int operator==<>(const Ref<T> &refL, const Ref<T> &refR);
#if defined(__NT__) || defined(hpux) || defined(Linux6) || defined(LinuxGcc3) || defined(LinuxGcc4) || defined(LinuxGcc5) || defined(LinuxGcc8)
	friend int operator==(const Ref<T> &refL, const Ref<T> &refR);
	friend int operator==(const Ref<T> &refL, const T *ptrR);
	friend int operator==(const T *ptrL, const Ref<T> &refR);
	friend int operator==(const Ref<T> &refL, const Ref_Any &anyR);
	friend int operator==(const Ref_Any &anyL, const Ref<T> &refR);
	friend int operator!=(const Ref<T> &refL, const Ref<T> &refR);
	friend int operator!=(const Ref<T> &refL, const T *ptrR);
	friend int operator!=(const T *ptrL, const Ref<T> &refR);
	friend int operator!=(const Ref<T> &refL, const Ref_Any &anyR);
	friend int operator!=(const Ref_Any &anyL, const Ref<T> &refR);
#else
	friend int operator==<>(const Ref<T> &refL, const Ref<T> &refR);
	friend int operator==<>(const Ref<T> &refL, const T *ptrR);
	friend int operator==<>(const T *ptrL, const Ref<T> &refR);
	friend int operator==<>(const Ref<T> &refL, const Ref_Any &anyR);
	friend int operator==<>(const Ref_Any &anyL, const Ref<T> &refR);
	friend int operator!=<>(const Ref<T> &refL, const Ref<T> &refR);
	friend int operator!=<>(const Ref<T> &refL, const T *ptrR);
	friend int operator!=<>(const T *ptrL, const Ref<T> &refR);
	friend int operator!=<>(const Ref<T> &refL, const Ref_Any &anyR);
	friend int operator!=<>(const Ref_Any &anyL, const Ref<T> &refR);
#endif
};

#endif
