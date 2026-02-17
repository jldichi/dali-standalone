/********************************************************************
* Copyright (c) 1995 InterSoft Co.  All Rights Reserved
* THIS IS UNPUBLISHED PROPRIETARY SOURCE CODE OF InterSoft Co.
* The copyright notice above does not evidence any
* actual or intended publication of such source code.
*
* $Id: ftrace.cc,v 1.1.1.1 1996/04/16 18:51:59 gustavof Exp $
*
* DESCRIPTION
*********************************************************************/

#ifndef NOFTRACE

#include <ifound.h>
#include <ifound/str.h>


Int FunctionTrace::level = 0;

FunctionTrace::FunctionTrace(const String &f, const String &m)
:	func(*tr_new String(f)), 
	msg(*tr_new String(m))
{ 
	for (Int i = 0; i < level; i++)
		fputc('\t', stderr);
	level++;
	fprintf(stderr, "Entering %s(%s)\n", toCharPtr(func), toCharPtr(msg));
}

FunctionTrace::~FunctionTrace()
{
	level--;
	for (Int i = 0; i < level; i++)
		fputc('\t', stderr);
	fprintf(stderr, "Leaving %s(%s)\n", toCharPtr(func), toCharPtr(msg));
	tr_delete(&func);
	tr_delete(&msg);
}

#endif
