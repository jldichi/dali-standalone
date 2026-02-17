/********************************************************************
* Copyright (c) 1992 InterSoft Co.  All Rights Reserved
* THIS IS UNPUBLISHED PROPRIETARY SOURCE CODE OF InterSoft Co.
* The copyright notice above does not evidence any
* actual or intended publication of such source code.
*
* $Id: pobject.h,v 1.1.1.1 1996/04/16 18:51:48 gustavof Exp $
*
* DESCRIPTION
* Class definitions for Persistent Object type.
*********************************************************************/

#ifndef POBJECT_H
#define POBJECT_H

#include <ifound.h>

#ifdef __GNUC__
#pragma interface
#endif

#include <essentia/objectid.h>

class gtype(PtrHashTab, PObject);

class PObject {

// private:

	ObjectId oId;

	// indicates that this PObject is volatile, no entry in hash table
	bool volat;

	static gtype(PtrHashTab,PObject) *hashtab;
	static Int hash(const void *node, Int modulo);
	static Int cmp(const void *left, const void *right);
    
    PObject(const PObject&)  ;
    void operator=(const PObject&)  ;
    
public:

	// Constructors & destructors
	PObject(UnInt cid, UnInt iid, bool vol = false);
	PObject(const ObjectId &oid, bool vol = false);
	virtual ~PObject();
	UnInt classId() const		{ return oId.classId(); }
	UnInt instId() const		{ return oId.instId(); }
	const ObjectId &oid() const { return oId; }
	void setOid(const ObjectId &oid);
	
	static PObject *find(const ObjectId &oid);
};

#endif // POBJECT
