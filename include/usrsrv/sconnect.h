/********************************************************************
* Copyright (c) 1992 InterSoft Co.  All Rights Reserved
* THIS IS UNPUBLISHED PROPRIETARY SOURCE CODE OF InterSoft Co.
* The copyright notice above does not evidence any
* actual or intended publication of such source code.
*
* $Id: sconnect.h,v 1.3 2000/01/31 18:17:41 diegon Exp $
*
* DESCRIPTION
* Class definitions for remote Connection type.
*********************************************************************/

#ifndef SCONNECT_H
#define SCONNECT_H

#include <ifound.h>

#ifdef HAVE_PRAGMA
#pragma interface
#endif

#include <essentia/connect.h>
#include <essentia/sockconn.h>
#include <usrsrv/shdw.h>
#include <essentia/except.h>

class ShadowConnection : public Connection {

	useIdCheck(0x87AE32C1);

// private:

	Int len;			// actual msg length
	Int objectId0;		// actual msg object Id
	short serverOp0;	// actual msg server option
	short flags0;		// actual msg continuation flag
	Int length0;		// actual msg length

	SocketConnection *con;

	void _flush();
	Int _fill();

	void readBuffer(char *);

// !!! DESDE ACA es la parte que necesitan Shadow y ShTube

	ShadowConnection(const ShadowConnection &);
	ShadowConnection(ExecMode mode = AUTOMATIC, String service = NULL_STRING);
#ifndef __NT__
	~ShadowConnection();
#endif
	// message execution initialization
	void begin(Int io, SvMessage so);

	Int getClientId()	 { shIdCheck(); return len; }
	Int getObjectId()	 { shIdCheck(); return objectId0; }
	short getServerOp()	 { shIdCheck(); return serverOp0; }
	short getFlags()	 { shIdCheck(); return flags0; }
	Int getLength()		 { shIdCheck(); return length0; }

#ifndef __NT__
	void readHdr();
#endif

	// to read the body but not the head in an external buffer (buffer->data)
	Int readMsg(Int *);
	Int readMsg(MsgBuff);

	void writeMsg();
	void writeMsg(MsgBuff, Int length);

	// message reply buffers
	void replyStatus(Int);

	// buffer handling
	MsgBuff getBuffer(Int &length)
	{ shIdCheck(); length = size; return buffer; }

	void setLen(Int length, short flags)
	{
		shIdCheck();
		rcv_pdata = snd_pdata = (Int *)((char *)(buffer->data) + length);
		buffer->h.setFlags(flags);
	}

	// send a complete buffer
	void snd(Int length, short flags)
	{ shIdCheck(); setLen(length, flags); _flush(); }

	// flag setting (MSG_CONT, MSG_DONE or MSG_VOID)
	void setFlags(short i) { shIdCheck(); buffer->h.setFlags(i); }

	/*
		Esto es un primer approach para darle a las subclases de
		Name Space una interface mas definida y restringida.
		Seguramente se podria hacer algo mejor que estos friend ...
	*/
	friend class Shadow;
	friend class ShTube;

public:
#ifdef __NT__
	void readHdr();
	SocketConnection *getSocketConnection() { return con; }
	~ShadowConnection();
#endif
    ShadowConnection(const String &,Int);

	Int readMsg();

	void prepare(Int);
	void reply();

	void replyException(const Exception &);

	void resizeBuffer(Int);
        void resizeBuffer(MsgBuff,Int);

};

#endif // SCONNECT_H
