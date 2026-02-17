/********************************************************************
* Copyright (c) 1987 InterSoft Co.  All Rights Reserved
* THIS IS UNPUBLISHED PROPRIETARY SOURCE CODE OF InterSoft Co.
* The copyright notice above does not evidence any
* actual or intended publication of such source code.
*
*
* $Id: wmcalc.cc,v 1.1.1.1 1996/04/16 18:50:22 gustavof Exp $
*
*
* DESCRIPTION:
*      Bussines calculator Module
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
#include <wm.h>
#include <ifound/priv/gfuncs.h>

#include <signal.h>

/* ++ Prototypes ++ */

static void Init(int row, int column);
static void Exit(void);
static int do_calculator(int dec);
static void DispNum(double num, short nrodec);
static int GetEvent(void);
static void facto(double num);
static void cal_err(void);
static void DisplCmd(UChar c);
static void cal_help(void);
static long power(int a, int b);
static double Valor(char *dato);
static int Validate(UChar cmd, short *nc);
static int GetPos(int cmd);
static void Oper(int cmd);
static void Spec(int cmd);
static void Conv(int cmd);
static void Memo(int cmd);

/* -- Prototypes -- */

#ifdef NO_SERVICES

void w_calculator(void) { return; }
void calculador(void) { return; }

#else

/*+++ CONFIG +++*/

#define NROLIN 8 
#define NROCOL 17 

#define H_ROWS	21		/* Cantidad de filas del Help */
#define H_COLS	22 		/* Cantidad de columnas del Help */

#define Db	10	/* default base */  
#define DSIZE	15	/* display size */
#define Nd	12	/* numero de decimales a imprimir por default */

#define Ad	'+' /* add */
#define Di	'/' /* dividir */
#define Mu	'*' /* multiply */
#define Su	'-' /* substract */
#define Pr	'%' /* average */
#define Eq	'=' /* equal */

#define Cb	'#'	/* cambio de base */
#define Cl	'C'	/* clear display */
#define Cs	'_'	/* cambio de signo */
#define Dd	'$'	/* decimal display */
#define Dp  '.'	/* decimal point */
#define Ee	'E'	/* notacion dd.ddedd */
#define Fc	'!'	/* factorial */
#define Ma	'A'	/* memory add */
#define Mc	'L'	/* memory clear */
#define Mi	'M'	/* memory in */
#define Mr	'R'	/* memory retrieve */
#define Ms	'S'	/* memory substract */
#define Mx	'X'	/* memory exchange */
#define Do  'Q'	/* dos ceros */
#define To	'W'	/* Tres ceros */

#define MAX_HELP_LEN	30

struct data {
	int txt;						/* Character */
	FPI fun;						/* Function  */
	bool monadic;					/* Monadic operator */
	char help[MAX_HELP_LEN+1];		/* Help text */
} _far data[] = {
	{Ad, Oper,		FALSE,	""},
	{Di, Oper,		FALSE,	""},
	{Mu, Oper,		FALSE,	""},
	{Su, Oper,		FALSE,	""},
	{Pr, Oper,		FALSE,	"Porcentaje"},
	{Eq, Spec,		TRUE,	""},
	{Cl, Spec,		TRUE,	"Borrar todo"},
	{K_ENTER, Spec,	TRUE,	""},
	{K_BACKSPACE,NULL, TRUE,"Retroceder"},
	{K_CLEAR_FLD,NULL, TRUE,"Borrar n\372mero"},
	{K_HELP, NULL,	TRUE,	"Ayuda"},
	{Cb, Conv,		TRUE,	"Cambio de base"},
	{Cs, Conv,		TRUE,	"Cambio de signo"},
	{Dd, Conv,		TRUE,	"Decimales"},
	{Ee, Conv,		TRUE,	"Exponencial"},
	{Do, Conv,		TRUE,	"00"},
	{To, Conv,		TRUE,	"000"},
	{Fc, Spec,		TRUE,	"Factorial"},
	{Ma, Memo,		TRUE,	"Memoria +"},
	{Mc, Memo,		TRUE,	"Limpiar Memoria"},
	{Mi, Memo,		TRUE,	"Ingresa Memoria"},
	{Mr, Memo,		TRUE,	"Recupera Memoria"},
	{Ms, Memo,		TRUE,	"Memoria -"},
	{Mx, Memo,		TRUE,	"Intercambio memoria"}
};

