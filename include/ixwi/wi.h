/*********************************************************************
* Copyright (c) 1987 InterSoft Co.  All Rights Reserved
* THIS IS UNPUBLISHED PROPRIETARY SOURCE CODE OF InterSoft Co.
* The copyright notice above does not evidence any
* actual or intended publication of such source code.
*
* $Id: wi.h,v 1.16 2004/06/25 20:21:45 diegoa Exp $
*
* DESCRIPTION
* Definition & interface file for WI routines.
*********************************************************************/

#ifndef WI_H
#define WI_H

#include <cfix/types.h>

#if HAVE_PRAGMA_PACK
#	pragma pack(4)
#endif

/*
 * Definiciones de los atributos de pantalla
 */


typedef WINDOWTYPE window;

/* Various video attributes */

#define A_NORMAL	(attr_type) 0x000000
#define A_UNDERLINE	(attr_type) 0x000100
#define A_REVERSE	(attr_type) 0x000200
#define A_BLINK		(attr_type) 0x000400
#define A_BOLD		(attr_type) 0x000800
#define A_ALTERNATE	(attr_type) 0x001000
#define A_STANDOUT	(attr_type) 0x002000
#define A_BLACK		(attr_type) 0x000000
#define A_RED		(attr_type) 0x010000
#define A_GREEN		(attr_type) 0x020000
#define A_BLUE		(attr_type) 0x040000
#define A_BLACK_BG	(attr_type) 0x000000
#define A_RED_BG	(attr_type) 0x100000
#define A_GREEN_BG	(attr_type) 0x200000
#define A_BLUE_BG	(attr_type) 0x400000
#define A_INVISIBLE	(attr_type) 0x804000
#define A_ITALIC    (attr_type) 0x008000

#define A_YELLOW	(A_GREEN|A_RED)
#define A_YELLOW_BG	(A_GREEN_BG|A_RED_BG)

#define A_CYAN		(A_GREEN|A_BLUE)
#define A_CYAN_BG	(A_GREEN_BG|A_BLUE_BG)

#define A_MAGENTA	 (A_RED|A_BLUE)
#define A_MAGENTA_BG (A_RED_BG|A_BLUE_BG)

#define A_WHITE		(A_RED|A_BLUE|A_GREEN)
#define A_WHITE_BG	(A_RED_BG|A_BLUE_BG|A_GREEN_BG)

/*
 * Constantes relativas al tipo de border
 *   +-----------------------------------+
 *   | Atr3 | Atr2| Atr1 | Limits - Tipo |
 *   +-----------------------------------+
 *    MSB
 */
/* tipo de borders */

#define BLANK_TYPE	0

/* Selecciona el caracter mas adecuado segun la terminal */
#define STAND_TYPE	1

#define SLINE_TYPE	2
#define DLINE_TYPE	3
#define ASTSK_TYPE	4

/* Atributos para el PopUpMenu */

#define POP_STATIC	1	/* el menu queda si eje. op.	 		*/
#define POP_VOLATIL	2	/* el menu desaparece si ejec. op.		*/
#define POP_BORDER	4	/* el borde cambia a simple si ejec. op.*/
#define POP_BLINK	8	/* la opcion se pone en blinking si ejec. op. */
#define POP_DEFAULT POP_STATIC|POP_BORDER

/* Flags para el ExecMenu	   */

#define MENU_MENU		1
#define MENU_PGM		2
#define MENU_MSGERR		4
#define MENU_DEFAULT	MENU_MENU|MENU_MSGERR
#define MENU_AUTO_BUILTIN	8

/* Zonas donde se dibujaran */

#define NO_BORDER		(attr_type) 0
#define TOP_BORDER		(attr_type) 0x10
#define LOW_BORDER		(attr_type) 0x20
#define LEFT_BORDER		(attr_type) 0x40
#define RIGHT_BORDER	(attr_type) 0x80
#define ALL_BORDER	    (attr_type) 0xF0

/* Constantes para usar directamente en create_win */

#define STAND_BORDER    (ALL_BORDER|A_REVERSE|STAND_TYPE)
#define SLINE_BORDER	(ALL_BORDER|SLINE_TYPE)
#define DLINE_BORDER	(ALL_BORDER|DLINE_TYPE)
#define ASTSK_BORDER	(ALL_BORDER|ASTSK_TYPE)

/* Constates para el modo de trabajo del WM */

#define	WM_NORMAL	0
#define WM_PURERAW	1
#define WM_RAW		2

/* Constates para acceder a la windos screen y current */

#define SCREEN		0
#define WICURRENT	1

/* Constants for WiDialog */

typedef long wdflag;
#define WD_PRESERVE		0
#define WD_OK			(1 << 0)
#define WD_YES			(1 << 1)
#define WD_NO			(1 << 2)
#define WD_ABORT		(1 << 3)
#define WD_RETRY		(1 << 4)
#define WD_IGNORE		(1 << 5)
#define WD_CANCEL		(1 << 6)
#define WD_FAIL			(1 << 7)
/* To show "Icon Stop" */
#define WD_ERROR		(1 << 8)
/* Do not show centered */
#define WD_HERE			(1 << 9)
#define WD_RELEASE		(1 << 10)

