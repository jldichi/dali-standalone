#ifndef _INTRINSIC_SQL_H
# define _INTRINSIC_SQL_H

#include "confg.h"

typedef unsigned char		UCHAR;
typedef long int		SDWORD;
typedef short int		SWORD;
typedef unsigned long int	UDWORD; 
typedef unsigned short int	UWORD;

typedef void FAR*		PTR;

typedef void FAR*		HENV;
typedef void FAR*		HDBC;
typedef void FAR*		HSTMT;

typedef signed short		RETCODE;
typedef	int		BOOL;
#define SQL_HANDLE_ENV             1

# ifdef WIN32
#   define SQL_API			__stdcall
# else
#   define SQL_API			EXPORT CALLBACK
# endif

# define ODBCVER			0x0200

#define SQL_HANDLE_DBC             2
#define SQL_HANDLE_STMT            3
#define SQL_HANDLE_DESC            4
#define SQL_HANDLE_DESC            4

/* generally useful constants */
# define SQL_MAX_MESSAGE_LENGTH		512
# define SQL_MAX_DSN_LENGTH		32

/* RETCODEs */
# define SQL_INVALID_HANDLE		(-2)
# define SQL_ERROR			(-1)
# define SQL_SUCCESS 			0
# define SQL_SUCCESS_WITH_INFO		1
# define SQL_NO_DATA_FOUND		100

/* standard SQL datatypes, using ANSI type numbering */
# define SQL_CHAR			1
# define SQL_NUMERIC 			2
# define SQL_DECIMAL 			3
# define SQL_INTEGER 			4
# define SQL_SMALLINT			5
# define SQL_FLOAT			6
# define SQL_REAL			7
# define SQL_DOUBLE			8
# define SQL_VARCHAR 			12

# define SQL_TYPE_MIN			SQL_CHAR
# define SQL_TYPE_NULL			0
# define SQL_TYPE_MAX			SQL_VARCHAR

/* C datatype to SQL datatype mapping SQL types */
# define SQL_C_CHAR			SQL_CHAR
# define SQL_C_LONG			SQL_INTEGER
# define SQL_C_SHORT   			SQL_SMALLINT
# define SQL_C_FLOAT   			SQL_REAL
# define SQL_C_DOUBLE			SQL_DOUBLE
# define SQL_C_DEFAULT 			99

# define SQL_NO_NULLS			0
# define SQL_NULLABLE			1
# define SQL_NULLABLE_UNKNOWN		2

/* Special length values */
# define SQL_NULL_DATA			(-1)
# define SQL_DATA_AT_EXEC		(-2)
# define SQL_NTS 			(-3)

/* SQLFreeStmt defines */
# define SQL_CLOSE			0
# define SQL_DROP			1
# define SQL_UNBIND			2
# define SQL_RESET_PARAMS		3

/* SQLTransact defines */
# define SQL_COMMIT			0
# define SQL_ROLLBACK			1

#define SQL_SQLSTATE_SIZE		  5 /* size of SQLSTATE 			   */
/* SQLColAttributes defines */
# define SQL_COLUMN_COUNT            	0
# define SQL_COLUMN_LABEL		18
# define SQL_COLATT_OPT_MAX		SQL_COLUMN_LABEL
# define SQL_COLUMN_DRIVER_START	1000

# define SQL_COLATT_OPT_MIN		SQL_COLUMN_COUNT

