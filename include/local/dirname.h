/********************************************************************
* Copyright (c) 1993 InterSoft Co.  All Rights Reserved
* THIS IS UNPUBLISHED PROPRIETARY SOURCE CODE OF InterSoft Co.
* The copyright notice above does not evidence any
* actual or intended publication of such source code.
*
* $Id: dirname.h,v 1.1.1.1 1996/04/16 18:51:49 gustavof Exp $
*
* DESCRIPTION
*********************************************************************/

#ifndef DIRNAME_H
#define DIRNAME_H

#ifdef __GNUC__
#pragma interface
#endif

#include <local/strfile.h>
#include <ifound/itypes.h>

class DirName
{

// private:

	String current;

public:

	DirName() { current = NULL_STRING; }
	explicit DirName(const String &d, const bool c = true);
	~DirName() {};

	void set(const String &d) { current = d;}
	void chDir(const String &p, const bool c = false);
	void compress();
	const String &value() { return current; }
};

#endif
