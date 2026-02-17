/********************************************************************
* Copyright (c) 1987 InterSoft Co.  All Rights Reserved
* THIS IS UNPUBLISHED PROPRIETARY SOURCE CODE OF InterSoft Co.
* The copyright notice above does not evidence any
* actual or intended publication of such source code.
*
* $Id: wipopup.cc,v 1.6 2007/03/07 15:21:58 hardaiz Exp $
*
* DESCRIPTION
* Rutinas de manejo de menues.
*
* ROUTINE       |  PURPOSE
*---------------+---------------------------------------------------
*PopUpMenu		 Rutina para desplegar popup menus.
*PopUpVMenu		 Muestra un menu, y retorna la opcion seleccionada.
*PopUpLMenu		 Similar a 'popupv', con lista de args.
*********************************************************************/

#include <string.h>
#ifdef _AIX
#include <strings.h>
#endif

#include <ideafix.h>
#include <cfix.h>
#include <ideafix/priv/gndefs.h>
#include <cracker/pserconn.h>

#ifndef IDEAWIN

Int buffLines(Int nrows, Int ncols);

Int buffLines(Int nrows, Int ncols)
{
	Int aux = 512/ncols;
	Int ret = nrows >= aux ? nrows+1 : aux;
	return ret+ret%2;
}


/* ++ Prototypes ++ */

extern void sendLines(Int, Int, CPFPCPI, char *);
extern int searchTxt(Int, bool, const char *, char *, CPFPCPI, Int = 0);
extern fpHomeSearch HomeSearch;

static void display_text(int i, int ncols, char *wtext, int txtsize);
static void scrolltxt(char *wtext, int nfils, int txtsize, int n);
static void chg_line_attr(int fila, int ncols, attr_type attr, char *wtext, int txtsize);
static void put_arrows(int down, int up, int nfils, int ncols);
static char *lget(char *av[], int n);
#ifdef EXEC_TAKES_TIME
static void save_menu_state(int row, int col, int atr, char *wtext, int txtsize);
#endif

/* -- Prototypes -- */

#define  CTCHARS  32

#define  StoreTxt(ind, txt) \
	 (void)strncpy(wtext+(txtsize*(ind)),(txt),txtsize);

#define  GetTxt(fila) (&wtext[txtsize*(fila)])

/*
 * PopUpMenu
 * -------
 *
 * Rutina para desplegar popup menus. Crea una
 * Window dejando dos lineas en blanco:
 *
 *                +label----------------------+   \
 *     blank ---->|                           | 0  |
 *                |                           | 1  |
 *                |                           | 2  | nfils = 4
 *     blank ---->|                           | 3  |
 *                +---------------------------+   /
 *
 * nfils, ncols : Dimensiones de la window utilizada.
 * label        : Titulo en la window.
 * get_optx     : Puntero a funcion
 * garg         : Argumento para get_optx:
 * execf        : Funcion que ejecuta opcion seleccionada.
 * earg         : argumento para execf.
 *
 * char *
 * get_optx(char *garg[], int n) retorna texto de las opciones
 *			  contando de 1.
 *            Retorna un puntero a caracter con el texto.
 *            Si n < 1 || n > numero de lineas de opciones
 *            debe retornar NULL.
 *
 * execf(earg,n) ejecuta la opcion n (cuenta de 0).
 *
 * Seleccion de opciones:
 *       Se utilizan las teclas de cursor, y paginacion. La
 *       opcion seleccionada aparece en reverse. Para seleccionar
 *       se oprime ENTER. Tambien se puede indicar la primera
 *       letra de la opcion. Se explora desde la opcion actual
 *       en reverse hacia abajo.
 *       Si oprime META, se puede buscar una expresion regular.
 *
 *
 * Si 'execf' es NULLFP, popup() retorna un numero coicidente con
 * la opcion seleccionada (desde 0), o bien ERR si el usuario
 * digito END.
 *
 * NOTA: Si no hay opciones retorna ERR.
 */

