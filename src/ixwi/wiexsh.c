/********************************************************************
* Copyright (c) 1987 InterSoft Co.  All Rights Reserved
* THIS IS UNPUBLISHED PROPRIETARY SOURCE CODE OF InterSoft Co.
* The copyright notice above does not evidence any
* actual or intended publication of such source code.
*
* $Id: wiexsh.c,v 1.2 2006/02/17 17:29:11 spark Exp $
*
* DESCRIPTION
* Execute a command throught the shell.
*
* ROUTINE       |  PURPOSE
*---------------+---------------------------------------------------
*ExecShell	Ejecuta un comando mediante el shell.
*********************************************************************/

#ifndef PCLIENT

#include <stdlib.h>
#include <unistd.h>
#include <ideafix.h>
#include <ideafix/priv/gndefs.h>

/* Extern Functions										*/

/*
 * ExecShell: Ejecuta un comando mediante el shell. El comando
 *              recibe toda la pantalla limpia. Cuando termina el
 *              contenido inicial de la terminal es repuesto.
 *
 * p: linea de comando a ejecutar.
 *
 * RETURN: Comand return status.
 */

#ifdef HAVE_PROC

int ExecShell(char *p)
{
	int status;

#ifdef HAVE_CTRLC 
	AssertInt();
#endif
	if (WiCreate(SCREEN, 0, 0, WiHeight(SCREEN), WiWidth(SCREEN), 
			NO_BORDER, "", A_NORMAL) == ERR) {
		w_beep();
		return ERR;
	}

	WiRefresh();
	WiResettty(); 
#ifdef HAVE_SETUID
	setuid(getuid());
#endif
	status = system(p);
	WiSettty(); 
	WiDelete(WICURRENT);
	WiRedraw();
	return status == 0 ? OK : status;
}

#else /* HAVE_PROC */

int system(const char *s);

#ifdef HAVE_SWAP
static int read_swap = 1;

extern char *_swappath;
#endif

int ExecShell(char *p)
{
	int status;
	char CurrDir[128];

	/* _PcCls(); */
	WiResettty(); 

#ifdef HAVE_SWAP
	/* check if there is a specified path for the swap file */
	if (read_swap) {
		FPCPCPI OldHandler = SetReadEnvHandler(NULLFP);

		_swappath = ReadEnv("SWAPPATH");

		SetReadEnvHandler(OldHandler);
		read_swap = 0;		/* flag we've already read it */
	}
#endif

	getcwd(CurrDir, sizeof(CurrDir)-1);
	status = system(p);
	/* Restore the current directory		*/
	chdir(CurrDir);
	WiSettty(); 
	WiRedraw();
	return status == 0 ? OK : status;
}
#endif

#endif
