/********************************************************************
* Copyright (c) 1987 InterSoft Co.  All Rights Reserved
* THIS IS UNPUBLISHED PROPRIETARY SOURCE CODE OF InterSoft Co.
* The copyright notice above does not evidence any
* actual or intended publication of such source code.
*
* wiexmenu.cc,v 1.1.1.1 1996/04/16 18:52:17 gustavof Exp
*
* DESCRIPTION:
*      Routines to execute a menu contained in a file.
*      Rutinas relacionadas con la ejecucion de menues,
*      leyendo su especificacion de un archivo.
*      La sintaxis de los mismos se  en 'OpenMenu'.
*
* ROUTINE       |  PURPOSE
*---------------+---------------------------------------------------
*ExecMenu		| Execute a menu on a window.
*OpenMenu		| Read a file with a menu specification
*CloseMenu		| Free memory allocated by OpenMenu.
*********************************************************************/

#ifndef PCLIENT

#include <string.h>
#include <unistd.h>

#include <ifound.h>
#include <ifound/str.h>
#include <ideafix/priv/gndefs.h>
#include <cfix.h>

// Cracker.
#include <ideafix/priv/functype.h>
extern "C" int WiGetPassword(char *, char *);
static FPCRACK p_execcrack = NULL;
void SetCracker(FPCRACK p)
{
	p_execcrack = p;
}

/* +++ CONFIG +++ */
#define BUFFSIZE 256
#define MENU_DEPTH	64
/* --- CONFIG --- */

#define MN_SYNTAX		1
#define MN_OPTYPE		2
#define MN_MANYOPTS		3


/* ++ Prototypes ++ */

static char *m_getopt(MENU *p, int n);
static void execcom(MENU *mn, int opt);
static void TabToChar(char *from, char *to, UChar c);
static char *SaveString(char *word);
static char *ParseString(char *line, char **word);
static int  NTabs(char *p);
static int ParseMenuLine(char *buff, MENU *menu);
static int MenuOptionType(char *opname);

/* -- Prototypes -- */


/* Mask to obtain child's return code					*/
#define MSK 0xff00

/*
 * Nombres de las opciones en un menu.
 */
static struct {
	char *name;
	int cmd;
} _men_types[] = {
	{ "WCMD", 	 MT_WCMD      },
	{ "MENU", 	 MT_MENU      },
	{ "SHELL",	 MT_SHELL     },
	{ "PIPE", 	 MT_PIPE      },
	{ "BUILTIN", MT_BUILT_IN  },
	{ "CD",		 MT_CD        },
	{ "FORM",	 MT_FORM      },
	{ "SETENV",	 MT_SETENV    },
	{ "RWCMD", 	 MT_RWCMD     },
	{  NULL,     	 0         }
};

static struct s_builtin {
	char name[16];
	FP   rut;
	struct s_builtin *next;
} *builtin = NULL;

static char *menu_names[MENU_DEPTH] = {NULL};
static char **pm = NULL;

static CPFPCPCPI p_execform = NULL;
static IFPCP p_setenv = NULL;

void SetExecForm(CPFPCPCPI rut)
{
	p_execform = rut;
}

void SetEnvVar(IFPCP rut)
{
	p_setenv = rut;
}

/*
 * ExecMenu:
 *
 * Ejecuta el menu cuyo archivo es 'nombre' en un window con label
 * 'titulo'.
 * Retorna ERR si no encuentra el archivo o si tiene errores de
 * sintaxis.
 *
 */
int ExecMenu(char *titulo, char *nombre, int flag)
{
	MENU *mn;

	if (!pm) pm = menu_names - 1;
	if ((mn = OpenMenu(nombre, flag & MENU_MSGERR)) == NULL)
		return ERR;

#ifndef HAVE_TTY
	if (pm - menu_names < MENU_DEPTH && (flag & (MENU_MENU|MENU_PGM)))
#else
	if (pm - menu_names < MENU_DEPTH && (flag & MENU_MENU))
#endif
		WiSetAplHelp(*++pm = nombre, "");

	PopUpMenu(mn->nroopts+2, mn->ancho, titulo,
			(CPFPCPI) m_getopt, (char *) mn, (FPCPPI) execcom,
			(char **) mn, (flag & MENU_MENU) ?
#ifdef EXEC_TAKES_TIME
					POP_DEFAULT|POP_BLINK
#else
					POP_DEFAULT
#endif
			: POP_VOLATIL);

#ifndef HAVE_TTY
	if (pm > menu_names && (flag & (MENU_MENU|MENU_PGM)))
#else
	if (pm > menu_names && (flag & MENU_MENU))
#endif
		WiSetAplHelp(*--pm, "");
	CloseMenu(mn);
	return OK;
}

