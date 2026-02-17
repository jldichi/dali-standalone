/********************************************************************
* Copyright (c) 1995 InterSoft Co.  All Rights Reserved
* THIS IS UNPUBLISHED PROPRIETARY SOURCE CODE OF InterSoft Co.
* The copyright notice above does not evidence any
* actual or intended publication of such source code.
*
* $Id: compconn.h,v 1.1 1996/07/19 22:14:50 gab Exp $
*
* DESCRIPTION:
*
*********************************************************************/

#ifndef COMPCONN_H
#define COMPCONN_H

#ifdef __GNUC__
#pragma interface
#endif

#include <essentia/connect.h>
#include <essentia/sockconn.h>
const Int MIN_COMPRESS_LEN = 128;
const Int DEF_COMPLEVEL	= 2;

class CompressedConnection
{
	SocketConnection *scon;
	bool needConvert;
	Int bufLen;
	Int commBuffSize;
	Int recDataLen;
	char *buffer;
	char *compressedBuffer;
	char *dataPtr;

	void _fill();
	void _flush();
	Int compress(Int,char*);
	void uncompress(Int);
	inline void _checkRcv(Int,Int);

public:
	CompressedConnection(SocketConnection *s, Int _commBuffSize, bool _needConvert, Int _bufLen );
	~CompressedConnection();
	Int send(Int, const char *);
	Int receive(Int, char *);
};

#endif
