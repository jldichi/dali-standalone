//- Copyright Notice
// -----------------------------------------------------------------------
// (C) Copyright 1998 InterSoft S.A.  All Rights Reserved
// THIS IS UNPUBLISHED PROPRIETARY SOURCE CODE OF InterSoft S.A.
// The copyright notice above does not evidence any actual or intended 
// publication of such source code.
//
// $Id: sqlfwmsg.h,v 1.3 2002/03/20 17:12:22 hardaiz Exp $
// -----------------------------------------------------------------------

#ifndef SQLFWMSG_H
#define SQLFWMSG_H

#define	SQLFW_MSG_TABLE	"sqlfw"
#define SQLFW_MSG_MAIN	"main"

String getSQLFWMsg(const char* msgid,
					const String &param1=NULL_STRING,
					const String &param2=NULL_STRING,
					const String &param3=NULL_STRING);

String getSQLFWMsg(const String &msgid,
					const String &param1=NULL_STRING,
					const String &param2=NULL_STRING,
					const String &param3=NULL_STRING);

#endif // SQLFWMSG_H