static void display_text(int i, int ncols, char *wtext, int txtsize)
{
	char *st = GetTxt(i-1);
	int len = ncols + strlen(st) - StrDspLen(st) - 2;

	(void) WiPrintf("\033[%d;2H\033[s%-*.*s\033[u", i+1,len,len,st);
}

static void scrolltxt(char *wtext, int nfils, int txtsize, int n)
{
	register int i;

	if (n>0)
		for (i = 0; i < nfils; i++)
			mCopy(GetTxt(i), GetTxt(i+1), txtsize);
	else
		for (i = nfils-1; i ; i--)
			mCopy(GetTxt(i), GetTxt(i-1), txtsize);
}

/*
 * Cambia el atributo de un texto en una fila
 */
static void chg_line_attr(int fila, int ncols, attr_type attr,
		char *wtext, int txtsize)
{
	char *st = GetTxt(fila-1);
	int len = ncols + strlen(st) - StrDspLen(st) - 2;
	char *satr;

	if ( attr == A_REVERSE ) satr = "7;34;47";
	else if ( attr == A_BLINK ) satr = "7;5;34;47";
	else satr = "";

	(void) WiPrintf("\033[%d;2H\033[s\033[%sm%-*.*s\033[u\033[0m",
		   fila+1, satr, len,len,st);
}

static void put_arrows(int down, int up, int nfils, int ncols)
{
	char *fmt = "\033[34;7m\033[%d;%dH%c\033[%d;%dH%c\033[0m";

	WiSetBackGr(A_NORMAL);
	if (down)
		WiPrintf(fmt, nfils+2, ncols+1, C_DARROW, nfils+2, 0, C_DARROW);
	if (up)
		WiPrintf(fmt, 1,       ncols+1, C_UARROW,       1, 0, C_UARROW);
	WiSetBackGr(A_BLUE_BG);
}

