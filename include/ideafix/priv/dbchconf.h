
/********************************************************************
* Copyright (c) 1987 InterSoft Co.  All Rights Reserved
* THIS IS UNPUBLISHED PROPRIETARY SOURCE CODE OF InterSoft Co.
* The copyright notice above does not evidence any
* actual or intended publication of such source code.
*
* $Id: dbchconf.h,v 1.2 1999/06/08 19:38:52 ekonig Exp $
*
* DESCRIPTION:
*
* ROUTINE           |  PURPOSE
*-------------------+---------------------------------------------------
* | 
*********************************************************************/
#ifndef __DBCHCONF_H
#define __DBCHCONF_H

#include <ifound/tlist.h>


class DbChConfigEntry {
	char *_scname;
	int _datatype;	
	
public:
	DbChConfigEntry(char *sc,int db);
	~DbChConfigEntry();
	char *scname() { return _scname;}
	int datatype() { return _datatype;}

};

declareTList(DbChConfigEntry);

class DbChConfig {
	static char *_ixname;
	static char *_esname;
	static char *_sqlname;
	static DbChConfig *_dbconf;

	DbChConfigEntryTList _dcl;
	DbChConfigEntry*  _find(char *schema);

	void _add(char *, int );
	void _loadFromFile();
	void _print();
	void _loadFromEnv();
	void _parseLine(char *);

	static int _parseDbName(char *);

public:
	DbChConfig();
	~DbChConfig();

	static void load();
	static void print();
	static int findType(char *schema);
	static char * _getDbName(int);
	static int getNumDbTypes();
	static char **getDatabasesNames();
};

#endif
