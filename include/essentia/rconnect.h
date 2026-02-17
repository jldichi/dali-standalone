/********************************************************************
* Copyright (c) 1992 InterSoft Co.  All Rights Reserved
* THIS IS UNPUBLISHED PROPRIETARY SOURCE CODE OF InterSoft Co.
* The copyright notice above does not evidence any
* actual or intended publication of such source code.
*
* $Id: rconnect.h,v 1.4 2002/03/15 20:41:35 hardaiz Exp $
*
* DESCRIPTION
* Class definitions for remote Connection type.
*********************************************************************/

#ifndef RCONNECT_H
#define RCONNECT_H

#include <ifound.h>

#ifdef __GNUC__
#pragma interface
#endif

#include <ifound/convconn.h>
#include <essentia/dbconn.h>
#include <essentia/sockconn.h>

#define BAD_SEND		-7
#define BAD_RECV		-8

class RemoteConnection : public DbConnection {

// private:

	SocketConnection *scon;
	BaseAction(StringRef, StringRef) *query_user; 	// useful for WINDOWS clients

	void _flush();
	Int _fill();

	int receiveFrom();
	int sendTo();
	void convertHeaders(MsgBuff rbuffer, Int len);

	RemoteConnection(const RemoteConnection&);
	
protected:

	bool needConvert;
	short (*convS)(short);
	Int   (*convI)(Int);
	void  (*convH)(MsgBuff);

public:

	RemoteConnection(const String &nm, String svName,
#ifndef __VC__
		ConnMode abort = abortAlways,
#else
 		ConnMode abort = ConnMode(abortAlways),
#endif
		String schname = NULL_STRING, 
		BaseAction(StringRef, StringRef) *query = NULL);
	~RemoteConnection();

	// operadores redefinidos
	Connection &operator<<(Int i);
	Connection &operator>>(Int &i);

	// nuevos operadores para los el push/pop de parametros
	void push(Int i)			{ RemoteConnection::operator<<(i); }
	void push(const String &s)	{ DbConnection::operator<<(s); }
	void push(void *v, Int i)	{ DbConnection::push(v, i); }
	void pop(Int &i)			{ RemoteConnection::operator>>(i); }
	void pop(String &s)			{ DbConnection::operator>>(s); }
	void pop(void *v, Int i)	{ DbConnection::pop(v, i); }

	void connect(bool secServ = false);

	const String &hostName()	{ return name(); } 
	
};


#endif // RCONNECT_H
