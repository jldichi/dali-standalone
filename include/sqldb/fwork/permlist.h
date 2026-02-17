//- Copyright Notice
// -----------------------------------------------------------------------
// (C) Copyright 1998 InterSoft S.A.  All Rights Reserved
// THIS IS UNPUBLISHED PROPRIETARY SOURCE CODE OF InterSoft S.A.
// The copyright notice above does not evidence any actual or intended 
// publication of such source code.
//
// $Id: permlist.h,v 1.6 2002/03/20 15:11:12 hardaiz Exp $
// -----------------------------------------------------------------------

#ifndef PERMLIST_H
#define	PERMLIST_H

#include <sqldb/fwork/permission.h>
#include <sqldb/fwork/objlist.h>

/**
 * This class is used to store permission objects in a list.
 * 
 * @version
 * @author Eduardo Chiocconi
 *
 * @see Permission
 * @see ObjectList
 */
class PermissionList : public ObjectList {

public:
	// Constructor & Destructor.
	PermissionList();
	~PermissionList();

	// Set methods.
	inline void add(const Permission *c);
	// Get Information methods.
	inline const Permission &getPermission(Int i) const;

private:
	PermissionList(const PermissionList &cl);
	PermissionList &operator=(const PermissionList &cl);
};

#include <sqldb/fwork/permlist.icc>

#endif // PERMLIST_H
