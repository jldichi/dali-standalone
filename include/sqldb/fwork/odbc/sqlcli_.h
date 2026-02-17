/*===========================================================================*/
/* sqlcli_.h                                                                 */
/*---------------------------------------------------------------------------*/
/* SQL Call Level Interface (CLI)                                            */
/*                                                                           */
/* (c) 1997 by Dirk Ohme - all right reserved                                */
/*---------------------------------------------------------------------------*/
/* definitions to be used:                                                   */
/*                                                                           */
/*   BCPP      - (with OS2 set) enable Borland C/C++ for OS/2                */
/*   IBMC      - (with OS2 set) set on IBM C/C++ compilers (VAC++)           */
/*   iODBC     - enable iODBC related stuff                                  */
/*   mSQL      - enable mSQL 1.x related stuff                               */
/*   mSQL2     - enable mSQL 2.x related stuff                               */
/*   ORACLE    - enable Oracle (tm) DB related stuff                         */
/*   OS2       - set when compiling under OS/2                               */
/*   WIN       - set when compiling under Windows                            */
/*   WINNT     - set when compiling under Windows NT                         */
/*   UNIDRV    - enable universal ODBC driver stuff                          */
/*                                                                           */
/*===========================================================================*/

/*---------------------------------------------------------------------------*/
/* frame                                                                     */
/*---------------------------------------------------------------------------*/
#ifndef __H_SQLCLI_
#define __H_SQLCLI_

/*---------------------------------------------------------------------------*/
/* ODBC version                                                              */
/*---------------------------------------------------------------------------*/
#define ODBCVER                         0x0200

/*---------------------------------------------------------------------------*/
/* system dependent data types                                               */
/*---------------------------------------------------------------------------*/
/* ... for OS/2 (BCPP = Borland C++)                                         */
/*---------------------------------------------------------------------------*/
#ifdef OS2
#  ifdef BCPP
#    define _Optlink
#    define _System                    _syscall
#  endif
#  define INCL_PM
#  define INCL_WINDIALOGS
#  include <os2.h>
#  define PASCAL                       _Pascal
#  define SQL_API                      APIENTRY
#  ifdef IBMC
#    define SQL_LOCAL_API              _Optlink
#  else
#    define SQL_LOCAL_API
#  endif

/*---------------------------------------------------------------------------*/
/* ... unknown systems                                                       */
/*---------------------------------------------------------------------------*/
#else
#  ifndef BOOL
#    define BOOL                       int
#  endif
#  ifndef CHAR
#    define CHAR                       char
#  endif
#  ifndef FALSE
#    define FALSE                      (0 != 0)
#  endif
#  ifndef HWND
#    define HWND                       int
#  endif
#  ifndef LONG
#    define LONG                       long
#  endif
#  ifndef PASCAL
#    define PASCAL
#  endif
#  ifndef SHORT
#    define SHORT                      short
#  endif
#  ifndef SQL_API
#    define SQL_API
#  endif
#  ifndef SQL_LOCAL_API
#    define SQL_LOCAL_API
#  endif
#  ifndef TRUE
#    define TRUE                       (0 == 0)
#  endif

  /*-----------------------------------------------------------------------*/
  /* replacement for msqlGetErrMsg() calls                                 */
  /*-----------------------------------------------------------------------*/
#  ifndef msqlGetErrMsg
#    define msqlGetErrMsg(x)           strcpy(x, msqlErrMsg)
#  endif

typedef unsigned char                  UCHAR;
typedef unsigned int                   UINT;
typedef unsigned long                  ULONG;
typedef unsigned short                 USHORT;
#endif

/*---------------------------------------------------------------------------*/
/* ... common                                                                */
/*---------------------------------------------------------------------------*/
#ifndef FAR
#  define                              FAR
#endif

#ifndef HANDLE
  typedef int                          HANDLE;
#endif

#ifndef MAX
# define MAX(x,y)                      ((x > y) ? x : y)
#endif

#ifndef MIN
# define MIN(x,y)                      ((x < y) ? x : y)
#endif

#ifndef NEAR
#  define NEAR
#endif

#ifndef RETCODE
#  define RETCODE                      short
#endif

/*---------------------------------------------------------------------------*/
/* ... for non-Windows systems                                               */
/*---------------------------------------------------------------------------*/
#ifndef WIN
typedef long                           DWORD;
typedef void                    FAR   *PTR;
typedef signed long                    SDWORD;
typedef signed short                   SWORD;
typedef unsigned long                  UDWORD;
typedef unsigned short                 UWORD;
typedef short                          WORD;
#endif

/*---------------------------------------------------------------------------*/
/* ... mSQL 1.x or 2.x                                                       */
/*---------------------------------------------------------------------------*/
#if defined(OS2) && defined(mSQL)
#  include <mSQL-1.x/msql.h>
#endif
#if defined(OS2) && defined(mSQL2)
#  include <mSQL-2.x/msql.h>
#endif
#ifndef OS2
#  if defined(mSQL) || defined(mSQL2)
#    include <msql.h>
#  endif
#endif

/*---------------------------------------------------------------------------*/
/* ... Oracle                                                                */
/*---------------------------------------------------------------------------*/
#ifdef ORACLE
#  include <oratypes.h>
#  include <ocidfn.h>
#  include <ocidem.h>
#  include <ociapr.h>
#endif

/*---------------------------------------------------------------------------*/
/* SQL data types                                                            */
/*---------------------------------------------------------------------------*/
/* ... definitions about data structures (length)                            */
/*---------------------------------------------------------------------------*/
#define SQL_DATE_LEN                    10
#define SQL_MAX_BUFFER_SIZE          65536
#define SQL_MAX_COLUMN                  64
#define SQL_MAX_CURSOR_NAME             32
#define SQL_MAX_DRV_ID_LENGTH          256
#define SQL_MAX_DSN_LENGTH              32
#define SQL_MAX_ID_LENGTH               18
#define SQL_MAX_MESSAGE_LENGTH         512
#define SQL_MAX_OPTION_STRING_LENGTH   511
#define SQL_MAX_PARAM                   16
#define SQL_SQLSTATE_SIZE                8
#define SQL_TIME_LEN                     8
#define SQL_TIMESTAMP_LEN               19

/*---------------------------------------------------------------------------*/
/* ... simple data types                                                     */
/*---------------------------------------------------------------------------*/
#define SQLPOINTER                     PTR
#define SQLRETURN                      RETCODE SQL_API
#define SQLUINTEGER                    UINT
#define SQLUSMALLINT                   USHORT

typedef unsigned char                  SQLCHAR;
typedef long                           SQLINTEGER;
typedef short                          SQLSMALLINT;
typedef double                         SQLDOUBLE;
typedef float                          SQLREAL;
typedef unsigned char                  SQLDECIMAL;
typedef unsigned char                  SQLNUMERIC;

/*---------------------------------------------------------------------------*/
/* ... extended data types                                                   */
/*---------------------------------------------------------------------------*/
typedef unsigned char                  SQLBINARY;
typedef BOOL                           SQLBIT;
typedef long                           SQLBLOBLOCATOR;
typedef long                           SQLCLOBLOCATOR;
typedef unsigned char                  SQLDBCHAR;
typedef long                           SQLDBCLOBLOCATOR;
typedef signed char                    SQLSCHAR;

#define SQLSHORT                       SQLTINYINT
#define SQLTINYINT                     SQLSCHAR

