/********************************************************************
* Copyright (c) 1987 InterSoft Co.  All Rights Reserved
* THIS IS UNPUBLISHED PROPRIETARY SOURCE CODE OF InterSoft Co.
* The copyright notice above does not evidence any
* actual or intended publication of such source code.
*
*
* $Id: wmaplhlp.cc,v 1.6 2010/07/15 20:34:31 hardaiz Exp $
*
*
* DESCRIPTION:
*      Application help Library module.
*
* ROUTINE       |  PURPOSE
*---------------+---------------------------------------------------
*
*
*********************************************************************/

#include <stdlib.h>
#include <unistd.h>

#include <ideafix.h>
#include <cfix.h>
#include <ifound/str.h>
#include <ideafix/priv/gndefs.h>
#include <wm.h>
#include <ideafix/priv/wmcom.h>
#include <ifound/priv/gfuncs.h>

#define IDEAFIXDIR "/usr/ideafix"

#ifndef __VC__
#	include <sys/signal.h>
#else
#	include <signal.h>
#endif

/* +++ Prototypes +++ */
extern int atoi(const char *c) __THROW;
/* --- Prototypes --- */

static char apl_module[LONG_MODULE];
static char apl_version[LONG_VERSION];

extern int Serv;
#define FILESERV	"/services"	/* archivo conf servicios	*/

char const *_ApplModule(void)
{
	return apl_module;
}

void _SetApplModule(char const *mod)
{
	strncpy(apl_module, mod, LONG_MODULE);
}

char const *_ApplVersion(void)
{
	return apl_version;
}

void _SetApplVersion(char const *mod)
{
	strncpy(apl_version, mod, LONG_VERSION);
}

void _WiAplHelp(void)
{
	char Identif[96];

	if (!apl_module[0]) {
		WiMsg(_WmErrmsg("NO_HELP"));
		return;
	}

	sprintf(Identif, "[%s: %s]", apl_module, apl_version);
	WiHelpFile(apl_module, Identif);
}

void _WiSetAplHelp(void)
{
	char *mod = wm_par[0].s ? (char *) wm_par[0].s : (char *)"";
	char *ver = wm_par[1].s ? (char *) wm_par[1].s : (char *)"";

	strncpy(apl_module, BaseName(mod), LONG_MODULE-1);
	apl_module[LONG_MODULE-1]  = '\0';
	strncpy(apl_version, ver, LONG_VERSION-1);
	apl_version[LONG_VERSION-1] = '\0';
}

/*
 * hlptxt:
 * Funcion para tomar lineas con la descripcion de las
 * teclas, de la tabla de configuracion de teclado.
 */

static char _far buf[100];

char *hlptxt(char *t, int n)
{
	static int i = 0;

	/* Calcular dimension la primera vez */
	if( !i )
		while( key_tab[i].code != 0 ) ++i;

	if( n < 1 || n > i )
		return NULL;

	--n;
	if (key_tab[n].code == 1) /* Titulo */
		(void) sprintf(buf, "\033[1;4m%s\033[m", key_tab[n].descr);
	
	else                            /* Linea de descripcion */
		(void)sprintf(buf, "\t%-15s -->\t%s", 
			_CharName(key_tab[n].code),
			key_tab[n].descr ? key_tab[n].descr:_WmErrmsg("UNDEF_MSG"));
	 
	return buf;
}
			

void GoToShell(void)
{
	char *shell;
	FPCPCPI OldHandler = SetReadEnvHandler((FPCPCPI)NULLFP);
	SigHandlerT savefp;

	shell = ReadEnv("SHELL");
	SetReadEnvHandler(OldHandler);
	if (shell && shell[0] == '0')
		w_beep();
	else {
#ifndef HAVE_TTY
		_PcCls();
#endif

#ifdef HAVE_FULL_SIGNALS
		savefp = SigSet(SIGCLD, SIG_DFL);
		ExecShell(shell ? shell : (char *)DEFAULT_SHELL);
		SigSet(SIGCLD, savefp);
#else
		ExecShell(shell ? shell : DEFAULT_SHELL);
#endif
	}
}

/* Translate an ALT-? to the corresponding letter */
UChar AltKey(UChar c)
{
	switch(c) {
	case 30: return 'A';
	case 48: return 'B';
	case 46: return 'C';
	case 32: return 'D';
	case 18: return 'E';
	case 33: return 'F';
	case 34: return 'G';
	case 35: return 'H';
	case 23: return 'I';
	case 36: return 'J';
	case 37: return 'K';
	case 38: return 'L';
	case 50: return 'M';
	case 49: return 'N';
	case 24: return 'O';
	case 25: return 'P';
	case 16: return 'Q';
	case 19: return 'R';
	case 31: return 'S';
	case 20: return 'T';
	case 22: return 'U';
	case 47: return 'V';
	case 17: return 'W';
	case 45: return 'X';
	case 21: return 'Y';
	case 44: return 'Z';
	}
	return '\0';
}


/*
*	_w_service:	This function popup a window to select a service
*	option.
*/
void _w_service(void)
{
	char service[100] = {0};
	int lenhome; 

	/* Check the environment to see if Services allowed	*/
	if (Serv == -1) {
		char *s; 
		FPCPCPI OldHandler = SetReadEnvHandler((FPCPCPI)NULLFP);

		if ((s = ReadEnv("SERVICES"))) {
			Serv = atoi(s); if (Serv < 0) Serv = 1;
		}
		else
			Serv = 1;
		SetReadEnvHandler(OldHandler);
	}
	if (!Serv) {
		w_beep(); return;
	}

	/* obtengo el nombre del archivo de servicios	*/
	char *home = ReadEnv("HOME");
	lenhome = strlen(home);
	strncpy(service, home, lenhome);
	strcat(service, FILESERV);

	char fullpath[100] = {0};
	sprintf(fullpath, "%s.mn", service);

	if (access(fullpath, R_OK) != 0)
	{
		// No encontro el archivo services.mn en el HOME del usuario.
		// Ahora lo busco en $IDEAFIX/bin

		//WiMsg("No lo encontro [%s]. Busco en IDEAFIX/bin", fullpath);
		char *ideafix = NULL;
		if ((ideafix = ReadEnv("IDEAFIX")) == NULL)
		{
			//WiMsg("No existe IDEAFIX. Uso /usr/ideafix");
			ideafix = IDEAFIXDIR;
		}
		sprintf(service, "%s/bin%s", ideafix, FILESERV);

		sprintf(fullpath, "%s.mn", service);
		if (access(fullpath, R_OK) != 0)
		{
			// No lo encontro en $IDEAFIX/bin
			warning("No se pudo ubicar ni el archivo %s%s.mn\n ni el archivo %s.mn",
					ReadEnv("HOME"), FILESERV, service);
			return;
		}
	}

	ExecMenu(_WmErrmsg("SERV_LABEL"), service, MENU_PGM);
}
