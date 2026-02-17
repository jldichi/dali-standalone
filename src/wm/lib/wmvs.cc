/********************************************************************
* Copyright (c) 1987 InterSoft Co.  All Rights Reserved
* THIS IS UNPUBLISHED PROPRIETARY SOURCE CODE OF InterSoft Co.
* The copyright notice above does not evidence any
* actual or intended publication of such source code.
*
*
* $Id: wmvs.cc,v 1.11 2008/07/07 19:59:03 hardaiz Exp $
*
*
* DESCRIPTION:
*      Funciones de manejo interno de VS.
*
* ROUTINE       |  PURPOSE
*---------------+---------------------------------------------------
*_vs_start		 start.
*_vs_end		 end.
*_vs_putc		 Write a character to the virtual screen.
*_vs_fill		 Fill a virtual screen area.
*ps_moveto		 Move the hardware cursor to specificate position.
*ps_putc		 Output a cell to the terminal.
*_vs_update		 Make sure that the display is right.
*_save_vs_status Save the virtual screen status.
*_rec_vs_status	 Restore The virtual screen status.
*_vs_do_border	 Draw window border.
*_vs_insline		 Insert a line in virtual screen.
*_vs_delline		 Delete a line in virtual screen.
*_vs_inschar		 Insert a character in virtual screen.
*_vs_delchar		 Delete a character in virtual screen.
*_ps_insline		 Insert a line in phisic screen.
*_ps_delline		 Delete a line in phisic screen.
*_vs_beep		 
*_vs_display_st_line Display st_line.
*call_vs_display_st_line	Calls to _vs_display_status_line
*_vs_set_alarm start.
*vs_alarm		 Called by alarm interrup.
*_st_line_setup	 Define how st_line works.
*ps_getc		 Get a character from terminal.
*translate		 
*_vs_getc		 Get a character and translate it.
*_vs_chgbackgr  Cambia el backgr de la window activa.
*********************************************************************/

#include <ideafix.h>
#include <cfix.h>

#include <ifound/str.h>

#include <ifound/priv/gfuncs.h>

#include <sys/types.h>
#include <sys/stat.h>
#include <errno.h>
#include <unistd.h>
#include <stdlib.h>
#ifdef HAVE_IPC
#include <sys/ipc.h>
#include <sys/sem.h>
#endif

#define SYS_TYPES

#include  <signal.h>
#include <ideafix/priv/gndefs.h>
#define WMVS
#include <wm.h>

/* ++ Prototypes ++ */

static void ps_moveto(short row, short col);
static void setsem(int semid);
static void unsetsem(int semid);
static void vs_label(char *p, int len);
static bool _ps_insline(int f, int n);
static bool _ps_delline(int f, int n);
static void _vs_display_st_line(void);
void call_vs_display_st_line(void);
static void vs_set_alarm(bool);
static void vs_alarm(void);
static UChar ps_getc(void);
static UChar translate(TRAD_ELEM *p, UChar c);
#ifdef HAVE_MAIL
static bool CheckMail(void);
#endif

/* -- Prototypes -- */

int tmout = 0;
static long tm_wait_getc;

#ifdef HAVE_TTY
int semid = ERR;
#else
#define ps_getc() KeyGetc()
#endif

extern bool retry;				// declarada en wminit.c
extern bool have_slave_printer;	// declarada en wm.cc

int wmstop = FALSE;

int WmStop()
{
	_send_signal(SIGTERM);
    wmstop = TRUE;
	tmout = 1;
	return 0;
}

void _vs_start(void)
{
#ifdef HAVE_TTY
    register int i;
    _tcap_open();

    vscreen = (VS_CELL **) Alloc(VS_LINES * sizeof(VS_CELL *));
    pscreen = (VS_CELL **) Alloc(VS_LINES * sizeof(VS_CELL *));

    for (i = 0; i < VS_LINES; ++i) {
        vscreen[i] = (VS_CELL *) Alloc(VS_COLS * sizeof(VS_CELL));
        pscreen[i] = (VS_CELL *) Alloc(VS_COLS * sizeof(VS_CELL));
	}
#else
    _PcInitVideo();
#endif
    first	= (short *) Alloc(VS_LINES * sizeof(short));
    last	= (short *) Alloc(VS_LINES * sizeof(short));
}

#ifdef HAVE_TTY

void _vs_end(void)
{
	register int i;

    for (i = 0; i < VS_LINES; ++i) {
        Free(vscreen[i]);
        Free(pscreen[i]);
	}
	Free(vscreen);
	Free(pscreen);
}