int PopUpMenu(int nfils, int ncols, char *label, CPFPCPI get_optx,
		char *garg, FPCPPI execf, char *earg[], int attrib)
{
	if (crackerClient()) {
		bool haveExecF = execf != NULL;
    	IFPICCPCCP _display_handler 	   	= displayHandler 	   (NULL);
		IFPICCPCCP _display_error_handler	= displayErrorHandler(NULL);
		IFPICCPCCP _fatal_error_handler   	= fatalErrorHandler  (NULL);
		IFPICCPCCP _ari_error_handler 		= ariErrorHandler    (NULL);

		Int buffLines_ = buffLines(nfils, ncols);
		prServerConn(PrCmd::popUpMenu) << (Int)nfils << (Int)ncols << label
									   << (Int)attrib << (Int)haveExecF;
		sendLines(1, buffLines_, get_optx, garg);

		Int req;
		prServerConn >> req;
		while (req != PrRequest::nullReq) {
			switch (req) {
			case PrRequest::getTxt:
			{
				Int i, fromLine, toLine;
				prServerConn >> fromLine >> toLine;
				for (i = fromLine; i <= toLine; i++) {
					char *opt = get_optx(garg, i);
					prServerConn << opt;
					// no more data
					if (opt == NULL)
						break;
				}
				prServerConn.flush();
				break;
			}
			case PrRequest::srchTxt:
			{
				Int from, backward;
				String rxp;
				prServerConn >> from >> backward >> rxp;

				from = searchTxt(from, backward, (const char *)toCharPtr(rxp),
							     garg, get_optx);

			   	assert(from >= 0);
				prServerConn << from;
	            if (from > 0)
	            	sendLines(from, from-1 + buffLines_, get_optx, garg);
				else
					prServerConn.flush();
			}

				break;
			case PrRequest::execFun:
			{
				assert(haveExecF);
				Int n;
				prServerConn >> n;
				(*execf)(earg, n);
				prServerConn(PrCmd::endEventLoop);
				prServerConn.flush();
				break;
			}
			default:
				assert(0);
				break;
			}
			prServerConn >> req;
		}
		displayHandler(_display_handler);
		displayErrorHandler(_display_error_handler);
		fatalErrorHandler(_fatal_error_handler);
		ariErrorHandler(_ari_error_handler);

		return prServerConn.result();
	} else {
		int opt, 	    	/* Numero de opcion, comienza de 1 */
			old_opt = 0,    /* Opcion anterior				   */
			line,       	/* Siguiente linea de titulos a leer */
			lastline = 0,	/* Ultima linea de texto existente */
			c, i;			/* Auxiliares						*/
		bool	more_up, more_down,
				prev_up, prev_down;
		bool select;    	/* flag. Indica opcion selecc. por letra */
		char *text = NULL;     	/* option's text */
		char *wtext;
		int  txtsize;

		/* Check wether label fits or not */
		if ((i = StrDspLen(label)) > (ncols-2)) ncols = i + 2;
		if (nfils < 2) nfils = 3;
		if (ncols < 2) ncols = 3;

		/* Calcular si las opciones llenan la window */
		txtsize = ncols + CTCHARS;
		wtext = (char *) Alloc((unsigned) txtsize * (nfils-2));
		for (i = 0; i < (nfils-2); ++i) {
			char *p = (*get_optx)(garg, i+1);
			if (!p) {
				lastline = i;
				break;
			}
			StoreTxt(i, p);
		}

		/* No options? */
		if (i == 0) {
			Free(wtext);
			(void) WiMsg((char*)_WiErrmsg(NO_OPTIONS));
			return ERR;
		}
		else if (i != (nfils-2)) nfils = i + 2;

		if (WiCreate(WiDefPar(), WiLine(WiDefPar())+1,
			WiCol(WiDefPar())+2, nfils, ncols, STAND_BORDER | A_BLUE,
			label, A_BLUE_BG) == ERR ) {
				w_beep();
				Free(wtext);
				return ERR;  /* Chequear !!! */
		}

		/* Get parameters */
		nfils = WiHeight(WICURRENT);
		ncols = WiWidth(WICURRENT);

		/* Ajustar parametros  iniciales							*/
		nfils   -= 2;
		WiSetScroll(1, nfils, 0, ncols-1);
		WiCursor(FALSE);
		opt      = 0;
		c        = ERR;		/* Indica que es primera vez		   */
		line     = 1;
		prev_up	 = prev_down = FALSE;

		do {
			select = FALSE;
			switch (c) {
			case ERR:				/* Primera vez						*/
			 {
				i = 1;
				do {
					display_text(i, ncols, wtext, txtsize);
					i++;
					line++;
				} while (i <= nfils);
				old_opt = 0;
				opt = line - nfils;
				break;
			 }
			case K_CURS_UP:
				if (WiLine(WICURRENT) == 1) {
					if (!(text = (*get_optx)(garg, line - nfils - 1))) {
						w_beep();
						continue;
					}
					else {
						line--;
						WiScroll(-1);
						scrolltxt(wtext, nfils, txtsize, -1);
						StoreTxt(0, text);
						display_text(1, ncols, wtext, txtsize);
					}
				}
				old_opt = opt--;
				break;

			case ' ':
			case K_CURS_DOWN:
				if (lastline && opt == lastline) {
					w_beep();
					continue;  /* This is last opt   */
				}
				if (WiLine(WICURRENT) == nfils) { /* cursor at bottom */
					/* more opts ? */
					if (!(text = (*get_optx)(garg, line))) {
						lastline = line - 1;
						w_beep();
						break;
					}
					line++;
					WiScroll(1);
					scrolltxt(wtext, nfils, txtsize, 1);
					StoreTxt(nfils - 1, text);
					display_text(nfils, ncols, wtext, txtsize);
				}
				old_opt = opt++;
				break;

			case K_PAGE_UP:
				if (line <= nfils + 1) {
					w_beep();
					continue;
				}
				line -= (nfils*2);
				if (line <= 0)
					line = 1;

disp_page:
			case K_PAGE_DOWN:
				if ((c != K_PAGE_UP && lastline && opt == lastline) ||
					!(text = (*get_optx)(garg, line))) {
					w_beep();
					continue;
				}
				i = 1;
				do {
					StoreTxt(i - 1, text ? text : "");
					display_text(i, ncols, wtext, txtsize);
					if (!lastline && !text) lastline = line - 1;
					i++; line++;
					text = (*get_optx)(garg, line);
				} while (i <= nfils);
				old_opt = 0;
				opt = line - nfils;
				break;

			case K_ENTER:
			case K_PROCESS:

#ifdef EXEC_TAKES_TIME
				save_menu_state(opt - line + nfils + 1, ncols,
						POP_BLINK, wtext, txtsize);
#endif

				if (execf == NULLFP) {
					WiDelete(WICURRENT);
					Free(wtext);
					return opt-1;
				}
				if (attrib & POP_VOLATIL)
					/* window should disappear */
					WiDelete(WICURRENT);
				else if (attrib & POP_BORDER) {
					/* border changes */
					WiSetBorder(SLINE_BORDER, label);
					WiSetBackGr(A_NORMAL);
				}

				{
					window wi = WiCurrent();
					(*execf)(earg, opt - 1);
					WiSwitchTo(wi);
				}

				if (attrib & POP_VOLATIL) {		/* si no esta me voy 	*/
					Free(wtext);
					return opt-1;
				}

				WiSetBorder(STAND_BORDER | A_BLUE, label);
				WiSetBackGr(A_BLUE_BG);
				put_arrows(prev_down, prev_up, nfils, ncols);
				old_opt = 0;
				break;

			case K_META:
			{
				int aux; /* Line to search from & result of search */

				aux = line - nfils;
				if (aux <= 0) aux = 1;
				if ((aux = HomeSearch(WiOrgRow(WICURRENT)+nfils,
									  WiOrgCol(WICURRENT),
									  ncols, aux, garg, get_optx))){
					line = aux;
					goto disp_page;
				}
				break;
			}
			case K_PAGE_LEFT:
				line = 1;
				opt = line - nfils;
				goto disp_page;
				break;
			case K_PAGE_RIGHT:
				if (!lastline) {
					lastline = line;
					while ((*get_optx)(garg, lastline++))
						;
					lastline -= 2;
				}
				line = lastline;
				opt = line - nfils;
				goto disp_page;
				break;
			default:
			  {
				int j, n, ch1;

				/* Buscar coincidencia con car. digitado (desde opt) */
				i=opt-line+nfils+1;
				/*
					"i = i % nfils, i++" en lugar de "i = i % nfils + 1"
					por problemas en el optimizador (pablog)
				*/
				for(n = 0; n < nfils; i = i % nfils, i++, n++) {

					for(j=1; WiInChar(i, j) == (UChar)' ' ; ++j)
						;                 /* NOTE:                */
					ch1 = WiInChar(i, j); /* Trying to aviod some */
										  /* compilers bug here ! */
					if(tolower(ch1) == tolower(c)) {
						old_opt = opt;
						opt     = line-nfils+i-1;
						c       = K_ENTER;
						select  = TRUE;
						WiRefresh();
						break;
					}
				}
				if( !select ) { /* No selecciona opcion. */
					w_beep();
					continue;
				}
			  }

			} /* end switch */

			/* Actualizar indicacion sobre si hay mas opciones */
			if (lastline) more_down = line <= lastline;
			else          more_down = (*get_optx)(garg, line) ? 1 : 0;
			more_up = (line - nfils) > 1;
			if (more_up!=prev_up || more_down!=prev_down) {
				WiSetBorder(STAND_BORDER | A_BLUE, label);
				put_arrows(more_down, more_up, nfils, ncols);
				prev_up	  = more_up;
				prev_down = more_down;
			}

			/* Poner en reverse de acuerdo a nuevo estado */
			if (old_opt > 0)
				chg_line_attr(old_opt+nfils-line+1,ncols,A_NORMAL,
														   wtext,txtsize);
			chg_line_attr(opt-line+nfils+1,ncols, A_REVERSE,wtext,txtsize);

		} while ((select || (c = WiGetc()) != K_END) && c);

		WiDelete(WICURRENT);
		Free(wtext);
		return ERR;
	}
}

