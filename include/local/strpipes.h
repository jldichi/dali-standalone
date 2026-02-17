/********************************************************************
* Copyright (c) 1991 InterSoft Co.  All Rights Reserved
* THIS IS UNPUBLISHED PROPRIETARY SOURCE CODE OF InterSoft Co.
* The copyright notice above does not evidence any
* actual or intended publication of such source code.
*
* $Id: strpipes.h,v 1.1.1.1 1996/04/16 18:51:49 gustavof Exp $
*
* DESCRIPTION
* StrPipes file class interface
*********************************************************************/

#ifndef	STRPIPES_H
#define	STRPIPES_H

#ifdef __GNUC__
#pragma interface
#endif

#include <ifound.h>

#include <local/ifdfiles.h>
#include <local/ofdfiles.h>

class InputPipe : public InputFdFile {

// private :

	Int pid;	// child process id

	InputPipe(const InputPipe &);
	
public :

	explicit InputPipe(const String &cmd, bool shell_flag = true, 
			  bool stderr_flag = true, short bufsize = DEF_BUFSIZE);
	virtual ~InputPipe();
	char fill();

	int operator!() const 	{ return (pid == ERR); }

};

class OutputPipe : public OutputFdFile {

// private :
	Int pid;	// child process id

	OutputPipe(const OutputPipe &);
	
public:

	explicit OutputPipe(const String &cmd, bool shell_flag = true, 
			   bool stderr_flag = true, short bufsize = DEF_BUFSIZE);
	virtual ~OutputPipe();

	int operator!() const 	{ return (pid == ERR); }

};

class InOutPipe : public InputFdFile, public OutputFdFile {

// private :

	Int pid;	// child process id

	InOutPipe(const InOutPipe &);
	
public:

	explicit InOutPipe(const String &cmd, bool shell_flag = true, 
			  bool stderr_shell = true,
			  short bufsize = DEF_BUFSIZE);
	virtual ~InOutPipe();
	void close();

	int operator!() const 	{ return (pid == ERR); }

};

class Printer : public OutputPipe {

// private:

	Printer(const Printer &);
	
public :

	Printer();
	~Printer();
};

#endif
