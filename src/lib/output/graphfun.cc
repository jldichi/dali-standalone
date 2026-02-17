/********************************************************************
* Copyright (c) 1987 InterSoft (IdeaSoft) Co.  All Rights Reserved
* THIS IS UNPUBLISHED PROPRIETARY SOURCE CODE OF InterSoft Co.
* The copyright notice above does not evidence any
* actual or intended publication of such source code.
*
* $Id: graphfun.cc,v 1.3 2004/09/29 17:19:58 damianm Exp $
*
* DESCRIPTION
* Functions to do graphic output 
*********************************************************************/

#include <math.h>
#include <string.h>
#include <stdlib.h>

#include <ideafix.h>
#include <cfix.h>
#include <stdio.h>
#include <ideafix/rpgraph.h>

#ifndef HUGE
#define HUGE MAX_DOUBLE
#endif

#define Min(a,b) ((a) < (b) ? (a) : (b))

#define DeleteS(Str)	Free(Str)
#define DeleteN(Ptr,N)	Free(Ptr)

#define NewN(N)			(char *) Alloc(N)
#define NewV(type,N)	(type *) Alloc(sizeof(type)*N)

#define FToL(ff)		((long) (float) ff)

typedef unsigned char *Display;

/*********************************************************************/
/* Variables Globales Estaticas:									*/
/*********************************************************************/

static int XTics, YTics, EjeX, Grilla, TipoGrafica = BARRAS;
static int cantXserie, cantDseries, cantA;
static int PantX, PantY, ImpX, ImpY, Usadas[MAX_SER], UsadoX = 0, MaxSer[MAX_SER];
static int PageX, PageY;
static int MarTit, MarST, HayDatos = 0, entrada, EtiquetoBarra;
static int GrillaComX, GrillaStepX;
static double YMin, YMax, YStep, *A, MemoriaAlloc = 0;
static double TipoEscalaY = GR_AUTO, LimiteInfY, LimiteSupY;
static char *Titulo = NULL, *Subtitulo = NULL , *LabelX = NULL, *LabelY = NULL;
static char  Patt[20];
static char Marca[MAX_SER], StrBuff[2];
static window wigraf;
static Display Screen = NULL, Im = NULL;
static char *L[MAX_SER] = {
	NULL, NULL, NULL, NULL, NULL, NULL
};

static char *X[MAX_VAL] =  { 
	NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL,
	NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL };

/* 
	Por ahora hay una limitacin de la cantidad de valores a graficar
	despus debo encontrar la cantidad correcta para pedir memoria 
*/

/* ++ Prototypes ++ */
static int ANumero(int c);
static int ACaracter(int n);
static int Suma(int c1, int c2);
static void Escribir(Display Sc, int pos, int car);
static void barra(int x0, int y0, int x1, int y1, char pat, Display Sc, int DX);
static void pilas(int x0, int y0, int x1, int Y[6], int cant, Display Sc, int DX);
static void pizza(int PZ[], int DY, int DX, Display Sc, int YOffset,int ComX);
static void ejes(int DX, int DY, Display Sc, int manual);
static void MaxMinArrayA(double *MX, double *MM);
static void MaxMinArray(double A[], double *MX, double *MM, int tam);
static double RedonUp(double X);
static double RedonDown(double X, double step);
static int RedonInt(double X);
static void RedonDoble(int DY);
static void CalculoLimites(void);
static int DatosPositivos(void);
static void ProcesoEntrada(int *Sector, int MaxSector);
static int GraficoBarras(Display Sc, int DX, int DY, int quien);
static int GraficoPilas(Display Sc, int DX, int DY, int quien);
static int GraficoPizza(Display Sc, int DX, int DY, int quien);
static int GraficoXY(Display Sc, int DX, int DY, int quien);
static void TituloGraf(Display Sc, int DX, int DY);
static void EtiquetoGraf(Display Sc, int Offset, int DX, int Y);
static void ImprimoLeyendas(Display Sc, int SerUsadas, int Desfasaje);
static void ImprimoFlechas(Display Sc, int Sector, int MaxSector);
static void DisplayPantalla(int tecla);
static void ResetZonaGrafica(Display Sc);

/* -- Prototypes -- */

#define Getc()	WiGetc()

#define LkAt(attr)	WiSetAttr(attr)

#define PutOnTop(dum1, dum2)	1

#define CloseWin(win)	WiDelete(win)

#define RefreshWin(win, dum)	WiRefresh()

#define At_BLINK	A_BLINK
#if IDEAWIN
#	define At_REVERSE	(A_RED_BG )
#	define At_NORMAL	(A_WHITE_BG | LT_BKCOLOR)
#	define WIN_BORDER (NO_BORDER | B_CHART_WND_CLASS)
#else
#	define At_REVERSE	A_REVERSE
#	define At_NORMAL	A_NORMAL
#	define WIN_BORDER NO_BORDER
#endif

#define	OpenWin(FilOrigen, ColOrigen, Largo, Ancho) WiCreate(WiDefPar(), FilOrigen, ColOrigen, Largo,  Ancho+1, WIN_BORDER, "", A_NORMAL)

#define P_CRSR_LT	K_CURS_LEFT
#define P_CRSR_RG	K_CURS_RIGHT

#define MaxColMntr	WiWidth(SCREEN)
#define MaxFilMntr	WiHeight(SCREEN)

extern"C" void LkMvAdd(int Fil, int Col, char *Str)
{
	WiMoveTo(Fil, Col);
	WiPuts(Str);
}


extern "C" void LkAtMvAdd(attr_type At, int Fil, int Col, char *Str)
{
	attr_type	AtAnt = WiGetAttr();

	LkAt(At);
	LkMvAdd( Fil, Col, Str);
	LkAt(AtAnt);
}


/*********************************************************************/
/*		Funcion Suma : Hace la suma de dos caracteres graficos. 	 */
/*********************************************************************/


static int ANumero(int c)
{
	if (c == ' ')
		return(0);
	else if (c == GLK_V)
		return(5);
	else if (c == GLK_H)
		return(10);
	else if (c == GLK_VII)
		return(3);
	else if (c == GLK_UP)
		return(11);
	else if (c == GLK_VID)
		return(9);
	else if (c == GLK_RG)
		return(13);
	else if (c == GLK_CEN)
		return(15);
	else if (c == GLK_LT)
		return(7);
	else if (c == GLK_VSI)
		return(6);
	else if (c == GLK_DW)
		return(14);
	else if (c == GLK_VSD)
		return(12);
	else
		return(-1);
}

static int ACaracter(int n)
{
	switch (n) {
	case 0 :   
		return(' ');
	case 3 :	
		return(GLK_VII);
	case 5 :	
		return(GLK_V);
	case 6 :	
		return(GLK_VSI);
	case 7 :	
		return(GLK_LT);
	case 9 :	
		return(GLK_VID);
	case 10 :	
		return(GLK_H);
	case 11:	
		return(GLK_UP);
	case 12:	
		return(GLK_VSD);
	case 13:	
		return(GLK_RG);
	case 14:	
		return(GLK_DW);
	case 15:	
		return(GLK_CEN);
	}
	return(n);
}

static int Suma(int c1, int c2)
{
	int	n1, n2;

	if ( ((n1 = ANumero(c1)) != -1)  &&  ((n2 = ANumero(c2)) != -1) )
		return(ACaracter(n1 | n2));	/* OR Lgico entre enteros */
	else
		return(c2);
}

static void Escribir(Display Sc, int pos, int car)
{
	if (Sc[pos] != ' ') {
		if (Sc[pos] != car)
			Sc[pos] =  (char) Suma(Sc[pos], car);
	} else
		Sc[pos] = (char) car;
}

/*********************************************************************/
/*		Funcion barra : Genera una barra							 */
/*********************************************************************/

