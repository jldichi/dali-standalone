/********************************************************************
* Copyright (c) 1991 InterSoft Co.  All Rights Reserved
* THIS IS UNPUBLISHED PROPRIETARY SOURCE CODE OF InterSoft Co.
* The copyright notice above does not evidence any
* actual or intended publication of such source code.
*
* $Id: msgtable.h,v 1.1.1.1 1996/04/16 18:51:33 gustavof Exp $
*
* DESCRIPTION
* Interface to MsgTable class
*********************************************************************/

#ifndef MSGTABLE_H
#define MSGTABLE_H

#include <ifound.h>
#include <ifound/str.h>

#ifdef __GNUC__
#	pragma interface
#endif

class MsgFile;
class Message;

class MsgTable {

	friend class MsgTableCursor;

	Int section;
	MsgFile *file;
	bool informError;

public:

	explicit MsgTable(const String &module,
			const String &section = NULL_STRING, bool infError = false);
	~MsgTable();

	String find(const String &name);
	String find(const String &name, bool infErr);
	String find(const String &sec, const String &name);
	String find(const String &sec, const String &name, bool infErr);

	Message operator()(const String &name);
	Message operator()(const String &sec, const String &name);
};

#endif // MSGTABLE_H