/*---------------------------------------------------------------------------*/
/* ... environment information for "SQLAllocEnv"                             */
/*---------------------------------------------------------------------------*/
#if defined(mSQL) || defined(mSQL2) || defined(ORACLE) || defined(UNIDRV)
typedef struct     tagENV                        /*--------------------------*/
  { UCHAR     szSqlState[SQL_SQLSTATE_SIZE];     /* status code              */
    UCHAR    *pszSqlMsg;                         /* status message           */
    UINT      ciConnections;                     /* number of DBC blocks     */
#  if defined(mSQL) || defined(mSQL2) || defined(UNIDRV)
    UCHAR     szEnvBuffer[SQL_MAX_BUFFER_SIZE];  /* internal env. buffer     */
#  endif
  } ENV, *HENV, *LPENV;                          /*--------------------------*/
#else
typedef void                           ENV,
                                FAR   *HENV,
                                FAR   *LPENV;
#endif
typedef HENV                           SQLHENV;

/*---------------------------------------------------------------------------*/
/* ... environment information for "SQLAllocConnect"                         */
/*---------------------------------------------------------------------------*/
#if defined(mSQL) || defined(mSQL2) || defined(ORACLE) || defined(UNIDRV)
typedef struct     tagDBC                        /*--------------------------*/
  { BOOL      fConnect;                          /* TRUE on open connection  */
    int       ciActive;                          /* counter for open trans.  */
    UCHAR    *szSqlState;                        /* status code              */
    UCHAR    *pszSqlMsg;                         /* status message           */
    LPENV     pSqlEnv;                           /* pointer to ENV structure */
#  if defined(mSQL) || defined(mSQL2)
    int       hDbSocket;                         /* communication socket     */
    char      szMsqlErrorMsg[256];               /* mSQL error message       */
#  endif
#  ifdef ORACLE
    Lda_Def   lda;                               /* connection info          */
    ub1       hda[256];                          /* communication area       */
#  endif
#  ifdef UNIDRV
    char      szPath[CCHMAXPATH];                /* connection path          */
    char      szErrorMsg[256];                   /* error message            */
#  endif
  } DBC, *HDBC, *LPDBC;                          /*--------------------------*/
#else
typedef void                           DBC,
                                FAR   *HDBC,
                                FAR   *LPDBC;
#endif
typedef HDBC                           SQLHDBC;

/*---------------------------------------------------------------------------*/
/* ... environment information for "SQLAllocStmt"                            */
/*---------------------------------------------------------------------------*/
#if defined(mSQL) || defined(mSQL2) || defined(ORACLE) || defined(UNIDRV)
typedef struct     tagSTMT                       /*--------------------------*/
  { char      szCursorName[SQL_MAX_CURSOR_NAME]; /* name of cursor           */
    char     *pszQuery;                          /* query string             */
    LPDBC     pSqlDbc;                           /* pointer to DB context    */
    UCHAR    *szSqlState;                        /* status code              */
    UCHAR    *pszSqlMsg;                         /* status message           */
    char     *szMsqlErrorMsg;                    /* mSQL error message       */
#  if defined(mSQL) || defined(mSQL2)
    m_result *pMsqlResult;                       /* mSQL results handle      */
    m_row     sRow;                              /* curr. processed row      */
#    if defined(mSQL2)
    int       cRows;                             /* number of rows altered   */
#    endif
#  endif
#  if defined(mSQL) || defined(mSQL2) || defined(UNIDRV)
                                                 /*--------------------------*/
    int       fSqlType[SQL_MAX_COLUMN];          /* type of database column  */
    int       fCType[SQL_MAX_COLUMN];            /* type of return column    */
    void     *pResultVar[SQL_MAX_COLUMN];        /* pointer to dest. var.    */
    SDWORD   *pResultLen[SQL_MAX_COLUMN];        /* pointer to dest. length  */
    int       cbMaxLen[SQL_MAX_COLUMN];          /* max. length for copy     */
                                                 /*--------------------------*/
    void     *pParamVar[SQL_MAX_PARAM];          /* pointer to parameter var.*/
    SDWORD    cbParamLen[SQL_MAX_PARAM];         /* length of transfered data*/
    SDWORD   *pcbParam[SQL_MAX_PARAM];           /* result var. for length   */
#  endif
#  ifdef ORACLE
    Cda_Def   cda;                               /* cursor definition        */
    SDWORD    ciCol;                             /* number of resulting cols */
#  endif
  } STMT, *HSTMT, *LPSTMT;                       /*--------------------------*/
#else
typedef void                           STMT,
                                FAR   *HSTMT,
                                FAR   *LPSTMT;
#endif
typedef HSTMT                          SQLHSTMT;

/*---------------------------------------------------------------------------*/
/* ... environment information for "???"                                     */
/*---------------------------------------------------------------------------*/
#if defined(mSQL) || defined(mSQL2) || defined(ORACLE) || defined(UNIDRV)
typedef struct     tagDESC                       /*--------------------------*/
  { short     DummyEntry;                        /* <unused>                 */
  } DESC, *HDESC, *LPDESC;                       /*--------------------------*/
#else
typedef void                           DESC,
                                FAR   *HDESC,
                                FAR   *LPDESC;
#endif
typedef HDESC                          SQLHDESC;

/*---------------------------------------------------------------------------*/
/* ... data structure for SQL_DATE                                           */
/*---------------------------------------------------------------------------*/
typedef struct tagDATE_STRUCT
  {
    SQLSMALLINT         year;
    SQLUSMALLINT        month;
    SQLUSMALLINT        day;
  }                                    DATE_STRUCT,
                                       SQLDATE;

/*---------------------------------------------------------------------------*/
/* ... data structure for SQL_TIME                                           */
/*---------------------------------------------------------------------------*/
typedef struct tagTIME_STRUCT
  {
    SQLUSMALLINT        hour;
    SQLUSMALLINT        minute;
    SQLUSMALLINT        second;
  }                                    TIME_STRUCT,
                                       SQLTIME;

/*---------------------------------------------------------------------------*/
/* ... data structure for SQL_TIMESTAMP                                      */
/*---------------------------------------------------------------------------*/
typedef struct tagTIMESTAMP_STRUCT
  {
    SQLSMALLINT         year;
    SQLUSMALLINT        month;
    SQLUSMALLINT        day;
    SQLUSMALLINT        hour;
    SQLUSMALLINT        minute;
    SQLUSMALLINT        second;
    SQLINTEGER          fraction;
  }                                    TIMESTAMP_STRUCT,
                                       SQLTIMESTAMP;

/*---------------------------------------------------------------------------*/
/* ... environment information for iODBC advanced driver setup               */
/*---------------------------------------------------------------------------*/
typedef struct _SQLDrvOptList {        /* driver options list                */
        char        szOptName[SQL_MAX_DRV_ID_LENGTH]; /* - option name       */
        char        szOptValue[SQL_MAX_DRV_ID_LENGTH]; /* - option value     */
        void       *pNext;             /*   - pointer to next element        */
} SQLDrvOptList, *SQLPDrvOptList;      /*                                    */
                                       /*------------------------------------*/
typedef struct _SQLDriverList {        /* driver list                        */
        char        szName[SQL_MAX_DRV_ID_LENGTH]; /* - name of driver       */
        SQLPDrvOptList pOptions;       /*   - start of options list          */
        void       *pNext;             /*   - pointer to next element        */
} SQLDriverList, *SQLPDriverList;      /*                                    */
                                       /*------------------------------------*/

