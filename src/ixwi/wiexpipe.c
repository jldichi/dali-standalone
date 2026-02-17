/********************************************************************
* Copyright (c) 1987 InterSoft Co.  All Rights Reserved
* THIS IS UNPUBLISHED PROPRIETARY SOURCE CODE OF InterSoft Co.
* The copyright notice above does not evidence any
* actual or intended publication of such source code.
*
* $Id: wiexpipe.c,v 1.4 2002/10/15 20:56:59 albertoi Exp $
*
* DESCRIPTION
* Execute a command throught a pipe.
*
* ROUTINE       |  PURPOSE
*---------------+---------------------------------------------------
*ExecPipe		 Execute a command throught a pipe.
*********************************************************************/

#ifndef PCLIENT

#include <ideafix.h>
#include <ideafix/priv/gndefs.h>
#ifdef __NT__
#	undef ClosePrinter
#	undef CreateCursor
#	include <windows.h>
#endif

#ifdef HAVE_UNDERSCORE_NAMES
#	define close _close
#	define fileno _fileno
#endif 

#ifdef HAVE_SET_STD_HANDLE
static char * tmpf; // name of temporal file.
#define PipeSize 4096

static void createRedirectedProcess(char *cmd)
{
	char buffer[PipeSize];
	DWORD length, total = 0;
	HANDLE thisProcess, childout, hfp;
	PROCESS_INFORMATION child;
	static STARTUPINFO startup;

	startup.cb = sizeof(startup);
	thisProcess = GetCurrentProcess();
	AllocConsole();

	tmpf = tempnam(NULL, "pipe");

	hfp = CreateFile(
		(LPCTSTR) tmpf,
		GENERIC_WRITE,
		0,
		NULL,
		CREATE_ALWAYS,
		FILE_ATTRIBUTE_NORMAL,
		NULL);

	childout = (void*)-1;

	DuplicateHandle(
		thisProcess,
		hfp,
		thisProcess,
		&childout,
		0,
		TRUE,
		DUPLICATE_CLOSE_SOURCE|DUPLICATE_SAME_ACCESS);

	SetStdHandle(STD_OUTPUT_HANDLE, childout);
	SetStdHandle(STD_ERROR_HANDLE, childout);

	CreateProcess(
		NULL, cmd,
		NULL, NULL,
		TRUE, 0,
		NULL, NULL,
		&startup, &child);

	CloseHandle(childout);
	FreeConsole();
	WaitForSingleObject(child.hProcess, INFINITE);
	CloseHandle(hfp);
}
#endif

 /*
  * ExecPipe: Ejecuta comando a traves de pipe, mostrando la salida
  *           en una window, cuyos parametros 'window' se indican.
  *           La window se crea en la posicion actual del cursor.
  *
  * cmd          : Linea de comando a ejecutar
  * nfils, ncols : Dimensiones window.
  * border       : Tipo de borde window.
  * label        : Titulo de la window.
  */

int ExecPipe(char *cmd, int nfils, int ncols, attr_type border,
		char *label)
{
#ifndef HAVE_PIPES
	int stdout_save, stderr_save;
#endif
	int Status;
	FILE *fp; 
	char buf[128]; /* Doesn't make sense lines longer than 128 */
	char MoreData[96];
	char NoMoreData[96];
	char Aux[64];
	char *p;
	int n, c;
	window wiFrame;

	sprintf(NoMoreData, "\033[7m  Fin (status=%%d). Presione una tecla  \033[0m");
	sprintf(MoreData, 
"\033[7m  \033[7;1m%s\033[7m: Continuar   \033[7;1m%s\033[7m: Fin  \033[0m",
			WiKeyHelp(K_ENTER,(UChar *) NoMoreData),
			WiKeyHelp(K_END,(UChar *) Aux));

	/* Make stdin & stdout of command the same */
#ifndef HAVE_SET_STD_HANDLE
#ifndef HAVE_PIPES
	if ((fp = tmpfile()) == NULL) return ERR;
#ifdef HAVE_CTRLC 
	AssertInt();
#endif
	stdout_save = dup(1);
	stderr_save = dup(2);
	dup2(fileno(fp), 1);
	dup2(fileno(fp), 2);
	Status = system(cmd);
	dup2(stdout_save, 1);
	dup2(stderr_save, 2);
	close(stdout_save);
	close(stderr_save);
	rewind(fp);
#else
	strcpy(buf, cmd);
	strcat(buf, " 2>&1");

	if ((fp = popen(buf, "r")) == NULL) return ERR;
#endif
#else  // HAVE_SET_STD_HANDLE
	createRedirectedProcess(cmd);
	fp = fopen(tmpf, FOPEN_READ_MODE);
	if (!fp)
		return ERR;
#endif
	/*
	 * Adjust nfils & ncols
	 */
	if (nfils <= 0) nfils = 10;
	if ( ncols < (c=StrDspLen(NoMoreData)+2) ) ncols = c;
	if ( ncols < (c=StrDspLen(label)+2) ) ncols = c;

	/*
	 * Create window
	 */
	if (WiCurrent() == SCREEN) wiFrame = SCREEN;
	else wiFrame = WiParent(WICURRENT);

	if (WiCreate(wiFrame, WiLine(wiFrame)+2, WiCol(wiFrame)+2, nfils, ncols, 
		STAND_BORDER, label, A_NORMAL) == ERR) {
			w_beep();
			return ERR;
	}

	WiRefresh();
	WiWrap(FALSE);

	n     = 0;
	nfils = WiHeight(WICURRENT);
	while ( fgets(buf,sizeof(buf),fp) != NULL ) {
		p = strrchr(buf, '\n');

		if (p) *p = 0; /* strip \n */
		WiMoveTo(n, 0);
		WiPuts(buf);
		if ( n == (nfils-1) ) {
			WiMoveTo(nfils, 1);
			WiPuts(MoreData);
			if ( (c=WiGetc()) == K_END) break;
			WiErase();
			WiSetBorder(border, label);
			WiMoveTo(0,0);
			n = 0;
		}
		else {
			n++;
/*			WiRefresh(); I dont need it.  */
		}
	}

#ifdef HAVE_PIPES
	Status = pclose(fp) ? ERR : OK;
#else
	Status = fclose(fp) ? ERR : Status;
#	ifdef HAVE_SET_STD_HANDLE
		unlink(tmpf);
#	endif
#endif

	if (c != K_END) {
		WiMoveTo(nfils, 1);
		WiPrintf(NoMoreData, Status);
		(void)WiGetc();
	}

	WiDelete(WICURRENT);
	return Status;
}

#endif