/* ESCAPE Sequences */
/*
char SCP[] = "\033[s";
char RCP[] = "\033[s";
char NORMAL[]	=	"\033[m";
char BOLD[]		=	"\033[1m";
char STANDOUT[]	=	"\033[2m";
char UNDERLINE[]=	"\033[4m";
char BLINK[]	=	"\033[5m";
char ALTERNATE[]=	"\033[6m";
char REVERSE[]	=	"\033[7m";
*/

/*
 * Flags para funciones de ingreso de campos
 */
#define WGET_DSPONLY	1
#define WGET_COMMA		2
#define WGET_SIGNED		4
#define WGET_AUTOENTER	8
#define WGET_CDIGIT		0x10
#define WGET_CDIGIT_DASH	0x20
#define WGET_CDIGIT_SLASH	0x40
/*
 * Maxima cantidad de caracteres en un texto de
 * descripcion de una tecla de funcion
 */
#define KEY_HELP_DSC	15

/*
 * Macros
 */
#define	BELL		'\007'
#ifdef IDEAWIN
#	ifdef __cplusplus
extern "C" {
#	endif
		extern void WiBeep(void);
#	ifdef __cplusplus
}
#	endif
#else
#   define WiBeep()    (WiPutc(BELL),WiFlush())
#endif
#define w_beep WiBeep

/*
 * Definiciones de Limites de Zonas de Pantalla.
 */
#define footer_orig() (WiHeight(SCREEN)-2)
#define footer_height() (2)
#define std_orig() (2)
#define std_height()   (WiHeight(SCREEN)-6)

/*------------------- MENU related functions ------------------------*/

#define MAX_OPCIONES_MENU  32

#define MT_WCMD		0
#define MT_MENU		1
#define MT_SHELL	2
#define MT_PIPE		3
#define MT_BUILT_IN	4
#define MT_CD		5
#define MT_SKIP     6
#define MT_FORM		7
#define MT_SETENV	8
#define MT_RWCMD	9
#define MT_LOCAL	10  /*execute commands locally */

/* Options for execform */

#define EF_OPT_K		1
#define EF_OPT_I		2
#define EF_OPT_D		4
#define EF_OPT_R		8