#else

void _vs_end(void) { vscreen_free(); }

#endif

/* Write a character to the virtual screen */

void _vs_putc(UChar ch)
{
	if (first[vsrow] > vscol) first[vsrow] = vscol;
	if (last [vsrow] < vscol) last [vsrow] = vscol;
	vscreen_set(vsrow, vscol++, ch | vsattr);
}

/*
 * Fill a  virtual screen area with a char with atribute.
 * Parameters	: 	row		: Initial Row.
 * 					col		: Initial Column.
 *              	nlines	: Number of lines to erase.
 *              	ncols	: Number of Columns.
 */

void _vs_fill(	attr_type cell, short row, short col, short nlines,
				short ncols)
{
	register short j, lastcol = col+ncols - 1;
	while (nlines--) {
		if (first[row] > col)		first[row] = col;
		if (last [row] < lastcol)	last [row] = lastcol;
		for (j = col; j <= lastcol; j++)
			vscreen_set(row, j, (VS_CELL) cell);
		row++;
	}
}

void _vs_chgbackgr(attr_type a)
{
	int i, j;

	/* si es el mismo entonces me voy */
	if (a==wcurr->backgr) return;

	for (i=wcurr->f_org;i<wcurr->f_org+wcurr->nfils;i++) {
		for (j=wcurr->c_org;j<wcurr->c_org+wcurr->ncols;j++) {
			VS_CELL c = vscreen_get(i, j);
			if (VS_BACKGR(c) == wcurr->backgr)
				vscreen_set(i, j, (c & ~wcurr->backgr) | a);
		}
		if (first[i] > wcurr->c_org) first[i] = wcurr->c_org;
		if (last[i] < (short)j-1) last[i] = (short)(j-1);
	}
}
	
/*
*Send a command to the terminal to move the hardware cursor to row "row"
*and column "col". The row and column arguments are origin 0.
* Optimize out random calls. Update "psrow" and "pscol".
*/

#ifdef HAVE_TTY

static void ps_moveto(short row, short col)
{
    if (row!=psrow || col!=pscol) {
        psrow = row;
        pscol = col;
		_tcap_moveto(row, col);
	}
}

/*
 * Output a cell to the terminal
 */
void ps_putc(VS_CELL c)
{
	c = out_tab[VS_CHAR(c)] | VS_ATTR(c);
	if (VS_ATTR(c) != psattr) {
		if (psattr) _tcap_resetattr();
		psattr = VS_ATTR(c);
		if (psattr) _tcap_setattr(psattr);
	}
	(void) _tcap_putc(VS_CHAR(c));
	++pscol;
}

