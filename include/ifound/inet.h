/****************************************************************
* Copyright (c) 1992 InterSoft Co.  All Rights Reserved
* THIS IS UNPUBLISHED PROPRIETARY SOURCE CODE OF InterSoft Co.
* The copyright notice above does not evidence any
* actual or intended publication of such source code.
*
* $Id: inet.h,v 1.1.1.1 1996/04/16 18:51:32 gustavof Exp $
*
* DESCRIPTION
* Class definitions for Local Connection type.
*********************************************************************/

#ifndef INET_H
#define INET_H

#include <ifound.h>

#ifdef __GNUC__
#pragma interface
#endif

/*
 * Definitions for TCP client/server progs.
 */
#include <stdio.h>
#include <fcntl.h>
#include <sys/types.h>

#ifdef HAVE_STD_TCP_HEADERS
#	include <sys/socket.h>
#	include <netinet/in.h>
#endif

#define DEV_TCP			"/dev/tcp"

#define MAXLINE			2048

#define TESTNUM			12345678

#endif	// INET_H