static void barra(int x0, int y0, int x1, int y1, char pat, Display Sc, int DX)
{
	int	i, j;
	if (y0 != y1) {
		for (i = x0 + 1; i < x1; i++) {
			for (j = y0; j < y1; j++)
				Escribir(Sc, j * DX + i, pat);
		}

		for (i = x0 + 1; i < x1; i++) {
			if (Sc[y0*DX+i] == GLK_V)
				Sc[y0*DX+i] = GLK_DW;
			else
				Sc[y0*DX+i] = GLK_H;

			if (Sc[y1*DX+i] == GLK_V )
				Sc[y1*DX+i] = GLK_UP;
			else
				Sc[y1*DX+i] = GLK_H;
		}

		for (j = y0 + 1; j < y1; j++) {
			Sc[j*DX+x0] = GLK_V;
			Sc[j*DX+x1] = GLK_V;
		}

		if (x0 != (x1 + 1)) {
			Escribir(Sc, y0 * DX + x0, GLK_VII);
			Escribir(Sc, y0 * DX + x1, GLK_VID);
		}

		if (y0 != (y1 + 1)) {
			Escribir(Sc, y1 * DX + x0, GLK_VSI);
			Escribir(Sc, y1 * DX + x1, GLK_VSD);
		}
	}
}

/***************************************************************************/
/*		Funcion pilas : Genera barras apiladas (Mximo 6(MAX_SER) pisos).  */
/***************************************************************************/

static void pilas(int x0, int y0, int x1, int Y[MAX_SER], int cant, Display Sc, int DX)
{
	int	MaxY, piso, i, j, jAnt;

	MaxY = Y[0];
	for (i = 0; i < cant; i++)
		if (Y[i] > MaxY)
			MaxY = Y[i];


	jAnt = EjeX;

	for (piso = 0; piso < cant; piso++) {
		for (j = jAnt; j < Y[piso]; j++) {
			for (i = x0; i <= x1; i++)
				Sc[j*DX+i] = Patt[piso];
		}
		jAnt = Y[piso];
	}

	for (i = x0 + 1; i < x1; i++) {
		Escribir(Sc, y0 * DX + i, GLK_H);	/* Piso */

		for (piso = 0; piso < cant; piso++)
			Sc[Y[piso]*DX+i] = GLK_H;
	}


	for (j = y0 + 1; j < MaxY; j++) {
		Escribir(Sc, j * DX + x0, GLK_V);
		Escribir(Sc, j * DX + x1, GLK_V);
	}

	for (piso = 0; piso < cant; piso++) {
		Escribir(Sc, Y[piso] * DX + x0, GLK_LT);
		Escribir(Sc, Y[piso] * DX + x1, GLK_RG);
	}

	if (x0 != (x1 + 1)) {
		Escribir(Sc, y0 * DX + x0, GLK_VII);
		Escribir(Sc, y0 * DX + x1, GLK_VID);
	}

	if (y0 != (MaxY + 1)) {
		Sc[MaxY*DX+x0] = GLK_VSI;
		Sc[MaxY*DX+x1] = GLK_VSD;
	}
}

/*********************************************************************/
/*		Funcion pizza : Genera barras 								 */
/*********************************************************************/


static void pizza(int PZ[], int DY, int DX, Display Sc, int YOffset,int ComX)
{
	int	i, Inf, Sup;

	Inf =  DY / 2 - YOffset;
	Sup =  DY - 4 - YOffset;


	if (ComX < 0) ComX = 0;
	for (i = 0; i <= MaxSer[0]; i++) {
		barra(ComX, Inf,ComX +  PZ[i], Sup, Patt[i], Sc, DX);
		ComX += PZ[i];
	}
}

/*********************************************************************/
/*		Funcion ejes: Genera los ejes con los Ticks adecuados		 */
/*********************************************************************/

static void ejes(int DX, int DY, Display Sc, int manual)
{
	int	i, j, XMn, XMx, YMn, YMx, temp, Largo, Sep = 0, Y;
	char	Buff[20], OF[10];
	double	MenorY, AuxStep;

	if (YStep == 0.0)
		return;

	XMn = DX / 10;	/* X Mnimo para la zona de grfica */
	XMx = DX;		/* X Mximo */
	YMn = DY / 12;
	YMx = DY - YMn * 2;

	if (manual == GR_MANUAL)
		MenorY = YMin;
	else
		MenorY = 0.0;

	AuxStep = YStep;

	/* porque hay problemas en xenix386 */

	if (fabs(MenorY + DX * 3 / 4 * YStep) > 10000000) {
		MenorY /= 1000000;
		AuxStep = YStep / 1000000;
		strcpy(OF, "Millones");
		Sep = 2;
	} else if (fabs(MenorY + DX * 3 / 4 * YStep) > 10000) {
		MenorY /= 1000;
		AuxStep = YStep / 1000;
		strcpy(OF, "Miles");
		Sep = 2;
	}


	for (i = XMn; i < XMx; i++)
		((i % XTics) == 0) ? Escribir(Sc, EjeX * DX + i, GLK_DW)  : Escribir(Sc, EjeX * DX + i, GLK_H) ;

	for (i = EjeX; i <= YMx; i++)	/* Numero la parte superior del eje Y */ {
		if (((i - EjeX) % YTics) == 0) {
			Escribir(Sc, i * DX + XMn - 1, GLK_LT);
			sprintf(Buff, "%g", (i - EjeX) * AuxStep + MenorY);
			Largo = strlen(Buff);
			if (Largo > XMn - 1 - Sep)
				Largo = XMn - 1 - Sep;
			strncpy((char *)(Sc + i * DX + Sep), Buff, Largo);

			if ( (Grilla == GR_HORIZ) || (Grilla == GR_AMBAS) )
				for (j = XMn; j < DX; j++)
					Escribir(Sc, i * DX + j, GLK_H);
		} else
			Escribir(Sc, i * DX + XMn - 1, GLK_V);
	}

	for (i = EjeX; i >= YMn; i--)	/* Numero la parte inferior del eje Y */ {
		if (((i - EjeX) % YTics) == 0) {
			Escribir(Sc, i * DX + XMn - 1, GLK_LT);
			sprintf(Buff, "%g", (i - EjeX) * AuxStep + MenorY);
			Largo = strlen(Buff);
			if (Largo > XMn - 1 - Sep)
				Largo = XMn - 1 - Sep;
			strncpy((char *)(Sc + i * DX + Sep), Buff, Largo);

			if ( (Grilla == GR_HORIZ) || (Grilla == GR_AMBAS) )
				for (j = XMn; j < DX; j++)
					Escribir(Sc, i * DX + j, GLK_H);
		} else
			Escribir(Sc, i * DX + XMn - 1, GLK_V);
	}


	if (Sep) {
		Y = DY * 3 / 5;
		i = 0;
		while (OF[i] != '\0') {
			Sc[Y*DX] = OF[i];
			i++;
			Y--;
		}
	}

	if ((Grilla == GR_AMBAS) || (Grilla == GR_VERT)) /* Ya dibuj la horizontal */ {

		for (i = 0; GrillaComX + i * GrillaStepX < DX; i++)/* Ahora dibujo la vertical */ {
			temp = GrillaComX + i * GrillaStepX;
			for (j = YMn + 1; j < YMx; j++)
				Escribir(Sc, temp + DX * j, GLK_V);
		}
	}

	if (EjeX == YMn)
		Sc[YMn*DX+XMn-1] = GLK_VII;
	else if (EjeX == YMx)
		Sc[YMx*DX+XMn-1] = GLK_VSI;
}

/*********************************************************************/
/*	Funcion MaxMinArrayA: Ubica el mximo y el mnimo del Array A.	 */
/*********************************************************************/

