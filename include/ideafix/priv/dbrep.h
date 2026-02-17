/********************************************************************
* Copyright (c) 1994 InterSoft Co.  All Rights Reserved
* THIS IS UNPUBLISHED PROPRIETARY SOURCE CODE OF InterSoft Co.
* The copyright notice above does not evidence any
* actual or intended publication of such source code.
*
* $Id: dbrep.h,v 1.1.1.1 1996/04/16 18:51:39 gustavof Exp $
*
* DESCRIPTION
*********************************************************************/

#ifndef DBREP_H
#define DBREP_H

#ifndef DBTYPE_H
#	include <ideafix/dbtype.h>
#endif

#define ENV_VAR_CODE	01

#define	MAX_KEY_LENGTH	256		// Maximun length of a key

#define DBDATA_SEPAR	"\t"	// separator used by methods like getSets,
								// getIndices, etc.

#define isVirtualMember(f)		((f)&(f_creat_stamp|f_modif_stamp|f_pcreat_stamp))
#define hasStampMembers(f)		((f)&(type_creat_stamp|type_modif_stamp|type_pcreat_stamp))
#define hasPCreatMembers(f)		((f)&type_pcreat_stamp)
#define hasCreatMembers(f)		((f)&type_creat_stamp)
#define isCreatStampMember(f)	((f)&(f_creat_stamp|f_pcreat_stamp))
#define isModifStampMember(f)	((f)&(f_modif_stamp))
#define isStringMember(t)		((t) == TY_STRING)
#define isDateMember(t)			((t) == TY_DATE)
#define isTimeMember(t)			((t) == TY_TIME)
#define isNumericMember(t) 		((t) == TY_NUMERIC || (t) == TY_INTEGER || \
								 (t) == TY_BOOL    || (t) == TY_FLOAT)

#define OID_NAME		"oid"
#define OID_DESCR		"Object Id"
#define CTIME 			"ctime"
#define MTIME 			"mtime"
#define CDATE 			"cdate"
#define MDATE 			"mdate"
#define CUID  			"cuid"
#define MUID  			"muid"
#define LENUID				5	// create/modif. user id. length

#ifndef ANYVALUE_H
#	include <ifound/anyvalue.h>
#endif

class DbObjectRep;
class DbMemberRep;
class DbTypeRep;
class DbSetRep;
class DbIndexRep;
class DataBaseRep;
class DbIndexRepPart;
class DbIndexRepImplem;
class DataBaseRepImplem;
class DbSetRepImplem;
class DbCursorRep;
class DbCursorRepImplem;
class DbField;
class DbExpr;
class InCheck;
class EssDataBase;
class EssObjectPrefix;
class TimeStamp;
class DbObjectRepBuffer;
class DbObjectRepStack;
class DbEssIndex;
class DbObjectRepPrefix;
class MsgTable;
class DbObjectRepCache;
class CDataBaseRep;
class CDbSetRep;

struct UpdateOp;
struct DbPrivgRep;

// typedef used to define dbm handlers
typedef void (*DBHANDLER)(DbSetRep *, const String &, const String &);

class gtype(PtrArray, DbIndexRepPart);
class gtype(PtrArray, DbIndexRep);
class gtype(PtrArray, DbSetRep);
class gtype(PtrArray, DbMemberRep);
class gtype(PtrArray, DbCursorRep);
class gtype(PtrArray, DbPrivgRep);
class gtype(PtrArray, DataBaseRep);
class gtype(PtrArray, DbTypeRep);
class gtype(PtrArray, DbObjectRepBuffer);
class gtype(PtrArrayCursor, DbObjectRepBuffer);
class gtype(PtrArray, UpdateOp);

enum {
	dbms_essentia,
	dbms_ideafix,
};

// cursor flags
enum {
	c_range_gt		= (1 << 0),
	c_range_ge		= (1 << 1),
	c_range_lt		= (1 << 2),
	c_range_le		= (1 << 3),
	c_control_break	= (1 << 4),
	c_restart		= (1 << 5),
	c_send_range	= (1 << 6),
	c_go_last		= (1 << 7),
	c_timeout		= (1 << 8),
	c_key_fields	= (1 << 9),
};

// structure used by the update operation
struct UpdateOp {
	Int memberNo;
	Int op;
	Int from;
	Int to;
	AnyValue av;	
};

// grant and privileges
enum {
	// This is to indicate type of id.
	priv_group		= 0,
	priv_user		= 1,
	priv_public		= 2,	// internal used only

	// The privg values for schema privileges
	priv_use		= 0x1,
	priv_temp       = 0x2,
	priv_manip      = 0x4,
	priv_alter      = 0x8,

	// The privg values for table & field privileges
	priv_insert 	= 0x1,
	priv_delete 	= 0x2,
	priv_update 	= 0x4,
	priv_select		= 0x8,
	priv_all		= 0xf,

