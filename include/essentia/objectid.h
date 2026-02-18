/********************************************************************
* Copyright (c) 1992 InterSoft Co.  All Rights Reserved
* THIS IS UNPUBLISHED PROPRIETARY SOURCE CODE OF InterSoft Co.
* The copyright notice above does not evidence any
* actual or intended publication of such source code.
*
* $Id: objectid.h,v 1.1.1.1 1996/04/16 18:51:48 gustavof Exp $
*
* DESCRIPTION
* Class definitions for Persistent Object type.
*********************************************************************/

#ifndef OBJECTID_H
#define OBJECTID_H

#include <ifound.h>

#ifdef __GNUC__
#	pragma interface
#endif

class PObject;
class String;
class Connection;

class ObjectId {

protected:

	UnInt cId;
	UnInt iId;

public:

	inline explicit ObjectId(UnInt cid = 0, UnInt iid = 0);
	inline ObjectId(const ObjectId &ptr);
	explicit ObjectId(const String &s);
     
    ObjectId &operator=(const ObjectId &oi); 
    
	inline UnInt classId() const;
	inline void setClassId(UnInt cid);
	inline UnInt instId() const;
	inline void setInstId(UnInt iid);

	inline bool operator==(const ObjectId &p) const;
	inline bool operator!=(const ObjectId &p) const;
	friend Int cmp(const ObjectId &p1, const ObjectId &p2);
	PObject *operator*() const;
	PObject *operator->() const;
	inline bool isNull() const;
	inline operator void *();
	inline bool operator!() const;

	void store(char *&buff) const;
	void store(FILE *fp) const;
	void load(const char *&buff);
	void load(FILE *fp);

	static ObjectId classObjectId(UnInt cNum);

	friend ObjectId toObjectId(const String &s);
	friend Connection &operator<<(Connection &c, const ObjectId &oid);
	friend Connection &operator>>(Connection &c, ObjectId &oid);

	inline static Int storeLength();
};

// DALI-STANDALONE: Defined outside class for ::cmp qualified lookup in GCC 13
inline Int cmp(const ObjectId &p1, const ObjectId &p2)
{ return p1.cId != p2.cId ? p1.cId - p2.cId : p1.iId - p2.iId; }

#include <essentia/objectid.icc>

#endif // OBJECTID_H
