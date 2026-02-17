/********************************************************************
* Copyright (c) 1987 InterSoft Co.  All Rights Reserved
* THIS IS UNPUBLISHED PROPRIETARY SOURCE CODE OF InterSoft Co.
* The copyright notice above does not evidence any
* actual or intended publication of such source code.
*
*
* $Id: wmmov.cc,v 1.1.1.1 1996/04/16 18:50:23 gustavof Exp $
*
*
* DESCRIPTION:
*    	Funciones para el movimiento de window.  
*     
*
* ROUTINE       |  PURPOSE
*---------------+---------------------------------------------------
* void servmov	servicio del wm para mover windows.
* void wmmov	funcion que mueve la window interactivamente.
* void wimov	funcion que mueve la window a las coodenadas especif.
*********************************************************************/

#include <ideafix.h>
#include <cfix.h>
#include <wm.h>

#ifdef NO_SERVICES

void servmov() {return;}
void wmmov(wi) window wi; {return;}
void wimov(wi, fil, col) window wi; int fil, col; {return;}

#else


/* ++ Prototypes ++ */

void setborder(int f, int c, int lf, int lc, int rest, int tipo);
static void setcurr(WINDOW locwcurr, WINDOW winew);
static void reccoor(WINDOW wi, int f_dif, int c_dif);
bool _Move(WINDOW wi);

/* -- Prototypes -- */


/*
 * Rutina que arma las esquinas de la window salvando los caracteres 
 * que hay debajo, las esquinas pueden ser puestas en blink mediante
 * el parametro type.
 */

VS_CELL border[4];

void setborder(int f, int c, int lf, int lc, int rest, int tipo)
{
	int sub = 0;

	if (!rest) border[sub++] = vscreen_get(f, c);
	_vs_fill(rest ? border[sub++] : (f==lf ? '[' : C_UL) | tipo, 
			 (short)f, (short)c, 1, 1);

	if (!rest) border[sub++] = vscreen_get(f, lc);
	_vs_fill(rest ? border[sub++] : (f==lf ? ']' : C_UR) | tipo, 
			 (short)f, (short)lc, 1, 1);

	/* si tiene una sola fila */
	if (f==lf) return;

	if (!rest) border[sub++] = vscreen_get(lf, lc);
	_vs_fill(rest ? border[sub++] : C_LR | tipo, (short)lf, (short)lc, 
			 1, 1);

	if (!rest) border[sub++] = vscreen_get(lf, c);
	_vs_fill(rest ? border[sub++] : C_LL | tipo, (short)lf, (short)c, 
			 1, 1);
}

static void setcurr(WINDOW locwcurr, WINDOW winew)
{
  	setborder(locwcurr->border.f_org, locwcurr->border.c_org,
 			  locwcurr->border.f_last,locwcurr->border.c_last,
  			  TRUE, (int) A_NORMAL);
  	setborder(winew->border.f_org, winew->border.c_org,
 			  winew->border.f_last,winew->border.c_last,
  			  FALSE, (int) A_BLINK);
	vs_moveto(winew->border.f_last, winew->border.c_last);
}

/*
 * Servicio de Ideafix para mover ventanas.
 * Primero se selecciona la window a mover (esquinas en blink), y
 * al digitar enter fijo que window voy a mover (esquinas normal)
 * una vez seleccionada la ventana llevo las esquinas a donde 
 * quiero mover la ventana y digitando enter hago efectivo el 
 * movimiento.
 */
void servmov(void)
{
	int t;
	WINDOW locwcurr = wcurr;
	WINDOW pscreen = _GetWPtr(SCREEN);

	/* verifico la window corriente */
	if (locwcurr->parent==NULL) return;

	/* apago el cursor */
	WiCursor(FALSE);

	/* me paro en la window corriente */
	setborder(locwcurr->border.f_org, locwcurr->border.c_org,
			  locwcurr->border.f_last,locwcurr->border.c_last,
			  FALSE, (int) A_BLINK);
	vs_moveto(locwcurr->border.f_last, locwcurr->border.c_last);

	/* me muevo por las windows */
	while ((t=WiGetc())!=K_END && t!=K_ENTER)
		switch (t) {
		case K_CURS_UP:
			if (locwcurr->parent && locwcurr->parent!=pscreen) {
				setcurr(locwcurr, locwcurr->parent);
				locwcurr = locwcurr->parent;
			}
			else
				w_beep();
			break;
		case K_CURS_DOWN:
			if (locwcurr->child) {
				setcurr(locwcurr, locwcurr->child);
				locwcurr = locwcurr->child;
			}
			else
				w_beep();
			break;
		case K_CURS_LEFT:
			if (locwcurr->prev) {
				setcurr(locwcurr, locwcurr->prev);
				locwcurr = locwcurr->prev;
			}
			else
				w_beep();
			break;
		case K_CURS_RIGHT:
			if (locwcurr->next) {
				setcurr(locwcurr, locwcurr->next);
				locwcurr = locwcurr->next;
			}
			else
				w_beep();
			break;	
		default:
			w_beep();
		}
	
	/* restauro lo que habia debajo del recuadro */
	setborder(locwcurr->border.f_org, locwcurr->border.c_org,
			  locwcurr->border.f_last,locwcurr->border.c_last,
			  TRUE, (int) A_NORMAL);

	/* si me teclearon end me voy */
	if (t==K_END) {
		WiCursor(TRUE);
		vs_moveto(wcurr->c_fil + wcurr->f_org,  
				  wcurr->c_col + wcurr->c_org);
		return;
	}

	(void) _Move(locwcurr);

	/* return */
	WiCursor(TRUE);
	vs_moveto(wcurr->c_fil + wcurr->f_org,  
			  wcurr->c_col + wcurr->c_org);
	return;
}