	all_privileges	= 0x03ff,
};

// Error constants
enum {
	io_locked			= -2,
	io_traborted		= -3,
	io_no_error			=  0,
	io_alter_perm,
	io_cant_connect,
	io_cant_creat,
	io_cant_mkdir,
	io_cant_open,
	io_dif_cpu,
	io_exist,
	io_illegal_mask,
	io_sch_corrupted,
	io_sch_locked,
	io_sch_not_found,
	io_tab_reference,
	io_temp_perm,
	io_too_many,
	io_trans_corrupt,
	io_use_perm,
	io_no_symbols,
	io_tab_not_found,
	io_no_curr_sch,
	io_no_members,
};

// for the get_tab_equiv function
enum {
	eq_identical	= 0x01000000,
	eq_force		= 0xFE000000,
	
	// Not fatal  differences

	eq_needs_change	= 0x00000000,
	eq_less_dim		= 0x02000000,
	eq_diff_mask	= 0x04000000,
	eq_less_len		= 0x08000000,
	eq_less_ndec	= 0x10000000,

	// Fatal  differences

	eq_no_field		= 0x20000000,
	eq_bad_type		= 0x40000000,
	eq_not_null		= 0x80000000,

	// equiv. masks
	
	eq_code_mask	= 0xFF000000,
	eq_field_mask	= 0x00FFFFFF,
};

inline Int eqCode(Int c)
{
	return c & eq_code_mask;
}

inline Int eqField(Int f)
{
	return f & eq_field_mask;
}

// for DbSetRep::copy method
enum {
	ct_normal,
	ct_copy_stamps,
};

// update operations
enum {
	u_add,
	u_sub,
	u_mul,
	u_div,
	u_set,
	u_copy,
};

// key field flags
enum {
	k_ascending		= 0,
	k_descending	= 1 << 0,
	k_not_null		= 1 << 1,
	k_auto_separ	= 1 << 2,
};

// internal data type
enum {
	it_char,
	it_byte,
	it_short,
	it_long,
	it_double,
	it_date,
	it_time,
	it_floatdec,
	it_num,
};

// field flags
enum {
	f_optional		=      0,
	f_not_null		= 1 << 0,
	f_resolve		= 1 << 1,
	f_cdigit		= 1 << 2,
	f_cdigit_dash	= 1 << 3,
	f_cdigit_slash	= 1 << 4,
	f_creat_stamp	= 1 << 5,
	f_modif_stamp	= 1 << 6,
	f_pcreat_stamp	= 1 << 7,
};

// key flags
enum {
	k_dup		= 	   0,
	k_unique	= 1 << 1,
	k_temp		= 1 << 2,
	k_fixed		= 1 << 3,
};

// table flags
enum {
	tab_normal		= 	   0,
	tab_temp		= 1 << 0,
	tab_recreation	= 1 << 4,
};

// type description flags
enum {
	type_normal			= 	   0,
	type_creat_stamp	= 1 << 1,
	type_modif_stamp	= 1 << 2,
	type_pcreat_stamp	= 1 << 3,
};

// find and lock modes
inline Int findMode(Int f)
{
	return f & (this_key|next_key|prev_key|first_key|last_key|
				partial_key|mode_seq);
}

inline Int lockMode(Int f)
{
	return f & (io_not_lock|io_lock|io_test|io_rlock|io_notify);
}

inline Int codeSet(Int dbId, Int setId)
{
	return ((dbId << 16) | setId);
}

inline void unCodeSet(Int id, Int &dbId, Int &setId)
{
	dbId	= (id >> 16) & 0xffff;
	setId	= id & 0xffff;
}

class DbObjectRep {

	useIdCheck(DbObjectRep_id);
	
	friend class DataBaseRep;
	friend class CDataBaseRep;
	friend class DbCursorRep;
	friend class DbEssIndex;		
	friend class DbEssSet;
	friend class EssObjectCache;
	
// private:

	DbSetRep *extent_;
	DbIndexRep *index_;
	DbTypeRep *typeRep_;
	DbObjectRepBuffer *buffer_;
	DbObjectRepStack *stack_;
	DbObjectRepCache *cache_;
	
	DbObjectRepBuffer *objectBuffer()		
		{ idCheck(); assert(buffer_ != NULL); return buffer_;	}
	DbIndexRep *index()		
		{ idCheck(); assert(index_ != NULL); return index_;		}
	DbObjectRepCache *cache()
		{ idCheck(); return cache_;								}
	
	DbObjectRepStack *stack();

	void create(void *buff, bool attachSet = true);

	void index(DbIndexRep *ind)			{ idCheck(); index_ = ind;		}
	void typeRep(DbTypeRep *t)			{ idCheck(); typeRep_ = t;	}

protected:

	DbObjectRep(DbSetRep *set, void *buff = NULL);

public:

