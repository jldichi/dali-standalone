/********************************************************************
* Copyright (c) 1987 InterSoft Co.  All Rights Reserved
* THIS IS UNPUBLISHED PROPRIETARY SOURCE CODE OF InterSoft Co.
* The copyright notice above does not evidence any
* actual or intended publication of such source code.
*
* $Id: wihelp.cc,v 1.2 2002/03/15 20:25:33 hardaiz Exp $
*
* DESCRIPTION
* Window function Library. Display message routine.
*
* ROUTINE       |  PURPOSE
*---------------+---------------------------------------------------
*WiHelp			 Display help text on the screen.
*********************************************************************/

#include <ideafix.h>
#include <cfix.h>
#include <ideafix/priv/gndefs.h>
#include <cracker/pserconn.h>

/*
 * WiHelp
 * ------
 *
 * Rutina para desplegar textos de ayuda por pantalla.
 * La ventana es desplegada en la posicion actual del cursor.
 *
 *                +label----------------------+   \
 *                |                           | 0  |
 *                |       t e x t o s         | 1  |
 *                |                           | 2  | nfils
 *                |                           | 3  |
 *                +---------------------------+   /
 *                | comandos para movimiento  |
 *                |  del texto en ventana     |
 *                +---------------------------+
 *
 * nfils, ncols : Dimensiones de la window de texto.
 * label        : Titulo en la window.
 * get_txt      : Puntero a funcion
 * garg         : Argumento para get_txt:
 *
 * get_txt(garg,n) retorna texto de las opciones contando de 1.
 *            Retorna un puntero a caracter con el texto.
 *            Si n < 1 || n > numero de lineas de opciones
 *            debe retornar NULL.
 *
 * COMANDOS:
 *            PAGE_UP  PAGE_DOWN  CURS_UP  CURS_DOWN  ENTER END
 *
 */

/* +++ CONFIG +++ */
#define MAXCOLS      256
/* --- CONFIG --- */

#define add_text(i, st)	WiMoveTo(i,1),WiEraEol(),WiPuts(st),\
						WiMoveTo(nfils-1,1)

extern fpHomeSearch HomeSearch;

/* VARARGS4 */
void WiHelp(int nfils, int ncols, char *label, CPFPCPI get_txt, char *garg)
{
	if (!crackerClient()) {
		int  HelpCols = 0;
		char HelpComms[3][MAXCOLS];
		int line,       /* Siguiente linea de titulos a leer */
			c, i, j;

		char *text;     /* Texto de ayuda leido con 'get_txt'*/
		UChar s1[256], s2[256];
		attr_type border;

		/* Crear descripcion de comandos  */
		(void) sprintf(HelpComms[0],(char*)_WiErrmsg(HELPFMT1),
				WiKeyHelp(K_END,s1),
				WiKeyHelp(K_META,s2));

		(void) sprintf(HelpComms[1], (char*)_WiErrmsg(HELPFMT2),
				WiKeyHelp(K_CURS_DOWN,s1),
				WiKeyHelp(K_CURS_UP,s2));

		(void) sprintf(HelpComms[2], (char*)_WiErrmsg(HELPFMT3),
						WiKeyHelp(K_PAGE_DOWN,s1), 
						WiKeyHelp(K_PAGE_UP,s2));

		/* Calcular la linea mas larga */
		for(i=0, c=0; i<3; ++i)
			if( (line = StrDspLen(HelpComms[i])) > c)
				c = line; 
		HelpCols = c + 2;

		/* Ajustar ancho de la window */
		if (ncols) ncols += 3;
		if ( ncols && ncols < HelpCols )
			ncols = HelpCols; 

		if ( ncols == WiWidth(WiDefPar()) || ncols == 0 )
			border = TOP_BORDER|LOW_BORDER|SLINE_TYPE|A_REVERSE|A_CYAN_BG;
		else 
			border = SLINE_BORDER | A_REVERSE|A_CYAN_BG;

		/* Crear la ventana donde desplegaremos texto */
		if(WiCreate(WiDefPar(), WiLine(WiDefPar())+1, WiCol(WiDefPar())+1, 
					nfils+4, ncols, border, label, A_NORMAL) == ERR) {
					 w_beep();
					 return;
		}

		WiWrap(FALSE);

		/* Ajustar dimensiones */
		nfils = WiHeight(WICURRENT) - 4;
		ncols = WiWidth(WICURRENT);

		/* Dibujar subdivision del recuadro e imprimir comandos */
		WiCursor(FALSE);
		WiSetAttr(A_REVERSE|A_CYAN_BG);
		WiMoveTo(nfils, -1);
		WiPutc(C_LEFT);
		for(i=0; i<ncols; i++) WiPutc(C_HORIZ);
		WiMoveTo(nfils, ncols);
		WiPutc(C_RIGHT);
		for (i = 0; i < 3; i++) {
			WiMoveTo(nfils+i+1, 0);
			WiPuts(HelpComms[i]);
			for (j=StrDspLen(HelpComms[i]); j < ncols; j++)
				WiPutc(' ');
		}
		WiSetAttr(A_NORMAL);

		/* Parametros iniciales */
		WiMoveTo(0, 0);
		WiSetScroll(0, nfils - 1, 0, ncols-1);
		c        = K_PAGE_DOWN;		/* fake command to begin */
		line     = 1;

		do {
			switch (c) {
			case K_CURS_UP:
				if (  !(text = (*get_txt)(garg,line-nfils-1)) ){
					w_beep();
					continue;
				}
				else {
					WiScroll(-1);
					add_text(0,text);
					line--;
				}
				break;

			case K_CURS_DOWN:
			case K_ENTER:
				if( !(text = (*get_txt)(garg,line))){ /* more opts ? */
					WiPutc('\007');
					continue;
				}
				WiScroll(1);
				add_text(nfils-1,text);
				line++;
				break;				

			case K_PAGE_UP:
				if(line <= nfils+1){
					WiPutc('\007');
					continue;
				}
				line -= (nfils*2);
				if(line <= 0)
					line = 1;

disp_page:
			case K_PAGE_DOWN:
				if(! (*get_txt)(garg,line)){
					w_beep();
					continue;
				}
				for(i = 0; i < nfils ;i++, line++){
					text = (*get_txt)(garg,line);
					add_text(i,text ? text : NULL);
				}
				break;

			case K_META:
			  {
				int aux; /* Line to search from & result of search */

				if((aux = HomeSearch(WiOrgRow(WICURRENT)+nfils,
									 WiOrgCol(WICURRENT),
									 ncols,line,garg,get_txt))){
					line = aux;
					goto disp_page;
				}
				break;
			  }
				
			default:
				w_beep();
				continue;
			}

		} while( (c = WiGetc()) != K_END ) ;

		WiDelete(WICURRENT);

	} else {
		prServerConn(PrCmd::wiHelp) << Int(nfils) << Int(ncols) << label;
		prServerConn.flush();
		
		Int req;
		prServerConn >> req;
		while (req == PrRequest::getTxt) {
			Int fromLine, toLine;
			
			prServerConn >> fromLine >> toLine;
			for (int i = fromLine; i <= toLine; i++) {
				char *opt = get_txt(garg, i);
				prServerConn << opt;
				// no more data
				if (opt == NULL) break;
			}			
			prServerConn.flush();

			prServerConn >> req;
		}			
	}
}

