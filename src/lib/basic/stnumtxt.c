/********************************************************************
* Copyright (c) 1987 InterSoft Co.  All Rights Reserved
* THIS IS UNPUBLISHED PROPRIETARY SOURCE CODE OF InterSoft Co.
* The copyright notice above does not evidence any
* actual or intended publication of such source code.
*
* $Id: stnumtxt.c,v 1.2 2006/02/17 15:58:29 spark Exp $
*
* DESCRIPTION
* Number to text conversion function
*
* ROUTINE       |  PURPOSE
*---------------+---------------------------------------------------
*NumToTxt			 Convert number to alpha.
*********************************************************************/

#include <ideafix.h>
#include <ideafix/priv/gndefs.h>

/* ++ Prototypes ++ */

static long dnum_to_txt(double m, char *x);
static void inum_to_txt(long n, char *x);

/* -- Prototypes -- */


/*
*       Rutina de conversion de numero a letras
*       Parametros :     Nro.    Tipo    Significado
*                      1        double    Numero a convertir.
*                      2        char[]    Buffer para almacenar el res.
*                      3        int     Maximo de renglones.
*                      4        int        Dimension de cada/renglon.
*                      5        int        1 = Uno final:'uno', 0 = 'un'
*                      6        char[]    String a concatenar adelante.
*                      7        char[]    String a concatenar atras.
*/

#define MAX_TEXT_SIZE	400

void NumToTxt(double m, void *buff, int f, int c, int uno,
		char adelante[], char atras[])
{
	char buffer[MAX_TEXT_SIZE], *p, *p1, *px;
	int j;
	long lastgroup;
	char *x = (char *) buff;

	strcpy(buffer, adelante);
	lastgroup = dnum_to_txt(m, buffer);

	if (uno && lastgroup % 10 == 1 && lastgroup % 100 != 11) {
		p = buffer + strlen(buffer) -
				strlen((char *) __StErrmsg("N_UNIDADES1"));
		strcpy(p, (char *) __StErrmsg("N_UNO"));
	}
	strcat(buffer, atras);
	mZero(x, f*c);
	
	for (p = buffer, j = 0; j < f && *p != 0; j++) {
		for (p1 = px = x + j*c; p1-px < c-1 && *p != 0; p++)
			if (*p != '-') *p1++ = *p;

		if (*p) {
			while (*p != ' ') {
				if (*--p == '-') {
					*p1++ = '-';
					break;
				}
				--p1;
			}
			++p;
		}
		*p1 = '\0';
	}
}
/*
*  Rutina de conversion de grupos.
*/

/* +++ CONFIG +++ */

#define BILLON 1000000000000.0
#define MILLON 1000000.0

/* Private Functions												*/

static long dnum_to_txt(double m, char *x)
{
	long n;
	if ((n = (long) (m/BILLON))) {
		inum_to_txt(n, x);
		strcat(x, n == 1 ? (char *) __StErrmsg("N_BILLONS1")
						 : (char *) __StErrmsg("N_BILLONS2"));
		m -= (double)n * BILLON;
	}
	if ((n = (long) (m/MILLON))) {
		inum_to_txt(n, x);
		strcat(x, n == 1 ? (char *) __StErrmsg("N_MILLONS1")
						 : (char *) __StErrmsg("N_MILLONS2"));
	}
	if ((n = (long) (m - (double) n * MILLON)))
		inum_to_txt(n, x);
	return n;
}
/*
*    Rutina de conversion propiamente dicha
*/

static void inum_to_txt(long n, char *x)
{
	long m;

	if (n <= 21) {
		char msgtxt[16];
		sprintf(msgtxt, "N_UNIDADES%d", (int) n);
		strcat(x, (char *) __StErrmsg(msgtxt));
	} else if (n < 100) {
		char msgtxt[16];
		sprintf(msgtxt, "N_DECENAS%d", (int)(n/10));
		strcat(x, (char *) __StErrmsg(msgtxt));
		if ((m = n % 10) != 0) {
			if (n > 30)
				strcat(x, (char *) __StErrmsg("N_YDECENAS"));
			inum_to_txt(m, x);
		}
	} else if (n == 100) {
		strcat(x, (char *) __StErrmsg("N_CIEN"));
	} else if (n < 1000) {
		char msgtxt[16];
		sprintf(msgtxt, "N_CENTENAS%d", (int)(n/100));
		strcat(x, (char *) __StErrmsg(msgtxt));
		if ((m = n % 100) != 0) {
			strcat(x, (char *) __StErrmsg("N_YCENTENAS"));
			inum_to_txt(m, x);
		}
	} else {
		if ((m = n / 1000) > 1)
			inum_to_txt(m, x);
		if (m) 
			strcat(x, m == 1 ? (char *) __StErrmsg("N_MIL1")
							 : (char *) __StErrmsg("N_MIL2"));
		if ((m = n % 1000) != 0)
			inum_to_txt(m, x);
	}
}
