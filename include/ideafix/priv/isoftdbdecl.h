/********************************************************************
*
* Copyright (c) 1999 InterSoft Co.  All Rights Reserved
* THIS IS UNPUBLISHED PROPRIETARY SOURCE CODE OF InterSoft Co.
* The copyright notice above does not evidence any
* actual or intended publication of such source code.
*
* $Id: isoftdbdecl.h,v 1.9 2006/02/14 20:57:11 hardaiz Exp $
* DESCRIPTION:
*
********************************************************************/
#ifndef ISOFTDBDECL_H
#define ISOFTDBDECL_H

/*-------------------------------------------------------------------*/

extern "C" void isoftsql_execSetDirty(dbfield );
extern "C" void isoftsql_execTrueInitCursor(dbcursor);
extern "C" schema isoftsql_OpenSchema(char *name, UShort mode);
extern "C" struct s_table * isoftsql__AttachTable(struct s_table *tp);
extern "C" long	isoftsql_GetRecord(dbindex ind,find_mode fmode,int mode, va_list ap);
extern "C" int isoftsql_AddIndField(dbindex ind, dbfield fld, k_field_flags flags, UShort n, UShort m);
extern "C" void isoftsql_AddRecord(dbtable tab);
extern "C" void isoftsql_AddRecordTS(dbtable tab, TIMESTAMP crts, TIMESTAMP modts);
extern "C" long isoftsql_AddRecordTest(dbtable tab);
extern "C" dbfield isoftsql_AddTabField(dbtable tbl, char *fname, char *descr, UShort dim, type ty, sqltype sql_ty, UShort len, UChar ndec, field_flags flags, char *def, char *mask);
extern "C" void isoftsql_BeginTrans(int mode);
extern "C" long isoftsql_BuildIndex(dbindex ind, int flags, va_list ap);
extern "C" TIMESTAMP isoftsql_CTimeStamp(dbtable tab);
extern "C" int isoftsql_ChownSchema(schema sch, long uid);
extern "C" void isoftsql_CloseAllSchemas(void);
extern "C" void isoftsql_CloseSchema(schema sch);
extern "C" int isoftsql_CompleteIndex(dbindex ind);
extern "C" int isoftsql_ConvertTable(dbtable tab, dbtable newtab, bool force, bool drop, IFCPVA err_func, FCPVA msg_func, unsigned int, unsigned int, bool correctRefs);
extern "C" void isoftsql_CopyTable(dbtable newtab, dbtable tab, dbfield *, int flag);
extern "C" void isoftsql_CorruptTrans(void);
extern "C" long isoftsql_CountCursor(dbcursor cursor);
extern "C" dbcursor isoftsql_CreateCursor(dbindex ind, int mode);
extern "C" dbindex isoftsql_CreateIndex(dbtable tbl, char *name, key_flags flags, UChar);
extern "C" schema isoftsql_CreateSchema(char *name, char *descr);
extern "C" dbtable isoftsql_CreateTable(schema sch, char *name, char *descr, tab_flags flags, ULong size, UChar n_flds);
extern "C" bool isoftsql_DbCheckPoint(schema);
extern "C" bool isoftsql_DbFlushReq(schema);
extern "C" int isoftsql_DelAllRecords(dbtable tab);
extern "C" void isoftsql_DelRecord(dbtable tab);
extern "C" void isoftsql_DeleteCursor(dbcursor cursor);
extern "C" int isoftsql_DeleteTable(dbtable tab);
extern "C" void isoftsql_DiscardRecord(dbtable tbl);
extern "C" int isoftsql_DropIndex(dbindex ind);
extern "C" int isoftsql_DropSchema(schema sch);
extern "C" int isoftsql_DropTable(dbtable tab);
extern "C" bool isoftsql_EndTrans(bool syn, bool rttrans);
extern "C" bool isoftsql_EndTransaction(void);
extern "C" long isoftsql_FindRecord(dbindex ind, find_mode fmode, va_list ap);
extern "C" schema isoftsql_FindSchemaVersion(char *name, long version);
extern "C" void isoftsql_FlushTable(dbtable tab);
extern "C" void isoftsql_FreeRecord(dbindex ind, find_mode fmode, va_list ap);
extern "C" void isoftsql_FreeSchema(schema sch);
extern "C" void isoftsql_FreeTable(dbtable tab);
extern "C" TIMESTAMP isoftsql_GetDownLoadTS(dbtable tab);
extern "C" int isoftsql_GetTransMode(void);
extern "C" bool isoftsql_InTransaction();
extern "C" void isoftsql_InitRecord(dbtable tab);
extern "C" int isoftsql_LockSchema(schema sch, int lock_mode);
extern "C" int isoftsql_LockTable(dbtable tab, int lock_mode);
extern "C" TIMESTAMP isoftsql_MTimeStamp(dbtable tab);
extern "C" schema isoftsql_OpenSchemaVersion(char *name, UShort mode, long version);
extern "C" void isoftsql_OptimisticTransaction();
extern "C" long isoftsql_PopRecord(dbtable tbl);
extern "C" void isoftsql_PushRecord(dbtable tbl);
extern "C" void isoftsql_PutRecord(dbtable tab);
extern "C" bool isoftsql_RenameField(dbfield fld, char *name);
extern "C" int isoftsql_RenameTable(dbtable tab, char *name);
extern "C" int isoftsql_RollBack(void);
extern "C" void isoftsql_SaneTrans(void);
extern "C" long isoftsql_SchemaVersion(schema sch);
extern "C" int isoftsql_SetCollateSet(dbfield fld, const char *cs);
extern "C" int isoftsql_SetDataCache(dbtable tab, UShort n, int mode);
extern "C" int isoftsql_SetDeflt(dbfield fld, struct DbExpr *expr);
extern "C" void isoftsql_SetDescrSchema(schema sch, char *descr);
extern "C" void isoftsql_SetDownLoadTS(dbtable tab, TIMESTAMP ts);
extern "C" int isoftsql_SetInCheck(dbfield fld, struct InCheck *inchk);
extern "C" void isoftsql_SetIndexCache(dbindex ind, UShort n);
extern "C" int isoftsql_SetRelCheck(dbfield fld, struct DbExpr *expr);
extern "C" int isoftsql_SetTabRelCheck(dbtable tab, struct DbExpr *expr);
extern "C" int isoftsql_SetTableCache(dbtable , long );
extern "C" schema isoftsql_SwitchToSchema(schema sch);
extern "C" long isoftsql_TabActualSize(dbtable tab);
extern "C" long isoftsql_TabNRecords(dbtable tab);
extern "C" TIMESTAMP isoftsql_TableTimeStamp(dbtable tab);
extern "C" bool isoftsql_TransOk(void);
//extern "C" void isoftsql_UpdateRecord(dbtable tab, va_list ap);
extern "C" int isoftsql_UpdateRecord(dbtable tab);  /*JC: BZ 3368*/

