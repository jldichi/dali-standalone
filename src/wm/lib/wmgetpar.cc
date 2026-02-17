/********************************************************************
* Copyright (c) 1987 InterSoft Co.  All Rights Reserved
* THIS IS UNPUBLISHED PROPRIETARY SOURCE CODE OF InterSoft Co.
* The copyright notice above does not evidence any
* actual or intended publication of such source code.
*
*
* $Id: wmgetpar.cc,v 1.5 2004/06/25 20:22:56 diegoa Exp $
*
*
* DESCRIPTION:
*      Window Manager Library module to operate with current
*      window.
*
* ROUTINE       |  PURPOSE
*---------------+---------------------------------------------------
*_WiGetAttr		 Return current attribute.
*_WiLine		 Return current line in a window.
*_WiCol			 Return current column in a window.
*_WiWidht		 Return real width in column of the window.
*_WiHeight		 Return real height in rows of the window.
*_WiInChar		 Return the specificate position character.
*_WiInAttr		 Return the specificate position attribute.
*_WiOrgCol		 Return the first column of the window
*_WiOrgRow		 Return the first row of the window
*_WiGetTab		 Return tab length
*_SetTab		 Set the tab length
*_WiParent       return the parent of the window
*_WiCurrent
*_GetSessionWM		 Return user data connection (SNODE)
*_SetSessionWM		 Set user data connection (SNODE)
*********************************************************************/

#include <ideafix.h>
#include <cfix.h>
#include <wm.h>
#include <ideafix/priv/wmcom.h>


FILE *fpdebug;
#define doit(s, v)         fpdebug = fopen("/tmp/exec.log", "ab+");        \
                        if (fpdebug != NULL){                           \
                        fprintf(fpdebug, s, v);                            \
                        fprintf(fpdebug, "\n");                 \
                        fflush(fpdebug);                \
                        fclose(fpdebug);        \
                        }


/* Funcion privada para este modulo: si no encuentra el descriptor de
 * window pasado como parametro entonces escribira algo y retornara el 
 * pointer de la window screen.
 */

static WINDOW WINDOW_TAB[MAX_WINDOWS];
WINDOW	wcurr;
SNODE sessionNode = {0, "", 0, 0, "", "", 0, 0};


WINDOW  _GetWPtr(window wi)
{
	if (wi < 0 || wi >= MAX_WINDOWS || WINDOW_TAB[wi]==NULL) {
		/* sacar algun mensaje de error y */
		return WINDOW_TAB[SCREEN];
	}
	return WINDOW_TAB[wi];
}

/* 
 * retorna la primera entrada libre dentro de la tabla de windows 
 */
window _GetNewWin(void)
{
	window i;
	for (i = 0; i < MAX_WINDOWS; i++)
		if (i != 1 && WINDOW_TAB[i] == NULL) {
			WINDOW_TAB[i] = (WINDOW) Alloc(sizeof(struct wistru));
			return i;
		}
	return ERROR;
}

/* 
 * setea la window p como corriente, si la anterior fue modificada
 * entonces se salva su contenido; retorna un status.
 */
int _SetCurrWin(window p)
{
	WINDOW 	q = WINDOW_TAB[WICURRENT]; /* tomo la window corriente */

	if (p<0 || p>MAX_WINDOWS || WINDOW_TAB[p]==NULL)
		return ERROR;
	wcurr = WINDOW_TAB[WICURRENT] = WINDOW_TAB[p];

	vs_moveto(wcurr->c_fil+wcurr->f_org, wcurr->c_col+wcurr->c_org);

	/* si la window es invisible -> me voy */
	if (q && (q->backgr & A_INVISIBLE)) return OK;

	if (q &&  q != WINDOW_TAB[SCREEN]) {
		if (q->flags&MODIF) {
			q->flags &= ~MODIF;
			return _save_vs_status(q);
		}
	}
	return OK;
}

/* 
 * setea la posicion de la tabla de windows [sub] con el pointer wi.
 */
void _SetWTab(window sub, WINDOW wi)
{
	if (sub<MAX_WINDOWS && sub>=0)
		WINDOW_TAB[sub] = wi;
}

