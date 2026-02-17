/********************************************************************
* Copyright (c) 1995 InterSoft Co.  All Rights Reserved
* THIS IS UNPUBLISHED PROPRIETARY SOURCE CODE OF InterSoft Co.
* The copyright notice above does not evidence any
* actual or intended publication of such source code.
*
* $Id: rcomm2.h,v 1.1.1.1 1996/04/16 18:51:50 gustavof Exp $
*
* DESCRIPTION:
*
*********************************************************************/

#ifndef  RCOM2_H
#define  RCOM2_H
          
#ifdef HAVE_UNISTD_H          
#	include <unistd.h> 
#endif
#include <errno.h>
#include <ifound/str.h>
#include <cracker/reqconn.h>
#include <local/rcmdenum.h>

class RemoteCommand2 {

//private:

	String node_,
		   user_,
		   password_,
		   command_;
	short packNum;

protected:

	RequestConnection *conn;

public:	
	RemoteCommand2(String, String, String, String, Int packNum = PKG_RUNTIME);
	virtual ~RemoteCommand2();

	virtual Int login(DebugConnType debug = noDebug);
	virtual Int logout();
	virtual Int execute() { return OK; }
	virtual const String &errorDesc() const;

	Int receive(Int len, char *buf) const;
	Int send(Int len, const char *buf) const;

	inline const String &node();
	inline RequestConnection *connection();
};

inline RequestConnection *RemoteCommand2::connection()
{
	return conn;
}

inline const String &RemoteCommand2::node()
{
	return node_;
}

inline Int RemoteCommand2::send(Int len, const char *buffer) const
{
#ifdef IDEAWIN
	return ::send(conn->sockDescr(), buffer, len, 0);
#else
	return _global_write(conn->sockDescr(), buffer, len);
#endif
}

inline Int RemoteCommand2::receive(Int len, char *buffer) const
{
	int res;
#ifdef IDEAWIN
	while (WSAIsBlocking())
		Sleep(1);
	do {
		res = ::recv(conn->sockDescr(), buffer, len, 0);
		check(res != SOCKET_ERROR);
	} while (res < 0 && (errno == WSAEINTR || errno == WSAEINPROGRESS));
#else
#ifdef HAVE_RESTARTABLE_SYSCALLS
	res = _global_read(conn->sockDescr(), buffer, len);
#else
	do {
		res = _global_read(conn->sockDescr(), buffer, len);
	} while (res < 0 && errno == EINTR);
#endif
#endif
	return res;
}

#endif // RCOMMAND_H

