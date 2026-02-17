/********************************************************************
*
* Copyright (c) 1995 InterSoft Co.  All Rights Reserved
* THIS IS UNPUBLISHED PROPRIETARY SOURCE CODE OF InterSoft Co.
* The copyright notice above does not evidence any
* actual or intended publication of such source code.
*
* $Id: clmon.h,v 1.1.1.1 1996/04/16 18:51:40 gustavof Exp $
*
* DESCRIPTION:
*
     Returns methods amount between program and essentia (dbmon like).
*
*********************************************************************/
#ifndef  CLMON_H
#define  CLMON_H

#include <ifound.h>
#include <essentia/dbconn.h>

#define ALL_SERVERS NULL_STRING

class ClientMonitor {

//private:
	DbConnection *head;
	String methodsByServer_;

public:	
	inline ClientMonitor();
    
	Int nMethods(DbConnection *conn);
	const char *nMethods(const String &srv = ALL_SERVERS);

	inline Int nMessages(const String &); // No way, yet ...
};

#include <ideafix/priv/clmon.icc>

#endif //CLMON_H