void _vs_update(short from, short to)
{
    register int i;
    register VS_CELL *vp, *pp, *vplast, *pplast, *vpblank, *vpaux;
	bool bflag;

	if (have_slave_printer)
		setsem(semid);	

	/* Special hacking if the screen is garbage.
	 * Clear the hardware screen, and update your copy to agree with it.
	 * Set all the virtual screen flags to force a full update.
	 */
	if (scr_garbage) {
        for (i = 0; i < VS_LINES; ++i) {
            first[i] = 0;
			last[i]  = VS_COLS-1;
            pp = pscreen[i];
			vpaux = &pscreen[i][VS_COLS];
			while (pp < vpaux) *pp++ = ' ';
		}
		_tcap_init();
		ps_moveto(0, 0);
		_tcap_cls();
		scr_garbage = FALSE;
	}

    /*
	 * Make sure that the physical and virtual displays agree. 
     */
    for (i = from; i <= to; ++i) {
		if (last[i] < 0) continue;
		/*
		 * Update a single line.
		 * It does try an exploit erase to end of line.
		 */
        vp = &vscreen[i][first[i]];
		pp = &pscreen[i][first[i]];

        vplast = &vscreen[i][last[i]];
		pplast = &pscreen[i][last[i]];
		if (i == VS_LINES-1 && last[i] == VS_COLS-1) {
			vplast--;
			pplast--;
		}
		/*
		 * Mark first and last markers
		 */
		 first[i] = HUGE;
		 last[i]  = -1;

		/* advance past any common chars at the left */

		while (vp <= vplast && vp[0] == pp[0]) {
			++vp;
			++pp;
		}

		/* if both lines are the same, no update needs to be done */

		if (vp > vplast) continue;

		/* find out if there is a match on the right */

		bflag = TRUE;
		while (vplast[0] == pplast[0]) {
			if (vplast[0] != ' ')		/* Note if any nonblank */
				bflag = FALSE;			/* in right match. */
			--vplast;
			--pplast;
		}
		vpblank = vplast;

		if (bflag && eol_length) {	/* Erase to EOL ? */
			while (vpblank >= vp && vpblank[0] == ' ')
				--vpblank;

			if (vplast - vpblank <= eol_length)
				/* Use only if erase is fewer characters. */
				vpblank = vplast;
			else {
				/* Check if blanks on the rigth */
				vpaux = &vscreen[i][VS_COLS-1];
				while (vpaux > vplast)
					if (*vpaux-- != ' ') {
						vpblank = vplast;
						break;
					}
			}
		}
		/* Go to start of line. */
		ps_moveto((short)i, (short)(vp-vscreen[i]));

		while (vp <= vpblank) {
			if (*pp != *vp) {
				ps_putc(*vp);
				*pp++ = *vp++;
			}
			else {
				vpaux = vp;
				do {
					pp++;vp++;
				} while (vp <= vpblank && *vp == *pp);

				if (vp-vpaux >= 10)
					ps_moveto(psrow, (short)((vp-vpaux)+pscol));
				else
					while (vpaux < vp)
						ps_putc(*vpaux++);
			}
		}

		if (vpblank != vplast) {		/* Erase. */
			if (psattr) {
				_tcap_resetattr();
				psattr = 0;
			}
			_tcap_eeol();
			while (vp <= vplast)
				*pp++ = *vp++;
		}
	}

         /* Finally,
	 * update the hardware cursor and attribute and flush out buffers.
	 */
        ps_moveto(vsrow, vscol);
	if (vsattr != psattr) {
		if (psattr) _tcap_resetattr();
		psattr = vsattr;
		if (psattr) _tcap_setattr(psattr);
	}

// Este codigo debe ser agregado si se utiliza AIX 5.x con SSH (hasta fin del 'else')
/* 	else {
                //if (psattr) {  	// Esta linea debe quedar comentada.
                        _tcap_resetattr();
                //}  			// Dejar comentada.
                if (psattr) {
                        _tcap_setattr(psattr);
                }
        }
*/

	(void) _tcap_flush();
	if (have_slave_printer)
		unsetsem(semid);	
}

static void setsem(int sid) 
{
	struct sembuf x[2];

	x[0].sem_num = 1;
	x[0].sem_op =  0;
	x[0].sem_flg = 0;

	x[1].sem_num = 1;
	x[1].sem_op =  1;
	x[1].sem_flg = 0;

	(void) semop(sid, x, 2);
#ifdef HAVE_RESTARTABLE_SYSCALLS
	errno=0;
    int retvalue=0;
    retvalue=semop(semid, x, 2);
	assert(retvalue == 0);
#else 
	errno = 0;
	while (semop(semid, x, 2) == ERR)
		assert(errno == EINTR);
#endif
}

static void unsetsem(int sid) 
{
	struct sembuf x[1];

	x[0].sem_num = 1;
	x[0].sem_op =  -1;
	x[0].sem_flg = 0;

	//(void) semop(sid, x, 1);
#ifdef HAVE_RESTARTABLE_SYSCALLS
   	errno=0;
	int retvalue =0;
	retvalue=semop(semid, x, 1);
	assert(retvalue == 0);
#else 
	errno = 0;
	while (semop(semid, x, 1) == ERR)
		assert(errno == EINTR);
#endif
}

#else /* HAVE_TTY */

/*
 * Make sure that the display is right.
 */
void _vs_update(short from, short to)
{
    register int i;

	/* Special hacking if the screen is garbage.
	 * Clear the hardware screen, and update your copy to agree with it.
	 * Set all the virtual screen flags to force a full update.
	 */
	if (scr_garbage) {
        for (i = 0; i < VS_LINES; ++i) {
            first[i] = 0;
			last[i]  = VS_COLS-1;
		}
		_PcCls();
		scr_garbage = FALSE;
	}

    /*
	 * Make sure that the physical and virtual displays agree. 
     */
    for (i = from; i <= to; ++i) {
		if (last[i] >= 0) {
			_PcPuts(i, first[i], last[i]-first[i]);
			first[i] = HUGE;
			last[i]  = -1;
		}
	}
    /* Finally, update the hardware cursor */
	_PcMoveTo(vsrow, vscol);
}

#endif

/*
 * _save_vs_status: Salva una parte de la pantalla virtual, y sus
 *			       parametros asociados, en una zona de memoria.
 */
