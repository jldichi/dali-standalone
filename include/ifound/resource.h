/********************************************************************
* Copyright (c) 1987 InterSoft Co.  All Rights Reserved
* THIS IS UNPUBLISHED PROPRIETARY SOURCE CODE OF InterSoft Co.
* The copyright notice above does not evidence any
* actual or intended publication of such source code.
*
* $Id: resource.h,v 1.1.1.1 1996/04/16 18:51:33 gustavof Exp $
*
*********************************************************************/

#ifndef RESOURCE_H
#define RESOURCE_H

#ifdef __GNUC__
#pragma interface
#endif

#ifdef ISDEBUG
#	include <ifound/libdebug.h>
#endif

class Resource {
	useIdCheck(RESOURCE_ID);

protected:
	Int refcount;
	
public:
	Resource()				{ idStart(); refcount = 0; }
	virtual ~Resource()		{ idCheck(); refcount = ERR; idStop(); }

    void ref()    	{ idCheck(); ++refcount; }
    void unRef()
    { idCheck(); assert(refcount > 0); if (--refcount == 0) delete this; }

	Int count()		{ idCheck(); return refcount; }
	
	static void ref(Resource *r)		{ if (r) r->ref(); }
	static void unRef(Resource *r)		{ if (r) r->unRef(); }
};
#endif
