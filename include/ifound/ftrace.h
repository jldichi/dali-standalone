/********************************************************************
* Copyright (c) 1995 InterSoft Co.  All Rights Reserved
* THIS IS UNPUBLISHED PROPRIETARY SOURCE CODE OF InterSoft Co.
* The copyright notice above does not evidence any
* actual or intended publication of such source code.
*
* $Id: ftrace.h,v 1.1.1.1 1996/04/16 18:51:34 gustavof Exp $
*
* DESCRIPTION: Main InterSoft Foundation header file.
*********************************************************************/

#ifndef FTRACE_H
#define FTRACE_H

#if defined(__cplusplus) && !defined(NOFTRACE)

class String;

class FunctionTrace {

	String &func;
	String &msg;

	static Int level;
	
public:

	FunctionTrace(const String &f, const String &m);
	~FunctionTrace();

};

#endif

#endif
