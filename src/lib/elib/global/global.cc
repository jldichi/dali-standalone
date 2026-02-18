/********************************************************************
* Copyright (c) 2026 Master_Solutions S.R.L.  All Rights Reserved
* THIS IS UNPUBLISHED PROPRIETARY SOURCE CODE OF Master_Solutions S.R.L.
* The copyright notice above does not evidence any
* actual or intended publication of such source code.
*
* $Id: global.cc,v 1.2 1998/11/19 13:31:17 ernestod Exp $
*
* DESCRIPTION
* Global variable declarations
*********************************************************************/

#include <local/strfile.h>
#include <ifound/anyvalue.h>

// standard stream to access stdout, stderr and stdin
OutputFdFile stdOut(1), stdErr(2);
InputFdFile stdIn(0);
OutputStream *stdMsg_address = &stdErr;