extern "C" long isoftsql_VersionNumber(char *name, DATE d);
extern "C" long isoftsql__AddKey(struct s_index *ip, long recno, char *rec_buff, struct tab_array *, dbindex ind);
extern "C" long isoftsql__FindKey(struct tab_array *tap, struct s_index *ip, char *rec_buff, find_mode fmode, UShort nparts, char **,int);
extern "C" bool isoftsql__FlushSchPrivileges(struct s_schema *sp);
extern "C" bool isoftsql__FlushTabPrivileges(struct s_schema *sp);
extern "C" void isoftsql__FlushTable(struct tab_array *tap, int ind);
extern "C" void isoftsql__FreeCursorCache(struct s_dbcur *cp);
extern "C" long isoftsql__GetNextBlock(struct s_dbcur *cp);
extern "C" long isoftsql__GetPrevBlock(struct s_dbcur *cp);
extern "C" void isoftsql__ModifySchema(struct s_schema *sp);
extern "C" dbtable isoftsql__ReCreateTable(schema sch, char *name, char *descr, tab_flags flags, ULong size, UChar n_flds);
extern "C" void isoftsql__ReOpenSchema(schema sch);
extern "C" void isoftsql__SendRangeCursor(struct s_dbcur *cp);
extern "C" s_schema *	isoftsql__GetSchPtr(schema sch) ;
extern "C" int isoftsql__TabSeqNumber(dbtable tbl);
extern "C" int isoftsql_dbmsType(void);
extern "C" char* isoftsql__StrValPutOrAlloc(struct s_schema *schptr,char *s,char *prev);
extern "C" char* isoftsql__MemValPutOrAlloc(struct s_schema *schptr,void *s,UShort len,void *prev);
extern "C" const char *isoftsql_GetSchemas();
extern "C" void isoftsql_execMoveCursorFirst(dbcursor cursor);
extern "C" void isoftsql_execMoveCursorLast(dbcursor cursor);
extern "C" void isoftsql_execSetCursor(dbcursor cursor);
/*-------------------------------------------------------------------*/
extern "C" schema isoftes_OpenSchema(char *name, UShort mode);
extern "C" struct s_table * isoftes__AttachTable(struct s_table *tp);
extern "C" long	isoftes_GetRecord(dbindex ind,find_mode fmode,int mode, va_list ap);
extern "C" int isoftes_AddIndField(dbindex ind, dbfield fld, k_field_flags flags, UShort n, UShort m);
extern "C" void isoftes_AddRecord(dbtable tab);
extern "C" void isoftes_AddRecordTS(dbtable tab, TIMESTAMP crts, TIMESTAMP modts);
extern "C" long isoftes_AddRecordTest(dbtable tab);
extern "C" dbfield isoftes_AddTabField(dbtable tbl, char *fname, char *descr, UShort dim, type ty, sqltype sql_ty, UShort len, UChar ndec, field_flags flags, char *def, char *mask);
extern "C" void isoftes_BeginTrans(int mode);
extern "C" long isoftes_BuildIndex(dbindex ind, int flags, va_list ap);
extern "C" TIMESTAMP isoftes_CTimeStamp(dbtable tab);
extern "C" int isoftes_ChownSchema(schema sch, long uid);
extern "C" void isoftes_CloseAllSchemas(void);
extern "C" void isoftes_CloseSchema(schema sch);
extern "C" int isoftes_CompleteIndex(dbindex ind);
extern "C" int isoftes_ConvertTable(dbtable tab, dbtable newtab, bool force, bool drop, IFCPVA err_func, FCPVA msg_func, unsigned int, unsigned int, bool correctRefs);
extern "C" void isoftes_CopyTable(dbtable newtab, dbtable tab, dbfield *, int flag);
extern "C" void isoftes_CorruptTrans(void);
extern "C" long isoftes_CountCursor(dbcursor cursor);
extern "C" dbcursor isoftes_CreateCursor(dbindex ind, int mode);
extern "C" dbindex isoftes_CreateIndex(dbtable tbl, char *name, key_flags flags, UChar);
extern "C" schema isoftes_CreateSchema(char *name, char *descr);
extern "C" dbtable isoftes_CreateTable(schema sch, char *name, char *descr, tab_flags flags, ULong size, UChar n_flds);
extern "C" bool isoftes_DbCheckPoint(schema);
extern "C" bool isoftes_DbFlushReq(schema);
extern "C" int isoftes_DelAllRecords(dbtable tab);
extern "C" void isoftes_DelRecord(dbtable tab);
extern "C" void isoftes_DeleteCursor(dbcursor cursor);
extern "C" int isoftes_DeleteTable(dbtable tab);
extern "C" void isoftes_DiscardRecord(dbtable tbl);
extern "C" int isoftes_DropIndex(dbindex ind);
extern "C" int isoftes_DropSchema(schema sch);
extern "C" int isoftes_DropTable(dbtable tab);
extern "C" bool isoftes_EndTrans(bool syn, bool rttrans);
extern "C" bool isoftes_EndTransaction(void);
extern "C" long isoftes_FindRecord(dbindex ind, find_mode fmode, va_list ap);
extern "C" schema isoftes_FindSchemaVersion(char *name, long version);
extern "C" void isoftes_FlushTable(dbtable tab);
extern "C" void isoftes_FreeRecord(dbindex ind, find_mode fmode, va_list ap);
extern "C" void isoftes_FreeSchema(schema sch);
extern "C" void isoftes_FreeTable(dbtable tab);
extern "C" TIMESTAMP isoftes_GetDownLoadTS(dbtable tab);
extern "C" int isoftes_GetTransMode(void);
extern "C" bool isoftes_InTransaction();
extern "C" void isoftes_InitRecord(dbtable tab);
extern "C" int isoftes_LockSchema(schema sch, int lock_mode);
extern "C" int isoftes_LockTable(dbtable tab, int lock_mode);
extern "C" TIMESTAMP isoftes_MTimeStamp(dbtable tab);
extern "C" schema isoftes_OpenSchemaVersion(char *name, UShort mode, long version);
extern "C" void isoftes_OptimisticTransaction();
extern "C" long isoftes_PopRecord(dbtable tbl);
extern "C" void isoftes_PushRecord(dbtable tbl);
extern "C" void isoftes_PutRecord(dbtable tab);
extern "C" bool isoftes_RenameField(dbfield fld, char *name);
extern "C" int isoftes_RenameTable(dbtable tab, char *name);
extern "C" int isoftes_RollBack(void);
extern "C" void isoftes_SaneTrans(void);
extern "C" long isoftes_SchemaVersion(schema sch);
extern "C" int isoftes_SetCollateSet(dbfield fld, const char *cs);
extern "C" int isoftes_SetDataCache(dbtable tab, UShort n, int mode);
extern "C" int isoftes_SetDeflt(dbfield fld, struct DbExpr *expr);
extern "C" void isoftes_SetDescrSchema(schema sch, char *descr);
extern "C" void isoftes_SetDownLoadTS(dbtable tab, TIMESTAMP ts);
extern "C" int isoftes_SetInCheck(dbfield fld, struct InCheck *inchk);
extern "C" void isoftes_SetIndexCache(dbindex ind, UShort n);
extern "C" int isoftes_SetRelCheck(dbfield fld, struct DbExpr *expr);
extern "C" int isoftes_SetTabRelCheck(dbtable tab, struct DbExpr *expr);
extern "C" int isoftes_SetTableCache(dbtable , long );
extern "C" schema isoftes_SwitchToSchema(schema sch);
extern "C" long isoftes_TabActualSize(dbtable tab);
extern "C" long isoftes_TabNRecords(dbtable tab);
extern "C" TIMESTAMP isoftes_TableTimeStamp(dbtable tab);
extern "C" bool isoftes_TransOk(void);
extern "C" void isoftes_UpdateRecord(dbtable tab, va_list ap);
extern "C" long isoftes_VersionNumber(char *name, DATE d);
extern "C" long isoftes__AddKey(struct s_index *ip, long recno, char *rec_buff, struct tab_array *, dbindex ind);
extern "C" long isoftes__FindKey(struct tab_array *tap, struct s_index *ip, char *rec_buff, find_mode fmode, UShort nparts, char **,int);
extern "C" bool isoftes__FlushSchPrivileges(struct s_schema *sp);
extern "C" bool isoftes__FlushTabPrivileges(struct s_schema *sp);
extern "C" void isoftes__FlushTable(struct tab_array *tap, int ind);
extern "C" void isoftes__FreeCursorCache(struct s_dbcur *cp);
extern "C" long isoftes__GetNextBlock(struct s_dbcur *cp);
extern "C" long isoftes__GetPrevBlock(struct s_dbcur *cp);
extern "C" void isoftes__ModifySchema(struct s_schema *sp);
extern "C" dbtable isoftes__ReCreateTable(schema sch, char *name, char *descr, tab_flags flags, ULong size, UChar n_flds);
extern "C" void isoftes__ReOpenSchema(schema sch);
extern "C" void isoftes__SendRangeCursor(struct s_dbcur *cp);
extern "C" s_schema *	isoftes__GetSchPtr(schema sch) ;
extern "C" int isoftes__TabSeqNumber(dbtable tbl);
extern "C" int isoftes_dbmsType(void);
extern "C" char* isoftes__StrValPutOrAlloc(struct s_schema *schptr,char *s,char *prev);
extern "C" char* isoftes__MemValPutOrAlloc(struct s_schema *schptr,void *s,UShort len,void *prev);
extern "C" const char *isoftes_GetSchemas();

