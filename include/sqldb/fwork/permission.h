//- Copyright Notice
// -----------------------------------------------------------------------
// (C) Copyright 1998 InterSoft S.A.  All Rights Reserved
// THIS IS UNPUBLISHED PROPRIETARY SOURCE CODE OF InterSoft S.A.
// The copyright notice above does not evidence any actual or intended 
// publication of such source code.
//
// $Id: permission.h,v 1.8 2002/03/20 15:09:24 hardaiz Exp $
// -----------------------------------------------------------------------

#ifndef PERMISSION_H
#define	PERMISSION_H

#include <ifound/str.h>
#include <sqldb/fwork/object.h>

/**
 * This class is used to group SQL permissions. It is a base class for ervery
 * other permission.
 * 
 * @version
 * @author Eduardo Chiocconi
 * @see Object
 */
class Permission : public Object {

public:
	Permission();
	virtual ~Permission();
	virtual Object *clone() const = 0;

protected:
	Permission(const Permission &p);
	Permission &operator=(const Permission &p);
};

// Valid table permissions.
class TblPerm {
public:
	enum Perm {
		ALL = 0,
		INSERT = 1,
		DELETE = 2,
		SELECT = 3,
		UPDATE = 4,
		UNKNOWN = 5,
	};
};

/**
 * This class is used to represent SQL table permissions.
 * There are two kinds of SQL permissions: grant & revoke. They both work
 * over a table and they have a permission to be granted or revoked to
 * users or group of them.
 * 
 * @version
 * @author Eduardo Chiocconi
 * @see Permission
 *
 * toDo: We may have permissions by column. This version only supports 
 * permission for all columns in a table.
 */
class TablePermission : public Permission {

public:
	// Constructor & Destructor.
	TablePermission(const TblPerm::Perm p, const String &on);
	~TablePermission();

	// Get Information methods.
	inline const TblPerm::Perm privilege() const;
	inline String on() const;
	virtual Object *clone() const = 0;
	
protected:
	TblPerm::Perm priv_d;
	String on_d;

	TablePermission(const TablePermission &p);
	TablePermission &operator=(const TablePermission &p);
	String getPrivilege() const;
};


/**
 * This class is used to represent SQL table grant permission. It can be
 * used to grant permissions to users.
 * 
 * @version
 * @author Eduardo Chiocconi
 * @see TablePermission
 *
 * toDo: We may have permissions by column. This version only supports 
 * permission for all columns in a table.
 * toDo: Allow more than one user to be added to Grant permission. "to" 
 * parameter may be a list.
 */
class GrantTablePermission : public TablePermission {

public:
	// Constructor & Destructor.
	GrantTablePermission(const TblPerm::Perm p, const String &on, 
							const String &to, bool grantOp);
	~GrantTablePermission();

	// Get Information methods.
	inline String to() const;
	inline bool hasGrantOption() const;
	
	// Debug
	String convertToString() const;
	void dump() const;
	Object *clone() const;

private:
	String to_d;
	bool grantOp_d;

	GrantTablePermission(const GrantTablePermission &p);
	GrantTablePermission &operator=(const GrantTablePermission &p);
};


/**
 * This class is used to represent SQL table revoke permission. It can be
 * used to remove or revoke permissions to users.
 * 
 * @version
 * @author Eduardo Chiocconi
 * @see TablePermission
 *
 * toDo: We may have permissions by column. This version only supports 
 * permission for all columns in a table.
 * toDo: Allow more than one user to be added to Revoke permission. "from" 
 * parameter may be a list.
 */
class RevokeTablePermission : public TablePermission {

public:
	// Constructor & Destructor.
	RevokeTablePermission(const TblPerm::Perm p, const String &on, 
							const String &from);
	~RevokeTablePermission();

	// Get Information methods.
	inline String from() const;

	// Debug
	String convertToString() const;
	void dump() const;
	Object *clone() const;

private:
	String from_d;

	RevokeTablePermission(const RevokeTablePermission &p);
	RevokeTablePermission &operator=(const RevokeTablePermission &p);
};


// Valid Database permissions.
class DbPerm {
public:
	enum Perm {
		ALL = 0,
		USE = 1,		// use. (no select allowed)
		TEMP = 2,		// select ... order by ... (read only)
		MANIP = 3,		// insert-update-delete (can write) (All DML stmt).
		ALTER = 4,		// create [schema|table] ... (All DDL & DML stmt)
		UNKNOWN = 5,
	};
};

/**
 * This class is used to represent SQL database permissions.
 * 
 * @version
 * @author Eduardo Chiocconi
 * @see Field
 */
class DbPermission : public Permission {

public:
	DbPermission(const DbPerm::Perm p, const String &on);
	~DbPermission();

	// Get Information methods.
	inline const DbPerm::Perm privilege() const;
	inline String on() const;
	virtual Object *clone() const = 0;

private:
	DbPerm::Perm perm_d;
	String on_d;

protected:
	DbPermission(const DbPermission &p);
	DbPermission &operator=(const DbPermission &p);
	String getPrivilege() const;
};


/**
 * This class is used to represent SQL database grant permissions.
 * 
 * @version
 * @author Eduardo Chiocconi
 */
class GrantDbPermission : public DbPermission {

public:
	GrantDbPermission(const DbPerm::Perm p, const String &to, bool canGrant);
	~GrantDbPermission();

	// Get Information methods.
	inline String to() const;

	// Debug
	String convertToString() const;
	void dump() const;
	Object *clone() const;
	inline bool canGrant() const;

private:
	String to_d;
	bool canGrant_d;

	GrantDbPermission(const GrantDbPermission &p);
	GrantDbPermission &operator=(const GrantDbPermission &p);
};


/**
 * This class is used to represent SQL database revoke permissions.
 * 
 * @version
 * @author Eduardo Chiocconi
 * @see Field
 */
class RevokeDbPermission : public DbPermission {

public:
	RevokeDbPermission(const DbPerm::Perm p, const String &on, const String &from);
	~RevokeDbPermission();

	// Get Information methods.
	inline String from() const;

	// Debug
	String convertToString() const;
	void dump() const;
	Object *clone() const;

private:
	String from_d;

	RevokeDbPermission(const RevokeDbPermission &p);
	RevokeDbPermission &operator=(const RevokeDbPermission &p);
};

#include <sqldb/fwork/permission.icc>

#endif	// PERMISSION_H
