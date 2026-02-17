/********************************************************************
* Copyright (c) 1996 InterSoft Co.  All Rights Reserved
* THIS IS UNPUBLISHED PROPRIETARY SOURCE CODE OF InterSoft Co.
* The copyright notice above does not evidence any
* actual or intended publication of such source code.
*
* $Id: c_msg.cc,v 1.3 2004/09/29 16:45:43 damianm Exp $
*
* DESCRIPTION:
*
* ROUTINE       |  PURPOSE
*---------------+---------------------------------------------------
*********************************************************************/

#include <string.h>

#include <ideafix.h>
#include <ifound/msgtable.h>
#include <ifound/priv/gfuncs.h>

#define MAX_ARGS		10
#define MAX_MSG_LENGTH	256

extern "C" const char *GetMessage(const char *module, const char *section,
							   	  const char *msg)
{
	static char retMsg[MAX_MSG_LENGTH+1] = { '\0' };	
	String message = MsgTable(module, section, true).find(msg);   
	if (message == HIGH_VALUE)
		return NULL;
	strncpy(retMsg, toCharPtr(message), MAX_MSG_LENGTH);
	retMsg[MAX_MSG_LENGTH] = '\0';
	return retMsg;
}

static String resolveMsg(const String &module, const String &section, 
						 const String &msg, va_list ap)
{
	String message = MsgTable(module, section, true).find(msg);
	if (message == HIGH_VALUE)
		return NULL_STRING;
	return toStr(message, ap);
}

extern "C" void MessageDisplay(const char *module, const char *section, 
							   const char *label, const char *msg, ...)
{
	va_list ap;
	va_start(ap, msg);

	display(label, resolveMsg(module, section, msg, ap));

	va_end(ap);
}

extern "C" void MessageDisplayError(const char *module, const char *section, 
									const char *msg, ...)
{
	va_list ap;
	va_start(ap, msg);

	displayError(resolveMsg(module, section, msg, ap));

	va_end(ap);
}

extern "C" void MessageFatalError(const char *module, const char *section, 
								  const char *msg, ...)
{
	va_list ap;
	va_start(ap, msg);

	fatalError(resolveMsg(module, section, msg, ap));

	va_end(ap);
}

