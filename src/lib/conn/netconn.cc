/********************************************************************
* Copyright (c) 1995 InterSoft Co.  All Rights Reserved
* THIS IS UNPUBLISHED PROPRIETARY SOURCE CODE OF InterSoft Co.
* The copyright notice above does not evidence any
* actual or intended publication of such source code.
*
* $Id: netconn.cc,v 1.8 2003/12/04 19:33:40 albertoi Exp $
*
* DESCRIPTION:
*
*********************************************************************/

#ifdef __GNUC__
#pragma implementation
#endif

#include <errno.h>

#include <ifound.h>
#include <ifound/stdc.h>
#include <essentia/netconn.h>
#include <essentia/compconn.h>
#include <essentia/sockconn.h>
#include <ifound/priv/pconfig.h>
#include <ifound/priv/macros.h>
#include <local/slog.h>


#define nAssert(EX) \
		do { if (EX) ; else NetConnection::error(String("Assertion failed (%s).\n%s:%d.\n"), quote(EX), __FILE__, __LINE__); } while(0)

#define MINIMUN_SIZE 64

static bool sending = false;
bool NetConnection::recursOnError = false;

NetConnection::NetConnection(const String &service, const String &host,
								   bool execSv)
:	Connection(),
	pMBuff(NULL)
{
	scon = tr_new SocketConnection(service, host, execSv);

	if (scon && scon->status() == OKEY) {
		needConvert = scon->needConvert();
		createBuffer(client);
	}
}

NetConnection::NetConnection(const String &service, bool localCli)
: 	Connection(),
	pMBuff(NULL)
{
#ifdef HAVE_PROC
	scon = tr_new SocketConnection(service, localCli);

	needConvert = false;

	if (scon && scon->status() == OKEY)
		createBuffer(server);
#else
	scon = NULL;
#endif
}

NetConnection::NetConnection(int sd, ConvType ct)
:	pMBuff(NULL)
{
#ifdef HAVE_PROC
	scon = tr_new SocketConnection(sd, ct);
	/*
		If the server will use this constructor
		'ct' (ConvType) MUST be NO_CONVERT.
		If the client use it 'ct' MUST be specified
		because 'testNum' test is not made.
		if ConvType is NULL_TYPE - I'am server and test was made by
		SocketConnection
	*/
	if (scon && scon->status() == OKEY) {
		needConvert = scon->needConvert();
		createBuffer(ct == NULL_TYPE ? server : clone);
	}
#else
	scon = NULL;
#endif
}

NetConnection::~NetConnection()
{
	tr_delete_vec(buffer);
	tr_delete(scon);
}

static inline void setComBuffSize(Int val)
{
	String var("COMBUFFSIZE=");
	var += toString(val);
	var.putEnv();
}

static inline void setUseCompression(Int val)
{
	String var("COMPRESS=");
	var += toString(val);
	var.putEnv();
}

#ifdef __NT__
static inline void setUseKeepAlive(Int val)
{
	String var("KEEPALIVE=");
	var += toString(val);
	var.putEnv();
}
#endif

static inline Int getComBuffSize()
{
	return toInt(String("COMBUFFSIZE").getEnv());
}

static inline bool getUseCompression()
{
	return toInt(String("COMPRESS").getEnv());
}

#ifdef __NT__
static inline bool getUseKeepAlive()
{
	return toInt(String("KEEPALIVE").getEnv());
}
#endif



/**
 *	En INA 32bits en la variable @fullsize, al realizar la siguiente operación 
 *		Int fullsize = (buffHdrSize + size) * sizeof(Int);
 *	Da como resultado 8192, siempre.
 *	En 64 bits, @fullsize, variaba entre dos valores: 1024 y 8192, por lo que si daba 1024, limitaba el ingreso
 *	de la cantidad de datos recibido desde el cliente INA. Por lo que si modificaba el INA
 *	y la cantidad de bytes superaba los 1024, no era posible recibir los datos desde el cliente
 *	Como @fullsize se usa como parámetro en otros métodos, se le asigna por defecto la constante
 *  @MAX_DATA
 */
const int MAX_DATA = 8192; 
const int MEMORY_SIZE = 8;

