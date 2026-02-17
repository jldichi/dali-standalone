/*********************************************************************
* Copyright (c) 1987 InterSoft Co.  All Rights Reserved
* THIS IS UNPUBLISHED PROPRIETARY SOURCE CODE OF InterSoft Co.
* The copyright notice above does not evidence any
* actual or intended publication of such source code.
*
* db.h,v 1.2 1996/04/30 19:38:19 gab Exp
*
* DESCRIPTION
* Definitions & Interface header for IO routines.
*
* Files that must be included before this:
* ----------------------------------------
*********************************************************************/

#ifndef DB_H
#define DB_H

#ifdef HAVE_CREATECURSOR_CONFLICT
#   define CreateCursor    DbCreateCursor
#endif

/* table, field and index descriptor types */

typedef long	schema;
typedef long	dbtable;
typedef long	dbfield;
typedef long	dbindex;
typedef long	dbcursor;

/* temporary */
#include <ifound/num.h>

/* Find modes for get, free and find */

typedef short find_mode;
/* Este modo se dejo de soportar #	define BY_RECNO	 (find_mode) 0 */

#define THIS_KEY	 (find_mode) 1
#define NEXT_KEY	 (find_mode) 2
#define PREV_KEY	 (find_mode) 4
#define FIRST_KEY	 (find_mode) 8
#define LAST_KEY	 (find_mode) 16
#define PARTIAL_KEY	 (find_mode) 32
#define	GETKEY_FLDS	 (find_mode) 64
#define MODE_SEQ	 (find_mode) 64
#define NEXT_SEQ	 (NEXT_KEY|MODE_SEQ)
#define PREV_SEQ	 (PREV_KEY|MODE_SEQ)

/* Table flags */

typedef short tab_flags;
#define 	TAB_NORMAL			(tab_flags) 0x00
#define 	TAB_TEMP			(tab_flags) 0x01
#define		TAB_CREAT_STAMP		(tab_flags) 0x02
#define		TAB_MODIF_STAMP		(tab_flags) 0x04
#define		TAB_PCREAT_STAMP	(tab_flags) 0x08
#define		TAB_RECREATION		(tab_flags) 0x10

/* Key flags */

typedef short key_flags;
#define	K_DUP		(key_flags) 0
#define	K_UNIQUE	(key_flags) 1
#define	K_TEMP		(key_flags) 2
/*#define K_AUTO_SEPAR	(key_flags) 4*/
#define	K_FIXED		(key_flags) 8
#define K_COMPLETE	(key_flags) 16
/*	Key Field Flags	*/

typedef short k_field_flags;
#define	K_ASCENDING		(k_field_flags) 0
#define	K_DESCENDING	(k_field_flags) 1
#define	K_NOT_NULL		(k_field_flags) 2
#define	K_AUTO_SEPAR	(k_field_flags) 4

/* Field Flags */

typedef short field_flags;
#define	F_OPTIONAL		((field_flags) 0)
#define	F_NOT_NULL		((field_flags) (1 << 0))
#define	F_RESOLVE		((field_flags) (1 << 1))
#define F_CDIGIT		((field_flags) (1 << 2))
#define F_CDIGIT_DASH	((field_flags) (1 << 3))
#define F_CDIGIT_SLASH	((field_flags) (1 << 4))
/* #define <FREE>	((field_flags) (1 << 5)) */
#define F_CREAT_STAMP	((field_flags) (1 << 6))
#define F_MODIF_STAMP	((field_flags) (1 << 7))
#define F_PCREAT_STAMP	((field_flags) (1 << 8))

/* UpdateRecord() operations */
typedef int update_op;
#define U_ADD		((update_op) 0)
#define U_SUB		((update_op) 1)
#define U_MUL		((update_op) 2)
#define U_DIV		((update_op) 3)
#define U_SET		((update_op) 4)
#define U_COPY		((update_op) 5)

/*    GRANT  &   PRIVILEGES									*/
/* This is to indicate type of id.    						*/
#define PRIV_GROUP			0
#define PRIV_USER			1
#define PRIV_PUBLIC			2	/* USED ONLY INTERNALLY !!! */

#define PRIV_NO_USER		65535
/* The privg values for schema privileges					*/
#define PRIV_USE        (UShort)0x1
#define PRIV_TEMP       (UShort)0x2
#define PRIV_MANIP      (UShort)0x4
#define PRIV_ALTER      (UShort)0x8