/* Rutina de lectura para pasarle a popup 					*/
static char *m_getopt(MENU *p, int n)
{
	return n > p->nroopts || n < 1 ? 0 : p->opt[n-1].titulo;
}

/*
 * execcom:
 *
 * Ejecuta el comando correspondiente al numero 'opt' contenido en
 * la estructura a la que apunta 'mn'
 *
 * Se utilizan las rutinas:
 *
 *          WiExecCmd()   ---  ejecutar programa usuario
 *			ExecShell()   ---  ejecutar comando shell
 *			ExecPipe()    ---  ejecutar pipe
 *			ExecMenu()	  ---  ejecutar menu
 *
 */
static void execcom(MENU *mn, int opt)
{
	char *dir1;
	struct MENU::menu_item *mi = & mn->opt[opt];
	bool restart = false;
	char aux[32];

	/* cambio de directorio */
	if (mi->dir)  {
		if (chdir(mi->dir) == ERR) {
			strcpy(aux, "CD ");
			strncpy(aux + 3, mi->dir, 28);
			aux[31] = 0;
			WiMsg((char *) _WiErrmsg(CANNOT_EXEC), aux);
			return;
		}
	}
	do {
		char *accion = mi->accion;
		if (mi->form && p_execform) {
			restart = mi->options & EF_OPT_R;
			WiSetAplHelp(mi->form, "");
			accion = (*p_execform)(mi->form, mi->accion, mi->options);
			if (accion == NULL) {
				strcpy(aux, "FORM ");
				strncpy(aux+5, mi->form, 26); aux[31] = 0;
				WiMsg((char *) _WiErrmsg(CANNOT_EXEC), aux);
				break;
			}
			else if (*accion == 0) break;
			if (mi->options & EF_OPT_D) continue;
		}

		switch (mi->tipo_acc) {
		case MT_MENU:
			ExecMenu(mi->titulo, accion, MENU_DEFAULT);
			break;
		case MT_PIPE:
			ExecPipe(accion, mi->wnfils, mi->wncols,
					(attr_type) mi->wborder, mi->titulo);
			break;

		case MT_WCMD:
#ifdef EXEC_TAKES_TIME
			if (!mi->form || !p_execform)
				HiLiteMenu();	/* make the sel. opt. blink */
#endif
			if (WiExecCmd(mi->titulo, accion) == ERR)
				WiMsg(_WiErrmsg(CANNOT_EXEC), accion);
			break;

		case MT_RWCMD :
		{
#ifdef EXEC_TAKES_TIME
			if (!mi->form || !p_execform)
				HiLiteMenu();	/* make the sel. opt. blink */
#endif

			if (p_execcrack) {
				char *cp;
				for (cp = accion; cp && *cp != ' ' && *cp != '\t'; cp++)
					;
				String host(accion, cp - accion),
					   command(cp);

				(void)(*p_execcrack)(command, host, noDebug, false, NULL_STRING);
			}

			break;
		}

		case MT_SHELL:
#if !defined(HAVE_TTY) && !defined(IDEAWIN) && !defined(HAVENT_UI)
			_PcCls();
#endif
			ExecShell(accion);
			break;
		case MT_BUILT_IN :
			/* busco la funcion */
			{
				struct s_builtin *p;
				for (p = builtin; p; p = p->next)
					if (strncmp(p->name, accion, sizeof(p->name))==0)
						break;
				if (p)
					(*p->rut)();
				else
					WiMsg((char *) _WiErrmsg(CANNOT_EXEC), accion);
			}
			break;
		case MT_SETENV:
			if ((*p_setenv)(accion) == ERR) {
				strcpy(aux, "SETENV ");
				strcat(aux, accion);
				WiMsg((char *) _WiErrmsg(CANNOT_EXEC), aux);
				break;
			}
		}
	} while (restart);

	/* restauro el directorio */
	for (dir1 = mi->dir; dir1 && *dir1; dir1++)
		if (*dir1 == '/' || dir1 == mi->dir) chdir("..");

 	/* restore application help file */
 	WiSetAplHelp(*pm, "");

	/* update time */
	WiRefresh();
}

/*
 * OpenMenu:
 *
 * Rutina que abre un archivo de menu  (xxxxxx.mn) e intenta cargarlo
 * en la estructura MENU
 *
 * SINTAXIS DEL ARCHIVO FUENTE :
 *
 * lineas: titulo - tipo - accion (strings), separados por " " o TAB.
 *
 * # Introduce comentario.
 *
 * Si titulo consta de varias palabras se deben poner entre comillas.
 */