/* SQLError defines */
# define SQL_NULL_HENV			0
# define SQL_NULL_HDBC			0
# define SQL_NULL_HSTMT			0
// ojo esto pertenece a ODBC 3 - guiller
# define SQL_NULL_HANDLE     	0L
# define SQL_NC_LOW             1
# define SQL_SCCO_READ_ONLY		0x00000001L
# define SQL_IC_MIXED			4
# define SQL_SCCO_READ_ONLY                  0x00000001L
# define SQL_SO_FORWARD_ONLY                 0x00000001L
# define SQL_GB_NO_RELATION                  0x0003
# define SQL_IC_MIXED                        4
# define SQL_NNC_NON_NULL                    0x0001
# define SQL_OU_PRIVILEGE_DEFINITION         0x00000010L
# define SQL_SU_PRIVILEGE_DEFINITION         SQL_OU_PRIVILEGE_DEFINITION
# define SQL_PS_POSITIONED_DELETE            0x00000001L
# define SQL_PS_POSITIONED_UPDATE            0x00000002L
# define SQL_QL_START                        0x0001
# define SQL_CL_START                        SQL_QL_START
# define SQL_QU_DML_STATEMENTS               0x00000001L
# define SQL_CU_DML_STATEMENTS               SQL_QU_DML_STATEMENTS
# define SQL_SQ_COMPARISON                   0x00000001L
# define SQL_SQ_EXISTS                       0x00000002L
# define SQL_SQ_IN                           0x00000004L
# define SQL_SQ_QUANTIFIED                   0x00000008L

/*	
 * Added by Santiago Pericas at InterSoft Argentina
 */

/* SQLColAttributes subdefines for SQL_COLUMN_SEARCHABLE */
/* These are also used by SQLGetInfo                     */
#define SQL_UNSEARCHABLE			0
#define SQL_LIKE_ONLY				1
#define SQL_ALL_EXCEPT_LIKE 		2
#define SQL_SEARCHABLE				3

/* SQLColAttributes subdefines for SQL_COLUMN_UPDATABLE */
#define SQL_ATTR_READONLY			0
#define SQL_ATTR_WRITE				1
#define SQL_ATTR_READWRITE_UNKNOWN	2
// agragados para compilar iODBC
#define SQL_ATTR_OUTPUT_NTS    10001
#define SQL_ATTR_AUTO_IPD      10001
#define SQL_ATTR_METADATA_ID   10014
#define SQL_ATTR_APP_ROW_DESC       10010
#define SQL_ATTR_APP_PARAM_DESC     10011
#define SQL_ATTR_IMP_ROW_DESC       10012
#define SQL_ATTR_IMP_PARAM_DESC     10013
#define SQL_ATTR_CURSOR_SCROLLABLE  (-1)
#define SQL_ATTR_CURSOR_SENSITIVITY (-2)


/* SQLColAttributes defines */
#define SQL_COLUMN_COUNT            0
#define SQL_COLUMN_NAME             1
#define SQL_COLUMN_TYPE             2
#define SQL_COLUMN_LENGTH           3
#define SQL_COLUMN_PRECISION        4
#define SQL_COLUMN_SCALE            5
#define SQL_COLUMN_DISPLAY_SIZE     6
#define SQL_COLUMN_NULLABLE         7
#define SQL_COLUMN_UNSIGNED         8
#define SQL_COLUMN_MONEY            9
#define SQL_COLUMN_UPDATABLE		10
#define SQL_COLUMN_AUTO_INCREMENT	11
#define SQL_COLUMN_CASE_SENSITIVE	12
#define SQL_COLUMN_SEARCHABLE		13
#define SQL_COLUMN_TYPE_NAME		14
#if (ODBCVER >= 0x0200)
#define SQL_COLUMN_TABLE_NAME		15
#define SQL_COLUMN_OWNER_NAME		16
#define SQL_COLUMN_QUALIFIER_NAME	17
#define SQL_COLUMN_LABEL			18
#define SQL_COLATT_OPT_MAX			SQL_COLUMN_LABEL
#else
#define SQL_COLATT_OPT_MAX			SQL_COLUMN_TYPE_NAME
#endif	/* ODBCVER >= 0x0200 */
#define SQL_COLUMN_DRIVER_START		1000

#define	SQL_COLATT_OPT_MIN			SQL_COLUMN_COUNT