/*---------------------------------------------------------------------------*/
/* common definitions                                                        */
/*---------------------------------------------------------------------------*/
/* ... about NULL pointers                                                   */
/*---------------------------------------------------------------------------*/
#define SQL_NULL_HENV                  NULL
#define SQL_NULL_HDBC                  NULL
#define SQL_NULL_HSTMT                 NULL
#define SQL_NULL_HDESC                 NULL
#define SQL_NULL_HANDLE                NULL

/*---------------------------------------------------------------------------*/
/* ... about NULL values                                                     */
/*---------------------------------------------------------------------------*/
#define SQL_NULL_DATA                     -1
#define SQL_DATA_AT_EXEC                  -2
#define SQL_NTS                           -3
#define SQL_NTSL                          -3L
#define SQL_NO_NULLS                       0
#define SQL_NULLABLE                       1
#define SQL_NULLABLE_UNKNOWN               2


/*---------------------------------------------------------------------------*/
/* ... about handle identifiers                                              */
/*---------------------------------------------------------------------------*/
#define SQL_HANDLE_ENV                     1
#define SQL_HANDLE_DBC                     2
#define SQL_HANDLE_STMT                    3
#define SQL_HANDLE_DESC                    4

/*---------------------------------------------------------------------------*/
/* ... about attributes                                                      */
/*---------------------------------------------------------------------------*/
#define SQL_ATTR_OUTPUT_NTS            10001
#define SQL_ATTR_AUTO_IPD              10001
#define SQL_ATTR_APP_ROW_DESC          10010
#define SQL_ATTR_APP_PARAM_DESC        10011
#define SQL_ATTR_IMP_ROW_DESC          10012
#define SQL_ATTR_IMP_PARAM_DESC        10013
#define SQL_ATTR_READONLY                  0
#define SQL_ATTR_WRITE                     1
#define SQL_ATTR_READWRITE_UNKNOWN         2

/*---------------------------------------------------------------------------*/
/* ... about column (attributes)                                             */
/*---------------------------------------------------------------------------*/
#define SQL_COLUMN_COUNT                   0
#define SQL_COLUMN_NAME                    1
#define SQL_COLUMN_TYPE                    2
#define SQL_COLUMN_LENGTH                  3
#define SQL_COLUMN_PRECISION               4
#define SQL_COLUMN_SCALE                   5
#define SQL_COLUMN_DISPLAY_SIZE            6
#define SQL_COLUMN_NULLABLE                7
#define SQL_COLUMN_UNSIGNED                8
#define SQL_COLUMN_MONEY                   9
#define SQL_COLUMN_UPDATABLE              10
#define SQL_COLUMN_AUTO_INCREMENT         11
#define SQL_COLUMN_CASE_SENSITIVE         12
#define SQL_COLUMN_SEARCHABLE             13
#define SQL_COLUMN_TYPE_NAME              14
#define SQL_COLUMN_TABLE_NAME             15
#define SQL_COLUMN_OWNER_NAME             16
#define SQL_COLUMN_QUALIFIER_NAME         17
#define SQL_COLUMN_LABEL                  18
#define SQL_COLUMN_SCHEMA_NAME            19
#define SQL_COLUMN_CATALOG_NAME           20
#define SQL_COLUMN_DRIVER_START         1000
#define SQL_COLUMN_DISTINCT_TYPE        1250
#define SQL_COLATT_OPT_MAX              SQL_COLUMN_LABEL
#define SQL_COLATT_OPT_MIN              SQL_COLUMN_COUNT

/*---------------------------------------------------------------------------*/
/* ... about descriptors                                                     */
/*---------------------------------------------------------------------------*/
#define SQL_DESC_COUNT                     1
#define SQL_DESC_TYPE                      2
#define SQL_DESC_LENGTH                    3
#define SQL_DESC_LENGTH_PTR                4
#define SQL_DESC_PRECISION                 5
#define SQL_DESC_SCALE                     6
#define SQL_DESC_DATETIME_INTERVAL_CODE    7
#define SQL_DESC_NULLABLE                  8
#define SQL_DESC_INDICATOR_PTR             9
#define SQL_DESC_DATA_PTR                 10
#define SQL_DESC_NAME                     11
#define SQL_DESC_UNNAMED                  12
#define SQL_DESC_ALLOC_TYPE               99

/*---------------------------------------------------------------------------*/
/* ... about the diagnostic area                                             */
/*---------------------------------------------------------------------------*/
#define SQL_DIAG_RETURNCODE                1
#define SQL_DIAG_NUMBER                    2
#define SQL_DIAG_ROW_COUNT                 3
#define SQL_DIAG_SQLSTATE                  4
#define SQL_DIAG_NATIVE                    5
#define SQL_DIAG_MESSAGE_TEXT              6
#define SQL_DIAG_DYNAMIC_FUNCTION          7
#define SQL_DIAG_CLASS_ORIGIN              8
#define SQL_DIAG_SUBCLASS_ORIGIN           9
#define SQL_DIAG_CONNECTION_NAME          10
#define SQL_DIAG_SERVER_NAME              11
#define SQL_DIAG_DYNAMIC_FUNCTION_CODE    12

/*---------------------------------------------------------------------------*/
/* ... about dynamic function codes                                          */
/*---------------------------------------------------------------------------*/
#define SQL_ALTER_DOMAIN                   3
#define SQL_ALTER_TABLE                    4
#define SQL_CREATE_ASSERTION               6
#define SQL_CREATE_CHARACTER_SET           8
#define SQL_CREATE_COLLATION              10
#define SQL_CREATE_DOMAIN                 23
#define SQL_CREATE_SCHEMA                 64
#define SQL_CREATE_TABLE                  77
#define SQL_CREATE_TRANSLATION            79
#define SQL_CREATE_VIEW                   84
#define SQL_DELETE_WHERE                  19
#define SQL_DROP_ASSERTION                24
#define SQL_DROP_CHARACTER_SET            25
#define SQL_DROP_COLLATION                26
#define SQL_DROP_DOMAIN                   27
#define SQL_DROP_SCHEMA                   31
#define SQL_DROP_TABLE                    32
#define SQL_DROP_TRANSLATION              33
#define SQL_DROP_VIEW                     36
#define SQL_DYNAMIC_DELETE_CURSOR         54
#define SQL_DYNAMIC_UPDATE_CURSOR         55
#define SQL_GRANT                         48
#define SQL_INSERT                        50
#define SQL_REVOKE                        59
#define SQL_SELECT                        41
#define SQL_SELECT_CURSOR                 85
#define SQL_SET_CATALOG                   66
#define SQL_SET_CONSTRAINT                68
#define SQL_SET_NAMES                     72
#define SQL_SET_SCHEMA                    74
#define SQL_SET_SESSION_AUTHORIZATION     76
#define SQL_SET_TIME_ZONE                 71
#define SQL_SET_TRANSACTION               75
#define SQL_UNKNOWN_STATEMENT              0
#define SQL_UPDATE_WHERE                  82

/*---------------------------------------------------------------------------*/
/* ... about SQL routines (iODBC)                                            */
/*---------------------------------------------------------------------------*/
#define SQL_API_ALL_FUNCTIONS              0

