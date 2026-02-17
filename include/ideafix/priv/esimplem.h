/********************************************************************
* Copyright (c) 1994 InterSoft Co.  All Rights Reserved
* THIS IS UNPUBLISHED PROPRIETARY SOURCE CODE OF InterSoft Co.
* The copyright notice above does not evidence any
* actual or intended publication of such source code.
*
* $Id: esimplem.h,v 1.1.1.1 1996/04/16 18:51:39 gustavof Exp $
*
* DESCRIPTION
*********************************************************************/

#ifndef ESIMPLEM_H
#define ESIMPLEM_H

#include <essentia/dbclobj.h>
#include <essentia/objectid.h>
#include <essentia/dbconn.h>

#include <ideafix/priv/dbimplem.h>

#include <ifound/tstamp.h>

// general functions
Int convertFlags(Int mode);

class gtype(PtrArray, UpdateOp);
class gtype(PtrArray, DbObjectRepBuffer);
class gtype(PtrArrayCursor, DbObjectRepBuffer);
class gtype(PtrHashTab, EssCacheBuffer);
class EssDataBase;

class EssObjectPrefix : public DbObjectRepPrefix {

// private:

	ObjectId oid_;
	TimeStamp cStamp_;
	TimeStamp mStamp_;

public:

	EssObjectPrefix(const ObjectId &o, const TimeStamp &cs, 
					const TimeStamp &ms);
	EssObjectPrefix();
	~EssObjectPrefix();
	
	EssObjectPrefix *getEssObjectPrefix()	{ return this;	}
	void init();
	Int length();
	void set(DbObjectRepPrefix *prefix);
		
	const ObjectId &oid()		{ return oid_;		}
	const TimeStamp &cStamp()	{ return cStamp_;	}
	const TimeStamp &mStamp()	{ return mStamp_;	}		
	
	void set(const ObjectId &o, const TimeStamp &cs, const TimeStamp &ms)
		{ oid_ = o; cStamp_ = cs; mStamp_ = ms;	}

	void send(DbConnection &con)
		{ con << oid_ << cStamp_ << mStamp_;	}
	void receive(DbConnection &con)
		{ con >> oid_ >> cStamp_ >> mStamp_;	}
	
	void resolveVirtual(DbMemberRep *member, char *buffer);
};

class EssCacheBuffer {

	DbObjectRepBuffer *objBuffer_;

	static Int elems_;

	EssCacheBuffer *next_, *prev_;
	static EssCacheBuffer *first_, *last_;

public:

	static Int elems()				{ return elems_;	}
	static EssCacheBuffer *last()	{ return last_;		}	
	static EssCacheBuffer *first()	{ return first_;	}
	
	EssCacheBuffer(DbObjectRepBuffer *buff);
	~EssCacheBuffer();

	void markAsLastUsed();

	DbObjectRepBuffer *objBuffer()	
			{ assert(objBuffer_ != NULL); return objBuffer_; }
	EssCacheBuffer *next()			{ return next_;		}
};

class EssObjectCache : public DbObjectRepCache {

// private:

	Int dim_;
	gtype(PtrHashTab, EssCacheBuffer) *hashTab_;
	
	void delAllBuffers();
	void free(Int n);

	bool full();

	
public:

	EssObjectCache(DbObjectRep *obj);
	~EssObjectCache();

	Int set(Int size, Int mode);
	void add();
	bool get();
	bool getByOid();
};

class DbEssSet : public ClientObject, public DbSetRepImplem {

// private:

	EssDataBase *essDb_;	// Pointer to Essentia data base
	UnInt classId_;			// class id
	Int   cBuffLen_;		// buffer connection length

	void resolveInCheck(DbMemberRep *member);
	void convertInCheck(InCheck *ic);
	
public:
	
	DbEssSet(EssDataBase *db, DbSetRep *set, Int classId);
	DbEssSet(EssDataBase *db, DbSetRep *set);
	~DbEssSet();
	
	DbIndexRep *readIndex(DbConnection &con);
	DbIndexRepPart *readIndexPart(DbConnection &con);
	void readMemberDescr(DbTypeRep *typeDescr, DbConnection &conn, Int m);

	Int classId()				{ idCheck(); return classId_;	}
	DbEssSet *getDbEssSet()		{ idCheck(); return this;		}
	
	EssDataBase *essDb()
		{ idCheck(); assert(essDb_ != NULL); return essDb_; }

	void attach(Int mode);
	bool create();
	void detach();

	void reCreate(const String &nm, const String &descr,
				  DbTypeRep *typeDescr, Int flags, Int size);

	void receiveObjectPrefix(DbObjectRepBuffer *buffer);
	Int receiveObject(DbObjectRepBuffer *buffer);
	void resolveVirtualMembers(DbObjectRepBuffer *buffer);
	void receiveMembers(PtrArray(DbMemberRep) &mbs, DbObjectRepBuffer *buff);
	void sendObjectPrefix(DbObjectRepBuffer *buffer);
	void sendObject(DbObjectRepBuffer *buffer);
	
	bool wasAttached();
	
	Int get(DbObjectRep *obj, Int mode, Int nparts);
	void free(DbIndexRep *index, DbObjectRepBuffer *buffer, Int mode, Int nparts);
	Int find(DbIndexRep *index, DbObjectRepBuffer *buffer, Int mode, Int nparts);
	Int find(DbEssIndex *index, DbObjectRepBuffer *buffer, Int mode, Int nparts);
	void del(DbObjectRepBuffer *buffer);
	void add(DbObjectRepBuffer *buffer);
	void add(DbObjectRepBuffer *buffer, const TimeStamp &crts, 
			 const TimeStamp &modts);
	void put(DbObjectRepBuffer *buffer);
	void push(DbObjectRepBuffer *buffer, DbObjectRepStack *stack);
	Int pop(DbObjectRep *obj, DbObjectRepStack *stack);
	void discard(DbObjectRepStack *stack);

