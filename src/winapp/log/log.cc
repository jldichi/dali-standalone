/********************************************************************
* Copyright (c) 1991 InterSoft Co.  All Rights Reserved
* THIS IS UNPUBLISHED PROPRIETARY SOURCE CODE OF InterSoft Co.
* The copyright notice above does not evidence any
* actual or intended publication of such source code.
*
* $Id: log.cc,v 1.1.1.1 1996/04/16 18:52:48 gustavof Exp $
*
* DESCRIPTION
* Implementation of log class
*********************************************************************/

#ifdef __GNUC__
#pragma implementation
#endif

#include <ifound.h>
#include <winapp/log.h>
#include <ifound/stdc.h>
#include <ifound/list.h>
#include <local/strfile.h>

#ifndef DEF_LIST_STRING
declare(List,String);
declare(ListCursor,String);
#define DEF_LIST_STRING
#endif

// Clase Log

Int Log::currOffset = 0;
Int Log::fileOffset = 0;
bool Log::modBuffer = false;

OsFile Log::file;
char Log::buffer[LOG_BUFFSIZE];

Log::Log()
{
	idStart();
	lenLog = 0;
	startOffset = currOffset;
}

Log::~Log()
{
	idCheck();
#if 0
	/*
		If log was not used, do nothing.
	*/
	if (!lenLog)
		return;
	/*
		Mark the log starting at startOffset as purgeable
	*/
	Int save = currOffset;

	goTo(startOffset);

	Int num = -2;
	write(num);
	write(lenLog);
	write(num);
	write(NULL_STRING);

	goTo(save);

	/*
		Now purge if necessary
	*/
	if (file.name() != NULL_STRING) {
		bool done = false;
		Int nPurge = 0;
		LogItem li;
		if (currOffset > MAX_FILESIZE) {
			/*
				Loop through all purgeable logs
			*/
			goTo(0);
			do {
				li = get();
				if (li.row() == -2) {
					nPurge += li.size() + li.col();
					goTo(nPurge);
				} else
					done = true;
			} while (!done);
			purge(nPurge);
		}
	}
#endif
	idStop();
}

Log &Log::operator=(const Log &l)
{
	idCheck();
	startOffset = l.startOffset;
	ptr = l.ptr;
	lenLog = l.lenLog;
	remains = l.remains;
	return *this;
}

void Log::goTo(Int n)
{
	idCheck();
	currOffset = n;
	/*
		Are we in bounds of the current buffer?
	*/
	if (currOffset < fileOffset ||
			currOffset >= fileOffset+LOG_BUFFSIZE) {
		if (modBuffer) {
			if (file.name() == NULL_STRING) {
				/*
					Create temp file
				*/
				if (!file.open(NULL_STRING, tmpnam(NULL), NULL_STRING, 
							 OsFile::READ|OsFile::WRITE|OsFile::CREAT))
					error("Log::goTo - can not create Log\'s temporary file.");

				fileOffset = 0;
			}
			file.write(buffer, fileOffset, LOG_BUFFSIZE);
			modBuffer = false;
		}

		fileOffset = currOffset;
		ptr = buffer;
		file.read(buffer, fileOffset, LOG_BUFFSIZE);
	} else
		ptr = CURRPTR();
}

#ifdef ISDEBUG
void Log::print()
{
	stdErr << "Longitud del log = " << currOffset << endl;
}

void Log::printContents()
{
	char *theBuff, buff[LOG_BUFFSIZE];

	if (file.name() == NULL_STRING) {
		stdErr << "No hay archivo" << endl;
		theBuff = buffer;
	} else {
		stdErr << "Archivo " << file.name() << endl;
		file.read(buff, 0, LOG_BUFFSIZE);
		theBuff = buff;
	}

	Int i, j;
	for (i = 0; i < 5; i++) {
		for (j = 0; j < 15; j++)
			stdErr << theBuff[i*15+j] << ", ";

		stdErr << endl;
	}
}
#endif

void Log::flush()
{
	if (file.name() == NULL_STRING) {
		/*
			Create temp file
		*/
		if (!file.open(NULL_STRING, tmpnam(NULL), NULL_STRING, 
					 OsFile::READ|OsFile::WRITE|OsFile::CREAT))
			error("Log::flush - can not create Log's temporary file.");

		fileOffset = 0;
	}
	file.write(buffer, fileOffset, LOG_BUFFSIZE);
	fileOffset += LOG_BUFFSIZE;
}

void Log::advance()
{
	if (file.name() == NULL_STRING)
		error("Can not advance (there wasn\'t any flush).");

	fileOffset += LOG_BUFFSIZE;
	file.read(buffer, fileOffset, LOG_BUFFSIZE);
}

void Log::write(const Int &num)
{
	idCheck();
	Int len = sizeof(num);
	char *buff = (char *) &num;
	currOffset += len;
	while (len--) {
		if (ptr >= buffer + LOG_BUFFSIZE) {
			flush();
			ptr = buffer;
		}
		*ptr++ = *buff++;
	}
	modBuffer = true;
}

