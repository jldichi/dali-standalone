/********************************************************************
* Copyright (c) 1993 InterSoft Co.  All Rights Reserved
* THIS IS UNPUBLISHED PROPRIETARY SOURCE CODE OF InterSoft Co.
* The copyright notice above does not evidence any
* actual or intended publication of such source code.
*
* $Id: bobjid.cc,v 1.2 2004/09/29 21:35:24 damianm Exp $
*
* DESCRIPTION
*********************************************************************/

#include <string.h>

#include <ifound.h>        
#include <ifound/str.h>
#include <ifound/priv/gfuncs.h>

#include <essentia/dbconn.h>
#include <essentia/cnffile.h>
#include <essentia/bobjid.h>

//
//	Class BinaryObjId
//
BinaryObjId::BinaryObjId()
	: cid_(0), iid_(0), fid_(0), 
	  fld_(ERR), btype_(ERR)
{                
}

BinaryObjId::BinaryObjId(const String &s)
	: fld_(ERR), btype_(ERR)
{
	cid_ = toInt(s( 0, 8), 16);
	iid_ = toInt(s( 8, 8), 16);
	fid_ = toInt(s(16, 4), 16);
}

BinaryObjId::BinaryObjId(Int cid, Int iid, Int fid)
	: cid_(cid), iid_(iid), fid_(fid),
	  fld_(ERR), btype_(ERR)
{                
}

// Get the path of a binary object.
// (This method should only be used within that shadow)

String BinaryObjId::path(const String &svname) const
{    
	String aux = toString(*this);
		
	ConfigFile cfile;
    cfile.read(svname, ICON);		// read .sv file
	return String(cfile.objectRepository() + PATH_SLASH + aux);	
}

void BinaryObjId::store(char *&buff) const
{
	stlong(cid_, buff);
	buff += sizeof(Int);
	stlong(iid_, buff);
	buff += sizeof(Int);
	stlong(fid_, buff);
	buff += sizeof(Int);
	stlong(fld_, buff);
	buff += sizeof(Int);
	stlong(btype_, buff);
	buff += sizeof(Int);
}

void BinaryObjId::load(const char *&buff)
{
	cid_  = ldlong(buff);
	buff += sizeof(Int);
	iid_  = ldlong(buff);
	buff += sizeof(Int);
	fid_  = ldlong(buff);
	buff += sizeof(Int);
	fld_  = ldlong(buff);
	buff += sizeof(Int);
	btype_ = ldlong(buff);
	buff += sizeof(Int);
}

Int BinaryObjId::storeLength()
{
	return 5*sizeof(Int);	
}
	
Connection& operator>>(Connection &conn, BinaryObjId &bobjid)
{
	Int cid, iid, fid, fld, btype;
	conn >> cid >> iid >> fid >> fld >> btype;
	bobjid.cid_ = cid; 
	bobjid.iid_ = iid; 
	bobjid.fid_ = fid;                       
	bobjid.fld_ = fld;
	bobjid.btype_ = btype;
	return conn;
}

Connection& operator<<(Connection &conn, const BinaryObjId &bobjid)
{
	conn << bobjid.cid_ << bobjid.iid_ << bobjid.fid_
	     << bobjid.fld_ << bobjid.btype_;  
	return conn;
}

String toString(const BinaryObjId &bobjid)
{
	char aux[25];            
	sprintf(aux, "%08X%08X%04X", bobjid.cid_, bobjid.iid_, bobjid.fid_);
	return String(aux);
}

bool operator==(BinaryObjId &b1, BinaryObjId &b2)
{             
	return (b1.cid_ == b2.cid_ && b1.iid_ == b2.iid_ &&
			b1.fid_ == b2.fid_);
}

bool operator!=(BinaryObjId &b1, BinaryObjId &b2)
{
	return (b1.cid_ != b2.cid_ || b1.iid_ != b2.iid_ ||
			b1.fid_ != b2.fid_);
}

