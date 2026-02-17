/********************************************************************
* Copyright (c) 1992 InterSoft Co.  All Rights Reserved
* THIS IS UNPUBLISHED PROPRIETARY SOURCE CODE OF InterSoft Co.
* The copyright notice above does not evidence any
* actual or intended publication of such source code.
*
* $Id: inconn.cc,v 1.1.1.1 1996/04/16 18:51:56 gustavof Exp $
*
* DESCRIPTION
*********************************************************************/

#include <ifound/parray.h>
#include <essentia/incheck.h>
#include <essentia/connect.h>
#include <ifound/str.h>

declare(PtrArray,AnyValue);
declare(PtrArrayCursor,AnyValue);

// InCheck class

InCheck::InCheck(Connection &c)
:	neg(BOOL_NULL),
	keyValues(*tr_new PtrArray(AnyValue)(0)),
	dspFields(*tr_new PtrArray(AnyValue)(0))
{
	Int nkv, ndf, n;
	c >> n >> nkv >> ndf;
	neg = bool(n);
	keyValues.reAlloc(nkv);
	keyValues.clear();
	dspFields.reAlloc(ndf);
	dspFields.clear();

	AnyValue tmp;
	Int j;

	for (j = 0; j < nkv; j++) {
		c >> tmp;
		keyValues.elem(j) = tr_new AnyValue(tmp);
	}

	for (j = 0; j < ndf; j++) {
		c >> tmp;
		dspFields.elem(j) = tr_new AnyValue(tmp);
	}
}

InCheckType InCheck::receive(Connection &c)
{
	Int aux;
	c >> aux;
	return InCheckType(aux);
}

void InCheck::sendEmpty(Connection &c)
{
	c << Int(IC_EMPTY);
}

// end of InCheck class

// Integrity class

Integrity::Integrity(Connection &c)
:	InCheck(c)
{
	c >> ind;
}

#if 0
void Integrity::send(Connection &c)
{
	InCheck::send(c);
	c << ind;
}
#endif

// end of Integrity class


// InList class

InList::InList(Connection &c)
:	InCheck(c)
{ }

// end of InList class