void Log::write(const String &str)
{
	idCheck();
	Int len = str.length()+1;
	const char *theBuff = toCharPtr(str);
	char tmp[1];
	tmp[0] = '\0';
	const char *buff = (theBuff ? theBuff : tmp);
	currOffset += len;
	while (len--) {
		if (ptr >= buffer + LOG_BUFFSIZE) {
			flush();
			ptr = buffer;
		}
		*ptr++ = *buff++;
	}
	modBuffer = true;
}

void Log::read(Int &num)
{
	idCheck();
	Int len = sizeof(num);
	char *buff = (char *) &num;
	currOffset += len;
	while (len--) {
		if (ptr >= buffer + LOG_BUFFSIZE) {
			advance();
			ptr = buffer;
		}
		*buff++ = *ptr++;
	}
}

void Log::read(String &str)
{
	idCheck();
	str = NULL_STRING;

	do {
		if (ptr >= buffer+LOG_BUFFSIZE) {
			advance();
			ptr = buffer;
		}
		if (*ptr)
			str += *ptr;
	} while (*ptr++);

	currOffset += str.length()+1;
}

void Log::cleanUp()
{
	idCheck();
	if (file.name() == NULL_STRING)
		return;

	file.unLink();
}

void Log::openInput()
{
	idCheck();
	goTo(currOffset - sizeof(lenLog));
	read(lenLog);
	goTo(currOffset - (remains = lenLog) - sizeof(lenLog));
}

LogItem Log::get()
{
	idCheck();
	LogItem item;

	if (remains <= 0)
		return LogItem();

	read(item.rowItem);
	read(item.colItem);
	read(item.prevLen);
	read(item.strItem);

	remains -= item.size();
	return item;
}

void Log::get(List(String) &lst)
{
	idCheck();
	Int dummy;
	String str;
	
	while (remains > 0) {
		read(dummy);
		read(dummy);
		read(dummy);
		read(str);
		lst.add(str);
		remains -= 3*sizeof(dummy)+str.length()+1;
	}
}

void Log::closeInput()
{
	idCheck();
	this->goTo(currOffset - lenLog);
	/*chsize(file.getFd(), currOffset < 0 ? 0 : currOffset+1);*/
}

void Log::openOutput()
{
	idCheck();
	lenLog = 0;
	ptr = CURRPTR();
}

void Log::put(const LogItem &item)
{
	idCheck();
	Int sz = item.size();

	write(item.rowItem);
	write(item.colItem);
	write(item.prevLen);
	write(item.strItem);

	lenLog += sz;
}

void Log::put(List(String) &lst)
{
	idCheck();
	Int dummy = 0;
	ListCursor(String) lc;	
	String str;

	for (lc = lst; lc; lc++) {
		write(dummy);
		write(dummy);
		write(dummy);
		write(str = lc.value());
		lenLog += 3*sizeof(dummy)+str.length()+1;
	}
}

void Log::closeOutput()
{
	idCheck();
	write(lenLog);
}

void Log::purge(Int nBytes)
{
	idCheck();
	currOffset -= nBytes;
	if (file.name() == NULL_STRING) {
		char *buffPtr;
		for (buffPtr = buffer; buffPtr-buffer < nBytes; buffPtr++)
			*buffPtr = *(buffPtr+nBytes);
	} else {
		if (modBuffer)
			file.write(buffer, fileOffset, LOG_BUFFSIZE);

		Int count = 0, bytesRead;

		while (count < currOffset) {
			bytesRead = file.read(buffer, nBytes+count, LOG_BUFFSIZE);
			file.write(buffer, count, bytesRead);
			count += bytesRead;
		}

	}
	fileOffset = currOffset - LOG_BUFFSIZE;
	if (fileOffset < 0)
		fileOffset = 0;
	modBuffer = false;
	if (file.name() != NULL_STRING)
		file.read(buffer, fileOffset, LOG_BUFFSIZE);
}

// Fin clase Log


// Clase LogItem

LogItem::LogItem(Int r, Int c, Int len, const String &s)
{
	idStart();
	rowItem = r;
	colItem = c;
	prevLen = len;
	strItem = s;
}

LogItem &LogItem::operator=(const LogItem &li)
{
	idCheck();
	rowItem = li.rowItem;
	colItem = li.colItem;
	prevLen = li.prevLen;
	strItem = li.strItem;
	return *this;
}

Int LogItem::size() const
{
	idCheck();
	return	strItem.length()+1+
			sizeof(rowItem)+
			sizeof(colItem)+
			sizeof(prevLen);
}

bool LogItem::operator==(const LogItem &item)
{
	idCheck();
	return (rowItem == item.row() &&
			colItem == item.col() &&
			prevLen == item.length() &&
			strItem == item.string());
}

// Fin clase LogItem
