/********************************************************************
* Copyright (c) 1995 InterSoft Co.  All Rights Reserved
* THIS IS UNPUBLISHED PROPRIETARY SOURCE CODE OF InterSoft Co.
* The copyright notice above does not evidence any
* actual or intended publication of such source code.
*
* $Id: ftractor.h,v 1.3 2000/01/31 18:13:46 diegon Exp $
*
* DESCRIPTION
*
* ROUTINE       |  PURPOSE
*---------------+---------------------------------------------------
*********************************************************************/

#ifndef __FTRACTOR_H
#define __FTRACTOR_H

#include <ideafix.h>
#include <cracker/pserconn.h>
#include <local/dbrowser.h>
#include <local/rcommand.h>
#include <ideafix/priv/ixsystem.h>

#ifdef __GNUC__
#pragma interface
#endif

#define FINISHING NULL_STRING
#define SEPAR " "
#define BOUND "$"


/**
 * Mother of a serie of classes destinated to translate files
 * from remote to local machines.
 * This, server-side abstract class, uses a PrServerConnection
 * object as nexus with its clients.
 */
class SvFileTractor
{
protected: 
	/// Connection object.
	PrServerConnection *prconn;
	
	void sendFile(const String fname);
public:
	SvFileTractor(PrServerConnection * conn);
	~SvFileTractor();
	PrServerConnection *getConn();
	Int execute();
};

/**
 * Final class destinated to send .mno files through the connection.
 * Mno files are stored IxSMenu objects. This files reside in 
 * $AUTHDIR/group_dir of the user owner of the process running.
 * SvMnoTractor determines the group which the user belongs and
 * browse the respective directory sending to the client .mno timestamps
 * in order to be aware about which files must be updates.
 */
class SvMnoTractor : public SvFileTractor
{
	void sendMnoTimeStamp();
	String fullMnoName(const String mnofile);
public:
	SvMnoTractor(PrServerConnection * conn);
	~SvMnoTractor(){}
	Int execute();
}; 


class ClFileTractor
{
protected: 
	RemoteCommand *rcmd;
	String base_dir;
	String group_d;
	
	void receiveFile(const String file);
public:
	ClFileTractor(RemoteCommand * conn);
	~ClFileTractor();
	void setBaseDir(const String dir)
	{
		base_dir = dir;
	}
	String &groupName(void)
	{
		return group_d;
	}
	Int execute();
};



class ClMnoTractor : public ClFileTractor
{
	String group_name;
	
	bool needUpdate(const String file, Time ts);
	bool verifyGroupDir();
	String verifyStamps();
public:
	ClMnoTractor(RemoteCommand *rc); 
	~ClMnoTractor(){}
	Int execute();
};

#endif /* __FTRACTOR_H */