/* The privg values for table & field privileges			*/
#define PRIV_INSERT 	(UShort)0x1
#define PRIV_DELETE 	(UShort)0x2
#define PRIV_UPDATE 	(UShort)0x4
#define PRIV_SELECT		(UShort)0x8
#define PRIV_ALL		(UShort)0xf

#define ALL_PRIVILEGES	(UShort)0x03ff

#define CURRENT_VERSION	(0L)

/* To convert SEL or UPD to FULL							*/
#define FULL(oper)		((oper)<<2|(oper))

/* The GRANT bits											*/
#define GRANT(p)		((p)<<6|(p))

/* To check a privilege										*/
#define PRVCHECK(priv,oper)	(((priv)&(oper))==(oper)?OK:ERR)

#define CheckTabPriv(tp, oper) PRVCHECK(tp->act_privg, oper)

/* Modes for LogStatus	*/

#define LOG_OFF			0
#define LOG_ENABLED		1
#define LOG_DISABLED	2
#define LOG_ROLLBACK	3

/* structure to hold a value, a data-base field or a form field */
/* Used in 'in' clauses */

typedef short rvaltype;
#define V_DBFIELD	0
#define V_FMFIELD	1
#define V_CONST		2
#define V_ENVVAR	3

/* defines for CopyTable Routine */
#define	CT_NORMAL		0x00
#define CT_COPY_STAMPS	0x01

/* IO operation modes */

/* Common to public */
#define	IO_DEFAULT		(0)
#define	IO_NOT_LOCK		(0)
#define	IO_WRITE		(0)
#define	IO_LOCK			(1 << 0)
#define	IO_TEST			(1 << 1)
#define	IO_EABORT		(1 << 2)
#define IO_SYMBOLS		(1 << 4)
#define IO_ALL_INDEX    	(1 << 6)
#define IO_CLEAR_INDEX  	(1 << 7)
#define IO_VERBOSE		(1 << 8)
#define IO_BLOCKED		(1 << 12)
#define IO_PLOCK		(1 << 13)
#define IO_READ	   		(1 << 14)

#define IO_RLOCK		(1 << 14)		/* IO_READ */

#define IO_SCROLL	   	(1 << 16)

/* Just for schemas */
#define IO_NOT_RENV		(1 << 5)
#define	IO_INC_RLOCK		(1 << 9)
#define	IO_DEC_RLOCK		(1 << 10)
#define IO_FORCE_OPEN		(1 << 11)
#define IO_NOBINARY		(1 << 15)		/* IO_CONTROL_BREAK */
#define IO_INTEGRITY            (1 << 15)		/* IO_CONTROL_BREAK */

/* Just for cursors */
#define IO_KEY_FIELDS		(1 << 3)
#define IO_NEW_CURSOR		(1 << 5)		/* Para cursores viejos de IDX. cristiand */
#define IO_CONTROL_BREAK	(1 << 15)
#define IO_KEEP_LOCKS		(1 << 11)		/* IO_FORCE_OPEN */
#define IO_DESCENDING		(1 << 6)	  	/* IO_ALL_INDEX  */

/* IO operations status		*/
#ifdef __cplusplus
	extern "C" {
#endif

#ifndef DLLDEPEND
#	define DLLDEPEND
#endif 
extern DLLDEPEND int _io_errno_;
extern DLLDEPEND int _io_err_detail;

#ifdef __cplusplus
	}
#endif


#define SetIoErrno(nerr)		(_io_errno_ = (nerr),ERR)
#define GetIoErrno()			(_io_errno_)
#define SetIoErrDetail(ndet)	_io_err_detail = ndet
#define GetIoErrDetail()		(_io_err_detail)
					
/* DBMS types */			
#define DBMS_ESSENTIA	1		
#define DBMS_IDEAFIX	2		
#define DBMS_SQLFW	3

// Estos son negativos a proposito por si se agregan mas DBMS a Sqlfw en mdmisc.h
// en el enum correspondiente.
#define PIQL_DBMS      -3
#define IDEAFIX_DBMS   -2
#define ESSENTIA_DBMS  -4
// Los siguientes salen de $ISRC/include/sqldb/metadata/mdmisc.h
// Mantener la unicidad.
#define UNKNOWN_DBMS   -1
#define INFORMIX_DBMS   0
#define ORACLE_DBMS     1
#define POSTGRE_DBMS	2