	DbObjectRep(void *buff = NULL);
	DbObjectRep(const String &db, const String &extnt, void *buff = NULL);
	DbObjectRep(const String &extnt, void *buff = NULL);
	DbObjectRep(DataBaseRep *db, const String &extnt, void *buff = NULL);
	DbObjectRep(const DbObjectRep &);

	virtual ~DbObjectRep();
		
	bool accessBy(const String &name);
	bool accessBy(Int nIndex);
	
	void init();									// InitRecord
	void del();										// DelRecord	
	void add();										// AddRecord
	void add(const TimeStamp &crts, const TimeStamp &modts);	// AddRecordTS
	void put();										// PutRecord
	Int  get(Int mode = this_key|default_lock, Int nparts = 0);	// GetRecord
	void free(Int mode = this_key, Int nparts = 0);	// FreeRecord			
	Int  find(Int mode = this_key, Int nparts = 0);	// FindRecord
	                                                  
	void update(gtype(PtrArray,UpdateOp) &ops);		// UpdateRecord

	void copy(DbObjectRep *objRep);					// CopyAliasRecord
	
	void push();									// PushRecord	
	Int  pop();		                        		// PopRecord
	Int  stackSize();                       		// RecordStackSize            
	void discard();                         		// DiscardRecord
	void discardAll();                      		// DiscardAllRecords

	TimeStamp mTimeStamp();							// MTimeStamp
    TimeStamp cTimeStamp();							// CTimeStamp

	Int setCache(Int n, Int mode);			// SetDataCache, SetTableCache

	const String &name();
	DbSetRep &extent();

	// these routines should be private
	char *buffer();
	Int length();
	DbSetRep *set();	
};

class DataBaseRep {

	useIdCheck(DataBaseRep_id);

// private:

	friend class DataBaseRepImplem;
	friend class EssDataBase;
	friend class IxDataBase;
	friend class DbEssSet;
				
	String 							name_;
	String 							descr_;
	Int 							mode_;
	Int 							owner_;
	Int 							actPrivg_;	
	Int								id_;
	DataBaseRepImplem 				*implem_;
	gtype(PtrArray, DbPrivgRep)		&privgs_;
	gtype(PtrArray, DbSetRep)		&sets_;
	gtype(PtrArray, DbTypeRep)		&typesDescr_;
			
	static DBHANDLER dbHandler_;
	static MsgTable *dmsg;

	static gtype(PtrArray, DataBaseRep) *dataBases_;
	static gtype(PtrArray, DataBaseRep) &dataBases();
	static Int addDataBaseRep(DataBaseRep *db);
	static Int delDataBaseRep(DataBaseRep *db);
	static bool changeId(DataBaseRep *db, Int newId);

	static Int ioErrNo_;
	static Int ioErrDetail_;

	void calculateActPrivg(Int nopen = 0);

	void addPrivg(DbPrivgRep *p);

	void addTypeRep(DbTypeRep *typeRep);
	Int delTypeRep(DbTypeRep *typeRep);
	
	void name(const String &nm)		{ idCheck(); name_ = nm;	}
	void descr(const String &d)		{ idCheck(); descr_ = d;	}
	void mode(Int m)				{ idCheck(); mode_ = m;		}
	void owner(Int o)				{ idCheck(); owner_ = o;	}
	
	static const char *detailMsg(Int nerr);

	Int dropSet(DbSetRep *set);

protected:

	virtual void addSet(DbSetRep *set);
	virtual void freeSetSlot(DbSetRep *s);

	virtual DbSetRep *newDbSetRep(const String &name, const String &descr, 
								  DbTypeRep *typeRep, Int flags, Int size);
	virtual DbSetRep *newDbSetRep(const String &name = NULL_STRING);

public:

	DataBaseRep(const String &name, const String &descr);	// CreateSchema
	DataBaseRep(const String &name, Int mode, Int version = CURRENT_VERSION);
															// OpenSchema
	virtual ~DataBaseRep();
	
	static String msg(const char *m);
	static String msg(const String &m);
	static String msg(Int m);

	static DBHANDLER setDbHandler(DBHANDLER f);
	static void dbHandler(DbSetRep *set, const String &err, const String &par);
		
	static void ioErrNo(Int err);
	static void ioErrDetail(Int errd);
	static Int ioErrNo();
	static Int ioErrDetail();
	
	static void beginTransaction(Int st = io_eabort);	// BeginTransaction
    static Int rollBack();								// RollBack
	static void optimisticTransaction();				// OptimisticTransaction
	static bool endTransaction();						// EndTransaction
	static bool endTransaction(bool syn, bool rttrans);	// EndTrans
	static bool endSyncTransaction();					// EndSyncTransaction
	static bool transOk();								// TransOk
	static void corruptTrans();							// CorruptTrans
	static Int transMode();								// TransMode
	static bool inTransaction();						// InTransaction
	static void saneTrans();							// SaneTrans
		
