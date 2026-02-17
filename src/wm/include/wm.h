/*********************************************************************
* Copyright (c) 1987 InterSoft Co.  All Rights Reserved
* THIS IS UNPUBLISHED PROPRIETARY SOURCE CODE OF InterSoft Co.
* The copyright notice above does not evidence any
* actual or intended publication of such source code.
*
* $Id: wm.h,v 1.6 2008/06/12 16:46:58 hardaiz Exp $
*                                                                   
* DESCRIPTION
* Definition & interface file for WM routines & the Window
* Manager process.
*
* Files that must be included before this:
* ----------------------------------------
* st.h
*********************************************************************/

/*
 * Definicion de la celda de video extendido 3 bytes pra el atributo
 * y uno para el caracter ppdicho.
 */

#ifdef SMALL_WM
#define SHORT_VS_CELL
#define NO_SERVICES
#endif

#ifdef SHORT_VS_CELL
typedef UShort VS_CELL;
#define VS_CHAR(x) ( (UChar) (x & 0xFF))
#define VS_ATTR(x) ( (x) & (VS_CELL) 0xFF00)
#define VS_BACKGR(x) ( (x) & (VS_CELL) (A_INVISIBLE|A_REVERSE))
#define VS_COLOR(x)  0
#define VS_BG_COLOR(x) 0
#else
typedef unsigned long VS_CELL;
#define VS_CHAR(x) ( (UChar) (INT((long)x) & 0xFF))
#define VS_ATTR(x) ( (x) & (VS_CELL) 0xFFFFFF00)
#define VS_BACKGR(x) ( (x) & (VS_CELL) \
				(A_INVISIBLE|A_RED_BG|A_BLUE_BG|A_GREEN_BG|A_REVERSE))
#define VS_COLOR(x)  ((x & A_WHITE) >> 16)
#define VS_BG_COLOR(x)  ((x & A_WHITE_BG) >> 20)
#endif

/* Estructura de un Window */
typedef struct wistru {
	int	 	pid;				/* Pid del proceso que la creo		*/
	short	f_org;				/* Fila de origen					*/
	short	c_org;				/* Col. de origen					*/
	short   nfils;				/* Cant. de filas					*/
	short   ncols;				/* Cant. columnas					*/
	struct {
		attr_type attr;			/* Tipo de borde  					*/
		short	f_org;			/* Borde: fila orig. 				*/
		short	c_org;			/* Borde: col. orig. 				*/
		short   f_last;			/* Borde: ult. fila. 				*/
		short   c_last;			/* Borde: ult. col.  				*/
	} border;
	short   c_fil;				/* Fila cursor       				*/
	short   c_col;				/* Col. cursor       				*/
	short   top_scrl;           /* Tope zona scroll  				*/
	short   bot_scrl;           /* Fondo zona scroll 				*/
	short   left_scrl;          /* Tope izq. zona scroll			*/
	short   right_scrl;         /* Tope der. zona scroll			*/
	attr_type attrib;           /* Atributo corriente      			*/
	attr_type backgr;           /* Background de la window			*/
	char   * save;			    /* Zona de memoria con datos de la
								   parte de VS sobreescrita        	*/
	UChar   flags;				/* Distintas caract. de la WINDOW 	*/
	struct wistru * parent; 	/* padre de la ventana 				*/
	struct wistru *	child; 		/* hijo de la ventana 				*/
	struct wistru *	next; 		/* proximo en la lista de hermanos 	*/
	struct wistru *	prev;		/* anterior en la lista de hermanos	*/ 
	short	sub;				/* subindice dentro de WINDOW_TAB	*/
	char	*label;				/* titulo de la ventana				*/
} *WINDOW;

extern WINDOW	wcurr;			/* Pointer to current window		*/

/* Valores para los flags de una ventana */

#define		WRAP_FLAG		2       /* Wrap ON/OFF (borde derecho) */
#define		LINEOVERFLOW	4       /* Marca, linea sobrepasa borde*/
#define		CURSOR			8	    /* Cursor visible or invisible */
#define		MODIF			16		/* La window a sido modificado */

/*
 * Elemento de la tabla traductora.
 * La tabla es armada de acuerdo a la informacion del archivo
 * de terminal.
 */
#define READ_NEXT	0

