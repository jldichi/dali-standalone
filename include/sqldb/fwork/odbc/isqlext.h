#ifndef	_INTRINSIC_SQLEXT_H
# define _INTRINSIC_SQLEXT_H

# include <sqldb/fwork/odbc/isql.h>

# define SQL_OV_ODBC3            	3UL

# define SQL_NO_TOTAL			(-4)

# define SQL_STILL_EXECUTING 		2
# define SQL_NEED_DATA			99

/* SQL extended datatypes */
# define SQL_DATE			9
# define SQL_TIME			10
# define SQL_TIMESTAMP			11
# define SQL_LONGVARCHAR 		(-1)
# define SQL_BINARY			(-2)
# define SQL_VARBINARY			(-3)
# define SQL_LONGVARBINARY		(-4)
# define SQL_BIGINT			(-5)
# define SQL_TINYINT 			(-6)
# define SQL_BIT 			(-7)
# define SQL_TYPE_DRIVER_START		(-80)

/* C datatype to SQL datatype mapping */
# define SQL_C_DATE			SQL_DATE
# define SQL_C_TIME			SQL_TIME
# define SQL_C_TIMESTAMP 		SQL_TIMESTAMP
# define SQL_C_BINARY			SQL_BINARY
# define SQL_C_BIT			SQL_BIT
# define SQL_C_TINYINT			SQL_TINYINT

# define SQL_SIGNED_OFFSET		(-20)
# define SQL_UNSIGNED_OFFSET		(-22)

# define SQL_C_SLONG			(SQL_C_LONG  + SQL_SIGNED_OFFSET)
# define SQL_C_SSHORT			(SQL_C_SHORT + SQL_SIGNED_OFFSET)
# define SQL_C_STINYINT			(SQL_TINYINT + SQL_SIGNED_OFFSET)
# define SQL_C_ULONG 			(SQL_C_LONG  + SQL_UNSIGNED_OFFSET)
# define SQL_C_USHORT			(SQL_C_SHORT + SQL_UNSIGNED_OFFSET)
# define SQL_C_UTINYINT			(SQL_TINYINT + SQL_UNSIGNED_OFFSET)
# define SQL_C_BOOKMARK			SQL_C_ULONG 			

# if defined(SQL_TYPE_MIN)
#   undef  SQL_TYPE_MIN  
#   define SQL_TYPE_MIN			SQL_BIT
# endif

# define SQL_ALL_TYPES			0
# define SQL_UNKNOWN			0
# define SQL_C_UNKNOWN			0
# define SQL_DEFAULT			99
# define SQL_C_DEFAULT			99

/* Options for SQLDriverConnect */
# define SQL_DRIVER_NOPROMPT		0
# define SQL_DRIVER_COMPLETE		1
# define SQL_DRIVER_PROMPT		2
# define SQL_DRIVER_COMPLETE_REQUIRED	3

/* SQLSetParam extensions */
# define SQL_DEFAULT_PARAM		(-5)
# define SQL_IGNORE			(-6)
# define SQL_LEN_DATA_AT_EXEC_OFFSET (-100)
# define SQL_LEN_DATA_AT_EXEC(length) (-length+SQL_LEN_DATA_AT_EXEC_OFFSET)

/* Defines for SQLGetFunctions */
# define SQL_API_SQLALLOCCONNECT	1   
# define SQL_API_SQLALLOCENV		2
# define SQL_API_SQLALLOCSTMT		3
# define SQL_API_SQLBINDCOL		4
# define SQL_API_SQLCANCEL		5
# define SQL_API_SQLCOLATTRIBUTES	6
# define SQL_API_SQLCONNECT		7
# define SQL_API_SQLDESCRIBECOL		8
# define SQL_API_SQLDISCONNECT		9
# define SQL_API_SQLERROR		10
# define SQL_API_SQLEXECDIRECT		11
# define SQL_API_SQLEXECUTE		12
# define SQL_API_SQLFETCH		13
# define SQL_API_SQLFREECONNECT		14
# define SQL_API_SQLFREEENV		15
# define SQL_API_SQLFREESTMT		16 
# define SQL_API_SQLGETCURSORNAME	17
# define SQL_API_SQLNUMRESULTCOLS	18
# define SQL_API_SQLPREPARE		19
# define SQL_API_SQLROWCOUNT		20
# define SQL_API_SQLSETCURSORNAME	21
# define SQL_API_SQLSETPARAM		22
# define SQL_API_SQLTRANSACT		23

# define SQL_NUM_FUNCTIONS		23

# define SQL_EXT_API_START		40

# define SQL_API_SQLCOLUMNS		40

# define SQL_API_SQLDRIVERCONNECT	41
# define SQL_API_SQLGETCONNECTOPTION	42
# define SQL_API_SQLGETDATA		43
# define SQL_API_SQLGETFUNCTIONS	44
# define SQL_API_SQLGETINFO		45
# define SQL_API_SQLGETSTMTOPTION	46
# define SQL_API_SQLGETTYPEINFO		47
# define SQL_API_SQLPARAMDATA		48
# define SQL_API_SQLPUTDATA		49
# define SQL_API_SQLSETCONNECTOPTION	50
# define SQL_API_SQLSETSTMTOPTION	51
# define SQL_API_SQLSPECIALCOLUMNS	52
# define SQL_API_SQLSTATISTICS		53
# define SQL_API_SQLTABLES		54

