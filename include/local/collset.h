/***********************************************************************
* Copyright (c) 1991 InterSoft Co.  All Rights Reserved
* THIS IS UNPUBLISHED PROPRIETARY SOURCE CODE OF InterSoft Co.
* The copyright notice above does not evidence any
* actual or intended publication of such source code.
*
* $Id: collset.h,v 1.1.1.1 1996/04/16 18:51:49 gustavof Exp $
*
* DESCRIPTION
* Class definitions for collate set type.
*********************************************************************/

#ifndef COLLSET_H
#define COLLSET_H

#ifdef __GNUC__
#pragma interface
#endif

#include <ifound.h>

#include <ifound/str.h>

#define NCHARS		(256)
#define NMAPPINGS	(8)

struct DoubleChar {

	UnChar ch;
	UnChar nch;
	UnChar mappings[NMAPPINGS];
	DoubleChar *next;

	DoubleChar(UnChar c, DoubleChar *n = NULL)
	: ch(c), nch(0), next(n)
	{ for (int i = 0; i < NMAPPINGS; i++) mappings[i] = '\0'; }

};

class CollateSet
	// Character mapping
{

// private:

	String name_;
	UnChar stdToCs[NCHARS];
	short csToStd[NCHARS];
	DoubleChar *dlist;
	UnChar nspcl;

	void initialize();
	void destroy();
	UnChar mapDouble(UnChar c1, UnChar c2, UnChar c) const;
	UnChar unMapDouble(UnChar c1, UnChar c2) const;
	void addDouble(UnChar c1, UnChar c2, short n);

public:

	explicit CollateSet(const String &file);
	~CollateSet();

	const String &name() const { return name_; }
	String map(const String &str) const;
	String unMap(const String &str) const;
	void map(char *to, const char *from, Int l) const;
	void unMap(char *to, const char *from, Int l) const;

#ifdef ISDEBUG
	void print() const;
#endif
};

#endif