	static String getDataBases();						// GetSchemas
	
	static bool checkPoint();							// DbCheckPoint
	static Int versionNumber(const String &schName, const Date &d);	
														// VersionNumber
	static DataBaseRep *find(const String &schName, Int vers = CURRENT_VERSION);
														// FindSchemaVersion
	
	void setDescr(const String &descr);		// SetDescrSchema
	Int drop();								// DropSchema
	Int lock(Int l);						// LockSchema
	void free();							// FreeSchema
	Int version();							// SchemaVersion
	void modify();							// ModifySchema

	Int dropSet(const String &name);		// DeleteTable
	Int dropSet(Int nSet);					// DeleteTable

	String getSets();						// GetTables
	
	static void reOpen(DataBaseRep *db);		// _ReOpenSchema
	
	static Int convert(DbSetRep *oldSet, DbSetRep *newSet, bool force, bool drop, 
					   IFCPVA err_func, FCPVA msg_func, unsigned int cache, 
					   unsigned int dcache, bool correctRefs);		
					   						// ConvertTable

	Int setSeqNumber(DbSetRep *set);			// _TabSeqNumber()

	void attach(DbSetRep *set);
	DbTypeRep *createTypeRep(const String &name, const String &descr, 
							      Int nMembers);
	Int deleteTypeRep(DbTypeRep *typeRep);
	
	DbSetRep *createSet(const String &name, const String &descr, 
					  Int flags, Int size, Int nMembers);
						  						// CreateTable
	DbSetRep *reCreateSet(const String &name, const String &descr, 
					  Int flags, Int size, Int nMembers);
												// _ReCreateTable
												
	bool chown(Int uid);						// ChownSchema

	const String &name()			{ idCheck(); return name_;	}
	const String &descr()			{ idCheck(); return descr_;	}
	Int mode()						{ idCheck(); return mode_;	}
	Int owner()						{ idCheck(); return owner_;	}
	Int id()						{ idCheck(); return id_;	}
	
	DataBaseRepImplem *implem()		
		{ idCheck(); assert(implem_ != NULL); return implem_;	}

	DbSetRep *set(const String &nm);
	Int nSets();
	DbSetRep *set(Int n);
	Int setNo(const String &nm);
	Int setNo(DbSetRep *s);
		
	virtual CDataBaseRep *getCDataBaseRep()		{ idCheck(); return NULL; }

	static Int dbmsType();

	static DataBaseRep *dataBase(const String &name);
	static DataBaseRep *current();
	static DataBaseRep *current(DataBaseRep *db);
	static Int nDataBases();
	static DataBaseRep *dataBase(Int i);
	static DataBaseRep *findNext(DataBaseRep *db);
};

class DbSetRep {

	useIdCheck(DbSetRep_id);
	
// private:

	friend class EssDataBase;
	friend class DataBaseRep;	
	friend class DbSetRepImplem;
	friend class DbEssSet;
	friend class DbCursorRep;
	friend class DbIndexRep;
		
	String	name_;
	String	descr_;
	Int		flags_;
	Int		size_;
	Int		mode_;
	Int		actPrivg_;

	gtype(PtrArray, DbPrivgRep) &privgs_;
	gtype(PtrArray, DbIndexRep) &indices_;
	gtype(PtrArray, DbCursorRep) &cursors_;
	DbTypeRep *typeRep_;
	DbSetRepImplem *implem_;
	DataBaseRep *dataBase_;

	void calculateActPrivg(Int nopen = 0);

	void descr(const String &d)		{ idCheck(); descr_ = d;		}
	void flags(Int f)				{ idCheck(); flags_ = f;		}
	void size(Int s)				{ idCheck(); size_ = s;			}
	void mode(Int m)				{ idCheck(); mode_ = m;			}
	void typeRep(DbTypeRep *t)		{ idCheck(); typeRep_ = t;	}
	void implem(DbSetRepImplem *i)	{ idCheck(); implem_ = i;		}
	void name(const String &nm)		{ idCheck(); name_ = nm;		}
	void actPrivg(Int p)			{ idCheck(); actPrivg_ = p;		}
	
	void addIndex(DbIndexRep *index);
	void delAllIndices();
	
	void addPrivg(DbPrivgRep *privg);
	void delAllPrivgs();
	
	Int addCursor(DbCursorRep *cur);
	void delCursor(DbCursorRep *cur);
	void delAllCursors();
		
	Int dropIndex(DbIndexRep *ind);
	
	bool create();
	
	void setPCreatStamp();			

	Int nIndices();
	
public:

	DbSetRep(DataBaseRep *dataBase, const String &name, const String &descr, 
		   DbTypeRep *typeRep, Int flags, Int size);
	DbSetRep(DataBaseRep *dataBase, const String &name = NULL_STRING);
	virtual ~DbSetRep();
	
