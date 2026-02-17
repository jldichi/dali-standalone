/********************************************************************
* Copyright (c) 1992 InterSoft Co.  All Rights Reserved
* THIS IS UNPUBLISHED PROPRIETARY SOURCE CODE OF InterSoft Co.
* The copyright notice above does not evidence any
* actual or intended publication of such source code.
*
* $Id: tcpcfile.h,v 1.1.1.1 1996/04/16 18:51:48 gustavof Exp $
*
* DESCRIPTION:
* Class definitions for DOS TCP Configuration.
*********************************************************************/

#ifndef TCPCFILE_H
#define TCPCFILE_H

#include <ifound.h>

#ifdef __GNUC__
#pragma interface
#endif

#include <ifound/gnr.h>

enum Token {
	BCAST,
	DOMAIN,
	HOSTS,
	IP,
	NAMESERVER,
	NETMASK,
	UNAME,
	USER,
};

// #include <essentia/cnffile.h>

#define CONFIG_DIR		"ESSENTIA"
#define SERVERS			"SERVERS"
#define SV_SUBDIR		"/servers/"
#define MAX_HOSTS		20
#define HOSTS_SEPAR		"="
							 /* Maximum of hosts in tcp config file */
class String;

class TcpConfigFile {
	String bcast;
	String netmask;
	String ns1;
	String domain;
	String hosts[MAX_HOSTS];
	String ip;
	String user;
	String uname;
	Int canthosts;
	void resolve(Token t, const String &str);
	Token findToken(const String &);

public:

	TcpConfigFile();
	~TcpConfigFile();
	const String &getBcast();
	const String &getNetmask();
	const String &getNs1();
	const String &getDomain();
	const String &getUname();
	const String &getUser();
	const String &getIp();
	const String getHostbyname(const String &);
	bool read(const String &);


};

#endif // TCPCFILE_H
