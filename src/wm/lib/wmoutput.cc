/********************************************************************
* Copyright (c) 1987 InterSoft Co.  All Rights Reserved
* THIS IS UNPUBLISHED PROPRIETARY SOURCE CODE OF InterSoft Co.
* The copyright notice above does not evidence any
* actual or intended publication of such source code.
*
*
* $Id: wmoutput.cc,v 1.1.1.1 1996/04/16 18:50:23 gustavof Exp $
*
*
* DESCRIPTION:
*      Output operations on current window.
*
* ROUTINE       |  PURPOSE
*---------------+---------------------------------------------------
*_WiPutc()		| Put a character in the current window.
*_WiKeyHelp()   | 
*********************************************************************/

#include <ideafix.h>
#include <cfix.h>
#include <wm.h>
#include <ideafix/priv/wmcom.h>

/* ++ Prototypes ++ */

static void w_setattr(attr_type a);
static void w_setbackgr(attr_type a);
static void w_rmoveto(short f, short c);
static void w_moveto(int f, int c);
static void w_savecup(void);
static void w_restcup(void);
static void w_wrap(int f);
static void w_cursor(int f);
static void w_setscroll(int top, int bot, int left, int right);
static void w_insline(int n);
static void w_delline(int n);
static void w_scroll(int n);
static void w_down(void);
static void w_up(void);
static void w_newline(void);
static void w_backspace(void);
static void w_erase(void);
static void w_eraeol(void);
static void w_eraeop(void);
static void w_eraline(void);
static void w_rawputc(UChar c);
static bool put_sec(UChar c);
static void w_inschar(int nchars, int pos);
static void w_delchar(int nchars, int pos);

/* -- Prototypes -- */

static void w_setattr(attr_type a)
{
	a = VS_ATTR(a);
	wcurr->attrib = a;
	vs_setattr(a);
}

static void w_setbackgr(attr_type a)
{
	a = VS_BACKGR(a);
	vs_setbackgr(a);
	_vs_chgbackgr(a);
	wcurr->backgr = a;
}

static void w_rmoveto(short f, short c)
{
	w_moveto(wcurr->c_fil + f, wcurr->c_col+c);
}

static void w_moveto(int f, int c)
{
	register short fabs, cabs;

	fabs = (short)(f + wcurr->f_org);
	if (fabs < wcurr->border.f_org)
		fabs = wcurr->border.f_org;
	else if (fabs > wcurr->border.f_last)
		fabs = wcurr->border.f_last;

	cabs = (short)(c + wcurr->c_org);
	if (cabs < wcurr->border.c_org)
		cabs = wcurr->border.c_org;
	else if (cabs > wcurr->border.c_last)
		cabs = wcurr->border.c_last-1;

	vs_moveto(fabs, cabs);
	wcurr->c_fil = fabs - wcurr->f_org;
	wcurr->c_col = cabs - wcurr->c_org;
}

static short savefil, savecol;

static void w_savecup(void)
{
	savefil = wcurr->c_fil;
	savecol = wcurr->c_col;
}

static void w_restcup(void)
{
	w_moveto(savefil, savecol);
}

/* 
* w_wrap:
*        Habilita o inhabilita el wraping. Si hay WRAP, al llegar a
*        borde derecho, baja a linea siguiente, (o scrollea) al
*        seguir escribiendo
*    
*/
static void w_wrap(int f)
{
	if( f )
		wcurr->flags |= WRAP_FLAG;
	else
		wcurr->flags &= ~WRAP_FLAG;
}

/* 
* w_cursor :
*        Apaga o enciende el cursor de la ventana
*        corriente.
*/
static void w_cursor(int f)
{
	if (f)
		wcurr->flags |= CURSOR;
	else
		wcurr->flags &= ~CURSOR;
	vs_cursor((short)f);
}

/*
* w_setscroll:
*             Define la region de scroll dentro de la ventana
*             corriente.
*
* top	: fila superior de la region.
* bot	: fila inferior "  "  "   "
* left	: columna del extremo izq. de la region.
* right: columna del extremo der. de la region.
*/
static void w_setscroll(int top, int bot, int left, int right)
{
	wcurr->top_scrl   = (short)top;
	wcurr->bot_scrl   = (short)bot;
	wcurr->left_scrl  = (short)left;
	wcurr->right_scrl = (short)right;
}