static void MaxMinArrayA(double *MX, double *MM)
{
	int	i, j;

	*MX = -HUGE;
	*MM = HUGE;

	for (i = 0; i < MAX_SER; i++) {
		for (j = 0; j < MAX_VAL; j++) {
			if (A[MAX_VAL*i+j] > *MX)
				*MX = A[MAX_VAL*i+j];

			if (A[MAX_VAL*i+j] < *MM)
				*MM = A[MAX_VAL*i+j];
		}
	}
}

static void MaxMinArray(double array[], double *MX, double *MM, int tam)
{
	int	i;

	*MX = -HUGE;
	*MM = HUGE;

	for (i = 0; i < tam; i++) {
		if (array[i] > *MX)
			*MX = array[i];

		if (array[i] < *MM)
			*MM = array[i];
	}
}

/*************************************************************************/
/*		Funcion RedonUp: Redondea un nmero para arriba en la 2da. c.sig */
/*************************************************************************/

static double RedonUp(double x)
{
	double	PrimerCS, Resto, Aprox;		/* Primera Cifra Significativa */
	int	i;


	if (x <= 0.0)
		return(0.0);
	else if (x >= 1.0) {
		for (i = 0; x > pow(10.0, (double) i); i++)
			;
		Aprox = x / pow(10.0, (double) (i - 1));
		PrimerCS = floor(Aprox);
		if (Aprox < 2.5) {
			Resto =  x - (PrimerCS * pow(10.0, (double) (i - 1)));
			if (Resto > 0.5 * pow(10.0, (double)(i - 1)))
				return((PrimerCS + 1) * pow(10.0, (double) (i - 1)));
			else if (Resto == 0.5 * pow(10.0, (double)(i - 1)))
				return(x);
			else
				return(PrimerCS * pow(10.0, (double) (i - 1)) + 5.0 * pow(10.0, (double) (i - 2)));
		} else if (Aprox == 2.5)
			return(2.5);
		else if (Aprox <= 5.0)
			return(5.0 * pow(10.0, (double) (i - 1)));
		else
			return(10.0 * pow(10.0, (double) (i - 1)));
	} else	if (x < 1 / HUGE)
		return(1 / HUGE);
	else {
		for (i = 0; x * pow(10.0, (double) i) < 1.0; i++)
			;
		i = -i;
		Aprox = x * pow(10.0, (double) (-i));
		PrimerCS = floor(Aprox);
		if (Aprox < 2.5) {
			Resto =  x - (PrimerCS * pow(10.0, (double) i));
			if (Resto > 0.5 * pow(10.0, (double) i ))
				return((PrimerCS + 1) * pow(10.0, (double) i));
			else
				return(PrimerCS * pow(10.0, (double) i) + 5.0 * pow(10.0, (double) (i - 1)));
		} else  if (Aprox < 5.0)
			return(5.0 * pow(10.0, (double) i));
		else
			return(10.0 * pow(10.0, (double) i));
	}
}

/*************************************************************************/
/*		Funcion RedonDown: Redondea un nmero para abajo				 */
/*************************************************************************/

static double RedonDown(double x, double step)
{
	double	Aprox;
	int	i;


	if (x <= 0.0)
		return(0.0);
	else {
		if (step >= 1.0) {
			for (i = 0; step > pow(10.0, (double) i); i++)
				;
			Aprox = x / pow(10.0, (double) i);
			return(floor(Aprox) * pow(10.0, (double) i));
		} else {
			for (i = 0; step * pow(10.0, (double) i) < 1; i++)
				;
			Aprox = x * pow(10.0, (double) i);
			return(floor(Aprox) * pow(10.0, (double) (-i)));
		}
	}
}



static int RedonInt(double x)
{
	if (x > 0) {
		if (x - floor(x) > 0.5)
			return((int) ceil(x));
		else
			return((int) floor(x));
	} else {
		if (ceil(x) - x > 0.5)
			return((int) floor(x));
		else
			return((int) ceil(x));
	}
}

/*************************************************************************/
/*	Procedimiento RedonDoble: Determina el YStep y el Mximo y Mnimo 	 */
/*  de una 	grfica dados dos valores uno positivo y otro negativo.		 */
/*************************************************************************/

static void RedonDoble(int DY)
{
	int	LinPos, LinNeg;
	double	PosStep, NegStep;


	LinPos = (int) floor(DY * 3.0 * YMax / (4.0 * (YMax - YMin)));
	LinNeg = (int) floor(DY * 3.0 / 4.0) - LinPos;
	EjeX  =  LinNeg + DY / 12;


	/*	WiMsg("LinPos=%d, LinNeg=%d, EjeX=%d", LinPos, LinNeg, EjeX);	 */


	if (LinPos != 0)
		PosStep = RedonUp(YMax / LinPos);
	else
		PosStep = 0.0;


	if (LinNeg != 0)
		NegStep = RedonUp(-YMin / LinNeg);
	else
		NegStep = 0.0;

	/*	WiMsg("PosStep=%f, NegStep=%f", PosStep, NegStep);	 */

	YStep = (PosStep > NegStep) ? PosStep : NegStep;
}

/*************************************************************************/
/*		Procedimiento CalculoLimites: Setear cantXserie, cantDseries 	 */
/*************************************************************************/

static void CalculoLimites(void)
{
	int	i;


	i = 5;
	while ((i >= 0) &&  (Usadas[i] == 0))		
		i--;
	cantDseries = i + 1;


	cantXserie = 0;
	for (i = 0; i < cantDseries; i++)
		if (MaxSer[i] > cantXserie)
			cantXserie = MaxSer[i];

	cantXserie++;

	cantA = cantXserie * cantDseries;
}

static int DatosPositivos(void)
{
	int	i, j;

	for (i = 0; i < MAX_VAL; i++)
		for (j = 0; j < MAX_SER; j++)
			if (A[MAX_VAL*j+i] < 0.0)
				return(0);
	return(1);
}

/*************************************************************************/
/*	Funcion ProcesoEntrada:										 		*/
/*************************************************************************/

static void ProcesoEntrada(int *Sector, int MaxSector)
{

	if (entrada == P_CRSR_LT) {
		if (*Sector > 1)
			*Sector = (*Sector - 1);
		else
			WiBeep();
	} else {
		if (entrada == P_CRSR_RG) {
			if (*Sector < MaxSector)
				*Sector = (*Sector + 1);
			else
				WiBeep();
		}
	}
}


/*************************************************************************/
/*	Funcion para inicializar Datos, Pantalla, Impresora					 */
/*************************************************************************/

extern "C" void InitDatos(void)
{
	int	i, j;

	HayDatos = 0;

	if (MemoriaAlloc == 0) {
		A = NewV(double, MAX_SER * MAX_VAL);
		MemoriaAlloc = 1;
	}

	for (i = 0; i < MAX_SER; i++) {
		Usadas[i] = 0;
		MaxSer[i] = 0;
		for (j = 0; j < MAX_VAL; j++)
			A[MAX_VAL*i+j] = 0.0;
	}
}

extern "C" void InitScreen(int quien)
{
	int	i, j;

	if (quien == GR_PANT) {
		for (i = 0; i < PantY; i++)
			for (j = 0; j < PantX; j++)
				Screen[i*PantX+j] = ' ';
	} else {
		for (i = 0; i < ImpY; i++)
			for (j = 0; j < ImpX; j++)
				Im[i*ImpX+j] = ' ';
	}
}

