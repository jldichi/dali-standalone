/********************************************************************
* Copyright (c) 2026 Master_Solutions S.R.L.  All Rights Reserved
* THIS IS UNPUBLISHED PROPRIETARY SOURCE CODE OF Master_Solutions S.R.L.
* The copyright notice above does not evidence any
* actual or intended publication of such source code.
*
* $Id: strpipes.cc,v 1.4 2004/09/29 21:05:36 damianm Exp $
*
* DESCRIPTION: Implementation of stream pipes
*********************************************************************/
#ifdef __GNUC__
#pragma implementation
#endif

#include <sys/types.h>
#include <sys/wait.h>
#include <unistd.h>


#include <local/strpipes.h>
#include <ifound/iopipes.h>
#include <ifound/stdc.h>

static Int wait_for_child(Int p);

char InputPipe::fill()
{
	char rv = InputFdFile::fill();
	if (eof()) {
		if (wait_for_child(pid) != 0)
			state |= IO_ERROR;
	}
	return rv;
}

InputPipe::InputPipe(const String &cmd, bool shell_flag, 
					 bool stderr_flag, short bufsize)
: InputFdFile(-1, bufsize)
{
	if ((pid = ioPipe(cmd,&fd,NULL,shell_flag,stderr_flag)) == ERR)
		state |= IO_ERROR;
}

InputPipe::~InputPipe()
{
	_global_close(fd);
	(void) wait_for_child(pid);
}

OutputPipe::OutputPipe(const String &cmd, bool shell_flag,
					   bool stderr_flag, short bufsize)
: OutputFdFile(-1, bufsize)
{
	if ((pid = ioPipe(cmd,NULL,&fd,shell_flag,stderr_flag)) == ERR)
		state |= IO_ERROR;
}

OutputPipe::~OutputPipe()
{
	flush();						// force buffer flush
	_global_close(fd);
	(void) wait_for_child(pid);
}

InOutPipe::InOutPipe(const String &cmd, bool shell_flag, 
					 bool stderr_flag, short bufsize)
: InputFdFile(-1, bufsize), OutputFdFile(-1, bufsize)
{
	pid = ioPipe(cmd,
				 &((InputFdFile  *)this)->fd,
				 &((OutputFdFile *)this)->fd,
				 shell_flag, stderr_flag);
	if (pid == ERR) {
		InputFdFile::addState(IO_ERROR);
		OutputFdFile::addState(IO_ERROR);
	}
}

InOutPipe::~InOutPipe()
{
	flush();						// force buffer flush
	_global_close(InputFdFile::fd);
	_global_close(OutputFdFile::fd);
	(void) wait_for_child(pid);
}

void InOutPipe::close()
{
	flush();
	_global_close(OutputFdFile::fd);
}

static Int wait_for_child(Int p)
{
	int s = ERR;
#if HAVE_PROC

#if HAVE_WAITPID
	waitpid(PidT(p), &s, 0);
#else
#	ifndef __NT__
	if (kill(int(p), 0) != ERR) {		// find if process exists
		int son_pid = -1;

		while (son_pid != p) 
			son_pid = wait(&s);
	}	// child does not exist
#	endif
#endif	// HAVE_WAITPID
#endif	// HAVE_PROC
	return s;
}

// end of strpipes