int _save_vs_status(WINDOW w)
{
	register char *s;
	register int  f, n;

	if (w->save == NULL)
		w->save = vscreen_alloc(  (w->border.f_last-w->border.f_org+1)
								* (w->border.c_last-w->border.c_org+1));
	s = w->save;

	for (f = w->border.f_org; f <= w->border.f_last; ++f) {
		n = (w->border.c_last-w->border.c_org+1);
		s += vscreen_save_to_buf(s, f, w->border.c_org, n);
	}
	return OK;
}

/*
 * _rec_vs_status: Funcion inversa a _save_vs_status
 *
 */
void _rec_vs_status(WINDOW w)
{
	register char *s;
	register int  f;
	short c, lastc;

	if (w->parent == NULL) {
		_vs_fill((attr_type)' ', 0, 0, VS_LINES, VS_COLS);
		return;
	}
	if (w->save == NULL) return;

	c	  = w->border.c_org;
	lastc = w->border.c_last;
	s = w->save;

	for (f = w->border.f_org; f <= w->border.f_last; ++f) {
		int n = lastc - c + 1;
		s += vscreen_rec_from_buf(s, f, c, n);
		if (first[f] > c)		first[f] = c;
		if (last[f]  < lastc)	last[f]	 = lastc;
	}

}

static void vs_label(char *p, int len)
{
	int n;
	attr_type attr;

	while (*p && len) {
		if (isprint((UChar) *p)) {
			_vs_putc(*p);
			len--;
		}
		else if (*p == '\033') {
			for (n=0,attr=0; *p && !isalpha((UChar) *p); p++) {
				if (isdigit(*p)) n = n*10 + *p - '0';
				else if (*p == ';') {
					attr |= attr_tab[n].value;
					n = 0;
				}
			}
			if (*p == 0) break;	
			if (*p == 'm')
				vs_setattr(attr | attr_tab[n].value);
		}
		++p;
	}
}

/*
 * _vs_do_border: Dibuja el recuadro de la window.  
 *
 * caracteres:
 *                     ul h h h h h h h ur
 *                     v                 v
 *                     v                 v
 *                     v                 v
 *                     dl h h h h h h h dr
 */
void _vs_do_border(WINDOW w, UChar *label)
{
	UChar ul,h,ur,v,dl,dr;
	char tipo;
	register int i, border;
	attr_type prev_attr;

	/* Si no hay border, retornamos */

	if( w->border.attr == NO_BORDER ) return;

	/* Decidir el tipo de caracteres que se utilizaran */
	tipo 	= (char) (INT((long) w->border.attr) & 0x0F);
	border	= INT((long) w->border.attr) & 0xF0;

	if (tipo == STAND_TYPE) {
#ifdef HAVE_TTY
		tipo = (char) ( has_line_drawing_chars ? DLINE_TYPE :
						term_rev ? BLANK_TYPE : SLINE_TYPE );
#else
		tipo = (char) DLINE_TYPE;
#endif
	}

	switch ( tipo ) {
	case ASTSK_TYPE :
		ul = h = ur = v = dl = dr = '*';
		break;

	case SLINE_TYPE:
		ul = C_UL;
		h  = C_HORIZ; 
		ur = C_UR;
		v  = C_VERT;
		dl = C_LL;
		dr = C_LR;
		break;

	case DLINE_TYPE:
		ul = C_D_UL;
		h  = C_D_HORIZ; 
		ur = C_D_UR;
		v  = C_D_VERT;
		dl = C_D_LL;
		dr = C_D_LR;
		break;

	case BLANK_TYPE:
	default :
		ul = h = ur = v = dl = dr = ' ';
		break;
	}

	/* Select attribute, and save the previous */
	prev_attr = vs_getattr();
	vs_setattr(VS_ATTR(w->border.attr));

	/* Dibujar el recuadro */
	if(border & TOP_BORDER){
		vs_moveto(w->border.f_org, w->border.c_org);
		_vs_putc(h);
		for(i = w->border.c_org+1; i < w->border.c_last; ++i)
			_vs_putc(h);
		_vs_putc(h);

		/* ahora va el label */
		if (label) {
			vs_moveto(w->border.f_org, w->border.c_org+1);
			vs_label((char*)label, w->ncols);					
			vs_setattr(VS_ATTR(w->border.attr));
		}
	}

	if(border & LOW_BORDER){
		vs_moveto(w->border.f_last, w->border.c_org);
		for (i = w->border.c_org; i <= w->border.c_last; ++i)
			_vs_putc(h);
	}
	if (border & LEFT_BORDER) {
		for (i = w->border.f_org; i <= w->border.f_last; ++i) {
			vs_moveto((short)i, w->border.c_org);
			_vs_putc(v);
		}
	}
	if (border & RIGHT_BORDER){
		for (i = w->border.f_org; i <= w->border.f_last; ++i) {
			vs_moveto((short)i, w->border.c_last);
			_vs_putc(v);
		}
	}
	if (tipo != SLINE_TYPE && tipo != DLINE_TYPE) return;

	/* Dibujar las esquinas */

	if (border & TOP_BORDER && border & LEFT_BORDER) {
		vs_moveto(w->border.f_org, w->border.c_org);
		_vs_putc(ul);
	}
	if (border & TOP_BORDER && border & RIGHT_BORDER) {
		vs_moveto(w->border.f_org, w->border.c_last);
		_vs_putc(ur);
	}

	if (border & LOW_BORDER && border & LEFT_BORDER) {
		vs_moveto(w->border.f_last, w->border.c_org);
		_vs_putc(dl);
	}
	if (border & LOW_BORDER && border & RIGHT_BORDER) {
		vs_moveto(w->border.f_last, w->border.c_last);
		_vs_putc(dr);
	}

	/* Restore previous atribute */
	vs_setattr(prev_attr);
}