# define SQL_API_SQLBROWSECONNECT	55
# define SQL_API_SQLCOLUMNPRIVILEGES	56
# define SQL_API_SQLDATASOURCES		57
# define SQL_API_SQLDESCRIBEPARAM	58
# define SQL_API_SQLEXTENDEDFETCH	59
# define SQL_API_SQLFOREIGNKEYS		60
# define SQL_API_SQLMORERESULTS		61
# define SQL_API_SQLNATIVESQL		62
# define SQL_API_SQLNUMPARAMS		63
# define SQL_API_SQLPARAMOPTIONS	64
# define SQL_API_SQLPRIMARYKEYS		65
# define SQL_API_SQLPROCEDURECOLUMNS	66
# define SQL_API_SQLPROCEDURES		67
# define SQL_API_SQLSETPOS		68
# define SQL_API_SQLSETSCROLLOPTIONS	69
# define SQL_API_SQLTABLEPRIVILEGES	70

# define SQL_API_SQLDRIVERS          	71
# define SQL_API_SQLBINDPARAMETER	72
# define SQL_EXT_API_LAST		SQL_API_SQLBINDPARAMETER

# define SQL_API_ALL_FUNCTIONS		0

/* Defines for SQLGetInfo */
# define SQL_INFO_FIRST			0
# define SQL_DRIVER_HDBC		3
# define SQL_DRIVER_HENV		4
# define SQL_DRIVER_HSTMT		5
# define SQL_DRIVER_NAME		6
# define SQL_ODBC_VER			10
# define SQL_CURSOR_COMMIT_BEHAVIOR	23
# define SQL_CURSOR_ROLLBACK_BEHAVIOR	24
# define SQL_DEFAULT_TXN_ISOLATION	26

# define SQL_TXN_ISOLATION_OPTION	72
# define SQL_NON_NULLABLE_COLUMNS	75

# define SQL_DRIVER_HLIB		76
# define SQL_DRIVER_ODBC_VER		77

# define SQL_QUALIFIER_LOCATION		114

# define SQL_INFO_LAST			SQL_QUALIFIER_LOCATION

# define SQL_INFO_DRIVER_START		1000


/* SQL_TXN_ISOLATION_OPTION masks */
# define SQL_TXN_READ_UNCOMMITTED	0x00000001L
# define SQL_TXN_READ_COMMITTED		0x00000002L
# define SQL_TXN_REPEATABLE_READ 	0x00000004L
# define SQL_TXN_SERIALIZABLE		0x00000008L
# define SQL_TXN_VERSIONING		0x00000010L

/* SQL_CURSOR_COMMIT_BEHAVIOR and SQL_CURSOR_ROLLBACK_BEHAVIOR values */

# define SQL_CB_DELETE			0x0000
# define SQL_CB_CLOSE			0x0001
# define SQL_CB_PRESERVE		0x0002

/* options for SQLGetStmtOption/SQLSetStmtOption */
# define SQL_QUERY_TIMEOUT		0
# define SQL_MAX_ROWS			1
# define SQL_NOSCAN			2
# define SQL_MAX_LENGTH			3
# define SQL_ASYNC_ENABLE		4
# define SQL_BIND_TYPE			5
# define SQL_CURSOR_TYPE 		6
# define SQL_CONCURRENCY 		7
# define SQL_KEYSET_SIZE 		8
# define SQL_ROWSET_SIZE 		9
# define SQL_SIMULATE_CURSOR 		10
# define SQL_RETRIEVE_DATA		11
# define SQL_USE_BOOKMARKS		12
# define SQL_GET_BOOKMARK		13	/* GetStmtOption Only */
# define SQL_ROW_NUMBER			14	/* GetStmtOption Only */
# define SQL_STMT_OPT_MAX		SQL_ROW_NUMBER

# define SQL_STMT_OPT_MIN		SQL_QUERY_TIMEOUT


/* SQL_QUERY_TIMEOUT options */
# define SQL_QUERY_TIMEOUT_DEFAULT	0UL

/* SQL_MAX_ROWS options */
# define SQL_MAX_ROWS_DEFAULT		0UL

/* SQL_MAX_LENGTH options */
# define SQL_MAX_LENGTH_DEFAULT		0UL

/* SQL_CONCURRENCY options */
# define SQL_CONCUR_READ_ONLY		1
# define SQL_CONCUR_LOCK 		2
# define SQL_CONCUR_ROWVER		3
# define SQL_CONCUR_VALUES		4

/* options for SQLSetConnectOption/SQLGetConnectOption */
# define SQL_ACCESS_MODE 		101
# define SQL_AUTOCOMMIT			102
# define SQL_LOGIN_TIMEOUT		103
# define SQL_OPT_TRACE			104
# define SQL_OPT_TRACEFILE		105
# define SQL_TRANSLATE_DLL		106
# define SQL_TRANSLATE_OPTION		107
# define SQL_TXN_ISOLATION		108
# define SQL_CURRENT_QUALIFIER		109
# define SQL_ODBC_CURSORS		110
# define SQL_QUIET_MODE			111
# define SQL_PACKET_SIZE 		112
# define SQL_CONN_OPT_MAX		SQL_PACKET_SIZE
# define SQL_CONNECT_OPT_DRVR_START	1000

# define SQL_CONN_OPT_MIN		SQL_ACCESS_MODE

/* SQL_ACCESS_MODE options */
# define SQL_MODE_READ_WRITE 		0UL
# define SQL_MODE_READ_ONLY		1UL
# define SQL_MODE_DEFAULT		SQL_MODE_READ_WRITE

/* SQL_AUTOCOMMIT options */
# define SQL_AUTOCOMMIT_OFF		0UL
# define SQL_AUTOCOMMIT_ON		1UL
# define SQL_AUTOCOMMIT_DEFAULT		SQL_AUTOCOMMIT_ON

/* SQL_LOGIN_TIMEOUT options */
# define SQL_LOGIN_TIMEOUT_DEFAULT	15UL

