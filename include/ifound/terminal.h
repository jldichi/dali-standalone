/********************************************************************
* Copyright (c) 1991 InterSoft Co.  All Rights Reserved
* THIS IS UNPUBLISHED PROPRIETARY SOURCE CODE OF InterSoft Co.
* The copyright notice above does not evidence any
* actual or intended publication of such source code.
*
*
* $Id: terminal.h,v 1.1.1.1 1996/04/16 18:51:33 gustavof Exp $
*
*
* DESCRIPTION:
*				Terminal management function prototypes
*
*********************************************************************/

#ifndef	TERMINAL_H
#define	TERMINAL_H

#include <ifound.h>

#ifdef __GNUC__
#pragma interface
#endif

#ifdef HAVE_TTY

void setTerminal();
void resetTerminal();
bool terminalIsSet();
char getChar();

#endif		// HAVE_TTY

#endif		// TERMINAL_H