// SQL Essentia

extern "C" sqlcursor  isoftes__CreateSQLCursor(schema sch, const char *sentence, int paramsCount);

extern "C" int        isoftes__FetchSQLNext(sqlcursor cur);
extern "C" void       isoftes__AddSQLCursorParameter(sqlcursor cur, type ty, UShort len, UShort ndec);
extern "C" int        isoftes__SetSQLCursorParameters(sqlcursor cur, va_list av);
extern "C" int        isoftes__SetSQLCursorParameter(sqlcursor cur, int fldIndex, char *value);
extern "C" void       isoftes__RestartSQLCursor(sqlcursor cur);
extern "C" int        isoftes__GetSQLIFld(sqlcursor cur, UShort fldIndex);
extern "C" long       isoftes__GetSQLLFld(sqlcursor cur, UShort fldIndex);
extern "C" char*      isoftes__GetSQLSFld(sqlcursor cur, UShort fldIndex);
extern "C" double     isoftes__GetSQLFFld(sqlcursor cur, UShort fldIndex);
extern "C" DATE       isoftes__GetSQLDFld(sqlcursor cur, UShort fldIndex);
extern "C" TIME       isoftes__GetSQLTFld(sqlcursor cur, UShort fldIndex);
extern "C" NUM        isoftes__GetSQLNFld(sqlcursor cur, UShort fldIndex);
extern "C" void       isoftes__DeleteSQLCursor(sqlcursor cur);
extern "C" void       isoftes__AddSQLFieldTarget(sqlcursor cur, UShort fldIndex, dbfield fld, int column);
extern "C" int        isoftes__FetchSQLLevel(sqlcursor cur, int level);
extern "C" void       isoftes__SetSQLLevel(sqlcursor cur, int level, dbtable tab);
extern "C" int        isoftes__ExecuteSQL(sqlcursor cur);

