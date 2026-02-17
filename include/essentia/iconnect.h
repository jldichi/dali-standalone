/****************************************************************
* Copyright (c) 1992 InterSoft Co.  All Rights Reserved
* THIS IS UNPUBLISHED PROPRIETARY SOURCE CODE OF InterSoft Co.
* The copyright notice above does not evidence any
* actual or intended publication of such source code.
*
* $Id: iconnect.h,v 1.4 2002/03/15 20:41:03 hardaiz Exp $
*
* DESCRIPTION
* Class definitions for Ipc Connection type.
*********************************************************************/

#ifndef ICONNECT_H
#define ICONNECT_H

#include <ifound.h>

#ifdef __GNUC__
#pragma interface
#endif

#include <essentia/dbconn.h>
#include <essentia/dbmcom.h>

class IpcConnection : public DbConnection
{

	friend class Shadow;
	friend class ShTube;

// private:

	// metodos que utilizan la clase Shadow, ShObject
	Int getActBuffSize() 			{ return actBuffSize; }
	void resetNMsgs()				{ nMsgs = 0; }
	MsgBuff getBuffer()				{ return buffer; }
	short getBuffFlags()			{ return short(buffer->h.flags()); }
	void setBuffFlags(short f)		{ buffer->h.setFlags(f); }
	void setCurrMsgLength(Int l)	{ buffer->h.setLength(l); }
	void setDefCurrMsgLength()
		{  setCurrMsgLength(snd_pdata - buffer->data);			}
	void setPtrBuff(Int len)
		{  snd_pdata = (Int *)((char *)(buffer->data) + len);	}

	Int getClientId()				{ return Connection::clientId; }

	void createError(const String &funct, int err);
	bool checkIntr(const String &msg, int errNo);

	int stchannel;
	int snd_channel;
	int cont_channel;
	int rcv_channel;
	int semid;

	void _flush();
	Int _fill();
	bool stopSv();

	bool connExists(char t, const String &hst, Int id, const String &svName)
	{ return DbConnection::connExists(t, hst, id, svName); }

	IpcConnection(const IpcConnection &);
	
public:

	IpcConnection(Int key, String svName,
#ifndef __VC__
				  ConnMode abort = abortAlways);
#else
 				  ConnMode abort = ConnMode(abortOnInit|abortOnRun));
#endif
	~IpcConnection();

	// rutina para la inicializacion de la ejecucion de la rutina
	void begin(Int io, SvMessage so);
	void connect(bool secServ = false);
	bool waitForServerHalt();
};

#endif // ICONNECT_H