#ifdef __cplusplus
extern "C" {
#endif

typedef int (*fpHomeSearch)(int, int, int, int, char *, CPFPCPI);
typedef int (*fpSearchTxt) (Int, bool, const char *,char *, CPFPCPI, Int);

typedef struct {
	int  nroopts;			/* Number of options 				*/
	int  ancho;				/* Length of biggest title  		*/
	struct menu_item {
		char 		*titulo;
		int			tipo_acc;
		char 		*accion;
		short 		wnfils;
		short		wncols;
		Int		wborder;
		char		*dir;
		char		*form;
		short		options;
	} opt[MAX_OPCIONES_MENU];
} MENU;


/* ++ Prototypes ++ */

extern void 	WiSettty(void);
extern void 	WiConnect(char *tty, char *module, char *version);
extern void 	WiDisconnect(char *tty);
extern void 	WiSetAplHelp(char *mod, char *ver);
extern void 	WiResettty(void);
extern void 	WiHelpFile(char *filename, char *label);
extern int		WiMsg(char *s, ...);
extern window	WiCreate(int,int,int,int,int,attr_type,char*,attr_type);
extern window 	WiDefPar(void);
extern void 	WiPutc(UChar c);
extern int		WiHeight(window wi);
extern void 	WiPrintf(char *s, ...);
extern void 	WiDelete(window wi);
extern void 	WiDeleteAll(void);
extern window	WiSwitchTo(window n);
extern void 	WiRefresh(void);
extern void 	WiRedraw(void);
extern void 	WiSetBorder(attr_type border, char *label);
extern void 	WiStatusLine(bool f);
extern attr_type WiGetAttr(void);
extern int		WiOrgCol(window wi);
extern int		WiOrgRow(window wi);
extern int		WiLine(window wi);
extern int		WiCol(window wi);
extern UChar 	WiGetc(void);
extern void 	WiFlush(void);
extern UChar 	*WiKeyHelp( UChar key, UChar *s);
extern void 	WiPuts(char *s);
extern void 	WiHelp(int nf, int nc, char *lbl, CPFPCPI get_txt, char *garg);
extern int		ExecShell(char *p);
extern int		ExecMenu(char *titulo, char *nombre, int flag);
extern int		ExecSystem(const char *name);
extern void 	WiAplHelp(void);
extern int		WiHandler(int flag, const char *label, const char *msg);
extern int		SetMenuBuiltIn(char *name, FP rut);
extern FP 		GetMenuBuiltIn(char *name);
extern int		PopUpLMenu(int f, int c, const char *s, ...);
extern int		WiWidth(window wi);
extern void 	WiInterrupts(int flag);
extern int		_EnCodeParameters(int i, char *buff, bool ip);
extern int		_DecodeParameters(int *i, char *buff, bool ip);
extern int		_WiComRem(int cmd);
extern void 	WiSetDefPar(window wi);
extern int		ExecMenu(char *titulo, char *nombre, int flag);
extern MENU 	*OpenMenu(char *nombre, int f);
extern int		ExecPipe(char *cmd, int nf, int nc, attr_type b, char *label);
extern int		WiExecCmd(char *titulo, char *cmd);
#ifdef	__cplusplus
extern int		WiExecCmd2(char *titulo, char *cmd, int wait=0);
#else
extern int		WiExecCmd2(char *titulo, char *cmd,int wait);
#endif
extern void 	CloseMenu(MENU *menu);
extern int		SetMenuBuiltIn(char *name, FP rut);
extern int		PopUpMenu(int,int,char*,CPFPCPI,char*,FPCPPI,char**,int);
extern window 	WiParent(window wi);
extern int		WiCreateProces(int pid, char *module, char *name,char *version);
extern int		WiDeleteProces(int pid);
extern void 	SetExecServ(IFPICPCP fp);
extern void		SetExecForm(CPFPCPCPI rut);
extern void 	SetEnvVar(IFPCP rut);
extern void 	setHomeSearch(fpHomeSearch);
extern fpSearchTxt setSearchTxt(fpSearchTxt);
extern int		WiOpenChannel(char *tty, int c, bool create_it);
extern int		WiOpenSem(char *tty, bool create_it);
extern void		WiRmSem(int semid);
extern void 	WiRmChannel(int channel);
extern int		WiReceiveMsg(int msgid, char *msg, int msgsize, Int msgtype);
extern int		WiDrain(int msgid, int pid);
extern window 	WiCurrent(void);
extern void 	DisplayMessage(bool wait_conf, char *msg);
extern void 	DisplayMsg(bool f, char *s, ...);
extern void 	DisplayVMsg(bool wait_conf, char *fmt, va_list ap);
extern void 	ClearMsg(void);
extern void 	WiScroll(int num);
extern void 	WiSetAttr(attr_type attr);
extern void 	WiSetBackGr(attr_type attr);
extern void 	WiWrap(bool f);
extern void 	WiMoveTo(int f, int c);
extern void 	WiRMoveTo(int f, int c);
extern void 	WiEraEol(void);
extern void 	WiEraEop(void);
extern void 	WiErase(void);
extern void 	WiEraLine(void);
extern void 	WiDelLines(int nlin, int pos);
extern void 	WiDelLine(int pos);
extern void 	WiInsLines(int nlin, int pos);
extern void 	WiInsLine(int pos);
extern void 	WiDelChar(int nchar, int col);
extern void 	WiInsChar(int nchar, int col);
extern void 	WiCursor(int f);
extern void 	WiSetScroll(int top, int bot, int left, int right);
extern void 	WiKHelp(UChar key);
extern UChar 	WiInChar(int fil, int col);
extern int 		WiSendMsg(int msgid, char *_msg, int msgsize);
extern int 		WiGetField(type,UChar*,UShort,short,short,short,char*, char*);
extern attr_type WiInAttr(int fil, int col);
extern int 		GetCmd(char *buff);
extern int 		PopUpVMenu(int nfils, int ncols, char *label, char **argv);
extern int 		WiGetTab(void);
extern void 	WiSetTab(int n);
extern int 		WiProcess(int stopping);
extern void 	WiSetFOpen(int i);
extern void		WiSetRaw(int f);
extern int		WiKeyReady(void);
extern wdflag	WiDialog(wdflag, wdflag, const char *, char *,...);
extern wdflag	WiVDialog(wdflag flags, wdflag def, const char *w_title,
		char * fmt, va_list ap);
extern int		WiVMsg(int flag, char *fmt, va_list ap);
extern int WiGetPassword(char *, char *);
#if !MSDOS
extern void		WiSetServices(bool status);
extern bool		WiMove(window wi);
extern bool		WiResize(window wi);
extern int		WiService(char *str);
#else
extern void		HiLiteMenu(void);
#endif
extern UChar *	WiGets(UChar *s);
#ifdef IDEAWIN
extern void *	SetWaitCursor(void);
extern void *	SetStdCursor(void);
extern void		RestoreCursor(void *save_cursor);
#endif

#ifdef __cplusplus
}
#endif

/* -- Prototypes -- */

#if HAVE_PRAGMA_PACK
#	pragma pack()
#endif



/* +++++++ IXSCA ++++++++*/
/* -------  DA   --------*/

typedef struct {
        long hostId;
        char hostName[255];
        int idSession;
	int idUser;
        char  login[255];
        char  password[255];
	char profile[255];
        DATE expDate;
        TIME expTime;
//      bool inUse;
//      struct *nextNode;
} SNODE;


extern SNODE getSessionFromWM(long hostId);
extern bool setSessionToWM(long hostId, char* login, char* pass, int idUser, char* profile, int idSession, DATE expDate, TIME expTime);


/* ------- IXSCA --------*/

#endif
