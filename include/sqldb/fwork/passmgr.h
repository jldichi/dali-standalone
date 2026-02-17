//- Copyright Notice
// -----------------------------------------------------------------------
// (C) Copyright 1998 InterSoft S.A.  All Rights Reserved
// THIS IS UNPUBLISHED PROPRIETARY SOURCE CODE OF InterSoft S.A.
// The copyright notice above does not evidence any actual or intended 
// publication of such source code.
//
// $Id: passmgr.h,v 1.2 2002/03/19 21:15:01 hardaiz Exp $
// -----------------------------------------------------------------------

#ifndef	PASSWMGR_H
#define	PASSWMGR_H

#include <ideafix.h>
#include <ifound.h>
#include <sqldb/fwork/sqlfwrc.h>

class SQLFWRcFileEntry;
class gtype(PtrArray, SQLFWRcFileEntry);

/**
 * This class is used to generate the configuration file to access other 
 * databases through the SQLFrameWork that is located in the home directory of
 * the user
 *
 * @version
 * @autor Edgardo König
 * @see SQLFWRcFileEntry 
 */
class PasswordManager {

public:
	PasswordManager(char *dsn);
	~PasswordManager();
	void	createSQLFWRcFile(char *homeDir, long usuario);
	void	add(const SQLFWRcFileEntry *entry);

private:
	// DSN to be used when generating the config file
	String	DSN_d;

	// List of URL entries
	gtype(PtrArray, SQLFWRcFileEntry) *list_d;	
};

/**
 * Objects of this class represents entries int the configuration file, they
 * are URL entries where the password field is encrypted
 *
 * @version
 * @autor Edgardo König
 * @see PasswordManager
 */
class SQLFWRcFileEntry {

public:
	SQLFWRcFileEntry(char *instance, char *user, char *password);
	inline String getInstance() const;
	inline String getUser() const;
	String	getPassword();
	static void cryptPassword(char *password);

	// Debug
	void	dump() const; 

private:
	String	instance_d;
	String	user_d;
	String	password_d;
};

#include "passmgr.icc"

#endif	// PASSWMGR_H
