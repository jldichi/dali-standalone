/********************************************************************
* Copyright (c) 2026 Master_Solutions S.R.L.  All Rights Reserved
* THIS IS UNPUBLISHED PROPRIETARY SOURCE CODE OF Master_Solutions S.R.L.
* The copyright notice above does not evidence any
* actual or intended publication of such source code.
*
* $Id: password.cc,v 1.8 2008/01/24 13:42:36 hardaiz Exp $
*
* DESCRIPTION
* Password class implementation
* It allows inspecting user information from '/etc/passwd' file.
*********************************************************************/

#ifdef __GNUC__
#pragma implementation
#endif

#include <sys/types.h>
#include <unistd.h>

#include <local/password.h>
#include <local/strfile.h>
#include <ifound/stdc.h>


#define	PASSWORD_FILE		"/etc/passwd"
#define	GROUP_FILE			"/etc/group"


#ifndef HAVE_NETAPI
// fill in user name and group id from password file 
// use userid as search key

void Password::parse_password()
{
	String id_str = toCharPtr(user_id);
	String line_r;
	String uid, gid;
	InputFile pswd_file(PASSWORD_FILE);

	while (pswd_file) {
		pswd_file >> line_r;

		uid = line_r.field(2, ":");
		if (uid == id_str) {
			user_name = line_r.field(0, ":");
			gid = line_r.field(3, ":");
			group_id = toInt(gid);
			home_dir = line_r.field(5, ":");
			login_shell = line_r.field(6, ":");
			return;
		}
	}
}

// fill in user id and group id from password file 
// use username as search key

void Password::parse_password(const String &unm)
{
	String line_r;
	String uid;
	String gid;
	InputFile pswd_file(PASSWORD_FILE);

	user_id = group_id = -1;

	while (pswd_file) {
		pswd_file >> line_r;

		if (unm == line_r.field(0, ":")) {
			uid = line_r.field(2, ":");
			user_id = toInt(uid);
			gid = line_r.field(3, ":");
			group_id = toInt(gid);
			home_dir = line_r.field(5, ":");
			login_shell = line_r.field(6, ":");
			return;
		}
	}
}

// get the group name from the group file

void Password::parse_group()
{
	String id_str = toCharPtr(group_id);
	String line_r;
	String gid;
	InputFile group_file(GROUP_FILE);

	while (group_file) {
		group_file >> line_r;

		gid = line_r.field(2, ":");
		if (gid == id_str) {
			group_name = line_r.field(0, ":");
			return;
		}
	}
}

Password &Password::operator=(const Password &pw)
{  
    if ( this != &pw )  {
		user_id    = pw.user_id;
		group_id   = pw.group_id;		 
	    user_name  = pw.user_name;
		group_name = pw.group_name;
		home_dir   = pw.home_dir;
		login_shell = pw.login_shell;
	}
	return *this;	
}

Password::Password()
{
	user_id = getuid();
	parse_password();
	parse_group();
}

Password::Password(Int userid)
{
	user_id = userid;
	parse_password();
	parse_group();
}

#if HAVE_PROC_DIR
#	include <sys/types.h>
#	include <sys/stat.h>
#endif

static Int uidFromPid(Int pid)
{
#if HAVE_PROC_DIR
	struct stat st;
	String str = toString("/proc/") + toString(pid);
	statFile(str, &st);
	return st.st_uid;
#else
	String str = PSUID_CMD;
#if defined(_AIX) || defined(hpux)
	InputPipe ip(toCharPtr(str), pid);
#else
	InputPipe ip(toString(toCharPtr(str), pid));
#endif
	ip >> str;
	ip >> str;
	str.prune();
	String uidstr = str.field(PSUID_POS, " ");
	return toInt(uidstr);
#endif
}

Password::Password(Int pid, Int)
{
	user_id = uidFromPid(pid);
	parse_password();
	parse_group();
}

Password::Password(const String &username)
{
	user_name = username;
	parse_password(user_name);
	parse_group();
}

// end of password
#endif

#ifdef __NT__
Password::Password(Int userid)
{
	user_id = userid;
	parse_password();
	parse_group();
}

Password::Password(const String &username)
{
	user_name = username;
	parse_password(user_name);
	parse_group();
}

Password::Password()
{
	user_id = getuId();

	parse_password();
	parse_group();
}
void Password::parse_password()
{
	group_id=userGid(user_id);
	home_dir=userHomeDir(user_id);
	login_shell="cmd";
}

void Password::parse_password(const String &uName)
{
	char *edy=toCharPtr(uName);
	user_id=::userId(edy);
	if(!user_id){
		user_id=group_id = -1;
		return;
	}
	parse_password();
}
void Password::parse_group(){};
// end of password
#endif