MENU *OpenMenu(char *nombre, int f)
{
	FILE  *fp;
	char buff[BUFFSIZE];
	MENU *menu;
	int  lineno, status;

	/*
		Try to open the file with the menu's definition.
	*/
	if ((fp = FopenPath(nombre, ".mn", "r")) == NULL) {
		if (f)
			WiMsg((char *) _WiErrmsg(E_MN_NOFILE), nombre);
		else
			w_beep();
		return NULL;
	}

	/* Allocate a structure for the menu data. */
	menu = (MENU *) Alloc(sizeof(MENU));

	/* Loop de lectura de caracteres hasta finalizar el archivo */
	lineno = 0;
	status = OK;
	while (status == OK && fgets(buff, BUFFSIZE, fp) != NULL) {
		lineno++;
		status = ParseMenuLine(buff, menu);
	}
	fclose(fp);

	/* Tratar los errores */
	switch (status) {
	case MN_SYNTAX:
		CloseMenu(menu);
		WiMsg((char *) _WiErrmsg(E_MN_SYNTAX), nombre, lineno, buff);
		return NULL;

	case MN_OPTYPE:
		CloseMenu(menu);
		WiMsg((char *) _WiErrmsg(E_MN_OPTYPE), nombre, lineno, buff);
		return NULL;

	case MN_MANYOPTS:
		WiMsg((char *) _WiErrmsg(E_MN_MANYOPTS), nombre, MAX_OPCIONES_MENU);
		break;
	}

	/* Check if Null menu file				*/
	if (menu->nroopts == 0) {
		CloseMenu(menu);
		WiMsg((char *) _WiErrmsg(E_MN_EMPTY), nombre);
		return NULL;
	}
	return menu;
}

/* 
 * CloseMenu:
 *
 *  Rutina que realiza el free de los buffers alocados para el menu
 *
 */
void CloseMenu(MENU *menu)
{
	register int i;

	for(i=0;i<menu->nroopts;i++) {
		Free(menu->opt[i].titulo);
		Free(menu->opt[i].accion);
		if (menu->opt[i].dir) Free(menu->opt[i].dir);
		if (menu->opt[i].form) Free(menu->opt[i].form);
	}
	Free(menu);
}

/*
* devuelve la rutina built in correspondiente
*/
FP GetMenuBuiltIn(char *name)
{
	struct s_builtin *p;

	for (p = builtin; p; p = p->next)
		if (strncmp(p->name, name, sizeof(p->name))==0)
			return (FP) p->rut;

	return NULL;
}

/*
* setea una rutina como una nueva built in
*/
int SetMenuBuiltIn(char *name, FP rut)
{
	struct s_builtin *p;

	for (p = builtin; p; p = p->next)
		if (strncmp(p->name, name, sizeof(p->name))==0) {
			p->rut = rut;
			return OK;
		}
	p = (struct s_builtin *) Alloc(sizeof(struct s_builtin));
	strncpy(p->name, name, sizeof(p->name));
	p->rut  = rut;
	p->next = builtin;
	builtin = p;
	return OK;
}

static void TabToChar(char *from, char *to, UChar c)
{
	int index=0, i=0;

	while (*from || i) {
		if (i) {
			*to++ = c;
			i--;
		}
		else {
			if (*from != '\t') {
				*to++ = *from++;
			}
			else {
				i = 4 - (index%4);
				index--; /* due to the tab char */
				from++;
			}
		}
		index++;
	}
	*to = 0;
}

/**************************************************************/

/*
Format of a menu line:

 "titulo" [CD dir] [FORM [-r -d -k -i] form] TIPO cmd

 TIPO: WCMD
	   SHELL
	   MENU   
       PIPE [fils] [cols]
*/

