/********************************************************************
* Copyright (c) 1987 InterSoft Co.  All Rights Reserved
* THIS IS UNPUBLISHED PROPRIETARY SOURCE CODE OF InterSoft Co.
* The copyright notice above does not evidence any
* actual or intended publication of such source code.
*
* $Id: wmgetc.cc,v 1.5 2004/09/29 18:35:16 damianm Exp $
*
* DESCRIPTION:
*
* ROUTINE       |  PURPOSE
*---------------+---------------------------------------------------
*_WiGetc		| get a character from keyboard.
*_WiKeyReady	| Checks if keyboard contains characters
*********************************************************************/

#include <stdlib.h>
#include <unistd.h>

#include <ideafix.h>
#include <cfix.h>

#include <ifound/str.h>

#include <ideafix/priv/gndefs.h>
#include <wm.h>
#include <signal.h>
#include <setjmp.h>
#include <ideafix/priv/wmcom.h>

/* ++ Prototypes ++ */

static UChar _read_demo_file(void);
static void SLEEP(int n);
static void _end_demo(void);

#ifndef HAVE_TTY
#define	SPANISH	1
static void _DoSpecialKey(UChar c);
#endif

/* -- Prototypes -- */


/*+++++ CONFIG ++++*/

#ifndef __NT__
#	define FILESERV	"/services"	 /* archivo conf servicios para Unix */
#else
#	define FILESERV	"\\services" /* archivo conf servicios para NT */
#endif

/* variables globales */
bool suspend;
int curr_raw = WM_NORMAL;
bool hab_susp = TRUE;	/* habilita la suspencion o no de la tarea	 */
bool hab_sig = TRUE;	/* habilita el invio de signal a curr_proc	 */
bool handle_serv = TRUE;	/* Should handle or not K_ATTENTION */

/*********** Here some language configurable messages	***********/

#define CAL_LEN	12	/* calculator field length */

/*----- CONFIG ----*/

/* Extern Variables taken from other modules		*/
extern bool read_demo;
extern char * dem_file;
extern int Serv;
extern char *TermName;


/* Functions										*/

/*
 * _WiGetc(): Lee un caracter del teclado. No lo despliega !.
 *			 Los caracteres son leidos mediante _vs_getc.
 *			 Si es un comando de wm lo procesa y lee nuevamente.
 *		     refresca la pantalla antes de leer.
 *
 * j.l.e.: The newly implemented WiKeyReady() used to call
 *         vs_refresh() only if no characters pending.
 */

void _WiGetc(void)
{
	bool habaux, habaux1;
	register UChar c;
	char WLabel[128];
	static int level=0;
	level++;

#ifndef HAVE_TTY
	if (level > 4) {
			w_beep();
			level--;
			wm_par[0].u = K_END;
	 		return;
	}
#endif
	Int fuse = 0;
	forever {
        if (fuse++ > 1000) {
        	FILE*f = fopen("/tmp/wm_abort", "a");
        	fprintf(f, "Wm(%d) in loop in _WiGetc, c = %d\n", getpid(), c);
        	fclose(f);
        	_global_exit(1);
        }

		/* Update screen only if no characters pending */
#ifndef __NT__
		if (!WiKeyReady())
#endif
			vs_refresh();

		suspend = FALSE;

		if (dem_file == NULL) {
			c = _vs_getc();
			if (suspend) return;
			if (curr_raw == WM_PURERAW ||
				(curr_raw == WM_RAW && c == K_SUSPEND) ||
				(!handle_serv && c == K_ATTENTION)) {
				level--;
				wm_par[0].u = c;
				return;
			}
		}
		else {
			(void)SetSigInt(_w_interrupt);
			c = _read_demo_file();
			(void)SetSigInt((FP)SIG_IGN);
			if (dem_file == NULL) continue;
			if (suspend) return;
		}

		switch (c) {

		case K_SUSPEND :
			if (hab_susp) {
				suspend = TRUE;
				return;
			}
			w_beep();
			break;

		case K_REDRAW :
			vs_redraw();
			break;

		case K_KEYCONF:
			sprintf(WLabel, "[%s %s]", _WmErrmsg("HELP_TITLE"), TermName);
			WiHelp(8, 40, WLabel, hlptxt, (char*)NULL);
			break;

		case K_APL_HELP:
			WiAplHelp();
			break;

		case K_ATTENTION:
			habaux  = hab_susp;
			habaux1 = hab_sig;
			hab_sig = hab_susp = FALSE;
			_w_service();
			hab_susp = habaux;
			hab_sig = habaux1;
			break;

#ifndef HAVE_TTY
		case 0: /* ALT */
			_DoSpecialKey(AltKey(KeyGetc()));
			break;
#endif

		default:
			level--;
			wm_par[0].u = c;
			return;
		}

		if (curr_raw == WM_RAW) {
			wm_par[0].u = c;
			return;
		}

#ifndef HAVE_TTY
		if (wm_mode == MODE_TERM && level==1) {
			level--;
			WiRefresh();
			wm_par[0].u = 0;
			return;
		}
#endif
	}
}