/* Global static variables                                         */
static char _far *w_calc_layout[] = {
		" 7 8 9  /       \n",
	    " 4 5 6  *       \n",
	    " 1 2 3  - C M R \n",
	    " 0 . =  + M+ M- ",
	    NULL,
};

static double acc;
static window win_cal, win_help, wprev;

static char _far _display[100], last_oper;
static double memory;		/* Valor de la memoria */
static short base,
				ndecs,		/* cantidad de decimales a imprimir */
				nbuttons;	/* Number of buttons available */
static bool help;			/* flag para llamar o borrar el help */

void w_calculator(int row, int column, int dec, UChar *dato,
		short length)
{
	Init(row, column);

	/* Operate												*/
	do {
		do_calculator(dec);
		acc = Valor(_display);
		FToStr(acc * power(10, dec), (char *)dato);
		if (strlen((char *)dato) > length) cal_err();
	} while (strlen((char *)dato) > length);

	Exit();
}

#define CAL_LEN	12	/* calculator field length */

void calculador(void)
{
	static UChar last_number[CAL_LEN+1];	/* of the calculator */
	(void) w_calculator(WiLine(WiDefPar())+2, WiCol(WiDefPar()), 0,
						last_number, CAL_LEN);
}

static void Init(int row, int column)
{
	int i;

	/* Remember in which window w're						*/
	wprev = WiCurrent();

	/* Create the Window to display de calc. layout			*/
	win_cal = WiCreate (WiDefPar(), row, column, NROLIN, NROCOL-1,
						ALL_BORDER|A_REVERSE|SLINE_TYPE|A_YELLOW_BG,
						"", A_NORMAL);
	WiWrap(FALSE);

	/* Show the display										*/
	WiMoveTo(7, 0);
	for(i = 0; w_calc_layout[i]; i++)
		WiPrintf("\033[1;33;43m%-s\033[m", w_calc_layout[i]);

	WiSetScroll(0, 2, 0, WiHeight(WICURRENT)-1);
	WiSetAttr(A_REVERSE|A_YELLOW_BG);
	WiMoveTo(5,NROCOL); WiPutc(165);
	for(i = 10; i<= 14; i+=2){
		WiMoveTo(4, i);
		WiPutc(160);
	}
	for(i = 11; i <= 15 ; i +=2) {
		WiMoveTo(5,i); WiPutc(161);
	}
	WiMoveTo(5, 10); WiPutc(162);
	WiMoveTo(5, 12); WiPutc(163);
	WiMoveTo(5, 14); WiPutc(163);

	WiMoveTo(3, NROCOL); WiPutc(165);
	WiMoveTo(3, -1); WiPutc(167);
	for(i = 0; i < 16; i++){
		WiMoveTo(3, i); WiPutc(161);
	}
	for(i = 10; i <= 14; i +=2){
		WiMoveTo(3, i); WiPutc(169);
	}
	WiSetAttr(A_NORMAL);
	WiWrap(TRUE);

	nbuttons = sizeof(data) / sizeof(data[0]);

	/* Inicializo HELP con el lenguaje corriente */
	for (i=0 ; i<nbuttons ; i++) {
		char *msg = _WmErrmsg(String("CALC_HLP") + toString(i));
		if (msg != NULL) {
			strncpy(data[i].help, msg, MAX_HELP_LEN);
			data[i].help[MAX_HELP_LEN] = '\0';
		}
	}

	memory = acc = 0.0;
	strcpy(_display, "0"); 
	last_oper = 0;
	base = Db;
	help = FALSE;
}

static void Exit(void)
{
	/* Disapear !											*/
	if (help)  {
		WiSwitchTo(win_help);
		WiDelete(WICURRENT);
	}
	WiSwitchTo(win_cal);
	WiDelete(WICURRENT);

	WiSwitchTo(wprev);
}

