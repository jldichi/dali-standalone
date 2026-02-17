/********************************************************************
* Copyright (c) 1992 InterSoft Co.  All Rights Reserved
* THIS IS UNPUBLISHED PROPRIETARY SOURCE CODE OF InterSoft Co.
* The copyright notice above does not evidence any
* actual or intended publication of such source code.
*
* $Id: dbinchk.h,v 1.1.1.1 1996/04/16 18:51:48 gustavof Exp $
*
* DESCRIPTION
* Class definitions for DbInCheck type.
*********************************************************************/

#ifndef DBINCHK_H
#define DBINCHK_H

#include <ifound.h>

#ifdef __GNUC__
#pragma interface
#endif

#include <essentia/incheck.h>

class DbTable;
class DbIndex;
class ObjectId;
class Connection;

class DbIntegrity : public Integrity
{

// private:

	const DbIndex *indp;

	DbIntegrity(const DbIntegrity &);

public:

	DbIntegrity();
	explicit DbIntegrity(Connection &c);
	explicit DbIntegrity(const ObjectId &oid, Int nk = 0, Int nd = 0,
			bool neg = false);
	~DbIntegrity();
	void setKey(DbTable *tbf, DbTable *tbt);
	const DbIndex *index() const { return indp; }
	String toString(Int) const 	 { return NULL_STRING; }
};

class DbInList : public InList {

// private:

	DbInList(const DbInList &);

public:

	DbInList();
	explicit DbInList(Connection &c);
	explicit DbInList(Int nv, bool neg = false);
	~DbInList();

	String toString(Int) const 	 { return NULL_STRING; }
};

#endif
