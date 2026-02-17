/********************************************************************
* Copyright (c) 1991 InterSoft Co.  All Rights Reserved
* THIS IS UNPUBLISHED PROPRIETARY SOURCE CODE OF InterSoft Co.
* The copyright notice above does not evidence any
* actual or intended publication of such source code.
*
* $Id: log.h,v 1.1.1.1 1996/04/16 18:52:53 gustavof Exp $
*
* DESCRIPTION: Interface to log class
*********************************************************************/

#ifndef LOG_H
#define LOG_H

#ifdef __GNUC__
#pragma interface
#endif

#ifdef ISDEBUG
#	include <ifound/iddebug.h>
#endif

#include <ifound.h>
#include <ifound/str.h>
#include <local/osfile.h>

class gtype(List,String);

#define CURRPTR()	(buffer+currOffset-fileOffset)

class Log;

class LogItem 
{
    useIdCheck(LogItem_id);

	friend class Log;

private:

	Int		rowItem;
	Int		colItem;
	Int		prevLen;
	String	strItem;

public:

    LogItem(const LogItem &li ) { idStart(); *this = li; }
	LogItem(Int r = ERROR, Int c = ERROR, Int len = ERROR,
			const String &s = NULL_STRING);

    LogItem &operator=(const LogItem &li);

	Int	size() const;
	Int row() const { idCheck(); return rowItem; }
	Int col() const { idCheck(); return colItem; }
	Int length() const { idCheck(); return prevLen; }
	const String &string() const { idCheck(); return strItem; }
	bool operator!=(const LogItem &item) { idCheck(); return !(*this == item); }
	bool operator==(const LogItem &item);
	bool isValid() const
	{
		idCheck();
		return (rowItem != ERROR && colItem != ERROR && prevLen != ERROR); 
	}
};

// +++ Config +++
const Int LOG_BUFFSIZE = 4096;
const Int MAX_FILESIZE = 524288;
// --- Config ---

class Log 
{
    useIdCheck(Log_id);

private:

	// Buffer
	static char buffer[LOG_BUFFSIZE];

	// Actual absolute position
	static Int currOffset;

	// File where the log flushes
	static OsFile file;

	// Current position within the file of the actual buffer
	static Int fileOffset;

	// True if the buffer has been modified and needs to be saved
	static bool modBuffer;

	// Flush current buffer
	static void flush();

	// Advance by reading next segment to buffer
	static void advance();

	// Starting offset of the log
	Int startOffset;

	// Current pointer (within the buffer)
	char *ptr;

	// Current log's length
	Int lenLog;

	// What remains to be 'get()'
	Int remains;

	// Take the buffer to position n (and update ptr)
	void goTo(Int n);

	// Purge temp file up to the given displacement
	void purge(Int n);

	// Write to the log's buffer
	void write(const String &str);
	void write(const Int &num);

	// Read from the log's buffer
	void read(Int &num);
	void read(String &str);

    Log(const Log &);

public:

#ifdef ISDEBUG
	// Used for debugging
	static void print();
	static void printContents();
#endif

	Log();
	~Log();

    Log &operator=(const Log &);

	// Clean everything up (erase temporary file)
	void cleanUp();

	// Input operations
	void openInput();
	LogItem get();
	void get(gtype(List,String) &);
	void closeInput();

	// Output operations
	void openOutput();
	void put(const LogItem &);
	void put(gtype(List,String) &);
	void closeOutput();

};
#endif