#ifdef EXEC_TAKES_TIME

/*
	Estas rutinas son para guardar el estado del menu, para
	despues poder pner la opcion en blinking si se va a
	llamar a un WCMD, o el shell
*/

static int sav_row = 0, sav_col = 0, sav_atr = 0, sav_txtsize = 0;
static char *sav_wtext = NULL;

static void save_menu_state(int row, int col, int atr,
		char *wtext, int txtsize)
{
	sav_row = row;
	sav_col = col;
	sav_atr = atr;
	sav_wtext = wtext;
	sav_txtsize = txtsize;
}

void HiLiteMenu(void)
{
	if ( sav_atr & POP_BLINK ) {
		chg_line_attr(sav_row , sav_col, A_BLINK,
								sav_wtext, sav_txtsize);
		WiRefresh();
	}
}

#endif

/*
 * Rutina para tomar un texto de una tabla. Esta finaliza
 * con string vacio, o puntero nulo.
 * La variable externa 'nops' es seteada con el numero maximo
 * de opciones antes de llamar a las funciones.
 * Es utilizada por 'PopUpLMenu' y 'PopUpVMenu'.
 */
/* Global static variables                                         */
static int nops = 0;

/* Private Functions												*/
static char *lget(char *av[], int n)
{
	return n < 1 || n > nops ? NULL : av[n-1];
}


