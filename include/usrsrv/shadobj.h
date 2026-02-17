/********************************************************************
* Copyright (c) 1992 InterSoft Co.  All Rights Reserved
* THIS IS UNPUBLISHED PROPRIETARY SOURCE CODE OF InterSoft Co.
* The copyright notice above does not evidence any
* actual or intended publication of such source code.
*
* $Id: shadobj.h,v 1.1.1.1 1996/04/16 18:51:51 gustavof Exp $
*
* DESCRIPTION
* Class definitions for Shadow Object type.
*********************************************************************/

#ifndef SHADOBJ_H
#define SHADOBJ_H

#include <ifound.h>

#ifdef HAVE_PRAGMA
#pragma interface
#endif

#include <essentia/gservobj.h>

class ShadowConnection;

class ShadowObject : public GServObj {

protected:

	// Connection
	static ShadowConnection *scon;

public:

	// Constructors & destructors
	ShadowObject();
	virtual ~ShadowObject();

	virtual void defa(Client *);

	ShadowObject *getShadowObject();
};

#endif // SHADOBJ_H
