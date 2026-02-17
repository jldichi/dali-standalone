/********************************************************************
* Copyright (c) 1991 InterSoft Co.  All Rights Reserved
* THIS IS UNPUBLISHED PROPRIETARY SOURCE CODE OF InterSoft Co.
* The copyright notice above does not evidence any
* actual or intended publication of such source code.
*
* $Id: types.cc,v 1.1.1.1 1996/04/16 18:51:55 gustavof Exp $
*
* DESCRIPTION
* Miscelaneous
*********************************************************************/

#include <ifound/itypes.h>

#define NTYPES 		11

static String *tyNames[NTYPES] = { NULL };

const String &typeName(Int t)
{
	if (t < 0 || t >= NTYPES) return NULL_STRING;

	if (tyNames[0] == NULL) {
		tyNames[TY_NUMERIC] = tr_new String("Num");
		tyNames[TY_STRING]	= tr_new String("String");
		tyNames[TY_DATE]	= tr_new String("Date");
		tyNames[TY_TIME]	= tr_new String("Time");
		tyNames[TY_FLOAT]	= tyNames[TY_NUMERIC];
		tyNames[TY_BOOL]	= tr_new String("Bool");
		tyNames[TY_ORDPAIR]	= tr_new String("OrdPair");
		tyNames[TY_INSTID]	= tr_new String("InstId");
		tyNames[TY_PTR]		= tr_new String("Pointer");
		tyNames[TY_BINARY]	= tr_new String("Binary");
		tyNames[TY_INTEGER]	= tr_new String("Integer");
	}
	return *tyNames[t];
}