void NetConnection::createBuffer(ConnType connType)
{
	nAssert(sizeof(Int) == MEMORY_SIZE); // Si no, cli y srv no se ponen de acuerdo.
    nAssert(sizeof(s_msgHdr) < MINIMUN_SIZE/2);

	// Calculate buffer size.
	// ---------------------------------------------------
	const Int buffHdrSize=(sizeof(struct s_msgHdr)+sizeof(Int)-1)/sizeof(Int);
 
	Int comBuffSize;
	s_msgHdr hdr;
 
#ifdef __NT__
	useKeepAlive=false;
#endif
	if (connType == client) {
		comBuffSize = getComBuffSize();
		if (comBuffSize == INT_NULL || comBuffSize <= 0) {
			comBuffSize = COM_BUFF_SIZE;
			setComBuffSize(comBuffSize); // Make it known to clone.
		}
		useCompression = getUseCompression();
#ifdef __NT__
		useKeepAlive = getUseKeepAlive();  
		setUseKeepAlive(useKeepAlive); // Make it known to clone.
		useKeepAlive=false;
#endif
	} else if (connType == server) {
		sending = false;
  
		Int rcv = scon->receive(sizeof(hdr), (char *) &hdr);
   
		if (rcv != sizeof(hdr)) {
			String tmp = libMsg("ERRRMSG") + toString(errno)
					   + ". Expected: " + toString(sizeof(hdr))
					   + "; received: " + toString(rcv);

			error(tmp);
		}
  
		if (needConvert)
			invH((MsgBuff)&hdr);
		comBuffSize = hdr.retValue;
		setUseCompression(hdr.clientId != ERR);
		hdr.clientId = ERR;
		hdr.retValue = ERR;
		useCompression=false;
#ifdef __NT__
		useKeepAlive=false;
#endif
		setComBuffSize(comBuffSize); // Make it known to clone.
	} else {
		nAssert (connType == clone);
		comBuffSize = getComBuffSize();
		useCompression = getUseCompression();
#ifdef __NT__
		useKeepAlive = getUseKeepAlive(); //from crackerrc (by now)
#endif
		nAssert (comBuffSize != INT_NULL && comBuffSize > 0);
	}

	// Need at least a little more than the header size
	if (comBuffSize < MINIMUN_SIZE) {
		String tmp = String("Communication buffer size should be greater than ") +
					 toString(MINIMUN_SIZE);
		error(tmp);
	}

	size = (comBuffSize/sizeof(Int)) - buffHdrSize;

	/*JC: Se deja comentado la siguiente línea para futuros cambios en el caso de necesitarlo*/
	// Int fullsize = (buffHdrSize + size) * sizeof(Int); 
	Int fullsize = MAX_DATA;

	// Create buffer.
	// ---------------------------------------------------
	buffer = (MsgBuff) (tr_new char[fullsize]);
	nAssert(buffer);

	// Clear buffer.
	// ---------------------------------------------------
	buffer->h.clientId = ERR;
	buffer->h.objectId = ERR;
	buffer->h.retValue = ERR;
	buffer->h.serverOp = ERR;
	buffer->h.status   = ERR;
	buffer->h.flags_   = 0;
	*(buffer->data)    = 0;

	//
	// ---------------------------------------------------
	reset();
	receivedBytes = 0;
	roundTrips = 0;
	sending = false;
	sentBytes =0;

	// Receive/send comunication buffer size (message size).
	// ---------------------------------------------------
	if (connType == client) {
		if(useCompression) buffer->h.clientId = 1;
		buffer->h.retValue = comBuffSize;
		buffer->h.setLength(0);
	} else if (connType == server) {
		// Receive the Body
		buffer->h = hdr;
		_receiveBody();
	}
	if(useCompression)
		ccon = new CompressedConnection(scon,128,needConvert,fullsize);

#ifdef __NT__
	if (useKeepAlive) 
		keepAlive();
#endif
}

short NetConnection::getOp()
{
	if (snd_pdata>buffer->data || sendOp!=ERR || buffer->h.retValue!=ERR)
		flush();

	_fill();
	// It must be not a continuation message so I increment again rcv_cnt
	// (It was previously decremented by _fill());
	// It must nAssert that wasn't a message continuation.
	rcv_cnt++;
	return buffer->h.serverOp;
}

void NetConnection::flush()
{
	if (pMBuff != NULL)
		putC(0);

	_flush();
	buffer->h.retValue = ERR;
}

void NetConnection::error(const String &msg, ...)
{
	if (recursOnError) return;

	va_list ap;
#ifdef HAVE_VARARGS_BUG
    const String *&s = &msg;
    va_start(ap, s);
#elif HAVE_VC_VARARGS_BUG
	const char *s=toCharPtr(msg);
    	va_start(ap, s + 16);
#else
    va_start(ap, msg);
#endif
	SysLog sysLog(_processConfig()->name);
	sysLog.logError(toStr(toCharPtr(msg), ap));
	recursOnError = true;
	stop(1 | ABNORMAL_STOP);
}

void NetConnection::_flush()
{
	if (recursOnError) return;

	if (scon->status() != OKEY) {
		String tmp = String(libMsg("CANT_OPCON")) +
					 " Status: " + toString(Int(scon->status())) +
					 ", errno: " + toString(errno);
		error(tmp);
	}

	// Calculate message length and send it.
	Int len  = Int((char *) snd_pdata - (char *) buffer),
		dlen = Int((char *) snd_pdata - (char *) buffer->data);
 
	nAssert(len > 0);
 

	buffer->h.setLength(dlen);

	if (needConvert)
		invH(buffer);

 
	if(useCompression ) {
		if (ccon->send(len, (const char *) buffer) != len) {
			String tmp = libMsg("ERRSMSG") + toString(errno);
			error(tmp);
		}
   
	} else {
		if (scon->send(len, (const char *) buffer) != len) {
			String tmp = libMsg("ERRSMSG") + toString(errno);
			error(tmp);
		}
	}

	reset();

	sentBytes+=len;
	if(!sending) {
		roundTrips++;
		sending = true;
	}

}

