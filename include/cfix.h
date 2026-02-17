/********************************************************************
* Copyright (c) 1987 InterSoft Co.  All Rights Reserved
* THIS IS UNPUBLISHED PROPRIETARY SOURCE CODE OF InterSoft Co.
* The copyright notice above does not evidence any
* actual or intended publication of such source code.
*
* $Id: cfix.h,v 1.3 2002/04/04 16:02:55 hardaiz Exp $
*
* DESCRIPTION
* Ideafix "C" interface main header module.
*********************************************************************/

#ifndef CFIX_H
#define CFIX_H

#include <cfix/types.h>

#include <ifound/num.h>
#include <ifound/stdc.h>

/* IDEAFIX include files */
#include <cfix/gn.h>
#include <cfix/st.h>
#include <cfix/tm.h>
#include <cfix/db.h>
#include <cfix/rp.h>
#ifdef IDEAWIN
#	include <mswi/iwwi.h>
#else
#	include <ixwi/wi.h>
#endif
#include <cfix/fm.h>
#include <cfix/ixsqlcur.h>
#include <cfix/wif.h>

#endif