/* 
 * setea en NULL la posicion de la tabla de windows [wi->sub].
 */
void _FreeVec(WINDOW wi)
{
	if (wi) {
		WINDOW_TAB[wi->sub] = NULL;
		if (WINDOW_TAB[WICURRENT] == wi)
			WINDOW_TAB[WICURRENT] = wcurr = NULL;
	}
}

/* 
 * retorna si wi1 es antecesor de wi2 dentro del arbol de windows.
 */
bool _Ante(window wi1, window wi2)
{
	WINDOW aux;

	for(aux=_GetWPtr(wi2);aux->parent && aux->parent!=_GetWPtr(wi1);
						  aux=aux->parent);
	return (aux->parent) ? TRUE : FALSE;
}		

/*
 * _WiGetAttr() : retorna el atributo corriente
 */
void _WiGetAttr(void)
{
	wm_par[0].l = wcurr ? 
				  wcurr->attrib : A_NORMAL;
}

/*
 *	_WiOrgCol: retorna la columna de origen de la window pasada como
 *    		   parametro
 */
void _WiOrgCol(void)
{
	 wm_par[0].i = _GetWPtr(wm_par[0].i)->c_org;
}

/*
 *	_WiOrgRow: retorna la fila de origen de la window pasada como
 *			   parametro
 */
void _WiOrgRow(void)
{
	wm_par[0].i = _GetWPtr(wm_par[0].i)->f_org;
}

/*
 * _WiLine: retorna la linea corriente relativa a la window pasada como 
 *			parametro
 */
void _WiLine(void)
{
	WINDOW w = _GetWPtr(wm_par[0].i);

	wm_par[0].i= wcurr && (w==wcurr || _Ante(wm_par[0].i, wcurr->sub)) ?
			 	 (wcurr->f_org-w->f_org)+wcurr->c_fil : 0;
}

/*
 * _WiCol: retorna columna corriente de la window pasada como 
 *   	   parametro
 */
void _WiCol(void)
{
	WINDOW w; 

	w = _GetWPtr(wm_par[0].i);

	wm_par[0].i = wcurr && (w==wcurr || _Ante(wm_par[0].i,wcurr->sub)) ?
			 	 (wcurr->c_org-w->c_org)+wcurr->c_col : 0;
}

/*
 * _WiWidth: retorna el ancho real en columnas de la window pasada
 *			 como parametro
 */
void _WiWidth(void)
{
	wm_par[0].i = _GetWPtr(wm_par[0].i)->ncols;
}

/*
 * _WiHeight: retorna el alto real en filas de la la window pasada
 *			 como parametro
 */
void _WiHeight(void)
{
	wm_par[0].i = _GetWPtr(wm_par[0].i)->nfils;
}

/*
 * _WiInChar: leer caracter en posicion fil, col.
 *           Si fil, col son DEFAULT se usa la posicion
 *           del cursor.
 */
void _WiInChar(void)
{
	/* parametros de la rutina */
	int fil = wm_par[0].i;
	int col = wm_par[1].i;

	if (wcurr == NULL) return;

	if (fil == DEFAULT) fil = wcurr->c_fil;
	if (col == DEFAULT) col = wcurr->c_col;

	if (fil < 0 || fil >= wcurr->nfils || col < 0 || col >= wcurr->ncols)
		wm_par[0].u = 0;
	else
		wm_par[0].u = vs_inchar(wcurr->f_org + fil, wcurr->c_org + col);
}

/*
 * _WiInAttr:leer atributo en posicion fil, col.
 *           Si fil, col son DEFAULT se usa la posicion
 *           del cursor.
 */
void _WiInAttr(void)
{
	/* parametros de la rutina */
	int fil = wm_par[0].i;
	int col = wm_par[1].i;

	if (wcurr == NULL) return;

	if (fil == DEFAULT) fil = wcurr->c_fil;
	if (col == DEFAULT) col = wcurr->c_col;

	if (fil < 0 || fil >= wcurr->nfils || col < 0 || col >= wcurr->ncols)
		wm_par[0].l = 0L;
	else
		wm_par[0].l = vs_inattr(wcurr->f_org + fil, wcurr->c_org + col);
}

