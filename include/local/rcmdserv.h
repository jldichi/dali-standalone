/********************************************************************
*
* Copyright (c) 1995 InterSoft Co.  All Rights Reserved
* THIS IS UNPUBLISHED PROPRIETARY SOURCE CODE OF InterSoft Co.
* The copyright notice above does not evidence any
* actual or intended publication of such source code.
*
* $Id: rcmdserv.h,v 1.1.1.1 1996/04/16 18:51:50 gustavof Exp $
*
* DESCRIPTION:
*              Services used by RemoteCommand and RemoteExecD to
*              connect throw.
*
*********************************************************************/
#ifndef RCMDSERV_H
#define RCMDSERV_H

#ifdef HAVE_INETD
#define RCMD_SERVICE "iexec"
#define RCMD_DBG_SERVICE "iexec-deb"
#else
#define RCMD_SERVICE "iexec/7140"
#define RCMD_DBG_SERVICE "iexec-deb/7150"
#endif

#endif //RCMDSERV_H
