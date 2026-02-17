// --------------------------------------------------------------------------
// Copyright (c) 1995 InterSoft Co.  All Rights Reserved
// THIS IS UNPUBLISHED PROPRIETARY SOURCE CODE OF InterSoft Co.
// The copyright notice above does not evidence any actual or intended 
// publication of such source code.
//
// $Id: enumobj.h,v 1.1.1.1 1996/04/16 18:51:41 gustavof Exp $
//
// Description:
// --------------------------------------------------------------------------

#ifndef ENUMOBJ_H
#define ENUMOBJ_H

#include <ifound/str.h>

class gtype(PtrArray, EnumObjectEntry);

class EnumObjectEntry {

public:

	inline EnumObjectEntry(const String &enumName, Int val = INT_MIN,
			const String &shortDescr = NULL_STRING,
			const String &longDescr = NULL_STRING);

	inline Int value() const;
	inline const String &name() const;

	void shortDescr(const String &shortDesc);
	inline const String &shortDescr() const;
	void longDescr(const String &longDesc);
	inline const String &longDescr() const;

private:

	String enumName_d;
	Int value_d;
	String shortDescr_d;
	String longDescr_d;

};

class EnumObject {

public:

	EnumObject(const String &nm);
	virtual ~EnumObject();

	Int numEntries() const;
	EnumObjectEntry *entry(Int i) const;

	bool addEntry(const String &entryName, Int val = INT_MIN);
	EnumObjectEntry *findEntry(const String &entryName) const;
	EnumObjectEntry *findEntry(Int value) const;
	void resolveDescriptions(const String &msg);
	
	inline const String &name() const;

	// for debugging only...
	void print();

private:

	String name_d;
	gtype(PtrArray, EnumObjectEntry) &entries_d;

};

#include <ideafix/priv/enumobj.icc>

#endif
