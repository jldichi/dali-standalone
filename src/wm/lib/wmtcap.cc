/********************************************************************
* Copyright (c) 1987 InterSoft Co.  All Rights Reserved
* THIS IS UNPUBLISHED PROPRIETARY SOURCE CODE OF InterSoft Co.
* The copyright notice above does not evidence any
* actual or intended publication of such source code.
*
*
* $Id: wmtcap.cc,v 1.7 2004/09/29 18:35:01 damianm Exp $
*
*
* DESCRIPTION:
*      Descripcion del proposito general del modulo
*
* ROUTINE       |  PURPOSE
*---------------+---------------------------------------------------
*_tcap_cursor	 
*_tcap_flush	 
*_tcap_putc	 
*_tcap_open	 
*_tcap_moveto	 
*_tcap_setsrc	 
*_tcap_insline	 
*_tcap_delline	 
*_tcap_eeol	 
*_tcap_cls	 
*_tcap_setattr	 
*_tcap_resetattr	 
*********************************************************************/

#include <unistd.h>
#include <stdlib.h>

#include <ideafix.h>
#include <cfix.h>

#include <ifound/str.h>

#include <ideafix/priv/gndefs.h>

#ifdef HAVE_TTY

#include <wm.h>

/* +++ Prototypes +++ */
void set_sgr(bool flag);
/* --- Prototypes --- */

/* +++ CONFIG +++ */
/* A buffer to hold the used capabilities */

#define TCAPSLEN 2048

static char tcapbuf[TCAPSLEN];
static attr_type curr_attr;
static bool USE_SGR = FALSE;

/* Used capabilities */
static char *cm,	/* Cursor Motion  			-  cup		*/
			*ce,	/* Clear end of line  		-  el		*/
			*cl,	/* Clear screen 			-  clear	*/
			*as,	/* Alternate char set		-  smacs	*/
			*ae,	/* End Alt char set			-  rmacs	*/			
			*mb,	/* Mode blink				-  blink	*/
			*md,	/* Mode bold				-  bold		*/
			*mr,	/* Mode reverse				-  rev		*/
			*us,	/* Mode underscore			-  smul		*/
			*so,	/* Mode standout			-  smso		*/
			*sa,	/* Define the video attributes #1 - #9 - sgr	*/
			*me,	/* Turn off attributes		-  sgr0		*/
			*al,	/* Insert a line			-  il1		*/
			*dl,	/* Delete a line			-  dl1		*/
			*cs,	/* Set Scroll region		-  csr		*/
			*sr,	/* Scroll reverse			-  ri		*/
			*is,	/* Initialize the terminal  -  is1		*/
			*vi,	/* Make cursor invisible	-  civis	*/
			*vs,	/* Make cursor very visible	-  cvvis	*/
			*ve,	/* Make cursor visible		-  cnorm	*/
			*le;	/* Move left one space		-  cub1		*/
static int sg;	/* Magic cookie glitch		-  xmc		*/
static bool ms,	/* Safe to move in standout mode - msgr	*/
			xs;		/* Standout not erased by overwriting (hp) - xhp	*/

// Global static variables
static unsigned int velen, vilen;

// Pad character
static char	PC;


#define OUTBUFFSIZE 4096
static UChar outbuff[OUTBUFFSIZE];
static UChar *_outp = outbuff;
static int pscursor = -1;

/* Macro to put a capab. */

inline void putpad(const char *str)
{
	if (_outp - outbuff + strlen(str) >= OUTBUFFSIZE - 1)
		_tcap_flush();
	tputs(str, 1, (IFP) _tcap_putc);
}

/* Private Functions												*/
void _tcap_cursor(int n)
{
	if (pscursor != n) {
		if		(n && ve)  (void) _global_write(1, n==1 ? ve : vs, velen);
		else if (!n && vi) (void) _global_write(1, vi, vilen);
		pscursor = n;
	}
}

void _tcap_flush(void)
{
	unsigned int len = _outp - outbuff;

	if (!vscursor || len > (2 * velen+vilen))
		_tcap_cursor(FALSE);

	(void) _global_write(1, (char *) outbuff, len);

	_tcap_cursor(vscursor);

	_outp = outbuff;
}

void _tcap_putc(UChar x)
{
	if (_outp-outbuff >= OUTBUFFSIZE) _tcap_flush();
	*_outp++ = x;
}

/*
 *   rutina que abre el termcap e inicializa las capacidades
 */

