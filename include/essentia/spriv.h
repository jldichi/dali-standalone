/********************************************************************
* Copyright (c) 1992 InterSoft Co.  All Rights Reserved
* THIS IS UNPUBLISHED PROPRIETARY SOURCE CODE OF InterSoft Co.
* The copyright notice above does not evidence any
* actual or intended publication of such source code.
*
* $Id: spriv.h,v 1.1.1.1 1996/04/16 18:51:48 gustavof Exp $
*
* DESCRIPTION
* Class definitions for Server Schema type.
*********************************************************************/

#ifndef SPRIV_H
#define SPRIV_H

#include <ifound.h>

#ifdef __GNUC__
#pragma interface
#endif

#include <ifound/itypes.h>

class gtype(PtrArray, PrivPerm);
class Connection;

enum {
	// Id types
	PRIVG_GROUP		= 0,
	PRIVG_USER		= 1,
	PRIVG_PUBLIC	= 2,

	PRIVG_NONE		= 0x00,

	// Schema privileges
	PRIVG_USE		= (1<<0),
	PRIVG_TEMP		= (1<<1),
	PRIVG_MANIP		= (1<<2),
	PRIVG_ALTER		= (1<<3),

	// Table privileges
	PRIVG_INSERT	= (1<<0),
	PRIVG_DELETE	= (1<<1),
	PRIVG_UPDATE	= (1<<2),
	PRIVG_SELECT	= (1<<3),
	PRIVG_ALL		= 0x03ff,
	PRIVG_MODIF		= (PRIVG_INSERT|PRIVG_DELETE|PRIVG_UPDATE),
};

struct PrivPerm {
	UnShort id;			// A group or user id
	UnShort idType:2;	// Is group or user id
	UnShort privg:14;	// Bits with privileges
};

class Privilege {

// private:

	gtype(PtrArray, PrivPerm) &perms;

    Privilege(const Privilege &);
    
public:

	Privilege();
	~Privilege();

	void add(UnShort ident, UnShort idtype, UnShort priv);

	// Query a privilege for a given user & group
	bool allowed(Int owner, Int user, Int group, Int perm);

	// Shift 6 (instead of 4) to leave 2 bits for future use?
	bool canGrant(Int owner, Int user, Int grp, Int perm)
	{ return allowed(owner, user, grp, perm << 6); }

	Int size() const;

	void clear();
	bool elem(Int i, UnShort &id, UnShort &idtype, UnShort &privg);

	Privilege &operator=(const Privilege &);
	friend Connection &operator<<(Connection &c, const Privilege &p);
	friend Connection &operator>>(Connection &c, Privilege &p);
	void print() const;
};

#endif