#define SQL_API_SQLALLOCCONNECT            1
#define SQL_API_SQLALLOCENV                2
#define SQL_API_SQLALLOCSTMT               3
#define SQL_API_SQLBINDCOL                 4
#define SQL_API_SQLCANCEL                  5
#define SQL_API_SQLCOLATTRIBUTES           6
#define SQL_API_SQLCONNECT                 7
#define SQL_API_SQLDESCRIBECOL             8
#define SQL_API_SQLDISCONNECT              9
#define SQL_API_SQLERROR                  10
#define SQL_API_SQLEXECDIRECT             11
#define SQL_API_SQLEXECUTE                12
#define SQL_API_SQLFETCH                  13
#define SQL_API_SQLFREECONNECT            14
#define SQL_API_SQLFREEENV                15
#define SQL_API_SQLFREESTMT               16
#define SQL_API_SQLGETCURSORNAME          17
#define SQL_API_SQLNUMRESULTCOLS          18
#define SQL_API_SQLPREPARE                19
#define SQL_API_SQLROWCOUNT               20
#define SQL_API_SQLSETCURSORNAME          21
#define SQL_API_SQLSETPARAM               22
#define SQL_API_SQLTRANSACT               23

#define SQL_NUM_FUNCTIONS                 23
#define SQL_EXT_API_START                 40

#define SQL_API_SQLCOLUMNS                40
#define SQL_API_SQLDRIVERCONNECT          41
#define SQL_API_SQLGETCONNECTOPTION       42
#define SQL_API_SQLGETDATA                43
#define SQL_API_SQLGETFUNCTIONS           44
#define SQL_API_SQLGETINFO                45
#define SQL_API_SQLGETSTMTOPTION          46
#define SQL_API_SQLGETTYPEINFO            47
#define SQL_API_SQLPARAMDATA              48
#define SQL_API_SQLPUTDATA                49
#define SQL_API_SQLSETCONNECTOPTION       50
#define SQL_API_SQLSETSTMTOPTION          51
#define SQL_API_SQLSPECIALCOLUMNS         52
#define SQL_API_SQLSTATISTICS             53
#define SQL_API_SQLTABLES                 54
#define SQL_API_SQLBROWSECONNECT          55
#define SQL_API_SQLCOLUMNPRIVILEGES       56
#define SQL_API_SQLDATASOURCES            57
#define SQL_API_SQLDESCRIBEPARAM          58
#define SQL_API_SQLEXTENDEDFETCH          59
#define SQL_API_SQLFOREIGNKEYS            60
#define SQL_API_SQLMORERESULTS            61
#define SQL_API_SQLNATIVESQL              62
#define SQL_API_SQLNUMPARAMS              63
#define SQL_API_SQLPARAMOPTIONS           64
#define SQL_API_SQLPRIMARYKEYS            65
#define SQL_API_SQLPROCEDURECOLUMNS       66
#define SQL_API_SQLPROCEDURES             67
#define SQL_API_SQLSETPOS                 68
#define SQL_API_SQLSETSCROLLOPTIONS       69
#define SQL_API_SQLTABLEPRIVILEGES        70
#define SQL_API_SQLDRIVERS                71
#define SQL_API_SQLBINDPARAMETER          72
#define SQL_EXT_API_LAST                SQL_API_SQLBINDPARAMETER

/*---------------------------------------------------------------------------*/
/* ... about data type codes (CLI, iODBC)                                    */
/*---------------------------------------------------------------------------*/
#define SQL_CHAR                           1
#define SQL_C_CHAR                         1
#define SQL_TINYINT                        1
#define SQL_C_TINYINT                      1
#define SQL_NUMERIC                        2
#define SQL_C_NUMERIC                      2
#define SQL_DECIMAL                        3
#define SQL_C_DECIMAL                      3
#define SQL_INTEGER                        4
#define SQL_C_INTEGER                      4
#define SQL_C_LONG                         4
#define SQL_SMALLINT                       5
#define SQL_C_SHORT                        5
#define SQL_C_SMALLINT                     5
#define SQL_FLOAT                          6
#define SQL_C_FLOAT                        6
#define SQL_REAL                           7
#define SQL_C_REAL                         7
#define SQL_DOUBLE                         8
#define SQL_C_DOUBLE                       8
#define SQL_DATETIME                       9
#define SQL_C_DATETIME                     9
#define SQL_INTERVAL                      10
#define SQL_C_INTERVAL                    10
#define SQL_VARCHAR                       12
#define SQL_C_VARCHAR                     12
#define SQL_BIT                           14
#define SQL_C_BIT                         14
#define SQL_BIT_VARYING                   15
#define SQL_C_BIT_VARYING                 15
#define SQL_DBCHAR                        16
#define SQL_C_DBCHAR                      16
#define SQL_DATE                          17
#define SQL_C_DATE                        17
#define SQL_TIME                          18
#define SQL_C_TIME                        18
#define SQL_TIMESTAMP                     19
#define SQL_C_TIMESTAMP                   19

/*---------------------------------------------------------------------------*/
/* ... about data type codes (ODBC)                                          */
/*---------------------------------------------------------------------------*/
#define SQL_BINARY                        20
#define SQL_C_BINARY                      20
#define SQL_BLOB                          21
#define SQL_C_BLOB                        21
#define SQL_BLOB_LOCATOR                  22
#define SQL_C_BLOB_LOCATOR                22
#define SQL_CLOB                          23
#define SQL_C_CLOB                        23
#define SQL_CLOB_LOCATOR                  24
#define SQL_C_CLOB_LOCATOR                24
#define SQL_DBCLOB                        25
#define SQL_C_DBCLOB                      25
#define SQL_DBCLOB_LOCATOR                26
#define SQL_C_DBCLOB_LOCATOR              26
#define SQL_GRAPHIC                       27
#define SQL_C_GRAPHIC                     27
#define SQL_LONGVARCHAR                   28
#define SQL_C_LONGVARCHAR                 28
#define SQL_LONGVARBINARY                 29
#define SQL_C_LONGVARBINARY               29
#define SQL_LONGVARGRAPHIC                30
#define SQL_C_LONGVARGRAPHIC              30
#define SQL_VARBINARY                     31
#define SQL_C_VARBINARY                   31
#define SQL_VARGRAPHIC                    32
#define SQL_C_VARGRAPHIC                  32

/*---------------------------------------------------------------------------*/
/* ... about smallest / greatest data type                                   */
/*---------------------------------------------------------------------------*/
#define SQL_TYPE_MIN                    SQL_BIT
#define SQL_TYPE_NULL                   SQL_NULL_HANDLE
#define SQL_TYPE_MAX                    SQL_VARCHAR

/*---------------------------------------------------------------------------*/
/* ... about bindery conversion code                                         */
/*---------------------------------------------------------------------------*/
#define SQL_ALL_TYPES                      0
#define SQL_UNKNOWN                        0
#define SQL_C_UNKNOWN                      0
#define SQL_DEFAULT                       99
#define SQL_C_DEFAULT                     99

/*---------------------------------------------------------------------------*/
/* ... about date/time sub-codes                                             */
/*---------------------------------------------------------------------------*/
#define SQL_CODE_DATE                      1
#define SQL_CODE_TIME                      2
#define SQL_CODE_TIMESTAMP                 3
#define SQL_CODE_TIME_ZONE                 4
#define SQL_CODE_TIMESTAMP_ZONE            5

/*---------------------------------------------------------------------------*/
/* ... about intervals                                                       */
/*---------------------------------------------------------------------------*/
#define SQL_DAY                            1
#define SQL_DAY_TO_HOUR                    2
#define SQL_DAY_TO_MINUTE                  3
#define SQL_DAY_TO_SECOND                  4
#define SQL_HOUR                           5
#define SQL_HOUR_TO_MINUTE                 6
#define SQL_HOUR_TO_SECOND                 7
#define SQL_MINUTE                         8
#define SQL_MINUTE_TO_SECOND               9
#define SQL_MONTH                         10
#define SQL_SECOND                        11
#define SQL_YEAR                          12
#define SQL_YEAR_TO_MONTH                 13

