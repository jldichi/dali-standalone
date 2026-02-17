/********************************************************************
* Copyright (c) 1991 InterSoft Co.  All Rights Reserved
* THIS IS UNPUBLISHED PROPRIETARY SOURCE CODE OF InterSoft Co.
* The copyright notice above does not evidence any
* actual or intended publication of such source code.
*
* $Id: strfile.h,v 1.1.1.1 1996/04/16 18:51:49 gustavof Exp $
*
* DESCRIPTION
* Stream file class interface
*********************************************************************/

#ifndef	STRFILE_H
#define	STRFILE_H

#ifdef __GNUC__
#pragma interface
#endif

#include <ifound.h>

#include <local/istream.h>
#include <ifound/str.h>
#include <local/osfile.h>


class InputFile : public InputStream {

// private :

	InputFile(const InputFile &);
	
	OsFile *osf;
	bool isalloc;

public  :

	explicit InputFile(short bufsize = DEF_BUFSIZE);
	explicit InputFile(OsFile &of, short bufsize = DEF_BUFSIZE);
	explicit InputFile(const String &fname, const String &fext = NULL_STRING,
				short bufsize = DEF_BUFSIZE);
	InputFile(const String &path, const String &fname,
		const String &fext = NULL_STRING, short bufsize = DEF_BUFSIZE);

	virtual ~InputFile();
	
	virtual char fill();
	bool open(const String &fname, const String &fext = NULL_STRING);

	Int tell() { return offset - cnt; }
	Int seek(Int position);
	Time mtime() { return osf->mtime(); }
};

enum {
	TRUNC_FILE,
	APPEND_TO_FILE
};

class OutputFile : public OutputStream {

// private :

	OutputFile(const OutputFile &);
	
	OsFile *osf;
	bool isalloc;

public:

	explicit OutputFile(short bufsize = DEF_BUFSIZE);
	explicit OutputFile(OsFile &os, short bufsize = DEF_BUFSIZE);
	explicit OutputFile(const String &fname, const String &fext = NULL_STRING,
				short cflag = TRUNC_FILE, short bufsize = DEF_BUFSIZE);

	virtual ~OutputFile();
	
	virtual void flush();

	bool open(const String &fname, const String &fext = NULL_STRING,
				short cflag = TRUNC_FILE);
	Int tell() { return offset + ptr - base; }
	Int seek(Int position);
	Int sync() { return osf ? osf->sync() : false; }
#ifdef ISDEBUG
	void print() const;
#endif
};

#include <local/strpipes.h>
#include <local/ofdfiles.h>
#include <local/ifdfiles.h>

#endif
