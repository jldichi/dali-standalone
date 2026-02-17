/********************************************************************
* Copyright (c) 1995 InterSoft Co.  All Rights Reserved
* THIS IS UNPUBLISHED PROPRIETARY SOURCE CODE OF InterSoft Co.
* The copyright notice above does not evidence any
* actual or intended publication of such source code.
*
* $Id: compconn.cc,v 1.3 2004/09/29 21:35:56 damianm Exp $
*
* DESCRIPTION:
*
*********************************************************************/

#ifdef __GNUC__
#pragma implementation
#endif

#include <string.h>

#include <ifound.h>
#include <ifound/stdc.h>
#include <essentia/compconn.h>
#include <ifound/priv/pconfig.h>
#include <ifound/priv/macros.h>

#include <errno.h>


CompressedConnection::CompressedConnection
(SocketConnection *s, Int _commBuffSize, bool _needConvert,Int _bufLen)
: scon(s), commBuffSize(_commBuffSize), bufLen(_bufLen),
needConvert(_needConvert),recDataLen(0)
{
	dataPtr=buffer = new char[bufLen];
	compressedBuffer= new char[bufLen];
	assert(buffer && compressedBuffer);
}

CompressedConnection::~CompressedConnection()
{
	delete[] buffer;
	delete[] compressedBuffer;
}

Int CompressedConnection::receive(Int len, char *data)
{
	assert(len >= sizeof(Int));
	if(!recDataLen){
		_fill();
		assert(dataPtr == buffer);
	}
	assert((len <= recDataLen-(dataPtr-buffer)));
		// There is enough remain data
	memcpy(data,dataPtr,len);
	dataPtr+=len;
//	recDataLen-=len;
	if(dataPtr==buffer+recDataLen){
		recDataLen=0;
		dataPtr=buffer;
	}
	return len;
}

Int CompressedConnection::send(Int len, const char *data)
{
	assert(len >= sizeof(Int));
	assert(len <= bufLen);
	assert(!recDataLen);
	Int sendlen = len;
	if(len >= MIN_COMPRESS_LEN){
		sendlen = compress(len,(char *)data);
		assert(sendlen + sizeof(Int) <= bufLen);
		memcpy(buffer+sizeof(Int),compressedBuffer,sendlen);
		*((Int *) buffer)= needConvert ? invI(sendlen) : sendlen;
		sendlen += sizeof(Int);
	}
	else
		memcpy(buffer,data,len);
	dataPtr = buffer;
	Int clen;
	while(clen = ((sendlen > commBuffSize) ? commBuffSize : sendlen)){
		if(scon->send(clen,(const char *) dataPtr) != clen)
			return ERR;
		sendlen -=clen;
		dataPtr += clen;
	}
	(*(Int *)buffer)=ERR;
	dataPtr = buffer;
	recDataLen = 0;

	return len;
}


inline void CompressedConnection::_checkRcv(Int rcv,Int expect)
{
	if (rcv != expect) {
		String tmp = libMsg("ERRRMSG") + toString(errno)
				   + ". Expected: " + toString(expect)
				   + "; received: " + toString(rcv);
				error(tmp);
	}
}

void CompressedConnection::_fill()
{
	Int size;
	Int rcv = scon->receive(sizeof(Int),( char *)&size);
	_checkRcv(rcv,sizeof(Int));
	dataPtr=buffer;
	if(size == ERR) {	// Old non -compressed protocol
		s_msgHdr * phdr = (s_msgHdr *)buffer;
		recDataLen=sizeof(s_msgHdr);
		rcv = scon->receive(recDataLen-sizeof(Int),((char *) phdr)+sizeof(Int));
		_checkRcv(rcv,recDataLen-sizeof(Int));
		short len = phdr->length();
		if(needConvert) {
  			invH((s_MsgBuff *)phdr);
			len = phdr->length();
			phdr->clientId = ERR;
  			invH((s_MsgBuff *)phdr);
		}
		if(len <= 0)
			return;
		rcv=scon->receive(len,buffer+recDataLen);
		_checkRcv(rcv,len);
		recDataLen += len;
	} else {
	if(needConvert)
		size = invI(size);
		rcv = scon->receive(size,( char * )compressedBuffer );
		_checkRcv(rcv,size);
		uncompress(size);
	}
}

#include <ifound/bred.h>

Int CompressedConnection::compress(Int len, char *data)
{
	assert(len > 0);
	Int blen = bufLen;
	Int res = reduce(data,len, &compressedBuffer, &blen, DEF_COMPLEVEL);
	assert(res > 0);
	return res;

}

void CompressedConnection::uncompress(Int size)
{
	recDataLen = bufLen;
	Int status =expand(compressedBuffer,size, &buffer, &recDataLen, DEF_COMPLEVEL);
	if(status < 0)
		error(libMsg("UNCOMPERR"),status);
	return;
}
