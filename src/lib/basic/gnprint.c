/********************************************************************
* Copyright (c) 1987 InterSoft Co.  All Rights Reserved
* THIS IS UNPUBLISHED PROPRIETARY SOURCE CODE OF InterSoft Co.
* The copyright notice above does not evidence any
* actual or intended publication of such source code.
*
* gnprint.c,v 1.1.1.1 1996/04/16 18:52:01 gustavof Exp
*
* DESCRIPTION
*
* ROUTINE       |  PURPOSE
*---------------+---------------------------------------------------
*ClosePrinter		 Close a lineprinter
*OpenPrinter		 Open a lineprinter
*
*********************************************************************/

#if defined (__NT__) && !defined(IDEAWIN)
#	define DATE dummyDATE
#	include <windows.h>
#	undef DATE
#endif
#include <ideafix.h>
#include <ideafix/priv/gndefs.h>

/* +++ CONFIG +++ */
#ifndef HAVE_PIPES
#	define DEF_TEMP_PRN "0UOYKCUF.$$$"
#endif
/* --- CONFIG --- */

/* Functions taken from other modules                               */
#ifdef IDEAWIN
extern int winpm(char *);
#ifdef __VC__
#	define tempnam _tempnam
#endif
#endif


#ifndef HAVE_PIPES
static int _ncopies = 0;
static char _dos_cmd[150] = {0};
static char *TEMP_PRN;
#endif

/* Global static variables                                         */
static char _tipo = '\0';

#define MAX_PRINTER_LEN	128

FILE *OpenPrinter(int ncopies, char *x)
{
	char *output, linea[MAX_PRINTER_LEN+1];

	output = ReadEnv("printer");
	_tipo = output[0];
	output = strchr(output, ':');
	if  (output == NULL) return NULL;

	output++;

	if (_tipo == 'P' || _tipo == 'p') {
#ifdef HAVE_PIPES
		sprintf(linea, output, ncopies, x);
		assert(strlen(linea) <= MAX_PRINTER_LEN);
		return popen(linea, "w");
#else
		FPCPCPI OldHandler = SetReadEnvHandler(NULLFP);

		char *tmpx;
		tmpx=(char *)malloc(150);
		if ((TEMP_PRN=ReadEnv("TEMPRINT"))==(char*)NULL){
			strcpy(tmpx,tempnam(NULL, "pr"));
			TEMP_PRN=tmpx;
}
		SetReadEnvHandler(OldHandler);

#ifndef HAVE_PM
		if (strncmp(output, "pm", 2))
			return (FILE *) NULL;
#endif
		_ncopies = ncopies;
		strcpy(_dos_cmd, output); /* save buffer */
		if(*(_dos_cmd+strlen(_dos_cmd)-1) != 's')
			strcat (_dos_cmd," %s");

		return fopen(TEMP_PRN, "w");
#endif
	}

	/* Normal file */

	sprintf(linea, output, x);
	assert(strlen(linea) <= MAX_PRINTER_LEN);
	return fopen(linea, "w");
}

void ClosePrinter(FILE *p)
{
	if (_tipo!='P' && _tipo != 'p') {
		fprintf(p, "\f");
		if (p) fclose(p);
		return;
	}
#ifdef HAVE_PIPES
	pclose(p);
#else
	{
		/* Build a cmd line, and execute it	*/

		char aux[255];
		if (p) fclose(p);
		sprintf(aux, _dos_cmd, TEMP_PRN);
#if IDEAWIN
		SetProcessWait();
#endif
		while (_ncopies--) {
#ifdef IDEAWIN
			(void ) winpm(TEMP_PRN);
#else
#	ifndef HAVE_PM
			int Argc = 0, wasspace = 0;
			char *Argv[256];
			char *q=aux;

			Argv[Argc] = q;
			q += 2;
			do {
				if (*q==' ') {
					wasspace = 1;
					*q = '\0';
				} else if (wasspace) {
					Argv[++Argc] = q;
					wasspace = 0;
				}
			} while (*++q);
			if (pm(++Argc, Argv) != 0)
				break;
#	else
#if defined (__NT__) && !defined(IDEAWIN)
	PROCESS_INFORMATION child;
	STARTUPINFO startup = {0};
	startup.cb = sizeof(startup);
	if(CreateProcess(NULL,aux,NULL,NULL,TRUE,DETACHED_PROCESS,NULL,NULL,
			&startup,&child) == FALSE)
		break;
	WaitForSingleObject(child.hProcess,INFINITE);
#else
			if (system(aux) != 0) break;
#endif
#	endif
#endif /* IDEAWIN */
		}
#if IDEAWIN
		ResetProcessWait();
#endif
		unlink(TEMP_PRN);
	}
#endif
}
