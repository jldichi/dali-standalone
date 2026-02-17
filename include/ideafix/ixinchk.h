/********************************************************************
* Copyright (c) 1992 InterSoft Co.  All Rights Reserved
* THIS IS UNPUBLISHED PROPRIETARY SOURCE CODE OF InterSoft Co.
* The copyright notice above does not evidence any
* actual or intended publication of such source code.
*
* $Id: ixinchk.h,v 1.1.1.1 1996/04/16 18:51:37 gustavof Exp $
*
* DESCRIPTION
* Class definitions for IxInCheck type.
*********************************************************************/

#ifndef IXINCHK_H
#define IXINCHK_H

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
struct s_schema;

class IxIntegrity : public Integrity {

	friend void convertIx(InCheck &ic, struct s_schema *sp);
	friend void sendConvertedIx(InCheck &ic, Connection &c);

public:

	IxIntegrity();
	IxIntegrity(const ObjectId &oid, Int nk = 0, Int nd = 0,
			bool neg = false);
	IxIntegrity(Connection &c);						/* ESDBMS */
	~IxIntegrity();

	void correctRefs(dbtable tbl, dbtable newtbl);
	void setKey(dbindex indx, int row, dbtable tbl, dbtable atab);
	bool valid(dbfield fld, int row, dbtable tbl_alias);
	String toString(Int) const;
	bool referenced(dbtable tab) const;
	void send(Connection &c);						/* ESDBMS */
};

class IxInList : public InList {

	friend void convertIx(InCheck &ic);
	friend void sendConvertedIx(InCheck &ic, Connection &c);

public:

	IxInList();
	IxInList(Int nv, bool neg = false);
	IxInList(Connection &c);					/* ESDBMS */
	~IxInList();

	String toString(Int t) const;
	Int find(const AnyValue &av) const;
	void send(Connection &c);					/* ESDBMS */
};

#if HAVE_PRAGMA_PACK
#	pragma pack()
#endif

#endif