/* SQL_OPT_TRACE options */
# define SQL_OPT_TRACE_OFF		0UL
# define SQL_OPT_TRACE_ON		1UL
# define SQL_OPT_TRACE_DEFAULT		SQL_OPT_TRACE_OFF
# define SQL_OPT_TRACE_FILE_DEFAULT	"odbc.log"

/* SQL_ODBC_CURSORS options */
# define SQL_CUR_USE_IF_NEEDED		0UL
# define SQL_CUR_USE_ODBC		1UL
# define SQL_CUR_USE_DRIVER		2UL
# define SQL_CUR_DEFAULT 		SQL_CUR_USE_DRIVER

/* Column types and scopes in SQLSpecialColumns. */
# define SQL_BEST_ROWID			1
# define SQL_ROWVER			2

# define SQL_SCOPE_CURROW		0
# define SQL_SCOPE_TRANSACTION		1
# define SQL_SCOPE_SESSION		2

/* Operations in SQLSetPos */
# define SQL_ADD			4

/* Lock options in SQLSetPos */
# define SQL_LOCK_NO_CHANGE		0 	
# define SQL_LOCK_EXCLUSIVE		1		
# define SQL_LOCK_UNLOCK 		2

/* SQLExtendedFetch "fFetchType" values */
# define SQL_FETCH_NEXT			1
# define SQL_FETCH_FIRST 		2
# define SQL_FETCH_LAST			3
# define SQL_FETCH_PRIOR		4
# define SQL_FETCH_ABSOLUTE		5
# define SQL_FETCH_RELATIVE		6
# define SQL_FETCH_BOOKMARK		8

/* Defines for SQLBindParameter/SQLProcedureColumns */
# define SQL_PARAM_TYPE_UNKNOWN		0
# define SQL_PARAM_INPUT		1
# define SQL_PARAM_INPUT_OUTPUT		2
# define SQL_RESULT_COL			3
# define SQL_PARAM_OUTPUT		4

/* Defines used by Driver Manager when mapping SQLSetParam to SQLBindParameter */
# define SQL_PARAM_TYPE_DEFAULT		SQL_PARAM_INPUT_OUTPUT
# define SQL_SETPARAM_VALUE_MAX		(-1L)

/* Defines for SQLStatistics */
# define SQL_INDEX_UNIQUE		0
# define SQL_INDEX_ALL			1

# define SQL_QUICK			0
# define SQL_ENSURE			1

/* Defines for SQLSetScrollOption */
# define SQL_SCROLL_FORWARD_ONLY	0L
# define SQL_SCROLL_KEYSET_DRIVEN	(-1L)
# define SQL_SCROLL_DYNAMIC		(-2L)
# define SQL_SCROLL_STATIC		(-3L) 

/*	
 * Added by Santiago Pericas at InterSoft Argentina
 */
typedef struct tagDATE_STRUCT
{
	SWORD year;
	UWORD month;
	UWORD day;
} DATE_STRUCT;

typedef struct tagTIME_STRUCT
{
	UWORD hour;
	UWORD minute;
	UWORD second;
} TIME_STRUCT;

typedef struct tagTIMESTAMP_STRUCT
{
	SWORD year;
	UWORD month;
	UWORD day;
	UWORD hour;
	UWORD minute;
	UWORD second;
	UDWORD fraction;
} TIMESTAMP_STRUCT;

typedef signed char   SCHAR;
typedef double        SDOUBLE;
typedef float         SFLOAT;
typedef char*         LPSTR;

typedef void (*FARPROC)(void);

#define SQL_INFO_FIRST                       0
#define SQL_ACTIVE_CONNECTIONS               0
#define SQL_ACTIVE_STATEMENTS                1
#define SQL_DATA_SOURCE_NAME                 2
#define SQL_DRIVER_HDBC                      3
#define SQL_DRIVER_HENV                      4
#define SQL_DRIVER_HSTMT                     5
#define SQL_DRIVER_NAME                      6
#define SQL_DRIVER_VER                       7
#define SQL_FETCH_DIRECTION                  8
#define SQL_ODBC_API_CONFORMANCE             9
#define SQL_ODBC_VER                        10
#define SQL_ROW_UPDATES                     11
#define SQL_ODBC_SAG_CLI_CONFORMANCE        12
#define SQL_SERVER_NAME                     13
#define SQL_SEARCH_PATTERN_ESCAPE           14
#define SQL_ODBC_SQL_CONFORMANCE            15

#define SQL_DBMS_NAME                       17
#define SQL_DBMS_VER                        18

#define SQL_ACCESSIBLE_TABLES               19
#define SQL_ACCESSIBLE_PROCEDURES           20
#define SQL_PROCEDURES                      21
#define SQL_CONCAT_NULL_BEHAVIOR            22
#define SQL_CURSOR_COMMIT_BEHAVIOR          23
#define SQL_CURSOR_ROLLBACK_BEHAVIOR        24
#define SQL_DATA_SOURCE_READ_ONLY           25
#define SQL_DEFAULT_TXN_ISOLATION           26
#define SQL_EXPRESSIONS_IN_ORDERBY          27
#define SQL_IDENTIFIER_CASE                 28
#define SQL_IDENTIFIER_QUOTE_CHAR           29
#define SQL_MAX_COLUMN_NAME_LEN             30
#define SQL_MAX_CURSOR_NAME_LEN             31
#define SQL_MAX_OWNER_NAME_LEN              32
#define SQL_MAX_PROCEDURE_NAME_LEN          33
#define SQL_MAX_QUALIFIER_NAME_LEN          34
#define SQL_MAX_TABLE_NAME_LEN              35
#define SQL_MULT_RESULT_SETS                36
#define SQL_MULTIPLE_ACTIVE_TXN             37
#define SQL_OUTER_JOINS                     38
#define SQL_OWNER_TERM                      39
#define SQL_PROCEDURE_TERM                  40
#define SQL_QUALIFIER_NAME_SEPARATOR        41
#define SQL_QUALIFIER_TERM                  42
#define SQL_SCROLL_CONCURRENCY              43
#define SQL_SCROLL_OPTIONS                  44
#define SQL_TABLE_TERM                      45
#define SQL_TXN_CAPABLE                     46
#define SQL_USER_NAME                       47