/* Error constants for _io_err_detail  */
#define IO_ALTER_PERM		(1)
#define IO_CANT_CONNECT		(2)
#define IO_CANT_CREAT		(3)
#define IO_CANT_MKDIR		(4)
#define IO_CANT_OPEN		(5)
#define IO_DIF_CPU			(6)
#define IO_EXIST			(7)
#define IO_ILLEGAL_MASK		(8)
#define IO_SCH_CORRUPTED	(9)
#define IO_SCH_LOCKED		(10)
#define IO_SCH_NOT_FOUND	(11)
#define IO_TAB_REFERENCE	(12)
#define IO_TEMP_PERM		(13)
#define IO_TOO_MANY			(14)
#define IO_TRANS_CORRUPT	(15)
#define IO_USE_PERM			(16)
#define IO_NO_SYMBOLS		(17)
// These defines are included to allow compatibility with version 432.
#define IO_CHECK_TAB 		(18)
#define IO_CHECK_FLD		(19)
#define IO_NULL_VALUE		(20)
#define IO_NO_PERMS			(21)
#define IO_INLST_FAILED		(22)
#define IO_REFER_FAILED		(23)
#define IO_DUP_UNIQUE		(24)



/* Macros */

#define AlFld(tab,fld) ((tab)|((fld)&0xFF))
#define AlInd(tab,ind) ((tab)|((ind)&0xFF))
#define Use(schema)    (void)OpenSchema(schema,IO_EABORT)

/* Because of collision with RM-COBOL 85 AddKey() while linking runcobol */
#define AddKey			AddIndKey

#define DoTransaction		for (BeginTrans(0); TransOk(); EndTrans(FALSE, TRUE))
#define DoSyncTransaction	for (BeginTrans(0); TransOk(); EndTrans(TRUE, TRUE))
#define VInd(ind)	_DbVerifyConsist(__FILE__, __LINE__, ind)

/* Constants */

#define IO_LOCKED		(-2)
#define IO_TRABORTED	(-3)
#define CURRENT_SCHEMA ((schema)0)

/* Binary data support */

#include <cfix/dbbin.h>

/* ++ Prototypes ++ */

STRUCT InCheck;
STRUCT DbExpr;
STRUCT DbmExpr;

