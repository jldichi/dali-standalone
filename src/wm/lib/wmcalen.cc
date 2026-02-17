/********************************************************************
* Copyright (c) 1987 InterSoft Co.  All Rights Reserved
* THIS IS UNPUBLISHED PROPRIETARY SOURCE CODE OF InterSoft Co.
* The copyright notice above does not evidence any
* actual or intended publication of such source code.
*
*
* $Id: wmcalen.cc,v 1.1.1.1 1996/04/16 18:50:22 gustavof Exp $ 
*
*
* DESCRIPTION:
*    Aplicacion calendario
*
* ROUTINE       |  PURPOSE
*---------------+---------------------------------------------------
*
*
*
*********************************************************************/

#include <ideafix.h>
#include <cfix.h>

#include <ifound/str.h>

#include <ideafix/priv/gndefs.h>
#include <ideafix/priv/ical.h>

#include <wm.h>

/* ++ Prototypes ++ */

DATE w_calendar(DATE idate, char *user, bool remind);
static void PongoCal(int fila, int col);
static void LlenarCal(void);
static void Flechas(int  c, int  cantidad);
static void SacoCal();
static bool CambioMes(int cantidad);
static void MovDiaArri(int cantidad);
static void RestSetAtrib(int cant, int filant, int colant, int pfila, int pcol);
static void MovDiaAba(int cantidad);
static void MovDiaIzq(int cantidad);
static void MovDiaDer(int cantidad);
static void SetAtrib(short d, short m, short y, short today, int *rest);

/* -- Prototypes -- */

static window wprev, wcal;          /* descriptores de window p/cal */
static DATE fecha_act;              /* indica la fecha corriente             */
static int  hoja[6][7];             /* matriz donde almaceno los dias del mes*/
static int  fila, col, pfila, pcol; /* fila y columnas activas del dia actual*/
static char dayw[21];		 		 /* vec. con los nombres de los dias 	  */

DATE w_calendar(DATE idate, char *user, bool remind)
{
	DATE today;				 /* fecha pasada como param o Today() 	  */
	int  c;					 /* var para captura del caracter         */
	int  cantidad = 1 ;		 /* utilizado para el desp. rapido en cal */
	int  longi    = 0 ;		 /* longitud en digitos cantidad (<=4)    */
	int  n = 0;				 /* var aux p/calculo de cantidad         */

	/* Obtiene el descriptor de la window actual */
	wprev = WiCurrent();

	/* seteo today con el parametro o con Today() */
	fecha_act = today = (idate == NULL_DATE) ? Today() : idate;

	/*
	 * Genera la linea con el nombre de los dias para el calendario
	 */
	for (c = 0; c < 7; c++) {
		strncpy(dayw+(c*3), DayName((DATE)c), 2);
		dayw[(c*3)+2] = ' ';
	}

	/* Coloca el calendario en pantalla      */
	PongoCal(WiLine(SCREEN)+1,WiCol(SCREEN)+2);

	/*
	 * Loop principal hasta que tecleen END
	 */
	while(1) {
		n = longi = 0;
		cantidad = 1;
		/* 
		 * acumulo los digitos para el movimiento rapido
		 */
		while (isdigit(c=WiGetc())) {
			if(longi<=3) {
				cantidad = n = (n * 10) + (c - '0');
				longi++;
			}
			else  {
				w_beep();
				n = longi = 0;
				cantidad = 1;
			}
		}
		if (c == K_END) break;
		/*
		 * si no es un digito entonces switcheo segun la tecla digitada
		 * K_CURS_UP     	--> Movimiento por semana hacia atras
		 * K_CURS_DOWN 		--> Movimiento por semana hacia adelante
		 * K_CURS_LEFT		--> Movimiento por dia hacia atras
		 * K_CURS_RIGHT		--> Movimiento por dia hacia adelante
		 * K_PAGE_DOWN		--> Movimiento por mes hacia atras
		 * K_PAGE_UP		--> Movimiento por mes hacia adelante
		 * t o T 			--> vuelve a today()
		 */
		switch(c){                              
			case K_CURS_UP:
			case K_CURS_DOWN:
			case K_CURS_LEFT:
			case K_CURS_RIGHT:
				Flechas(c,cantidad);
				break;
			case K_PAGE_DOWN:
				fecha_act = AddMonth(fecha_act,cantidad);
				LlenarCal();
				break;
			case K_PAGE_UP:
				fecha_act = AddMonth(fecha_act,-cantidad);
				LlenarCal();
				break;
			case 't':
			case 'T':
				fecha_act = today;
				LlenarCal();
				break;
			default:
				w_beep();
		}
	}
	
	SacoCal();						 /* Borro todas las ventanas  	  */
	WiSwitchTo(wprev);				/* Restauro la window original */
	
	/* Retorna fecha actual */	
	return fecha_act;
}

