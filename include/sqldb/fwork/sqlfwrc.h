//- Copyright Notice
// -----------------------------------------------------------------------
// (C) Copyright 1998 InterSoft S.A.  All Rights Reserved
// THIS IS UNPUBLISHED PROPRIETARY SOURCE CODE OF InterSoft S.A.
// The copyright notice above does not evidence any actual or intended 
// publication of such source code.
//
// $Id: sqlfwrc.h,v 1.9 2006/08/14 20:33:24 cristiand Exp $
// -----------------------------------------------------------------------

#ifndef SQLFWRC_H
#define SQLFWRC_H

#include <ifound/str.h>

class SQLFWUrl;

#define SQLFW_CONFIG_FILE	".sqlfwrc"
#define MAX_URL_LINE		256
#define	FILE_URL_TOKEN		"URL"

//extern "C" SQLFWUrl getURLForSchema(const String &schemaName);
SQLFWUrl getURLForSchema(const String &schemaName);
extern "C" String getConfigFileName();
            
class SQLFWUrl;            

/**
 * This class is used to manipulate the user configuration file. 
 * Probably this file will be a .sqlfwrc file in the user's home directory.
 * Inside this file will be the URLs to use and some other variables.
 * 
 * @version
 * @author Pablo Rubinstein
 */
class SQLFWRcFile {

public:
	SQLFWRcFile(const String &fileName);
	~SQLFWRcFile();
	SQLFWUrl fetchRCFile(const String &schemaName);
	SQLFWUrl getURLForSchema(const String &schemaName);
	String getDSN();

	inline String fileName() const { return fileName_d; }
private:
	String fileName_d;	// full path of the config file
	FILE *fp;
	bool isOpen_d;

	bool open();
	bool close();
	bool isOpen() const;
	String buildURL(const String &schemaName, const String &line) const;
	String parseURL(const String &line) const;
};

#endif // SQLFWRC_H