#define SQL_CONVERT_FUNCTIONS               48
#define SQL_NUMERIC_FUNCTIONS               49
#define SQL_STRING_FUNCTIONS                50
#define SQL_SYSTEM_FUNCTIONS                51
#define SQL_TIMEDATE_FUNCTIONS              52

#define SQL_CONVERT_BIGINT                  53
#define SQL_CONVERT_BINARY                  54
#define SQL_CONVERT_BIT                     55
#define SQL_CONVERT_CHAR                    56
#define SQL_CONVERT_DATE                    57
#define SQL_CONVERT_DECIMAL                 58
#define SQL_CONVERT_DOUBLE                  59
#define SQL_CONVERT_FLOAT                   60
#define SQL_CONVERT_INTEGER                 61
#define SQL_CONVERT_LONGVARCHAR             62
#define SQL_CONVERT_NUMERIC                 63
#define SQL_CONVERT_REAL                    64
#define SQL_CONVERT_SMALLINT                65
#define SQL_CONVERT_TIME                    66
#define SQL_CONVERT_TIMESTAMP               67
#define SQL_CONVERT_TINYINT                 68
#define SQL_CONVERT_VARBINARY               69
#define SQL_CONVERT_VARCHAR                 70
#define SQL_CONVERT_LONGVARBINARY           71

#define SQL_TXN_ISOLATION_OPTION            72
#define SQL_ODBC_SQL_OPT_IEF                73

/*** ODBC SDK 1.0 Additions ***/
#define SQL_CORRELATION_NAME				74
#define SQL_NON_NULLABLE_COLUMNS			75

/*** ODBC SDK 2.0 Additions ***/
#if (ODBCVER >= 0x0200)
#define SQL_DRIVER_HLIB 					76
#define SQL_DRIVER_ODBC_VER 				77
#define SQL_LOCK_TYPES						78
#define SQL_POS_OPERATIONS					79
#define SQL_POSITIONED_STATEMENTS			80
#define SQL_GETDATA_EXTENSIONS				81
#define SQL_BOOKMARK_PERSISTENCE			82
#define SQL_STATIC_SENSITIVITY				83
#define SQL_FILE_USAGE						84
#define SQL_NULL_COLLATION					85
#define SQL_ALTER_TABLE 					86
#define SQL_COLUMN_ALIAS					87
#define SQL_GROUP_BY						88
#define SQL_KEYWORDS						89
#define SQL_ORDER_BY_COLUMNS_IN_SELECT		90
#define SQL_OWNER_USAGE 					91
#define SQL_QUALIFIER_USAGE 				92
#define SQL_QUOTED_IDENTIFIER_CASE			93
#define SQL_SPECIAL_CHARACTERS				94
#define SQL_SUBQUERIES						95
#define SQL_UNION							96
#define SQL_MAX_COLUMNS_IN_GROUP_BY 		97
#define SQL_MAX_COLUMNS_IN_INDEX			98
#define SQL_MAX_COLUMNS_IN_ORDER_BY 		99
#define SQL_MAX_COLUMNS_IN_SELECT		   100
#define SQL_MAX_COLUMNS_IN_TABLE		   101
#define SQL_MAX_INDEX_SIZE				   102
#define SQL_MAX_ROW_SIZE_INCLUDES_LONG	   103
#define SQL_MAX_ROW_SIZE				   104
#define SQL_MAX_STATEMENT_LEN			   105
#define SQL_MAX_TABLES_IN_SELECT		   106
#define SQL_MAX_USER_NAME_LEN			   107
#define SQL_MAX_CHAR_LITERAL_LEN 		   108
#define SQL_TIMEDATE_ADD_INTERVALS		   109
#define SQL_TIMEDATE_DIFF_INTERVALS 	   110
#define SQL_NEED_LONG_DATA_LEN			   111
#define SQL_MAX_BINARY_LITERAL_LEN		   112
#define SQL_LIKE_ESCAPE_CLAUSE			   113
#define SQL_QUALIFIER_LOCATION			   114

#define SQL_INFO_LAST						SQL_QUALIFIER_LOCATION
#else
#define SQL_INFO_LAST						SQL_NON_NULLABLE_COLUMNS
#endif	/* ODBCVER >= 0x0200 */

#define SQL_INFO_DRIVER_START             1000

/* SQL_FETCH_DIRECTION masks */

#define SQL_FD_FETCH_NEXT			0x00000001L
#define SQL_FD_FETCH_FIRST			0x00000002L
#define SQL_FD_FETCH_LAST			0x00000004L
#define SQL_FD_FETCH_PRIOR			0x00000008L
#define SQL_FD_FETCH_ABSOLUTE		0x00000010L
#define SQL_FD_FETCH_RELATIVE		0x00000020L
#define SQL_FD_FETCH_RESUME 		0x00000040L
#if (ODBCVER >= 0x0200)
#define SQL_FD_FETCH_BOOKMARK		0x00000080L
#endif	/* ODBCVER >= 0x0200 */

/* SQL_FILE_USAGE values */

#define SQL_FILE_NOT_SUPPORTED		0x0000
#define SQL_FILE_TABLE				0x0001
#define SQL_FILE_QUALIFIER			0x0002

/* SQL_GETDATA_EXTENSIONS values */

