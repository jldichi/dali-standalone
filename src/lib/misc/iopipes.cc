// -----------------------------------------------------------------------
// Copyright (c) 1995 InterSoft Co.  All Rights Reserved
// THIS IS UNPUBLISHED PROPRIETARY SOURCE CODE OF InterSoft Co.
// The copyright notice above does not evidence any actual or intended
// publication of such source code.
//
// $Id: iopipes.cc,v 1.4 2004/09/29 17:23:18 damianm Exp $
//
// Description:
// Input output pipes.
// cmd is the command to execute
// read_fd is a pointer to the file desc of read pipe
// write_fd is a pointer to the file desc of write pipe
// if both are no null, the pipe is for io,
// if read_fd is null, the pipe is for writing,
// if write_fd is null the pipe is for reading.
// -----------------------------------------------------------------------

#include <unistd.h>
#include <string.h>

#include <ifound.h>
#include <ifound/iopipes.h>
#include <ifound/stdc.h>

#define MAXARGS					20
#define MAX_CMD_LENGTH			1024

#ifdef IDEAWIN
void _exit(int);
#endif

void execCmd(String cmd, bool shell_flag)
{
	register char *p;
	char *argv[MAXARGS], aux[MAX_CMD_LENGTH + 1];

	if (shell_flag)
		cmd = String("SHELL").getEnv()+" -c \""+cmd+"\"";

	int inquotes = 0, argc = 1;
	argv[0] = p = strncpy(aux, toCharPtr(cmd), MAX_CMD_LENGTH);
	aux[MAX_CMD_LENGTH] = 0;

	while (*p) {
		if (*p == char(inquotes) || !inquotes && isspace(*p)) {

			*p++ = 0;

			while (isspace(*p))
				++p;

			if (*p == 0)
				break;

			if (*p == '\'' || *p == '"')
				inquotes = *p++;
			else
				inquotes = 0;

			argv[argc++] = p;
		}
		else ++p;
	}

	argv[argc] = NULL;

	execvp(argv[0], argv);
	_exit(0);
}

Int ioPipe(String cmd, int *read_fd, int *write_fd, bool shell_flag,
		   bool stderr_flag)
{
#ifndef HAVE_PIPES
	return 0;
#else
	// check if it is a valid string for a command

	if (!cmd)
		error(libMsg("INVCMD"));

	int in_pipe[2], out_pipe[2];

	// we want an input pipe
	if (read_fd != NULL) {
		if (pipe(in_pipe) == -1)
			return ERR;
		*read_fd = in_pipe[0];
	}

	// we want an output pipe
	if (write_fd != NULL) {
		if (pipe(out_pipe) == -1)
			return ERR;
		*write_fd = out_pipe[1];
	}

	Int child_pid = fork();

	if (child_pid == ERR)
		return ERR;

	if (!child_pid) {

		if (read_fd != NULL) {
			_global_close(1);	// connect child's stdout with input pipe
			dup(in_pipe[1]);
			// Check if I want to redirect stderr
			if (stderr_flag) {
				_global_close(2);
				dup(in_pipe[1]);
			}
			_global_close(in_pipe[1]);
			_global_close(in_pipe[0]);
		}

		if (write_fd != NULL) {
			_global_close(0);	// connect child's stdin with ouput pipe
			dup(out_pipe[0]);
			_global_close(out_pipe[0]);
			_global_close(out_pipe[1]);
		}

		// now exec the real command
		execCmd(cmd, shell_flag);
	}

	if (read_fd != NULL)
		_global_close(in_pipe[1]);

	if (write_fd != NULL)
		_global_close(out_pipe[0]);

	return child_pid;
#endif
}

// end of iopipe
