//- Copyright Notice
// -----------------------------------------------------------------------
// (C) Copyright 1998 InterSoft S.A.  All Rights Reserved
// THIS IS UNPUBLISHED PROPRIETARY SOURCE CODE OF InterSoft S.A.
// The copyright notice above does not evidence any actual or intended 
// publication of such source code.
//
// $Id: privmd.h,v 1.5 2002/03/19 21:21:55 hardaiz Exp $
// -----------------------------------------------------------------------

#ifndef PRIVMD_H
#define	PRIVMD_H

#include <ideafix.h>
#include <ideafix/priv/dbdefs.h>

class SchemaMetadata;
class TableMetadata;

#define	PRIV_RESIZE_FACTOR	2

/**
 * 
 * @version
 * @author Eduardo Chiocconi.
 * @see
 */
class PrivilegesMetadata {

public:
	PrivilegesMetadata(Int nPriv);
	virtual ~PrivilegesMetadata();

	// Set methods.
	virtual bool addUserPrivilege(UnInt uId, UnInt uPriv) = 0;
	virtual bool addGroupPrivilege(UnInt gId, UnInt gPriv) = 0;
	virtual bool addPublicPrivilege() = 0;
	bool delUserPrivilege();
	bool delGroupPrivilege();
	bool delPublicPrivilege();

	// Get methods.
	inline struct dbprivg *privilegesRep() const;
	bool isAValidUser(UnInt uId) const;
	bool isAValidGroup(UnInt gId) const;
	Int nPrivileges() const;
	void resetCounters();

	//Debugging method
	void dump() const;

protected:
	inline Int nPrivs() const;
	inline Int lastPriv() const;
	void resize();

private:
	struct dbprivg *priv_d;
	Int nPriv_d;
	Int lastPriv_d;

	// Private methods that add a user or group privilege from Privileges
	// structure.
	void addUser(UnInt gId, UnInt gPriv);
	void addGroup(UnInt gId, UnInt gPriv);
	void addPublic();
	void initialize();
	bool isThereAPermissionInSlot(Int pos) const;
};


/**
 * 
 * @version
 * @author Eduardo Chiocconi.
 * @see
 */
class TablePrivilegesMetadata : public PrivilegesMetadata {

	TableMetadata *tmd_d;
	inline TableMetadata *tableMetadata() const;
public:
	TablePrivilegesMetadata(TableMetadata *tmd, Int nPrivs = 1);
	~TablePrivilegesMetadata();
	bool addUserPrivilege(UnInt uId, UnInt uPriv);
	bool addGroupPrivilege(UnInt gId, UnInt gPriv);
	bool addPublicPrivilege();
};


/**
 * 
 * @version
 * @author Eduardo Chiocconi.
 * @see
 */
class SchemaPrivilegesMetadata : public PrivilegesMetadata {

	SchemaMetadata *smd_d;
	inline SchemaMetadata *schemaMetadata() const;
public:
	SchemaPrivilegesMetadata(SchemaMetadata *smd, Int nPrivs = 1);
	~SchemaPrivilegesMetadata();
	bool addUserPrivilege(UnInt uId, UnInt uPriv);
	bool addGroupPrivilege(UnInt gId, UnInt gPriv);
	bool addPublicPrivilege();

	bool emptyPrivileges();
};

#include <sqldb/metadata/privmd.icc>

#endif	// PRIVMD_H