static int do_calculator(int dec)
{
	int pos, cmd;

	dec += 4;
	ndecs = (short) ((dec <= 0 || dec > Nd) ? Nd : dec);
    
#ifdef HAVE_FULL_SIGNALS    
	SigSet(SIGFPE, (FPI) cal_err);
#endif

	WiCursor(FALSE);
	WiRefresh();
	WiMoveTo(2, 0);
	(void) WiPrintf("%*.*g\b", DSIZE, DSIZE-5, 0.0);
	DisplCmd(Cb); /* imprime la base en la que se trabajara */

	acc = (*_display) ? Valor(_display) : 0.0;

	while ((cmd = GetEvent()) != K_END) {
		if ( (pos = GetPos(cmd)) == ERROR) {
			w_beep();
			continue;
		}

		/* Resolver la cuenta pendiente */
		if (last_oper && cmd!=Mr) {
			(*data[GetPos((int)last_oper)].fun)((int) last_oper);
			DispNum(acc, ndecs);
		}
		else
			acc = Valor(_display);

		/* Si es operador monadico resolver ahora */
		if (data[pos].monadic) {
			(*data[pos].fun)(cmd);
			if (cmd!=Mr) last_oper = 0;
		}
		else
			last_oper = (char)cmd;

		DispNum(acc, ndecs);
	}
	return ndecs - 4;
}

static int GetPos(int cmd)
{
	int i;
	for (i=0 ; i<nbuttons ; i++)
		if (data[i].txt == cmd) return i;

	return ERROR;
}

/* operadores diadiacos */
static void Oper(int cmd)
{
	switch(cmd) {
	case Ad:	/* add */
		acc += Valor(_display);
		break;
	case Su:	/* substract */
		acc -= Valor(_display);
		break;
	case Mu:	/* multiplicar */
		acc *= Valor(_display);
		break;
	case Di:	/* dividir */
		if (Valor(_display))
			acc /= Valor(_display);
		else {
			acc = 0.0;
			Warning(_WmErrmsg("DIV_BY_ZERO"));
		}
		break;
	case Pr:/* porcentaje */
		acc *= Valor(_display);
		acc /= 100; 
		break;
	default:
		Error("-------> %c  No hallado en Oper()", cmd);
	}
}

/* Operaciones con memoria */
static void Memo(int cmd)
{
	double aux_mem;
	switch(cmd) {
	case Mi:	/* memory in */
		memory = acc;
		break;
	case Mr:	/* memory retrieve */
		acc = memory;
		break;
	case Mx:	/* memory exchange */
		aux_mem = memory;
		memory = acc;
		acc = aux_mem;
		break;
	case Mc:	/* memory clear */
		memory = 0.0;
		break;
	case Ma:	/* memory add */
		memory += acc;
		break;
	case Ms:	/* memory substract */
		memory -= acc;
		break;
	default:
		Error("-------> %c  No hallado en Memo()", cmd);
	}
	WiMoveTo(4, 13);
	WiSetAttr(A_YELLOW_BG | A_GREEN | A_BOLD);
	WiPutc((UChar) (memory!=0.0 ? 'M' : ' '));
	WiSetAttr(A_NORMAL);
	WiRefresh();
}

static void Conv(int cmd)
{
	switch (cmd) {
	case Cl:	/* clear */
		acc = 0.0; 
		break;
	case Cb: /* cambio de base */
		if (base) {
			base += (short) ((base == 8) ? 2: 6);
			if (base > 16) base = 2;
		}
		DisplCmd((UChar)cmd);
		break;
	case Ee:	/* notacion exponencial */
		base = (short) (base ? 0: 10);
		DisplCmd((UChar)cmd);
		break;
	case Cs:	/* cambio de signo */
		acc = -acc;
		break;
	case Dd:	/* resta 1 a la cantidad de decimales */
		{
			double faux;
			if ((faux = Valor(_display)) > (double)Nd) break;
			ndecs = (short) (faux - 1);
			if (ndecs < 0 || ndecs > Nd) ndecs = Nd;
			break;
		}
	default:
		Error("-------> %c  No hallado en Conv()", cmd);
	}
}