typedef struct trad {
	char c;					/* caracter a comparar		*/
	UChar  action;			/* READ => leer char_!READ => retornar */
	struct trad *match;		/* siguiente si hay match   */
	struct trad *nomatch;	/* siguiente si no hay match*/
} TRAD_ELEM;

/*
 * Tabla con las asignaciones de default a las teclas de
 * funcion y caracteres especiales. Para cada codigo por
 * arriba de 128 se le da:
 */

typedef struct	{

	UChar    code;						/* Codigo asociado			*/
	attr_type def_output; 				/* Output default */
	char    *def_key;					/* Secuencia tecla default	*/
	char 	descr[KEY_HELP_DSC+1];		/* String con nombre tecla  */

} KEY_TAB;

extern 	KEY_TAB	 _far key_tab[];

/*
 * Tabla con nombres de los atributos de output
 * y el valor interno que lo representa.
 *
 * La tabla se  en el modulo WMCONF.C
 */

#define MAX_LANG_NAME	30

typedef struct	{
	char 	lang_name[MAX_LANG_NAME];	/* Nombre en leng. configurado */
	char 	*std_name;					/* Nombre de la funcion std.   */
	attr_type	value;					/* Valor asociado			*/
} ATTR_TAB;

extern 	ATTR_TAB _far attr_tab[];


/* Para DOS modo de funcionamiento (Emulador de terminal o WM remoto) */

#define MODE_WM		0
#define MODE_TERM	1
#define MODE_WMR	2
extern UShort wm_mode;

/* maxima cantidad de tareas susp */
#define MAX_TASK	   10	

#define HUGE	30000

/* Para rec_vs_status() */
#define FREE 1
#define NOFREE 0

/* 
 *Definiciones para manejo de la terminal virtual 
 */
#ifdef WMVS

bool	scr_garbage;			/* Flag to mark screen as garbage */

short vsrow = 0;			/* Row location of SW cursor */
short vscol = 0;			/* Column location of SW cursor */
attr_type  vsattr   = 0;		/* SW Atribute */
attr_type  vsbackgr = 0;		/* Atribute */

static short psrow = HUGE;		/* Row location of HW cursor */
static short pscol = HUGE;		/* Column location of HW cursor */
static	attr_type  psattr;		/* HW Atribute */

#if !MSDOS
VS_CELL **vscreen;     	/* Virtual screen. */
static
VS_CELL **pscreen;     	/* Physical screen. */
#endif

static short *first;			/* First char modified in a line */

static short *last;				/* Last  char modified in a line */

short eol_length;				/* Long de la sec. para erase eol */
short   VS_LINES;				/* Nro. lineas de pantalla */
short   VS_COLS;				/* Nro. de columnas " "    */
short   TAB = 4;				/* Separacion entre tabs */
short	vscursor;				/* 1 if cursor display, 0 = no */

/*
 * Vector de traduccion de input
 *
 * Si el elemento tiene puntero 'tr' distinto NULL,
 * se debe traducir_De otro modo, se retorna el valor encontrado
 * en 'value'.
 */


struct far {
	UChar value;
	TRAD_ELEM *tr;
} _far chset[NCHSET];

/*
 * Vector de traduccion de output.
 * Cada valor a enviar tiene su atributo, y el valor del caracter.
 */
attr_type _far out_tab[NCHSET];

/* 
 * Vectores donde estan los colores de la terminal en uso.
 */
#define MAXCOL	8

char * col_fg[MAXCOL];
char * col_bg[MAXCOL];

/*
 * Variables globales
 */
bool has_line_drawing_chars;  /* Sin comentarios ! */
bool has_idlsec;			  /* Has insert-delete line secuences */
bool has_setscr;			  /* Has Set scroll  secuence */
bool has_color = FALSE;	  	  /* Has color */
bool tty_set;				  /* Tty was set */
bool term_rev = 1;			  /* Si la terminal tiene reverse */
short wm_intr_char;	  		  /* Interrupt character */


#else

