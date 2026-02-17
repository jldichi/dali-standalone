/********************************************************************
* Copyright (c) 1994 InterSoft Co.  All Rights Reserved
* THIS IS UNPUBLISHED PROPRIETARY SOURCE CODE OF InterSoft Co.
* The copyright notice above does not evidence any
* actual or intended publication of such source code.
*
* $Id: ifound.h,v 1.3 1998/09/04 15:00:23 mauro Exp $
*
* DESCRIPTION: Main InterSoft Foundation header file.
*********************************************************************/

#ifndef IFOUND_H
#define IFOUND_H

#include <ifound/osdep.h>

#if defined(__VC__) && !defined(IDEAWIN)
#	include <math.h>
#endif
	

#include <ifound/istdarg.h>
#include <ifound/functype.h>
#include <ifound/macros.h>

#ifndef BUFSIZ
#	include <stdio.h>
#endif

#include <ifound/btypes.h>
#include <ifound/gfuncs.h>
#include <ifound/chset.h>
#include <ifound/parray.h>
#include <ifound/resource.h>
#include <ifound/ftrace.h>

#endif
