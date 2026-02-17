/********************************************************************
* Copyright (c) 1987 InterSoft Co.  All Rights Reserved
* THIS IS UNPUBLISHED PROPRIETARY SOURCE CODE OF InterSoft Co.
* The copyright notice above does not evidence any
* actual or intended publication of such source code.
*
* wiexwcmd.cc,v 1.1.1.1 1996/04/16 18:52:16 gustavof Exp
*
* DESCRIPTION
* Execute InterSoft program.
*
* ROUTINE       |  PURPOSE
*---------------+---------------------------------------------------
*WiExecCmd		Execute a WINDOW MANAGER user command.
*********************************************************************/
#include <ifound.h>

#include <string.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <unistd.h>

#ifdef HAVENT_UI
#include <local/activ.h>
#include <local/rcmdserv.h>

#include <winsock.h>
#include <local/slog.h>
#include <ifound/priv/pconfig.h>
#include <essentia/netconn.h>
extern "C" int WiCreateProces(int pid, char *module, char *name, char *version);
extern "C" int WiDeleteProces(int pid);
extern "C" int WiExecCmd2(char *titulo, char *cmd,int wt);

extern "C" int WiExecCmd(char *titulo, char *cmd)
{
	return WiExecCmd2(titulo, cmd, 0);
}

extern "C" int WiExecCmd2(char *titulo, char *cmd,int wt)
{
	HANDLE hEvent1,hEvent2,hFMap;
	HANDLE hObjects[2];
	PROCESS_INFORMATION child;
	SECURITY_ATTRIBUTES	sa;
	STARTUPINFO startup = {0};
	startup.cb = sizeof(startup);
	Int *fmap = NULL;

	SetLastError(0);

	if(CreateProcess(NULL,cmd,NULL,NULL,TRUE,DETACHED_PROCESS,NULL,NULL,
			&startup,&child) == FALSE){
		return ERR;
	}
	// Open IPC comm channel
	Int cpid = child.dwProcessId;
	String ipcName = toString(cpid)+"i";
	String evName1 = toString(cpid)+"1";
	String evName2 = toString(cpid)+"2";

	sa.nLength 				= sizeof(SECURITY_ATTRIBUTES);
	sa.lpSecurityDescriptor	= NULL;
	sa.bInheritHandle 		= TRUE;

	hEvent1 = CreateEvent(&sa,FALSE,FALSE,toCharPtr(evName1));
	hEvent2 = CreateEvent(&sa,FALSE,FALSE,toCharPtr(evName2));

//	WaitForSingleObject(hEvent1,INFINITE);
	hObjects[0] = hEvent1;
	hObjects[1] = child.hProcess;
	DWORD res;
	if((res = WaitForMultipleObjects(2,hObjects,FALSE,INFINITE)) != 0){
// I received object != 0 => application terminated before event1 was signales
// OK - may be it is not Window command (example: iql output to printer)
		CloseHandle(hEvent1);
		CloseHandle(hEvent2);
//		return ERR;
		return 0;
	}

	// OK - child made Filemap

	hFMap = OpenFileMapping(FILE_MAP_WRITE,FALSE,toCharPtr(ipcName));
	fmap = (Int *)MapViewOfFile(hFMap,FILE_MAP_WRITE,0,0,0);

	require( fmap != 0 );
	//  Write some information and let my child work;
	fmap[0] = GetCurrentProcessId();
	fmap[1] = ((NetConnection *)_processConfig()->conn)->sockDescr();
	fmap[2] = NO_CONVERT;
	SetEvent(hEvent2);

	WaitForSingleObject(hEvent1,INFINITE);
	if(fmap[0] != OK){
		SetEvent(hEvent2);
		return ERR;
	}
	// Successfully login.
	SetEvent(hEvent2);

	CloseHandle(hEvent1);
	CloseHandle(hEvent2);
	UnmapViewOfFile(fmap);
	CloseHandle(hFMap);
// 	Stop right here now !!
#if 1
	WaitForSingleObject(child.hProcess,INFINITE);
#else
	WiCreateProces(cpid, cmd, titulo, NULL);
	WaitForSingleObject(child.hProcess,INFINITE);
	WiDeleteProces(cpid);
#endif
	CloseHandle(child.hProcess);
	return 0;
}

