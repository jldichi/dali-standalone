/********************************************************************
* Copyright (c) 1993 InterSoft Co.  All Rights Reserved
* THIS IS UNPUBLISHED PROPRIETARY SOURCE CODE OF InterSoft Co.
* The copyright notice above does not evidence any
* actual or intended publication of such source code.
*
* $Id: bobjid.h,v 1.1.1.1 1996/04/16 18:51:49 gustavof Exp $
*
* DESCRIPTION
*********************************************************************/

#ifndef BOBJID_H
#define BOBJID_H

#include <ifound.h>

class Connection;

//
//	Class BinaryObjId
//
class BinaryObjId {

	Int cid_, iid_, fid_;
	Int btype_;
	Int fld_;

public:                

	BinaryObjId();                           
	BinaryObjId(const String &s);
	BinaryObjId(Int cid, Int iid, Int fid);

	void load(const char *&buff);
	void store(char *&buff) const;

	Int& fld();		// get & set
	Int& btype();	// get & set
	String path(const String &svname) const;	

	friend String toString(const BinaryObjId &bobjid);
	friend bool operator!=(BinaryObjId &b1, BinaryObjId &b2);
	friend bool operator==(BinaryObjId &b1, BinaryObjId &b2);
	friend Connection& operator>>(Connection &conn, BinaryObjId &bobjid);
	friend Connection& operator<<(Connection &conn, const BinaryObjId &bobjid);

	static Int storeLength();

};

inline Int& BinaryObjId::fld()
{
	return fld_;
}

inline Int& BinaryObjId::btype()
{
	return btype_;
}

#endif

