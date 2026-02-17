/********************************************************************
* Copyright (c) 1995 InterSoft Co.  All Rights Reserved
* THIS IS UNPUBLISHED PROPRIETARY SOURCE CODE OF InterSoft Co.
* The copyright notice above does not evidence any
* actual or intended publication of such source code.
*
* $Id: netconn.h,v 1.7 1998/04/24 18:02:19 mauro Exp $
*
* DESCRIPTION:
*
*********************************************************************/

#ifndef NETCONN_H
#define NETCONN_H

#ifdef __GNUC__
#pragma interface
#endif

#include <essentia/connect.h>
#include <essentia/sockconn.h>

class CompressedConnection;
class NetConnection : public Connection {

	friend void setPackageNumber(NetConnection *, Int);
	friend Int getPackageNumber(NetConnection *);

//private:

	// Common
	SocketConnection *scon;
	CompressedConnection *ccon;
	bool  needConvert;
	short sendOp;

	// Buffer and pointer used by getC/putC
	union {
		Int dummy;
		char mBuff[sizeof(Int)];
	};

	char *pMBuff;

	// For stop recusive use of scon on connection error.
	static bool recursOnError;

	// For determine who set and how to get the comm. msg. size (buff. size).
	enum ConnType {
		server,
		client,
		clone,
	};
	bool useCompression;
#ifdef __NT__
	bool useKeepAlive;
#endif

	inline void reset();
	void createBuffer(ConnType);
	Int _receiveBody();

protected:

	// Common
	virtual void _flush();
	virtual Int _fill();

public:

	// Client.
	// ------------------------------------------------
	NetConnection(const String &, const String &, bool execSv = true);

	// Server.
	// ------------------------------------------------
	explicit NetConnection(const String &s = NULL_SERVICE,
			bool localCli = true);

#ifdef __NT__
	inline bool keepAlive();
#endif
	// Common.
	// ------------------------------------------------
	explicit NetConnection(int, ConvType ct = NO_CONVERT); // Clone.
	~NetConnection();

	virtual void flush();

	inline void flushResult(Int r);

	static void error(const String &msg, ...);
	inline Status status() const;

	inline static bool isRemoteChannel(Int fd);

	// Options handling
	short getOp();
	inline NetConnection &operator()(Int opt);

	inline Int result();

 	// Guiltys : RemoteComand and RemoteExecD.
	inline int sockDescr() const;
	inline int fDescr() const;

	// Parameters handling
	virtual Connection &operator<<(Int);
	virtual Connection &operator<<(const String &s);
	virtual Connection &operator<<(const char *s);

	virtual Connection &operator>>(Int &i);
	virtual Connection &operator>>(String &s);
	virtual Connection &operator>>(char *&cp);

	void putC(char c);
	char getC();

	inline bool moreData();
	Int receivedBytes;
	Int sentBytes;
	Int roundTrips;
};

void NetConnection::reset()
{
	sendOp = ERR;
	rcv_pdata = snd_pdata = buffer->data;
  	snd_cnt = size - 1;
  	rcv_cnt = 0;
}

bool NetConnection::moreData()
{
	return rcv_cnt > 0;
}

Int NetConnection::result()
{
	Int i;
	*this >> i;
	return i;
}

NetConnection& NetConnection::operator()(Int opt)
{
	buffer->h.serverOp = sendOp = opt;
	return *this;
}

void NetConnection::flushResult(Int r)
{
	*this << r;
	flush();
}

Status NetConnection::status() const
{
	assert(scon);
	return scon->status();
}

bool NetConnection::isRemoteChannel(Int fd)
{
	return SocketConnection::isSocket(fd);
}

int NetConnection::sockDescr() const
{
	assert(scon);
	return scon->sockDescr();
}


int NetConnection::fDescr() const
{
	assert(scon);
	return scon->fDescr();
}

#ifdef __NT__
#include <stdlib.h>

bool NetConnection::keepAlive()
{
	char FAR* optval = (char *)malloc(sizeof(char));
	int FAR* optlen = (int *)malloc(sizeof(int));

	optval[0]='1';
	*optlen=sizeof(int);

	WSASetLastError(0);

	if (setsockopt(scon->sockDescr(), IPPROTO_TCP, SO_KEEPALIVE, optval, *optlen) == SOCKET_ERROR){
		int err = WSAGetLastError();
//		SysLog s("netconn.keepAlive");
//		s.logWarning("Can't set keep-alives. Error: %d\n",err);
		return false;
	}
	return true;
}
#endif // __NT__
#endif // NETCONN_H