/*
* w_insline: Inserta una linea en blanco, en la fila 'n'. La pantalla
*          hace "scroll" hacia abajo.
*          Si la linea 'n' esta fuera de la zona de scrolling,
*          se scrollea hasta el tope de la misma, o hasta el
*          fin de la ventana.
*/
static void w_insline(int n)
{
	int nf;

	/* Probar validez de n */

	if (n < 0 || n >= wcurr->nfils) return;

	/* Calcular numero de filas a mover */

	nf	= (n < wcurr->top_scrl ? wcurr->top_scrl
		:  n < wcurr->bot_scrl ? wcurr->bot_scrl+1
		:  wcurr->nfils) - n;

		_vs_insline(wcurr->f_org+n, wcurr->c_org, nf, wcurr->ncols);
		w_rmoveto(0, 0);
}

/*
 * w_delline: Borra una linea. Todas las lineas suben una fila,
 *           y la ultima queda en blanco.
 *           Idem comportamiento w_insline().
 */
static void w_delline(int n)
{
	int nf;

	/* Probar validez de n */

	if (n < 0 || n >= wcurr->nfils) return;

	/* Calcular numero de filas a mover */

	nf	= (n < wcurr->top_scrl ? wcurr->top_scrl
		:  n < wcurr->bot_scrl ? wcurr->bot_scrl+1
		:  wcurr->nfils) - n;

	_vs_delline(wcurr->f_org+n, wcurr->c_org, nf, wcurr->ncols);
	w_rmoveto(0, 0);
}

/*
 * w_scroll:
 *           Realiza scroll de la zona de scrolling de la ventana
 *           corriente, en 'n' filas. Si 'n' es positivo, scroll
 *           hacia arriba. Si es negativo, scroll hacia abajo.
 */
static void w_scroll(int n)
{
	if(n > 0)
	  while(n--)
		_vs_delline(wcurr->f_org+wcurr->top_scrl,
					wcurr->c_org+wcurr->left_scrl,
				   	wcurr->bot_scrl-wcurr->top_scrl+1,
					wcurr->right_scrl-wcurr->left_scrl+1);
	else
	  while(n++)
		_vs_insline(wcurr->f_org+wcurr->top_scrl,
					wcurr->c_org+wcurr->left_scrl,
				   	wcurr->bot_scrl-wcurr->top_scrl+1,
				   	wcurr->right_scrl-wcurr->left_scrl+1);
}
/*
 * Move cursor down. Check for scroll
 */
static void w_down(void)
{

	/* Check for Bottom Scroll */

	if ( wcurr->c_fil == wcurr->bot_scrl ) w_scroll(1);	

	/* Check for low border */

	else if(wcurr->c_fil < wcurr->nfils-1) wcurr->c_fil++;

	w_rmoveto(0, 0);
}
/*
 * Move cursor up check for scroll
 */
static void w_up(void)
{
	/* Check for Top Scroll */

	if ( wcurr->c_fil == wcurr->top_scrl ) w_scroll(-1);	

	/* Check for Top border */

	else if (wcurr->c_fil > 0) wcurr->c_fil--;

	w_rmoveto(0, 0);
}

/*
 *	Process a newline
 */
static void w_newline(void)
{
	if (wm_mode != MODE_TERM) wcurr->c_col = 0;
	w_down();
}
/*
 * Process a backspace
 */
static void w_backspace(void)
{
	if (wcurr->c_col > 0) w_rmoveto(0, -1);
	else {
		wcurr->c_col = wcurr->ncols - 1;
		w_up();
	}
}

static void w_erase(void)
{
	vs_erase(wcurr->f_org,wcurr->c_org,wcurr->nfils,wcurr->ncols);
	w_moveto(0,0);
}

static void w_eraeol(void)
{
	vs_erase(wcurr->f_org+wcurr->c_fil,wcurr->c_org+wcurr->c_col,
			1, wcurr->ncols - wcurr->c_col);
}

static void w_eraeop(void)
{
	vs_erase(wcurr->f_org + wcurr->c_fil,
			 wcurr->c_org + wcurr->c_col,
			 1,
			 wcurr->ncols - wcurr->c_col);
	vs_erase(wcurr->f_org + wcurr->c_fil + 1, wcurr->c_org,
			 wcurr->nfils - wcurr->c_fil - 1 ,wcurr->ncols);
}

static void w_eraline(void)
{
	vs_erase(wcurr->f_org + wcurr->c_fil, wcurr->c_org,
			 1, wcurr->ncols - wcurr->c_col);
}

/*
 * _WiKeyHelp:Retorna un puntero a string con la descripcion
 *            de la tecla cuyo codigo de recibe.
 *  La long. del texto fue truncada en inicializacion a 
 *  KEY_HELP_DSC chars.
 *  Si el segundo argumento es NULL retorna puntero a buffer
 * interno estatico.
 */
