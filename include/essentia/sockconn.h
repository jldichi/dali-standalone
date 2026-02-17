/***********************************************************************
* Copyright (c) 1991 InterSoft Co.  All Rights Reserved
* THIS IS UNPUBLISHED PROPRIETARY SOURCE CODE OF InterSoft Co.
* The copyright notice above does not evidence any
* actual or intended publication of such source code.
*
* $Id: sockconn.h,v 1.9 2002/03/15 20:21:08 hardaiz Exp $
*
* DESCRIPTION
*********************************************************************/

#ifndef SOCKCONN_H
#define SOCKCONN_H

#include <sys/types.h>
#ifdef HAVE_STD_TCP_HEADERS
#	include <sys/socket.h>
#endif

#ifndef __VC__
extern "C" int errno;
#else
#	include <errno.h>
#endif

#include <ifound.h>
#include <ifound/convconn.h>
#include <ifound/str.h>

#define LOCAL_HOST		NULL_STRING
#define NULL_SERVICE	NULL_STRING

#ifdef HAVE_WINSOCK
	#undef ClosePrinter
	#undef CreateCursor
	#include <ifound.h>
#ifdef __VC__
#ifdef IDEAWIN
	#define DATE	dummyDATE
	#undef Free
	#undef Alloc
	#include <winsock.h>
	#undef DATE
	#define Alloc(n)(def_Alloc(n,__FILE__,__LINE__))
	#define Free(p)(def_Free(p,__FILE__,__LINE__))
#else // IDEAWIN
	#include <winsock.h>
#endif // IDEAWIN
#else  // __VC__
	#include <win32/winsock.h>
#endif // __VC__
	#define CreateCursor    DbCreateCursor
	#define ClosePrinter	_ClosePrinter
	#undef  ERROR
	#define ERROR	(-1)
	#undef  OK
	#define OK		(0)
#else  // HAVE_WINSOCK
	#define SOCKET		int
#endif // HAVE_WINSOCK

enum ConvType {
	NULL_TYPE,
	NO_CONVERT,
	INVERT
};

enum Status {
	OKEY,					// Status 0
	NO_SERVICE,				// Status 1
	NO_HOST,				// Status 2
	NO_SERVER,				// Status 3
	NO_SOCKET,				// Status 4
	NO_AF_UNIX,				// Status 5
	NO_CLI_LOCATION,		// Status 6
	CANT_CONNECT,			// Status 7
	CANT_BIND,				// Status 8
	CANT_LISTEN,			// Status 9
	CANT_ACCEPT,			// Status 10
	CANT_SEND_TESTNUM,		// Status 11
	UNKNOWN_CONV,			// Status 12
	INVALID_SOCK_ID,		// Status 13
	CANT_UNLINK				// Status 14
};

const Int testNum = 12345678;

class SocketConnection {

// private:

	// Common
	// ------------------------------------------------
	Status status_;
	SOCKET sockDescr_;

	// Client
	// ------------------------------------------------
	ConvType convType;
	void defineConvType();
	void inetConnect(const String &, const String &);
	void unixConnect(Int);

	// Server
	// ------------------------------------------------
	Int sendTestNum();
	int inetCreate(const String &);
	void inetAccept(int);
	int unixCreate(Int);
	void unixAccept(int);
	int fDescr_;				// Socket before accept;

public:

	// Server methods
	// ------------------------------------------------
	explicit SocketConnection(const String &service = NULL_SERVICE,
					 bool localClient = true);

	// Client methods
	// ------------------------------------------------
	SocketConnection(const String &, const String &, bool execSv=true);
	explicit SocketConnection(SOCKET sd, ConvType ct = NO_CONVERT);
	~SocketConnection();

	inline bool needConvert();
	inline short convert(short i);
	inline Int convert(Int i);
	inline Status status() const;
	Int send(Int, const char *);
	Int receive(Int, char *);
	inline SOCKET sockDescr() const;
	inline int fDescr()	const;
	static inline bool isSocket(SOCKET sd);

};

#include <essentia/sockconn.icc>

#ifdef HAVE_AF_UNIX
enum HostLoc {
	err = -1,
	local,
	remote,
};

/* Function used to determine if "host" parameter is local or not. */
extern "C" HostLoc hostLoc(const String &host);

#endif // HAVE_AF_UNIX

static const char *svSockName(int id);

#endif // SOCKCONN_H