extern "C" void InitOpciones(void)
{
	int	i;

	if (Titulo)
		DeleteS(Titulo);
	Titulo = NULL;

	if (Subtitulo)
		DeleteS(Subtitulo);
	Subtitulo = NULL;

	if (LabelX)
		DeleteS(LabelX);
	LabelX = NULL;

	if (LabelY)
		DeleteS(LabelY);
	LabelY = NULL;

	MarTit = GR_CEN;
	MarST = GR_CEN;

	for (i = 0; i < MAX_VAL ; i++) {
		if (X[i])
			DeleteS(X[i]);
		X[i] = NULL;
	}

	for (i = 0; i < MAX_SER ; i++) {
		if (L[i])
			DeleteS(L[i]);
		L[i] = NULL;
	}

	XTics = 1000;
	YTics = 2;
	EjeX = 2;
	LimiteSupY = 100;
	LimiteInfY = 0;
	TipoEscalaY = GR_AUTO;
	UsadoX = 0;


	Patt[0] = '~';
	Patt[1] = '*';
	Patt[2] = '>';
	Patt[3] = '<';
	Patt[4] = '%';
	Patt[5] = '+';
	Patt[6] = 'X';
	Patt[7] = '#';
	Patt[8] = '-';
	Patt[9] = '@';
	Patt[10] = '/';
	Patt[11] = '\\';
	Patt[12] = ']';
	Patt[13] = '[';
	Patt[14] = GLK_CEN;
	Patt[15] = '&';
	Patt[16] = ':';
	Patt[16] = '$';
	Patt[17] = '^';
	Patt[18] = '?';
	Patt[19] = '=';

	for (i = 0; i < MAX_SER; i++)
		Marca[i] = Patt[1+i];
}

extern "C" void ResetSerie(int Indice)
{
	int	i;

	Usadas[Indice] = 0;
	for (i = 0; i <= MaxSer[Indice]; i++)
		A[MAX_VAL*Indice+i] = 0.0;

	HayDatos = 0;
	i = 0;
	while ( (i < MAX_SER) && (HayDatos == 0) ) {
		if (Usadas[i] == 1)
			HayDatos = 1;
		i++;
	}

	MaxSer[Indice] = 0;
}

extern "C" void ResetRangoX(void)
{
	int	i;

	for (i = 0; i < MAX_VAL; i++)
		if (X[i] ) {
			DeleteS(X[i]);
			X[i] = NULL;
		}

	UsadoX = 0;
}

/*************************************************************************/
/*	Funcion GraficoBarras: Recibe un array de datos calcula todo y grafica */
/*************************************************************************/

static int GraficoBarras(Display Sc, int DX, int DY, int quien)
{
	int	i, j, k, s, Ancho, Y1, XOffset, XLey, Largo, signo = 0;
	int	Ssalt, SerUsadas, aux, Gap, OffGap, YLabel, AlturaY;
	int	Sector = 1, SectorAnterior, MaxSector = 1, grupXpant = 0;
	int	IndiceJ, Dibujo;
	int	temp;  /*  Auxliliar para acelerar el clculo */
	double	Altura;
	char	Buff[20];



	CalculoLimites();

	XOffset = DX / 8;
	XLey = DX - XOffset;
	AlturaY = (int) floor(DY * 3.0 / 4.0);
	MaxMinArrayA(&YMax, &YMin);

	if ((YMax == YMin) && (YMax == 0.0))
		return OK;

	Ssalt = 0;
	for (i = 0; i < cantDseries; i++)
		if (Usadas[i] == 0)
			Ssalt++;

	SerUsadas = cantDseries - Ssalt;

	/*	WiMsg("XOffset=%d, XLey=%d, YMax=%f, YMin=%f", XOffset, XLey, YMax, YMin); */

	ImprimoLeyendas(Sc, SerUsadas, 0);
	TituloGraf(Sc, DX, DY);

	YLabel = DY - DY / 12 - 1;
	EtiquetoGraf(Sc, XOffset, DX, YLabel);


	if (SerUsadas == 1) {
		Ancho = XLey / (cantXserie + 1);
		Gap = (int) ceil((double) Ancho / (double) (cantXserie + 1));
		while ((Ancho + Gap) * cantXserie >= XLey)
			Ancho--;
		GrillaComX = XOffset + Ancho / 2;
		GrillaStepX = Ancho + Gap;
		SerUsadas = 0; /* Porque despus siempre le sumo 1 por la separacin */
	} else {
		Ancho = (XLey - 1) / (cantXserie * (SerUsadas + 1) - 1);
		Gap = 0;
		GrillaComX = XOffset + Ancho * (SerUsadas / 2);
		GrillaStepX  = Ancho * (SerUsadas + 1);
	}


	if (Ancho < 4) {
		if (quien == GR_PANT) {
			Ancho = 3;
			if (SerUsadas) {
				GrillaComX = XOffset + Ancho * (SerUsadas / 2);
				GrillaStepX  = Ancho * (SerUsadas + 1);	 /* Recalculo Grilla */
				grupXpant = (XLey - 1) / ((SerUsadas + 1) * (Ancho + Gap) - 1) - 1;
				/*	WiMsg("XLey=%d, gXp=%d", XLey, grupXpant);  */
			} else {
				GrillaComX = XOffset + Ancho / 2;
				GrillaStepX  = (Ancho + Gap) * (SerUsadas + 1);	 /* Recalculo Grilla */
				grupXpant = ((XLey - 1) / (Ancho + Gap)) - 1;
			}

			MaxSector = cantXserie - grupXpant;

		} else {
			if (Ancho < 3)
				return G_ERR1;
		}
	} else {
		grupXpant = XLey / ((SerUsadas + 1) * (Ancho + Gap));
		MaxSector = cantXserie - grupXpant;
		/*	WiMsg("Ancho>=4 : Ancho=%d, gXp=%d, MaxSec=%d", Ancho, grupXpant, MaxSector);*/
	}


	if (TipoEscalaY == GR_AUTO) {

		if (YMin >= 0.0) {
			YStep = RedonUp(YMax * 4.0 / (DY * 3.0));
			EjeX = DY / 12;
		} else
			RedonDoble(DY);

		ejes(DX, DY, Sc, GR_AUTO);
	}


	if (TipoEscalaY == GR_MANUAL) {
		YMin = LimiteInfY;
		YMax = LimiteSupY;

		if (YMax > 0) {
			if (YMin < 0)
				RedonDoble(DY);
			else {
				YStep = RedonUp((YMax - YMin) * 4.0 / (DY * 3.0));
				EjeX = DY / 12;
			}
		} else {
			YStep = RedonUp((YMax - YMin) * 4.0 / (DY * 3.0));
			EjeX =  DY - DY / 6 - 2;
		}

		ejes(DX, DY, Sc, GR_MANUAL);
	}


	entrada = ' ';
	Dibujo = 1;
	while ((entrada == ' ') || (entrada == P_CRSR_LT) || (entrada == P_CRSR_RG) ) {

		IndiceJ = -1;
		for (j = 0; j < cantXserie; j++) {
			if ( ((j + 1) >= Sector) && ((j + 1) <= Sector + grupXpant) && (Dibujo == 1) ) {
				IndiceJ++;
				Ssalt = 0;
				for (s = 0; s < cantDseries; s++) {
					if (Usadas[s] == 0)
						Ssalt++;
					else {
						if (cantDseries == 1)
							i = IndiceJ;	/* IndiceJ==j */

						else
							i = IndiceJ * (SerUsadas + 1) + s - Ssalt;

						if (TipoEscalaY == GR_MANUAL) {
							if (YMin >= 0.0) {
								if (A[MAX_VAL*s+j] < YMax)
									if (A[MAX_VAL*s+j] < YMin)
										Altura = 0.0;
									else {
										Altura = (A[MAX_VAL*s+j] - YMin) / YStep;
									}
								else
									Altura = YMax / YStep;
							} else {
								if (YMax >= 0.0) {
									if (A[MAX_VAL*s+j] < YMax)
										if (A[MAX_VAL*s+j] > YMin)
											Altura = A[MAX_VAL*s+j] / YStep;
										else
											Altura = YMin / YStep;
									else
										Altura = YMax / YStep;
								} else	/* Entonces YMin < 0  && YMax < 0 */						 {
									if (A[MAX_VAL*s+j] >= YMax)
										Altura = 0.0;
									else if (A[MAX_VAL*s+j] <= YMin)
										Altura = YMin / YStep;
									else
										Altura = (A[MAX_VAL*s+j] - YMax) / YStep;
								}
							}
						} else
							Altura = A[MAX_VAL*s+j] / YStep;

						if (fabs(Altura - floor(Altura)) > 0.5)
							Y1 = (int) ceil(Altura);
						else
							Y1 = (int) floor(Altura);

						if (Y1 > AlturaY)
							Y1 = AlturaY;	/* Por si se pas por escala MANUAL */
						else if (Y1 < -AlturaY)
							Y1 = -AlturaY;

						if (SerUsadas == 0)
							OffGap = IndiceJ * Gap;
						else
							OffGap = i * Gap;


						if (Y1 > 0) {
							signo = 1;
							barra(XOffset + OffGap + Ancho * i, EjeX, XOffset + OffGap + Ancho * (i + 1), Y1 + EjeX, Patt[s], Sc, DX);
						}

						if (Y1 < 0) {
							signo = -1;
							barra(XOffset + OffGap + Ancho * i, Y1 + EjeX, XOffset + OffGap + Ancho * (i + 1), EjeX, Patt[s], Sc, DX);
						}


						if (EtiquetoBarra == GR_SI) {
							if ((Y1 + EjeX < DY * 5 / 6) && (Y1 + EjeX > DY / 12)) {
								temp = XOffset + OffGap + Ancho * i + (Y1 + EjeX + signo) * DX;

								sprintf(Buff, "%d", (int) A[MAX_VAL*s+j]);
								if (strlen(Buff) >= Ancho - 1)
									sprintf(Buff, "%g", A[MAX_VAL*s+j]);

								Largo = strlen(Buff);
								if (Largo > Ancho - 1)
									Largo = Ancho - 1;

								aux = (Ancho - Largo) >> 1;
								if (aux > 0)
									temp += aux;
								strncpy((char *)(Sc + temp + 1), Buff, Largo);
							}
						}
					}	/* else del if Usadas == 0 */
				}	/* For s */


				if (X[j] ) {
					Largo = strlen(X[j]);

					if (SerUsadas == 0)		/* Fu 1 pero lo dej en 0 */ {
						Largo = (Largo > Ancho) ? Ancho : Largo;
						aux = (Ancho - Largo) / 2;
						temp = DX + XOffset + IndiceJ * (Ancho + Gap) + aux;
						/*		WiMsg("Entr a SerUsadas==0. X[%d]=%s, temp=%d, Ancho=%d, Gap=%d", j, X[j], temp, Ancho, Gap); */
						for (k = 0; k < Largo; k++)
							Sc[temp+k] = *(X[j] + k);
					} else {
						Largo = (Largo > Ancho * SerUsadas) ? Ancho * SerUsadas : Largo;
						aux = (Ancho * SerUsadas - Largo) / 2;
						/*				WiMsg("Entr al else : X[%d]=%s, IndiceJ=%d", j, X[j], IndiceJ);*/
						temp = DX + XOffset + IndiceJ * (SerUsadas + 1) * Ancho + aux;
						for (k = 0; k < Largo; k++)
							Sc[temp+k] = *(X[j] + k);
					}
				}
			}	/* IF cae dentro del Sector*/
		}  /* FOR  j*/

		if (quien == GR_PANT) {
			ImprimoFlechas(Sc, Sector, MaxSector);

			if (Dibujo == 1)
				DisplayPantalla(1);	/*	 Aqu se setea entrada */
			else {
				RefreshWin(wigraf, 1);
				entrada = Getc();
			}

			SectorAnterior = Sector;

			ProcesoEntrada(&Sector, MaxSector);

			Dibujo = (SectorAnterior != Sector);
			if (Dibujo)
				ResetZonaGrafica(Sc);
		} else
			entrada = 'f';		/* Finaliza porque fu llamada para impresin */

	} 	/* WHILE */

	return OK;
}