#define SQL_GD_ANY_COLUMN			0x00000001L
#define SQL_GD_ANY_ORDER			0x00000002L
#define SQL_GD_BLOCK				0x00000004L
#define SQL_GD_BOUND				0x00000008L

/* SQL_ODBC_API_CONFORMANCE values */

#define SQL_OAC_NONE				0x0000
#define SQL_OAC_LEVEL1				0x0001
#define SQL_OAC_LEVEL2				0x0002

/* SQL_ODBC_SAG_CLI_CONFORMANCE values */

#define SQL_OSCC_NOT_COMPLIANT		0x0000
#define SQL_OSCC_COMPLIANT			0x0001

/* SQL_ODBC_SQL_CONFORMANCE values */

#define SQL_OSC_MINIMUM 			0x0000
#define SQL_OSC_CORE				0x0001
#define SQL_OSC_EXTENDED			0x0002

/*	Deprecated defines from prior versions of ODBC */
#define SQL_DATABASE_NAME			16	/* Use SQLGetConnectOption/SQL_CURRENT_QUALIFIER */
#define SQL_FD_FETCH_PREV			SQL_FD_FETCH_PRIOR
#define SQL_FETCH_PREV				SQL_FETCH_PRIOR
#define SQL_CONCUR_TIMESTAMP		SQL_CONCUR_ROWVER
#define SQL_SCCO_OPT_TIMESTAMP		SQL_SCCO_OPT_ROWVER
#define SQL_CC_DELETE				SQL_CB_DELETE
#define SQL_CR_DELETE				SQL_CB_DELETE
#define SQL_CC_CLOSE				SQL_CB_CLOSE
#define SQL_CR_CLOSE				SQL_CB_CLOSE
#define SQL_CC_PRESERVE 			SQL_CB_PRESERVE
#define SQL_CR_PRESERVE 			SQL_CB_PRESERVE
#define SQL_FETCH_RESUME			7	/* Not supported by 2.0 drivers */
#define SQL_SCROLL_FORWARD_ONLY 	0L	/*-SQL_CURSOR_FORWARD_ONLY	*/
#define SQL_SCROLL_KEYSET_DRIVEN	(-1L) /*-SQL_CURSOR_KEYSET_DRIVEN */
#define SQL_SCROLL_DYNAMIC			(-2L) /*-SQL_CURSOR_DYNAMIC		*/
#if (ODBCVER >= 0x0200)
#define SQL_SCROLL_STATIC			(-3L) /*-SQL_CURSOR_STATIC		*/
#endif	/* ODBCVER >= 0x0200 */

/* SQL_TIMEDATE_ADD_INTERVALS and SQL_TIMEDATE_DIFF_INTERVALS functions */

#if (ODBCVER >= 0x0200)
#define SQL_FN_TSI_FRAC_SECOND		0x00000001L
#define SQL_FN_TSI_SECOND			0x00000002L
#define SQL_FN_TSI_MINUTE			0x00000004L
#define SQL_FN_TSI_HOUR 			0x00000008L
#define SQL_FN_TSI_DAY				0x00000010L
#define SQL_FN_TSI_WEEK 			0x00000020L
#define SQL_FN_TSI_MONTH			0x00000040L
#define SQL_FN_TSI_QUARTER			0x00000080L
#define SQL_FN_TSI_YEAR 			0x00000100L
#endif	/* ODBCVER >= 0x0200 */

/* SQL_TXN_CAPABLE values */

#define SQL_TC_NONE 				0x0000
#define SQL_TC_DML					0x0001
#define SQL_TC_ALL					0x0002
#if (ODBCVER >= 0x0200)
#define SQL_TC_DDL_COMMIT			0x0003
#define SQL_TC_DDL_IGNORE			0x0004
#endif	/* ODBCVER >= 0x0200 */

/* SQL_CORRELATION_NAME values */

#define SQL_CN_NONE 				0x0000
#define SQL_CN_DIFFERENT			0x0001
#define SQL_CN_ANY					0x0002

/* SQL_STRING_FUNCTIONS functions */

#define SQL_FN_STR_CONCAT			0x00000001L
#define SQL_FN_STR_INSERT			0x00000002L
#define SQL_FN_STR_LEFT 			0x00000004L
#define SQL_FN_STR_LTRIM			0x00000008L
#define SQL_FN_STR_LENGTH			0x00000010L
#define SQL_FN_STR_LOCATE			0x00000020L
#define SQL_FN_STR_LCASE			0x00000040L
#define SQL_FN_STR_REPEAT			0x00000080L
#define SQL_FN_STR_REPLACE			0x00000100L
#define SQL_FN_STR_RIGHT			0x00000200L
#define SQL_FN_STR_RTRIM			0x00000400L
#define SQL_FN_STR_SUBSTRING		0x00000800L
#define SQL_FN_STR_UCASE			0x00001000L
#define SQL_FN_STR_ASCII			0x00002000L
#define SQL_FN_STR_CHAR 			0x00004000L
#if (ODBCVER >= 0x0200)
#define SQL_FN_STR_DIFFERENCE		0x00008000L
#define SQL_FN_STR_LOCATE_2 		0x00010000L
#define SQL_FN_STR_SOUNDEX			0x00020000L
#define SQL_FN_STR_SPACE			0x00040000L
#endif	/* ODBCVER >= 0x0200 */

/* SQL_NUMERIC_FUNCTIONS functions */

