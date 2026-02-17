/********************************************************************
* Copyright (c) 1987 InterSoft Co.  All Rights Reserved
* THIS IS UNPUBLISHED PROPRIETARY SOURCE CODE OF InterSoft Co.
* The copyright notice above does not evidence any
* actual or intended publication of such source code.
*
* $Id: c_errmsg.cc,v 1.2 1998/09/04 18:39:06 mauro Exp $
*
* DESCRIPTION
*
* ROUTINE       |  PURPOSE
*---------------+---------------------------------------------------
* _GnErrmsg()
* _DbErrmsg()
* _FmErrmsg()
* _GnErrmsg()
* _RpErrmsg()
* _StErrmsg()
* _WiErrmsg()
*********************************************************************/

#include <ideafix.h>
#include <cfix.h>
#include <ifound/msgtable.h>
#include <ifound/str.h>

#ifdef IDEAWIN
#	include <mswi/winidea.h>
#endif

#define POOL_LEN	10
static String &msgTab()
{
	static String *msgTab_ = NULL;
	static Int offset = 0;
	
	if (msgTab_ == NULL)
		msgTab_ = tr_new String[POOL_LEN];

	String &ret = msgTab_[offset];
	offset = (offset + 1) % POOL_LEN;

	return ret;
}

static String &getMsg(MsgTable *table, const String &message)
{
 	String &aux = msgTab() = table->find(message);
	return aux;
}

extern "C" char *__DbErrmsg(const char *message)
{
	static MsgTable *dbMsg = NULL;
	if (dbMsg == NULL)
		dbMsg = tr_new MsgTable("lib", "db");
 	return (char *) toCharPtr(getMsg(dbMsg, message));
}


extern "C" char *__FmErrmsg(const char *message)
{
	static MsgTable *fmMsg = NULL;
	if (fmMsg == NULL)
		fmMsg = tr_new MsgTable("lib", "fm");
 	return (char *) toCharPtr(getMsg(fmMsg, message));
}

extern "C" char *__GnErrmsg(const char *message)
{
	static MsgTable *gnMsg = NULL;
	if (gnMsg == NULL)
		gnMsg = tr_new MsgTable("lib", "gn");
 	return (char *) toCharPtr(getMsg(gnMsg, message));
}

extern "C" char *__RpErrmsg(const char *message)
{
	static MsgTable *rpMsg = NULL;
	if (rpMsg == NULL)
		rpMsg = tr_new MsgTable("lib", "rp");
 	return (char *) toCharPtr(getMsg(rpMsg, message));
}

extern "C" char *__StErrmsg(const char *message)
{
	static MsgTable *stMsg = NULL;
	if (stMsg == NULL)
		stMsg = tr_new MsgTable("lib", "st");
 	return (char *) toCharPtr(getMsg(stMsg, message));
}

extern "C" char *__WiErrmsg(const char *message)
{
	static MsgTable *wiMsg = NULL;
	if (wiMsg == NULL)
		wiMsg = tr_new MsgTable("lib", "wi");
 	return (char *) toCharPtr(getMsg(wiMsg, message));
}