void _tcap_open(void)
{

	char *t, *p, *TERM, *LINES, *COLUMNS;
	char tcbuf[2048];
	
	TERM = ReadEnv("TERM");

#if HAVE_TGETENT_DIFFERENT_RETURN_VALUE
	if (tgetent(tcbuf, TERM) != 0) {
#else
	if (tgetent(tcbuf, TERM) != 1) {
#endif
		(void) fprintf(stderr, _WmErrmsg("INVALID_TERM"), TERM);
		_global_exit(1);
	}
#if HAVE_TERMINFO_LINES_BUG
	FPCPCPI prevhandler = SetReadEnvHandler(NULL);

	if ((LINES = ReadEnv("LINES")) != NULL) {
		VS_LINES = atoi(LINES);
	}
	else
		VS_LINES = (short)tgetnum("li");

	if ((COLS = ReadEnv("COLUMNS")) != NULL) {
		VS_COLS = atoi(COLUMNS);
	}
	else
		VS_COLS = (short)tgetnum("co");
#else
	VS_LINES = (short)tgetnum("li");
	VS_COLS  = (short)tgetnum("co");
#endif
	/*
	* Read used capabilities
	*/
	p = tcapbuf;
	t = tgetstr("pc", &p);
	if (t) PC = *t;
	
	is = tgetstr("is", &p);
	cm = tgetstr("cm", &p);
	ce = tgetstr("ce", &p);
	cl = tgetstr("cl", &p);

	vi = tgetstr("vi", &p);
	ve = tgetstr("ve", &p);
	vs = tgetstr("vs", &p);
	if (vs==NULL) vs = ve;

	velen = ve ? strlen(ve) : 0;
	vilen = vi ? strlen(vi) : 0;

	cs = tgetstr("cs", &p);
	if (cs) {
		has_setscr = TRUE;
		sr = tgetstr("sr", &p);
	}
	else {
		al = tgetstr("al", &p);
		dl = tgetstr("dl", &p);
		if (al && dl) has_idlsec = TRUE;
	}	

	so = tgetstr("so", &p);
	as = tgetstr("as", &p);
	ae = tgetstr("ae", &p);	
	mb = tgetstr("mb", &p);
	md = tgetstr("md", &p);

	/* veo si la terminal tiene reverse */
	mr = tgetstr("mr", &p);
	if (mr==NULL) term_rev = 0;

	sa = tgetstr("sa", &p);
	me = tgetstr("me", &p);
	us = tgetstr("us", &p);
	le = tgetstr("le", &p);
	sg = tgetnum("sg");
	ms = tgetflag("ms");
	xs = tgetflag("xs");

	/* If has magic_cookie_glitch or */
	/* ceol_standout_glitch then no atributes */
	if (sg > 0 || xs)
		term_rev = 0;

	/* do not erase cursor when no reverse */
	if (!term_rev)
		vi = vs = ve; 

	if (p>=&tcapbuf[TCAPSLEN]) {
		(void) fprintf(stderr, _WmErrmsg("TCAP_TOO_LONG"));
		_global_exit(1);
	}
	if (cl==NULL || cm==NULL) {
		(void) fprintf(stderr, _WmErrmsg("TCAP_INCOMPLETE"));
		_global_exit(1);
	}
	
	if (ce != NULL) eol_length = (short)strlen(ce);
	_tcap_init();
}

/*
* Init the screen 
*/
void _tcap_init(void)
{
	if (is) putpad(is);
}

void _tcap_moveto(register int row, register int col)
{
	putpad(tgoto(cm, col, row));
	if (!ms)
		_tcap_setattr(curr_attr);
}

void _tcap_setscr(register int from, register int to)
{
	putpad(tgoto(cs, to, from));
}

void _tcap_scrup(void)
{
	putpad(sr);
}

void _tcap_insline(void)
{
	putpad(al);
}

void _tcap_delline(void)
{
	putpad(dl);
}

void _tcap_eeol(void)
{
	putpad(ce);
}

void _tcap_cls(void)

{
	putpad(cl);
}

void _tcap_setattr(attr_type n)
{
	/* if not magic_cookie_glitch and */
	/* not ceol_standout_glitch set attributes */
	if (sg <= 0 && !xs)
#ifdef HAVE_SGR
		if (USE_SGR) {
			putpad(tparm(sa, n & A_STANDOUT, n & A_UNDERLINE, has_color ? 0 : (n & A_REVERSE), n & A_BLINK, 0, n & A_BOLD, 0, 0, n & A_ALTERNATE));
			if (has_color) {
				if (n & A_REVERSE) {
					putpad(col_fg[VS_COLOR(A_BLACK)]);
					putpad(col_bg[VS_BG_COLOR(A_WHITE_BG)]);
				}
				if (VS_COLOR(n))	putpad(col_fg[VS_COLOR(n)]);
				if (VS_BG_COLOR(n))	putpad(col_bg[VS_BG_COLOR(n)]);
			}
		}
		else
#endif
		{
			if ((n & A_STANDOUT) && so)		putpad(so);
			if ((n & A_BLINK) && mb)		putpad(mb);
			if ((n & A_BOLD) && md)			putpad(md);
			if ((n & A_UNDERLINE) && us)	putpad(us);
			if (has_color) {
				if (n & A_REVERSE) {
					putpad(col_fg[VS_COLOR(A_BLACK)]);
					putpad(col_bg[VS_BG_COLOR(A_WHITE_BG)]);
				}
				if (VS_COLOR(n))	putpad(col_fg[VS_COLOR(n)]);
				if (VS_BG_COLOR(n))	putpad(col_bg[VS_BG_COLOR(n)]);
			}
			else
				if ((n & A_REVERSE) && mr)		putpad(mr);
			if ((n & A_ALTERNATE) && as)	putpad(as);
		}
	curr_attr |= n;
}

void _tcap_resetattr(void)
{
	/* if not magic_cookie_glitch and */
	/* not ceol_standout_glitch reset attributes */
	if (sg <= 0 && !xs) {
		if (me) putpad(me);
		if (ae) putpad(ae);
	}
	curr_attr = 0;
}

void use_sgr(bool flag)
{
	USE_SGR = flag;
}
#endif