#define SQL_FN_NUM_ABS				0x00000001L
#define SQL_FN_NUM_ACOS 			0x00000002L
#define SQL_FN_NUM_ASIN 			0x00000004L
#define SQL_FN_NUM_ATAN 			0x00000008L
#define SQL_FN_NUM_ATAN2			0x00000010L
#define SQL_FN_NUM_CEILING			0x00000020L
#define SQL_FN_NUM_COS				0x00000040L
#define SQL_FN_NUM_COT				0x00000080L
#define SQL_FN_NUM_EXP				0x00000100L
#define SQL_FN_NUM_FLOOR			0x00000200L
#define SQL_FN_NUM_LOG				0x00000400L
#define SQL_FN_NUM_MOD				0x00000800L
#define SQL_FN_NUM_SIGN 			0x00001000L
#define SQL_FN_NUM_SIN				0x00002000L
#define SQL_FN_NUM_SQRT 			0x00004000L
#define SQL_FN_NUM_TAN				0x00008000L
#define SQL_FN_NUM_PI				0x00010000L
#define SQL_FN_NUM_RAND 			0x00020000L
#if (ODBCVER >= 0x0200)
#define SQL_FN_NUM_DEGREES			0x00040000L
#define SQL_FN_NUM_LOG10			0x00080000L
#define SQL_FN_NUM_POWER			0x00100000L
#define SQL_FN_NUM_RADIANS			0x00200000L
#define SQL_FN_NUM_ROUND			0x00400000L
#define SQL_FN_NUM_TRUNCATE 		0x00800000L
#endif	/* ODBCVER >= 0x0200 */

/* SQL_TIMEDATE_FUNCTIONS functions */

#define SQL_FN_TD_NOW				0x00000001L
#define SQL_FN_TD_CURDATE			0x00000002L
#define SQL_FN_TD_DAYOFMONTH		0x00000004L
#define SQL_FN_TD_DAYOFWEEK 		0x00000008L
#define SQL_FN_TD_DAYOFYEAR 		0x00000010L
#define SQL_FN_TD_MONTH 			0x00000020L
#define SQL_FN_TD_QUARTER			0x00000040L
#define SQL_FN_TD_WEEK				0x00000080L
#define SQL_FN_TD_YEAR				0x00000100L
#define SQL_FN_TD_CURTIME			0x00000200L
#define SQL_FN_TD_HOUR				0x00000400L
#define SQL_FN_TD_MINUTE			0x00000800L
#define SQL_FN_TD_SECOND			0x00001000L
#if (ODBCVER >= 0x0200)
#define SQL_FN_TD_TIMESTAMPADD		0x00002000L
#define SQL_FN_TD_TIMESTAMPDIFF 	0x00004000L
#define SQL_FN_TD_DAYNAME			0x00008000L
#define SQL_FN_TD_MONTHNAME 		0x00010000L
#endif	/* ODBCVER >= 0x0200 */

/* SQL_SYSTEM_FUNCTIONS functions */

#define SQL_FN_SYS_USERNAME 		0x00000001L
#define SQL_FN_SYS_DBNAME			0x00000002L
#define SQL_FN_SYS_IFNULL			0x00000004L

/* SQL_TIMEDATE_ADD_INTERVALS and SQL_TIMEDATE_DIFF_INTERVALS functions */

#if (ODBCVER >= 0x0200)
#define SQL_FN_TSI_FRAC_SECOND		0x00000001L
#define SQL_FN_TSI_SECOND			0x00000002L
#define SQL_FN_TSI_MINUTE			0x00000004L
#define SQL_FN_TSI_HOUR 			0x00000008L
#define SQL_FN_TSI_DAY				0x00000010L
#define SQL_FN_TSI_WEEK 			0x00000020L
#define SQL_FN_TSI_MONTH			0x00000040L
#define SQL_FN_TSI_QUARTER			0x00000080L
#define SQL_FN_TSI_YEAR 			0x00000100L
#endif	/* ODBCVER >= 0x0200 */

