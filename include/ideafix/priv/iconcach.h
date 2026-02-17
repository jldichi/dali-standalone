// -----------------------------------------------------------------------
// Copyright (c) 1995 InterSoft Co.  All Rights Reserved
// THIS IS UNPUBLISHED PROPRIETARY SOURCE CODE OF InterSoft Co.
// The copyright notice above does not evidence any actual or intended 
// publication of such source code.
//
// $Id: iconcach.h,v 1.1.1.1 1996/04/16 18:51:42 gustavof Exp $
//
// Description:
// -----------------------------------------------------------------------

#ifndef ICONCACH_H
#define ICONCACH_H

#include <ifound/str.h>

class GridIcon;

class gtype(PtrHashTab, GridIcon);

class IconCache
{

	gtype(PtrHashTab, GridIcon) *icons_;
	GridIcon *iconArray_;
	Int nIcons_;
	Int next_;

	GridIcon *add(const String &n);

public:

	IconCache(Int nIcons);
	~IconCache();

	GridIcon *get(const String &n);

	static Int cmpIcons(const void *left, const void *right);
	static Int hashIcons(const void *node, Int modulo);
};

#endif   // ICONCACH_H