/*
 * _vs_insline: insercion de una linea en la pantalla virtual.
 * Parametros:
 *
 *			f_org : fila donde se insertara.
 *          c_org : columna a partir de donde inserta.
 *          ncols : cantidad de columnas de la zona.
 *          nfils : cantidad de filas de la zona.
 *
 *                       V.S.
 *
 *                      |<- c_org
 *          +---------------------------------+
 *          |                                 |
 *          |           <-- ncols --->        |
 *  f_org ->|           +------------+        | ^
 *          |           |            |        | |  
 *          |           |            |        | | nfils
 *          |           |            |        | |
 *          |           +------------+        | v
 *          +---------------------------------+
 *      
 */
void _vs_insline(int f_org, int c_org, int nfils, int ncols)
{
	short first_col = (short)c_org;
	short last_col  = (short)(c_org+ncols-1);
#ifdef HAVE_TTY
	register int f;
	register VS_CELL *pp, *top;

	/*
	* If the width of the window is long 
	* Insert line secuence
	*/
	if (ncols > VS_COLS-4 && _ps_insline(f_org, nfils)) {
		for (f = f_org + nfils - 1; f > f_org; f--)
			mCopy(pscreen[f], pscreen[f-1], VS_COLS*sizeof(VS_CELL));
		pp = pscreen[f_org]; 
		top = pp + VS_COLS;
		while (pp < top) *pp++ = ' ';
		first_col = 0;
		last_col  = VS_COLS-1;
	}
	if (ncols == VS_COLS) {
		f = f_org + nfils - 1;
		top = vscreen[f];
		while (f > f_org) {
			vscreen[f] = vscreen[f-1];
			first[f] = 0;
			last[f]  = VS_COLS-1;
			f--;
		}
		vscreen[f_org] = top;
	}
	else {
		for (f = f_org + nfils - 1; f >= f_org; f--){
			if (f > f_org)
				mCopy(&vscreen[f][c_org], &vscreen[f-1][c_org],
						ncols * sizeof(VS_CELL));
			if (first[f] > first_col) first[f] = first_col;
			if (last[f]  < last_col)  last[f]  = last_col;
		}
	}
	vs_erase((short)f_org ,(short)c_org, 1, (short)ncols);
#else
	register int f, i;

	for (f = f_org + nfils - 1; f >= f_org; f--){
		if (f > f_org) {
			for (i = 0; i < ncols; i++)
				vscreen_set(f, c_org+i, vscreen_get(f-1, c_org+i));
		}			
		if (first[f] > first_col) first[f] = first_col;
		if (last[f]  < last_col)  last[f]  = last_col;
	}
	vs_erase((short)f_org ,(short)c_org, 1, (short)ncols);
#endif
}

/*
 * _vs_delline: borrado de una linea en la pantalla virtual.
 * Parametros:
 *
 *			f_org : fila a borrar.
 *          c_org : columna a partir de donde borrara.
 *          ncols : cantidad de columnas de la zona.
 *          nfils : cantidad de filas de la zona.
 */
