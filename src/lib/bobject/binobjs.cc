/********************************************************************
* Copyright (c) 1992 InterSoft Co.  All Rights Reserved
* THIS IS UNPUBLISHED PROPRIETARY SOURCE CODE OF InterSoft Co.
* The copyright notice above does not evidence any
* actual or intended publication of such soucone code.
*
* $Id: binobjs.cc,v 1.2 2004/09/29 21:35:31 damianm Exp $
*
* DESCRIPTION
*********************************************************************/

#include <string.h>

#include <ideafix.h>
#include <cfix.h>

#include <ifound/priv/gfuncs.h>

#include <essentia/dbconst.h>
#include <essentia/dbconn.h>
#include <essentia/except.h>
#include <essentia/spriv.h>
#include <essentia/bobjid.h>
#include <essentia/bobjcli.h>

#include <ideafix/ixinchk.h>
#include <ideafix/priv/gndefs.h>
#include <ideafix/priv/dbdefs.h>

static const char *binaryTypeDesc[] = {
	"",		   		// BINARY_EMPTY
	"TEXT",			// BINARY_TEXT
	"GIF",			// BINARY_GIF
	"PM",			// BINARY_PM
	"PBM",			// BINARY_PBM
	"XBM",			// BINARY_XBM
	"BMP",			// BINARY_BMP
	"PCX",			// BINARY_PCX
	"TIFF",			// BINARY_TIFF
	"WAV",			// BINARY_WAV
	"???",			// BINARY_UNKNOWN
};

static BinaryObjClient* binaryObjClient(DbConnection *conn)
{      
	static BinaryObjClient *_binaryObjClient = NULL;
	return (_binaryObjClient) ? _binaryObjClient :
			(_binaryObjClient = tr_new BinaryObjClient(conn));
}

// NOTE: For internal use only (C++ linkage)
//
int _GetBinaryToFile(DbConnection *conn, const BinaryObjId &bobjid, char *path)
{
	return binaryObjClient(conn)->getBinaryObj(bobjid, path);
}

// Functions for retrieving and storing binary data
//
extern "C" int GetBinaryToFile(dbfield f, char *path)
{
	struct s_table *tp = _GetTabPtr(f);
    record_prefix *rp = (record_prefix *) _GetTabBuff(f);

	BinaryObjId bobjid(tp->u.es.objectId, rp->oid.iid,  FLDTOI(f));
	return binaryObjClient(tp->u.es.con)->getBinaryObj(bobjid, path);
}

extern "C" int GetBinaryToMem(dbfield f, void *buffer, long size)
{
	struct s_table *tp = _GetTabPtr(f);
    record_prefix *rp = (record_prefix *) _GetTabBuff(f);

	BinaryObjId bobjid(tp->u.es.objectId, rp->oid.iid, FLDTOI(f));
	return binaryObjClient(tp->u.es.con)->getBinaryObj(bobjid, buffer, size);
}

extern "C" int StoreBinaryFromFile(dbfield f, char *path)
{
	int result;
	dbtable tab = f & 0x00ffff00;
	
	// Store the binary type and rewrite the record
	SetLFld(f, (long) IdentifyBinaryType(path));
    PutRecord(tab);

	// Flush the connection and reread the record to get the iid
	if (GetRecord(tab, THIS_KEY, IO_DEFAULT) < 0)
		return ERR;

	// Store the binary object (Shadow)
	struct s_table *tp = _GetTabPtr(f);
    record_prefix *rp = (record_prefix *) _GetTabBuff(tab);
	BinaryObjId bobjid(tp->u.es.objectId, rp->oid.iid, FLDTOI(f));
	result = binaryObjClient(tp->u.es.con)->storeBinaryObj(bobjid, path);
    
	return result;
}

extern "C" int StoreBinaryFromMem(dbfield f, void *buffer, long size)
{
	int result;
	dbtable tab = f & 0x00ffff00;
	
	// Store the binary type and rewrite the record
	SetLFld(f, (long) IdentifyMagic(buffer, size));
    PutRecord(tab);

	// Flush the connection and reread the record to get the iid
	if (GetRecord(tab, THIS_KEY, IO_DEFAULT) < 0)
		return ERR;

	// Store the binary object (Shadow)
	struct s_table *tp = _GetTabPtr(f);
    record_prefix *rp = (record_prefix *) _GetTabBuff(tab);
	BinaryObjId bobjid(tp->u.es.objectId, rp->oid.iid, FLDTOI(f));
	result = binaryObjClient(tp->u.es.con)->storeBinaryObj(bobjid, buffer, size);

	return result;
}