	const String &name()		{ idCheck(); return name_;					}
	Int flags()					{ idCheck(); return flags_;					}
	const String &descr()		{ idCheck(); return descr_;					}
	Int size()					{ idCheck(); return size_;					}
	DbTypeRep *typeRep()	
				{ idCheck(); assert(typeRep_ != NULL); return typeRep_;	}	
	DataBaseRep *dataBase()		
				{ idCheck(); assert(dataBase_ != NULL);	return dataBase_;	}
	DbSetRepImplem *implem()		
				{ idCheck(); assert(implem_ != NULL); return implem_;		}

	DbCursorRep *cursor(Int n);

	bool wasAttached();
	
	bool hasVirtuals();
	bool hasPCreatStamp();
	bool hasCreatStamp();
		
	bool isTemp()				{ idCheck(); return flags_&tab_temp; }

	Int nMembers();
	Int memberNo(const String &n);
	Int memberNo(DbMemberRep *m);
	DbMemberRep *member(const String &n);
	DbMemberRep *member(Int n);

	DbIndexRep *index(const String &name);
	DbIndexRep *index(Int nIndex);
	DbIndexRep *primaryKey();
	Int indexNo(const String &name);
				
	Int typeLength();
	
	Int get(DbObjectRep *obj, Int mode, Int nparts);
	void free(DbIndexRep *index, DbObjectRepBuffer *buffer, Int mode, Int nparts);
	Int find(DbIndexRep *index, DbObjectRepBuffer *buffer, Int mode, Int nparts);
	void init(DbObjectRepBuffer *buffer);
	void del(DbObjectRepBuffer *buffer);
	void add(DbObjectRepBuffer *buffer);
	void add(DbObjectRepBuffer *buffer, const TimeStamp &crts, 
			 const TimeStamp &modts);
	void put(DbObjectRepBuffer *buffer);
	void push(DbObjectRepBuffer *buffer, DbObjectRepStack *stack);
	Int pop(DbObjectRep *obj, DbObjectRepStack *stack);
	void discard(DbObjectRepStack *stack);
	String checkObject(DbObjectRepBuffer *buffer, String &par, Int nInd = ERR);
	void update(gtype(PtrArray,UpdateOp) &ops);
				
	Int rename(const String &newName);	// RenameTable
	Int delAll();						// DelAllRecords
	Int lock(Int lockMode);				// LockTable
	void free();						// FreeTable
	void flush();						// FlushTable
	Int actualSize();					// TabActualSize
	Int nObjects();						// TabNRecords

	Int dropIndex(const String &indname);	// DropIndex
	Int dropIndex(Int indnum);				// DropIndex

	DbMemberRep *addMember(const String &name, const String &descr, 
							  Int dim, Int ty, Int sqlTy, Int len, Int ndec, 
							  Int flags, const String &def, 
							  const String &mask);
											// AddTabField

	bool renameMember(DbMemberRep *m, const String &newName);// RenameField

	TimeStamp mTimeStamp(DbObjectRepBuffer *buffer);
    TimeStamp cTimeStamp(DbObjectRepBuffer *buffer);

	String getIndices();				// GetIndices
	String getMembers();				// GetTabFlds

										// CreateIndex
	DbIndexRep *createIndex(const String &name, Int flags, Int separ);

	Int setDeflt(DbMemberRep *m, DbExpr *deflt);	// SetDeflt
	Int setRelCheck(DbMemberRep *m, DbExpr *rchk);	// SetRelCheck
	Int setCollSet(DbMemberRep *m, const String &cs);// SetCollateSet
	Int setInCheck(DbMemberRep *m, InCheck *ic);		// SetInCheck

	Int *getEquiv(DbSetRep *to);							// GetTabEquiv
	void copy(DbSetRep *to, Int *members, Int flag);		// CopyTable

	void copyPrivgs(DbSetRep *from);						// CopyPrivg

	virtual CDbSetRep *getCDbSetRep()	{ idCheck(); return NULL; }
};


class DbObjectRepBuffer {

// private:

	DbObjectRepPrefix *objectPrefix_;
	Int objectNo_;
	char *buffer_;
	DbSetRep *set_;
	bool wasAlloced_;
		
public:

	DbObjectRepBuffer(DbSetRep *set, void *buffer = NULL);
	~DbObjectRepBuffer();
		
	DbObjectRepBuffer &operator=(DbObjectRepBuffer &ob);
  
	char *buffer();
	Int length();
	Int fullLength();

	DbObjectRepPrefix *objectPrefix()
		{ assert(objectPrefix_ != NULL); return objectPrefix_;	}
	Int objectNo()
		{ return objectNo_;										}
	void objectNo(Int oNo)
		{ objectNo_ = oNo;										}
};

class DbObjectRepStack {

// private:

