/********************************************************************
* Copyright (c) 1992 InterSoft Co.  All Rights Reserved
* THIS IS UNPUBLISHED PROPRIETARY SOURCE CODE OF InterSoft Co.
* The copyright notice above does not evidence any
* actual or intended publication of such source code.
*
* $Id: typeinfo.h,v 1.1.1.1 1996/04/16 18:51:49 gustavof Exp $
*
* DESCRIPTION
* Class definitions for type information.
*********************************************************************/

#ifndef TYPEINFO_H
#define TYPEINFO_H

#ifdef __GNUC__
#pragma interface
#endif

#include <ifound.h>

class TypeInfo {

public :

	TypeInfo(const char *name, FP init_code);
	const char *name() { return _name; }
	static void init();
	
// private:

	const char *_name;
	FP _init_code;
	TypeInfo *next;
};

#define DefineClass(cname,fp)  \
	static TypeInfo  paste(cname,_type) (quote(cname), fp); \
	TypeInfo &cname::type() { return paste(cname,_type); }

#endif // TYPEINFO_H

