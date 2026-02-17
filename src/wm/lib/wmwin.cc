/********************************************************************
* Copyright (c) 1987 InterSoft Co.  All Rights Reserved
* THIS IS UNPUBLISHED PROPRIETARY SOURCE CODE OF InterSoft Co.
* The copyright notice above does not evidence any
* actual or intended publication of such source code.
*
* $Id: wmwin.cc,v 1.3 1998/03/27 18:57:48 pablor Exp $
*
* DESCRIPTION
* Inicializacion del manejo de pantalla virtual.
* Funciones de creacion de windows.
*
* ROUTINE       |  PURPOSE
*---------------+---------------------------------------------------
*_WiRefresh		Refresh
*_WiRedraw		Redraw whole screen.
*_WiCreate		Create a window.
*_WiDelete		Delete current window.
*_WiDeleteAll 
*_WiSwitchTo
*_WiMove		Interactively moves indicated window
*_WiResize		Interactively resizes indicated window
*_WiSetBorder	Change window border.
*_WiStatusLine  Set status line.
*********************************************************************/

#include <ideafix.h>
#include <cfix.h>
#include <wm.h>
#include <ideafix/priv/wmcom.h>

/* ++ Prototypes ++ */

extern void call_vs_display_st_line(void);

static void _AdjustSize(WINDOW parent, WINDOW _new);
static void _AdjustOrg(WINDOW parent, WINDOW _new);
static void _delete_win(WINDOW wi);
#ifdef HAVE_PROC
static int _Resize(WINDOW wi);
#endif

/* -- Prototypes -- */


/* Private Functions												*/
/*
 *  Manejo de windows:
 *
 *			_WiRefresh()
 *		    _WiRedraw()
 *
 *			_WiCreate()
 *			_WiDelete()
 *			_WiDeleteAll()
 *			_WiSwitchTo()
 *
 *			_WiSetBorder()
 *
 *
 * Internas:
 *			_create_win()
 */


#define BDR(b, ext) (!!(b & ext))

/*
 * _WiCreate    : Crea un window de trabajo, que se convierte en
 *			      el activo. El 'pid' es el correspondiente al
 *                proceso activo.
 */