void _vs_delline(int f_org, int c_org, int nfils, int ncols)
{
#ifdef HAVE_TTY
	register int f;
	register VS_CELL *pp, *top;
	short first_col = (short)c_org;
	short last_col  = (short)(c_org+ncols-1);

	/*
	* If the width of the window long use 
	* Delete line secuence
	*/
	if (ncols > VS_COLS-4 && _ps_delline(f_org, nfils)) {
		for (f = f_org ; f < (f_org+nfils-1); f++)
			mCopy(pscreen[f], pscreen[f+1], VS_COLS*sizeof(VS_CELL));
		pp = pscreen[f_org+nfils-1]; 
		top = pp + VS_COLS;
		while (pp < top) *pp++ = ' ';
		first_col = 0;
		last_col  = VS_COLS-1;
	}
	if (ncols == VS_COLS) {
		top = vscreen[f_org];
		for (f = f_org ; f < (f_org+nfils-1); f++){
			vscreen[f] = vscreen[f+1];
			first[f] = 0;
			last[f]  = VS_COLS-1;
		}
		vscreen[f] = top;
	}
	else {
		for (f = f_org ; f <= (f_org+nfils-1); f++){
			if (f < f_org+nfils-1)
				mCopy(&vscreen[f][c_org], &vscreen[f+1][c_org],
						ncols*sizeof(VS_CELL));
			if (first[f] > first_col) first[f] = first_col;
			if (last[f]  < last_col)  last[f]  = last_col;
		}
	}
	vs_erase((short)(f_org+nfils-1),(short)c_org, 1, (short)ncols);
#else
	register int f, i;
	short first_col = (short)c_org;
	short last_col  = (short)(c_org+ncols-1);

	for (f = f_org ; f <= (f_org+nfils-1); f++) {
		if (f < f_org+nfils-1) {
			for (i = c_org; i <= last_col; i++) {
				vscreen_set(f, i, vscreen_get(f+1, i));
			}
		}
		if (first[f] > first_col) first[f] = first_col;
		if (last[f]  < last_col)  last[f]  = last_col;
	}
	vs_erase((short)(f_org+nfils-1),(short)c_org, 1, (short)ncols);
#endif
}

/*
 * _vs_inschar: insercion de un caracter en la pantalla virtual.
 * Parametros:
 *
 *			f_org : fila corriente
 *          c_org : columna a partir de donde inserta
 *          nchars: cantidad de caracteres a insertar
 */
void _vs_inschar(int f_org, int c_org, int nchars)
{
	int col = (wcurr->c_org+wcurr->ncols) - 1;
	
	for (; col > c_org + nchars -1 ; col--)
		vscreen_set(f_org, col, vscreen_get(f_org, col-nchars));
		
	/* Lleno con blancos */
	for ( ; col >= c_org ; col--)
		vscreen_set(f_org, col, ' ');
		
	if (first[f_org] > (short)c_org)
		first[f_org] = (short)c_org;
	if (last[f_org] < wcurr->c_org+wcurr->ncols-1)
		last[f_org] = (wcurr->c_org+wcurr->ncols) - 1;
}

/*
 * _vs_delchar: borrado de un caracter en la pantalla virtual.
 * Parametros:
 *
 *			f_org : fila corriente
 *          c_org : columna a partir de donde borra
 *          nchars: cantidad de caracteres a borrar
 */
void _vs_delchar(int f_org, int c_org, int nchars)
{
	int col;
	for (col = c_org ; col < (wcurr->c_org+wcurr->ncols)-nchars; col++)
		vscreen_set(f_org, col, vscreen_get(f_org, col+nchars));

	/* Lleno con blancos */
	for ( ; col < wcurr->c_org+wcurr->ncols ; col++)
		vscreen_set(f_org, col, ' ');
		
	if (first[f_org] > (short)c_org)
		first[f_org] = (short)c_org;
	if (last[f_org] < wcurr->c_org+wcurr->ncols-1) 
		last[f_org] = (wcurr->c_org+wcurr->ncols) - 1;
}

#ifdef HAVE_TTY

static bool _ps_insline(int f, int n)
{
	if (has_setscr)  {
		_tcap_setscr(f, f+n-1);
		psrow = pscol = 0;
		ps_moveto((short)f, 0);
		_tcap_scrup();
		_tcap_setscr(0, VS_LINES-1);
		psrow = pscol = 0;
	}
	else if (has_idlsec) {
		if (f+n != VS_LINES) {
			ps_moveto((short)(f+n-1), 0);
			_tcap_delline();
		}
		ps_moveto((short)f, 0);
		_tcap_insline();
	}
	else return FALSE;
	return TRUE;
}

