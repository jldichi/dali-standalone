/*- Copyright Notice
 -----------------------------------------------------------------------
 Copyright (c) 1995 InterSoft Co.  All Rights Reserved
 THIS IS UNPUBLISHED PROPRIETARY SOURCE CODE OF InterSoft Co.
 The copyright notice above does not evidence any actual or intended 
 publication of such source code.

 $Id: libinfo.h,v 1.1.1.1 1996/04/16 18:51:34 gustavof Exp $

 Description:
 -----------------------------------------------------------------------*/

#ifndef LIBINFO_H
#define LIBINFO_H
#include <ideafix.h>
#if __cplusplus
class LibraryInfo
{
public:
	LibraryInfo(const String &nm, const String &data);
	static const String libraries();
};
#else
char *libInfo();	
#endif
#endif