void _WiCreate(void)
{
	WINDOW aux;

	/*
	 * Create the window and copy the values
	 */
	window _new_wd = _GetNewWin();
	WINDOW _new    = _GetWPtr(_new_wd);
	WINDOW parent = _new_wd == SCREEN ? NULL : _GetWPtr(wm_par[0].i);

	_new->f_org 		= wm_par[1].i;
	_new->c_org 		= wm_par[2].i; 
	_new->nfils 		= wm_par[3].i;
	_new->ncols 		= wm_par[4].i;
	_new->border.attr= wm_par[5].l;
	_new->backgr		= VS_BACKGR(wm_par[6].l);


	/* si la window es invisible -> no hay borde */
	if (_new->backgr & A_INVISIBLE) {
		_new->backgr = A_INVISIBLE;
		_new->border.attr = NO_BORDER;
	}

	/* preparo el label */
	if (wm_par[7].s && wm_par[7].s[0]) {
		_new->label = (char *) Alloc(strlen((char *) wm_par[7].s)+1);
		strcpy(_new->label, (char *) wm_par[7].s);
	} else
		_new->label = NULL;

	/* Ajustar c_org y f_org si son negativos				*/
	if (_new->c_org < 0) _new->c_org = (short)WiCol(parent->sub);
	if (_new->f_org < 0) _new->f_org = (short)WiLine(parent->sub);

	/* Ajustar nfils y ncols si son negativos				*/
	if (_new->nfils <= 0) _new->nfils = (short)WiHeight(parent->sub);
	if (_new->ncols <= 0) _new->ncols = (short)WiWidth(parent->sub);
	
	/* coloco los bordes */
	/*
	_new->f_org += BDR(_new->border.attr,TOP_BORDER);
	_new->c_org += BDR(_new->border.attr,LEFT_BORDER);
	*/

	/*
	 * Link in the window tree
	 */
	_new->next  = NULL;
	_new->prev = NULL;
	_new->child = NULL;
	_new->parent= parent;
	if (parent) {
		if (parent->child == NULL)
			parent->child = _new;
		else {
			for (aux=parent->child; aux->next!=NULL; aux=aux->next);

			aux->next  = _new;
			_new->prev  = aux;
		}

		/*
		 * Ajustar los parametros
		 */
		_AdjustSize(parent, _new);
		_AdjustOrg (parent, _new);
	}

	/* Inicializar */

	/* Subindice */
	_new->sub = (short)_new_wd;

	/* Flags */
	_new->flags = 0;

	/* Habilitar cursor */
	_new->flags |= CURSOR; 

	/* Inicializacion de los bordes */
	_new->border.f_org = _new->f_org - BDR(_new->border.attr,TOP_BORDER);
	_new->border.c_org = _new->c_org - BDR(_new->border.attr,LEFT_BORDER);
	_new->border.f_last= _new->f_org + _new->nfils - 1 + 
						BDR(_new->border.attr,LOW_BORDER);
	_new->border.c_last= _new->c_org + _new->ncols - 1 + 
						BDR(_new->border.attr,RIGHT_BORDER);

	if (parent == NULL) return;

	/* seteo el numero de proceso */
	_new->pid = curr_pid;

	/* reconstruyo el arbol */
	if (parent->sub!=SCREEN)
		_RecTree(_FindParCom(WICURRENT, parent->sub), parent->sub);

	/* setear window corriente y salvar zona afectada */
	if (_SetCurrWin(_new_wd)==ERROR) {
		wm_par[0].i = ERROR;
		return;
	}

	/* Dibujar el border */
	_vs_do_border(_new, (UChar *) _new->label);

	/* seteamos el background */
	vs_setbackgr(_new->backgr);

	/* si es invisible termino aca */
	if (_new->backgr & A_INVISIBLE) {
		wm_par[0].i = (short)_new_wd;
		return;
	}

	/* Borrar la nueva window */
	vs_erase(_new->f_org, _new->c_org, _new->nfils, _new->ncols);
	vs_moveto(_new->f_org, _new->c_org);

	/* Arrancamos como background como atributo */
	_new->attrib = _new->backgr;
	vs_setattr(_new->backgr);

	/* Arrancamos con zona de scroll en toda la ventana */
	_new->top_scrl  	= 0;
	_new->bot_scrl   = _new->nfils-1;
	_new->left_scrl  = 0;
	_new->right_scrl = _new->ncols-1;

	/* Inicialmente, habilitar WRAP */
	_new->flags |= WRAP_FLAG;

	/* toco el flag de modif para no perder la window */
	_new->flags |= MODIF;

	/* Inicialmente, habilitar Cursor */
	vs_cursor(1);

	/* Return */
	wm_par[0].i = (short)_new_wd;
	return;
}

static void _AdjustSize(WINDOW parent, WINDOW _new)
{
	attr_type b_new = _new->border.attr;
	attr_type bpar = parent->border.attr;

	if (_new->nfils+BDR(b_new,TOP_BORDER)+
					BDR(b_new,LOW_BORDER) >
		parent->nfils+BDR(bpar,TOP_BORDER)+
					  BDR(bpar,LOW_BORDER))
		_new->nfils = parent->nfils+BDR(bpar,TOP_BORDER)+
								   BDR(bpar,LOW_BORDER)-
					  (BDR(b_new,TOP_BORDER)+
					   BDR(b_new,LOW_BORDER));

	if (_new->ncols+BDR(b_new,LEFT_BORDER)+
				   BDR(b_new,RIGHT_BORDER) >
		parent->ncols+BDR(bpar,LEFT_BORDER)+
					  BDR(bpar,RIGHT_BORDER))
		_new->ncols = parent->ncols+BDR(bpar,LEFT_BORDER)+
								   BDR(bpar,RIGHT_BORDER)-
					 (BDR(b_new,LEFT_BORDER)+
					  BDR(b_new,RIGHT_BORDER));
}

