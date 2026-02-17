/********************************************************************
* Copyright (c) 1994 InterSoft Co.  All Rights Reserved
* THIS IS UNPUBLISHED PROPRIETARY SOURCE CODE OF InterSoft Co.
* The copyright notice above does not evidence any
* actual or intended publication of such source code.
*
* $Id
*
* DESCRIPTION
*********************************************************************/

#ifndef ESIMPLEM_H
#define ESIMPLEM_H

#include <ideafix/priv/dbmimplem.h>

class IxDataBase;

// Log DEFINITIONS
enum {
	close_schema	= 1,
	open_schema,
	begin_transaction,
	end_transaction,
	del_record,
	put_record,
	add_record,
	switch_to_schema,
};

// Modes for LogStatus
enum {
	log_off,
	log_enabled,
	log_disabled,
	log_rollback,
};

struct DbmCache {
	char *node_;
	Int n_;
	Int changed_;
}; 

struct Header {
	Int last_;     // Ultimo registro ocupado
	Int pool_;     // Primer registro en el pool
	Int nreg_;     // Numeros de registros ocupados
	Header (Int l = 0, Int p = 0, Int nr = 0)
	:	last_(l), pool_(p), nreg_(nr)
	{ }
};

#if HAVE_LOCK_CHAIN
#	define LOCK_NODE_SIZE  5
// struct for a node for lock list */
struct LockTable {
  Int lockPos_[LOCK_NODE_SIZE];
  LockTable *next_;
};
#endif

#define DEF_NFILES	12	 // Default Max Number of used Unix files
#define MAX_VFILES	100	 // Max No of virtual files opened (all)

class VirtualFile {

	useIdCheck(VirtualFile_id);
	
// private:

	IxDataBase *dataBase_;
	String  name_;
	String  ext_;
 	Int		mode_;
	Int		fd_;
	Int		nLocks_;
	VirtualFile *next_;
	VirtualFile *prev_;

#if HAVE_LOCK_CHAIN
	Int lockWhole_;
	LockTable *lockTable_;		// Pointer to first table
#endif

	VirtualFile(IxDataBase *db, const String &name, const String &ext, 
				Int mode);
	VirtualFile();
	~VirtualFile();
	
	void link();
	void unLink();
	void phyClose();
	Int stat(enum stat_elem elem);
	Int write(char *buff, unsigned length, Int offset);
	Int vfd(bool ret);
	Int read(char *buff, unsigned length, Int offset, Int mode);
	void free(UnShort length, Int offset);
	Int lockf(Int lock_mode);

	static void lockLog(VirtualFile *f, char *func, Int offset, Int length);
	static void closeOne();
	static void flushAll();

#ifndef HAVE_REAL_LOCKS
	Int freeVtab();
	Int *findLockSlot(Int offset);
#endif

#ifdef HAVE_PSEUDO_CONCURR
	static bool multi_;
#endif	
	static bool hadAlarm_;
	static Int nFiles_;				// Max number of Open files
	static Int nOpen_;				// Current number of open files
	static Int maxVFiles_;			// number of vfiles
	static VirtualFile *vTab_;		// virtual file	table
	static VirtualFile *first_;		// First virt. file in the open list

public:

	static VirtualFile *build(IxDataBase *db, const String &name, 
							  const String &ext, Int mode);
					   
	IxDataBase *dataBase()		{ idCheck(); return dataBase_;	}
	const String  &name()		{ idCheck(); return name_;		}
	const String  &ext()		{ idCheck(); return ext_;		}
 	Int mode()					{ idCheck(); return mode_;		}
	Int	fd()					{ idCheck(); return fd_;		}
	Int	nLocks()				{ idCheck(); return nLocks_;	}
	VirtualFile *next()			{ idCheck(); return next_;		}
	VirtualFile *prev()			{ idCheck(); return prev_;		}

#if HAVE_LOCK_CHAIN
	Int lockWhole()				{ idCheck(); return lockWhole_;	}
	LockTable *lockTable()		{ idCheck(); return lockTable_;	}
#endif
};

class IxObjectPrefix : public DbmObjectPrefix {

// private:

public:

};

class IxDataBase : public DataBaseImplem {

// private:

	useIdCheck(IxDataBase_id);
	
