/********************************************************************
* Copyright (c) 1993 InterSoft Co.  All Rights Reserved
* THIS IS UNPUBLISHED PROPRIETARY SOURCE CODE OF InterSoft Co.
* The copyright notice above does not evidence any
* actual or intended publication of such source code.
*
*
* $Id: bobjserv.h,v 1.1.1.1 1996/04/16 18:51:51 gustavof Exp $
*
*
* DESCRIPTION:
*
* ROUTINE       |  PURPOSE
*---------------+---------------------------------------------------
*
*********************************************************************/

#ifndef BOBJSERV_H
#define BOBJSERV_H

#ifdef __GNUC__
#pragma interface
#endif

#include <usrsrv/shadobj.h>
#include <ifound/ns.h>
#include <ifound/hashtab.h>
#include <ifound/parray.h>
#include <essentia/bobjid.h>

#define IO_NO_BINARY	(-3)		// defined in <cfix/db.h>

//	Class BinaryObjServer
//
class BinaryObjServer : public ShadowObject {

	DeclareNameSpace

	Int    fp_;
	char   *buffer_;
	OsFile *ifile_;	
    BinaryObjId bobjid_;
    
	void monitor(BinaryObjId &bobjid);
	String serverName(BinaryObjId &bobjid) const;		

public:              

	BinaryObjServer();
    virtual ~BinaryObjServer();
                    
	void   getBinaryObj();
	void   storeBinaryObj();
	void   discardBinaryObj();
	void   getBinaryInfo();

};

#define ReturnToClient(sts)	 { scon->prepare(sts); 	 \
							   scon->reply();     	 \
				  			   return; }

#endif
