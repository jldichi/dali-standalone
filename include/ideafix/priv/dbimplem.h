/********************************************************************
* Copyright (c) 1994 InterSoft Co.  All Rights Reserved
* THIS IS UNPUBLISHED PROPRIETARY SOURCE CODE OF InterSoft Co.
* The copyright notice above does not evidence any
* actual or intended publication of such source code.
*
* $Id: dbimplem.h,v 1.1.1.1 1996/04/16 18:51:39 gustavof Exp $
*
* DESCRIPTION
*********************************************************************/

#ifndef DBIMPLEM_H
#define DBIMPLEM_H

#include <ifound.h>

#ifdef __GNUC__
#	pragma interface
#endif

class DataBaseRep;
class EssDataBase;
class DbEssSet;
class DbEssIndex;
class DbEssCursor;
class IxDataBaseRep;
class DbIxSet;
class DbIxIndex;
class DbIxCursor;
class DbField;

class gtype(PtrArray, UpdateOp);

class DbObjectRepPrefix {

// private:

public:

	DbObjectRepPrefix();
	virtual ~DbObjectRepPrefix();
	
	virtual EssObjectPrefix *getEssObjectPrefix()	{ return NULL; }

	virtual void init() = 0;
	virtual Int length() = 0;
	virtual void set(DbObjectRepPrefix *prefix) = 0;
};

class DbObjectRepCache {

// private:

	DbObjectRep *obj_;
	
public:

	DbObjectRepCache(DbObjectRep *obj);
	virtual ~DbObjectRepCache();

	virtual Int set(Int size, Int mode) = 0;
	virtual void add() = 0;
	virtual bool get() = 0;
	virtual bool getByOid() = 0;
			
	DbObjectRep *obj()		{ assert(obj_ != NULL); return obj_; }
};

class DataBaseRepImplem {

// private:

	useIdCheck(DataBaseRepImplem_id);

protected:

	DataBaseRep *dataBase_;
	
public:

	DataBaseRepImplem(DataBaseRep *db);
	virtual ~DataBaseRepImplem();

	DataBaseRep *dataBase()
		{ idCheck(); assert(dataBase_ != NULL); return dataBase_;	}

	virtual EssDataBase *getEssDataBase()	{ idCheck(); return NULL;	}
	virtual IxDataBase *getIxDataBase()		{ idCheck(); return NULL;	}
	
	static DbObjectRepPrefix *buildObjectPrefix();
	static void deleteObjectPrefix(DbObjectRepPrefix *op);

	static DbObjectRepCache *buildObjectCache(DbObjectRep *obj);
	static void deleteObjectCache(DbObjectRepCache *ca);
	
	static DbField *buildDbField(DbMemberRep *memberDescr);
	static void	deleteDbField(DbField *field);

	static DbCursorRepImplem *buildCursorImplem(DbCursorRep *cur);
	static void deleteCursorImplem(DbCursorRep *cur);
	
	static DataBaseRepImplem *build(DataBaseRep *db);				// create
	static DataBaseRepImplem *build(DataBaseRep *db, Int version);	// attach

	static void beginTransaction(Int st = io_eabort);
	static Int rollBack();
	static void optimisticTransaction();
	static bool endTransaction();
	static bool endTrans(bool syn, bool rttrans);
	static bool endSyncTransaction();
	static bool transOk();
	static void corruptTrans();
	static Int transMode();
	static bool inTransaction();
	static void saneTrans();
	
	static void reOpen(DataBaseRep *db);
	
	static Int convert(DbSetRep *oldset, DbSetRep *newSet, bool force, 
					   bool drop, IFCPVA err_func, FCPVA msg_func, 
					   unsigned int cache, unsigned int dcache, 
					   bool correctRefs);		

	static String getDataBases();
	
	static Int dbmsType();

	static DataBaseRep *find(const String &schName, Int vers = CURRENT_VERSION);
	static Int versionNumber(const String &schName, const Date &d);
	virtual void attach(DbSetRep *set, Int mode) = 0;
	virtual DbSetRep *createSet(const String &name, const String &descr, 
							  DbTypeRep *typeDescr, Int flags, 
							  Int size) = 0;
	virtual DbSetRep *reCreateSet(DbSetRep *set, const String &nm, 
								const String &descr, DbTypeRep *typeDescr, 
								Int flags, Int size) = 0;

	virtual Int lock(Int l) = 0;
	virtual void free() = 0;
	virtual void setDescr(const String &descr) = 0;
	virtual bool checkPoint() = 0;
	virtual Int version() = 0;
	virtual void modify() = 0;
	virtual Int drop() = 0;
	virtual Int dropSet(DbSetRep *set) = 0;

	virtual Int setSeqNumber(DbSetRep *set) = 0;
	virtual bool chown(Int uid) = 0;
};