	String 		dbase_;			// dbase of this schema
	VirtualFile	*fd_;			// .sco file descriptor
	bool		locked_;		// locking flag
	Int			semId_;			// Semaphore id. of this schema
	Int			magic_;			// sco magic number
	Int			nOpen_;			// no of process using this schema
	Int			cpuMagic_;		// Magic to define the kind of CPU
	bool		wasAltered_;	// sch needs to be stored
	
	String extractDbase(const String &nm, String &dbn);
	String fileName(const String &name, const String &ext);
	void unLink(const String &name, const String &ext);
	void rename(const String &oldname, const String &ext,
		 		const String &newname, const String &nex_ext);
	Int open();
	void open(const String &dbase, const String &name, Int mode);
	void openError(Int nerr, const String &tname = NULL_STRING, 
				   FILE *fil = NULL);
				   
	void initValues();

	Int load(FILE *f, String &errDetail, Int mode);
	Int store();		
	void store(FILE *f);

	static bool transActive_;
	static bool transOk_;
	static Int  syncr_;
	static Int  transMode_;
	static bool execBegTrans_;

	static Int logStatus_;
	static FILE *logFile_;
	static String logName_;

	static void flushAll();
	static void flush(IxDataBase *db);

public:

	IxDataBase(DataBase *db);					// CreateSchema
	IxDataBase(DataBase *db, Int version);		// OpenSchema
	~IxDataBase();

	IxDataBase *getIxDataBase()		{ idCheck(); return this; }
	
	const String &name();
	
	void attach(DbmSet *set, Int mode);
	bool create(DbmSet *set);
	DbmSet *createSet(const String &name, const String &descr, 
					  DbmTypeDescr *typeDescr, Int flags, 
					  Int size);
	Int lock(Int l);
	void free();

	void reOpen();
	Int versionNumber(const String &, const Date &);
	Int version();
	void modify();

	static void beginTrans(Int st = io_eabort);
	static void beginTransaction(Int st = io_eabort);
	static void optimisticTransaction();
	static bool endTransaction();
	static void optimisticTransaction();
	static bool endTrans(bool syn, bool rttrans);
	static bool endSyncTransaction();
	static Int transMode();
	static bool transOk();
	static void saneTrans();
	static void corruptTrans();
	static bool inTransaction();
	static Int rollBack();

};

class DbmIxSet : public DbmSetImplem {

// private:

	useIdCheck(DbmIxSet_id);
	
	IxDataBase *ixDb_;
	Header hdr_;		// table header
	VirtualFile *fd_;			
	bool   hdrLock_;	// The table header is locked
	bool   locked_;		// Flag of table locked
	DbmCache *cache_;	// table cache
	
	void loadMemberDescr(DbmTypDescr *typeDescr, FILE *f, Int mode);
	DbmIndex loadIndex(FILE *f);
	DbmIndexPart loadIndexPart(FILE *f);
	void storeTypeDescr(FILE *f);
	void storeMemberDescr(DbmMemberDescr *member, FILE *f);
	void storeIndices(FILE *f);
	void storeIndex(DbmIndex *index, FILE *f);
	void storeIndexPart(DbmIndexPart *indPart, FILE *f);

public:

	DbmIxSet(IxDataBase *db, DbmSet *set);

	DbmIxSet *getDbmIxSet()	{ idCheck(); return this;	}

	IxDataBase *ixDb()		{ idCheck(); assert(ixDb_!=NULL); return ixDb_; }

	Int load(FILE *f, Int mode);
	void store(FILE *f);
};

class DbmIxIndex : public DbmIndexImplem {

// private:

	useIdCheck(DbmIxIndex_id);

	Header	hdr_;
	VirtalVile fd_;
	bool locked_;
	bool nodeChanged_;
	char *root_;
	char *node_;
	TabArray *lastTap_;				// (pablog)
	Int max_, 
	Int	mTime_;
	Int	rootSlot_,
	Int leafSlot_,
	Int	leafLength_,
	Int	currNode_;
	DbmCache *cache;
	Int cacheSize_;
	Int nodeLen_;

public:
	
	DbmIxIndex(DbmIndex *index, Int nodelen);
	
	DbmIxIndex *getDbmIxIndex()		{ idCheck(); return this; }
};

class DbmIxCursor : public DbmCursorImplem {

// private:

	useIdCheck(DbmIxCursor_id);
	
	Int lNrec_;		// number of locked records

public:

	DbmIxCursor *getDbmIxCursor()		{ idCheck(); return this;	}
};


#endif
