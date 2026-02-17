/********************************************************************
* Copyright (c) 1991 InterSoft Co.  All Rights Reserved
* THIS IS UNPUBLISHED PROPRIETARY SOURCE CODE OF InterSoft Co.
* The copyright notice above does not evidence any
* actual or intended publication of such source code.
*
* $Id: password.h,v 1.1.1.1 1996/04/16 18:51:49 gustavof Exp $
*
* DESCRIPTION
* Password class
*********************************************************************/

#ifndef	PASSWORD_H
#define	PASSWORD_H

#ifdef __GNUC__
#pragma interface
#endif

#include <ifound.h>

#include <ifound/str.h>

class Password {

// private:

	Int user_id,			// user name
		group_id;			// user group
	String user_name,
		   group_name,
		   home_dir,
		   login_shell; 

	void parse_password();
	void parse_password(const String &uName);
	void parse_group();

public:

	Password();
	Password(const Password &pw) { *this=pw; };
	explicit Password(Int userid);
	// dummy is just to disambiguate... sorry.
	Password(Int pid, Int);
	explicit Password(const String &username);
	
    Password &operator=(const Password &pw);
    
	Int userId() const				{ return user_id; }
	const String &userName() const	{ return user_name; }
	Int groupId() const				{ return group_id; }
	const String &groupName() const	{ return group_name; }
	const String &homeDir() const	{ return home_dir; }
	const String &loginShell() const { return login_shell; }

	operator bool()	 const { return bool(user_id != ERR); }
	bool operator!() const { return bool(user_id == ERR); }

};

#endif		// PASSWORD_H