	char	*stack_;	// Object stack
	Int		aObjs_;		// Room for this many objects w/out realloc
	Int		nObjs_;		// Current number of objs

public:

	DbObjectRepStack();
	~DbObjectRepStack();

	void push(char *buff, Int len);
	char *pop(Int len);			

	Int size()				{ return nObjs_;	}

	void discardAll();	
};

class DbMemberRep {

	useIdCheck(DbMemberRep_id);
	
// private:

	friend class DbSetRepImplem;
	friend class DbEssSet;
		
	String		name_;
	String		descr_;
	Int			dim_;
	Int			offset_;
	Int			mType_;
	Int			mIType_;
	Int			sqlType_;
	Int			flags_;
	Int			len_;
	Int			nDec_;
	String		mask_;
	DbExpr		*relCheck_;
	DbExpr		*deflt_;
	InCheck		*inCheck_;
	String		cMask_;
	String		collset_;
	DbField	*fld_;				// used only by Essentia DBM

	void name(const String &n)		{ idCheck(); name_ = n;				}
	void mType(Int t)				{ idCheck(); mType_ = t;			}
	void deflt(DbExpr *d)			{ idCheck(); deflt_ = d;			}
	void mask(const String &m)		{ idCheck(); mask_ = m;				}
	void dim(Int d)					{ idCheck(); dim_ = d;				}
	void len(Int l)					{ idCheck(); len_ = l;				}
	void mIType(Int it)				{ idCheck(); mIType_ = it;			}
	void cMask(const String &cm)	{ idCheck(); cMask_ = cm;			}
	void nDec(Int nd)				{ idCheck(); nDec_ = nd;			}
	void inCheck(InCheck *ic)		{ idCheck(); inCheck_ = ic;			}
	void relCheck(DbExpr *rc)		{ idCheck(); relCheck_ = rc;		}
	void collset(const String &cs)	{ idCheck(); collset_ = cs;			}
	void descr(const String &d)		{ idCheck(); descr_ = d;			}
	void flags(Int f)				{ idCheck(); flags_ = f;			}
	void offset(Int o)				{ idCheck(); offset_ = o;			}
	void sqlType(Int sqlt)			{ idCheck(); sqlType_ = sqlt;		}
		
	Int calcIType();
	Int calcStampFlags();

public:

	DbMemberRep(const String &name, const String &descr, Int flags, 
				   Int type, Int sqlTy, const String &mask, Int dim, 
				   Int len, Int ndec, Int offset, DbExpr *deflt = NULL, 
				   DbExpr *relCheck = NULL, InCheck *inCheck = NULL,
				   const String &collset = NULL_STRING);
	~DbMemberRep();
	
	bool isNull();			// IsNull

	Int mType()				{ idCheck(); return mType_; 					}
	Int len()				{ idCheck(); return len_;						}
	Int flags()				{ idCheck(); return flags_;						}
	Int dim()				{ idCheck(); return dim_;						}
	DbExpr *deflt()			{ idCheck(); return deflt_;						}
	bool isString()			{ idCheck(); return isStringMember(mType_);		}
	bool isNumeric()		{ idCheck(); return isNumericMember(mType_);	}	
	bool isDate()			{ idCheck(); return isDateMember(mType_);		}
	bool isTime()			{ idCheck(); return isTimeMember(mType_);		}
	const String &collset()	{ idCheck(); return collset_;					}
	const String &mask()	{ idCheck(); return mask_;						}
	const String &cMask()	{ idCheck(); return cMask_;						}
	DbField *fld()			{ idCheck(); return fld_;						}
	Int offset()			{ idCheck(); return offset_;					}
	bool isVirtual()		{ idCheck(); return isVirtualMember(flags_);	}
	bool isCreatStamp()		{ idCheck(); return isCreatStampMember(flags_);	}
	bool isModifStamp()		{ idCheck(); return isModifStampMember(flags_);	}
	Int mIType()			{ idCheck(); return mIType_;					}
	const String &name()	{ idCheck(); return name_;						}
	Int nDec()				{ idCheck(); return nDec_;						}
	const String &descr()	{ idCheck(); return descr_;						}
	InCheck *inCheck()		{ idCheck(); return inCheck_;					}
	Int sqlType()			{ idCheck(); return sqlType_;					}
		
	void setPCreatStamp();

	Int storeLength(Int l = ERR);
	void store(char *buff, char *&key);		
	void load(char *buff, const char *&key);		
	
	void set(char *buffer, const AnyValue &av, Int row = 0, Int rep = 1,
			 Int offset = 0, Int len = ERR);
	Int cmp(char *buffer, Int dima, Int dimb);
	AnyValue get(char *buffer, Int row = 0, Int offset = 0, Int len = ERR);
	void init(DbSetRep *set, char *buffer);
	AnyValue value(const String &val);
};

class DbIndexRep {