static void _AdjustOrg(WINDOW parent, WINDOW _new)
{
	attr_type b_new = _new->border.attr;
	attr_type bpar = parent->border.attr;

	if (_new->f_org + _new->nfils + BDR(b_new, LOW_BORDER) > 
		parent->nfils + BDR(bpar, LOW_BORDER))
		_new->f_org = parent->nfils + BDR(bpar, LOW_BORDER) - 
					 (_new->nfils + BDR(b_new, LOW_BORDER));

	if (_new->f_org==0 && BDR(b_new,TOP_BORDER) && !BDR(bpar,TOP_BORDER))
		_new->f_org = 1;

	_new->f_org += parent->f_org;
	

	if (_new->c_org + _new->ncols + BDR(b_new, RIGHT_BORDER) > 
		parent->ncols + BDR(bpar, RIGHT_BORDER))
		_new->c_org = parent->ncols + BDR(bpar, RIGHT_BORDER) - 
					 (_new->ncols + BDR(b_new, RIGHT_BORDER));

	if (_new->c_org==0 && BDR(b_new,LEFT_BORDER) && 
						!BDR(bpar,LEFT_BORDER))
		_new->c_org = 1;

	_new->c_org += parent->c_org;
}

/*
 * _WiDelete   : Elimina el window corriente. 
 *               El window activo pasa a ser el anterior del
 *               proceso activo. Si el proceso no tiene mas
 *               Windows, queda indefinido.
 */

void _WiDelete(void)
{
	WINDOW aux = _GetWPtr(wm_par[0].i);
	WINDOW par = aux->parent;

	/* Verifico si es valido */
	if (par == NULL) return;

	if (aux!=wcurr && wcurr && wcurr->sub!=SCREEN && 
							   wcurr->flags&MODIF) {
			wcurr->flags &= ~MODIF;
			_save_vs_status(wcurr);
	}

	_delete_win(aux);

	/* hago el cambio de ventana */
	aux = par;
	while (aux->child)
		for(aux=aux->child;aux->next;aux=aux->next);

	/* si borre la corriente -> seteo nueva corriente */
	if (wcurr == NULL) {
		(void) _SetCurrWin(aux->sub);
		_Reset();
	}

	/* si es invisible busco al padre */
	for (aux=par; aux->backgr & A_INVISIBLE; aux=aux->parent);

	refresh_all(aux);
}


static void _delete_win(WINDOW wi)
{
	/* borro todo lo que tenga que ver con los hijos */

	while (wi->child)
		_delete_win(wi->child);

	/* si es el primer hijo */
	if (! wi->prev)
		wi->parent->child = wi->next;
	else
		wi->prev->next = wi->next;
	if (wi->next)
		wi->next->prev = wi->prev;

	_FreeVec(wi);

	if (wi->save)
		Free(wi->save);

	if (wi->label)
		Free(wi->label);

	Free(wi);
}

/*
 * _WiDeleteAll      : Se eliminan todos los windows que pertenezcan
 *                    al proceso actual.
 *
 */
void _WiDeleteAll(void)
{
	WINDOW pscr = _GetWPtr(SCREEN);
	WINDOW aux  = pscr->child;

	while (aux) {
		WINDOW wi = aux->next;
		if (aux->pid == curr_pid) 
			_delete_win(aux);
		aux = wi;
	}

	/* Set _new current window */

	(void) _SetCurrWin(pscr->sub);
	_Reset();
	refresh_all(pscr);
}

/*
 * Refresca los windows existentes.
 */
void refresh_all(WINDOW wi)
{
	register WINDOW aux;

	_rec_vs_status(wi);
	for (aux=wi->child;aux;aux=aux->next)
		refresh_all(aux);
}

/* 
 * Repongo atributo, posicion del cursor, y estado del mismo de la
 * window corriente.
 */
void _Reset(void)
{
	vs_setattr(wcurr->attrib);
	vs_setbackgr(wcurr->backgr);
	vs_moveto(wcurr->f_org+wcurr->c_fil, wcurr->c_org+wcurr->c_col);
	vs_cursor(wcurr->flags & CURSOR);
}

