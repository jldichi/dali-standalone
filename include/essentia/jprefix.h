/********************************************************************
* Copyright (c) 1992 InterSoft Co.  All Rights Reserved
* THIS IS UNPUBLISHED PROPRIETARY SOURCE CODE OF InterSoft Co.
* The copyright notice above does not evidence any
* actual or intended publication of such source code.
*
* $Id: jprefix.h,v 1.1.1.1 1996/04/16 18:51:48 gustavof Exp $
*
* DESCRIPTION
* Structure definition for journal file headers
*********************************************************************/

#ifndef JPREFIX_H
#define JPREFIX_H

#include <ifound.h>
#include <ifound/str.h>

// +++ Config +++
#define JRNL_FILE_LEN	31
#define JRNL_MAGIC		0x7F832BC1
// --- Config ---

struct JournalPrefix {
	Int magic;
	Int serverId;
	Int closed;
	Int flags;
	char next[JRNL_FILE_LEN+1];
	char prev[JRNL_FILE_LEN+1];
};

#endif