static int ParseMenuLine(char *buff, MENU *menu)
{
	char *pbuff, *word;          /* Para recorrer al ir parseando */
	int  largo;
	struct MENU::menu_item *mi;

	/* Elimino los blancos y tabs iniciales 	*/
	pbuff=buff;
	while (isspace((UChar)*pbuff)) ++pbuff;

	/* Si es un comentario lee otra linea 		*/
	if (*pbuff=='#' || *pbuff == '\0' ) return OK;

	/* Sacar todo lo que sea blanco al final	*/
	word = pbuff + strlen(pbuff) - 1;
	while (isspace(*word)) --word;
	word[1] = 0;

	/* Ver si hay lugar para esta opcion		*/
	if (menu->nroopts == MAX_OPCIONES_MENU)
		return MN_MANYOPTS;
	mi = &menu->opt[menu->nroopts];

	/* Leo titulo  								*/
	pbuff = ParseString(pbuff, &word);
	if (*pbuff == '\0')
		return MN_SYNTAX;
	mi->titulo = (char *)Alloc(strlen(word) + 4*NTabs(word) + 1);
	TabToChar(word, mi->titulo, ' ');

	/* Calcular maxima longitud visible			*/
	largo = StrDspLen(mi->titulo);
	if (menu->ancho < (largo+2)) menu->ancho = largo+2;

	/* [CD dir] */
	pbuff = ParseString(pbuff, &word);
	if ((mi->tipo_acc=MenuOptionType(word))==MT_CD)  {
		pbuff        = ParseString(pbuff, &word);
		mi->dir      = SaveString(word);
		pbuff        = ParseString(pbuff, &word);
		mi->tipo_acc = MenuOptionType(word);
	}

	/* [FORM [-r -d -k -i] form] */
	if (mi->tipo_acc == MT_FORM) {
		mi->options = 0;
		while (isspace(*pbuff)) pbuff++;
		while (*pbuff == '-') {
			switch (*(++pbuff)) {
				case 'k': mi->options |= EF_OPT_K; break;
				case 'i': mi->options |= EF_OPT_I; break;
				case 'd': mi->options |= EF_OPT_D; break;
				case 'r': mi->options |= EF_OPT_R; break;
				default : 
					/* It's an invalid option */
					buff[0] = '-'; buff[1] = *pbuff; buff[2] = 0; 
					return MN_OPTYPE;
			}
			++pbuff;
			while (isspace(*pbuff)) ++pbuff;
		}
		pbuff        = ParseString(pbuff, &word);
		mi->form     = SaveString(word);
		pbuff        = ParseString(pbuff , &word);
		mi->tipo_acc = MenuOptionType(word);
	}	

	/* TIPO [filas] [cols] cmd */		
	if (mi->tipo_acc == MT_PIPE) {
		int nfils = 0, ncols = 0;

		mi->wborder = DLINE_TYPE | A_REVERSE;
		while (isspace(*pbuff)) ++pbuff;
		while (isdigit((UChar) *pbuff)) 
			nfils = nfils*10 + *pbuff++ - '0';
		if (nfils) mi->wborder |= (TOP_BORDER|LOW_BORDER);

		while (isspace((UChar) *pbuff)) ++pbuff;
		while (isdigit((UChar) *pbuff)) 
			ncols = ncols*10 + *pbuff++ - '0';
		if (ncols) mi->wborder|=(RIGHT_BORDER|LEFT_BORDER);
		while (isspace((UChar) *pbuff)) ++pbuff;

		mi->wnfils = (short)nfils;
		mi->wncols = (short)ncols;
	}

	if (mi->tipo_acc == ERR) {
		strcpy(buff, word);
		return MN_OPTYPE;
	}

	/* Cargo el final de la linea como comando 						*/
	while (isspace(*pbuff)) ++pbuff;
	if (*pbuff == '\0')
		return MN_SYNTAX;
	mi->accion = SaveString(pbuff);
	menu->nroopts++;
	return OK;
}

static int MenuOptionType(char *opname)
{
	int cant;

	for(cant=0; _men_types[cant].name!=NULL && 
				str_ne(_men_types[cant].name,opname); cant++)
		;

	return _men_types[cant].name==NULL ? ERR :_men_types[cant].cmd;
}

/*

Tomar el siguiente string de "line". Si esta entre comillas lo interpreta.
Retorna puntero al caracter luego del string tomado. En word deja
puntero al string terminado en \0.

                    +pbuff
                    |  
 "dsafsa fdsafsafs "   XXXX
  ^                ^
  +--- word        + \0

*/
static char *ParseString(char *line, char **word)
{
	bool quotes = false;
	
	while (isspace(*line)) line ++;
	quotes = line[0] == '"';
	*word = quotes ? line+1 : line; 
	if (quotes) {
		line ++;
		while (*line && *line != '"') line ++;
	}
	else
		while (*line && !isspace(*line)) line ++;

	if (*line) { *line = 0; return ++line; }	
	else return line;
}

static char *SaveString(char *s)
{
	char *p = (char*)Alloc(strlen(s) + 1);
	strcpy(p, s);
	return p;
}

static int NTabs(char *p)
{
	int count = 0;
	
	while (*p) if (*(p++) == '\t') count++;
	return count;
}

/**************************************************************/

#endif
