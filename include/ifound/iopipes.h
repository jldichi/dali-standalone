/********************************************************************
* Copyright (c) 1991 InterSoft Co.  All Rights Reserved
* THIS IS UNPUBLISHED PROPRIETARY SOURCE CODE OF InterSoft Co.
* The copyright notice above does not evidence any
* actual or intended publication of such source code.
*
*
* $Id: iopipes.h,v 1.1.1.1 1996/04/16 18:51:32 gustavof Exp $
*
*
* DESCRIPTION:
*				Interface to IPC pipes
*
*********************************************************************/


#ifndef	IOPIPES_H
#define	IOPIPES_H

#include <ifound.h>

#ifdef __GNUC__
#pragma interface
#endif



#ifndef	STR_H
#include <ifound/str.h>
#endif

Int ioPipe(String cmd, int *read_fd, int *write_fd, 
		   bool shell_flag = true, bool stderr_flag = true);

#endif		//	IOPIPES_H