/*-------------------------------------------------------------------*/
extern "C" schema isoftix_OpenSchema(char *name, UShort mode);
extern "C" struct s_table * isoftix__AttachTable(struct s_table *tp);
extern "C" long	isoftix_GetRecord(dbindex ind,find_mode fmode,int mode, va_list ap);
extern "C" int isoftix_AddIndField(dbindex ind, dbfield fld, k_field_flags flags, UShort n, UShort m);
extern "C" void isoftix_AddRecord(dbtable tab);
extern "C" void isoftix_AddRecordTS(dbtable tab, TIMESTAMP crts, TIMESTAMP modts);
extern "C" long isoftix_AddRecordTest(dbtable tab);
extern "C" dbfield isoftix_AddTabField(dbtable tbl, char *fname, char *descr, UShort dim, type ty, sqltype sql_ty, UShort len, UChar ndec, field_flags flags, char *def, char *mask);
extern "C" void isoftix_BeginTrans(int mode);
extern "C" long isoftix_BuildIndex(dbindex ind, int flags, va_list ap);
extern "C" TIMESTAMP isoftix_CTimeStamp(dbtable tab);
extern "C" int isoftix_ChownSchema(schema sch, long uid);
extern "C" void isoftix_CloseAllSchemas(void);
extern "C" void isoftix_CloseSchema(schema sch);
extern "C" int isoftix_CompleteIndex(dbindex ind);
extern "C" int isoftix_ConvertTable(dbtable tab, dbtable newtab, bool force, bool drop, IFCPVA err_func, FCPVA msg_func, unsigned int, unsigned int, bool correctRefs);
extern "C" void isoftix_CopyTable(dbtable newtab, dbtable tab, dbfield *, int flag);
extern "C" void isoftix_CorruptTrans(void);
extern "C" long isoftix_CountCursor(dbcursor cursor);
extern "C" dbcursor isoftix_CreateCursor(dbindex ind, int mode);
extern "C" dbindex isoftix_CreateIndex(dbtable tbl, char *name, key_flags flags, UChar);
extern "C" schema isoftix_CreateSchema(char *name, char *descr);
extern "C" dbtable isoftix_CreateTable(schema sch, char *name, char *descr, tab_flags flags, ULong size, UChar n_flds);
extern "C" bool isoftix_DbCheckPoint(schema);
extern "C" bool isoftix_DbFlushReq(schema);
extern "C" int isoftix_DelAllRecords(dbtable tab);
extern "C" void isoftix_DelRecord(dbtable tab);
extern "C" void isoftix_DeleteCursor(dbcursor cursor);
extern "C" int isoftix_DeleteTable(dbtable tab);
extern "C" void isoftix_DiscardRecord(dbtable tbl);
extern "C" int isoftix_DropIndex(dbindex ind);
extern "C" int isoftix_DropSchema(schema sch);
extern "C" int isoftix_DropTable(dbtable tab);
extern "C" bool isoftix_EndTrans(bool syn, bool rttrans);
extern "C" bool isoftix_EndTransaction(void);
extern "C" long isoftix_FindRecord(dbindex ind, find_mode fmode, va_list ap);
extern "C" schema isoftix_FindSchemaVersion(char *name, long version);
extern "C" void isoftix_FlushTable(dbtable tab);
extern "C" void isoftix_FreeRecord(dbindex ind, find_mode fmode, va_list ap);
extern "C" void isoftix_FreeSchema(schema sch);
extern "C" void isoftix_FreeTable(dbtable tab);
extern "C" TIMESTAMP isoftix_GetDownLoadTS(dbtable tab);
extern "C" int isoftix_GetTransMode(void);
extern "C" bool isoftix_InTransaction();
extern "C" void isoftix_InitRecord(dbtable tab);
extern "C" int isoftix_LockSchema(schema sch, int lock_mode);
extern "C" int isoftix_LockTable(dbtable tab, int lock_mode);
extern "C" TIMESTAMP isoftix_MTimeStamp(dbtable tab);
extern "C" schema isoftix_OpenSchemaVersion(char *name, UShort mode, long version);
extern "C" void isoftix_OptimisticTransaction();
extern "C" long isoftix_PopRecord(dbtable tbl);
extern "C" void isoftix_PushRecord(dbtable tbl);
extern "C" void isoftix_PutRecord(dbtable tab);
extern "C" bool isoftix_RenameField(dbfield fld, char *name);
extern "C" int isoftix_RenameTable(dbtable tab, char *name);
extern "C" int isoftix_RollBack(void);
extern "C" void isoftix_SaneTrans(void);
extern "C" long isoftix_SchemaVersion(schema sch);
extern "C" int isoftix_SetCollateSet(dbfield fld, const char *cs);
extern "C" int isoftix_SetDataCache(dbtable tab, UShort n, int mode);
extern "C" int isoftix_SetDeflt(dbfield fld, struct DbExpr *expr);
extern "C" void isoftix_SetDescrSchema(schema sch, char *descr);
extern "C" void isoftix_SetDownLoadTS(dbtable tab, TIMESTAMP ts);
extern "C" int isoftix_SetInCheck(dbfield fld, struct InCheck *inchk);
extern "C" void isoftix_SetIndexCache(dbindex ind, UShort n);
extern "C" int isoftix_SetRelCheck(dbfield fld, struct DbExpr *expr);
extern "C" int isoftix_SetTabRelCheck(dbtable tab, struct DbExpr *expr);
extern "C" int isoftix_SetTableCache(dbtable , long );
extern "C" schema isoftix_SwitchToSchema(schema sch);
extern "C" long isoftix_TabActualSize(dbtable tab);
extern "C" long isoftix_TabNRecords(dbtable tab);
extern "C" TIMESTAMP isoftix_TableTimeStamp(dbtable tab);
extern "C" bool isoftix_TransOk(void);
//extern "C" void isoftix_UpdateRecord(dbtable tab, va_list ap);
extern "C" int isoftix_UpdateRecord(dbtable tab); /*JC BZ 3368 */

