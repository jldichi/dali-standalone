/********************************************************************
* Copyright (c) 1992 InterSoft Co.  All Rights Reserved
* THIS IS UNPUBLISHED PROPRIETARY SOURCE CODE OF InterSoft Co.
* The copyright notice above does not evidence any
* actual or intended publication of such source code.
*
* $Id: shobj.h,v 1.1.1.1 1996/04/16 18:51:51 gustavof Exp $
*      
* DESCRIPTION
*********************************************************************/

#ifndef SHOBJECT_H
#define SHOBJECT_H

#include <ifound.h>

class ShObject : public SvObject 
{
	friend class shadobj;

	private:

		Int objId;					//Object Id.
		Int type;					//Object type.	
		IpcConnection *icon;		//local conn from shadow to server.

		void snd(Int len, short f);
		void prep();

	public:
		ShObject(IpcConnection *, Int, Int);
		~ShObject();
		virtual void defa();
		virtual void attach();
		virtual void detach();
		virtual void create()	{ attach(); }
		virtual void delet()	{ detach(); }
		Connection &operator()(Int so);
		Int objectType() const { return type; }
		static ShObject *getShObject(Int); 
		static Int getObjId(Int i)			
			{ return getShObject(i)->objId; }
		static IpcConnection *getIcon(Int i)
			{ return getShObject(i)->icon; }
		Int getType(Int i)
			{ return getShObject(i)->type; }
		Int getObjId()				{ return objId; }

	// +++ Debug +++
	String objType();
	// --- Debug ---
};

#endif
