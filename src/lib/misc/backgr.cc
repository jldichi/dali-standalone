/********************************************************************
* Copyright (c) 1987 InterSoft Co.  All Rights Reserved
* THIS IS UNPUBLISHED PROPRIETARY SOURCE CODE OF InterSoft Co.
* The copyright notice above does not evidence any
* actual or intended publication of such source code.
*
* $Id: backgr.cc,v 1.2 2004/09/29 17:23:06 damianm Exp $
*
* DESCRIPTION
* Go to Background work.
*********************************************************************/

#include <unistd.h>
#include <stdlib.h>

#include <ifound.h>
#include <ifound/stdc.h>
#include <ifound/priv/pconfig.h>

void background()
{
#if HAVE_PROC
	int n;
	if (_processConfig()->background || getenv("DEBUG") != (char *) NULL)
		return;
	n = fork();
	if (n == ERR)
		return;
	if (n) {
// resettty();
		_global_exit(0);
	}
	_processConfig()->background = true;
#endif
}