/*---------------------------------------------------------------------------*/
/* ... about option values                                                   */
/*---------------------------------------------------------------------------*/
#define SQL_FALSE                      FALSE
#define SQL_TRUE                       TRUE

/* Defines for SQLBindParameter/SQLProcedureColumns */
#define SQL_PARAM_TYPE_UNKNOWN             0     /*--------------------------*/
#define SQL_PARAM_INPUT                    1     /* SQLBindParameter,        */
#define SQL_PARAM_INPUT_OUTPUT             2     /* SQLProcedureColumns      */
#define SQL_RESULT_COL                     3     /*--------------------------*/
#define SQL_PARAM_OUTPUT                   4

#define SQL_PARAM_TYPE_DEFAULT         SQL_PARAM_INPUT_OUTPUT
#define SQL_SETPARAM_VALUE_MAX         -1L

#define SQL_UNSEARCHABLE                   0     /*--------------------------*/
#define SQL_LIKE_ONLY                      1     /* SQLColAttributes         */
#define SQL_ALL_EXCEPT_LIKE                2     /*--------------------------*/
#define SQL_SEARCHABLE                     3

#define SQL_DRIVER_NOPROMPT                0     /*--------------------------*/
#define SQL_DRIVER_COMPLETE                1     /* SQLDriverConnect         */
#define SQL_DRIVER_PROMPT                  2     /*--------------------------*/
#define SQL_DRIVER_COMPLETE_REQUIRED       3

#define SQL_FETCH_NEXT                     1     /*--------------------------*/
#define SQL_FETCH_FIRST                    2     /* SQLExtendedFetch         */
#define SQL_FETCH_LAST                     3     /*--------------------------*/
#define SQL_FETCH_PRIOR                    4
#define SQL_FETCH_ABSOLUTE                 5
#define SQL_FETCH_RELATIVE                 6
#define SQL_FETCH_BOOKMARK                 8

#define SQL_CLOSE                          0     /*--------------------------*/
#define SQL_DROP                           1     /* SQLFreeStmt              */
#define SQL_UNBIND                         2     /*--------------------------*/
#define SQL_RESET_PARAMS                   3

#define SQL_ACCESS_MODE                  101     /*--------------------------*/
#define SQL_AUTOCOMMIT                   102     /* SQLGetConnectionOption,  */
#define SQL_LOGIN_TIMEOUT                103     /* SQLSetConnectionOption   */
#define SQL_OPT_TRACE                    104     /*--------------------------*/
#define SQL_OPT_TRACEFILE                105
#define SQL_TRANSLATE_DLL                106
#define SQL_TRANSLATE_OPTION             107
#define SQL_TXN_ISOLATION                108
#define SQL_CURRENT_QUALIFIER            109
#define SQL_ODBC_CURSORS                 110
#define SQL_QUIET_MODE                   111
#define SQL_PACKET_SIZE                  112

#define SQL_CONN_OPT_MAX               SQL_PACKET_SIZE
#define SQL_CONN_OPT_MIN               SQL_ACCESS_MODE
#define SQL_CONNECT_OPT_DRVR_START      1000

#define SQL_MODE_READ_WRITE            0UL       /* SQL_ACCESS_MODE options  */
#define SQL_MODE_READ_ONLY             1UL
#define SQL_MODE_DEFAULT               SQL_MODE_READ_WRITE

#define SQL_AUTOCOMMIT_OFF             0UL       /* SQL_AUTOCOMMIT options   */
#define SQL_AUTOCOMMIT_ON              1UL
#define SQL_AUTOCOMMIT_DEFAULT         SQL_AUTOCOMMIT_ON

#define SQL_LOGIN_TIMEOUT_DEFAULT      15UL /* SQL_LOGIN_TIMEOUT options     */

#define SQL_OPT_TRACE_OFF              0UL       /* SQL_OPT_TRACE options    */
#define SQL_OPT_TRACE_ON               1UL
#define SQL_OPT_TRACE_DEFAULT          SQL_OPT_TRACE_OFF
#define SQL_OPT_TRACE_FILE_DEFAULT     "odbc.log"

#define SQL_CUR_USE_IF_NEEDED          0UL       /* SQL_ODBC_CURSORS options */
#define SQL_CUR_USE_ODBC               1UL
#define SQL_CUR_USE_DRIVER             2UL
#define SQL_CUR_DEFAULT                SQL_CUR_USE_DRIVER

#define SQL_INFO_FIRST                     0     /*--------------------------*/
#define SQL_DRIVER_HDBC                    3     /* SQLGetInfo               */
#define SQL_DRIVER_HENV                    4     /*--------------------------*/
#define SQL_DRIVER_HSTMT                   5
#define SQL_DRIVER_NAME                    6
#define SQL_DRIVER_VER                     7
#define SQL_ODBC_VER                      10
#define SQL_CURSOR_COMMIT_BEHAVIOR        23
#define SQL_CURSOR_ROLLBACK_BEHAVIOR      24
#define SQL_DEFAULT_TXN_ISOLATION         26
#define SQL_TXN_ISOLATION_OPTION          72
#define SQL_NON_NULLABLE_COLUMNS          75
#define SQL_DRIVER_HLIB                   76
#define SQL_DRIVER_ODBC_VER               77
#define SQL_QUALIFIER_LOCATION           114

#define SQL_INFO_LAST                  SQL_QUALIFIER_LOCATION
#define SQL_INFO_DRIVER_START           1000

#define SQL_TXN_READ_UNCOMMITTED  0x00000001UL   /*--------------------------*/
#define SQL_TXN_READ_COMMITTED    0x00000002UL   /* SQL_TXN_ISOLATION_OPTION */
#define SQL_TXN_REPEATABLE_READ   0x00000004UL   /* masks ...                */
#define SQL_TXN_SERIALIZABLE      0x00000008UL   /*--------------------------*/
#define SQL_TXN_VERSIONING        0x00000010UL

#define SQL_CB_DELETE             0x0000    /*-------------------------------*/
#define SQL_CB_CLOSE              0x0001    /* ...SQL_CURSOR_COMMIT_BEHAVIOR */
#define SQL_CB_PRESERVE           0x0002    /* SQL_CURSOR_ROLLBACK_BEHAVIOR  */
                                            /*-------------------------------*/

#define SQL_QUERY_TIMEOUT                  0     /*--------------------------*/
#define SQL_MAX_ROWS                       1     /* SQLGetStmtOption,        */
#define SQL_NOSCAN                         2     /* SQLSetStmtOption         */
#define SQL_MAX_LENGTH                     3     /*--------------------------*/
#define SQL_ASYNC_ENABLE                   4
#define SQL_BIND_TYPE                      5
#define SQL_CURSOR_TYPE                    6
#define SQL_CONCURRENCY                    7
#define SQL_KEYSET_SIZE                    8
#define SQL_ROWSET_SIZE                    9
#define SQL_SIMULATE_CURSOR               10
#define SQL_RETRIEVE_DATA                 11
#define SQL_USE_BOOKMARKS                 12
#define SQL_GET_BOOKMARK                  13     /* GetStmtOption Only       */
#define SQL_ROW_NUMBER                    14     /* GetStmtOption Only       */

#define SQL_STMT_OPT_MAX               SQL_ROW_NUMBER
#define SQL_STMT_OPT_MIN               SQL_QUERY_TIMEOUT