bool _Move(WINDOW wi)
{
	WINDOW parent;
	int f, c, lf, lc;
	int t;

	/* obtengo las coordenadas de la window que voy a mover */
	f = wi->border.f_org;
	c = wi->border.c_org;
	lf= wi->border.f_last;
	lc= wi->border.c_last;

	parent = wi->parent;

	/* selecciono la window que voy a mover */
	setborder(f, c, lf, lc, FALSE, (int) A_NORMAL);
	vs_moveto((short)f, (short)c);

	/* muevo la window */
	while ((t = WiGetc()) != K_END && t != K_ENTER)
		switch (t) {
		case K_CURS_UP:
			if (f > parent->border.f_org) {
  				setborder (f--, c, lf--, lc, TRUE, (int) A_NORMAL);
  				setborder (f, c, lf, lc, FALSE, (int) A_NORMAL);
				vs_moveto((short)lf, (short)lc);
			}
			else
				w_beep();
			break;
		case K_CURS_DOWN:
			if (lf < parent->border.f_last) {
  				setborder (f++, c, lf++, lc, TRUE, (int) A_NORMAL);
  				setborder (f, c, lf, lc, FALSE, (int) A_NORMAL);
				vs_moveto((short)lf, (short)lc);
			}
			else
				w_beep();
			break;
		case K_CURS_LEFT:
			if (c > parent->border.c_org) {
  				setborder (f, c--, lf, lc--, TRUE, (int) A_NORMAL);
  				setborder (f, c, lf, lc, FALSE, (int) A_NORMAL);
				vs_moveto((short)lf, (short)lc);
			}
			else
				w_beep();
			break;
		case K_CURS_RIGHT:
			if (lc < parent->border.c_last) {
  				setborder (f, c++, lf, lc++, TRUE, (int) A_NORMAL);
  				setborder (f, c, lf, lc, FALSE, (int) A_NORMAL);
				vs_moveto((short)lf, (short)lc);
			}
			else
				w_beep();
			break;
		default:
				w_beep();
		}

	/* restauro lo que habia debajo de las esquinas */
	setborder(f, c, lf, lc, TRUE, (int) A_NORMAL);

	/* si me teclearon end me voy */
	if (t == K_END)
		return FALSE;

	/* finalmente muevo la window */
	wimov(wi->sub, f-(wi->border.f_org-wi->f_org)
		   				  -WiOrgRow(parent->sub),
		  				 c-(wi->border.c_org-wi->c_org)
		   				  -WiOrgCol(parent->sub));
	return TRUE;
}

/* 
 * Rutina que dado el descriptor de una window permite mover esta
 * de manera interactiva mediante la teclas de cursor.
 * Esta rutina tiene el control hasta que se teclee END.
 */
void wmmov(window wi)
{
	int fila,col,c;
	WINDOW pwi = _GetWPtr(wi);

	/* si es la SCREEN me voy */
	if (pwi->parent==NULL) return;

	while ((c=WiGetc())!=K_END) {
		fila = WiOrgRow(wi)-WiOrgRow(pwi->parent->sub);
		col  = WiOrgCol(wi)-WiOrgCol(pwi->parent->sub);
		switch(c) {
		case K_CURS_UP:
			wimov(wi, fila-1, col);
			break;
		case K_CURS_DOWN:
			wimov(wi, fila+1, col);
			break;
		case K_CURS_LEFT:
			wimov(wi, fila  , col-1);
			break;
		case K_CURS_RIGHT:
			wimov(wi, fila  , col+1);
			break;
		default:
			w_beep();
		}
	}
}

static void reccoor(WINDOW wi, int f_dif, int c_dif)
{
	WINDOW aux;

	/* si no hay dif entonces me voy */
	if (f_dif==0 && c_dif==0) return;

	/* actualizo el origen */
	wi->f_org += (short)f_dif;
	wi->c_org += (short)c_dif;

	/* actualizo los bordes */
	wi->border.f_org  += (short)f_dif;
	wi->border.c_org  += (short)c_dif;
	wi->border.f_last += (short)f_dif;
	wi->border.c_last += (short)c_dif;

	/* hago lo mismo con todos mis hijos */
	for (aux=wi->child;aux;aux=aux->next)
		reccoor(aux, f_dif, c_dif);
}

/* 
 * rutina que dado el descriptor de window y una fila y columna 
 * mueve la window a estas coordenadas.
 */
void wimov(window wi, int fil, int col)
{
	WINDOW pwi, aux;

	pwi = _GetWPtr(wi);

	/* si alguno de los parametros es negativo me voy */
	if (fil<0 || col<0) return;

	/* valido la wi pasada como parametro */
	if (pwi->parent==NULL) return;

	/* salvo window corriente */
	_save_vs_status(_GetWPtr(WICURRENT));

	/* ajusto las dimensiones con el padre */
	if (fil + pwi->nfils > pwi->parent->nfils)
		fil = pwi->parent->nfils - pwi->nfils;
	if (col + pwi->ncols > pwi->parent->ncols)
		col = pwi->parent->ncols - pwi->ncols;

	reccoor(pwi, pwi->parent->f_org + fil - pwi->f_org, 
				 pwi->parent->c_org + col - pwi->c_org);

	/* busco una window sin A_INVISIBLE */
	for(aux=_GetWPtr(_FindParCom(WICURRENT, wi)); aux->backgr & 
					 A_INVISIBLE; aux=aux->parent);

	/* hago el refresh desde el padre */
	refresh_all(aux);

	/* restauro la posicion del cursor */
	vs_moveto(wcurr->c_fil + wcurr->f_org,  
			  wcurr->c_col + wcurr->c_org);
}
#endif /* NO_SERVICES */
