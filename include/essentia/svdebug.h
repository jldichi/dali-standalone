/********************************************************************
* Copyright (c) 1993 InterSoft Co.  All Rights Reserved
* THIS IS UNPUBLISHED PROPRIETARY SOURCE CODE OF InterSoft Co.
* The copyright notice above does not evidence any
* actual or intended publication of such source code.
*
* $Id: svdebug.h,v 1.1.1.1 1996/04/16 18:51:48 gustavof Exp $
*
* DESCRIPTION:
*
*********************************************************************/

#if 0
#ifndef SVDEBUG_H
#define SVDEBUG_H

#ifdef ISDEBUG

/* +++ No Tags +++ */

#define	Coroutine		_D_Coroutine
#define SvConnection	_D_SvConnection
#define RunLog			_D_RunLog
#define SvExpr			_D_SvExpr
#define SvField			_D_SvField
#define SvInList		_D_SvInList
#define SvIntegrity		_D_SvIntegrity
#define SvIndFld		_D_SvIndFld
#define IndexNode		_D_IndexNode
#define SvConfigFile	_D_SvConfigFile
#define Transaction		_D_Transaction
#define LockObject		_D_LockObject
#define NotifyLock		_D_NotifyLock
#define Journal			_D_Journal
#define BlockedFile		_D_BlockedFile
#define GServObj		_D_GServObj
#define Client			_D_Client
#define CacheBuffer		_D_CacheBuffer

#define COROUTINE_ID	0x8AB96453
#define SVCONNECTION_ID	0xE0CC2880
#define RUNLOG_ID		0x87C2987E
#define SVEXPR_ID		0xABC79876
#define SVFIELD_ID		0xABE8EF89
#define SVINLIST_ID		0x97FAE782
#define SVINTEGRITY_ID	0x4983BEBA
#define SVINDFLD_ID		0xA819F411
#define INDEXNODE_ID	0xADE1290F
#define SVCONFIGFILE_ID	0xCBEA45B6
#define TRANSACTION_ID	0xAD56CB19
#define LOCKOBJECT_ID	0xAF6734CB
#define NOTIFYLOCK_ID	0x98ACF5E2
#define JOURNAL_ID		0x9AB471F4
#define BLOCKEDFILE_ID	0xA8CA111B
#define GSERVOBJ_ID		0xEFAB824A
#define CLIENT_ID		0x67ACB178
#define CACHEBUFFER_ID	0x344444A8

/* --- No Tags --- */

#endif	// ISDEBUG

#endif	// SVDEBUG_H
#endif