static void Spec(int cmd)
{
	switch(cmd) {
	case Fc:	/* factorial */
		facto(Valor(_display));
		break;
	case K_ENTER: /* igual con scroll */
		WiMoveTo(2, 0);
		DispNum(acc, ndecs);
		(void) WiPrintf("\n");
		DispNum(acc, ndecs);
		break;
	case Cl:
		_display[0] = '\0';
		acc=0.0;
		break;
	case Eq:	/* igual */
		/* No hago nada */
		break;
	default:
		Error("-------> %c  No hallado en Spec()", cmd);
	}
}

static void DispNum(double num, short nrodec)
{
	WiMoveTo(2, 0);

	switch(base) {
	case 0: /* notacion exponencial */
		sprintf(_display, "%*.*e", DSIZE, DSIZE-5, num);
		break;
	case 2:
		if (num < (double)0xffff)
			sprintf(_display, "%*b", DSIZE, (long) num);
		else
			cal_err();
		break;
	case 8:
		sprintf(_display, "%*o", DSIZE, (long) num);
		break;
	case 10:
		{
			int l; char aux[100], *p;

			sprintf(aux, "%*.*g", DSIZE, DSIZE-5, num);
			if ( (l = strlen(aux)) > DSIZE) {
				p = (char *) strchr(aux, 'e');
				if (p) strcpy(p - (l- DSIZE), p);
			}
			sprintf(_display, "%s", aux);
			break;
		}
	case 16:
		sprintf(_display, "%*x", DSIZE, (long) num);
	}
	WiPrintf("%*s\b", DSIZE, _display);
	WiMoveTo(2, 0);
}

static int GetEvent(void)
{
	register UChar c;
	short nc, pos_pto_dec = 0;
	bool e_exp	 = TRUE,
		 pto_dec = TRUE;

	WiSetAttr(A_NORMAL);
	nc = 0; /* number count */

	while(((c = WiGetc()) == K_BACKSPACE || !iskey(c) || c == K_HELP
			|| c == K_CLEAR_FLD) && c) {

		if (Validate(c, &nc)) continue;

		if (c == K_BACKSPACE) {
			if (!nc) continue;
			--nc;
			if(_display[nc] == Dp) pto_dec = 1;
			if(_display[nc] == 'e') e_exp = 1;
		} 
		else if (nc < DSIZE) {
			if ((isdigit(c) && (short)(c - '0') < base) ||
					(nc == 0 && c == '-') ||
					(c >= 'a' && c <= 'f' && base == 16) ||
					(c + base == c && isdigit(c)))
				_display[nc++] = c;
			else if (c == 'e') {
				if ( !e_exp || base != 10) continue;
				_display[nc++] = c;
				e_exp = FALSE;
			}
			else if (c == Dp) {
				if ( ! pto_dec || base != 10) continue;
				if (!nc) _display[nc++] = '0';
				pos_pto_dec = nc;
				_display[nc++] = Dp;
				pto_dec = FALSE;
			}
			else if( c == Do ){
				strcat(_display,"00");
				nc += 2;
			}
			else if( c == To ){
				strcat(_display, "000");
				nc += 3;
			}
			else if(isdigit(c)) continue;
			else {
				DisplCmd(c);
				return c;
			}
		}
		else return c;
		_display[nc] = '\0';
		WiMoveTo(2, 0);
		if(!pto_dec) _display[pos_pto_dec] = DECPOINT();
		WiPrintf("%*s\b", DSIZE, _display);
		if(!pto_dec) _display[pos_pto_dec] = Dp;
	}
	WiMoveTo(2, 0);
	return c ? c : K_END;
}