void _WiSwitchTo(void)
{
	window parcom;
	WINDOW aux;

	/* routine parameters */
	window n = wm_par[0].i;
	WINDOW w = _GetWPtr(n);

	/* if invalid or no children -> error */
	if (w->sub == SCREEN || w->child) {
		wm_par[0].i = ERROR;
		return;
	}

	/* return current window */
	wm_par[0].i = _GetWPtr(WICURRENT)->sub;

	if (n == wm_par[0].i || n == WICURRENT) return;
	
	/* look for father in common */
	parcom = _FindParCom(WICURRENT, n);

	/* rebuild the tree */
	_RecTree(parcom, n);

	/* set current window */
	if (_SetCurrWin(n) == ERROR) {
		wm_par[0].i = ERROR;
		return;
	}

	/* reset active window's attributes */
	_Reset();

	/* if invisible, search for father */
	for (aux = _GetWPtr(parcom); aux->backgr & A_INVISIBLE; 
			aux = aux->parent);

	/* refresh starting at father */
	refresh_all(aux);	
}

window _FindParCom(window wi1, window wi2)
{
	return _Ante(wi1, wi2) ? wi1 :
	       _FindParCom(_GetWPtr(wi1)->parent->sub, wi2);
}

void _RecTree(window parcom, window wi)
{
	WINDOW aux;
	WINDOW pwi = _GetWPtr(wi);

	/* fin de la recursividad */
	if (parcom==wi) return;
	
	/* busco donde termina la lista de hermanos */
	for (aux=pwi;aux->next;aux=aux->next);
	
	/* si tengo por lo menos un hermano a mi derecha opero */
	if (aux!=pwi) {

		/* si soy el primer hijo -> engancho mi primer hermano */
		if (pwi->parent->child == pwi) 
			pwi->parent->child = pwi->next;
		else
			pwi->prev->next = pwi->next;

		pwi->next->prev = pwi->prev;
		pwi->next = NULL;
		pwi->prev = aux;
		aux->next = pwi;
	}

	/* llamo a partir de mi padre */
	_RecTree(parcom, pwi->parent->sub);
}

void _WiRefresh(void)
{
	vs_refresh();
	call_vs_display_st_line ();
}

/* Redibujar la pantalla completa */

void _WiRedraw(void)
{
	vs_redraw();
}

/* Cambiarle el border a una window */
void _WiSetBorder(void)
{
	wcurr->border.attr = wm_par[0].l;
	wcurr->flags |= MODIF;

	if (wcurr->label)
		Free(wcurr->label);
		
	if (wm_par[1].s && wm_par[1].s[0]) {
		wcurr->label = (char *) Alloc(strlen((char *) wm_par[1].s)+1);
		strcpy(wcurr->label, (char *) wm_par[1].s);
	} else
		wcurr->label = NULL;

	_vs_do_border(wcurr, (UChar *) wcurr->label);
}

/*
 * _WiStatusLine: Define el Funcionamiento de la status line.
 */
void _WiStatusLine(void)
{
	_st_line_setup(wm_par[0].i);
}

#ifdef HAVE_PROC
/*
	Resizes the specified window (interactively).
*/

#define NO_CHANGE		1
#define NEEDS_REDRAW	2
#define NEEDS_NO_REDRAW	3

void _WiResize(void)
{
	WINDOW wi = _GetWPtr(wm_par[0].i);
	int res;

	if (wi->child != NULL) {
		wm_par[0].i = FALSE;
		return;
	}

	if ((res = _Resize(wi)) == NO_CHANGE) {
		wm_par[0].i = FALSE;
		return;
	}

	if (wi->save) {
		Free(wi->save);
		wi->save = NULL;
	}

	wi->c_fil = wi->c_col = 0;

	_vs_do_border(wi, (unsigned char *)wi->label);

	wi->top_scrl = wi->left_scrl = 0;
	wi->bot_scrl = wi->nfils - 1;
	wi->right_scrl = wi->ncols - 1;

	_save_vs_status(wi);

	refresh_all(_GetWPtr(SCREEN));

	wm_par[0].i = (res == NEEDS_REDRAW ? TRUE : FALSE);
}

