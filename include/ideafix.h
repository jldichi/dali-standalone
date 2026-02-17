/********************************************************************
* Copyright (c) 1987 InterSoft Co.  All Rights Reserved
* THIS IS UNPUBLISHED PROPRIETARY SOURCE CODE OF InterSoft Co.
* The copyright notice above does not evidence any
* actual or intended publication of such source code.
*
* $Id: ideafix.h,v 1.12 2006/07/11 15:38:49 hardaiz Exp $
*
* DESCRIPTION
* Ideafix API
*********************************************************************/

#ifndef IDEAFIX_H
#define IDEAFIX_H

/* Macros to define IdeaFix version and release date */
#define IDEA_VERSION    	610		
#define IDEA_VERSION_STRING	"6.1.0"
#define IDEA_RELEASE_DATE	"15/02/2016"

/* typedef ixBool Bool; */

#ifndef __cplusplus

/* "C" User API */

#	include <ifound/osdep.h>
#	include <ifound/macros.h>
#	include <ifound/istdarg.h>
#	include <ifound/functype.h>
#	include <ifound/chset.h>

#	ifndef BUFSIZ
#		include <stdio.h>
#	endif

#	include <cfix.h>

#else

// "C++" User API

#	include <ifound.h>
#	include <ifound/itypes.h>
#	include <ifound/msgtable.h>
#	include <ifound/message.h>

#if 0
#	include <ideafix/dbm.h>
#endif
#ifdef WXFM
#	include <ideafix/fm.h>
#else
#	include <ideafix/form.h>
#endif
#	include <ideafix/report.h>

// just to be used by Ideafix 4.x programs
#	include <cfix.h>

#endif

// @DIEGOS 2006-04-25 (17:29)
// Si estoy utilizando Ideanet x Telnet
#ifdef IXSCA
#include <ideafix/priv/c_ixsca.h>
#endif

#endif 