/*
Coloca la ventana del calendario y muestra la fecha corriente
*/
static void PongoCal(int f, int c)
{

	wcal = WiCreate(SCREEN, f, c, LARG_CAL, ANCH_CAL, 
					STAND_BORDER|A_YELLOW_BG,
					_WmErrmsg("LEY_CALENDAR"), A_BLUE_BG);
	LlenarCal();
}
                   
/*
 * Rutina que verifica si hay cambio de mes si lo hay 
 * llena el calendario con el mes correspondiente
 */
static bool CambioMes(int cantidad)
{
	long fecha;
	bool result;
	short d, m, y;
	static short anio = 0;
	static short mes  = 0;

	/* debo verificar para no irme de rango */
	fecha  = (long) fecha_act + (long) cantidad;

	if (fecha<0 || fecha>=((long) MAX_SHORT)) {
		w_beep();
		return TRUE;
	}

	DToDMY(fecha_act+cantidad, &d, &m, &y);

	if (m!=mes || anio!=y) {
		fecha_act += (DATE)cantidad;
		LlenarCal();
		result = TRUE;
	}
	else
		result = FALSE;
		
	anio = y;
	mes  = m;

	return result;
}

/*
 * Rutina para el movimiento del calendario por dias o por semanas
 */
static void Flechas(int c, int cantidad)
{
	switch(c) {
	case K_CURS_UP:
		if (CambioMes(-(7*cantidad))) return;
		MovDiaArri(cantidad);
		break;
	case K_CURS_DOWN:
		if (CambioMes(7*cantidad)) return;
		MovDiaAba(cantidad);
		break;
	case K_CURS_LEFT:
		if (CambioMes(-cantidad)) return;
		MovDiaIzq(cantidad);
		break;
	case K_CURS_RIGHT:
		if (CambioMes(cantidad)) return;
		MovDiaDer(cantidad);
	} 
}

/*
 * Rutina para moverme por semana(s) hacia atras
 * En caso de moverme de a uno entonces no reescribo toda la ventana 
 * del calendario. Debo restaurara el atributo del dia que me voy por
 * lo cual llamo a RestSetAtrib.
 * En caso que el desplazamiento sea mayor que 1 entoces directamente 
 * reescribo la ventana del calendario.
 */
static void MovDiaArri(int cantidad)
{
	int colant,filant;

	colant = pcol;
	filant = pfila;

	fila  -= cantidad;
	pfila -= cantidad;
	fecha_act -= (DATE)(7*cantidad);
	RestSetAtrib(7*cantidad, filant, colant, pfila, pcol);
}

/*
 * Rutina que restaura el dia que acabo de abandonar 
 * (recordar que el dia corriente esta en reverse).
 */
static void RestSetAtrib(int cant, int filant, int colant, int f, int c)
{
	short d, m, y;
	int rest = 0;

	DToDMY(fecha_act, &d, &m, &y);
	SetAtrib(d + cant, m, y, d, &rest);
	WiMoveTo(filant, colant-1);
	WiPrintf("%2d",d + cant);

	WiMoveTo(f, c-1);
	SetAtrib(d, m, y, d, &rest);
	WiPrintf("%2d",d);

	WiMoveTo(f, c);
	if (rest) WiSetAttr(A_NORMAL|A_BLUE_BG);
}