extern bool scr_garbage, tty_set;
extern short wm_intr_char;
extern short vsrow, vscol, eol_length;
extern attr_type vsattr;
extern attr_type vsbackgr;
#if !MSDOS
extern VS_CELL **vscreen;
#endif
extern short   VS_LINES;			/* Nro. lineas de pantalla */
extern short   VS_COLS;				/* Nro. de columnas " "    */
extern short   TAB;					/* Separacion entre tabs */
extern short vscursor;
extern attr_type _far out_tab[];
extern char * col_fg[];
extern char * col_bg[];
extern struct far {
	UChar value;
	TRAD_ELEM *tr;
} _far chset[];
extern bool has_line_drawing_chars;
extern bool has_idlsec, has_setscr;
extern bool has_color;
extern bool term_rev;

#endif

/*
 * Macros
 */
#define vs_moveto(r,c)((vsrow=r),(vscol=c))
#define vs_getcur(r,c)((*r = vsrow),(*c = vscol))
#define vs_setattr(a)(vsattr = a)
#define vs_getattr()(vsattr)
#define vs_cursor(n) (vscursor=n)
#define vs_setbackgr(a)(vsbackgr = a)

#define vs_erase(r,c,nr,nc) _vs_fill((attr_type)' ' | vsbackgr,  \
									 r, c, nr, nc)
#define vs_eol() _vs_fill((attr_type)' ' | vsbackgr, vsrow, vscol, \
						  1,VS_COLS-vscol)
#define vs_cls() (_vs_fill((attr_type)' ' | vsbackgr, 0, 0, \
				  VS_LINES, VS_COLS), scr_garbage = TRUE)
#define vs_redraw() (scr_garbage = TRUE)
#define vs_refresh() _vs_update(0, VS_LINES-1)

#if MSDOS
VS_CELL vscreen_get(int i, int j);
void	vscreen_set(int i, int j, VS_CELL c);
char	*vscreen_alloc(int size);
void	vscreen_free(void);
int		vscreen_save_to_buf(char *buf, int i, int j, int n);
int		vscreen_rec_from_buf(char *buf, int i, int j, int n);
#else
#	define vscreen_get(i,j)		(vscreen[i][j])
#	define vscreen_set(i,j,c)	(vscreen[i][j] = (c))
#	define vscreen_alloc(size)	(char *) Alloc(sizeof(VS_CELL) * (size))
#	define vscreen_save_to_buf(buf, i, j, n)	\
		(memcpy((buf),(char*)&vscreen[i][j],(n*=sizeof(VS_CELL))), n)
#	define vscreen_rec_from_buf(buf, i, j, n)	\
		(memcpy((char*)&vscreen[i][j], (buf), (n*=sizeof(VS_CELL))), n)
#endif

/* leer un caracter de la pantalla virtual */
#define vs_inchar(i,j) VS_CHAR(vscreen_get(i, j))

/* leer atributo de posicion de pantalla virtual */
#define vs_inattr(i,j) VS_ATTR(vscreen_get(i, j))

/* 
* Manejo de la pantalla fisica
*/

/*
* Another general functions
*/
extern char	*wm_errtab[];
extern int size_wmerrtab;

/* Macro to get a wm msg */

#define wm_msg(n) (n>0 && n<size_wmerrtab ? wm_errtab[n] : "")

/* ++ Prototypes ++ */