/*
 * _WiGetTab		 Return tab length
 */
void _WiGetTab(void)
{
	wm_par[0].i = TAB;
}

/*
 * _WiSetTab		 Set the tab length
 */
void _WiSetTab(void)
{
	TAB = wm_par[0].i;
}

/* 
 * _WiParent()		 Return the parent of the window
 */
void _WiParent(void)
{
	if (wm_par[0].i <= SCREEN  ||  wm_par[0].i > MAX_WINDOWS)
		wm_par[0].i = SCREEN;
	else	
		wm_par[0].i = (_GetWPtr(wm_par[0].i)->parent)->sub;
}

void _WiCurrent(void)
{
	wm_par[0].i = _GetWPtr(WICURRENT)->sub;
}


/* +++++ IXSCA +++++ */

void _GetSessionWM(void) {
		
	int hostId = wm_par[0].l;

/*	if (hostId == sessionNode.hostId || sessionNode.hostId == 0) {
		// ver si caduco la sesion
		DATE expirationDate;
		TIME expirationTime;

		expirationDate = sessionNode.expDate;
		expirationTime = sessionNode.expTime;

		DATE todayDate = Today();
		TIME actualTime = Hour();
		
		if (expirationDate > todayDate) {
			wm_par[0].i = (short) sessionNode.idSession;
		} else {
			if ((expirationDate == todayDate) && (expirationTime > actualTime)) { 
				wm_par[0].i = (short) sessionNode.idSession;
			} else {
				wm_par[0].i = (short) -1;
			}
		}
	}
*/
	if (sessionNode.hostId != 0) {
		wm_par[0].l = (long) sessionNode.idSession;
		wm_par[1].s = (UChar*) sessionNode.login;
		wm_par[2].s = (UChar*) sessionNode.idUser;;
		wm_par[3].s = (UChar*) sessionNode.password;
		wm_par[4].s = (UChar*) sessionNode.profile;
		char date[50];
		DToStr(sessionNode.expDate, date, DFMT_SEPAR);
		char time[50];
		TToStr(sessionNode.expTime, time, TFMT_SEPAR);
		wm_par[5].s = (UChar*) date;
		wm_par[6].s = (UChar*) time;
	} else {
		wm_par[0].l = 0;
		wm_par[1].s = (UChar*) "";
		wm_par[2].s = (UChar*) "";
                wm_par[3].s = (UChar*) "";
                wm_par[4].s = (UChar*) "";
                wm_par[5].s = (UChar*) "";
                wm_par[6].s = (UChar*) "";
	}	
}

void _SetSessionWM(void) {
	
	char *date;
	char *time;


	int hostId = (int) wm_par[0].l;
	int idSession = (int) wm_par[1].l;
	int idUser = (int) wm_par[2].l;
	String login((char*) wm_par[3].s);
	String password((char*) wm_par[4].s);
	String profile((char*) wm_par[5].s);
//	String date((char*) wm_par[6].s);
//	String time((char*) wm_par[7].s);
/*	
	date = (char *) Alloc(strlen((char *) wm_par[6].s)+1);
	strcpy(date, (char *) wm_par[6].s);

	time = (char *) Alloc(strlen((char *) wm_par[7].s)+1);
	strcpy(time, (char *) wm_par[7].s);
	
	DATE expDate = StrToD(date);
	TIME expTime = StrToT(time);
*/
	// retorna 0 o 1, false, o true ;
//	if (sessionNode.hostId == 0) {
		sessionNode.hostId = hostId;
		sessionNode.idSession = idSession;
		sessionNode.idUser = idUser;
		strcpy(sessionNode.login, toCharPtr(login));
		strcpy(sessionNode.password, toCharPtr(password));
		strcpy(sessionNode.profile, toCharPtr(profile));
//		sessionNode.expDate = expDate;
//		sessionNode.expTime = expTime;
		wm_par[0].i = TRUE;
//	} else {
//		wm_par[0].i = FALSE;
//	}
}

/* ----- IXSCA ----- */