static bool _ps_delline(int f, int n)
{
	if (f==0 && n == VS_LINES) {
		ps_moveto(VS_LINES-1, 0);
		_tcap_putc('\n');
	}
	else if (has_setscr)  {
		_tcap_setscr(f, f+n-1);
		psrow = (short)f;
		pscol = 0;
		ps_moveto((short)f+n-1, 0);
		_tcap_putc('\n');
		_tcap_setscr(0, VS_LINES-1);
		psrow = pscol = 0;
	}
	else if (has_idlsec) {
		ps_moveto((short)f, 0);
		_tcap_delline();
		if (f+n != VS_LINES) {
			ps_moveto((short)(f+n-1), 0);
			_tcap_insline();
		}
	}
	else return FALSE;
	return TRUE;
}

/*
 * _vs_beep(): Suena el pitito.
 */
void _vs_beep(void)
{
	char c = '\007';
	(void) write(1, &c, 1);
}

#else  /* HAVE_TTY */

void _vs_beep(void)
{
#ifndef __NT__
	int hz;

#ifdef HAVE_SOUND_BEEP
	void sound_beep(int freq);

	for(hz=500; hz<2000; hz+=800) 
		sound_beep(hz);	
#else
	void sound(unsigned freq);
	void nosound(void);
	
	for(hz=500; hz<2000; hz+=800) {
		sound(hz); delay(100); 
	}
	sound(500); delay(100); nosound();
#endif
#else
	ntbeep(2000, 100);
#endif
}

#endif /* HAVE_TTY */

/*
 * Estructura que maneja el comportamiento de la linea de status.
 */
#define STATUS_COL	(80-26)

static struct {
	long	 last;	/* Last time displayed					*/
	unsigned secs;	/* Cada cuantos redesplegar 0 = nodisp.	*/
	int f;			/* fila para display hora				*/
	int c;			/* col. para display hora				*/
	attr_type attr; /* Attr. para display hora				*/
	int fmtdate;	/* Formato del date						*/
	int fmttime;	/* Formato del time						*/
	char buf[80];   /* Buffer para obtener hora				*/
} st_line = {
	-1L, 0, 0, STATUS_COL, A_BOLD, DFMT_STANDARD,
	TFMT_SEPAR, "",
};

/*
 * _st_line_setup: Define el funcionamiento del st_line.
 *
 */
static bool d_f_status = FALSE;

void _st_line_setup(bool f)
{
	if (f) {
		if (st_line.secs) return;
		st_line.secs = 30;
	}
	else {
		st_line.last = -1L;
		st_line.secs = 0;
		d_f_status = FALSE;
		return;
	}

	_vs_display_st_line();
#ifdef HAVE_FULL_SIGNALS
	SigSet(SIGALRM, SIG_IGN);
#endif
}



/*
 * call_vs_display_st_line: Llama a la función _vs_display_st_line
 *          		   		Se hace así porque la llamada está en otro módulo
 *							y la funcón es static. Es llamada por WiRefresh()
 *							para que actualice la hora. Antes la volvía atrás.
 */
void call_vs_display_st_line(void)
{
	_vs_display_st_line();
}



/*
 * _vs_display_st_line: Muestra la hora, con los parametros determinados
 *          		   en st_line_setup
 *
 */
static void _vs_display_st_line(void)
{
	int cf, cc, i;
	char *p;

	if (!st_line.secs) return;

	p = st_line.buf;

#ifdef HAVE_MAIL
	if (!tty_set) return;
	/* Verificar y mostrar el mail */
	sprintf(p, "%-7.7s ", CheckMail() ? _WmErrmsg("WM_MAIL") : "");
	p += strlen(p);
#endif

	/* Mostrar la fecha */

	if (st_line.fmtdate != -1) {
		DToStr(Today(), p, st_line.fmtdate);
		if (st_line.fmttime != -1) {
			p += strlen(p);
			*p++ = ' ';
		}
	}
	/* Mostrar la hora */
	if (st_line.fmttime != -1) 
		TToStr(Hour(), p, st_line.fmttime);


	cf = vsrow;
	cc = vscol;
	vs_moveto((short)st_line.f, (short)st_line.c);
	{
		attr_type attr = vsattr;
		vsattr = st_line.attr;
		for(i = 0; st_line.buf[i]; i++)
			_vs_putc((UChar) st_line.buf[i]);
		vsattr = attr;
	}

	/* Reponer cursor */
	vs_moveto((short)cf, (short)cc);

	/* Refrescar renglon modificado */
	_vs_update((short)st_line.f, (short)st_line.f);

#ifdef HAVE_FULL_SIGNALS
	/*	verifica tareas para avisar (alarma) */      
	if (d_f_status)   
		FindTask();
	else 
		d_f_status = TRUE;
#endif
}

