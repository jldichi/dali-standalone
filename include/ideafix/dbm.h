/********************************************************************
* Copyright (c) 1994 InterSoft Co.  All Rights Reserved
* THIS IS UNPUBLISHED PROPRIETARY SOURCE CODE OF InterSoft Co.
* The copyright notice above does not evidence any
* actual or intended publication of such source code.
*
* $Id: dbm.h,v 1.1.1.1 1996/04/16 18:51:37 gustavof Exp $
*
* DESCRIPTION
* Database manager API
*********************************************************************/

#ifndef DBM_H
#define DBM_H

#ifndef DBTYPE_H
#	include <ideafix/dbtype.h>
#endif

#include <ideafix/ref.h>

// size_t typedef
#include <sys/types.h>

class DataBase;
class DataBaseRep;
class DbObjectRep;
class DbSet;
class DbSetRep;
class DbCursorRep;
class AnyValue;

class Persistent_Object {

public:

	Persistent_Object();
	Persistent_Object(const Persistent_Object &);
	virtual ~Persistent_Object();
	
	void mark_modified();		// mark the object as modified

	void *operator new(size_t size, const char *typeName = 0);
	void *operator new(size_t size, Ref<Persistent_Object> cluster,
					   const char *typeName = 0);
	void *operator new(size_t size, DataBase *database, 
					   const char *typeName = 0);
	virtual void odb_activate();
	virtual void odb_deactivate();
};

class Transaction {

public:

	Transaction();
	~Transaction();
	
	void begin();
	void commit();
	void abort();
	void checkpoint();
	
};

class DataBase {

	useIdCheck(DataBase_id);

	friend class DbObject;
	
//private:

	DataBaseRep *rep_;
	
	DataBaseRep *rep() { idCheck(); assert(rep_ != NULL); return rep_; }	

public:

	// ODMG interface 
	enum access_status { not_open, read_write, read_only, exclusive };

	DataBase();
	
	void open(const char *database_name, access_status status = read_write);
	void close();

	void set_object_name(const Ref_Any &theObject, const char *theName);
	const char *get_object_name(const Ref_Any &) const;
	void rename_object(const char *oldName, const char *newName);
	Ref_Any lookup_object(const char *name) const;

	// Ideafix interface
	DataBase(const String &name, const String &descr);
	DataBase(const String &name, Int mode, Int version = CURRENT_VERSION);

	~DataBase();

	Int lock(Int l);
	void free();
};

class DbObject : public Persistent_Object {

	useIdCheck(DbObject_id);

// private:

	DbObjectRep *rep_;
	DbSet *set_;

	DbObjectRep *rep() const
		{ idCheck(); assert(rep_ != NULL); return rep_;}
	DbSet *set()
		{ idCheck(); assert(set_ != NULL); return set_; }

public:

	DbObject(void *buff = NULL);
	DbObject(const String &db, const String &extnt, void *buff = NULL);
	DbObject(const String &extnt, void *buff = NULL);
	DbObject(DataBase &db, const String &extnt, void *buff = NULL);
	DbObject(const DbObject &);

	~DbObject();
		
	bool accessBy(const String &name);
	bool accessBy(Int nIndex);
	
	void init();
	void del();
	void add();
	void put();
	Int  get(Int mode = this_key|default_lock, Int nparts = 0);
	void free(Int mode = this_key, Int nparts = 0);
	Int  find(Int mode = this_key, Int nparts = 0);
	                                                  
	DbObject &operator=(const DbObject &);
	
	DbSet &extent()		{ idCheck(); return *set(); }
};


class DbSet {

	useIdCheck(DbSet_id);

	friend class DbObject;
	
// private:

	DbSetRep *rep_;
	
	DbSetRep *rep() { idCheck(); assert(rep_ != NULL); return rep_; }
	
	DbSet(DbSetRep *r);
	~DbSet();
	
public:

	Int delAll();
	Int lock(Int lockMode);
	void free();

};

class DbCursor {

	useIdCheck(DbCursor_id);

// private:

	DbCursorRep *rep_;
		
	DbCursorRep *rep()	{ idCheck(); assert(rep_ != NULL); return rep_; }

public:

	DbCursor(DbObjectRep &obj, const String &indName = NULL_STRING, 
			  Int mode = io_not_lock);
	~DbCursor();

	Int fetch();
	Int fetchPrev();
	Int fetchThis();
	void moveFirst();
	void moveLast();
	Int count();

	Int operator++() 		{ idCheck(); return fetch(); }
	Int operator--() 		{ idCheck(); return fetchPrev(); }

	DbCursor &operator<<(const String &name);

	DbCursor &operator<(const AnyValue &av);
	DbCursor &operator<=(const AnyValue &av);
	DbCursor &operator>=(const AnyValue &av);
	DbCursor &operator>(const AnyValue &av);
	DbCursor &operator,(const AnyValue &av);

	void setFromFld(int nfield, const AnyValue &av);
	void setToFld(int nfield, const AnyValue &av);
};

#endif