#ifndef HAVE_TTY

static UChar last_number[CAL_LEN+1];	/* of the calculator */

static void _DoSpecialKey(UChar c)
{
	char WLabel[128];
	static int level = 0;

	if(++level > 4){
 		level--;
		w_beep();
		return;
	}

	switch(c) {
	case 'C' :
		w_calculator(WiLine(SCREEN)+2, WiCol(SCREEN), 0, last_number,
				CAL_LEN);
		break;
	case 'D' :	/* DOS */
		GoToShell();
		break;
	case 'H' :	/* Help */
		do_help();
		break;
	case 'I' :	/* Juego de caracteres de Ideafix */
		_print_ichset();
		break;
	case 'K' :	/* Keyboard */
		sprintf(WLabel, "[%s %s]", _WmErrmsg("HELP_TITLE"), TermName);
		WiHelp(8, 40, WLabel, hlptxt, (char*)NULL);
		break;
	case 'P' :	/* Print Screen */
		_print_scr();
		break;
	case 'R' :	/* Redraw */
		vs_redraw();
		break;
	}

	level--;
}

/*********** Here some language configurable messages	***********/
/* Menu de help */
static char *help[] = {
#if ENGLISH
	"C - Calculator",
	"D - DOS",
	"H - This menu",
	"I - Ideafix Char. Set",
	"K - Keyboard help",
	"P - Print screen",
	"R - Redraw",
	"X - Stop WM",
	NULL,
#endif

#if SPANISH
	"C - Calculadora",
	"D - Ir a DOS",
	"H - Menu de ayuda",
	"I - Juego de caracteres",
	"K - Ayuda teclado",
	"P - Imprimir pantalla",
	"R - Redibujar pantalla",
	"X - Salir del WM",
	NULL,
#endif

};

#define CALCULATOR 		0
#define CALL_SHELL 		1
#define AYUDA   		2
#define I_CHSET  		3 
#define KEY_HELP 		4
#define PRNT_SCR 		5
#define REDRAW 			6 
#define EXIT_WM 		7 

/* --- CONFIG --- */

