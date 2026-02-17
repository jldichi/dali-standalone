/********************************************************************
* Copyright (c) 1995 InterSoft Co.  All Rights Reserved
* THIS IS UNPUBLISHED PROPRIETARY SOURCE CODE OF InterSoft Co.
* The copyright notice above does not evidence any
* actual or intended publication of such source code.
*
* $Id: anynull.cc,v 1.2 1998/04/24 16:25:24 mauro Exp $
*
* DESCRIPTION
*
* Null AnyValue data
*
*********************************************************************/

#define NULL_ANYVALUE_DEF

#include <ifound/anyvalue.h>

struct AnyAuxiliar {

	short ty;
	char x[sizeof(AnyValue)-sizeof(short)];

};

extern "C"  {
	DLLDEPEND AnyAuxiliar NULL_ANYVALUE = { ERR };
}