#ifdef __cplusplus
extern "C" {						/* Assume C declarations for C++   */
#endif	/* __cplusplus */

/* Core Function Prototypes */

RETCODE SQL_API SQLAllocConnect(
    HENV        henv,
    HDBC   FAR *phdbc);

RETCODE SQL_API SQLAllocEnv(
    HENV   FAR *phenv);

RETCODE SQL_API SQLAllocStmt(
    HDBC        hdbc,
    HSTMT  FAR *phstmt);

RETCODE SQL_API SQLBindCol(
    HSTMT       hstmt,
    UWORD       icol,
    SWORD       fCType,
    PTR         rgbValue,
    SDWORD      cbValueMax,
    SDWORD FAR *pcbValue);

RETCODE SQL_API SQLCancel(
    HSTMT       hstmt);

RETCODE SQL_API SQLColAttributes(
    HSTMT       hstmt,
    UWORD       icol,
    UWORD       fDescType,
    PTR         rgbDesc,
	SWORD       cbDescMax,
    SWORD  FAR *pcbDesc,
    SDWORD FAR *pfDesc);

RETCODE SQL_API SQLConnect(
    HDBC        hdbc,
    UCHAR  FAR *szDSN,
    SWORD       cbDSN,
    UCHAR  FAR *szUID,
    SWORD       cbUID,
    UCHAR  FAR *szAuthStr,
    SWORD       cbAuthStr);

RETCODE SQL_API SQLDescribeCol(
    HSTMT       hstmt,
    UWORD       icol,
    UCHAR  FAR *szColName,
    SWORD       cbColNameMax,
    SWORD  FAR *pcbColName,
    SWORD  FAR *pfSqlType,
    UDWORD FAR *pcbColDef,
    SWORD  FAR *pibScale,
    SWORD  FAR *pfNullable);

RETCODE SQL_API SQLDisconnect(
    HDBC        hdbc);

RETCODE SQL_API SQLError(
    HENV        henv,
    HDBC        hdbc,
    HSTMT       hstmt,
    UCHAR  FAR *szSqlState,
    SDWORD FAR *pfNativeError,
    UCHAR  FAR *szErrorMsg,
    SWORD       cbErrorMsgMax,
    SWORD  FAR *pcbErrorMsg);

RETCODE SQL_API SQLExecDirect(
    HSTMT       hstmt,
    UCHAR  FAR *szSqlStr,
    SDWORD      cbSqlStr);

RETCODE SQL_API SQLExecute(
    HSTMT       hstmt);

RETCODE SQL_API SQLFetch(
    HSTMT       hstmt);

RETCODE SQL_API SQLFreeConnect(
    HDBC        hdbc);

RETCODE SQL_API SQLFreeEnv(
    HENV        henv);

RETCODE SQL_API SQLFreeStmt(
    HSTMT       hstmt,
    UWORD       fOption);

RETCODE SQL_API SQLGetCursorName(
    HSTMT       hstmt,
    UCHAR  FAR *szCursor,
    SWORD       cbCursorMax,
    SWORD  FAR *pcbCursor);

RETCODE SQL_API SQLNumResultCols(
    HSTMT       hstmt,
    SWORD  FAR *pccol);

RETCODE SQL_API SQLPrepare(
    HSTMT       hstmt,
    UCHAR  FAR *szSqlStr,
    SDWORD      cbSqlStr);

RETCODE SQL_API SQLRowCount(
    HSTMT       hstmt,
    SDWORD FAR *pcrow);

RETCODE SQL_API SQLSetCursorName(
    HSTMT       hstmt,
    UCHAR  FAR *szCursor,
    SWORD       cbCursor);

RETCODE SQL_API SQLTransact(
    HENV        henv,
    HDBC        hdbc,
    UWORD       fType);

/*	Deprecrated functions from prior versions of ODBC */
#ifndef RC_INVOKED

RETCODE SQL_API SQLSetParam(		/*	Use SQLBindParameter */
    HSTMT       hstmt,
    UWORD       ipar,
    SWORD       fCType,
    SWORD       fSqlType,
    UDWORD      cbColDef,
    SWORD       ibScale,
    PTR         rgbValue,
    SDWORD FAR *pcbValue);

#endif /* RC_INVOKED */

#ifdef __cplusplus
}                                    /* End of extern "C" { */
#endif	/* __cplusplus */

#endif  