/*
*	This function popup a window to select a service option.
*/
void do_help(void)
{
	static int ncols = -1;
	static int nrows = -1;
	int tmp; 
	char **p;
	char WLabel[128];
	static int level = 0;

	if(++level > 4){
 		level--;
		w_beep();
		return;
	}

	/* Calculate number of rows & columns needed			*/
	if (ncols == -1) {
		for (p=help, ncols=0, nrows = 2; *p; p++, nrows++) {
			if ((tmp=StrDspLen(*p)) > ncols)
				ncols = tmp;
		}
		ncols += 2;
	}
	
	/* Show a Menu and get the user selection			*/
	switch( PopUpVMenu(nrows, ncols, _WmErrmsg("POP_TITLE"), help) ) {
	case KEY_HELP:
		sprintf(WLabel, "[%s %s]", _WmErrmsg("HELP_TITLE"), TermName);
		WiHelp(8, 40, WLabel, hlptxt, (char*)NULL);
		break;
		
	case REDRAW:
		vs_redraw();
		break;

	case AYUDA:
		break;
		
	case PRNT_SCR:
		_print_scr();
		break;

	case I_CHSET:
		_print_ichset();
		break;

	case CALCULATOR:
		(void)w_calculator(	WiLine(SCREEN)+2, WiCol(SCREEN), 0,
							last_number, CAL_LEN);
		break;

	case CALL_SHELL:
		GoToShell();
		break;
		
	case EXIT_WM:
		Stop(0);
		break;
	}

	level--;
}
#endif /* HAVE_TTY */


#ifndef HAVE_PROC

/* --------- WMDEMO --------- */

static void _end_demo(void) { }
static UChar _read_demo_file(void) { return 0; }

#else

FILE *fin;
int last_cmd;
int ntimes;

#define MAX_BUFF  80

static void _end_demo(void)
{
	if (fin == NULL) return;

	if (!read_demo) {
		if (last_cmd >= 128) {
			fprintf(fin, ".%s", _CharName(last_cmd | 0x100));
			if (ntimes > 1)
				fprintf(fin, " %d", ntimes);
			fputc('\n', fin);
		}
	}
	fclose(fin);
}