/*
Rutina para moverme por semana(s) hacia adelante
En caso de moverme de a uno entonces no reescribo toda la ventana 
del calendario. Debo restaurara el atributo del dia que me voy por
lo cual llamo a RestSetAtrib.
En caso que el desplazamiento sea mayor que 1 entoces directamente 
reescribo la ventana del calendario.
*/
static void MovDiaAba(int cantidad)
{
	int colant,filant;

	colant = pcol;
	filant = pfila;

	fila  += cantidad;
	pfila += cantidad;
	fecha_act += (DATE)(7*cantidad);
	RestSetAtrib(-(7*cantidad), filant, colant, pfila, pcol);
}

/* Macro para obtener el valor absoluto */
#define Abs(a) ((a)>=0) ? (a) : (-(a))

/*
Rutina para moverme por dia(s) hacia atras
Debo restaurara el atributo del dia que me voy por lo cual llamo 
a RestSetAtrib.
*/
static void MovDiaIzq(int cantidad)
{
	int colant,filant;

	colant = pcol;
	filant = pfila;

	if (col-cantidad>=0) {
		pcol -= (3*cantidad);
		col  -= cantidad;
	}
	else {
		fila -= (1-((col-cantidad)/7));
		pfila-= (1-((col-cantidad)/7));
		col   = Abs(((7 + (col-(cantidad%7)))%7));
		pcol  = (3*col)+1;
	}
	fecha_act -= (DATE)cantidad;
	RestSetAtrib(cantidad, filant, colant, pfila, pcol);
}

/*
Rutina para moverme por dia(s) hacia adelante
Debo restaurara el atributo del dia que me voy por lo cual llamo 
a RestSetAtrib.
*/
static void MovDiaDer(int cantidad)
{
	int colant,filant;

	colant = pcol;
	filant = pfila;

	if (col+cantidad<=6) {
		pcol += 3*cantidad;
		col  += cantidad;
	}
	else {
		fila += (col+cantidad)/7;
		pfila+= (col+cantidad)/7;
		col   = Abs(((7 - (col+cantidad))%7));
		pcol  = (3*col)+1;
	}	
	fecha_act += (DATE)cantidad;
	RestSetAtrib(-cantidad, filant, colant, pfila, pcol);
}

/*
Rutina que deletea todas las ventanas activas antes de salir de pgm
*/
static void SacoCal()
{
	WiSwitchTo(wcal);
	WiDelete(WICURRENT);
	WiSwitchTo(wprev);
}

/*
Rutina que llena la ventana del calendario la fecha correspondiente
en fecha_act
*/
static void LlenarCal(void)
{
	int rest,f,c;
	short d,m,y;

	WiErase();
	DToDMY(fecha_act, &d, &m, &y);
	WiMoveTo(0,0);
	WiPrintf("\033[1m   %s %u\033[m\n", MonthName(m), y);
	WiMoveTo(1,0);
	WiPrintf("\033[1m%s\033[m\n", dayw);
	Cal(m, y, hoja);

	for(f=0; f<6; f++) { 
		if (f!=5) WiMoveTo(f+2,0);
		for (c=0; c<7; c++) {
			if (hoja[f][c]==0) WiPrintf("   ");
			else {
				SetAtrib((short) hoja[f][c], m, y, d, &rest);
				if (hoja[f][c]<10) WiPrintf(" ");
				WiPrintf("%d",hoja[f][c]);
				if (rest) {
					WiSetAttr(A_BLUE_BG);
					rest = 0;
				}
				WiPrintf(" ");
				if (hoja[f][c]==d) {
					pfila = f+2;
					pcol  = (c*3)+1;
					fila  = f;
					col   = c;
				}
			}
		}
	}
	WiMoveTo(pfila,pcol);
}

/*
Rutina que setea el atributo para imprimir un dia del mes, determinado
por la fecha fecha_act.
Si el dia es feriado se imprime en BOLD, si es el dia corriente se 
imprime en REVERSE y, si es feriado y corriente entonces va en
BOLD|REVERSE
*/
static void SetAtrib(short d, short m, short y, short today, int *rest)
{
	if (IsHolliday(DMYToD(d, m, y))) {
		WiSetAttr(A_RED|A_BOLD|(d==today ? A_REVERSE : A_BLUE_BG));
		*rest = 1;
		return;
	}

	if (d == today) {
		*rest = 1;
		WiSetAttr(A_REVERSE);
		return;
	}

	WiSetAttr(A_BLUE_BG);
}