static void vs_set_alarm(bool interrupt)
{
	long tm_alarm1, tm_alarm2;
	long l = time((TIMEPARAM1TYPE) NULL);
    if ( ! interrupt )
		tm_wait_getc = l;    	
    else if (tmout && l >= tm_wait_getc + tmout) {
    	WmStop();
    	return;
    }	
    	
	if (st_line.secs && l >= st_line.last+st_line.secs) {
		st_line.last = l;                    
		_vs_display_st_line();
	}
#ifdef HAVE_FULL_SIGNALS  
	tm_alarm1 = tmout ? tmout - (l - tm_wait_getc) : MAX_LONG;
	tm_alarm2 = st_line.secs ? st_line.secs - (l-st_line.last) : MAX_LONG;
	SigSet(SIGALRM, vs_alarm);
	alarm(tm_alarm1 < tm_alarm2 ? tm_alarm1 : tm_alarm2);
#if !defined(HAVE_RESTARTABLE_SYSCALLS) || defined(HAVE_SIGVEC)
	retry = TRUE;
#endif
#endif
}

static void vs_alarm(void)
{
	vs_set_alarm(TRUE);
}


/************** Rutinas de Input *******************/

/*
 * Lee un caracter de terminal.
 * si falla retorna 0.
 */
#ifdef HAVE_TTY

static UChar ps_getc(void)
{
	UChar c = 0;
	static int retries = 0;
	if (read(0, (char *) &c, 1) == 1)
		retries = 0;
	else {
		if (retries++ > 100) {
			if (errno == EIO) {  
				perror("wm");
				KillChildren();
				exit(1);
			}
//			return 0;
		}
		return 0;
	}
	return c ? c : 128;
}

#endif

/*
 * translate: Traduce una secuencia de caracteres, segun la tabla
 *            de traduccion asociada.
 *			  Si no lo encuentra retorna 0 y suena el beep.
 */
static int	  char_queue[10],
			 *char_queuep = char_queue;

static void putChar(int c)
{
	if (char_queuep-char_queue < 10)
		*char_queuep++ = c;
}

static int getChar()
{
	if (char_queuep <=char_queue)
        return ERR; 
    int ret = char_queue[0];
	--char_queuep;

	for (int *p = char_queue; p < char_queuep; ++p)
		p[0] = p[1];
	return ret;
}

static UChar translate(TRAD_ELEM *p, UChar c)
{
    putChar(c);
	while (p) {
		if ((UChar) p->c == c) {
		 	c = p->action? p->action : translate(p->match,ps_getc());
			if (c) getChar();
			return c;
		}
		 p = p->nomatch;
	}
	return 0;
}

/*
 * Lee un caracter y traduce el mismo si corresponde
 * Devuelve un caracter InterSoft valido
 */

extern bool retry;

UChar _vs_getc(void)
{
	register int c;
                          
	/* Si corresponde llamo a rutina de reloj */
	if (st_line.secs || tmout) vs_set_alarm(FALSE); 
	SetSigInt(_w_interrupt);

	do {
		retry = FALSE;
		/* Traducir el caracter */
		if ((c = getChar()) == ERR)
		    c = ps_getc();

	    if (c == 0) continue;
        
        int tr_c = 0;
        if (chset[c].tr)
        	tr_c = translate(chset[c].tr, ps_getc());
		c = tr_c ? tr_c :  chset[c].value;
	} while (retry && !wmstop);

	SetSigInt((FP)SIG_IGN);

#ifdef HAVE_FULL_SIGNALS
	/* Desabilito interrupcion por reloj */
	if (st_line.secs || tmout) {
		 SigSet(SIGALRM, SIG_IGN);
		alarm(0);
	}
#endif   
	return c;
}

#ifdef HAVE_MAIL

static bool CheckMail(void)
{
	static char mail_file[40];
	struct stat st_buf;

	/* We don't want to Abort	*/
	if (mail_file[0] == 0) {
		char *cp;
		FPCPCPI OldHandler = SetReadEnvHandler((FPCPCPI)NULLFP); 

		if ((cp = ReadEnv("MAIL")) == NULL)
			(void) sprintf(mail_file, "/usr/mail/%s", cuserid(NULL));
		else
			(void)strcpy(mail_file, cp);
		(void)SetReadEnvHandler(OldHandler); 
	}

	if (stat(mail_file, &st_buf)==ERR) return false; /* No mail box */

	return st_buf.st_size != 0;
}

#endif









