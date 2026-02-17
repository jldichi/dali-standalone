/********************************************************************
* Copyright (c) 1991 InterSoft Co.  All Rights Reserved
* THIS IS UNPUBLISHED PROPRIETARY SOURCE CODE OF InterSoft Co.
* The copyright notice above does not evidence any
* actual or intended publication of such source code.
*
* $Id: strnull.cc,v 1.2 1998/04/24 16:50:27 guiller Exp $
*
* DESCRIPTION
* Null String data
*********************************************************************/

#define NULL_STRING_DEF

#include <ifound/str.h>

struct StrAuxiliar {

	srep *x;
	// char x[sizeof(String)];

};

extern "C"  {
	DLLDEPEND StrAuxiliar NULL_STRING = { NULL };
}