#define SQL_QUERY_TIMEOUT_DEFAULT      0UL  /* SQL_QUERY_TIMEOUT options     */
#define SQL_MAX_ROWS_DEFAULT           0UL  /* SQL_MAX_ROWS options          */
#define SQL_MAX_LENGTH_DEFAULT         0UL  /* SQL_MAX_LENGTH options        */

#define SQL_CONCUR_READ_ONLY               1     /* SQL_CONCURRENCY options  */
#define SQL_CONCUR_LOCK                    2
#define SQL_CONCUR_ROWVER                  3
#define SQL_CONCUR_VALUES                  4

#define SQL_SCROLL_FORWARD_ONLY            0L    /*--------------------------*/
#define SQL_SCROLL_KEYSET_DRIVEN          -1L    /* SQLSetScrollOption       */
#define SQL_SCROLL_DYNAMIC                -2L    /*--------------------------*/
#define SQL_SCROLL_STATIC                 -3L

#define SQL_DEFAULT_PARAM                 -5     /*--------------------------*/
#define SQL_IGNORE                        -6     /* SQLSetParam              */
                                                 /*--------------------------*/

#define SQL_ADD                            4     /*--------------------------*/
                                                 /* SQLSetPos                */
#define SQL_LOCK_NO_CHANGE                 0     /*--------------------------*/
#define SQL_LOCK_EXCLUSIVE                 1
#define SQL_LOCK_UNLOCK                    2

#define SQL_BEST_ROWID                     1     /*--------------------------*/
#define SQL_ROWVER                         2     /* SQLSpecialColumns        */
                                                 /*--------------------------*/
#define SQL_SCOPE_CURROW                   0
#define SQL_SCOPE_TRANSACTION              1
#define SQL_SCOPE_SESSION                  2

#define SQL_INDEX_UNIQUE                   0     /*--------------------------*/
#define SQL_INDEX_ALL                      1     /* SQLStatistics            */
                                                 /*--------------------------*/
#define SQL_QUICK                          0
#define SQL_ENSURE                         1

#define SQL_NAMED                          0     /*--------------------------*/
#define SQL_UNNAMED                        1     /* others ...               */
#define SQL_DESC_ALLOC_AUTO                1     /*--------------------------*/
#define SQL_DESC_ALLOC_USER                2

#define SQL_COMMIT                     SQL_FALSE
#define SQL_ROLLBACK                   SQL_TRUE

/*---------------------------------------------------------------------------*/
/* ... about return codes                                                    */
/*---------------------------------------------------------------------------*/
#define SQL_SUCCESS                        0
#define SQL_SUCCESS_WITH_INFO              1
#define SQL_STILL_EXECUTING                2
/* #define SQL_NEED_DATA                  99 */
#define SQL_NEED_DATA                    100
#define SQL_NO_DATA                      100
#define SQL_NO_DATA_FOUND                100
#define SQL_ERROR                         -1
#define SQL_INVALID_HANDLE                -2
#define SQL_SUCCEEDED(rc)              (((rc)&(~1))==0)

/*---------------------------------------------------------------------------*/
/* function prototypes "CLI"                                                 */
/*---------------------------------------------------------------------------*/
#if defined(__cplusplus) || defined(__IBMCPP__)
         extern  "C" {
#endif

SQLRETURN  SQLAllocHandle(
                             SQLSMALLINT         HandleType,
                             SQLINTEGER          InputHandle,
                             SQLINTEGER         *OutputHandle
                              );

SQLRETURN  SQLBindParam(
                             SQLHSTMT            StatementHandle,
                             SQLSMALLINT         ParamNumber,
                             SQLSMALLINT         BufferType,
                             SQLSMALLINT         ParamType,
                             SQLINTEGER          ParamLength,
                             SQLSMALLINT         Scale,
                             SQLPOINTER          Data,
                             SQLINTEGER         *DataLength
                       );

SQLRETURN  SQLCloseCursor(   SQLHSTMT            StatementHandle );

SQLRETURN  SQLCopyDesc(
                             SQLHDESC            SourceDescHandle,
                             SQLHDESC            TargetDescHandle
                      );

SQLRETURN  SQLEndTran(
                             SQLSMALLINT         HandleType,
                             SQLHENV             Handle,
                             SQLSMALLINT         CompletionType
                     );

SQLRETURN  SQLFreeHandle(
                             SQLSMALLINT         HandleType,
                             SQLINTEGER          Handle
                        );

SQLRETURN  SQLGetCol(
                             SQLHSTMT            StatementHandle,
                             SQLSMALLINT         ColumnNumber,
                             SQLPOINTER          TargetValue,
                             SQLINTEGER          BufferLength,
                             SQLINTEGER         *StringLength,
                             SQLINTEGER         *IndicatorValue
                    );

SQLRETURN  SQLGetConnectAttr(
                             SQLHDBC             ConnectionHandle,
                             SQLINTEGER          Attribute,
                             SQLPOINTER          Value,
                             SQLINTEGER          BufferLength,
                             SQLINTEGER         *StringLength
                            );

SQLRETURN  SQLGetDescField(
                             SQLHDESC            DescriptorHandle,
                             SQLSMALLINT         RecordNumber,
                             SQLSMALLINT         FieldIdentifier,
                             SQLPOINTER          Value,
                             SQLINTEGER          BufferLength,
                             SQLINTEGER         *StringLength
                          );

SQLRETURN  SQLGetDescRec(
                             SQLHDESC            DescriptorHandle,
                             SQLSMALLINT         RecordNumber,
                             SQLCHAR            *Name,
                             SQLSMALLINT         BufferLength,
                             SQLSMALLINT        *StringLength,
                             SQLSMALLINT        *Type,
                             SQLSMALLINT        *SubType,
                             SQLINTEGER         *Length,
                             SQLSMALLINT        *Precision,
                             SQLSMALLINT        *Scale,
                             SQLSMALLINT        *Nullable
                        );

SQLRETURN  SQLGetDiagField(
                             SQLSMALLINT         HandleType,
                             SQLINTEGER          Handle,
                             SQLSMALLINT         RecordNumber,
                             SQLSMALLINT         DiagIdentifier,
                             SQLPOINTER          DiagInfo,
                             SQLSMALLINT         BufferLength,
                             SQLSMALLINT        *StringLength
                          );

SQLRETURN  SQLGetDiagRec(
                             SQLSMALLINT         HandleType,
                             SQLINTEGER          Handle,
                             SQLSMALLINT         RecordNumber,
                             SQLCHAR            *Sqlstate,
                             SQLINTEGER         *NativeError,
                             SQLCHAR            *MessageText,
                             SQLSMALLINT         BufferLength,
                             SQLSMALLINT        *StringLength
                        );

SQLRETURN  SQLGetEnvAttr(
                             SQLHENV             EnvironmentHandle,
                             SQLINTEGER          Attribute,
                             SQLPOINTER          Value,
                             SQLINTEGER          BufferLength,
                             SQLINTEGER         *StringLength
                        );

SQLRETURN  SQLGetStmtAttr(
                             SQLHSTMT            StatementHandle,
                             SQLINTEGER          Attribute,
                             SQLPOINTER          Value,
                             SQLINTEGER          BufferLength,
                             SQLINTEGER         *StringLength
                         );

SQLRETURN  SQLLanguages(     SQLHSTMT            StatementHandle );

SQLRETURN  SQLReleaseEnv(    SQLHENV             EnvironmentHandle );

SQLRETURN  SQLServerInfo(
                             SQLHSTMT            StatementHandle,
                             SQLCHAR            *ServerAttribute,
                             SQLSMALLINT         AttributeLength
                        );

SQLRETURN  SQLSetDescField(
                             SQLHDESC            DescriptorHandle,
                             SQLSMALLINT         RecordNumber,
                             SQLSMALLINT         FieldIdentifier,
                             SQLPOINTER          Value,
                             SQLINTEGER          BufferLength
                          );

SQLRETURN  SQLSetDescRec(
                             SQLHDESC            DescriptorHandle,
                             SQLSMALLINT         RecordNumber,
                             SQLSMALLINT         Type,
                             SQLSMALLINT         SubType,
                             SQLINTEGER          Length,
                             SQLSMALLINT         Precision,
                             SQLSMALLINT         Scale,
                             SQLPOINTER          Data,
                             SQLINTEGER         *StringLength,
                             SQLSMALLINT *Indicator
                        );

SQLRETURN  SQLSetEnvAttr(
                             SQLHENV             EnvironmentHandle,
                             SQLINTEGER          Attribute,
                             SQLPOINTER          Value,
                             SQLINTEGER          StringLength
                        );

SQLRETURN  SQLSetStmtAttr(
                             SQLHSTMT            StatementHandle,
                             SQLINTEGER          Attribute,
                             SQLPOINTER          Value,
                             SQLINTEGER          StringLength
                         );

#if defined(__cplusplus) || defined(__IBMCPP__)
         }
