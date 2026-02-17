/********************************************************************
* Copyright (c) 1993 InterSoft Co.  All Rights Reserved
* THIS IS UNPUBLISHED PROPRIETARY SOURCE CODE OF InterSoft Co.
* The copyright notice above does not evidence any
* actual or intended publication of such source code.
*
* $Id: bobjcli.h,v 1.1.1.1 1996/04/16 18:51:48 gustavof Exp $
*
* DESCRIPTION
*********************************************************************/

#ifndef BOJBCLI_H
#define BOJBCLI_H

#include <ifound.h>            
#include <essentia/dbclobj.h>
#include <cfix/dbbin.h>

// Also defined in <cfix/dbbin.h>
#define IO_OPEN_BINARY	(-2)
#define IO_NO_BINARY	(-3)

#define MAGIC_SIZE		(8)

class BinaryObjId;
class DbConnection;
struct BinaryInfo;

//	Class BinaryObjClient
//
class BinaryObjClient : public ClientObject {

	Int		fp_;
    char    *buffer_;
    Int		file_;
    
public:
	explicit BinaryObjClient(DbConnection *conn);
	~BinaryObjClient();
	
	Int getBinaryObj(const BinaryObjId &bobjid, const String &path);
	Int getBinaryObj(const BinaryObjId &bobjid, void *buffer, Int size);
	Int storeBinaryObj(const BinaryObjId &bobjid, const String &path);
	Int storeBinaryObj(const BinaryObjId &bobjid, void *buffer,	Int size);
	Int discardBinaryObj(const BinaryObjId &bobjid);
	Int getBinaryInfo(const BinaryObjId &bobjid, BinaryInfo *binaryInfo);
		
};

#endif