extern "C" long isoftix_VersionNumber(char *name, DATE d);
extern "C" long isoftix__AddKey(struct s_index *ip, long recno, char *rec_buff, struct tab_array *, dbindex ind);
extern "C" long isoftix__FindKey(struct tab_array *tap, struct s_index *ip, char *rec_buff, find_mode fmode, UShort nparts, char **,int);
extern "C" bool isoftix__FlushSchPrivileges(struct s_schema *sp);
extern "C" bool isoftix__FlushTabPrivileges(struct s_schema *sp);
extern "C" void isoftix__FlushTable(struct tab_array *tap, int ind);
extern "C" void isoftix__FreeCursorCache(struct s_dbcur *cp);
extern "C" long isoftix__GetNextBlock(struct s_dbcur *cp);
extern "C" long isoftix__GetPrevBlock(struct s_dbcur *cp);
extern "C" void isoftix__ModifySchema(struct s_schema *sp);
extern "C" dbtable isoftix__ReCreateTable(schema sch, char *name, char *descr, tab_flags flags, ULong size, UChar n_flds);
extern "C" void isoftix__ReOpenSchema(schema sch);
extern "C" void isoftix__SendRangeCursor(struct s_dbcur *cp);
extern "C" s_schema *	isoftix__GetSchPtr(schema sch) ;
extern "C" int isoftix__TabSeqNumber(dbtable tbl);
extern "C" int isoftix_dbmsType(void);
extern "C" char* isoftix__StrValPutOrAlloc(struct s_schema *schptr,char *s,char *prev);
extern "C" char* isoftix__MemValPutOrAlloc(struct s_schema *schptr,void *s,UShort len,void *prev);
extern "C" const char* isoftix_GetSchemas();