	useIdCheck(DbIndexRep_id);
	
// private:

	friend class DbEssSet;
	friend class EssObjectCache;
		
	String name_;
	Int flags_;
	Int storeLength_;
	DbIndexRepImplem *implem_;
	gtype(PtrArray, DbIndexRepPart) &parts_;
	DbSetRep *set_;
	
	void implem(DbIndexRepImplem *i)		{ idCheck(); implem_ = i;	}
	
	char *buildKey(DbObjectRepBuffer *buffer);
	void unBuildKey(DbObjectRepBuffer *buffer, const char *key);
			
public:

	DbIndexRep(DbSetRep *dbSet, const String &name, Int flags);
	DbIndexRep(const String &n, Int f);
	~DbIndexRep();
										
	Int build(Int flags, ...);		// BuildIndex
	Int complete();					// CompleteIndex
	Int addKey(DbObjectRep &obj);		// AddKey
	
	Int addMember(DbMemberRep *member, Int flags, Int off, Int len);
									// AddIndField
	
	void addPart(DbIndexRepPart *p);
	
	const String &name()			{ idCheck(); return name_;			}	
	Int flags()						{ idCheck(); return flags_;			}
	Int storeLength()				{ idCheck(); return storeLength_;	}
	bool isTemp()					{ idCheck(); return flags_&k_temp;	}
	bool isUnique()					{ idCheck(); return flags_&k_unique;}
	bool isPrimaryKey();

	DbIndexRepImplem *implem()		
					{ idCheck(); assert(implem_ != NULL); return implem_;	}
	DbSetRep *set()	{ idCheck(); assert(set_ != NULL); return set_;			}

	
	Int nParts();
	DbIndexRepPart *part(Int n);
	bool isPart(DbMemberRep *m);
	
	Int cmp(char *buffer1, char *buffer2);
	void setNullValue(DbObjectRepBuffer *buffer);
	void setHighValue(DbObjectRepBuffer *buffer);
	bool checkKey(DbObjectRepBuffer *buffer);
	bool findKey(DbSetRep *set, DbObjectRepBuffer *buffer, Int fmode = this_key, 
				 Int nparts = 0, char **key_ptr = NULL, Int lmode = io_lock);
	Int cmpKey(DbObjectRepBuffer *buff1, DbObjectRepBuffer *buff2);

	String getMembers();				// GetIndFlds

	void setCache(Int n);				// SetIndexCache
};

class DbTypeRep {

	useIdCheck(DbTypeRep_id);
	
// private:

	String name_;
	String descr_;
	Int   len_;
	Int	  flags_;
	Int	  maxMembers_;
	gtype(PtrArray, DbMemberRep) &members_;
	DataBaseRep *dataBase_;

	Int sizeOf(Int ty);
	void align(Int ty);

	DataBaseRep *dataBase()	
		{ idCheck(); assert(dataBase_ != NULL); return dataBase_; }

public:
	
	DbTypeRep(DataBaseRep *db, const String &name, const String &descr, 
				 Int nMembers);
	~DbTypeRep();

	DbMemberRep *addMember(const String &name, const String &descr, 
							  Int dim, Int ty, Int sqlTy, Int len, Int ndec, 
							  Int flags, const String &deflt, 
							  const String &mask);
	DbMemberRep *addMember(const String &nm, const String &descr, 
							  Int flags, Int ty, Int sqlTy, 
							  const String &mask, Int dim, Int len, Int ndec,
							  DbExpr *deflt = NULL, DbExpr *relCheck = NULL, 
							  InCheck *incheck = NULL, 
							  const String &collset = NULL_STRING);

    Int memberNo(const String &n);
	Int memberNo(DbMemberRep *m);
	DbMemberRep *member(Int mno);
	DbMemberRep *member(const String &n);
	
	Int len()				{ idCheck(); return len_;	}
	Int flags()				{ idCheck(); return flags_;	}

	Int nMembers();
	bool isCompleted();

	String getMembers();

	void setPCreatStamp();			
};

// This structure holds access privileges for users
struct DbPrivgRep {
	Int id_;		// A group or user id.
	Int idType_;	// Is group or user id.
	Int privg_;		// Bits with privileges

	DbPrivgRep(Int id, Int idType, Int privg)
	:	id_(id), idType_(idType), privg_(privg) 
	{ }
};

class DbIndexRepPart {

	useIdCheck(DbIndexRepPart_id);
	
// private:

	friend class DbSetRepImplem;
	friend class DbEssSet;
	
	Int				mIType_;
	Int				flags_;
	Int				len_;
	Int				offset_;
	DbMemberRep	*member_;

	void member(DbMemberRep *m)		{ idCheck(); member_ = m;	}
	void flags(Int f)				{ idCheck(); flags_ = f;	}
	void mIType(Int it)				{ idCheck(); mIType_ = it;	}
	void offset(Int o)				{ idCheck(); offset_ = o;	}
	
public:

