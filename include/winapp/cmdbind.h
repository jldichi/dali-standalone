/********************************************************************
* Copyright (c) 1993 InterSoft Co.  All Rights Reserved
* THIS IS UNPUBLISHED PROPRIETARY SOURCE CODE OF InterSoft Co.
* The copyright notice above does not evidence any
* actual or intended publication of such source code.
*
* $Id: cmdbind.h,v 1.1.1.1 1996/04/16 18:51:43 gustavof Exp $
*
* DESCRIPTION: Class definitions for class CmdBinding. 
*********************************************************************/

#ifndef CMDBIND_H
#define CMDBIND_H

#ifdef __GNUC__
#pragma interface
#endif

#include <ifound.h>

// maxima cantidad de comandos
#define MAX_COMMAND	255

// Key translation table
#define ALTER_KEY		-2
#define	MENU_KEY		-3
#define	END_KEY			-4
#define	HELP_KEY		-5

// Macros to compose to keys
#define COMPOSE(a,b)	(((a) << 8) | (b))
#define	EVENTMASK		0x8000
#define MENUEVENT(a)	(EVENTMASK|a)

// descompone teclas
#define LCHAR(c)		((c) & 0xff00)
#define RCHAR(c)		((c) & 0x00ff)

typedef	Int CmdId;
typedef	Int EventId;

// We use a hash-table with Int as key and Int as content
// declare2(HashTab,Int,Int);
// declare2(HashTabCursor,Int,Int);
// class CmdBinding : public HashTab(Int,Int) {

class CmdBinding {
// private:

	// tabla de traduccion de key
	void *tcmds;

	// busca la key el la tcmds
    void operator=(const CmdBinding&) ;
    CmdBinding(const CmdBinding&);

public:
	CmdBinding();
	~CmdBinding();

	// Command mapping
	void bind(EventId evid, CmdId cmdid);
	void bindMenu(const String &key);
	
	// Get command Id to search in the Command table
	CmdId getCmdId(EventId eid) const;
};

#endif

