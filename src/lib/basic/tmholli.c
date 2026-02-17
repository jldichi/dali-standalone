/********************************************************************
* Copyright (c) 1987 InterSoft Co.  All Rights Reserved
* THIS IS UNPUBLISHED PROPRIETARY SOURCE CODE OF InterSoft Co.
* The copyright notice above does not evidence any
* actual or intended publication of such source code.
*
* $Id: tmholli.c,v 1.3 2006/02/17 16:00:19 spark Exp $
*
* DESCRIPTION
*
* ROUTINE       |  PURPOSE
*---------------+---------------------------------------------------
* AddMonth       Add n Months to a Date.
* IsHolliday     Return holliday description or NULL.
*********************************************************************/

#include <stdlib.h>
#include <ideafix.h>
#include <ideafix/priv/gndefs.h>

#define  FERIADOS "holliday.dat" /* Arch. de feriados	 			 */
#define  LGFER     29  /* longitud de la descripcion del feriado     */
#define  MES_MIN   5   /* mes minimo para el calendario				 */
#define  ANIO_MIN 1894 /* anio minimo para el calendario			 */
#define  MES_MAX   8   /* mes de la maxima fecha       				 */ 
#define  ANIO_MAX 2073 /* anio maximo para el calendario			 */
#define  IDEAFIXDIR	"/usr/ideafix"
#define  DEF_LANG	"spanish"
#define  PATH_LENGTH	256

#define	 PASCUA		'P'
#define	 M_PASCUA	-1

/* 
struct que conforma la tabla en memoria donde almaceno el archivo de
feriados
*/
struct holiday {
	short day;
	short month;
	short year;
	char  descr[LGFER+1];
	struct holiday *next;
};

/* ++ Prototypes ++ */

static struct holiday *CargoFeriados(void);
static bool IsPascua(short d, short m, short y);

/* -- Prototypes -- */


/*
	This routine returns for a given date, whether it is a holiday or not.
	If it is a holiday, it returns its description.
	The first time it is called it generates a linked list in memory
	from the holiday file.
*/
char *IsHolliday(DATE fecha) 
{
	static struct holiday *hdays = NULL;
	struct holiday *h;
	short d, m, y;

	if (hdays == NULL)
		hdays = CargoFeriados();

	for (h = hdays; h != NULL; h = h->next) {
		switch (h->day) {
		case M_PASCUA:
			DToDMY(fecha - h->month, &d, &m, &y);
			if (IsPascua(d, m, y))
				return h->descr;
			break;
		default:
			DToDMY(fecha, &d, &m, &y);
			if (h->day == d && h->month == m &&
					(h->year == 0 || h->year == y))
				return h->descr;
		}
	}

	if (((int)fecha % 7) == 0)
		return DayName(fecha);	

	return NULL;
}

/*
	This routine loads the contents of the holiday file into a linked
	list in memory.
*/

static struct holiday *CargoFeriados(void)
{
	FILE *fp = NULL;
	char linea[256];
	int desp;
	FPCPCPI save;
	char path[PATH_LENGTH], *ideafix, *lang;
	struct holiday *tmp, *h = NULL;

	save = SetReadEnvHandler(NULL);
	if ((ideafix = ReadEnv("IDEAFIX")) == NULL)
		ideafix = IDEAFIXDIR;
	if ((lang = ReadEnv("LANGUAGE")) == NULL)
		lang = DEF_LANG;
	sprintf(path, "%s/data/%s/%s", ideafix, lang, FERIADOS);
	SetReadEnvHandler(save);

	if (fp == NULL && (fp = fopen(path, "r")) == NULL) {
		WiMsg("No se puede acceder al Archivo de Feriados: %s", FERIADOS);
		return NULL;
	}

	while (fgets(linea, sizeof(linea), fp) != NULL) {
		tmp = h;
#if IDEAWIN
		h = (struct holiday *) malloc(sizeof(struct holiday));
#else
		h = (struct holiday *) Alloc(sizeof(struct holiday));
#endif /* IDEAWIN */
		h->next = tmp;
		switch (toupper(linea[0])) {
		case PASCUA:
			h->day = M_PASCUA;
			desp = 1;	
			if (linea[desp] == '+' || linea[desp] == '-') {
				h->month = (short) atoi(linea + desp);
				desp += 2;
			}
			desp++; 	
			break;
		default:
			h->day = (short) atoi(linea);
			desp = isdigit((UChar) linea[1]) ? 3 : 2;

			h->month = (short) atoi(linea + desp);
            desp += isdigit((UChar) linea[desp + 1]) ? 3 : 2;

			if (linea[desp - 1] == '/') {
				h->year = (short) atoi(linea + desp);
				desp += 5;
			}
		}
		strncpy(h->descr, linea + desp, LGFER);
		h->descr[LGFER] = '\0';
	}
	fclose(fp);

	return h;
}

/*
Rutina que dado una fecha le suma una cantidad de meses 
*/

DATE AddMonth(DATE fecha_act, int cantidad)
{
	short d, m, y;
	long i;

	DToDMY(fecha_act, &d, &m, &y);
	i = m + (y*12) + cantidad;
	m = (short) (i % 12);
	y = (short) (i / 12);
	if (m==0) {
		m = 12;
		y--;
	}

	if (y>ANIO_MAX) {
		y = ANIO_MAX;
		m = MES_MAX;
	}
	if (y==ANIO_MAX && m>MES_MAX)  m = MES_MAX;

	if (y<ANIO_MIN) {
		y = ANIO_MIN;
		m = MES_MIN;
	}

	return DMYToD(1, m, y);
}

/* algoritmo sacado del Knuth (Volumen 1 pag. 168-169) */

static bool IsPascua(short d, short m, short y)
{
	static short dia = 0, mes = 0, anio = 0;
	int g, c, x, z, D, e, n;

	/* si para este anio fue calculado */
	if (anio == y) {
		if (d == dia && m == mes) return TRUE;
		return FALSE;
	}

	g = (y%19)+1;	

	c = (y/100)+1;

	x = (3*c/4)-12;

	z = ((8*c+5)/25)-5;

	D = (5*y/4)-x-10;

	e = (11*g+20+z-x)%30;

	if ((e == 25 && g > 11) || e == 24) e++;

	n = 44-e;

	if (n < 21) n += 30;

	n = n+7-((D+n)%7);

	anio = y;

	if (n > 31) {
		dia = (short) n-31;
		mes = 4;
	}
	else {
		dia = (short) n;
		mes = 3;
	}

	if (d == dia && m == mes) return TRUE;

	return FALSE;
}