	DbIndexRepPart(DbMemberRep *member, Int mIType, Int flags, Int len, 
				 Int offset);
	~DbIndexRepPart();

	DbMemberRep *member()			{ idCheck(); return member_;}
	Int offset()					{ idCheck(); return offset_;}
	Int len()						{ idCheck(); return len_;	}
	Int flags()						{ idCheck(); return flags_;	}
	Int storeLength();
};

class DbCursorRep {

	useIdCheck(DbCursorRep_id);

	friend class DbCursor;
	friend class DataBaseRepImplem;
	friend class EssDataBase;
	friend class DbCursorRepImplem;
	friend class DbEssCursor;
	friend class DbIxCursor;
			
// private:

	DbObjectRep *obj_;			// DbObjectRep associated with this cursor
	DbIndexRep *ind_;			// index used
	Int lMode_;					// lock mode
	Int flags_;					// range flags
	Int wkFlags_;				// work flags
	Int fNRec_;					// there are fnrecs forwarded records in cache
	Int bNRec_;					// there are bnrecs backwarded records in cache
	bool moreFRecs_;			// are there more forwarded records ?
	bool moreBRecs_;			// are there more backwarded records ?
	DbObjectRepBuffer *rangeFrom_;// from range
	DbObjectRepBuffer *rangeTo_;	// to range
	DbObjectRepBuffer *currRange_;// current range
	Int rangeMember_;			// current member inside the current range
	gtype(PtrArray, DbMemberRep) &members_;
						// fields array which this cursor was created
	gtype(PtrArray, DbObjectRepBuffer) &objBuffers_;
	gtype(PtrArrayCursor, DbObjectRepBuffer) &objBuffCur_;
	DbCursorRepImplem *implem_;
	Int id_;
	
	DbObjectRep *object()	{ idCheck(); assert(obj_ != NULL); return obj_;	}
	void flags(Int f)		{ idCheck(); flags_ = f;						}
	void wkFlags(Int f)		{ idCheck(); wkFlags_ = f;						}
	void fNRec(Int n)		{ idCheck(); fNRec_ = n;						}
	void bNRec(Int n)		{ idCheck(); bNRec_ = n;						}
	void moreFRecs(Int n)	{ idCheck(); moreFRecs_ = n;					}
	void moreBRecs(Int n)	{ idCheck(); moreBRecs_ = n;					}
		
	Int flags()				{ idCheck(); return flags_;						}
	Int wkFlags()			{ idCheck(); return wkFlags_;					}
	Int lMode()				{ idCheck(); return lMode_;						}
	Int fNRec()				{ idCheck(); return fNRec_;						}
	Int bNRec()				{ idCheck(); return bNRec_;						}

	DbObjectRepBuffer *rangeFrom()	
		{ idCheck(); assert(rangeFrom_ != NULL); return rangeFrom_;	}
	DbObjectRepBuffer *rangeTo()		
		{ idCheck(); assert(rangeTo_ != NULL); return rangeTo_;		}
	DbCursorRepImplem *implem()		
		{ idCheck(); assert(implem_ != NULL); return implem_;		}
	
	gtype(PtrArray, DbMemberRep) &members();

	void reset();
	void resetValues(bool forw);
	void clearRanges();
	bool subFRecord();
	bool subBRecord();
	void addFRecord();
	void addBRecord();
	void moveFOffset();
	void moveBOffset();
	Int setRecord(bool forw);
	Int setRecord();
	void prepareRange();

	Int nMembers();
	DbMemberRep *member(Int i);
	void reAllocBuffers(Int n);
	DbObjectRepBuffer *objectBuffer(Int i);
	void goCurrObjTo(Int n);
	void goCurrObjToFirst();
	void goCurrObjToLast();
			
	void setRangeValue(DbObjectRepBuffer *buffer, int nfld, const AnyValue &av);
	Int addMember(const String &name);
	void setCond(Int range);
	void addRangeValue(const AnyValue &av);

	bool haveToSendRange();
		
public:

	DbCursorRep(DbObjectRep &obj, const String &indName = NULL_STRING, 
			  Int mode = io_not_lock);					// CreateCursor
	~DbCursorRep();										// DeleteCursor

		
	Int fetch();										// FetchCursor
	Int fetchPrev();									// FetchCursorPrev
	Int fetchThis();									// FetchCursorThis
	void moveFirst();									// MoveCursorFirst
	void moveLast();									// MoveCursorLast
	Int count();										// CountCursor

	void setFromFld(int nfield, const AnyValue &av);	// SetCursorFromFld
	void setToFld(int nfield, const AnyValue &av);		// SetCursorToFld
	
	Int id()				{ idCheck(); return id_; }
	DbIndexRep *index()		{ idCheck(); assert(ind_ != NULL); return ind_;	}

};

#endif
