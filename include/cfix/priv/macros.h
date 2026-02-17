/*
// --------------------------------------------------------------------
// Copyright (c) 1995 InterSoft Co.  All Rights Reserved
// THIS IS UNPUBLISHED PROPRIETARY SOURCE CODE OF InterSoft Co.
// The copyright notice above does not evidence any actual or intended 
// publication of such source code.
//
// $Id: macros.h,v 1.3 1996/12/02 16:09:06 eduardoc Exp $
//
// Description:
// Date/time conversion header
// --------------------------------------------------------------------
*/

#ifndef CFIX_PRIV_MACROS_H
#define CFIX_PRIV_MACROS_H

#include <cfix/types.h>

/* macros for compatibility between ideafix and metafix */
#define STIMETOL(t)	((long) (((unsigned short) (t)) == MIN_TIME ? INT_MIN : \
							((t)-1) << 1))
#define LTIMETOS(t)	(unsigned short)(((long) (t)) == LONG_MIN ? MIN_TIME : \
			 	 			((long) (t)) == LONG_MAX ? MAX_TIME : \
							(((t)%86400 >> 1) + 1))
#define SDATETOL(t)	((long) (((short) (t)) == MIN_DATE ? INT_MIN : (t)))
#define LDATETOS(t)	((short) (((long) (t)) <= MIN_DATE ? MIN_DATE : \
			 	 ((long) (t)) >= MAX_DATE ? MAX_DATE : (t)))

#endif