// SQL Ideafix
extern "C" void	      isoftix__TurnSQLFilterOn(void); 
extern "C" void	      isoftix__TurnSQLFilterOff(void); 

extern "C" sqlcursor  isoftix__CreateSQLCursor(schema sch, const char *sentence, int paramsCount);
extern "C" int        isoftix__FetchSQLNext(sqlcursor cur);
extern "C" void       isoftix__AddSQLCursorParameter(sqlcursor cur, type ty, UShort len, UShort ndec);
extern "C" int        isoftix__SetSQLCursorParameters(sqlcursor cur, va_list av);
extern "C" int        isoftix__SetSQLCursorParameter(sqlcursor cur, int fldIndex, char *value);
extern "C" void       isoftix__RestartSQLCursor(sqlcursor cur);
extern "C" int        isoftix__GetSQLIFld(sqlcursor cur, UShort fldIndex);
extern "C" long       isoftix__GetSQLLFld(sqlcursor cur, UShort fldIndex);
extern "C" char*      isoftix__GetSQLSFld(sqlcursor cur, UShort fldIndex);
extern "C" double     isoftix__GetSQLFFld(sqlcursor cur, UShort fldIndex);
extern "C" DATE       isoftix__GetSQLDFld(sqlcursor cur, UShort fldIndex);
extern "C" TIME       isoftix__GetSQLTFld(sqlcursor cur, UShort fldIndex);
extern "C" NUM        isoftix__GetSQLNFld(sqlcursor cur, UShort fldIndex);
extern "C" void       isoftix__DeleteSQLCursor(sqlcursor cur);
extern "C" void       isoftix__AddSQLFieldTarget(sqlcursor cur, UShort fldIndex, dbfield fld, int column);
extern "C" int        isoftix__FetchSQLLevel(sqlcursor cur, int level);
extern "C" void       isoftix__SetSQLLevel(sqlcursor cur, int level, dbtable tab);
extern "C" int        isoftix__ExecuteSQL(sqlcursor cur);