#else
#include <ideafix.h>
#include <cfix.h>
#ifdef HAVE_PROC

#include <signal.h>
#include <ideafix/priv/gndefs.h>
#include <ifound/priv/gfuncs.h>
#include <local/critsect.h>
#include <cracker/pserconn.h>

/* +++ CONFIG +++ */
#define MAXARGS 50
#define COD_ERR 10
#define MAX_CMD_LENGTH	2048

/*
 * WiExecCmd: Ejecuta un comando usuario del WINDOW MANAGER
 */

static IFPICPCP punfunc = NULL;

void SetExecServ(IFPICPCP fp)
{
	punfunc = fp;
}


extern "C" int WiExecCmd(char *titulo, char *cmd)
{
	return WiExecCmd2(titulo, cmd, 0);
}

int WiExecCmd2(char *titulo, char *cmd, int wt)
{
	register char *p;
	int pid, argc, inquotes;
	int cond;			/*   condition   */
	char *argv[MAXARGS], aux[MAX_CMD_LENGTH+1];

	inquotes = 0;
	argc	 = 1;
	argv[0]  = p = strncpy(aux, cmd, MAX_CMD_LENGTH);
	aux[MAX_CMD_LENGTH] = 0;

	while ( *p && argc < MAXARGS ) {
		if (*p == (char)inquotes || (!inquotes && isspace((UChar) *p)))
		{
			/* Allow quotes to be escaped */
			if (inquotes && *(p-1) == '\\')
				mMove(p-1, p, MAX_CMD_LENGTH - (p-argv[0]));
			else {
				*p++ = 0;
				while (isspace((UChar) *p)) ++p;
				if (*p == 0) break;
				if (*p == '\'' || *p == '"')
					inquotes = *p++;
				else
					inquotes = 0;
				argv[argc++] = p;
			}
		}
		else ++p;
	}

	argv[argc] = NULL;

	if (punfunc) {
		SigSet(SIGCLD, SIG_IGN);
	}

	//CCriticalSection cs;
	//cs.init();
	
	if ((pid=fork()) == ERR) return ERR;
	
	if (pid == 0) {
		//cs.P();
		execvp((EXECVPPARAM1TYPE) argv[0],
			   (EXECVPPARAM2TYPE) argv);
		_exit(COD_ERR);
	}

	if (punfunc) return (*punfunc)(pid, argv[0], titulo);
	else {

		// 8 Oct 2010: Se encontro un error al utilizar el Liquidador con GIF evitando enviar los 2 mensajes de WiCreateProcess,
		// porque el programa "padre" del Liquidador no se cerraba bien
		//if (!crackerClient()) // Se encontraron errores al utilizar el Liquidador con GIF porque 
		{
			// Por GIF llegan 2 WiCreateProces: este y el proximo que envia el wcmd/Program
			// Este primer mensaje es innecesario y ademas producia errores en HPUX 11.31, Telecom
			//
			// 8 Oct 2010: Este error se arreglo con la semaforizacion. Sumado a que enviar 1 solo mensaje de WiCreateProcess
			// da un error en el Liquidador, se vuelven a enviar los 2 mensajes
			WiCreateProces(pid, argv[0], titulo, NULL);
		}

		//cs.V();
		if (wait(&cond) == ERR || (cond >> 8) == COD_ERR)
		{
			cond = ERR;
		}
		//cs.destroy();

		//if (!crackerClient())
		{
			// Esto cierra el WiCreateProces enviado mas arriba. Por cada WiCreateProces se envia un WiDeleteProces
			WiDeleteProces(pid);
		}

		return cond;
	}
}

#else /* HAVE_PROC */

extern "C" int WiExecCmd(char *titulo, char *cmd)
{
	return WiExecCmd2(titulo, cmd, 0);
}

int WiExecCmd2(char *titulo, char *cmd, int wait)
{
	return ExecShell(cmd);
}

#endif

#endif