void _WiKeyHelp(void)
{
	/* parametros de la rutina */
	UChar k  = wm_par[0].u;
	UChar *s = wm_par[1].s;

	static   UChar internal[KEY_HELP_DSC+1];
	register UChar *dest;
	KEY_TAB  *pk   = key_tab;

	while (pk->code && pk->code != k) 
		pk++;

	dest = s ? s : internal;
	if (pk->code == k) 
		strcpy((char*)dest, pk->descr);
	else
		strcpy((char*)dest, _CharName(k));

	wm_par[0].s = dest;
}


/*
 * Coloca un caracter en la posicion actual de la pantalla.
 * Actualiza el cursor.
 */
static void w_rawputc(UChar c)
{
	/* Check for rigth border */
	if ( wcurr->c_col++ < (wcurr->ncols-1) ) {
		wcurr->flags &= ~LINEOVERFLOW;
		_vs_putc(c);
	}
	else if( wcurr->flags & WRAP_FLAG ) {
		_vs_putc(c);
		if (wcurr->c_col == wcurr->ncols) {
			wcurr->c_col = 0;
			w_newline();
		}
	}
	else {
		/* If already in OVERFLOW mark with a right arrow */
		if (wcurr->flags & LINEOVERFLOW) c = C_RARROW;
		wcurr->flags |= LINEOVERFLOW;
		_vs_putc(c);
		w_rmoveto(0, -1);
	}
}
/*
 * Now the processing of the ANSI secuences
 * All Secuences have the form :
 *				ESC intchr p1 ; p2 ; ..... LETTER
 *
 *	intchr can be 0 = Inexistent.
 *				  [ = The Standard.
 *  LETTER  the action.
 *
 *			ESC [ p1 A = Cursor Up p1 lines.
 *			ESC [ p1 B = Cursor Down p1 lines.
 *			ESC [ p1 C = Cursor Forward p1 chars.
 *			ESC [ p1 D = Cursor Backward p1 chars.
 *
 *			ESC [ p1 ; p2 H = Cursor position.
 *			ESC [ p1 ; p2 f =  "  "   "   "
 *
 *			ESC [ J	   = Erase to end of display.
 *			ESC [ 2 J  = Erase display.
 *			ESC [ K	   = Erase to end of line.
 *			ESC [ 2 K  = Erase line.
 *
 *			ESC [ p1 ; ... ; pn m = Stea los atributos marcados.
 *
 *			ESC [ p1 ; p2 L = Insert p1 lines at line p2 (def actual).
 *			ESC [ p1 ; p2 M = Delete p1 lines at line p2 (def actual).
 *
 *			ESC [ p1 ; p2 @ = Insert p1 chars at col. p2 (def actual).
 *			ESC [ p1 ; p2 P = Delete p1 chars at col. p2 (def actual).
 *
 *			ESC [ 4 h		= Insert mode on.
 *			ESC [ 4 l		= Insert mode off.
 *
 *			ESC [ 5 h		= Cursor on.
 *			ESC [ 5 l		= Cursor off.
 *
 *			ESC [ 7 h		= Wrap on.
 *			ESC [ 7 l		= Wrap off.
 *
 *			ESC [ s			= Save Cursor Position.
 *			ESC [ u			= Restor Cursor Position.
 *
 *			ESC [ p1 z		= Set current atribute.
 *
 *			ESC [ p1 ; p2 r = Set Scroll Region.
 *
 *			ESC [ p1 k		= Imprime el texto de help de la tecla p1.
 *
 *			ESC [ p1 y		= Scroll p1 lines.
 *
 *			ESC D = Cursor Down Scroll if last line.
 *			ESC M = Cursor Up Scroll if first line.
 *			ESC E = Newline.
 */