Int NetConnection::_fill()
{
	if (recursOnError) return ERR;

	if (scon->status() != OKEY) {
		String tmp = String(libMsg("CANT_OPCON")) +
					 " Status: " + toString(Int(scon->status())) +
					 ", errno: " + toString(errno);
		error(tmp);
	}

	Int len, rcv;

	// Read header
	len = sizeof(s_msgHdr);
	sending = false;
	if(useCompression)
		rcv = ccon->receive(len, (char *) buffer);
	else
		rcv = scon->receive(len, (char *) buffer);
	if (rcv != len) {
		String tmp = libMsg("ERRRMSG") + toString(errno)
				   + ". Expected: " + toString(len)
				   + "; received: " + toString(rcv);
		error(tmp);
	}
	if (needConvert)
  
		invH(buffer);

	// Get the body
	rcv = _receiveBody();
	// If called from popWord I've to decrement rcv_cnt
    rcv_cnt--;
	return rcv;
}

								  

Int NetConnection::_receiveBody()
{
	Int len = buffer->h.length(),
		rcv = sizeof(s_msgHdr);
 
	// Read the body
	if (len > 0) {
		sending = false;
		if(useCompression)
			rcv = ccon->receive(len, (char *) buffer->data);
		else
			rcv = scon->receive(len, (char *) buffer->data);
   
		if (rcv != len) {
			String tmp = libMsg("ERRRMSG") + toString(errno)
					   + ". Expected: " + toString(len)
					   + "; received: " + toString(rcv);
			error(tmp);
		}
	}
	else {
		nAssert(len == 0);
	}

	rcv_cnt = len / sizeof(Int);
	snd_cnt = size - 1;

	rcv_pdata = snd_pdata = buffer->data;
	receivedBytes += (sizeof(s_msgHdr) + len);
	return rcv;
}

Connection &NetConnection::operator<<(Int i)
{
	nAssert(rcv_pdata == buffer->data);
	pushWord(needConvert ? invI(i) : i);
	return *this;
}

Connection &NetConnection::operator<<(const String &s)
{
	nAssert(rcv_pdata == buffer->data);
	return Connection::operator<<(s);
}

Connection &NetConnection::operator<<(const char *s)
{
	nAssert(rcv_pdata == buffer->data);
	return Connection::operator<<(s);
}

Connection &NetConnection::operator>>(Int &i)
{
	if (snd_pdata>buffer->data || sendOp!=ERR || buffer->h.retValue!=ERR)
		flush();

 
	popWord(i);
 
	if (needConvert)
		i = invI(i);
	if ((char *)rcv_pdata == (char *)buffer->data + buffer->h.length())
		reset();
	return *this;
}

Connection &NetConnection::operator>>(String &s)
{
	if (snd_pdata>buffer->data || sendOp!=ERR || buffer->h.retValue!=ERR)
		flush();

	Connection::operator>>(s);
 
	if ((char *)rcv_pdata == (char *)buffer->data + buffer->h.length())
		reset();
	return *this;
}

Connection &NetConnection::operator>>(char *&)
{
	nAssert(0);		// This function should never be used
	return *this;	// just for the warning
}

#include <ifound/stdc.h>
void NetConnection::putC(char c)
{
	if (pMBuff == NULL)
		pMBuff = mBuff;
	else if (pMBuff == mBuff+sizeof(Int)) {
		pushWord(*(Int*)mBuff);
		pMBuff = mBuff;
	}
	*pMBuff++ = c;
	if (c == '\0') {
		pushWord(*(Int*)mBuff);
		pMBuff = NULL;
	}
}

char NetConnection::getC()
{
	if (pMBuff == NULL || pMBuff == mBuff+sizeof(Int)) {
		pMBuff = mBuff;
		popWord(*(Int*)mBuff);
	}
	char c = *pMBuff++;
	if (c == '\0')
		pMBuff = NULL;
	return c;
}

// For cracker ...
void setPackageNumber(NetConnection *conn, Int pn)
{
	assert(conn->buffer);
	assert(conn->buffer->h.serverOp == ERR);
	conn->buffer->h.serverOp = pn;
}

Int getPackageNumber(NetConnection *conn)
{
	assert(conn->buffer);
	Int ret = conn->buffer->h.serverOp;
	conn->buffer->h.serverOp = ERR; // Reset !.
	return ret;
}