/****************************************************************************/
/*	Funcion GraficoPilas: Genera una grfica de pilas segn variables glob. */
/****************************************************************************/

static int GraficoPilas(Display Sc, int DX, int DY, int quien)
{
	int	i, j, k, s, Ancho, Y1, signo, BP[MAX_SER], XOffset, XLey;
	int	Largo, Ssalt, SerUsadas, IndiceSerie, Indent, Gap, YLabel;
	int	Sector = 1, SectorAnterior, MaxSector = 1, Dibujo, IndiceJ;
	int	temp, grupXpant = 0;
	double	Altura, BPDouble[MAX_VAL];

	CalculoLimites();

	XOffset = DX / 8;
	XLey = DX - XOffset;

	Ssalt = 0;	 /* Series Salteadas.  i.e. Sin valores */

	for (i = 0; i < cantXserie; i++) {
		BPDouble[i] = 0.0;
		for (s = 0; s < cantDseries; s++)
			BPDouble[i] += A[MAX_VAL*s+i];
	}


	MaxMinArray(BPDouble, &YMax, &YMin, cantXserie);

	if (HayDatos == 0)
		return G_ERR2;
	else {

		for (i = 0; i < cantDseries; i++)
			if (Usadas[i] == 0)
				Ssalt++;

		SerUsadas = cantDseries - Ssalt;

		ImprimoLeyendas(Sc, SerUsadas, 0);
		TituloGraf(Sc, DX, DY);

		YLabel = DY - DY / 12 - 1 ;

		EtiquetoGraf(Sc, XOffset, DX, YLabel);

		Ancho = (int) floor(DX * 7.0 / (8.0 * (1 + cantXserie)));
		Gap = Ancho / cantXserie;


		if (Ancho < 8) {
			if (quien == GR_PANT) {
				Ancho = 8;
				Gap = 4;
				grupXpant = XLey / 12;
				MaxSector = cantXserie - grupXpant;
				GrillaStepX = 12;
				GrillaComX  = XOffset + 4;
			} else {
				if (Ancho < 3)
					return G_ERR1;
				GrillaStepX = Ancho + Gap;
				GrillaComX  = XOffset + (Ancho / 2);
			}
		} else {
			grupXpant = XLey / (Ancho + Gap);
			MaxSector = cantXserie - grupXpant;
			GrillaStepX = Ancho + Gap;
			GrillaComX  = XOffset + (Ancho / 2);
		}


		if (YMin >= 0.0) {
			YStep = RedonUp(YMax * 4.0 / (DY * 3.0));
			EjeX = DY / 12;
			ejes(DX, DY, Sc, GR_AUTO);
		} else {
			RedonDoble(DY);
			ejes(DX, DY, Sc, GR_AUTO);
		}

		if ((YMax == YMin) && (YMax == 0.0))
			return OK;

		entrada = ' ';
		Dibujo = 1;
		while ((entrada == ' ') || (entrada == P_CRSR_LT) || (entrada == P_CRSR_RG) ) {

			IndiceJ = -1;
			for (j = 0; j < cantXserie; j++) {
				if ( (j + 1 >= Sector) && (j + 1 <= Sector + grupXpant) && (Dibujo == 1) ) {
					IndiceJ++;
					signo = (A[j] >= 0.0) ? 1 : -1;
					for (IndiceSerie = 0; IndiceSerie < cantDseries; IndiceSerie++) {
						Altura = A[MAX_VAL*IndiceSerie+j] / YStep;

						if (fabs(Altura - floor(Altura)) > 0.5) {
							if (A[MAX_VAL*IndiceSerie+j] >= 0.0)
								Y1 = (int) ceil(Altura);
							else
								Y1 = (int) floor(Altura);
						} else {
							if (A[MAX_VAL*IndiceSerie+j] >= 0.0)
								Y1 = (int) floor(Altura);
							else
								Y1 = (int) ceil(Altura);
						}

						if (((signo == 1) && (Y1 < 0)) || ((signo == -1) && (Y1 > 0)))
							return G_ERR3;

						if (IndiceSerie == 0)
							BP[IndiceSerie] = Y1 + EjeX;
						else
							BP[IndiceSerie] = Y1 + BP[IndiceSerie-1];
					}

					pilas(XOffset + (Ancho + Gap) * IndiceJ, EjeX, XOffset - 1 + Gap * IndiceJ + Ancho * (IndiceJ + 1), BP, cantDseries, Sc, DX);

					if (signo == 1) {
						Escribir(Sc, EjeX * DX + XOffset + (Ancho + Gap) * IndiceJ, GLK_UP);
						Escribir(Sc, EjeX * DX + XOffset - 1 + Gap * IndiceJ + Ancho * (IndiceJ + 1), GLK_UP);
					} else {
						Escribir(Sc, EjeX * DX + XOffset + (Ancho + Gap) * IndiceJ, GLK_DW);
						Escribir(Sc, EjeX * DX + XOffset - 1 + Gap * IndiceJ + Ancho * (IndiceJ + 1), GLK_DW);
					}

					if (X[j] ) {
						Largo = strlen(X[j]);
						Largo = (Largo > Ancho) ? Ancho : Largo;
						Indent = (Ancho - Largo) / 2;
						temp = DX + XOffset + (Ancho + Gap) * IndiceJ + Indent;
						for (k = 0; k < Largo; k++)
							Sc[temp+k] = *(X[j] + k);
					}

				}		/* IF */
			}	/* FOR j */

			if (quien == GR_PANT) {
				ImprimoFlechas(Sc, Sector, MaxSector);

				if (Dibujo == 1)
					DisplayPantalla(1);	/*	 Aqu se setea entrada */
				else {
					RefreshWin(wigraf, 1);
					entrada = Getc();
				}

				SectorAnterior = Sector;

				ProcesoEntrada(&Sector, MaxSector);

				Dibujo = (SectorAnterior != Sector);

				if (Dibujo)
					ResetZonaGrafica(Sc);
			} else
				entrada = 'f';	/* Finalizo xq fu llamada para imprimir */

		}	/* WHILE */
	}

	return OK;
}


