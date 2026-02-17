/********************************************************************
* Copyright (c) 1992 InterSoft Co.  All Rights Reserved
* THIS IS UNPUBLISHED PROPRIETARY SOURCE CODE OF InterSoft Co.
* The copyright notice above does not evidence any
* actual or intended publication of such source code.
*
* $Id: dbmcom.h,v 1.5 2003/05/29 22:55:47 albertoi Exp $
*
* DESCRIPTION
* Definicion de las macros para la comunicacion
*********************************************************************/

#ifndef DBMCOM_H
#define DBMCOM_H

#include <ifound.h>

#ifdef __GNUC__
#pragma interface
#endif

// ESSENTIA version * 1000
#define ESSENTIA_VERSION			3302

// ESSENTIA protocol version * 1000
#define ESSENTIA_PROTOCOL_VERSION	1100

#define MANGLEOID(oId)		((oId) | ((((oId) & 0x7) ^ 0x7) << 28))
#define UNMANGLEOID(oId)	((oId) & ~(0x7 << 28))
#define SERVERID			MANGLEOID(0L)
#define NOEXECUTE			(-1234)

// internal codes for each message
enum SvMessage {
	DBM_FIRSTMSG,
	DBM_ATTACH = DBM_FIRSTMSG,
	DBM_DETACH,
	DBM_LOCK,
	DBM_FREE,
	DBM_CLEAR,
	DBM_RENAME,
	DBM_GETINFO,
	DBM_GETTABLES,
	DBM_CONNECT,
	DBM_DISCONNECT,
	DBM_GETCLIENTSINFO,
	DBM_FINDRECORD,
	DBM_GETRECORD,
	DBM_PUTRECORD,
	DBM_DELRECORD,
	DBM_ADDRECORD,
	DBM_UPDATERECORD,
	DBM_GETNEXTBLOCK,
	DBM_GETPREVBLOCK,
	DBM_SETRANGE,
	DBM_FREERECORD,
	DBM_DROP,
	DBM_CREATE,
	DBM_BUILD,
	DBM_INSKEY,
	DBM_SHUT,
	DBM_STOP,
	DBM_CHECKPOINT,
	DBM_BEGTRANSACT,
	DBM_ENDTRANSACT,
	DBM_ABORTTRANSACT,
	DBM_NEWVERSION,
	DBM_CARDINAL,
	DBM_SETATTRIB,
	DBM_DEBUG,
	DBM_MONITOR,
	DBM_GETDATAFILES,
	DBM_GETVERSINFO,
	DBM_SETPRIVG,
	DBM_ALTER,
	DBM_COPY,
	DBM_REPLACE,
	DBM_ESTIMATION,
	DBM_ACTIVATION,
	DBM_OBJMETHODEXEC,
	DBM_READYTOEND,
	DBM_SHADOWDBG,
	DBM_ADDRECORDTEST,
	
	// Must be the last one!
	DBM_LASTMSG,
};

enum GetInfoType {
	GI_SCHEMAS		= 1 << 0,
	GI_CONFIGFILE	= 1 << 1,
	GI_TRANSTATUS	= 1 << 2,
	GI_TRANSTATE	= 1 << 3,
	GI_FLUSH		= 1 << 4,
	GI_TIMESTAMP	= 1 << 5,
	GI_STATS		= 1 << 6,
};

enum TransactionMode {
	TRANS_NORMAL,
	TRANS_OPTIMISTIC,
};

enum Flag {
	MSG_DONE = 0x00,
	MSG_CONT = 0x01,
};

enum SvMessageAttribute {
	SVMSG_NULL			= 0,
	SVMSG_VOID			= 1 << 0,
	SVMSG_TERMTRANS		= 1 << 1,
	SVMSG_CONNECT		= 1 << 2,
	SVMSG_STOP			= 1 << 3,
	SVMSG_DISCONNECT	= 1 << 4,
	SVMSG_MODIFIES		= 1 << 5,
};

bool mayModify(SvMessage operation);
bool isDisconnect(SvMessage operation);
bool isConnect(SvMessage operation);
bool terminateTrans(SvMessage operation);
bool isVoid(SvMessage operation);
bool isStop(SvMessage operation);

// MsgBuff lives, now, at msghdr.h.
#include <essentia/msghdr.h>

//#define COM_BUFF_SIZE  max(1024, toInt(String("COMBUFFSIZE").getEnv()))
#define COM_BUFF_SIZE	(256*sizeof(long))		// sizeof(Int)

#endif