#ifdef __cplusplus
extern "C"  {
#endif

#ifdef HAVE_REAL_LOCKS
extern bool idea_lock(int fd, int mode, long length);
#else
extern bool idea_lock(int fd, int mode, long offset, long length);
#endif
extern int GetDBMSName(schema sc);
extern long BuildIndex(dbindex ind, int flags, ...);
extern int CompleteIndex(dbindex ind);
extern void CopyTable(dbtable t_to, dbtable t_from, dbfield *flds, int flag);
extern int CopyAliasRecord(dbtable t_to, dbtable t_from);
extern dbfield *GetTabEquiv(dbtable t_to, dbtable t_from);
extern schema CreateSchema(char *name, char *descr);
extern void SetDescrSchema(schema sch, char *descr);
extern dbtable CreateTable(schema sch, char *name, char *descr, tab_flags flags, ULong size, UChar n_flds);
extern long AddIndKey(dbindex ind);
extern dbfield AddTabField(dbtable tbl, char *fname, char *descr, UShort dim, type ty, sqltype sql_ty, UShort len, UChar ndec, field_flags flags, char *def, char *mask);
extern bool DbCheckPoint(schema sch);
extern bool DbFlushReq(schema sch);
extern int RenameTable(dbtable tab, char *name);
extern bool RenameField(dbfield fld, char *name);
extern dbindex CreateIndex(dbtable tbl, char *name, key_flags flags, UChar separ);
extern int AddIndField(dbindex ind, dbfield fld, k_field_flags flags, UShort n, UShort m);
extern int DelAllRecords(dbtable tab);
extern int DropSchema(schema sch);
extern dbtable FindTabReference(dbtable tab);
extern int DropTable(dbtable tab);
extern int DeleteTable(dbtable tab);
extern int DropIndex(dbindex ind);
extern dbcursor CreateCursor(dbindex ind, int mode);
extern void SetCursorFrom(dbcursor cursor, ...);
extern void SetCursorTo(dbcursor cursor, ...);
extern int SetCursorFlds(dbcursor cursor, ...);
extern int SetCursorVFlds(dbcursor cursor, dbfield *fld);
extern long FetchCursorPrev(dbcursor cursor);
extern long FetchCursor(dbcursor cursor);
extern long FetchCursorThis(dbcursor cursor);
extern void MoveCursorFirst(dbcursor cursor);
extern void MoveCursorLast(dbcursor cursor);
extern void SetCursorFromFld(dbcursor cursor, int nfield, char *value);
extern void SetCursorToFld(dbcursor cursor, int nfield, char *value);
extern void DeleteCursor(dbcursor cursor);
extern long CountCursor(dbcursor cursor);
extern dbfield FindDbField(dbtable td, const char *name);
extern dbindex FindDbIndex(dbtable td, const char *name);
extern dbfield KeyDbField(dbindex ind, int n);
extern char *InDescr(dbfield fld, char *val);
extern int LockSchema(schema sch, int lock_mode);
extern void FreeSchema(schema sch);
extern int LockTable(dbtable tab, int lock_mode);
extern void FreeTable(dbtable tab);
extern schema OpenSchema(char *name, UShort mode);
extern long VersionNumber(char *name, DATE d);
extern long SchemaVersion(schema sch);
extern schema OpenSchemaVersion(char *name, UShort mode, long version);
extern long DbCheckSum(schema sch);
extern void DbVerifyCheckSum(schema sch, long chksum);
extern int TabNFields(dbtable td);
extern int IndNFields(dbtable ind);
extern FPLCPCP SetDbHandler(FPLCPCP f);
extern void FreeRecord(dbindex ind, find_mode fmode, ...);
extern long FindRecord(dbindex ind, find_mode fmode, ...);
extern long GetRecord(dbindex ind, find_mode fmode, int mode, ...);
extern void PushRecord(dbtable tbl);
extern long PopRecord(dbtable tbl);
extern long RecordStackSize(dbtable tbl);
extern void DiscardRecord(dbtable tbl);
extern void DiscardAllRecords(dbtable tbl);
extern schema FindSchema(char *name);
extern schema FindSchemaVersion(char *name, long version);
extern schema FindNextSchema(schema sch);
extern schema CurrentSchema(void);
extern void SetFld(dbfield f, char *val, ...);
extern void SetIFld(dbfield f, int val, ...);
extern void SetLFld(dbfield f, long val, ...);
extern void SetFFld(dbfield f, double val, ...);
extern void SetDFld(dbfield f, DATE val, ...);
extern void SetTFld(dbfield f, TIME val, ...);
extern void SetNFld(dbfield f, NUM *val, ...);
extern void GetFld(dbfield f, char *val, ...);
extern void CopyFld(dbfield from, dbfield to, ...);
extern void GetDspFld(dbfield f, char *val, ...);
extern bool IsNull(dbfield f, ...);
extern UShort FldRows(dbfield f);
extern int LenDspFld(dbfield f);
extern char *SFld(dbfield f, ...);
extern int IFld(dbfield f, ...);
extern long LFld(dbfield f, ...);
extern double FFld(dbfield f, ...);
extern DATE DFld(dbfield f, ...);
extern TIME TFld(dbfield f, ...);
extern NUM NFld(dbfield f, ...);
extern void SetKey(dbindex ind, ...);
extern void SetKeyFld(dbindex ind, int n, char *val);
extern void GetKeyFld(dbindex ind, int n, char *val);
extern void GetKeyStr(dbindex ind, char *val);
extern void _DbVerifyConsist(char *f, int line, dbindex ind);
extern dbtable FindDbTable(schema sd, const char *name);
extern dbtable CreateAlias(dbtable t);
extern void DeleteAlias(dbtable t);
extern void DeleteAllAlias(schema sch);
extern void DeleteAllCursors(schema sch);
extern long TabActualSize(dbtable tab);
extern long TabNRecords(dbtable tab);
extern void InitRecord(dbtable tab);
extern void PutRecord(dbtable tab);
//extern void UpdateRecord(dbtable tab, ...);
extern int UpdateRecord(dbtable tab);  /*! JC: BZ 3368 */

extern void AddRecord(dbtable tab);
extern long AddRecordTest(dbtable tab);
extern void CloseAllSchemas(void);
extern int LogStatus(void);
extern void CloseSchema(schema sch);
extern void OptimisticTransaction();
extern void BeginTransaction(void);
extern bool EndTransaction(void);
extern void DelRecord(dbtable tab);
extern schema SwitchToSchema(schema sch);
extern FILE *OpenLog(char *file, char *mode);
extern int RollBack(void);
extern void SetLogStatus(int status);
extern void FlushTable(dbtable tab);
extern void SetRelation(dbtable tab, int depth, int errflg);
extern int GetRelDepth(dbtable tab);
extern int GetRelErrFlag(dbtable tab);
extern itype CalcItype(type ty, int len);
extern int PopUpDbMenu(int height, int width, char *label, dbcursor c, int nparts, IFP func, FPCP dspFunc);
extern const char *GetSchemas();
extern const char *GetTables(schema sc);
extern const char *GetIndices(dbtable t);
extern const char *GetTabFlds(dbtable tab);
extern const char *GetIndFlds(dbindex ind);
extern const char *GetFldDesc(dbfield fld);
extern const char *GetFldName(dbfield fld);
extern type GetFldType(dbfield fld);
extern int GetFldLen(dbfield fld);
extern int GetFldNDec(dbfield fld);
extern const char *GetTabInds(dbtable tab);
extern const char *GetIndName(dbindex ind);
extern const char *GetSchDescr(schema sch);
extern const char *GetTabDescr(dbtable tab);
extern void AddRecordTS(dbtable tab, TIMESTAMP crts, TIMESTAMP modts);
extern TIMESTAMP MTimeStamp(dbtable tab);
extern TIMESTAMP CTimeStamp(dbtable tab);
extern TIMESTAMP StrToTS(char *s);
extern void TSToStr(TIMESTAMP ts, char *s);
extern char *DbIdeaVersion();
extern void DbBanner(char const *);
extern TIMESTAMP TableTimeStamp(dbtable tab);
extern int SetTableCache(dbtable tbl, long size);

extern TIMESTAMP GetDownLoadTS(dbtable tab);
extern void SetDownLoadTS(dbtable tab, TIMESTAMP ts);

extern int SetInCheck(dbfield fld, STRUCT InCheck *ic);
extern int SetDeflt(dbfield fld, STRUCT DbExpr *deflt);
extern int SetRelCheck(dbfield fld, STRUCT DbExpr *rchk);
// This prototype below was added for compatibility with version 432.
extern int SetTabRelCheck(dbtable tab, STRUCT DbExpr *expr);
extern int SetCollateSet(dbfield fld, const char *cs);
extern int SetNewNullValueForField(const char *tabName, dbfield fld, const char *nnv);
extern bool EndSyncTransaction(void);
extern void BeginTrans(int flags);
extern bool TransOk(void);
extern void SaneTrans(void);
extern bool InTransaction(void);
extern bool EndTrans(bool syn, bool rttrans);
extern int dbmsType(void);
extern int dbmsTypeSc(schema sc);

extern int GetTextToFile(dbfield fld, char *path);
extern int GetTextToMem(dbfield fld, void *buffer, long size);
extern int StoreTextFromFile(dbfield fld, char *path);
extern int StoreTextFromMem(dbfield fld, void *buffer, long size);
extern int DiscardText(dbfield fld);
extern int GetBinaryToFile(dbfield fld, char *path);
extern int GetBinaryToMem(dbfield fld, void *buffer, long size);
extern int StoreBinaryFromFile(dbfield fld, char *path);
extern int StoreBinaryFromMem(dbfield fld, void *buffer, long size);
extern int DiscardBinary(dbfield fld);
extern int GetBinaryInfo(dbfield f, BinaryInfo *binaryInfo);
extern BinaryType IdentifyMagic(void *buffer, long size);
extern BinaryType IdentifyBinaryType(char *path);
extern const char *BinaryTypeDesc(BinaryType bintype);

extern const char *NMethods(const char *server);
#ifdef __cplusplus
extern int ShowDbImage(dbfield, char *,int x=-1, int y=-1);
#else
extern int ShowDbImage(dbfield, char *,int x, int y);
#endif

#ifdef __cplusplus
}
#endif

/* -- Prototypes -- */

#endif
