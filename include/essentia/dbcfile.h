/********************************************************************
* Copyright (c) 1992 InterSoft Co.  All Rights Reserved
* THIS IS UNPUBLISHED PROPRIETARY SOURCE CODE OF InterSoft Co.
* The copyright notice above does not evidence any
* actual or intended publication of such source code.
*
* $Id: dbcfile.h,v 1.1.1.1 1996/04/16 18:51:47 gustavof Exp $
*
* DESCRIPTION
* Class definitions for general Db Config File.
*********************************************************************/

#ifndef DBCFILE_H
#define DBCFILE_H

#include <ifound.h>

#ifdef __GNUC__
#pragma interface
#endif

#include <essentia/cnffile.h>

#define ACCESS_SERVER_SEPAR	"\t"

class String;

class DbConfigFile : public ConfigFile {

// private:

	DbConfigFile(const DbConfigFile &);
	
public:

	DbConfigFile();
	~DbConfigFile();
		
	bool find(const String &schname = NULL_STRING, char tt = '\0');
	bool findServer(const String &schname, bool check = true);

	// this is for the shadow to get the config file without SERVERS
	bool getServer(const String &svName) 
		{ return read(svName, ICON); }

	bool addSchema(const String &schname);
	bool delSchema(const String &schname);

	String accessiblesServers();
};

#endif // DBCFILE_H
