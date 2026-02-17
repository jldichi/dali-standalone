/********************************************************************
*
* Copyright (c) 1999 InterSoft Co.  All Rights Reserved
* THIS IS UNPUBLISHED PROPRIETARY SOURCE CODE OF InterSoft Co.
* The copyright notice above does not evidence any
* actual or intended publication of such source code.
*
* $Id: isoftdbsql.h,v 1.6 2002/05/08 20:13:03 hardaiz Exp $
* DESCRIPTION:
*
********************************************************************/
#ifndef ISOFTDBSQL_H
#define ISOFTDBSQL_H
//#ifdef __VC__
//#define inline
//#endif

class IsoftDbSql : public IsoftDb {

public:
	IsoftDbSql() {};
	virtual ~IsoftDbSql() {};
	

virtual inline schema OpenSchema(char *name, UShort mode);
virtual inline struct s_table *	_AttachTable(struct s_table *tp) ;
virtual inline int	GetRecord(dbindex ind,find_mode fmode,int mode, va_list ap);
virtual inline int	AddIndField(dbindex ind, dbfield fld, k_field_flags flags, UShort n, UShort m)	;
virtual inline void	AddRecord(dbtable tab)	;
virtual inline void	AddRecordTS(dbtable tab, TIMESTAMP crts, TIMESTAMP modts);
virtual inline long	AddRecordTest(dbtable tab);
virtual inline dbfield	AddTabField(dbtable tbl, char *fname, char *descr, UShort dim, type ty, sqltype sql_ty, UShort len, UChar ndec, field_flags flags, char *def, char *mask);
virtual inline void	BeginTrans(int mode);
virtual inline long	BuildIndex(dbindex ind, int flags, va_list ap);
virtual inline TIMESTAMP	CTimeStamp(dbtable tab);
virtual inline int	ChownSchema(schema sch, long uid);
virtual inline void	CloseAllSchemas(void);
virtual inline void	CloseSchema(schema sch);
virtual inline int	CompleteIndex(dbindex ind);
virtual inline int	ConvertTable(dbtable tab, dbtable newtab, bool force, bool drop, IFCPVA err_func, FCPVA msg_func, unsigned int, unsigned int, bool correctRefs);
virtual inline void	CopyTable(dbtable newtab, dbtable tab, dbfield *, int flag);
virtual inline void	CorruptTrans(void);
virtual inline long	CountCursor(dbcursor cursor);
virtual inline dbcursor	CreateCursor(dbindex ind, int mode);
virtual inline dbindex	CreateIndex(dbtable tbl, char *name, key_flags flags, UChar);
virtual inline schema	CreateSchema(char *name, char *descr);
virtual inline dbtable	CreateTable(schema sch, char *name, char *descr, tab_flags flags, ULong size, UChar n_flds);
virtual inline bool	DbCheckPoint(schema);
virtual inline bool	DbFlushReq(schema);
virtual inline int	DelAllRecords(dbtable tab);
virtual inline void	DelRecord(dbtable tab);
virtual inline void	DeleteCursor(dbcursor cursor);
virtual inline int	DeleteTable(dbtable tab);
virtual inline void	DiscardRecord(dbtable tbl);
virtual inline int	DropIndex(dbindex ind);
virtual inline int	DropSchema(schema sch);
virtual inline int	DropTable(dbtable tab);
virtual inline bool	EndTrans(bool syn, bool rttrans);
virtual inline bool	EndTransaction(void);
virtual inline long	FindRecord(dbindex ind, find_mode fmode, va_list ap);
virtual inline schema	FindSchemaVersion(char *name, long version);
virtual inline void	FlushTable(dbtable tab);
virtual inline void	FreeRecord(dbindex ind, find_mode fmode, va_list ap);
virtual inline void	FreeSchema(schema sch);
virtual inline void	FreeTable(dbtable tab);
virtual inline TIMESTAMP	GetDownLoadTS(dbtable tab);
virtual inline int	GetTransMode(void);
virtual inline bool	InTransaction();
virtual inline void	InitRecord(dbtable tab);
virtual inline int	LockSchema(schema sch, int lock_mode);
virtual inline int	LockTable(dbtable tab, int lock_mode);
virtual inline TIMESTAMP	MTimeStamp(dbtable tab);
virtual inline schema	OpenSchemaVersion(char *name, UShort mode, long version);
virtual inline void	OptimisticTransaction();
virtual inline long	PopRecord(dbtable tbl);
virtual inline void	PushRecord(dbtable tbl);
virtual inline void	PutRecord(dbtable tab);
virtual inline bool	RenameField(dbfield fld, char *name);
virtual inline int	RenameTable(dbtable tab, char *name);
virtual inline int	RollBack(void);
virtual inline void	SaneTrans(void);
virtual inline long	SchemaVersion(schema sch);
virtual inline int	SetCollateSet(dbfield fld, const char *cs);
virtual inline int	SetDataCache(dbtable tab, UShort n, int mode);
virtual inline int	SetDeflt(dbfield fld, struct DbExpr *expr);
virtual inline void	SetDescrSchema(schema sch, char *descr);
virtual inline void	SetDownLoadTS(dbtable tab, TIMESTAMP ts);
virtual inline int	SetInCheck(dbfield fld, struct InCheck *inchk);
virtual inline void	SetIndexCache(dbindex ind, UShort n);
virtual inline int	SetRelCheck(dbfield fld, struct DbExpr *expr);
virtual inline int	SetTabRelCheck(dbtable tab, struct DbExpr *expr);
virtual inline int	SetTableCache(dbtable , long );
virtual inline schema	SwitchToSchema(schema sch);
virtual inline long	TabActualSize(dbtable tab);
virtual inline long	TabNRecords(dbtable tab);
virtual inline TIMESTAMP	TableTimeStamp(dbtable tab);
virtual inline bool	TransOk(void);
//virtual inline void	UpdateRecord(dbtable tab, va_list ap);
virtual inline int UpdateRecord(dbtable tab); /*JC: BZ 3368 */

virtual inline long	VersionNumber(char *name, DATE d);
virtual inline long	_AddKey(struct s_index *ip, long recno, char *rec_buff, struct tab_array *, dbindex ind);
virtual inline long	_FindKey(struct tab_array *tap, struct s_index *ip, char *rec_buff, find_mode fmode, UShort nparts, char **,int);
virtual inline bool	_FlushSchPrivileges(struct s_schema *sp);
virtual inline bool	_FlushTabPrivileges(struct s_schema *sp);
virtual inline void	_FlushTable(struct tab_array *tap, int ind);
virtual inline void	_FreeCursorCache(struct s_dbcur *cp);
virtual inline long	_GetNextBlock(struct s_dbcur *cp);
virtual inline long	_GetPrevBlock(struct s_dbcur *cp);
virtual inline void	_ModifySchema(struct s_schema *sp);
virtual inline dbtable	_ReCreateTable(schema sch, char *name, char *descr, tab_flags flags, ULong size, UChar n_flds);
virtual inline void	_ReOpenSchema(schema sch);
virtual inline void	_SendRangeCursor(struct s_dbcur *cp);
virtual inline struct s_schema *	_GetSchPtr(schema sch) ;
virtual inline int	_TabSeqNumber(dbtable tbl);
virtual inline int	dbmsType(void);
virtual inline char*	_StrValPutOrAlloc(struct s_schema *schptr,char *s,char *prev);
virtual inline char*	_MemValPutOrAlloc(struct s_schema *schptr,void *s,UShort len,void *prev);

virtual inline void TurnSQLFilterOn(void);
virtual inline void TurnSQLFilterOff(void);

virtual inline sqlcursor CreateSQLCursor(schema sch, const char *sentence, int paramsCount);


virtual inline int  FetchSQLNext(sqlcursor cur);
virtual inline void  AddSQLCursorParameter(sqlcursor cur, type ty, UShort len, UShort ndec);
virtual inline int  SetSQLCursorParameters(sqlcursor cur, va_list av);
virtual inline int  SetSQLCursorParameter(sqlcursor cur, int fldIndex, char *value);
virtual inline void RestartSQLCursor(sqlcursor cur);
virtual inline int  GetSQLIFld(sqlcursor cur, UShort fldIndex);
virtual inline long  GetSQLLFld(sqlcursor cur, UShort fldIndex);
virtual inline char *GetSQLSFld(sqlcursor cur, UShort fldIndex);
virtual inline double  GetSQLFFld(sqlcursor cur, UShort fldIndex);
virtual inline DATE  GetSQLDFld(sqlcursor cur, UShort fldIndex);
virtual inline TIME  GetSQLTFld(sqlcursor cur, UShort fldIndex);
virtual inline NUM  GetSQLNFld(sqlcursor cur, UShort fldIndex);
virtual inline void DeleteSQLCursor(sqlcursor cur);
virtual inline void AddSQLFieldTarget(sqlcursor cur, UShort fldIndex, dbfield fld, int column = -1);
virtual inline int  FetchSQLLevel(sqlcursor cur,int level);
virtual inline void SetSQLLevel(sqlcursor cur, int level, dbtable tab);

virtual inline int ExecuteSQL(sqlcursor cur);
};

#include <ideafix/priv/isoftdbsql.icc>
#endif