/*
 * PopUpVMenu:
 *         Muestra un menu, y retorna la opcion seleccionada.
 *
 * nfils, ncols, label : Parametros de la Window.
 * argv                : Arreglo de punteros con texto de opciones.
 *                       Debe indicarse el final con string vacio
 *                       o puntero nulo.
 */
int PopUpVMenu(int nfils, int ncols, char *label, char *argv[])
{
	if (crackerClient()) {
		prServerConn(PrCmd::popUpVMenu) << (Int)nfils << (Int)ncols << label;
		int nops = 0;
		do {
			prServerConn << argv[nops];
		} while (argv[nops] && argv[nops++][0]);
		return prServerConn.result();
	} else {
		int ret;

		/* contar la cantidad de opciones */
		for(nops = 0; argv[nops] && argv[nops][0]; nops++)
			;

		ret = PopUpMenu(nfils, ncols, label, (CPFPCPI)lget, (char *) argv,
				 (FPCPPI)NULLFP,(char**)NULL, POP_STATIC|POP_BORDER);
		WiRefresh();
		return ret;
	}
}

/*
 * PopUpLMenu:
 *         Muestra un menu, y retorna la opcion seleccionada.
 *         Similar a 'popupv', con lista de args.
 *
 * Sinopsis:
 * 		   PopUpLMenu(nfils, ncols, label, arg1, ..... , NULL)
 *
 * nfils, ncols, label : Parametros de la Window.
 * arg1, ... argn      : Strings de opciones.
 *
 */

/* +++ CONFIG +++ */

#define MAXARGS		50

int PopUpLMenu(int nfils, int ncols, const char *label, ...)
{
	if (crackerClient()) {
		prServerConn(PrCmd::popUpVMenu) << (Int)nfils << (Int)ncols << label;

		va_list ap;
		va_start(ap, label);
		char *arg;
		do {
			arg = va_arg(ap, char*);
		    prServerConn << arg;
	   } while (arg);

	    return prServerConn.result();
	} else {
		int  ret;
		char *args[MAXARGS];
		va_list ap;

		nops  = 0;
		va_start(ap, label);
		while( (args[nops++] = va_arg(ap, char*)) != NULL)
			;
		nops--;
		va_end(ap);
		ret = PopUpMenu(nfils, ncols, (char*)label, (CPFPCPI)lget,
						(char *) args, (FPCPPI)NULLFP,(char**)NULL,
						POP_STATIC|POP_BORDER);
		WiRefresh();
		return ret;
	}
}

#endif /*IDEAWIN*/

void sendLines(Int from, Int to, CPFPCPI get_optx, char *garg)
{
 	for (Int i = from; i <= to; i++) {
		char *line = get_optx(garg, i);
 		String sline = (line ? toString(line) : NULL_STRING) ;
		if(sline.rTrim() == NULL_STRING &&line)
			sline = " ";
		prServerConn << sline;
	    // No more data
		if (!line)
			break;
	}
	prServerConn.flush();

}

