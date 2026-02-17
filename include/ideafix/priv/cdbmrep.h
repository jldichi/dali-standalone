/********************************************************************
* Copyright (c) 1994 InterSoft Co.  All Rights Reserved
* THIS IS UNPUBLISHED PROPRIETARY SOURCE CODE OF InterSoft Co.
* The copyright notice above does not evidence any
* actual or intended publication of such source code.
*
* $Id: cdbmrep.h,v 1.1.1.1 1996/04/16 18:51:39 gustavof Exp $
*
* DESCRIPTION
*********************************************************************/

#include <ideafix.h>
#include <cfix.h>

inline int schToI(schema sch)
{
	return (int)((sch >> 16) & 0xff);
}

inline schema iToSch(int n)
{
	return (schema)(n << 16);
}

inline int tabToI(dbtable tab)
{
	return (int)(((tab >> 8) & 0xff)-1);
}

inline dbtable iToTab(int n)
{
	return (dbtable)((n+1) << 8);
}

inline int fldToI(dbfield fld)
{
	return (int)((fld & 0xff)-1);
}

inline dbfield iToFld(dbtable tab, int slot)
{
	return (dbfield)((tab) | (slot + 1));
}

inline int indToI(dbindex ind)
{
	return (int)(ind & 0xff);
}

inline int curToI(dbcursor cur)
{
	return (int)(cur & 0xff);
}

inline dbcursor iToCur(dbtable tab, int cur)
{
	return (dbcursor)(tab|cur);
}

inline dbindex iToInd(dbtable tab, int ind)
{
	return (dbindex)(tab|ind);
}

DataBase *findDataBase(Int n);
DbmObject *findObject(Int n);
DbmSet *findSet(Int n);
DbmMemberDescr *findMember(Int f);
DbmIndex *findIndex(Int n);
DbmCursor *findCursor(Int n);
Int oldToNewMode(Int mode);
DbmMemberDescr *findIndexMember(Int ind, Int fld);
AnyValue value(DbmMemberDescr *m, va_list &ap);

