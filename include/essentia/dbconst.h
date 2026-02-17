/********************************************************************
* Copyright (c) 1992 InterSoft Co.  All Rights Reserved
* THIS IS UNPUBLISHED PROPRIETARY SOURCE CODE OF InterSoft Co.
* The copyright notice above does not evidence any
* actual or intended publication of such source code.
*
* $Id: dbconst.h,v 1.5 2000/09/17 20:30:40 smartino Exp $
*
* DESCRIPTION
* Constants for db library
*********************************************************************/

#ifndef DBCONST_H
#define DBCONST_H

#include <ifound.h>

#ifdef __GNUC__
#pragma interface
#endif

struct DbFlags {
	enum {
		io_not_lock		= 0,
		io_lock 		= 1 << 0,
		io_test			= 1 << 1,
		io_read			= 1 << 2,
		io_write		= 1 << 3,
		io_notify		= 1 << 4,
		io_eabort		= 1 << 5,
		io_verbose		= 1 << 6,
		this_key	 	= 1 << 7,
		next_key	  	= 1 << 8,
		prev_key	  	= 1 << 9,
		first_key	 	= 1 << 10,
		last_key	 	= 1 << 11,
		partial_key	 	= 1 << 12,
		io_plock		= 1 << 13,
		io_nobinary		= 1 << 14
	};
};

enum IndFlags {
	IND_DUP			= 1 << 0,
	IND_UNIQUE		= 1 << 1,
	IND_HAS_DESC	= 1 << 2,
	IND_HAS_NNULL	= 1 << 3,

	// Everybody's modifications update this index.
	IND_PUBLIC		= 1 << 4,

	// Independent (does not include primary key and is unique)
	IND_INDEP		= 1 << 5,

	// This kind of index is used when we have less keys than in the
	// table.
	IND_INC			= 1 << 6,
};

enum IndFldFlags {
	IND_ASCENDING   = 1 << 0,
	IND_DESCENDING  = 1 << 1,
	IND_NOT_NULL    = 1 << 2,
};

enum TabFlags {
	TAB_CREATE_STAMP	= 1 << 0,
	TAB_PCREATE_STAMP	= 1 << 1,
	TAB_POID			= 1 << 2,
	TAB_HAS_INDEP_KEY	= 1 << 3,
};

enum TabFldFlags {
	TAB_FLD_NOT_NULL		= 1 << 0,
	TAB_FLD_VIRTUAL			= 1 << 1,
	// tiene que hacerse de otra forma ... (expresiones ?)
	TAB_FLD_MODIF			= 1 << 2,
	TAB_FLD_CREATE			= 1 << 3,
	TAB_FLD_CDIGIT			= 1 << 4,
	TAB_FLD_CDIGIT_SLASH	= 1 << 5,
	TAB_FLD_CDIGIT_DASH		= 1 << 6,
};

enum Objects {
	SERVER 		= 1 << 0,
	SHADOW 		= 1 << 1,
	INDEX  		= 1 << 3,
	SCHEMA 		= 1 << 4,
	TABLE  		= 1 << 5,
	CURSOR 		= 1 << 6,
	SHOBJECT 	= 1 << 7,
	SHTUBE		= 1 << 8,
	COPYTABLE	= 1 << 9,
	NAMESPACE	= 1 << 10,
	IQLSRV      = 1 << 11,
};

enum CheckPointType {
	CCCP		= 0,
	TCCP		= 1 << 0,
	FORCE_CP	= 1 << 1,
};

enum CheckPointResult {
	// Do not use bit 0 (reserved for currCP)!
	CPR_CORRUPT		= 1 << 1,
	CPR_IDXCORRUPT	= 1 << 2,
	CPR_COMPLETE	= 1 << 3,
	CPR_QUEUED		= 1 << 4,
};

enum FldAttribType { 				// Should be renamed to AttribType !!
	FLD_INCHECK		= 1 << 0,
	FLD_EXPR		= 1 << 1,
	FLD_DEFAULT		= 1 << 2,
	FLD_COLLSET		= 1 << 3,
	FLD_TABEXPR		= 1 << 4,		// > 431
};

enum DataFilesType {
	FILESDATA		= 1 << 0,
	STATEDATA		= 1 << 1,
	CNFDATA			= 1 << 2,
	JRNLDATA		= 1 << 3,
	DCONFIGDATA		= 1 << 4,   // Appeared in version >= 431
	ALLDATA			= FILESDATA|STATEDATA|CNFDATA|JRNLDATA|DCONFIGDATA,
};

enum ServerOptions {
	OPT_BACKUP		= 1 << 0,
	OPT_MIRRORING	= 1 << 1,
	OPT_TCORRTOL	= 1 << 2,
};

enum SetPrivgOptions {
	SPR_NONE		= 0,
	SPR_SCHEMA		= 1,
	SPR_TABLE		= 2,
	SPR_COPYTABLE	= 3,
	SPR_OWNER		= 4,
};

enum AlterFlags {
	ALTER_MOD		= 0,
	ALTER_NO_MOD	= 1,
};

enum UpdateOperation {
	UPDT_ADD		= 0,
	UPDT_SUB		= 1,
	UPDT_MUL		= 2,
	UPDT_DIV		= 3,
	UPDT_SET		= 4,
	UPDT_COPY		= 5,
};

enum CursorFlags {
	C_RANGE_GT		= (1 << 0),
	C_RANGE_GE		= (1 << 1),
	C_RANGE_LT		= (1 << 2),
	C_RANGE_LE		= (1 << 3),
	C_CONTROL_BREAK	= (1 << 4),
	C_RESTART		= (1 << 5),
	C_SEND_RANGE	= (1 << 6),
	C_GO_LAST		= (1 << 7),
	C_TIMEOUT		= (1 << 8),
	C_KEY_FIELDS	= (1 << 9),
	C_DESCENDING	= (1 << 10),
	C_KEEP_LOCKS	= (1 << 11),
	C_SCROLL_CURSOR	= (1 << 12),
};

#endif // DBCONST_H
