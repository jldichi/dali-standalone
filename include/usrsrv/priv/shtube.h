/********************************************************************
* Copyright (c) 1992 InterSoft Co.  All Rights Reserved
* THIS IS UNPUBLISHED PROPRIETARY SOURCE CODE OF InterSoft Co.
* The copyright notice above does not evidence any
* actual or intended publication of such source code.
*
* $Id: shtube.h,v 1.1.1.1 1996/04/16 18:51:51 gustavof Exp $
*
* DESCRIPTION
* Class definitions for ShTube type.
*********************************************************************/

#ifndef SHTUBE_H
#define SHTUBE_H

#include <ifound.h>

#ifdef HAVE_PRAGMA
#pragma interface
#endif

#include <usrsrv/shadobj.h>
#include <essentia/dbconst.h>
#include <essentia/dbmcom.h>

class IpcConnection;

class ShTube : public ShadowObject 
{

//private:

	// connection to THE server.
	IpcConnection *ipcon;

	void prep();
	void snd(Int len, short f, Int l);
	bool patchClientId(MsgBuff buff, Int len);

public:

	// Constructors & destructors
	ShTube(const String svName);
	~ShTube();

	// ShTube operations
	void defa(Client *);			// default action for the shadow

	// to end attach/detach to/from a schema
	void endAttachSchema(Int full, Int create, String name0, String descr);
	void endDetachSchema(Int);

	Int objectType() const			{ return SHTUBE; }

	ShTube *getShTube()				{ return this; }
	IpcConnection *ipCon()			{ return ipcon; }
	
	// +++ Debug +++
	String objType();
	// --- Debug ---
};

#endif