#ifdef __cplusplus
extern "C" {						/* Assume C declarations for C++   */
#endif	/* __cplusplus */

/* Level 1 Prototypes */
RETCODE SQL_API SQLColumns(
    HSTMT       hstmt,
    UCHAR  FAR *szTableQualifier,
    SWORD       cbTableQualifier,
    UCHAR  FAR *szTableOwner,
    SWORD       cbTableOwner,
    UCHAR  FAR *szTableName,
    SWORD       cbTableName,
    UCHAR  FAR *szColumnName,
    SWORD       cbColumnName);

RETCODE SQL_API SQLDriverConnect(
    HDBC        hdbc,
    HWND        hwnd,
    UCHAR  FAR *szConnStrIn,
    SWORD       cbConnStrIn,
    UCHAR  FAR *szConnStrOut,
    SWORD       cbConnStrOutMax,
    SWORD  FAR *pcbConnStrOut,
    UWORD       fDriverCompletion);

RETCODE SQL_API SQLGetConnectOption(
    HDBC        hdbc,
    UWORD       fOption,
    PTR         pvParam);

RETCODE SQL_API SQLGetData(
    HSTMT       hstmt,
    UWORD       icol,
    SWORD       fCType,
    PTR         rgbValue,
    SDWORD      cbValueMax,
    SDWORD FAR *pcbValue);

RETCODE SQL_API SQLGetFunctions(
    HDBC        hdbc,
    UWORD       fFunction,
    UWORD  FAR *pfExists);

RETCODE SQL_API SQLGetInfo(
    HDBC        hdbc,
    UWORD       fInfoType,
    PTR         rgbInfoValue,
    SWORD       cbInfoValueMax,
    SWORD  FAR *pcbInfoValue);

RETCODE SQL_API SQLGetStmtOption(
    HSTMT       hstmt,
    UWORD       fOption,
    PTR         pvParam);

RETCODE SQL_API SQLGetTypeInfo(
    HSTMT       hstmt,
    SWORD       fSqlType);

RETCODE SQL_API SQLParamData(
    HSTMT       hstmt,
    PTR    FAR *prgbValue);

RETCODE SQL_API SQLPutData(
    HSTMT       hstmt,
    PTR         rgbValue,
    SDWORD      cbValue);

RETCODE SQL_API SQLSetConnectOption(
    HDBC        hdbc,
    UWORD       fOption,
    UDWORD      vParam);

RETCODE SQL_API SQLSetStmtOption(
    HSTMT       hstmt,
    UWORD       fOption,
    UDWORD      vParam);

RETCODE SQL_API SQLSpecialColumns(
    HSTMT       hstmt,
    UWORD       fColType,
    UCHAR  FAR *szTableQualifier,
    SWORD       cbTableQualifier,
    UCHAR  FAR *szTableOwner,
    SWORD       cbTableOwner,
    UCHAR  FAR *szTableName,
    SWORD       cbTableName,
    UWORD       fScope,
    UWORD       fNullable);

RETCODE SQL_API SQLStatistics(
    HSTMT       hstmt,
    UCHAR  FAR *szTableQualifier,
    SWORD       cbTableQualifier,
    UCHAR  FAR *szTableOwner,
    SWORD       cbTableOwner,
    UCHAR  FAR *szTableName,
    SWORD       cbTableName,
    UWORD       fUnique,
    UWORD       fAccuracy);

RETCODE SQL_API SQLTables(
    HSTMT       hstmt,
    UCHAR  FAR *szTableQualifier,
    SWORD       cbTableQualifier,
    UCHAR  FAR *szTableOwner,
    SWORD       cbTableOwner,
    UCHAR  FAR *szTableName,
    SWORD       cbTableName,
    UCHAR  FAR *szTableType,
    SWORD       cbTableType);

/* Level 2 Prototypes */
RETCODE SQL_API SQLBrowseConnect(
    HDBC        hdbc,
    HWND		hwnd,			/* Differs with the manual ! */
	UCHAR  FAR *szConnStrIn,
    SWORD       cbConnStrIn,
    UCHAR  FAR *szConnStrOut,
    SWORD       cbConnStrOutMax,
    SWORD  FAR *pcbConnStrOut);

RETCODE SQL_API SQLColumnPrivileges(
    HSTMT       hstmt,
    UCHAR  FAR *szTableQualifier,
    SWORD       cbTableQualifier,
    UCHAR  FAR *szTableOwner,
    SWORD       cbTableOwner,
    UCHAR  FAR *szTableName,
    SWORD       cbTableName,
    UCHAR  FAR *szColumnName,
    SWORD       cbColumnName);

RETCODE SQL_API SQLDataSources(
    HENV        henv,
    UWORD       fDirection,
    UCHAR  FAR *szDSN,
    SWORD       cbDSNMax,
    SWORD  FAR *pcbDSN,
    UCHAR  FAR *szDescription,
    SWORD       cbDescriptionMax,
    SWORD  FAR *pcbDescription);

RETCODE SQL_API SQLDescribeParam(
    HSTMT       hstmt,
    UWORD       ipar,
    SWORD  FAR *pfSqlType,
    UDWORD FAR *pcbColDef,
    SWORD  FAR *pibScale,
    SWORD  FAR *pfNullable);
               
RETCODE SQL_API SQLExtendedFetch(
    HSTMT       hstmt,
    UWORD       fFetchType,
    SDWORD      irow,
    UDWORD FAR *pcrow,
    UWORD  FAR *rgfRowStatus);

RETCODE SQL_API SQLForeignKeys(
    HSTMT       hstmt,
    UCHAR  FAR *szPkTableQualifier,
    SWORD       cbPkTableQualifier,
    UCHAR  FAR *szPkTableOwner,
    SWORD       cbPkTableOwner,
    UCHAR  FAR *szPkTableName,
    SWORD       cbPkTableName,
    UCHAR  FAR *szFkTableQualifier,
    SWORD       cbFkTableQualifier,
    UCHAR  FAR *szFkTableOwner,
    SWORD       cbFkTableOwner,
    UCHAR  FAR *szFkTableName,
    SWORD       cbFkTableName);

RETCODE SQL_API SQLMoreResults(
    HSTMT       hstmt);

RETCODE SQL_API SQLNativeSql(
    HDBC        hdbc,
    UCHAR  FAR *szSqlStrIn,
    SDWORD      cbSqlStrIn,
    UCHAR  FAR *szSqlStr,
    SDWORD      cbSqlStrMax,
    SDWORD FAR *pcbSqlStr);

RETCODE SQL_API SQLNumParams(
    HSTMT       hstmt,
    SWORD  FAR *pcpar);

RETCODE SQL_API SQLParamOptions(
    HSTMT       hstmt,
    UDWORD      crow,
    UDWORD FAR *pirow);

RETCODE SQL_API SQLPrimaryKeys(
    HSTMT       hstmt,
    UCHAR  FAR *szTableQualifier,
    SWORD       cbTableQualifier,
    UCHAR  FAR *szTableOwner,
    SWORD       cbTableOwner,
    UCHAR  FAR *szTableName,
    SWORD       cbTableName);

RETCODE SQL_API SQLProcedureColumns(
    HSTMT       hstmt,
    UCHAR  FAR *szProcQualifier,
    SWORD       cbProcQualifier,
    UCHAR  FAR *szProcOwner,
    SWORD       cbProcOwner,
    UCHAR  FAR *szProcName,
    SWORD       cbProcName,
    UCHAR  FAR *szColumnName,
    SWORD       cbColumnName);

RETCODE SQL_API SQLProcedures(
    HSTMT       hstmt,
    UCHAR  FAR *szProcQualifier,
    SWORD       cbProcQualifier,
    UCHAR  FAR *szProcOwner,
    SWORD       cbProcOwner,
    UCHAR  FAR *szProcName,
    SWORD       cbProcName);

RETCODE SQL_API SQLSetPos(
    HSTMT       hstmt,
    UWORD       irow,
    UWORD       fOption,
    UWORD       fLock);

RETCODE SQL_API SQLTablePrivileges(
    HSTMT       hstmt,
    UCHAR  FAR *szTableQualifier,
    SWORD       cbTableQualifier,
    UCHAR  FAR *szTableOwner,
    SWORD       cbTableOwner,
    UCHAR  FAR *szTableName,
    SWORD       cbTableName);

/* SDK 2.0 Additions */

#if (ODBCVER >= 0x0200)
RETCODE SQL_API SQLDrivers(
    HENV        henv,
    UWORD       fDirection,
	UCHAR FAR  *szDriverDesc,
	SWORD		cbDriverDescMax,
	SWORD FAR  *pcbDriverDesc,
	UCHAR FAR  *szDriverAttributes,
    SWORD       cbDrvrAttrMax,
    SWORD  FAR *pcbDrvrAttr);

RETCODE SQL_API SQLBindParameter(
    HSTMT       hstmt,
	UWORD		ipar,
	SWORD		fParamType,
    SWORD       fCType,
	SWORD		fSqlType,
    UDWORD      cbColDef,
    SWORD       ibScale,
    PTR         rgbValue,
	SDWORD		cbValueMax,
    SDWORD FAR *pcbValue);
#endif	/* ODBCVER >= 0x0200 */

#ifdef __cplusplus
}                                    /* End of extern "C" { */
#endif	/* __cplusplus */

