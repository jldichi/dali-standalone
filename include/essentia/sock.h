//- Copyright Notice
// -----------------------------------------------------------------------
// (C) Copyright  1998 InterSoft Argentina S.A.  All Rights Reserved
// THIS IS UNPUBLISHED PROPRIETARY SOURCE CODE OF InterSoft Co.
// The copyright notice above does not evidence any actual or intended 
// publication of such source code.
//
// $Id: sock.h,v 1.1 1998/07/30 13:25:28 damian Exp $
// -----------------------------------------------------------------------

#ifndef SOCK_H
#define SOCK_H

#include <ifound.h>

#ifdef HAVE_PRAGMA
#pragma interface
#endif

/**
 * The idea of these classes is to mimic those defined in Java
 * (java.net.Socket & java.net.ServerSocket).  Of course, instead of
 * using SockImpl we use direct file descriptors obtained from the
 * socket() system call.  The idea is to encapsulate the OS differences
 * in the implementation of these classes to provide a uniform,
 * OS-independent, interface to our users (f.e. shadow/usrsrv).
 */

/**
 * Encapsulates a socket
 */
class Socket
{

protected:

	// the real socket
	Int fd_d;

public:

	// Constructors & destructors
	explicit Socket(Int fd);
	virtual ~Socket();
	void close();

	inline Int getFileDescriptor() const;
};

/**
 * Encapsulates a server-side socket which can be used to accept new
 * incoming connections.
 */
class ServerSocket
{

protected:

	Int fd_d;

public:

	explicit ServerSocket(Int port);
	virtual ~ServerSocket();
	Socket accept();
	void close();

};

#include <essentia/sock.icc>

#endif