/*************************************************************************/
/*	Funcion GraficoPizza: Genera un grfico PIE rectangular				 */
/*************************************************************************/

static int GraficoPizza(Display Sc, int DX, int DY, int quien)
{
	int	j, J, k, Largo, MaxLargoX, Y1, XOffset, YOffset, temp, PZ[MAX_VAL];
	int TamY1;
	int	LargoBuff, SplitX = 0;
	double	suma, Ancho;
	char	Buff[20];

	CalculoLimites();

	TituloGraf(Sc, DX, DY);

	suma = 0.0;
	for (k = 0; k <= MaxSer[0]; k++)
		suma += A[k];


	if (suma == 0.0)
		return ERR;

	YStep = RedonUp(suma / (DX - cantXserie));

	if (YStep == 0.0)
		return ERR;

	YOffset = (DY / 2 - MaxSer[0]) / 2;
	if (YOffset < 0)
		YOffset = 0;

	MaxLargoX = 0;
	for (k = 0; k <= MaxSer[0]; k++)
		if (X[k] )
			if ((Largo = strlen(X[k])) > MaxLargoX)
				MaxLargoX = Largo;

	if (MaxSer[0] >= 6)
		SplitX = 1;		/* Los X-rangos aparecen en dos columnas */

	TamY1 = 0;
	for (j = 0; j <= MaxSer[0]; j++) {
		Ancho = A[j] / YStep;

		if (fabs(Ancho - floor(Ancho)) > 0.5)
			Y1 = (int) ceil(Ancho);
		else
			Y1 = (int) floor(Ancho);

		if (Y1 >= 0)
			Y1++;

		PZ[j] = Y1;
		TamY1 += Y1;

		if ((SplitX == 1) && (j > MaxSer[0] / 2)) {
			XOffset = DX / 2;
			J = j - (MaxSer[0] + 1) / 2;
		} else {
			XOffset = 0;
			J = j;
		}

		temp = (J + YOffset) * DX + XOffset;
		Sc[temp] = '(';
		sprintf(Buff, "%g", (100 * A[j]) / suma);
		LargoBuff = strlen(Buff);
		if (LargoBuff > 4)
			LargoBuff = 4;
		strncpy((char *)(Sc + temp + 1), Buff, LargoBuff);
		Sc[temp+5] = ')';
		Sc[temp+6] = '%';

		if (X[j] ) {
			Largo = strlen(X[j]);

			if (SplitX)
				Largo =  (Largo > DX) ? DX : Largo;
			else
				Largo =  (Largo > DX / 2 - 6) ? DX / 2 - 6 : Largo;

			k = 0;
			temp += 8;
			while ( (k < Largo) && (*(X[j] + k) != '\0') ) {
				Sc[temp] = Patt[j];
				Sc[temp+1] = '=';
				Sc[temp+2+k] = *(X[j] + k);
				k++;
			}
		}
	}


	pizza(PZ, DY, DX, Sc, YOffset / 2,(DX - TamY1)/2);

	if (quien == GR_PANT)
		DisplayPantalla(1);

	return OK;
}



/*************************************************************************/
/*	Funcion GraficoXY: Genera un grfico de tipo XY						 */
/*************************************************************************/

static int GraficoXY(Display Sc, int DX, int DY, int quien)
{
	int	i, j, SerUsadas, GrafX, GrafY, XOffset, YOffset, LinPos, LinNeg;
	int	Ssalt, YLabel, Largo, temp, DatosNulos;
	double	MinX, MaxX, MinY, MaxY, XStep, StepAnt, ValX[MAX_VAL], aux;
	char	Buff[15];

	CalculoLimites();

	if (UsadoX == 0)
		return G_ERR6;

	for (i = 0; i < cantXserie; i++)
		if (X[i] == NULL)
			ValX[i] = 0.0;
		else
			ValX[i] = atof(X[i]);

	MaxMinArray(ValX, &MaxX, &MinX, cantXserie);

	XOffset = DX / 8;
	YOffset = DY / 12;

	SerUsadas = 0;
	MaxY = -HUGE;
	MinY = HUGE;

	for (i = 0; i < MAX_SER; i++) {
		if (Usadas[i] == 1) {
			SerUsadas++;
			for (j = 0; j <= MaxSer[i]; j++) {
				if (A[MAX_VAL*i+j] > MaxY)
					MaxY = A[MAX_VAL*i+j];
				if (A[MAX_VAL*i+j] < MinY)
					MinY = A[MAX_VAL*i+j];
			}
		}
	}


	DatosNulos = ((MaxY == MinY) && (MaxY == 0.0));


	if (!SerUsadas)
		return G_ERR7;

	Ssalt = 0;
	for (i = 0; i < MAX_SER; i++)
		if (Usadas[i] == 0)
			Ssalt++;

	ImprimoLeyendas(Sc, Ssalt, 1);
	TituloGraf(Sc, DX, DY);

	YLabel = DY - DY / 12 - 1;
	EtiquetoGraf(Sc, XOffset, DX, YLabel);


	/*	WiMsg("XY: MaxY=%f, MinY=%f \n MaxX=%f, MinX=%f", MaxY, MinY, MaxX, MinX);  */


	if (MinY >= 0.0) {
		EjeX = DY / 12;
		YMin = RedonDown(MinY, (MaxY - MinY) * 4.0 / (DY * 3.0)); /* Para que quede redondeado */
		MinY = YMin;
		YStep = RedonUp((MaxY - MinY) * 4.0 / (DY * 3.0));
		/*	WiMsg("MinY>=0.0 : EjeX=%d, YMin=%f, MinY=%f", EjeX, YMin, MinY); */
	} else if (MaxY <= 0.0) {
		EjeX = (DY * 5) / 6;
		MinY = -RedonUp(-MinY);
		YStep = RedonUp((MaxY - MinY) * 4.0 / (DY * 3.0));
		YMin = MinY + (int) (YStep * DY * 3.0 / 4.0);
	} else /* ==> Max > 0 && Min < 0 */		 {
		LinPos = RedonInt(DY * 3.0 * MaxY / (4.0 * (MaxY - MinY)));
		LinNeg = (int) floor(DY * 3.0 / 4.0) - LinPos;
		EjeX  =  LinNeg + DY / 12;
		StepAnt = RedonUp((MaxY - MinY) * 4.0 / (DY * 3.0));
		/*	WiMsg("StepAnt=%f", StepAnt);  */
		MinY = -LinNeg * StepAnt;
		/*	WiMsg("MinY=%f", MinY); */
		YStep = RedonUp((MaxY - MinY) * 4.0 / (DY * 3.0));
		/*	WiMsg("YStep=%f", YStep); */
		MinY = -LinNeg * YStep;
		YMin = 0.0;
		/*	WiMsg("While: YMin=%f, MinY=%f, YStep=%f \n StepAnt=%f, EjeX=%d", YMin, MinY, YStep, StepAnt, EjeX); */
	}

	XStep = RedonUp((MaxX - MinX) * 8.0 / (DX * 7.0));

	/*	WiMsg("XStep=%f, YStep=%f", XStep, YStep); */


	if (XStep == 0.0)
		return ERR;

	if (DatosNulos)
		YStep = 1.0;


	GrillaComX = XOffset / 2 + DX / 8 + 2;
	GrillaStepX = DX / 8;


	ejes(DX, DY, Sc, GR_MANUAL);


	for (i = 0; i < 7; i++) {
		aux = MinX + i * XStep * 10;

		if (fabs(aux) < 1e0 - 8)  /* PARCHE */
			sprintf(Buff, "%g", 0.0);
		else
			sprintf(Buff, "%g", MinX + i * XStep * 10);

		Largo = strlen(Buff);

		if (Largo > 9)
			Largo = 9;

		temp = XOffset + 10 * i;

		strncpy((char *)(Sc + temp + DX), Buff, Largo);

		Sc[DX*EjeX+temp] = GLK_CEN;
	}


	for (i = 0; i < cantDseries; i++)
		if (Usadas[i] == 1)
			for (j = 0; j <= MaxSer[i]; j++) {
				GrafX = RedonInt((ValX[j] - MinX) / XStep) + XOffset;
				GrafY = RedonInt((A[MAX_VAL*i+j] - MinY) / YStep) + YOffset;
				/*			WiMsg("Sc[%d,%d]=%c", GrafX-XOffset, GrafY-YOffset, Marca[i]);*/
				Escribir(Sc, GrafY * DX + GrafX, Marca[i]);
			}

	if (quien == GR_PANT)
		DisplayPantalla(1);

	return OK;
}