class DbSetRepImplem {

// private:

	useIdCheck(DbSetRepImplem_id);

protected:

	DbSetRep *set_;
	
public:

	DbSetRepImplem(DbSetRep *set);
	virtual ~DbSetRepImplem();

	DbSetRep *set()						
		{ idCheck(); assert(set_ != NULL); return set_;	}

	virtual DbEssSet *getDbEssSet()	{ idCheck(); return NULL;	}
	virtual DbIxSet *getDbIxSet()	{ idCheck(); return NULL;	}
	
	virtual bool wasAttached() = 0;
	
	virtual bool create() = 0;
	
	virtual Int get(DbObjectRep *obj, Int mode, Int nparts) = 0;
	virtual void free(DbIndexRep *index, DbObjectRepBuffer *buffer, Int mode, Int nparts) = 0;
	virtual Int find(DbIndexRep *index, DbObjectRepBuffer *buffer, Int mode, Int nparts) = 0;
	virtual void del(DbObjectRepBuffer *buffer) = 0;
	virtual void add(DbObjectRepBuffer *buffer) = 0;
	virtual void add(DbObjectRepBuffer *buffer, const TimeStamp &crts, 
					 const TimeStamp &modts) = 0;
	virtual void put(DbObjectRepBuffer *buffer) = 0;
	virtual void push(DbObjectRepBuffer *buffer, DbObjectRepStack *stack) = 0;
	virtual Int pop(DbObjectRep *obj, DbObjectRepStack *stack) = 0;
	virtual void discard(DbObjectRepStack *stack) = 0;

	virtual void update(gtype(PtrArray, UpdateOp) &uOp) = 0;
	
	virtual Int lock(Int lockMode) = 0;
	virtual void free() = 0;
	virtual Int rename(const String &newName) = 0;
	virtual bool renameMember(DbMemberRep *m, const String &newName) = 0;
	virtual TimeStamp mTimeStamp(DbObjectRepBuffer *buffer) = 0;
    virtual TimeStamp cTimeStamp(DbObjectRepBuffer *buffer) = 0;
	virtual Int delAll() = 0;
	virtual void flush() = 0;
	virtual Int actualSize() = 0;
	virtual Int nObjects() = 0;
	virtual Int dropIndex(DbIndexRep *ind) = 0;
	virtual DbIndexRep *createIndex(const String &name, Int flags, 
								  Int separ) = 0;
	virtual Int setDeflt(DbMemberRep *m, DbExpr *deflt) = 0;
	virtual Int setRelCheck(DbMemberRep *m, DbExpr *rchk) = 0;
	virtual Int setCollSet(DbMemberRep *m, const String &cs) = 0;
	virtual Int setInCheck(DbMemberRep *m, InCheck *ic) = 0;

	virtual Int *getEquiv(DbSetRep *to) = 0;
	virtual void copy(DbSetRep *to, Int *members, Int flag) = 0;
};

class DbIndexRepImplem {

// private:

	useIdCheck(DbIndexRepImplem_id);

protected:

	DbIndexRep *index_;
	
public:

	DbIndexRepImplem(DbIndexRep *index);
	virtual ~DbIndexRepImplem();

	DbIndexRep *index()						
		{ idCheck(); assert(index_ != NULL); return index_;	}

	virtual DbEssIndex *getDbEssIndex()	{ idCheck(); return NULL;	}
	virtual DbIxIndex *getDbIxIndex()		{ idCheck(); return NULL;	}

	virtual bool findKey(DbSetRep *set, DbObjectRepBuffer *buffer, 
						 Int fmode = this_key, Int nparts = 0, 
						 char **key_ptr = NULL, Int lmode = io_lock) = 0;

	virtual bool canAddMember() = 0;
	virtual void addMember(DbIndexRepPart *indPart) = 0;
	virtual Int build(Int flags) = 0;
	virtual Int complete() = 0;
	virtual Int addKey(DbObjectRep &obj) = 0;

	virtual void setCache(Int n) = 0;
};

class DbCursorRepImplem {

// private:

	useIdCheck(DbCursorRepImplem_id);

protected:

	DbCursorRep *cursor_;
	
public:

	DbCursorRepImplem(DbCursorRep *cursor);
	virtual ~DbCursorRepImplem();

	DbCursorRep *cursor()						
		{ idCheck(); assert(cursor_ != NULL); return cursor_;	}
	
	virtual DbEssCursor *getDbEssCursor()	{ idCheck(); return NULL;		}
	virtual DbIxCursor *getDbIxCursor()	{ idCheck(); return NULL;		}

	virtual void freeCache() = 0;
	virtual void sendRange() = 0;
	virtual Int getNextBlock() = 0;
	virtual Int getPrevBlock() = 0;
	virtual Int count() = 0;
};


	
	

#endif
