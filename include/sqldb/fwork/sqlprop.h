//- Copyright Notice
// -----------------------------------------------------------------------
// (C) Copyright 1998 InterSoft S.A.  All Rights Reserved
// THIS IS UNPUBLISHED PROPRIETARY SOURCE CODE OF InterSoft S.A.
// The copyright notice above does not evidence any actual or intended 
// publication of such source code.
//
// $Id: sqlprop.h,v 1.4 2002/03/19 21:56:26 hardaiz Exp $
// -----------------------------------------------------------------------

#ifndef	SQLFWPROPERTIES_H
#define	SQLFWPROPERTIES_H

#include <ideafix.h>
#include <ifound/tlist.h>

/**
 * This class is used to stored the pair (key;value) to construct the properties
 * table.
 * 
 * @author	Edgardo König
 */
class SQLFWPropertiesEntry {
public:
	SQLFWPropertiesEntry(const String &key, const String &value);
	~SQLFWPropertiesEntry();
	inline const String &getValue() const;
	inline const String &getKey() const;
	// Debug
	void dump() const;

private:
	String key_d;
	String value_d;
};

// Declare the list and the associated classes to manage it
declareTList(SQLFWPropertiesEntry);

/**
 * This class is used to get properties for a specific driver implementation.
 * This class must be inherited so driver specific values can be looked for
 * in the file supplied by getPropertiesFile.
 * 
 * @author	Eduardo Chiocconi
 */
class SQLFWProperties {
public:
	SQLFWProperties();
	~SQLFWProperties();
	const String &getProperty(const String &key);
	// Debug
	void dump() const;
	
protected:
	virtual const String &getPropertiesFile() const;

private:
	bool isOpen_d;
	FILE *propFile_d;
	SQLFWPropertiesEntryTList sqlfwPropList_d;
	
	bool open();
	bool close();
	inline bool isOpen() const;
	bool checkFileLoaded();
	void loadTable();
	SQLFWProperties(const SQLFWProperties& sqlProp);
	void addProperty(const String &key, const String &value);
	SQLFWPropertiesEntry *findProperty(const String &key) const;
};

#include <sqldb/fwork/sqlprop.icc>

#endif	// SQLFWPROPERTIES_H