/*************************************************************************/
/*		Funcion TituloGraf: Setea el ttulo y subttulo a una grafica 	 */
/*************************************************************************/


static void TituloGraf(Display Sc, int DX, int DY)
{
	int	i, ltt, lst, offset = 0;

	if (Titulo ) {
		ltt = strlen(Titulo);
		if (ltt > DX) 	
			ltt = DX;

		switch (MarTit) {
		case 0:		
			offset = 0;
			break;
		case 1:		
			offset = (DX - ltt) / 2;
			break;
		case 2:		
			offset = DX - ltt - 2;
			break;
		}

		for (i = 0; i < ltt; i++)
			Sc[(DY-2)*DX+i+offset] = *(Titulo + i);
	}


	if (Subtitulo  ) {
		lst = strlen(Subtitulo);
		if (lst > DX) 	
			lst = DX;

		switch (MarST) {
		case 0:		
			offset = 0;
			break;
		case 1:		
			offset = (DX - lst) / 2;
			break;
		case 2:		
			offset = DX - lst - 2;
			break;
		}

		for (i = 0; i < lst; i++)
			Sc[(DY-3)*DX+i+offset] = *(Subtitulo + i);
	}
}


/*************************************************************************/
/*		Funcion EtiquetoGraf: Setea las etiquetas X e Y					 */
/*************************************************************************/

static void EtiquetoGraf(Display Sc, int Offset, int DX, int Y)
{
	int	i, aux, lx, ly;

	if (LabelX ) {
		lx = strlen(LabelX);
		if (lx > DX) 	
			lx = DX;

		aux = DX - lx;
		for (i = aux; i < DX; i++)
			Sc[i] = *(LabelX + i - aux);
	}

	if (LabelY  ) {
		ly = strlen(LabelY);
		if (ly > Offset) 	
			ly = Offset;

		aux = Offset - ly;
		for (i = aux; i < Offset; i++)
			Sc[Y*DX + i] = *(LabelY + i - aux);
	}
}


/***************************************************************************/
/*	Funcion ImprimoLeyendas: Imprime las leyendas seteadas que correspondan */
/***************************************************************************/

static void ImprimoLeyendas(Display Sc, int SerUsadas, int Desfasaje)
{
	int	Disponible, salteadas, temp, s, Largo, k;


	if (SerUsadas) {
		if (LabelX)
			Disponible = (PantX - strlen(LabelX)) / SerUsadas;
		else
			Disponible = PantX / SerUsadas;
		salteadas = 0;

		for (s = 0; s < cantDseries; s++) {
			temp = (s - salteadas) * Disponible;
			if ((Usadas[s] == 1) && (L[s] )) {
				Sc[temp] = Patt[s+Desfasaje];
				Sc[temp+1] = '=';
				Largo = strlen(L[s]);
				Largo = (Largo > Disponible - 3) ? (Disponible - 3) : Largo ;
				for (k = 0; k < Largo; k++)
					Sc[temp+k+2] = *(L[s] + k);
			} else
				salteadas++;
		}
	}
}


/*************************************************************************/
/*	Procedimiento ImprimoFlechas: Imprime las flechas que marcan que una */
/*	grfica contina a los costados.									 */
/*************************************************************************/

static void ImprimoFlechas(Display Sc, int Sector, int MaxSector)
{

	if ( (Sector > 1) && (Sector < MaxSector) ) {
		Escribir(Sc, PantX * EjeX + PantX - 1, '>');
		Escribir(Sc, PantX * EjeX + PantX - 2, '<');
	} else {
		if (Sector < MaxSector) {
			Escribir(Sc, PantX * EjeX + PantX - 2, '>');
			Escribir(Sc, PantX * EjeX + PantX - 1, '>');
		} else if (Sector > 1) {
			Escribir(Sc, PantX * EjeX + PantX - 2, '<');
			Escribir(Sc, PantX * EjeX + PantX - 1, '<');
		}
	}
}



/*************************************************************************/
/*	Funcion Display: Despliega el contenido de Screen en una  Ventana	*/
/*************************************************************************/

static void DisplayPantalla(int tecla)
{
	int	i, j;


	StrBuff[1] = '\0';
	LkAt(At_NORMAL);

	for (i = 0; i < PantY; i++) {
		for (j = 0; j < PantX; j++) {
			if (Screen[i*PantX+j] == '~') {
				StrBuff[0] = ' ';
				LkAtMvAdd(At_REVERSE, PantY - i - 1, j, StrBuff);
			} else {
				StrBuff[0] = Screen[i*PantX+j];

				if ( (i == EjeX) && ((TipoGrafica == PILAS) || (TipoGrafica == BARRAS)) &&  ((Screen[i*PantX+j] == '<') || (Screen[i*PantX+j] == '>')) )
					LkAtMvAdd(At_BLINK, PantY - i - 1, j, StrBuff);
				else
					LkMvAdd(PantY - i - 1, j, StrBuff);
			}
		}
	}

	if (tecla == 1)
		entrada = Getc();
}

static void ResetZonaGrafica(Display Sc)
{
	int	i, j, Xi, Ys, temp;


	Xi = PantX / 8 - 1;
	Ys = PantY - PantY / 12 - 1;

	for (j = 1; j < Ys; j++) {
		temp = j * PantX;
		for (i = Xi; i < PantX; i++)
			Sc[temp+i] = ' ';
	}

	ejes(PantX, PantY, Sc, (int)FToL( TipoEscalaY ) );

	DisplayPantalla(0);
}


