// --------------------------------------------------------------------------
// Copyright (c) 1995 InterSoft Co.  All Rights Reserved
// THIS IS UNPUBLISHED PROPRIETARY SOURCE CODE OF InterSoft Co.
// The copyright notice above does not evidence any actual or intended 
// publication of such source code.
//
// $Id: _enumobj.cc,v 1.1.1.1 1996/04/16 18:52:11 gustavof Exp $
//
// Description:
// --------------------------------------------------------------------------

#include <ifound.h>
#include <ideafix/priv/enumobj.h>

declare(PtrArray, EnumObjectEntry);
implement(PtrArray, EnumObjectEntry);
declare(PtrArrayCursor, EnumObjectEntry);
implement(PtrArrayCursor, EnumObjectEntry);

void EnumObjectEntry::shortDescr(const String &shortDesc)
{
	shortDescr_d = shortDesc;
}

void EnumObjectEntry::longDescr(const String &longDesc)
{
	longDescr_d = longDesc;
}

EnumObject::EnumObject(const String &nm)
:	name_d(nm),
	entries_d(*tr_new PtrArray(EnumObjectEntry)(0, 8))
{
}

EnumObject::~EnumObject()
{
	for (PtrArrayCursor(EnumObjectEntry) c(entries_d); c; ++c)
		tr_delete(*c);
	tr_delete(&entries_d);
}

EnumObjectEntry *EnumObject::findEntry(const String &entryName) const
{
	for (PtrArrayCursor(EnumObjectEntry) c(entries_d); c; ++c) {
		if (c->name() == entryName)
			return *c;
	}
	return NULL;
}

EnumObjectEntry *EnumObject::findEntry(Int value) const
{
	for (PtrArrayCursor(EnumObjectEntry) c(entries_d); c; ++c) {
		if (c->value() == value)
			return *c;
	}
	return NULL;
}

bool EnumObject::addEntry(const String &entryName, Int val)
{
	if (findEntry(entryName))
		return false;

	if (val == INT_MIN) {
		Int nEntries = entries_d.dim();
		val = (nEntries == 0) ? 0 : entries_d[nEntries-1]->value() + 1;
	}

	entries_d.add(tr_new EnumObjectEntry(entryName, val));

	return true;
}

Int EnumObject::numEntries() const
{
	return entries_d.dim();
}

EnumObjectEntry *EnumObject::entry(Int i) const
{
	return entries_d[i];
}

void EnumObject::resolveDescriptions(const String &msg)
{
	for (StrCursor c(msg, "\n"); c; ++c) {
		String entryValue = c.value();
		// toDo: should check for ':' inside quotes, etc. etc.
		String enumName = entryValue.field(0, ":");
		String shortDescr = entryValue.field(1, ":");
		String longDescr = entryValue.field(2, ":");
		EnumObjectEntry *entry = findEntry(enumName);
		if (entry != NULL) {
			if (shortDescr == NULL_STRING)
				shortDescr = enumName.toUpper();
			entry->shortDescr(shortDescr);
			entry->longDescr(longDescr);
		}
	}
}

void EnumObject::print()
{
	printf("Start of Enum '%S'\n", &name_d);
	for (PtrArrayCursor(EnumObjectEntry) c(entries_d); c; ++c)
		printf("\t%S = %ld (%S:%S)\n", &c->name(),
					c->value(), &c->shortDescr(), &c->longDescr());
	printf("End of Enum '%S'\n", &name_d);
}

