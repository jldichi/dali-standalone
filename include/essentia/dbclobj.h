/********************************************************************
* Copyright (c) 1992 InterSoft Co.  All Rights Reserved
* THIS IS UNPUBLISHED PROPRIETARY SOURCE CODE OF InterSoft Co.
* The copyright notice above does not evidence any
* actual or intended publication of such source code.
*
* $Id: dbclobj.h,v 1.1.1.1 1996/04/16 18:51:47 gustavof Exp $
*
* DESCRIPTION
* Class definitions for client object class.
*********************************************************************/

#ifndef DBCLOBJ_H
#define DBCLOBJ_H

#include <ifound.h>

#ifdef __GNUC__
#pragma interface
#endif

#include <ifound/str.h>

class DbConnection;

class ClientObject {

// private:          

     ClientObject(const ClientObject &);
     void operator=(const ClientObject &);

protected:

	String f_name;		// object name
	Int objectId;		// object identifier within server
	Int nref;			// reference count
	DbConnection *con;	// client object's connection

public:

	explicit ClientObject(const String &nm, DbConnection *conn = 0);

	virtual ~ClientObject();
	inline const String &name();
	inline Int objId() const;
	inline DbConnection *getConnection();

	DbConnection &operator()(Int so);
	DbConnection &operator()(Int oid, Int so);
	Int flush();

	bool ok() const;
	void attach();
	int detach();
	inline Int nRef() const;
};

#include <essentia/dbclobj.icc>

#endif
