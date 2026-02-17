/********************************************************************
* Copyright (c) 1993 InterSoft Co.  All Rights Reserved
* THIS IS UNPUBLISHED PROPRIETARY SOURCE CODE OF InterSoft Co.
* The copyright notice above does not evidence any
* actual or intended publication of such source code.
*
* $Id: bobjcli.cc,v 1.7 2007/03/07 18:42:22 hardaiz Exp $
*
* DESCRIPTION
*********************************************************************/

#include <stdlib.h> 
#include <fcntl.h>
#include <string.h>
#ifdef _AIX
#include <strings.h>
#endif
#include <unistd.h>


#include <ifound.h>
#include <ifound/stdc.h>
#include <ifound/namespac.h>         
#include <essentia/rconnect.h>
#include <essentia/dbconst.h>   
#include <cfix/dbbin.h>

#include <essentia/bobjid.h>
#include <essentia/bobjmeth.h>            
#include <essentia/bobjcli.h>

//
//	Class BinaryObjClient
//
BinaryObjClient::BinaryObjClient(DbConnection *conn)
	: ClientObject("BinaryObjClient", conn),
	  fp_(ERR), file_(ERR)
{
	long vers, owner;
	String descr;

    require( con != NULL );
	
	objectId = (*con) (DBM_ATTACH) << (Int)SCHEMA
					  << (Int)false << (Int)false
					  << "BinaryObjServer" >> vers >> owner >> descr;
					   
	buffer_ = tr_new char[PACKET_SIZE];

	ensure( objectId != ERR );								   
}

BinaryObjClient::~BinaryObjClient()
{   
	tr_delete_vec(buffer_);
}

// Returns the # of bytes read from the connection,
// the value must be equal to the size of the file.

Int BinaryObjClient::getBinaryObj(const BinaryObjId &bobjid, 
	const String &path)
{
	Int result;
	String aux;

    fp_ = 0;
	file_ = _global_open(toCharPtr(path), O_CREAT|O_TRUNC|O_WRONLY);
    if (file_ == ERR)
    	return IO_OPEN_BINARY;		

	do {
		result = (*con) (objectId, DBM_OBJMETHODEXEC) 
		  		 << (Int)BinaryObjServer_getBinaryObj 
		  		 << bobjid << (Int)PACKET_SIZE;
		
		if (result != OK) return result;
		
		(*con) >> aux;		// Beware!! - Binary string
        Int nread = aux.length();

		mCopy(buffer_, toCharPtr(aux), nread); 		// yet another copy !
		_global_write(file_, buffer_, nread);
    	fp_ += nread;

	} while (result != ERR);
	    
    _global_close(file_);
    
	return fp_;
}

// A positive (or zero) value returned indicates the number of
// bytes actually copied (< size). This function is intended to
// be called several times in order to get an object.

Int BinaryObjClient::getBinaryObj(const BinaryObjId &bobjid, 
	void *buffer, Int chunkSize)
{
	String aux;          

	if (chunkSize <= 0 || chunkSize > PACKET_SIZE)
		return IO_INV_BUFFER;

	Int result = (*con) (objectId, DBM_OBJMETHODEXEC) 
		  		 << (Int)BinaryObjServer_getBinaryObj
		  		 << bobjid << chunkSize;

	if (result != OK) return result;

	(*con) >> aux;		// Beware!! - Binary string
	Int nread = aux.length();
	mCopy(buffer, toCharPtr(aux), nread);
	    
	return nread;    	// the # of bytes actually read
}

// Returns the # of bytes written to the connection,
// the value must be equal to the size of the file.

Int BinaryObjClient::storeBinaryObj(const BinaryObjId &bobjid, 
	const String &path)
{
	fp_ = 0;

	file_ = _global_open(toCharPtr(path), O_RDONLY);
    if (file_ == ERR)
    	return IO_OPEN_BINARY;		

	Int nread;
	do {
		nread = _global_read(file_, buffer_, PACKET_SIZE);
	    if (nread > 0) fp_ += nread; else nread = 0;
	    	
		String aux(nread, buffer_);		// binary string (yet another copy!)
		Int result = (*con) (objectId, DBM_OBJMETHODEXEC) 
			  		 << (Int)BinaryObjServer_storeBinaryObj 
			  		 << bobjid << aux;
		assert(result == OK);
	} while (nread > 0);
	
	con->flush();
	
	_global_close(file_);		// closes the file
	
	return fp_;
}

// Returns the # of bytes written to the connection, the value 
// must always be 'chunkSize'. This function is intended to be
// called several times in order to store an object.

Int BinaryObjClient::storeBinaryObj(const BinaryObjId &bobjid, 
	void *buffer, Int chunkSize)
{
	if (chunkSize < 0 || chunkSize > PACKET_SIZE)
		return IO_INV_BUFFER;

	buffer_ = (char *) buffer;

	// A buffer with chunkSize equal to 0 is used to 
	// indicate the end of the object.
		
	String aux(chunkSize, buffer_);
	Int result = (*con) (objectId, DBM_OBJMETHODEXEC) 
			  		 << (Int)BinaryObjServer_storeBinaryObj 
			  		 << bobjid << aux;

	if (result != OK) return result;
	
	return chunkSize;
}

// Discards an object associated to a record.

Int BinaryObjClient::discardBinaryObj(const BinaryObjId &bobjid)
{
	Int result = (*con) (objectId, DBM_OBJMETHODEXEC) 
					 << (Int)BinaryObjServer_discardBinaryObj << bobjid; 
	return result;
}

// Retrieves information related to an object. Its modification
// time and its size (for the moment).

Int BinaryObjClient::getBinaryInfo(const BinaryObjId &bobjid, 
	BinaryInfo *binaryInfo)
{                     
	require( binaryInfo != NULL );
	
	Int result = (*con) (objectId, DBM_OBJMETHODEXEC) 
					 << (Int)BinaryObjServer_getBinaryInfo << bobjid; 

	if (result == IO_NO_BINARY) return result;

	Int mtime, size;
	(*con) >> mtime >> size;
	binaryInfo->timeStamp = mtime;
	binaryInfo->actualSize = size;

	return result;
}