/*************************************************************************/
/*		Funcion PrintScreen: Escribe una archivo para ser impreso		 */
/*************************************************************************/

extern "C" int PrintScreen(FILE *PantScr)
{
	int	ok = 0, inverso;

	if (HayDatos == 0)
		return G_ERR2;
	else {
		if ( (DatosPositivos() == 0) && ((TipoGrafica == PILAS) || (TipoGrafica  == PIE)) )
			return G_ERR3;

		InitScreen(GR_PRINT);

		switch (TipoGrafica) {
		case BARRAS :	
			ok = GraficoBarras(Im, ImpX, ImpY, GR_PRINT);
			break;
		case PILAS :	
			ok = GraficoPilas(Im, ImpX, ImpY, GR_PRINT);
			break;
		case PIE   :	
			ok = GraficoPizza(Im, ImpX, ImpY, GR_PRINT);
			break;
		case XY    :	
			ok = GraficoXY(Im, ImpX, ImpY, GR_PRINT);
			break;
		}

		if (ok != OK)
			return ok;
		else {
			if (PantScr  == NULL)
				return G_ERR5;
			else {
				int NroPag, NroColPag;
				int FilaActual, ColActual;
				int h, i, j, k, IndX, IndY;

				NroPag = ImpY/PageY + (ImpY%PageY ? 1 : 0);
				NroColPag = ImpX/PageX + (ImpX%PageX ? 1 : 0);
				inverso = 0;
				for(h = 0; h < NroPag; h++) {
					FilaActual = ImpY - (PageY*h) - 1; 

					for(k = 0,ColActual = 0; k < NroColPag ; k++ , ColActual += PageX) {
						for (i = PageY - 1,IndY = FilaActual; i >= 0 && IndY >= 0; i--, IndY--) {
							for (j = 0, IndX = ColActual; j < PageX && IndX < ImpX; j++, IndX++) {
								if (Im[IndY*ImpX+IndX] == '~') {
									if (inverso == 0) {
										fputc(27, PantScr);
										fputc('[', PantScr);
										fputc('7', PantScr);	/* Seteo el modo inverso */
										fputc('m', PantScr);
										inverso = 1;
									}

									fputc(' ', PantScr);

									if (Im[IndY*ImpX+IndX+1] != '~') {
										fputc(27, PantScr); /* Seteo el modo normal */
										fputc('[', PantScr);
										fputc('0', PantScr);
										fputc('m', PantScr);
										inverso = 0;
									}
								} else
									fputc(Im[IndY*ImpX+IndX], PantScr);
							}
							if (inverso) {
								fputc(27, PantScr); /* Seteo el modo normal */
								fputc('[', PantScr);
								fputc('0', PantScr);
								fputc('m', PantScr);
								inverso = 0;
							}
							fputc('\n', PantScr);		/* Fin de lnea de impresin */
						}
					}
				}
				fflush(PantScr);
			}
		}
		return OK;
	}
}


/*************************************************************************/
/*		Funciones de  INTERFASE con otros mulos.						 */
/*************************************************************************/

extern "C" void SetearTipo(int Tipo)
{
	TipoGrafica = Tipo;
}

extern "C" void SetearDato(int Serie, int Indice, double Valor)
{
	if (Indice <= MAX_VAL) {
		HayDatos = 1;

		Usadas[Serie] = 1;
		A[MAX_VAL*Serie+Indice] = Valor;
		if (Indice > MaxSer[Serie])
			MaxSer[Serie] = Indice;
	}
}

extern "C" void SetearTitulo(const char *Tit, int Mar)
{
	int	largo;

	if (Titulo )
		DeleteS(Titulo);

	largo = strlen(Tit);
	Titulo = NewN(largo + 1);
	strcpy(Titulo, Tit);

	MarTit = Mar;
}

extern "C" void SetearSubtitulo(const char *Subtit, int Mar)
{
	int	largo;

	if (Subtitulo )
		DeleteS(Subtitulo);
	largo = strlen(Subtit);
	Subtitulo = NewN(largo + 1);
	strcpy(Subtitulo, Subtit);

	MarST = Mar;
}

extern "C" void SetearLabelX(const char *Label)
{
	int	largo;

	if (LabelX )
		DeleteS(LabelX);

	largo = strlen(Label);
	LabelX = NewN(largo + 1);
	strcpy(LabelX, Label);
}

extern "C" void SetearLabelY(const char *Label)
{
	int	largo;

	if (LabelY )
		DeleteS(LabelY);

	largo = strlen(Label);
	LabelY = NewN(largo + 1);
	strcpy(LabelY, Label);
}

extern "C" void SetearLeyenda(int Indice, const char *Ley)
{
	int	largo;

	assert(Indice < MAX_SER);

	if (L[Indice] )
		DeleteS(L[Indice]);
	largo = strlen(Ley);
	L[Indice] = NewN(largo + 1);
	strcpy(L[Indice], Ley);
}

extern "C" void SetearRangoX(int Indice, const char *str)
{
	int	largo;

	UsadoX++;
	if (X[Indice] )
		DeleteS(X[Indice]);
	largo = strlen(str);
	X[Indice] = NewN(largo + 1);
	strcpy(X[Indice], str);
}

/*
	Not used

extern "C" void SetearEjes(int XT, int YT, int CeroX, double YInf, double YSup)
{
	XTics = XT;
	YTics = YT;
	EjeX = CeroX;
	LimiteSupY = YSup;
	LimiteInfY = YInf;
}
*/

extern "C" void SetearGrilla(int Tipo)
{
	Grilla = Tipo;
}

extern "C" int SetearPantalla(void)
{
	PantX = MaxColMntr - 1;
	PantY = MaxFilMntr - 1;
	Screen = NewV(unsigned char, PantX * PantY + 1);
	return (Screen ? OK : ERR );
}

extern "C" int SetearImpresora(int GX, int GY, int IX, int IY)
{
	if (Im )
		DeleteN(Im, ImpX * ImpY + 1);

	ImpX = GX;
	ImpY = GY;
	PageX = IX;
	PageY = IY;
	Im = NewV(unsigned char, GX * GY + 1);
	return(Im ? OK : ERR);
}


extern "C" void SetearTipoEscalaY(int Tipo)
{
	TipoEscalaY = Tipo;
}

extern "C" void SetearPattern(int indice, char car)
{
	Patt[indice] = car;
}

extern "C" void SetearValorEnBarras(int Tipo)
{
	EtiquetoBarra = Tipo;
}

extern "C" int Graficar(void)
{
	int	ok = 0;

	if (HayDatos == 0) 
		return G_ERR2;
	else {
		if ( (DatosPositivos() == 0) && ((TipoGrafica == PILAS) || (TipoGrafica  == PIE)) )
			return G_ERR3;

		wigraf = OpenWin(0, 0, PantY + 1, PantX + 1);
		PutOnTop(wigraf, 1);

		InitScreen(GR_PANT);

		switch (TipoGrafica) {
		case BARRAS :	
			ok = GraficoBarras(Screen, PantX, PantY, GR_PANT);
			break;
		case PILAS :	
			ok = GraficoPilas(Screen, PantX, PantY, GR_PANT);
			break;
		case PIE   :	
			ok = GraficoPizza(Screen, PantX, PantY, GR_PANT);
			break;
		case XY    :	
			ok = GraficoXY(Screen, PantX, PantY, GR_PANT);
			break;
		}

		CloseWin(wigraf);

		if (ok != OK)
			return ok;

		return OK;
	}
}

extern "C" void EndGrafics()
{
	if (Screen) {
		DeleteN(Screen, PantX * PantY + 1);
	}
	InitOpciones();
}