#endif

/*---------------------------------------------------------------------------*/
/* function prototypes "ODBC"                                                */
/*---------------------------------------------------------------------------*/
#if defined(__cplusplus) || defined(__IBMCPP__)
         extern  "C" {
#endif

RETCODE SQL_API SQLAllocConnect(
                             LPENV               lpenv,
                             HDBC FAR           *phdbc
                               );

RETCODE SQL_API SQLAllocEnv( HENV FAR           *phenv );

RETCODE SQL_API SQLAllocStmt(
                             LPDBC               lpdbc,
                             HSTMT FAR          *phstmt
                           );

RETCODE SQL_API SQLBindCol(
                             LPSTMT              lpstmt,
                             UWORD               icol,
                             SWORD               fCType,
                             PTR                 rgbValue,
                             SDWORD              cbValueMax,
                             SDWORD FAR         *pcbValue
                           );

RETCODE SQL_API SQLBrowseConnect(
                             LPDBC               lpdbc,
                             UCHAR FAR          *szConnStrIn,
                             SWORD               cbConnStrIn,
                             UCHAR FAR          *szConnStrOut,
                             SWORD               cbConnStrOutMax,
                             SWORD FAR          *pcbConnStrOut
                           );

RETCODE SQL_API SQLCancel(   LPSTMT              lpstmt );

RETCODE SQL_API SQLColAttributes(
                             LPSTMT              lpstmt,
                             UWORD               icol,
                             UWORD               fDescType,
                             UCHAR FAR          *rgbDesc,
                             SWORD               cbDescMax,
                             SWORD FAR          *pcbDesc,
                             SDWORD FAR         *pfDesc
                           );

RETCODE SQL_API SQLColumnPrivileges(
                             HSTMT               hstmt,
                             UCHAR FAR          *szTableQualifier,
                             SWORD               cbTableQualifier,
                             UCHAR FAR          *szTableOwner,
                             SWORD               cbTableOwner,
                             UCHAR FAR          *szTableName,
                             SWORD               cbTableName,
                             UCHAR FAR          *szColumnName,
                             SWORD               cbColumnName
                           );

RETCODE SQL_API SQLColumns(
                             LPSTMT              lpstmt,
                             UCHAR FAR          *szTableQualifier,
                             SWORD               cbTableQualifier,
                             UCHAR FAR          *szTableOwner,
                             SWORD               cbTableOwner,
                             UCHAR FAR          *szTableName,
                             SWORD               cbTableName,
                             UCHAR FAR          *szColumnName,
                             SWORD               cbColumnName
                           );

RETCODE SQL_API SQLConnect(
                             LPDBC               lpdbc,
                             UCHAR FAR          *szDSN,
                             SWORD               cbDSN,
                             UCHAR FAR          *szUID,
                             SWORD               cbUID,
                             UCHAR FAR          *szAuthStr,
                             SWORD               cbAuthStr
                           );

RETCODE SQL_API SQLDescribeCol(
                             LPSTMT              lpstmt,
                             UWORD               icol,
                             UCHAR FAR          *szColName,
                             SWORD               cbColNameMax,
                             SWORD FAR          *pcbColName,
                             SWORD FAR          *pfSqlType,
                             UDWORD FAR         *pcbColDef,
                             SWORD FAR          *pibScale,
                             SWORD FAR          *pfNullable
                           );

RETCODE SQL_API SQLDescribeParam(
                             LPSTMT              lpstmt,
                             UWORD               ipar,
                             SWORD FAR          *pfSqlType,
                             UDWORD FAR         *pcbColDef,
                             SWORD FAR          *pibScale,
                             SWORD FAR          *pfNullable
                           );

RETCODE SQL_API SQLDisconnect( LPDBC             lpdbc );

RETCODE SQL_API SQLDriverConnect(
                             LPDBC               lpdbc,
                             HWND                hwnd,
                             UCHAR FAR          *szConnStrIn,
                             SWORD               cbConnStrIn,
                             UCHAR FAR          *szConnStrOut,
                             SWORD               cbConnStrOutMax,
                             SWORD FAR          *pcbConnStrOut,
                             UWORD               fDriverCompletion
                           );
#if defined(OS2)
RETCODE SQL_API SQLDriverSetup(
                             LPDBC               lpdbc,
                             HWND                hwnd,
                             HMODULE             hmod,
                             SQLPDriverList      pDrvEntry
                                );
#endif

RETCODE SQL_API SQLError(
                             LPENV               lpenv,
                             LPDBC               lpdbc,
                             LPSTMT              lpstmt,
                             UCHAR FAR          *szSqlState,
                             SDWORD FAR         *pfNativeError,
                             UCHAR FAR          *szErrorMsg,
                             SWORD               cbErrorMsgMax,
                             SWORD FAR          *pcbErrorMsg
                           );

RETCODE SQL_API SQLExecDirect(
                             LPSTMT              lpstmt,
                             UCHAR FAR          *szSqlStr,
                             SDWORD              cbSqlStr
                           );

RETCODE SQL_API SQLExecute(  LPSTMT              lpstmt );

RETCODE SQL_API SQLExtendedFetch(
                             LPSTMT              lpstmt,
                             UWORD               fFetchType,
                             SDWORD              irow,
                             UDWORD FAR         *pcrow,
                             UWORD FAR          *rgfRowStatus
                           );

RETCODE SQL_API SQLFetch(    LPSTMT              lpstmt );

RETCODE SQL_API SQLFreeConnect( LPDBC            lpdbc );

RETCODE SQL_API SQLFreeEnv(  LPENV               lpenv );

RETCODE SQL_API SQLForeignKeys(
                             HSTMT               hstmt,
                             UCHAR FAR          *szPkTableQualifier,
                             SWORD               cbPkTableQualifier,
                             UCHAR FAR          *szPkTableOwner,
                             SWORD               cbPkTableOwner,
                             UCHAR FAR          *szPkTableName,
                             SWORD               cbPkTableName,
                             UCHAR FAR          *szFkTableQualifier,
                             SWORD               cbFkTableQualifier,
                             UCHAR FAR          *szFkTableOwner,
                             SWORD               cbFkTableOwner,
                             UCHAR FAR          *szFkTableName,
                             SWORD               cbFkTableName
                           );

RETCODE SQL_API SQLFreeStmt(
                             LPSTMT              lpstmt,
                             UWORD               fOption
                           );

RETCODE SQL_API SQLGetConnectOption(
                             LPDBC               lpdbc,
                             UWORD               fOption,
                             PTR                 pvParam
                           );

RETCODE SQL_API SQLGetCursorName(
                             LPSTMT              lpstmt,
                             UCHAR FAR          *szCursor,
                             SWORD               cbCursorMax,
                             SWORD FAR          *pcbCursor
                           );

RETCODE SQL_API SQLGetData(
                             LPSTMT              lpstmt,
                             UWORD               icol,
                             SWORD               fCType,
                             PTR                 rgbValue,
                             SDWORD              cbValueMax,
                             SDWORD FAR         *pcbValue
                           );

RETCODE SQL_API SQLGetInfo(
                             LPDBC               lpdbc,
                             UWORD               fInfoType,
                             PTR                 rgbInfoValue,
                             SWORD               cbInfoValueMax,
                             SWORD FAR          *pcbInfoValue
                           );

RETCODE SQL_API SQLGetStmtOption(
                             LPSTMT              lpstmt,
                             UWORD               fOption,
                             PTR                 pvParam
                           );

RETCODE SQL_API SQLGetTypeInfo(
                             LPSTMT              lpstmt,
                             SWORD               fSqlType
                           );

RETCODE SQL_API SQLMoreResults( LPSTMT           lpstmt );

RETCODE SQL_API SQLNativeSql(
                             LPSTMT              lpstmt,
                             UCHAR FAR          *szSqlStrIn,
                             SDWORD              cbSqlStrIn,
                             UCHAR FAR          *szSqlStr,
                             SDWORD              cbSqlStrMax,
                             SDWORD FAR         *pcbSqlStr
                           );

RETCODE SQL_API SQLNumParams(
                             LPSTMT              lpstmt,
                             SWORD FAR          *pcpar
                           );

RETCODE SQL_API SQLNumResultCols(
                             LPSTMT              lpstmt,
                             SWORD FAR          *pccol
                           );

RETCODE SQL_API SQLParamData(
                             LPSTMT              lpstmt,
                             PTR FAR            *prbgValue
                           );

RETCODE SQL_API SQLParamOptions(
                             LPSTMT              lpstmt,
                             UDWORD              crow,
                             UDWORD FAR         *pirow
                           );

RETCODE SQL_API SQLPrepare(
                             LPSTMT              lpstmt,
                             UCHAR FAR          *szSqlStr,
                             SDWORD              cbSqlStr
                           );

RETCODE SQL_API SQLPrimaryKeys(
                             HSTMT               hstmt,
                             UCHAR FAR          *szTableQualifer,
                             SWORD               cbTableQualifier,
                             UCHAR FAR          *szTableOwner,
                             SWORD               cbTableOwner,
                             UCHAR FAR          *szTableName,
                             SWORD               cbTableName
                           );

RETCODE SQL_API SQLProcedureColumns(
                             HSTMT               hstmt,
                             UCHAR FAR          *szProcQualifier,
                             SWORD               cbProcQualifier,
                             UCHAR FAR          *szProcOwner,
                             SWORD               cbProcOwner,
                             UCHAR FAR          *szProcName,
                             SWORD               cbProcName,
                             UCHAR FAR          *szColumnName,
                             SWORD               cbColumnName
                           );

RETCODE SQL_API SQLProcedures(
                             HSTMT               hstmt,
                             UCHAR FAR          *szProcQualifier,
                             SWORD               cbProcQualifier,
                             UCHAR FAR          *szProcOwner,
                             SWORD               cbProcOwner,
                             UCHAR FAR          *szProcName,
                             SWORD               cbProcName
                           );

RETCODE SQL_API SQLPutData(
                             LPSTMT              lpstmt,
                             PTR                 rgbValue,
                             SDWORD              cbValue
                           );

RETCODE SQL_API SQLRowCount(
                             LPSTMT              lpstmt,
                             SDWORD FAR         *pcrow
                           );

RETCODE SQL_API SQLSetConnectOption(
                             LPDBC               lpdbc,
                             UWORD               fOption,
                             UDWORD              vParam
                           );

RETCODE SQL_API SQLSetCursorName(
                             LPSTMT              lpstmt,
                             UCHAR FAR          *szCursor,
                             SWORD               cbCursor
                           );

RETCODE SQL_API SQLSetParam(
                             LPSTMT              lpstmt,
                             UWORD               ipar,
                             SWORD               fCType,
                             SWORD               fSqlType,
                             UDWORD              cbColDef,
                             SWORD               ibScale,
                             PTR                 rgbValue,
                             SDWORD FAR         *pcbValue
                           );

RETCODE SQL_API SQLSetPos(
                             LPSTMT              lpstmt,
                             UWORD               irow,
                             BOOL                fRefresh,
                             BOOL                fLock
                           );

RETCODE SQL_API SQLSetScrollOptions(
                             LPSTMT              lpstmt,
                             UWORD               fConcurrency,
                             SDWORD              crowKeyset,
                             UWORD               crowRowset
                           );

RETCODE SQL_API SQLSetStmtOption(
                             LPSTMT              lpstmt,
                             UWORD               fOption,
                             UDWORD              vParam
                           );

RETCODE SQL_API SQLSpecialColumns(
                             HSTMT               hstmt,
                             UWORD               fColType,
                             UCHAR FAR          *szTableQualifer,
                             SWORD               cbTableQualifier,
                             UCHAR FAR          *szTableOwner,
                             SWORD               cbTableOwner,
                             UCHAR FAR          *szTableName,
                             SWORD               cbTableName,
                             UWORD               fScope,
                             UWORD               fNullable
                           );

RETCODE SQL_API SQLStatistics(
                             HSTMT               hstmt,
                             UCHAR FAR          *szTableQualifier,
                             SWORD               cbTableQualifier,
                             UCHAR FAR          *szTableOwner,
                             SWORD               cbTableOwner,
                             UCHAR FAR          *szTableName,
                             SWORD               cbTableName,
                             UWORD               fUnique,
                             UWORD               fAccuracy
                           );

RETCODE SQL_API SQLTablePrivileges(
                             HSTMT               hstmt,
                             UCHAR FAR          *szTableQualifier,
                             SWORD               cbTableQualifier,
                             UCHAR FAR          *szTableOwner,
                             SWORD               cbTableOwner,
                             UCHAR FAR          *szTableName,
                             SWORD               cbTableName
                           );

RETCODE SQL_API SQLTables(
                             LPSTMT              lpstmt,
                             UCHAR FAR          *szTableQualifier,
                             SWORD               cbTableQualifier,
                             UCHAR FAR          *szTableOwner,
                             SWORD               cbTableOwner,
                             UCHAR FAR          *szTableName,
                             SWORD               cbTableName,
                             UCHAR FAR          *szTableType,
                             SWORD               cbTableType
                           );

RETCODE SQL_API SQLTransact(
                             LPENV               lpenv,
                             LPDBC               lpdbc,
                             UWORD               fType
                           );

#if defined(__cplusplus) || defined(__IBMCPP__)
         }
#endif

/*---------------------------------------------------------------------------*/
/* frame                                                                     */
/*---------------------------------------------------------------------------*/
#endif

/*===| end of file |=========================================================*/