static bool put_sec(UChar c)
{
#define MAXPAR 10
	static short _par[MAXPAR], npar;
	static UChar intchr;
	static bool minus = FALSE;
	int i;

	if (isdigit(c)) {
		if (_par[npar] == DEFAULT) _par[npar] = 0; 
		_par[npar] = _par[npar] * 10 + c - '0';
		if (minus) {
			_par[npar] = - _par[npar];
			minus = FALSE;
		}
		return TRUE;
	}

	switch (c) {
	case '\033' :
		npar = 0;
		for (i = 0; i < MAXPAR; ++i) _par[i] = DEFAULT;
		intchr = 0;
		return TRUE;

	case ';' : 
		if (npar < MAXPAR-1) ++npar;
		return TRUE;

	case '[' :
		intchr = c;
		return TRUE;
	case '-' :
		minus = TRUE;
		return TRUE;
	case 'E' :
		w_newline();
		break;

	case 'D' :
		if (intchr == 0) {
			w_down();
			break;
		}
	case 'A' :
	case 'B' :
	case 'C' :
		if (_par[0] == DEFAULT) _par[0] = 1;
		w_rmoveto( 	(c=='B') ? _par[0] : (c=='A') ? -_par[0] : 0,
					(c=='C') ? _par[0] : (c=='D') ? -_par[0] : 0);
		break;
	case 'H' :
		w_moveto(_par[0] == DEFAULT ? 0 : _par[0]-1,
				 _par[1] == DEFAULT ? 0 : _par[1]-1);
		break;
	case 'f' :
		w_rmoveto(_par[0], _par[1]);
		break;
	case 'F' :
		w_setbackgr(((long)_par[0] << 15) | _par[1]);
		break;
	case 'J' :
		if (_par[0] == 2) w_erase();
		else w_eraeop();
		break;
	case 'K' :
		if (_par[0] == 2) w_eraline();
		else w_eraeol();
		break;

	case 'm' :
		{
			attr_type attr = vsbackgr;
			for (i = 0; i <= npar; ++i)
				if (_par[i] != DEFAULT) attr |= attr_tab[_par[i]].value;
			w_setattr(attr);
		}
		break;

	case 'L' :
		for (i = _par[0] == DEFAULT ? 1 : _par[0]; i ; i--)
			w_insline(_par[1] == DEFAULT ? wcurr->c_fil : _par[1]);
		break;

	case 'M' :
		if (intchr == 0) w_up();
		else {
			for (i = _par[0] == DEFAULT ? 1 : _par[0]; i ; i--)
				w_delline(_par[1]==DEFAULT ? wcurr->c_fil : _par[1]);
		}
		break;

	/* case '@' : */
	case '(' :
		w_inschar(_par[0]==DEFAULT ? 1 : _par[0],
				  _par[1]==DEFAULT ? wcurr->c_col : _par[1]);
		break;
	case 'P' :
		w_delchar(_par[0]==DEFAULT ? 1 : _par[0],
				  _par[1]==DEFAULT ? wcurr->c_col : _par[1]);
		break;

	case 'h' :
	case 'l' :
		i = c == 'h';
		switch (_par[0]) {
		case 5 :
			w_cursor(i);
			break;
		case 7 :
			w_wrap(i);
			break;
		}
		break;

	case 's' :
		w_savecup();
		break;
	case 'u' :
		w_restcup();
		break;

	case 'r' :
		w_setscroll(_par[0] == DEFAULT ? 0 : _par[0] - 1,
				   (_par[1] == DEFAULT ? wcurr->nfils : _par[1]) - 1,
				    _par[2] == DEFAULT ? 0 : _par[2] - 1,
				   (_par[3] == DEFAULT ? wcurr->ncols : _par[3]) - 1);
		break;
	case 'k' :
		{
			UChar s[KEY_HELP_DSC+1];
			int   j = 0;
			register UChar *p = WiKeyHelp((UChar) _par[0], s);

			for (j = 0; *p; j++) w_rawputc(*p++);
			for (; j<KEY_HELP_DSC; j++) w_rawputc(' ');
			break;
		}

	case 'y' :
		w_scroll(_par[0]);
		break;
	case 'z' :
		w_setattr(((long)_par[0] << 15) | _par[1]);
		break;
	}
	return FALSE;
}

void _WiFlush(void)
{
	static bool in_sec = FALSE;
	register UChar * baux;
	register UChar c7;

	/* si es invisible me voy */
	if (wcurr->backgr & A_INVISIBLE) return;

	wcurr->flags |= MODIF;

	for (baux = wm_par[0].s; *baux; baux++) {
		if (in_sec) {
			in_sec = put_sec(*baux);
			continue;
		}

		if ((c7 = *baux & 0x7F) >= ' ') {
			w_rawputc(*baux);
			continue;
		}

		switch ( c7 ){
		case '\007' :
			_vs_beep();	/* Bell */
			break;

		case '\b' :
			w_backspace();
			break;

		case '\r':
			wcurr->c_col = 0;
			w_rmoveto(0, 0);
			break;

		case '\013' :
			w_down();
			break;

		case '\n' 	:
			w_newline();
			break;
		
		case '\f' :
			w_erase();
			break;

		case '\t':
			do {
				w_rawputc(' ');
			} while( ! (wcurr->flags&LINEOVERFLOW) && wcurr->c_col%TAB);
			break;

		case '\033' :
			in_sec = put_sec('\033');
			break;
		}
	}
}

static void w_inschar(int nchars, int pos)
{
	_vs_inschar(wcurr->f_org + wcurr->c_fil,
				wcurr->c_org + wcurr->c_col, nchars);
}

static void w_delchar(int nchars, int pos)
{
	_vs_delchar(wcurr->f_org + wcurr->c_fil,
				wcurr->c_org + wcurr->c_col, nchars);
}
