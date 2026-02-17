//- Copyright Notice
// -----------------------------------------------------------------------
// Copyright (c) 1995 InterSoft Co.  All Rights Reserved
// THIS IS UNPUBLISHED PROPRIETARY SOURCE CODE OF InterSoft Co.
// The copyright notice above does not evidence any actual or intended
// publication of such source code.
//
// libinfo.cc,v 1.1.1.1 1996/04/16 18:51:59 gustavof Exp
//
// Description:
// -----------------------------------------------------------------------
#include <ifound.h>
#include <ifound/libinfo.h>

static String *libs__ = NULL;

LibraryInfo::LibraryInfo(const String &nm, const String &data)
{
	if (libs__ == NULL)
		libs__ = new String;
	*libs__ += String("\n") + String("> ")  + nm +  String(" <") + "\n" + data + "\n";
}

const String
LibraryInfo::libraries()
{
	return libs__? *libs__ : NULL_STRING;
}