static UChar _read_demo_file(void)
{
	static int step = 0;
	static int delay = 0;
	static int lineno = 0;
	static char _far buf[BUFSIZ];
	static char *pt = NULL;

	if (fin==NULL) {
		if (read_demo) {
			if ((fin = fopen(dem_file, "r"))==NULL) {
				fprintf(stderr,"No puedo abrir %s\n", dem_file);
				Stop(1);
			}
		}
		else {
			if ((fin = fopen(dem_file, "w"))==NULL){
				fprintf(stderr,"No puedo abrir %s\n", dem_file);
				Stop(1);
			}
			else {
				fprintf(fin, "# ------- %s ----------", dem_file);
			}
		}
		OnStop(_end_demo);
	}

	if (!read_demo) {
		long t  = 0;
		UChar c = _vs_getc();
		if (c == K_REDRAW) {
			t = time(NULL);
			c = _vs_getc();
			t = time(NULL) - t;
		}

		if (last_cmd >= 128 && (t || last_cmd != c)) {
			fprintf(fin, ".%s", _CharName(last_cmd | 0x100));
			if (ntimes > 1)
				fprintf(fin, " %d", ntimes);
			fputc('\n', fin);
			ntimes = 0;
		}

		if (t) {
			if (last_cmd < 128 && last_cmd != 0) fputc('\n', fin);
			fprintf(fin, ".SLEEP %d\n", t);
		}

		if (c == '.' && (last_cmd>128 || last_cmd == 0))
			fputc('.', fin);
		if (c < 128)
			fputc(c, fin);
		else {
			if (!t && last_cmd < 128 && last_cmd != 0) fputc('\n', fin);
			ntimes++;
		}

		return (UChar) (last_cmd = (int)c);
	}
	if (ntimes) {
		ntimes --;
		return (UChar)last_cmd;
	}

	if (pt!=NULL && *pt != '\0' && *pt != '\n') /* Returning a string */
		return *(pt++);

	forever {
		++lineno;
		if (fgets(buf, sizeof buf, fin) == NULL) {
			rewind(fin);
			lineno = 0;
			continue;
		}

		if (buf[0] == '#' || buf[0] == '\n')
			continue;

		if (buf[0] == '.' && buf[1] != '.') {
			char *cmd = buf+1, *arg;

			arg = cmd;
			while(!isspace(*arg)) arg++;
			*arg++ = 0;
			while (isspace(*arg)) arg++; 

			if (strncmp(cmd, "SLEEP", 5)==0) {
				SLEEP(atoi(arg)); continue;
			}
			if (strncmp(cmd, "PAUSE", 5)==0) {
				msgCtrl(MSG_PRESERVE);
				Warning(*arg && *arg!='\n' ? arg:_GnErrmsg(HITANYKEY));
				(void) _vs_getc();
				msgCtrl(MSG_RELEASE);
				continue;
			}
			if (strncmp(cmd, "DELAY", 5)==0) {
				delay = atoi(arg); continue;
			}
			if (strncmp(cmd, "COMMENT", 7)==0) {
				int filas,columnas=0,i,centrar=FALSE,letras;
				long aux = ftell(fin);
				char buff[MAX_BUFF];

				if (*arg=='C') centrar=TRUE;

				for (filas=letras=0; fgets(buff, MAX_BUFF, fin)!=NULL &&
					strncmp(buff, ".END COMMENT", 12)!=0; filas++) {
					if ((i=StrDspLen(buff)) > columnas) columnas=i;
					letras += i;
				}
                fseek(fin, aux, 0);
				WiCreate(SCREEN, WiLine(SCREEN), WiCol(SCREEN),
					filas, columnas+2, SLINE_BORDER|A_GREEN_BG|A_REVERSE, "", 
					A_GREEN_BG);
				WiSetAttr( A_BOLD | A_GREEN_BG);
				for (i=0; i<filas; i++) {
					int j,k;
					char blancos[81];
					fgets(buff, MAX_BUFF, fin);
					if (i==filas-1) buff[strlen(buff)-1]='\0';
					if (!centrar)
						WiPuts(" ");
					else {
						j=(columnas-StrDspLen(buff))/2;
						for (k=0; k<=j; k++) blancos[k]=' ';
						blancos[k]='\0';
						WiPuts(blancos);
					}
					WiPuts(buff);
				}
				fgets(buff, MAX_BUFF, fin);
				WiRefresh();
				SLEEP(letras/15+1);
				WiDelete(WICURRENT);
				WiRefresh();
				continue;
			}
			if (strncmp(cmd, "STEP", 4)==0) {
				step = atoi(arg); continue;
			}
			if (strncmp(cmd, "STOP", 4)==0) 
				Stop(0);	

			if ((ntimes = atoi(arg))) ntimes--;
			if ((last_cmd = _FindCharCode(cmd)) != ERR) {
				pt = NULL;
				if (step) _vs_getc();
				else if (delay) SLEEP(delay);
				return (UChar)last_cmd;
			}
			fprintf(stderr,"linea %d:Mandato invalido %s\n",lineno,cmd);
			Stop(1);
		}
		else {
			pt = buf;
			if (*pt == '.') pt++;
			return (UChar)*(pt++);
		}
	}
}

/*
	This is really bad, but it seems to be a problem
	with the SIAGLRM signal & the clock in the window manager that
	messes up the sleep.
	Im Sorry CPU!
*/
static void SLEEP(int n)
{
	long l = time(NULL);

	if (n > 60) n = 60;
	while((time(NULL) - l) < n)
		;
}

void _WiSetServices(void)
{
	handle_serv = (bool) wm_par[0].i;
}

void _WiService(void)
{
	FP func;
	if ((func = GetMenuBuiltIn((char*)wm_par[0].s))) {
		(*func)();
		wm_par[0].i = OK;
	} else
		wm_par[0].i = ERR;
}
#endif /* HAVE_PROC */

/*
 *
 * _WiSetRaw(): habilita o deshabilita el modo raw para el proceso
 *              corriente
 *
 */
void _WiSetRaw(void)
{
	curr_raw = wm_par[0].i;	
}


/*
 *
 * _WiKeyReady(): Checks if keyboard contains characters
 *
 */
void _WiKeyReady(void)
{
	wm_par[0].i = (short)_KeyReady();
}