/*-------------------------------------------------------------------*/




extern "C" void	      isoftsql__TurnSQLFilterOn(void); 
extern "C" void	      isoftsql__TurnSQLFilterOff(void); 
extern "C" sqlcursor  isoftsql__CreateSQLCursor(schema sch, const char *sentence, int paramsCount);
extern "C" int  isoftsql__FetchSQLNext(sqlcursor cur);
extern "C" void  isoftsql__AddSQLCursorParameter(sqlcursor cur, type ty, UShort len, UShort ndec);
extern "C" int  isoftsql__SetSQLCursorParameters(sqlcursor cur, va_list av);
extern "C" int  isoftsql__SetSQLCursorParameter(sqlcursor cur, int fldIndex, char *value);
extern "C" void isoftsql__RestartSQLCursor(sqlcursor cur);
extern "C" int  isoftsql__GetSQLIFld(sqlcursor cur, UShort fldIndex);
extern "C" long  isoftsql__GetSQLLFld(sqlcursor cur, UShort fldIndex);
extern "C" char *isoftsql__GetSQLSFld(sqlcursor cur, UShort fldIndex);
extern "C" double  isoftsql__GetSQLFFld(sqlcursor cur, UShort fldIndex);
extern "C" DATE  isoftsql__GetSQLDFld(sqlcursor cur, UShort fldIndex);
extern "C" TIME  isoftsql__GetSQLTFld(sqlcursor cur, UShort fldIndex);
extern "C" NUM  isoftsql__GetSQLNFld(sqlcursor cur, UShort fldIndex);
extern "C" void isoftsql__DeleteSQLCursor(sqlcursor cur);
extern "C" void isoftsql__AddSQLFieldTarget(sqlcursor cur, UShort fldIndex, dbfield fld, int column);
extern "C" int  isoftsql_FetchSQLLevel(sqlcursor cur, int level);
extern "C" void isoftsql_SetSQLLevel(sqlcursor cur, int level, dbtable tab);

extern "C" int isoftsql_ExecuteSQL(sqlcursor cur);

#endif
