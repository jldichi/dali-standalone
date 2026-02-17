//- Copyright Notice
// -----------------------------------------------------------------------
// (C) Copyright 1998 InterSoft S.A.  All Rights Reserved
// THIS IS UNPUBLISHED PROPRIETARY SOURCE CODE OF Intersoft S.A.
// The copyright notice above does not evidence any actual or intended 
// publication of such source code.
//
// $Id: odbcmisc.h,v 1.33 1999/05/06 19:39:28 eduardoc Exp $
// -----------------------------------------------------------------------

#ifndef H_ODBCMISC
#define H_ODBCMISC

#include <ifound.h>
#include <cfix.h>
#include <ideafix/priv/dbdefs.h>
#include <sqldb/metadata/fieldmd.h>
#include <sqldb/fwork/sqlmisc.h>
#include <sqldb/fwork/odbc/odbcconn.h>
#include <sqldb/fwork/odbc/odbcstat.h>

#ifdef iODBC
    #include <sqldb/fwork/odbc/isqlext.h>
#endif iODBC

#ifdef INTERSOLV
	#ifdef _CTYPE_DEFINED //chset.h conflicts
		#undef _N
		#undef _S
		#undef _P
		#undef _C
	#endif
	#include <sql.h>
	#ifdef __NT__ 
		#include <mswi/winidea.h>
	#endif __NT__ 
	#include <sqlext.h>
	#ifdef _CTYPE_DEFINED
		#define _N	 04
		#define _S	010
		#define _P	020
		#define _C	040
	#endif
#endif INTERSOLV

// errors
#define ODBCERR_COULD_NOT_CONNECT   					-1 
#define ODBCERR_TRUNCATED_PARAMS    					-2
#define ODBCERR_RECORD_ERROR    						-3   
#define ODBCERR_NO_RECORD_DEL_UPD   					-4 
#define ODBCERR_MORE_THAN_A_ROW_DEL_UPD  				-5  
#define ODBCERR_WRONG_PARAM_NUMBER  					-6     
#define ODBCERR_CONNECTION_ALREADY_OPENED   			-7   
#define ODBCERR_CONNECTION_NOT_OPENED   				-8      
#define ODBCERR_BROKEN_CONNECTION  						-9         
#define ODBCERR_INVALID_PARAMETER     					-10       
#define ODBCERR_TIMEOUT 								-11                  
#define ODBCERR_CONSTRAINT_CHECK_FAILED 				-12 
#define ODBCERR_INV_USER_OR_PASSWD   					-13        
#define ODBCERR_WRONG_PARAMETER_NUMBER  				-14
#define ODBCERR_UNKNOWN_ERROR   						-15      
#define ODBCERR_COULD_NOT_DISCONNECT					-16
#define ODBCERR_INVALID_CUR_POSITION					-17
#define ODBCERR_RECORD_LOCKED							-18
#define ODBCERR_DATABASE_ALREADY_EXISTS					-19
#define ODBCERR_INVALID_COLUMN_NAME						-20
#define ODBCERR_FK_OVER_NON_UNIQUE_FIELDS				-21
#define ODBCERR_CANT_WRITE_IN_TABLESPACE				-22
#define ODBCERR_TABLE_DOESNT_EXIST						-23
#define ODBCERR_NO_PERMISSIONS							-24
#define	ODBCERR_NAME_ALREADY_EXISTS						-25
#define	ODBCERR_INDEX_DOES_NOT_EXIST					-26
#define	ODBCERR_DUPLICATED_KEYS_FOUND					-27
#define	ODBCERR_CANNOT_USE_COLUMN						-28
#define	ODBCERR_ORACLE_RAN_OUT_OF_ROLLBACK_SEGMENTS		-29
#define ODBCERR_INDEX_ALREADY_EXISTS					-30
#define ODBCERR_INDEX_ALREADY_EXISTS_WITH_SAME_FIELDS	-31
#define	ODBCERR_CANNOT_DROP_NON_EXISTANT_CONSTRAINT		-32

#define ODBC_NULL_DATE "1894-01-01 "
#define ODBC_NULL_TIME " 00:00:00"

//toDo: cambiar esto por algo mas dinamico (chequeo con SQLGetTypeInfo al levantar)
#ifdef ORACLE
	#define TSPREF "{ts \'"
	#define TSSUFF "\' }"
	#define	TSPREFlen 5 //strlen(TSPREF)
	#define TSSUFFlen 3 //strlen(TSSUFF)
#endif ORACLE

#ifdef INFORMIX
	#define TSPREF ""
	#define TSSUFF ""
	#define	TSPREFlen 0
	#define TSSUFFlen 0
#endif INFORMIX
                    

//defines used during timestamp convertion                    
#define TSYEARLEN 4		//length of the year in a sqlTimestamp
#define TSMONTHLEN 2	//length of the month in a sqlTimestamp
#define TSDAYLEN 2  	//length of the day in a sqlTimestamp
#define TSHOURLEN 2		//length of the hour in a sqlTimestamp
#define TSMINLEN 2		//length of the minutes in a sqlTimestamp
#define TSSECLEN 2		//length of the seconds in a sqlTimestamp
#define TSDATESEP "-"	//separator between the components of the date in a timestamp
#define TSTIMESEP ":" 	//separator between the components of the time in a timestamp
#define TSDTSEP " "		//separator between the date and the time in a timestamp
#define TSDATELEN 19	//length of the complete string (without prefix and suffix) (yyyy-mm-dd hh:mm:ss)
#define DSEP "/"		//separator between the components of the Ideafix date
#define DLEN 10     	//length of a date string (dd/mm/yyyy)
#define DYEARLEN 4  	//length of the year in a date
#define DMONTHLEN 2		//length of the month in a date
#define DDAYLEN 2		//length of the day in a date
#define TSEP ":"		//separator between the components of the Ideafix time
#define THOURLEN	2	//length of the hour in a time 
#define TMINLEN	2		//length of the minutes in a time
#define TSECLEN	2		//length of the seconds in a time 
#define DATEBUFF	32 // length of buffer date

#define TIMESTAMP_LENGTH	TSDATELEN+TSPREFlen+TSSUFFlen
                    
// Miscelanous functions 

struct SqlTypeInfo {
		int 	data_type;
		char 	*type_name;
		int	precision;
		int	length;
	};	              
	
#ifdef __cplusplus
extern "C" {
#endif

Int calcPrecision(const FieldMetadata &fm);
Int calcLength(const FieldMetadata &fm);
Int calcScale(const FieldMetadata &fm);
Int calcRadix(const FieldMetadata &fm);
Int mapDefaultTy(const FieldMetadata &fm);
String SQLTypeToString(sqltype ty);
const String getSQLType(SWORD type);
const String getScale(SWORD scale);
const String getPrecision(UDWORD precision);
const String GetNullable(SWORD nullable);
void Translate(const ODBCConnectionImpl &conn, const char *opName,
				RETCODE ret, HSTMT=SQL_NULL_HSTMT);
sqltype SqlTyToSql(Int ty);
Int TyToSqlTy(type t);
String dateToSqlTimestamp(const char * date);
String sqlTimestampToDate(const char * sqlTimestamp);
String timeToSqlTimestamp(const char * time);
String sqlTimestampToTime(const char * sqlTimestamp);
String errorToString(Int error);
Int getError(const ODBCConnectionImpl *conn, HSTMT hstmt=SQL_NULL_HSTMT);
char * getErrorMsg();
Int * getNullDataPtr();

#ifdef __cplusplus
}
#endif

#endif //H_ODBCMISC
