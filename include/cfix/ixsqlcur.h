#ifndef IXSQLCUR_H
#define IXSQLCUR_H

typedef long sqlcursor;


/* ++ Prototypes ++ */


#ifdef __cplusplus
extern "C"  {
#endif
extern void TurnSQLFilterOn(void);
extern void TurnSQLFilterOff(void);

extern sqlcursor CreateSQLCursor(schema sch, const char *sentence, int paramsCount);


extern int  FetchSQLNext(sqlcursor cur);
extern int  ExecuteSQL(sqlcursor cur);

extern void  AddSQLCursorParameter(sqlcursor cur, type ty, UShort len, UShort ndec);
extern int  SetSQLCursorParameters(sqlcursor cur, ...);
extern int  SetSQLCursorParameter(sqlcursor cur, int fldIndex, char *value);

extern void RestartSQLCursor(sqlcursor cur);

extern int  GetSQLIFld(sqlcursor cur, UShort fldIndex);
extern long  GetSQLLFld(sqlcursor cur, UShort fldIndex);
extern char *GetSQLSFld(sqlcursor cur, UShort fldIndex);
extern double  GetSQLFFld(sqlcursor cur, UShort fldIndex);
extern DATE  GetSQLDFld(sqlcursor cur, UShort fldIndex);
extern TIME  GetSQLTFld(sqlcursor cur, UShort fldIndex);
extern NUM  GetSQLNFld(sqlcursor cur, UShort fldIndex);

extern void DeleteSQLCursor(sqlcursor cur);

extern void SetSQLLevel(sqlcursor cur, int level, dbtable tab);
extern int FetchSQLLevel(sqlcursor cur, int level);


/* Asociacion entre resultados de la consulta y buffers CFIX para mantener
	el codigo de acceso actual.
*/
extern void AddSQLFieldTarget(sqlcursor cur, UShort fldIndex, dbfield fld, ...);


#ifdef __cplusplus
}
#endif

/* -- Prototypes -- */

#endif