#define MIN_HEIGHT	1
#define MIN_WIDTH	1

static int _Resize(WINDOW wi)
{
	int t, retVal;
	int	lf = wi->border.f_last,
		lc = wi->border.c_last,
		f = wi->f_org - 1,
		c = wi->c_org - 1,
		maxCol = wi->parent->c_org + wi->parent->ncols +
				BDR(wi->parent->border.attr, RIGHT_BORDER) - 1,
/*				BDR(wi->parent->border.attr, LEFT_BORDER) - 1,*/
		maxRow = wi->parent->f_org + wi->parent->nfils +
				BDR(wi->parent->border.attr, LOW_BORDER) - 1,
/*				BDR(wi->parent->border.attr, TOP_BORDER) - 1,*/
		minCol = c + MIN_WIDTH + BDR(wi->border.attr, LEFT_BORDER) +
				BDR(wi->border.attr, RIGHT_BORDER) - 1,
		minRow = f + MIN_HEIGHT + BDR(wi->border.attr, TOP_BORDER) +
				BDR(wi->border.attr, LOW_BORDER) - 1;

#ifndef __NT__
	setborder(f, c, lf, lc, FALSE, (int) A_NORMAL);
#endif

	while ((t = WiGetc()) != K_END && t != K_ENTER) {
		switch (t) {
		case K_CURS_UP:
			if (lf > minRow) {
#ifndef __NT__
  				setborder(f, c, lf--, lc, TRUE, (int) A_NORMAL);
  				setborder(f, c, lf, lc, FALSE, (int) A_NORMAL);
#endif
				vs_moveto((short)lf, (short)lc);
			} else
				w_beep();
			break;
		case K_CURS_DOWN:
			if (lf < maxRow) {
#ifndef __NT__
  				setborder(f, c, lf++, lc, TRUE, (int) A_NORMAL);
  				setborder(f, c, lf, lc, FALSE, (int) A_NORMAL);
#endif
				vs_moveto((short)lf, (short)lc);
			} else
				w_beep();
			break;
		case K_CURS_LEFT:
			if (lc > minCol) {
#ifndef __NT__
  				setborder(f, c, lf, lc--, TRUE, (int) A_NORMAL);
  				setborder(f, c, lf, lc, FALSE, (int) A_NORMAL);
#endif
				vs_moveto((short)lf, (short)lc);
			} else
				w_beep();
			break;
		case K_CURS_RIGHT:
			if (lc < maxCol) {
#ifndef __NT__
  				setborder(f, c, lf, lc++, TRUE, (int) A_NORMAL);
  				setborder(f, c, lf, lc, FALSE, (int) A_NORMAL);
#endif
				vs_moveto((short)lf, (short)lc);
			} else
				w_beep();
			break;
		default:
			w_beep();
		}
	}
	
	if (t != K_END) {
		if (lf > wi->border.f_last || lc > wi->border.c_last)
			retVal = NEEDS_REDRAW;
		else
			retVal = NEEDS_NO_REDRAW;

		wi->border.f_last = lf;
		wi->border.c_last = lc;
		wi->nfils = lf - wi->f_org + 1 -
				BDR(wi->border.attr, LOW_BORDER);
		wi->ncols = lc - wi->c_org + 1 -
				BDR(wi->border.attr, RIGHT_BORDER);

		return retVal;
	} else {
#ifndef __NT__
		setborder(f, c, lf, lc, TRUE, (int) A_NORMAL);
#endif
		return NO_CHANGE;
	}
}

/*
	Moves the specified window (interactively).
*/
void _WiMove(void)
{
	WINDOW wi = _GetWPtr(wm_par[0].i);
#ifndef __NT__
	wm_par[0].i = _Move(wi);
#endif
}
#endif
