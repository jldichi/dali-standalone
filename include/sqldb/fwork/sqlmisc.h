//- Copyright Notice
// -----------------------------------------------------------------------
// (C) Copyright 1998 InterSoft S.A.  All Rights Reserved
// THIS IS UNPUBLISHED PROPRIETARY SOURCE CODE OF InterSoft S.A.
// The copyright notice above does not evidence any actual or intended 
// publication of such source code.
//
// $Id: sqlmisc.h,v 1.13 2002/03/19 21:49:59 hardaiz Exp $
// -----------------------------------------------------------------------

#ifndef	SQLMISC_H
#define	SQLMISC_H

#include <ifound.h>
#include <cfix.h>
#include <ideafix/priv/dbdefs.h>

#define	CONSTRAINT_POSTFIX	"_UQ"
#define	DATE_MAX_LEN		10
#define	TIME_MAX_LEN		8
#define	BOOL_MAX_LEN		1
#define	FLOAT_MAX_LEN		32
#define	MIN_ACCEPTABLE_DOUBLE_VALUE	-1e49
#define	MAX_ACCEPTABLE_DOUBLE_VALUE	1e49

#define	NULL_CHAR_STRING	"!"
#define	NULL_FLOAT_VALUE	(-1e30 - 1)
#define	NULL_DATE_STRING	"15/04/1894"
#define	NULL_TIME_STRING	"00:00:00"
#define	NULL_BOOL_STRING	"-1"

class FieldMetadata;

#ifdef __cplusplus
extern "C" {
#endif  // __cplusplus

char *CFIXInternalTypeToCharPtr(itype ty);
char *CFIXTypeToCharPtr(type ty);
char *CFIXSQLTypeToCharPtr(sqltype ty);
Int getDisplaySize(const FieldMetadata &fmd);
AnyValue *getAnyValue(const FieldMetadata &fmd, const char *nnv, DateFmt fmt);
AnyValue *getNullAnyValue(const FieldMetadata *fmd);
AnyValue *getMinAnyValue(const FieldMetadata *fmd);
AnyValue *getMaxAnyValue(const FieldMetadata *fmd);
AnyValue *deleteDecimalPeriod(const AnyValue &av, const FieldMetadata &fmd);
void insertDecimalPeriod(AnyValue *&av, const FieldMetadata &fmd);
Num getMinNumeric(Int len, Int ndec);
Num getMaxNumeric(Int len, Int ndec);

#ifdef __cplusplus
}
#endif  // __cplusplus

#endif	// SQLMISC_H