#define SQL_ATTR_ODBC_VERSION                           200
#define SQL_ATTR_CONNECTION_POOLING                     201
#define SQL_ATTR_CP_MATCH                                       202
#define SQL_ATTR_ACCESS_MODE            SQL_ACCESS_MODE
#define SQL_ATTR_AUTOCOMMIT                     SQL_AUTOCOMMIT
#define SQL_ATTR_CONNECTION_TIMEOUT     113
#define SQL_ATTR_CURRENT_CATALOG        SQL_CURRENT_QUALIFIER
#define SQL_ATTR_DISCONNECT_BEHAVIOR    114
#define SQL_ATTR_ENLIST_IN_DTC          1207
#define SQL_ATTR_ENLIST_IN_XA           1208
#define SQL_ATTR_LOGIN_TIMEOUT          SQL_LOGIN_TIMEOUT
#define SQL_ATTR_ODBC_CURSORS           SQL_ODBC_CURSORS
#define SQL_ATTR_PACKET_SIZE            SQL_PACKET_SIZE
#define SQL_ATTR_QUIET_MODE                     SQL_QUIET_MODE
#define SQL_ATTR_TRACE                          SQL_OPT_TRACE
#define SQL_ATTR_TRACEFILE                      SQL_OPT_TRACEFILE
#define SQL_ATTR_TRANSLATE_LIB          SQL_TRANSLATE_DLL
#define SQL_ATTR_TRANSLATE_OPTION       SQL_TRANSLATE_OPTION
#define SQL_ATTR_TXN_ISOLATION          SQL_TXN_ISOLATION
#define SQL_ASYNC_ENABLE                4       /* same as SQL_ATTR_ASYNC_ENABLE */     
#define SQL_ATTR_ASYNC_ENABLE                           4
#define SQL_ATTR_CONCURRENCY                            SQL_CONCURRENCY
#define SQL_ATTR_CURSOR_TYPE                            SQL_CURSOR_TYPE
#define SQL_ATTR_ENABLE_AUTO_IPD                        15
#define SQL_ATTR_FETCH_BOOKMARK_PTR                     16
#define SQL_ATTR_KEYSET_SIZE                            SQL_KEYSET_SIZE
#define SQL_ATTR_MAX_LENGTH                                     SQL_MAX_LENGTH
#define SQL_ATTR_MAX_ROWS                                       SQL_MAX_ROWS
#define SQL_ATTR_NOSCAN                                         SQL_NOSCAN
#define SQL_ATTR_PARAM_BIND_OFFSET_PTR          17
#define SQL_ATTR_PARAM_BIND_TYPE                        18
#define SQL_ATTR_PARAM_OPERATION_PTR            19
#define SQL_ATTR_PARAM_STATUS_PTR                       20
#define SQL_ATTR_PARAMS_PROCESSED_PTR           21
#define SQL_ATTR_PARAMSET_SIZE                          22
#define SQL_ATTR_QUERY_TIMEOUT                          SQL_QUERY_TIMEOUT
#define SQL_ATTR_RETRIEVE_DATA                          SQL_RETRIEVE_DATA
#define SQL_ATTR_ROW_BIND_OFFSET_PTR            23
#define SQL_ATTR_ROW_BIND_TYPE                          SQL_BIND_TYPE
#define SQL_ATTR_ROW_NUMBER                                     SQL_ROW_NUMBER          /*GetStmtAttr*/
#define SQL_ATTR_ROW_OPERATION_PTR                      24
#define SQL_ATTR_ROW_STATUS_PTR                         25
#define SQL_ATTR_ROWS_FETCHED_PTR                       26
#define SQL_ATTR_ROW_ARRAY_SIZE                         27      
#define SQL_ATTR_SIMULATE_CURSOR                        SQL_SIMULATE_CURSOR
#define SQL_ATTR_USE_BOOKMARKS                          SQL_USE_BOOKMARKS       
#define SQL_ATTR_READONLY               0
#define SQL_ATTR_WRITE                  1
#define SQL_ATTR_READWRITE_UNKNOWN      2

               
#endif
