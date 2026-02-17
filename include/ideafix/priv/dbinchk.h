/********************************************************************
* Copyright (c) 1994 InterSoft Co.  All Rights Reserved
* THIS IS UNPUBLISHED PROPRIETARY SOURCE CODE OF InterSoft Co.
* The copyright notice above does not evidence any
* actual or intended publication of such source code.
*
* $Id: dbinchk.h,v 1.1.1.1 1996/04/16 18:51:39 gustavof Exp $
*
* DESCRIPTION
* Class definitions for DbInCheck type.
*********************************************************************/

#ifndef DBINCHK_H
#define DBINCHK_H

#include <essentia/incheck.h>
#include <cfix.h>

#if HAVE_PRAGMA_PACK
#	pragma pack(4)
#endif

#ifdef private
#	undef private
#endif

class Connection;
class ObjectId;
class DbEssSet;
class DbSetRep;
class DbIndexRep;
class DbMemberRep;
class DbObjectRep;

DbSetRep *getDbSetRep(Int db, Int set);
DbObjectRep *getDbObjectRep(Int db, Int obj);
DbSetRep *getDbSetRep(const ObjectId &oid);
DbMemberRep *getDbMemberRep(const ObjectId &oid);
DbIndexRep *getDbIndexRep(const ObjectId &oid);
DbMemberRep *getDbMemberRep(Int memberDb, Int memberSet, Int memberNo);

DbEssSet *getDbEssSet(const ObjectId &oid);

class DbIntegrity : public Integrity {

	friend class DbEssSet;
	friend void sendConvertedDb(InCheck &ic, Connection &c);

public:

	DbIntegrity();
	DbIntegrity(const ObjectId &oid, Int nk = 0, Int nd = 0,
				 bool neg = false);
	DbIntegrity(Connection &c);						// ESDBMS
	~DbIntegrity();

	static bool referenced(InCheck *ic, Int fullSetId);

	void setKey(Int indDb, Int indSet, Int ind, Int row, Int setDb, 
				Int set, Int aSet);
	void correctRefs(Int oldSet, Int newSet);
//	bool valid(dbfield fld, int row, dbtable tbl_alias);
	String toString(Int) const;
	bool referenced(Int fullSetId) const;
	void send(Connection &c);							// ESDBMS

	Int indId();
	Int fullSetId();
	Int dspFieldFullSetId(Int fld);
	Int dspFieldNo(Int fld);
};

class DbInList : public InList {

	friend class DbEssSet;
	friend void sendConvertedDb(InCheck &ic, Connection &c);

public:

	DbInList();
	DbInList(Int nv, bool neg = false);
	DbInList(Connection &c);							// ESDBMS
	~DbInList();

	String toString(Int t) const;
	Int find(const AnyValue &av) const;
	void send(Connection &c);							// ESDBMS
};

#if HAVE_PRAGMA_PACK
#	pragma pack()
#endif

#endif