void	KillChildren(void);
int		_ForkPgm(void);
int		_WiExeServ(int pid, char *argv, char *titulo);
void	ExitWm(void);
int		GetRemoteCmd(char *buff);
bool	ExecRemCmd(char *buff);
int		SelectProcess(char *buff, bool save_buffer);
void	RetRemoteCmd(char *buff);
int		FindProc(char *buff);
int		_TransmitBuff(UChar * buff, int lenbuff);
int		_ReceiveBuff(UChar * buff);
void 	_WiCreateProces(void);
void	use_sgr(bool flag);
void 	_WiDeleteProces(void);
void 	_WiProcess(void);
void 	_WiAplHelp(void);
void 	_WiSetAplHelp(void);
void 	_vs_fill(attr_type cell, short row, short col, short nlines, short ncols);
void 	_vs_update(short from, short to);
char 	*_WmErrmsg(const String &msg);
void 	w_calculator(int row, int column, int dec, UChar *dato, short length);
void 	calculador(void);
DATE	w_calendar(DATE idate, char *user, bool remind);
void 	_WiGetc(void);
void 	GoToShell(void);
void 	_w_service(void);
char 	*hlptxt(char *t, int n);
void 	_w_interrupt(void);
WINDOW 	_GetWPtr(window wi);
window 	_GetNewWin(void);
int 	_SetCurrWin(window p);
void 	_SetWTab(window sub, WINDOW wi);
void 	_FreeVec(WINDOW wi);
bool 	_Ante(window wi1, window wi2);
void 	_WiCol(void);
void 	_WiWidth(void);
void 	_WiHeight(void);
void 	_WiInChar(void);
void 	_WiInAttr(void);
void 	_WiGetTab(void);
void 	_WiSetTab(void);
void 	_WiParent(void);
void 	_WiCurrent(void);
int 	_save_vs_status(WINDOW w);
void 	_WiGets(void);
void 	w_init(void);
char const *_ApplModule(void);
char const *_ApplVersion(void);
void _SetApplModule(char const *);
void _SetApplVersion(char const *);
#if !MSDOS
int 	w_chkperm(void);
#endif
void 	w_end(void);
void 	initialize(void);
void 	_send_signal(int sig);
void 	_WiInterrupts(void);
void 	servmov(void);
void 	wmmov(window wi);
void 	wimov(window wi, int fil, int col);
window 	_FindParCom(window wi1, window wi2);
void 	refresh_all(WINDOW wi);
void 	_WiKeyHelp(void);
void 	_WiFlush(void);
void 	_print_scr(void);
void 	_print_ichset(void);
void 	WmStdAlone(UShort nf);
void 	WmSetStdAlone(void);
void 	_WiCreate(void);
void 	_WiDelete(void);
void 	_WiDeleteAll(void);
void 	_WiSwitchTo(void);
void 	_WiRefresh(void);
void 	_WiRedraw(void);
void 	_WiSetBorder(void);
void 	_WiStatusLine(void);
void 	_WiGetAttr(void);
void 	_WiOrgCol(void);
void 	_WiOrgRow(void);
void 	_WiLine(void);
void 	_WiSettty(void);
void 	_WiResettty(void);
void 	_WiGetField(void);
void 	_tcap_cursor(int n);
void 	_tcap_flush(void);
void 	_tcap_putc(UChar x);
void 	_tcap_open(void);
void 	_tcap_init(void);
void 	_tcap_moveto(int row, int col);
void 	_tcap_setscr(int from, int to);
void 	_tcap_scrup(void);
void 	_tcap_insline(void);
void 	_tcap_delline(void);
void 	_tcap_eeol(void);
void 	_tcap_cls(void);
void 	_tcap_setattr(attr_type n);
void 	_tcap_resetattr(void);
void 	_vs_start(void);
void 	_vs_end(void);
void 	_vs_putc(UChar ch);
void 	_vs_fill(attr_type cell, short row, short col, short nlines, short ncols);
void 	_vs_chgbackgr(attr_type a);
void 	ps_putc(VS_CELL c);
void 	_vs_update(short from, short to);
int 	_save_vs_status(WINDOW w);
void 	_rec_vs_status(WINDOW w);
void 	_vs_do_border(WINDOW w, UChar *label);
void 	_vs_insline(int f_org, int c_org, int nfils, int ncols);
void 	_vs_delline(int f_org, int c_org, int nfils, int ncols);
void 	_vs_inschar(int f_org, int c_org, int nchars);
void 	_vs_delchar(int f_org, int c_org, int nchars);
void 	_vs_beep(void);
void 	_st_line_setup(bool f);
UChar 	_vs_getc(void);
void 	_RecTree(window parcom, window wi);
void 	_Reset(void);
UChar   AltKey(UChar c);
void    WmStdAlone(UShort nf);
void	_WiSetRaw(void);
int		_KeyReady(void);
void	_WiKeyReady(void);

#if MSDOS

void _PcInitVideo(void);
void _PcMoveTo(int row, int col);
void _PcCls(void)	;
void _PcPuts(int line, int col, int n);
void _DosBackWinSetup(void);
void do_help(void);

#else

void	_WiSetServices(void);
void	_WiMove(void);
void	_WiResize(void);
void	_WiService(void);
void	setborder(int f, int c, int lf, int lc, int rest, int tipo);
bool 	_Move(WINDOW wi);

#endif

/* -- Prototypes -- */


/* ++ IXSCA ++ */

void _GetSessionWM(void);
void _SetSessionWM(void);

/* -- IXSCA -- */
