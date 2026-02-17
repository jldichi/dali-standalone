/********************************************************************
* Copyright (c) 1987 InterSoft Co.  All Rights Reserved
* THIS IS UNPUBLISHED PROPRIETARY SOURCE CODE OF InterSoft Co.
* The copyright notice above does not evidence any
* actual or intended publication of such source code.
*
*
*
* $Id: edef.h,v 1.2 1996/08/07 16:14:15 gab Exp $
*
*
*
* DESCRIPTION:
*	Global variable definitions for Ideafix Editor
*
* Files that must be included before this:
* ----------------------------------------
*
*
*
*********************************************************************/

#pragma pack(4)

#ifdef __cplusplus
extern "C" {
#endif
#define private static

struct WINDOW;
struct BUFFER;
struct LINE;
struct REGION;
typedef struct WINDOW * PWINDOW;
typedef struct BUFFER * PBUFFER;
typedef struct LINE * PLINE;
typedef struct REGION * PREGION;

#ifdef	maindef

/* for MAIN.C */

/* initialized global definitions */

int     fillcol = 72;         /* Current fill column          */
int 	init_mode;				/* line command mode		*/
short   kbdm[NKBDM] = {CTLX|')'};   /* Macro                        */
UChar    pat[NPAT];                  /* Search pattern		*/
UChar	rpat[NPAT];			/* replacement pattern		*/
char	sarg[NSTRING] = "";		/* string argument for line exec*/
int	eolexist = TRUE;		/* does clear to EOL exist	*/
int	revexist = FALSE;		/* does reverse video exist?	*/

/*	mode flags	for a buffer */
char	*modename[] = {			/* name of modes		*/
	"WRAP", "CMODE", "SPELL", "EXACT", "VIEW", "OVER",
	"GS", "GD","GDS", "TROFF","FORM","REPORT","DGEN", "SHELL",
};
int     NUMMODES = (sizeof(modename)/sizeof(modename[0]));
char	modecode[] = "WCSEVOGGGTFRDS";	/* letters to rep. modes	*/

int	gmode = 0;			/* global editor mode		*/
int     sgarbf  = TRUE;             /*TRUE if screen is garbage	*/
int     mpresf  = FALSE;            /* TRUE if message in last line */
int	clexec	= FALSE;		/* command line execution flag	*/

/* uninitialized global definitions */

int     currow;                 /* Cursor row                   */
int     curcol;                 /* Cursor column                */
int     thisflag;               /* Flags, this command          */
int     lastflag;               /* Flags, last command          */
int     curgoal;                /* Goal for C-P, C-N            */
PWINDOW curwp;                 /* Current window               */
PBUFFER curbp;                 /* Current buffer               */
PWINDOW wheadp;                /* Head of list of windows      */
PBUFFER bheadp;                /* Head of list of buffers      */
PBUFFER blistp;                /* Buffer for C-X C-B           */
short   *kbdmip;                /* Input pointer for above      */
short   *kbdmop;                /* Output pointer for above     */
char    **arg_list; 			/* File list					*/
int     nargs;
int		act_arg;
short	TAB;					/*Tab size						*/

/* Same as a REGION but expressed in lines & columns				*/
struct EXTREGION {
	PLINE  startline, endline;
	int    startcol, endcol;
};

#else
/* for all the other .C files */

/* initialized global external declarations */

extern  int     NUMMODES; 	/* # of defined modes		*/
extern  int     fillcol;                /* Fill column             */
extern	int 	init_mode;				/* line command mode		*/
extern  short   kbdm[];                 /* Holds kayboard macro data */
extern  UChar    pat[];                  /* Search pattern         */
extern	UChar	rpat[];			/* Replacement pattern		*/
extern	char	sarg[];			/* string argument for line exec*/
extern	int	eolexist;		/* does clear to EOL exist?	*/
extern	int	revexist;		/* does reverse video exist?	*/
extern	char *modename[];		/* text names of modes		*/
extern	char	modecode[];		/* letters to represent modes	*/
extern	int	gmode;			/* global editor mode		*/
extern  int     sgarbf;     /* State of screen unknown      */
extern  int     mpresf;     /* Stuff in message line        */
extern	int	clexec;			/* command line execution flag	*/

/* initialized global external declarations */

extern  int     currow;     /* Cursor row                   */
extern  int     curcol;     /* Cursor column                */
extern  int     thisflag;   /* Flags, this command          */
extern  int     lastflag;   /* Flags, last command          */
extern  int     curgoal;    /* Goal for C-P, C-N            */
extern  PWINDOW curwp;     /* Current window               */
extern  PBUFFER curbp;     /* Current buffer               */
extern  PWINDOW wheadp;    /* Head of list of windows      */
extern  PBUFFER bheadp;    /* Head of list of buffers      */
extern  PBUFFER blistp;    /* Buffer for C-X C-B           */
extern  short   *kbdmip;    /* Input pointer for above      */
extern  short   *kbdmop;    /* Output pointer for above     */

extern  char    **arg_list; /* File list					*/
extern  int     nargs;
extern  int		act_arg;

extern  short	TAB;		/*	Tab size					*/

/* Same as a REGION but expressed in lines & columns				*/
struct EXTREGION {
	PLINE  startline, endline;
	int    startcol, endcol;
};

#endif

/*	Definiciones de los mensajes	*/
extern char *msg[];
#define ER		0
#define EM		1

#define	NO_NEG				1
#define LINE_TOO_SMALL		2
#define MARK_SET			3
#define	NO_MARK				4
#define	DESC_KEY			5
#define	NOT_BOUND			6
#define	KEY_BINDINGS		7
#define	NOT_YET				8
#define	EXEC_BUFF			9
#define	FILE_TO_EXEC		10
#define	USE_BUFFER			11
#define	KILL_BUFFER			12
#define	BUFFER_USED			13
#define CHG_NAME_BUFF		14
#define	SAVE_FILE			15
#define	NO_PREV_FILE		16
#define	NO_NEXT_FILE		17
#define	FILES				18
#define	EDIT_HIST			19
#define	SET_HIST			20
#define SAVING				21
#define	LEAVE_ANYWAY		22
#define	START_MACRO			23
#define	END_MACRO			24
#define	ABORTED				25
#define	ILLEGAL_IN_VIEW		26
#define	READ_FILE			27
#define	INSERT_FILE			28
#define	FIND_FILE			29
#define	VIEW_FILE			30
#define	OLD_BUFFER			31
#define	BUFFER_NAME			32
#define	CANNOT_CR_BUFF		33
#define	READING				34
#define READ_ONE			35
#define	READ_NTH			36
#define	WRAP_READ			37
#define	CANNOT_READ			38
#define	READ_ERROR			39
#define	NEW_FILE			40
#define	WRITE_FILE			41
#define	NO_FNAME			42
#define	WRITING				43
#define	WROTE_ONE			44
#define	WROTE_NTH			45
#define	NAME				46
#define	NO_FILE				47
#define	INSERTING			48
#define	INS_ONE				49
#define	INS_NTH				50
#define	WRAP_INS			51
#define	CANNOT_WRITE		52
#define	CLOSE_ERROR			53
#define	WRITE_ERROR			54
#define	LONG_LINE			55
#define	FUNNY_LINE			56
#define	SET_HIST_ERROR		57
#define	SET_HIST_ERROR_NTH	58
#define	HIST_IS				59
#define	HIST_NOT_FOUND		60
#define	EDIT_MODE_ERR		61
#define	LINE_ALLOC_ERR		62
#define	FILL_COLUMN			63
#define NO_FILL_COLUMN		64
#define	SHOW_POS			65
#define	TAB_SIZE			66
#define	GLOBAL_MODE_TO		67
#define	MODE_TO				68
#define	ADD					69
#define	DELETE				70
#define	NO_MODE				71
#define	SEARCH				72
#define	NOT_FOUND			73
#define	NO_PATTERN			74
#define	R_SEARCH			75
#define	REPLACE				76
#define	Q_REPLACE			77
#define	WITH				78
#define	CANT_SUBSTIT		79
#define	CANNOT_SPLIT		80
#define	CANNOT_MAKE_WINDOW	81
#define	WHICH_NAME			82
#define	HIT_KEY				83
#define IE_USAGE			84
#define _FILE				85
#define INSERT				86
#define COLUMN				87
#define IQLNAME				88
#define CMD					89
#define SHELL				90
#define ED_YES				91
#define ED_NO 				92
#define WORKING				93
#define WIND0				94
#define WIND1				95
#define WIND2				96
#define WIND3				97
#define WIND4				98
#define QUERY_HELP			99
#define BUFF_OVER			100

/*		Definiciones para emconf.c	*/

#define DES_TAB					1
#define DES_BACKCHAR			2
#define DES_FORWCHAR			3
#define DES_INSSPACE			4
#define DES_FORWDEL				5
#define DES_BACKDEL				6
#define DES_NEWLINE				7
#define DES_FORWLINE			8
#define DES_BACKLINE			9
#define DES_FORWPAGE			10
#define DES_BACKPAGE			11
#define DES_FIX_PGLEFT			12
#define DES_FIX_PGRIGHT			13
#define DES_CLOSE_WINDOW		14
#define DES_HELP				15
#define DES_GOTOBOL				16
#define DES_GOTOEOL				17
#define DES_GOTOEOB				18
#define DES_GOTOBOB				19
#define DES_OPENLINE			20
#define DES_EMAC_QUIT			21
#define DES_SET_TABSIZE			22
#define DES_SELECT_ARG			23
#define DES_BACKWORD			24
#define DES_FORWWORD			25
#define DES_SWAP_OVER			26
#define DES_RAED_PREV			27
#define DES_REPOSITION			28
#define DES_ABS_MARGIN			29
#define DES_SETMARK				30
#define DES_PUT_ATRIBUTE		31
#define DES_BACKSEARCH			32
#define DES_USR_WINDOW			33
#define DES_KILLTEXT			34
#define DES_FORWSEARCH			35
#define DES_GOTOLINE			36
#define DES_INDENT				37
#define DES_DESKEY				38
#define DES_SE_LIST				39
#define DES_READ_NEXT			40
#define DES_COPYREGION			41
#define DES_QUOTE				42
#define DES_SREPLACE			43
#define DES_SPLITWINDOW			44
#define DES_KILLREGION			45
#define DES_YANK				46
#define DES_LOWERWORD			47
#define DES_UPPERWORD			48
#define DES_SPAWN				49
#define DES_GOTOCOL				50
#define DES_SHOWCOLUMN			51
#define DES_DELBWORD			52
#define DES_FILEREAD			53
#define DES_DELMODE				54
#define DES_DELFWORD			55
#define DES_CTRLG				56
#define DES_NEXTWINDOW			57
#define DES_DELBLANK			58
#define DES_PREVWIND			59
#define DES_INSFILE				60
#define DES_FILESAVE			61
#define DES_TWIDDLE				62
#define DES_FILEWRITE			63
#define DES_CAPWORD				64
#define DES_SET_GRAPH			65
#define DES_LOWERREGION			66
#define DES_SWAPMARK			67
#define DES_SHOWCPOS			68
#define DES_QREPLACE			69
#define DES_SETMODE				70
#define DES_UPPERREGION			71
#define DES_SETTROFF			72
#define DES_PROCESS				73
#define DES_KILLINE				74
#define DES_ACCENTUATE			75
#define DES_SETFILLCOL			76
#define DES_DOFRAME				77
#define DES_PADREGION			78
#define DES_FILLPARA			79
#define DES_MAKE				80
#define DES_EDHIS				81
#define DES_SETHIS				82
#define DES_PREVCMD				83
#define DES_NEXTCMD				84
#define DES_FINDP				85
#define DES_FINDN				86



/* ++ Prototypes ++ */


void adj_margin(PWINDOW wp);
int gotocol(int f,int n);
int gotobol(int f,int n);
int backchar(int f,register int n);
int gotoeol(int f,int n);
int forwchar(int f,register int n);
int gotoline(int f,int n);
int gotobob(int f,int n);
int gotoeob(int f,int n);
int gotoeop(int f, int n);
int forwline(int f,int n);
int backline(int f,int n);
int gotobop(int f,int n);
int getgoal(register PLINE dlp);
int forwpage(int f,register int n);
int backpage(int f,register int n);
int setmark(int f,int n);
int swapmark(int f,int n);
int abs_margin(int f,int n);
int _abs_margin(int n);
int fix_pgleft(int f,int n);
int fix_pgright(int f,int n);
void deskey(int f,int n);
int getckey(void);
void describe_key(int c,char * out);
void help(int f,int n);
PBUFFER get_curbuf(void);
void save_curbuf(void);
void restore_bp(void);
char * get_bname(PBUFFER bp);
int usebuffer(int f, int n);
int nextbuffer(int f, int n);
int swbuffer(PBUFFER bp);
int killbuffer(int f, int n);
bool zotbuf(register PBUFFER bp);
int namebuffer(int f,int n);
int add_tline(char *text, PBUFFER bp);
int addline(char *text, int ntext, PBUFFER bp);
int anycb(void);
PBUFFER bfind(register char *bname, int cflag, int bflag, int search);
int bclear(register PBUFFER bp);
PBUFFER ignore_flg_chng(PBUFFER bp);
int read_prev(int f,int n);
int read_next(int f,int n);
int select_arg(int f,int n);
void set_scanbuf(PBUFFER bp);
int edgetc(void);
void edungetc(int c);
void update(void);
private void print_line(PLINE lp, int offset, int cant);
private void print_lline(PLINE lp, int offset, int cant);
void mlerase(void);
int mlyesno(UChar *prompt);
int mlreply (UChar *prompt, UChar *buf, int nbuf, UChar *s_default);
int mlreplyt(UChar *prompt, UChar *buf, int nbuf, char eolchar, UChar *s_default);
void mlwrite(char *fmt, ...);
void mlputs(char *s);
void mlputi(int i, int r);
void mlputli(long l, int r);
int PadRegion(int f,int n);
int DoFrame(int f,int n);
int FileGetc(FILE *fp);
void FilePutc(UChar c, FILE *fp);
char *_Msg(int module, int msg);
void run_editor(void);
int execute(int c, int f, int n);
int getkey(int *n);
void quickexit(int f, int n);
int emac_quit(int f, int n);
int ctlxlp(int f, int n);
int ctlxrp(int f, int n);
int ctlxe(int f, int n);
int ctrlg(int f, int n);
int rdonly(void);
int isprt(int c);
int fileread(int f, int n);
int insfile(int f, int n);
int filefind(int f, int n);
int viewfile(int f, int n);
int getfile(char *fname, int lockfl);
int readin(char *fname, int lockfl);
void makename(char *bname, char *fname);
int filewrite(int f, int n);
int filesave(int f, int n);
int writeout(char *fn);
int filename(int f, int n);
int ifile(char *fname);
int ffropen(char *fn);
int ffwopen(char *fn);
int ffclose(void);
int ffputline(char *buf, int nbuf);
int ffgetline(register char *buf, int nbuf);
void set_history(int f,int n);
int prev_instr(int f,int n);
int next_instr(int f,int n);
void history(int f,int n);
bool new_buffer(void);
void _clearerror(FP func_error);
void ed_start(int mode, int b_mode, int argc, char **argv, char *caller, char *version, bool ident, int errwindow);
PLINE lalloc(register int used);
void lfree(register PLINE lp);
void lchange(register int flag, PBUFFER bp);
void insspace(int f,int  n);
int linsert(int n, int c);
void add_line(char *s);
int lnewline(void);
int ldelete(int n, int kflag);
int ldelnewline(void);
void kdelete(void);
int kinsert(int c);
int kremove(int n);
int lockchk(char *fname);
int lockrel(void);
int ed_file_lock(char *fname);
int unlock(char *fname);
void lckerror(char *errstr);
void SetWorkingMsg(int flag);
void modeline(PWINDOW wp);
void upmode(void);
int make(int f, int n);
int process(void);
int process(void);
int get_emode(PBUFFER bp);
void set_emode(PBUFFER bp, int mode);
int setfillcol(int f, int n);
int se_list(int f,int n);
int showcolumn(int f,int n);
int showcpos(int f, int n);
int accentuate(int f,int n);
int getccol(int bflg,PWINDOW wp);
int getcol(int bflg,PLINE dotp,int doto);
int getsubind(PLINE lp, int disppos, int listmode);
int twiddle(int f, int n);
void reset_tab(void);
int set_tabsize(int f,int n);
int tab(int f, int n);
int openline(int f, int n);
int newline(int f, int n);
int cinsert(void);
int insbrace(int n, int c);
int inspound(void);
int deblank(int f, int n);
int indent(int f, int n);
int forwdel(int f,int  n);
int backdel(int f, int n);
int killtext(int f, int n);
int killine(int f, int n);
int yank(int f, int n);
void setmode(int f, int n);
void delmode(int f, int n);
void setgmode(int f, int n);
void delgmode(int f, int n);
void swap_over(int f,int n);
int graphic(int c);
int put_atribute(int f,int n);
int set_troff(int f,int n);
int set_graph(int f,int n);
int killregion(int f, int n);
int copyregion(int f, int n);
int lowerregion(int f, int n);
int upperregion(int f, int n);
bool ChkOver(short *len, short sum);
#ifdef IDEAWIN
int getregion(register PREGION rp);
#else
int getregion(register REGION *rp);
#endif
int forwsearch(int f, int n);
int forwhunt(int f, int n);
int backsearch(int f, int n);
int backhunt(int f, int n);
int binsearch(int f, int n);
int findprev(int f, int n);
int _findnext(int f, int n);
int eq(int bc, int pc);
int readpattern(char *prompt);
int sreplace(int f, int n);
int qreplace(int f, int n);
int replaces(int kind, int f, int n);
int forscan(UChar *patrn, int leavep);
int expandp(char *srcstr, char *deststr, int maxlength);
int spawncli(int f, int n);
int spawn(int f, int n);
PWINDOW get_hdwind(void);
PWINDOW get_curwind(void);
void save_curwind(void);
void restore_wp(void);
int reposition(int f, int n);
int nextwind(int f, int n);
int prevwind(int f, int n);
void active_wind(void);
int splitwind(int f, int n);
int usr_window(int f,int n);
int link_wnd(int orow,int ocol, int nrow, int ncol, PBUFFER bp);
int cr_filwindow(int orow,int ocol, int nrow, int ncol, char *file);
int _create_windw(int orow, int ocol, int nrow, int ncol, char * name, int t_name);
PWINDOW findwind(PBUFFER bp);
PWINDOW *ind_prev_wndw(PWINDOW wp);
int close_window(int f,int n);
int backword(int f, int n);
int forwword(int f, int n);
int upperword(int f, int n);
int lowerword(int f, int n);
int capword(int f, int n);
int delfword(int f, int n);
int delbword(int f, int n);
int inword(void);
int fillpara(int f, int n);
int killpara(int f, int n);
void cmdstr(int c, char *seq);
void ResetTroffFonts(void);
int currentLine(void);
int grep(char *pattern);
int obtener(int *orow, int *ocol, int *nrow, int *ncol, char *file);
EHAND FindEditorFunction(int c);
char *FindFunctionDes(int c);

/* -- Prototypes -- */

#ifdef __cplusplus
  }
#endif

#pragma pack()

