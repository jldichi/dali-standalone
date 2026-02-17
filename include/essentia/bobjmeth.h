/********************************************************************
* Copyright (c) 1993 InterSoft Co.  All Rights Reserved
* THIS IS UNPUBLISHED PROPRIETARY SOURCE CODE OF InterSoft Co.
* The copyright notice above does not evidence any
* actual or intended publication of such source code.
*
* $Id: bobjmeth.h,v 1.1.1.1 1996/04/16 18:51:49 gustavof Exp $
*
* DESCRIPTION
*********************************************************************/

#ifndef BOJBMETH_H
#define BOBJMETH_H

#include <essentia/msghdr.h>                          
#include <essentia/dbmcom.h>

#define PACKET_SIZE		(COM_BUFF_SIZE - sizeof(s_msgHdr)*2)

#define BinaryObjServerMethods				\
{											\
	&BinaryObjServer::getBinaryObj,	    	\
	&BinaryObjServer::storeBinaryObj, 		\
	&BinaryObjServer::discardBinaryObj,	    \
	&BinaryObjServer::getBinaryInfo,	    \
}

enum BinaryObjServerMessage {
	BinaryObjServer_getBinaryObj,
	BinaryObjServer_storeBinaryObj,
	BinaryObjServer_discardBinaryObj,
	BinaryObjServer_getBinaryInfo,

	// Must be the last one!
	BinaryObjServer_lastmsg,
};

#endif