	void update(gtype(PtrArray, UpdateOp) &uOp);

	Int lock(Int lockMode);
	void free();
	void flush();
	Int rename(const String &newName);
	bool renameMember(DbMemberRep *m, const String &newName);
	TimeStamp mTimeStamp(DbObjectRepBuffer *buffer);
    TimeStamp cTimeStamp(DbObjectRepBuffer *buffer);
	Int delAll();
	Int actualSize();
	Int nObjects();
	Int dropIndex(DbIndexRep *ind);
	DbIndexRep *createIndex(const String &name, Int flags, Int separ);
	Int setDeflt(DbMemberRep *m, DbExpr *deflt);
	Int setRelCheck(DbMemberRep *m, DbExpr *rchk);
	Int setCollSet(DbMemberRep *m, const String &cs);
	Int setInCheck(DbMemberRep *m, InCheck *ic);

	Int *getEquiv(DbSetRep *);
	void copy(DbSetRep *to, Int *members, Int flag);
};

class DbEssIndex : public ClientObject, public DbIndexRepImplem {

// private:

public:
	
	DbEssIndex(DbIndexRep *ind, DbConnection *con, Int objectId);
	DbEssIndex(DbEssSet *set, DbIndexRep *index);
	~DbEssIndex();
	
	DbEssIndex *getDbEssIndex()	{ idCheck(); return this; }
	EssDataBase *essDb();

	void sendKey(DbObjectRepBuffer *buffer, Int mode);
	Int receiveKey(char *buffer);
	void _receiveKey(char *buffer);

	bool findKey(DbSetRep *set, DbObjectRepBuffer *buffer, 
				 Int fmode = this_key, Int nparts = 0, 
				 char **key_ptr = NULL, Int lmode = io_lock);
	bool canAddMember();
	void addMember(DbIndexRepPart *indPart);
	Int build(Int flags);
	Int complete();
	Int addKey(DbObjectRep &obj);
	bool create();
	void setCache(Int );
};

class DbEssCursor : public ClientObject, public DbCursorRepImplem {

// private:

public:

	DbEssCursor(DbCursorRep *cur, DbConnection *c, Int oid);
	~DbEssCursor();
		
	DbEssCursor *getDbEssCursor()		{ idCheck(); return this; }
	EssDataBase *essDb();

	void freeCache();
	void sendRange();
	Int getNextBlock();
	Int getPrevBlock();
	Int count();

	Int buildIndex(DbEssIndex *index, bool clear);
};

class EssDataBase : public ClientObject, public DataBaseRepImplem {

// private:

	String			server_;		// server of this schema
	Int 			version_;		// schema version

	static bool		transOk_;
	static Int		transMode_;
		
	DbSetRep *createSet(const String &name, const String &descr, 
					  DbTypeRep *typeDescr, Int flags, Int size);
	DbSetRep *reCreateSet(DbSetRep *set, const String &nm, 
						const String &descr, DbTypeRep *typeDescr, 
						Int flags, Int size);
	void attach(DbSetRep *set, Int mode);
	bool create(DbSetRep *set);
	bool detach();
	void detach(DbSetRep *set);

	static String extractServer(const String &name, String &server);

	static Int alter(DbSetRep *oldSet, DbSetRep *newSet, bool force,  
					 Int *&indices);
	static void replace(DbSetRep *oldSet, DbSetRep *newSet);

public:

	EssDataBase(DataBaseRep *db);					// CreateSchema
	EssDataBase(DataBaseRep *db, Int version);		// OpenSchema
    ~EssDataBase();


	const String &server()			{ idCheck(); return server_;	}
	
	Int lock(Int);
	void free();
	void setDescr(const String &descr);
	bool checkPoint();
	Int version();
	void modify();
	Int drop();
	Int dropSet(DbSetRep *set);
	
	EssDataBase *getEssDataBase()	{ idCheck(); return this;		}

	DbCursorRepImplem *createCursor(DbCursorRep *cur, DbEssSet *setImplem, 
								  DbEssIndex *indImplem);
	void deleteCursor(DbCursorRep *cur);

	static DbEssCursor *getImplem(DbCursorRep *cur);
	static DbEssIndex *getImplem(DbIndexRep *index);
	static EssObjectPrefix *getImplem(DbObjectRepPrefix *op);
	static DbEssSet *getImplem(DbSetRep *set);

	static void beginTransaction(Int st = io_eabort);
	static void optimisticTransaction();
	static bool endTransaction();
	static bool endSyncTransaction();
	static Int rollBack();
	static void restartTrans(bool rttrans);
	static bool endTrans(bool sy, bool rttrans);
	static void corruptTrans();
	static Int transMode();
	static bool transOk();
	static bool inTransaction();
	static void saneTrans();

	static void reOpen(DataBaseRep *db);

	static Int convert(DbSetRep *oldSet, DbSetRep *newSet, bool force, bool drop, 
				   	   IFCPVA err_func, FCPVA msg_func, unsigned int cache, 
				   	   unsigned int dcache, bool correctRefs);

	static String getDataBases();
	
	static Int dbmsType();

	static Int versionNumber(const String &schName, const Date &d);
	static DataBaseRep *find(const String &schName, Int vers = CURRENT_VERSION);
	static Int internalVersionNumber(const String &schName, Int vers);
	
	bool beginTrans(bool condStart);
	bool endTrans();

	Int setNo(Int classId);

	Int setSeqNumber(DbSetRep *set);

	bool chown(Int uid);
};

#endif
