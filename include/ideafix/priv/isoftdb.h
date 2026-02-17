/********************************************************************
*
* Copyright (c) 1999 InterSoft Co.  All Rights Reserved
* THIS IS UNPUBLISHED PROPRIETARY SOURCE CODE OF InterSoft Co.
* The copyright notice above does not evidence any
* actual or intended publication of such source code.
*
* $Id: isoftdb.h,v 1.5 2002/02/01 18:39:49 albertoi Exp $
* DESCRIPTION:
*
********************************************************************/
#ifndef ISOFTDB_H
#define ISOFTDB_H

class IsoftDb {

public:
	IsoftDb() {};
	virtual ~IsoftDb() {};
	

virtual schema	OpenSchema(char *name, UShort mode) = 0;
virtual struct s_table *	_AttachTable(struct s_table *tp) = 0;
virtual int	GetRecord(dbindex ind,find_mode fmode,int mode, va_list ap) = 0;
virtual int	AddIndField(dbindex ind, dbfield fld, k_field_flags flags, UShort n, UShort m)	= 0;
virtual void	AddRecord(dbtable tab)	= 0;
virtual void	AddRecordTS(dbtable tab, TIMESTAMP crts, TIMESTAMP modts) = 0;
virtual long	AddRecordTest(dbtable tab) = 0;
virtual dbfield	AddTabField(dbtable tbl, char *fname, char *descr, UShort dim, type ty, sqltype sql_ty, UShort len, UChar ndec, field_flags flags, char *def, char *mask) = 0;
virtual void	BeginTrans(int mode) = 0;
virtual long	BuildIndex(dbindex ind, int flags, va_list ap) = 0;
virtual TIMESTAMP	CTimeStamp(dbtable tab) = 0;
virtual int	ChownSchema(schema sch, long uid) = 0;
virtual void	CloseAllSchemas(void) = 0;
virtual void	CloseSchema(schema sch) = 0;
virtual int	CompleteIndex(dbindex ind) = 0;
virtual int	ConvertTable(dbtable tab, dbtable newtab, bool force, bool drop, IFCPVA err_func, FCPVA msg_func, unsigned int, unsigned int, bool correctRefs) = 0;
virtual void	CopyTable(dbtable newtab, dbtable tab, dbfield *, int flag) = 0;
virtual void	CorruptTrans(void) = 0;
virtual long	CountCursor(dbcursor cursor) = 0;
virtual dbcursor	CreateCursor(dbindex ind, int mode) = 0;
virtual dbindex	CreateIndex(dbtable tbl, char *name, key_flags flags, UChar) = 0;
virtual schema	CreateSchema(char *name, char *descr) = 0;
virtual dbtable	CreateTable(schema sch, char *name, char *descr, tab_flags flags, ULong size, UChar n_flds) = 0;
virtual bool	DbCheckPoint(schema) = 0;
virtual bool	DbFlushReq(schema) = 0;
virtual int	DelAllRecords(dbtable tab) = 0;
virtual void	DelRecord(dbtable tab) = 0;
virtual void	DeleteCursor(dbcursor cursor) = 0;
virtual int	DeleteTable(dbtable tab) = 0;
virtual void	DiscardRecord(dbtable tbl) = 0;
virtual int	DropIndex(dbindex ind) = 0;
virtual int	DropSchema(schema sch) = 0;
virtual int	DropTable(dbtable tab) = 0;
virtual bool	EndTrans(bool syn, bool rttrans) = 0;
virtual bool	EndTransaction(void) = 0;
virtual long	FindRecord(dbindex ind, find_mode fmode, va_list ap) = 0;
virtual schema	FindSchemaVersion(char *name, long version) = 0;
virtual void	FlushTable(dbtable tab) = 0;
virtual void	FreeRecord(dbindex ind, find_mode fmode, va_list ap) = 0;
virtual void	FreeSchema(schema sch) = 0;
virtual void	FreeTable(dbtable tab) = 0;
virtual TIMESTAMP	GetDownLoadTS(dbtable tab) = 0;
virtual int	GetTransMode(void) = 0;
virtual bool	InTransaction() = 0;
virtual void	InitRecord(dbtable tab) = 0;
virtual int	LockSchema(schema sch, int lock_mode) = 0;
virtual int	LockTable(dbtable tab, int lock_mode) = 0;
virtual TIMESTAMP	MTimeStamp(dbtable tab) = 0;
virtual schema	OpenSchemaVersion(char *name, UShort mode, long version) = 0;
virtual void	OptimisticTransaction() = 0;
virtual long	PopRecord(dbtable tbl) = 0;
virtual void	PushRecord(dbtable tbl) = 0;
virtual void	PutRecord(dbtable tab) = 0;
virtual bool	RenameField(dbfield fld, char *name) = 0;
virtual int	RenameTable(dbtable tab, char *name) = 0;
virtual int	RollBack(void) = 0;
virtual void	SaneTrans(void) = 0;
virtual long	SchemaVersion(schema sch) = 0;
virtual int	SetCollateSet(dbfield fld, const char *cs) = 0;
virtual int	SetDataCache(dbtable tab, UShort n, int mode) = 0;
virtual int	SetDeflt(dbfield fld, struct DbExpr *expr) = 0;
virtual void	SetDescrSchema(schema sch, char *descr) = 0;
virtual void	SetDownLoadTS(dbtable tab, TIMESTAMP ts) = 0;
virtual int	SetInCheck(dbfield fld, struct InCheck *inchk) = 0;
virtual void	SetIndexCache(dbindex ind, UShort n) = 0;
virtual int	SetRelCheck(dbfield fld, struct DbExpr *expr) = 0;
virtual int	SetTabRelCheck(dbtable tab, struct DbExpr *expr) = 0;
virtual int	SetTableCache(dbtable , long ) = 0;
virtual schema	SwitchToSchema(schema sch) = 0;
virtual long	TabActualSize(dbtable tab) = 0;
virtual long	TabNRecords(dbtable tab) = 0;
virtual TIMESTAMP	TableTimeStamp(dbtable tab) = 0;
virtual bool	TransOk(void) = 0;
//virtual void	UpdateRecord(dbtable tab, va_list ap) = 0;
virtual int     UpdateRecord(dbtable tab) = 0;   /*JC: BZ 3368 */

virtual long	VersionNumber(char *name, DATE d) = 0;
virtual long	_AddKey(struct s_index *ip, long recno, char *rec_buff, struct tab_array *, dbindex ind) = 0;
virtual long	_FindKey(struct tab_array *tap, struct s_index *ip, char *rec_buff, find_mode fmode, UShort nparts, char **,int) = 0;
virtual bool	_FlushSchPrivileges(struct s_schema *sp) = 0;
virtual bool	_FlushTabPrivileges(struct s_schema *sp) = 0;
virtual void	_FlushTable(struct tab_array *tap, int ind) = 0;
virtual void	_FreeCursorCache(struct s_dbcur *cp) = 0;
virtual long	_GetNextBlock(struct s_dbcur *cp) = 0;
virtual long	_GetPrevBlock(struct s_dbcur *cp) = 0;
virtual void	_ModifySchema(struct s_schema *sp) = 0;
virtual dbtable	_ReCreateTable(schema sch, char *name, char *descr, tab_flags flags, ULong size, UChar n_flds) = 0;
virtual void	_ReOpenSchema(schema sch) = 0;
virtual void	_SendRangeCursor(struct s_dbcur *cp) = 0;
virtual struct s_schema *	_GetSchPtr(schema sch) = 0;
virtual int	_TabSeqNumber(dbtable tbl) = 0;
virtual int	dbmsType(void) = 0;
virtual char*	_StrValPutOrAlloc(struct s_schema *schptr,char *s,char *prev) = 0;
virtual char*	_MemValPutOrAlloc(struct s_schema *schptr,void *s,UShort len,void *prev) = 0;

virtual sqlcursor CreateSQLCursor(schema sch, const char *sentence, int paramsCount) = 0;

virtual int  FetchSQLNext(sqlcursor cur) = 0;
virtual void  AddSQLCursorParameter(sqlcursor cur, type ty, UShort len, UShort ndec) = 0;
virtual int  SetSQLCursorParameters(sqlcursor cur, va_list av) = 0;
virtual int  SetSQLCursorParameter(sqlcursor cur, int fldIndex, char *value) = 0;
virtual void RestartSQLCursor(sqlcursor cur) = 0;
virtual int  GetSQLIFld(sqlcursor cur, UShort fldIndex) = 0;
virtual long  GetSQLLFld(sqlcursor cur, UShort fldIndex) = 0;
virtual char *GetSQLSFld(sqlcursor cur, UShort fldIndex) = 0;
virtual double  GetSQLFFld(sqlcursor cur, UShort fldIndex) = 0;
virtual DATE  GetSQLDFld(sqlcursor cur, UShort fldIndex) = 0;
virtual TIME  GetSQLTFld(sqlcursor cur, UShort fldIndex) = 0;
virtual NUM  GetSQLNFld(sqlcursor cur, UShort fldIndex) = 0;
virtual void DeleteSQLCursor(sqlcursor cur) = 0;
virtual void AddSQLFieldTarget(sqlcursor cur, UShort fldIndex, dbfield fld, int column = -1) = 0;
virtual int  FetchSQLLevel(sqlcursor cur,int level) = 0;
virtual void SetSQLLevel(sqlcursor cur, int level, dbtable tab) = 0;

virtual int  ExecuteSQL(sqlcursor cur) = 0;
};

#endif