static int Validate(UChar cmd, short *nc)
{
	if (cmd == K_BACKSPACE) return 0;
	if (cmd == K_CLEAR_FLD) {
		*nc=0;
		_display[0] = '\0';
		WiMoveTo(2, 0);
		(void) WiPrintf("%*s\b", DSIZE, " ");
		WiMoveTo(2, 0);
		WiRefresh();
		return 1;
	}
	if (cmd == K_HELP){
		if ((help = !help))
			cal_help();
		else {
			WiSwitchTo(win_help);
			WiDelete(WICURRENT);
		}
		return 1;
	}
	if (cmd >= 'a' && cmd <= 'f' && base != 16 && cmd != 'e') {
		w_beep();
		return 1;
	}
	if (cmd == 'e' && (base == 8 || base == 2)){
		w_beep();
		return 1;
	}
	if (cmd == '0' && *nc == 1 && _display[0] == '0')
		return 1;

	return 0;
}

static void facto(double num)
{	
	for(acc = 1.0; acc != 0.0 && num != 0.0; num--)
		acc *= num;
}

static double Valor(char *dato)
{
	char chr;
	double num = 0.0;

	if (*dato == 0) return 0.0;
	if (base==10 || base==0) return StrToF(dato);

	while ( (chr = *dato++) )
		if (chr != ' ')
			num = num * base +  (chr < 'a' ? chr-'0' : chr-'a' + 10);

	return num;
}

static long power(int a, int b)
{
	long res = 1;

	while (b--) res *= a;

	return res;
}

static void cal_err(void)
{   
#ifdef HAVE_FULL_SIGNALS
	SigSet(SIGFPE, (FPI) cal_err);
#endif

	WiCursor(FALSE);
	WiMoveTo(4, 15);
	WiSetAttr(A_YELLOW_BG | A_BLINK|A_BOLD|A_GREEN);
	WiPutc('E');
	WiRefresh();

	WiGetc();

	WiMoveTo(4, 15);
	WiSetAttr(A_YELLOW_BG | A_BOLD|A_GREEN);
	WiPutc(Cl);
	WiSetAttr(A_NORMAL);
	WiRefresh();

	WiMoveTo(2,0);
	WiCursor(FALSE);
	WiRefresh();

	_display[0] = '\0';
	acc = 0.0;
	base = 10;
	DispNum(acc, 2);
}

static void DisplCmd(UChar c)
{
	int col;

	WiCursor(FALSE);
	if( c == Cb || c == Ee){
		switch(base){
		case 0:
			c = 'E';
			break;
		case 2:
			c = 'B';
			break;
		case 8:
			c = 'O';
			break;
		case 10:
			c = 'D';
			break;
		case 16:
			c = 'H';
		}
		col = 11;
		WiSetAttr(A_YELLOW_BG | A_BOLD|A_GREEN);
		WiMoveTo(4,col);
		WiPutc(c);
		WiSetAttr(A_NORMAL);
		c = Cb;
	}
	col = c >= '0' && c <= '9' ? 13 : 15;
	WiMoveTo(4,col);
	WiSetAttr(A_YELLOW_BG | A_BOLD|A_GREEN);
	WiPutc(c);
	WiSetAttr(A_NORMAL);
	WiRefresh();

	WiMoveTo(2,0);
	WiCursor(FALSE);
	WiRefresh();
}

static void cal_help(void)
{
	short x, y, i;

	WiMoveTo(2,0);
	x = (short)WiLine(WiDefPar());
	y = (short)WiCol(WiDefPar());

	y -= (short) ((y < 21) ? -18: 22);
	/* y - 22 */
	win_help = WiCreate (WiDefPar(), x - 2, y, H_ROWS, H_COLS, 
						 ALL_BORDER|A_REVERSE|SLINE_TYPE,
						 "HELP", A_NORMAL);

	WiMoveTo(-1, 0);

	for (i=0 ; i<nbuttons ; i++)
		if (data[i].help[0]) {
				UChar xx[50];
				WiKeyHelp((UChar)data[i].txt, xx);
				WiPrintf("\n %s %s\b", xx, data[i].help);
		}
	WiRefresh();
	WiSwitchTo(win_cal);
}
#endif /* NO_SERVICES */