extern "C" int DiscardBinary(dbfield f)
{
	dbtable tab = f & 0x00ffff00;

	// Store the binary type and rewrite the record
	SetLFld(f, (long) BINARY_EMPTY);
    PutRecord(tab);

	// Flush the connection and reread the record to get the iid
	if (GetRecord(tab, THIS_KEY, IO_DEFAULT) < 0)
		return ERR;

	struct s_table *tp = _GetTabPtr(f);
    record_prefix *rp = (record_prefix *) _GetTabBuff(f);

	BinaryObjId bobjid(tp->u.es.objectId, rp->oid.iid, FLDTOI(f));
	return binaryObjClient(tp->u.es.con)->discardBinaryObj(bobjid);
}

// This function gets information of a particular binary
// object stored in the server.
//
extern "C" int GetBinaryInfo(dbfield f, BinaryInfo *binaryInfo)
{
	struct s_table *tp = _GetTabPtr(f);
    record_prefix *rp = (record_prefix *) _GetTabBuff(f);

	// Get the binary type from the table's buffer
	binaryInfo->binaryType = (BinaryType) LFld(f);

	// Get the size and mofication time from the server	
	BinaryObjId bobjid(tp->u.es.objectId, rp->oid.iid,  FLDTOI(f));
	return binaryObjClient(tp->u.es.con)->getBinaryInfo(bobjid, binaryInfo);
}

// Functions for retrieving and storing text data
// 1st version: the text type is no enforced !!
//
extern "C" int GetTextToFile(dbfield f, char *path)
{
	return GetBinaryToFile(f, path);
}

extern "C" int GetTextToMem(dbfield f, void *buffer, long size)
{
	return GetBinaryToMem(f, buffer, size);
}

extern "C" int StoreTextFromFile(dbfield f, char *path)
{
	return StoreBinaryFromFile(f, path);
}

extern "C" int StoreTextFromMem(dbfield f, void *buffer, long size)
{
	return StoreBinaryFromMem(f, buffer, size);
}

extern "C" int DiscardText(dbfield f)
{
	return DiscardBinary(f);
}

// Functions for determining the type of a binary datum
//
extern "C" BinaryType IdentifyMagic(void *buffer, long size)
{
	BinaryType filetype = BINARY_UNKNOWN;		// unknown type

	if (size < MAGIC_SIZE)
		return filetype;

	char *magic = (char*) buffer;
	if (strncmp(magic, "GIF87", 5) == 0) 
		filetype = BINARY_GIF;
	else if (strncmp(magic, "VIEW", 4) == 0 ||
			 strncmp(magic, "WEIV", 4) == 0) 
		filetype = BINARY_PM;
	else if (magic[0] == 'P' && magic[1] >= '1' && magic[1] <= '6') 
		filetype = BINARY_PBM;
	else if (strncmp(magic, "#define", 7) == 0) 
		filetype = BINARY_XBM;
	else if (magic[0] == 'B' && magic[1] == 'M') 
		filetype = BINARY_BMP;
	else if (magic[0] == 0x0a && magic[1] <= 5) 
		filetype = BINARY_PCX;
	else {
		int i;
		for (i = 0; i < MAGIC_SIZE && isascii(magic[i]); i++)
			;		// if the magic is ascii, is likely to be text
		if (i == MAGIC_SIZE)
			filetype = BINARY_TEXT;		  			
	}
	    	
	return filetype;
}

extern "C" BinaryType IdentifyBinaryType(char *path) 
{
	FILE *file = fopen(path, "rb");
	if (file == NULL)
		return BINARY_EMPTY;

	char magic[MAGIC_SIZE + 1];
	fread(magic, MAGIC_SIZE, 1, file);
	fclose(file);

	return IdentifyMagic(magic, MAGIC_SIZE);
}

extern "C" const char *BinaryTypeDesc(BinaryType bintype)
{
	return binaryTypeDesc[bintype];
}


