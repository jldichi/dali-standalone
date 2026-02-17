//- Copyright Notice
// -----------------------------------------------------------------------
// (C) Copyright 1998 InterSoft S.A.  All Rights Reserved
// THIS IS UNPUBLISHED PROPRIETARY SOURCE CODE OF InterSoft S.A.
// The copyright notice above does not evidence any actual or intended 
// publication of such source code.
//
// $Id: mdmisc.h,v 1.19 2006/05/19 21:05:24 cristiand Exp $
// -----------------------------------------------------------------------

#ifndef	MDMISC_H
#define MDMISC_H

#include <ifound.h>
#include <ifound/str.h>
#include <cfix.h>

#define	METADATA_SCHEMA_NAME		"metadata"

#define	MAX_DESCR_LENGTH			64
#define	COMPLETE_INDEX_POSITION		1

#define	SELECT_THIS_KEY				"="
#define	SELECT_NEXT_KEY				">"
#define	SELECT_THIS_KEY_NEXT_KEY	">="
#define	SELECT_PREV_KEY				"<"
#define	SELECT_THIS_KEY_PREV_KEY	"<="
#define SELECT_FIRST_KEY			"<<"
#define SELECT_LAST_KEY             ">>"
#define	UPDATE_THIS_KEY				"="
#define	DELETE_THIS_KEY				"="

/**
 * Enumeration for GetRecord modes.
 */
enum GRMode {
	GR_THIS_KEY	= 0,
	GR_NEXT_KEY,
	GR_PREV_KEY,
	GR_THIS_KEY_PREV_KEY,
	GR_THIS_KEY_NEXT_KEY,
	GR_FIRST_KEY,
	GR_LAST_KEY,
	GR_GET_KEY_FIELDS
};

/**
 * Enumeration for Locking modes.
 */
enum LockMode {
	LOCK	= 0,
	LOCK_TEST,
	NO_LOCK
};

//Agregamos estos #undef para evitar conflictos con db.h

#undef ORACLE_DBMS
#undef INFORMIX_DBMS
#undef POSTGRE_DBMS
#undef UNKNOWN_DBMS

/**
* This enum is used to determine the DBMS being used.
*/

enum DbmsType {
	UNKNOWN_DBMS = -1,
	INFORMIX_DBMS = 0,
	ORACLE_DBMS = 1,
	POSTGRE_DBMS = 2
};

/** 
 * Enumeration for isolation levels
 */
enum IsoLevel {  
	DIRTY_READ = 0,
	COMMITTED_READ, 
	CURSOR_STABILITY,
	REPEATABLE_READ
};  //toDO: generalizar????

/*
 * Enumeration for the storing mode in metadata DB.
 */
enum StoreMode {
	TAB_NEW = 0,
	TAB_UPDATE,
	IND_NEW,
	IND_UPDATE,
	FIELD_NEW,
	FIELD_UPDATE,
	IFIELD_NEW,
	IFIELD_UPDATE,
	PERM_NEW,
	PERM_UPDATE,
	NO_MODE
};

/**
 * Enumeration for the table flags.
 */
enum TableFlag {
	TABLE_NORMAL = 0,
	TABLE_TEMPORARY
};

/**
 * Enumeration for the exceptions that can occur when the cursor buffer 
 * is being loaded.
 */
enum Exceptions {
	EXC_LOCKED = -2,
	EXC_ERR = -1,
	EXC_OK = 0
};    
       
extern "C" bool indexesBelongToSameTable(dbindex ind1, dbindex ind2);
extern "C" String printFieldFlags(field_flags ff);
extern "C" String printPerms(Int priv, bool sch);

#endif	// MDMISC_H
