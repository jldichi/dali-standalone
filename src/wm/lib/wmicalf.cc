/********************************************************************
* Copyright (c) 1987 InterSoft Co.  All Rights Reserved
* THIS IS UNPUBLISHED PROPRIETARY SOURCE CODE OF InterSoft Co.
* The copyright notice above does not evidence any
* actual or intended publication of such source code.
*
*
* $Id: wmicalf.cc,v 1.6 2010/09/28 19:55:13 hardaiz Exp $
*
*
* DESCRIPTION:
*    Aplicacion agenda/calendario
*
* ROUTINE       |  PURPOSE
*
* Rutinas que hace de interfaz entre la tabla de tareas y el pgm
* 
* OpenAgenda(tareas, today, flag):	
*				 Esta rutina abre el archivo de tareas y carga a estas 
*				 en el vector VERTOR.
*				 Today es la fecha actual para depurar el archivo.
*				 Flag muestra o no el msg de error si el archivo esta
*				 lockeado.
*
* ReadTask(punt_cla,modo,error):
*				Esta rutina devuelve en el buffer apuntado por la var
*				punt_cla la tarea leida, la variable modo indica si la
*				lectura es por mayor, por menor o por igual.
*				Si la clave buscada no se encuentra entoces la var error
*				retornara TRUE.
*
* AddTask(punt_cla):
*				 Esta rutina inserta la tarea contenida en el buffer 
*				 apuntado por punt_cla, en la tabla de tareas.
* 
* DeleteTask(punt_cla):
*				 Esta rutina deletea la tarea contenida en el buffer 
*				 apuntado por punt_cla, de la tabla de tareas.
* 
* CloseAgenda(tareas):
*				 Esta rutina baja el vector de tareas al archivo de 
*				 tareas.
*
* FTaskName(user, tareas): 
*				 Retorna el nombre del archivo de tareas teniemdo
*				 en cuenta el usuario pasado como parametro.
*
* TodayTask(lista, dim):
*				 Retorna en la estructura lista las tareas del dia
*				 devolviendo en dim la cantidad de tareas.
*---------------+---------------------------------------------------
*
*********************************************************************/

#include <stdlib.h>

#ifdef _AIX
#include <sys/lockf.h>
#endif

#include <ideafix.h>

#if defined(hpux) || defined(SunSol)
#include <unistd.h>
#endif

#include <cfix.h>
#include <ideafix/priv/gndefs.h>
#include <ideafix/priv/ical.h>


/* ++ Prototypes ++ */

static int Comp (struct clave *c1, struct clave *c2);
static void Numsem(DATE fecha_act, int hoja[][7], int *cantsem, int *semact);
static int Compar(struct t_task *p1, struct t_task *p2);

/* -- Prototypes -- */




FILE *fp = NULL;			 /* descriptor del archivo de tareas      */
int  cantreg;				 /* cantidad de reg. del vector de tareas */

/* vector donde almaceno el archivo de tareas en memoria */
struct clave _far vector[MAXTAREA];

int OpenAgenda(char *tareas, DATE today)
{
	int len, j;
	struct clave aux;
	long lugar;
	char linea[MAXCAR];

	if (fp) return OK;

	cantreg = 0;

	if (fp == NULL && (fp=fopen(tareas, "r+")) == NULL) {
		fp=fopen(tareas, "w");
		if (fp == NULL) return ERROR;
	}

#ifdef HAVE_LOCKF
	/* si el archivo no puede hacer lock entonces imprime msg y retorna 
	   error */
	if (fp==NULL || lockf(fileno(fp), 2, 0) == ERROR) {
		fp = NULL;
		return ERROR;
	}
#endif

	while (fgets(linea, MAXCAR, fp)!=NULL) {

		linea[8]  = '\0';
		linea[13] = '\0';
		linea[18] = '\0';

		/* cargo los datos claves */ 
		aux.tipo = linea[0];
		aux.fecha= StrToD(linea+2);
		aux.hora = StrToT(linea+9);

		/* cargo los demas datos */
		aux.puntdat = (struct datos *) Alloc(sizeof(struct datos));
		aux.puntdat->dura = StrToT(linea+14);
		aux.puntdat->alarma=atoi(linea+19);
		strncpy(aux.puntdat->titulo, linea+22, LGTAREA);
		aux.puntdat->titulo[LGTAREA] = '\0';

		/* cargo los datos para el calculo de periodicidad,
		   esto sirve solo para tareas repetitivas */
		strncpy(aux.puntdat->semana, linea+53, 5);
		strncpy(aux.puntdat->dia   , linea+59, 7);
	    fgets(linea, MAXCAR, fp);
		for (j=0;j<25;j++) {
			aux.puntdat->diario[j] = atoi(linea+(j*3));
			if (aux.puntdat->diario[j]==0)
				aux.puntdat->diario[j] = NULL_SHORT;
		}
	    fgets(linea, MAXCAR, fp);
		linea[strlen(linea)-1] = '\0';
		for (;j<31;j++) {
			aux.puntdat->diario[j] = atoi(linea+((j-25)*3));
			if (aux.puntdat->diario[j]==0)
				aux.puntdat->diario[j] = NULL_SHORT;
		}
		aux.puntdat->a_alarm = StrToD(linea+((j-25)*3));

	    fgets(linea, MAXCAR, fp);
		strncpy((char *)aux.puntdat->enedia, linea, MAXENEDIA);
		for(j = strlen(linea);j<MAXENEDIA;j += 2) 
			strncpy(aux.puntdat->enedia[j/2], "  ", 2);

		/* calculo primero la longitud de las lineas de descripcion */ 
		lugar = ftell(fp);
		len = 0;
		while (fgets(linea,MAXCAR, fp)!=NULL && linea[0]!=FIN_DESC) 
			len += strlen(linea);					
		len += 2;	

		/* luego aloca mem para la descripcion con la longitud ya
		   calculada, y guardo aqui la misma */
		aux.puntdat->puntbuff = (char *) Alloc(len);
		aux.puntdat->textsize = len;
		fseek(fp, lugar, 0);
		j = 0;
		while (fgets(linea,MAXCAR, fp)!=NULL && linea[0]!=FIN_DESC) {
			strcpy(aux.puntdat->puntbuff+j, linea);
			j += strlen(linea);
			aux.puntdat->puntbuff[j-1] = '\0';
		}
		/* por ultimo coloco la marca de fin de descripcion */
		aux.puntdat->puntbuff[j] = FIN_DESC;
		aux.puntdat->puntbuff[j+1] = '\0';

		/* salteo tareas temporales de mas de una semana */ 
		if (aux.fecha<(today-DIA_DEPUR) && aux.tipo==TT) {
			free(aux.puntdat->puntbuff);
			free(aux.puntdat);
			continue;
		}

		/* coloco la tareas en la tabla de tareas */
		if (!AddTask(&aux))
			break;
	}
	return OK;
}

void ReadTask(struct clave *punt_cla, int modo, int *e)
{ 
	int j;

	/* hago el locate para saber de donde leer la tarea */
	j = Locate((char*)punt_cla, (char*)&vector[0], cantreg, 
								 sizeof(struct clave), (FP)Comp);

	/* segun el modo de lectura opero */
	switch (modo) {
	case POR_IGUAL:
		if (j < 0) 
			*e = 1;
		else {
			*punt_cla = vector[j];
			*e = 0;
		}
		break;
	case POR_MAYOR:
		if (j < 0 ) 
			j = ~j;
		else 
			j += 1;
		if (j == cantreg) 
			*e = 1;
		else {
			*punt_cla = vector[j];
			*e = 0;
		}
		break;
	case POR_MENOR:
		if (j < 0)
			j = (~j) - 1;
		else 
			j -= 1;
		if (j < 0) 
			*e = 1;
		else {
			*punt_cla = vector[j];
			*e = 0;
		}
	}
}

bool AddTask(struct clave *punt_cla)
{
	int j,i;


	/* hago el locate para saber donde colocar la tarea */
	j = Locate((char*)punt_cla, (char*)&vector[0], cantreg, 
								 sizeof(struct clave), (FP)Comp);

	/* luego opero segun resultado */
	if (j<0) {
		if (cantreg==(MAXCAR-1))
			return FALSE;
		j = ~j;	
		for (i=cantreg; i>j; i--)
			vector[i] = vector[i-1];
		cantreg++;
	}
	vector[j] = *punt_cla;
	return TRUE;
}

void DeleteTask(struct clave *punt_cla)
{
	int j,i;

	/* hago el locate para saber donde esta la tarea */
	j = Locate((char*)punt_cla, (char*)&vector[0], cantreg, 
								 sizeof(struct clave), (FP)Comp);
	/* opero segun resultado */
	if (j>=0) {
		for (i=j; i<cantreg && i<MAXTAREA; i++)
			vector[i] = vector[i+1];
		cantreg--;
	}
}

void CloseAgenda(char *tareas)
{
	int i,j;

	if (fp==NULL) return;

	fclose(fp);
	fp = fopen(tareas, "w");

	/* grabo en el archivo uno a uno todas las tareas del vector */
	for (i = 0; i < cantreg; i++) {
		fprintf(fp,"%c,%D,%T,%T,%2d,%-*.*s,%-5.5s,%-7.7s\n",
										  vector[i].tipo,
										  vector[i].fecha,
										  vector[i].hora,
						 				  vector[i].puntdat->dura,
										  vector[i].puntdat->alarma,
										  LGTAREA,
										  LGTAREA,
										  vector[i].puntdat->titulo,
										  vector[i].puntdat->semana,
										  vector[i].puntdat->dia);
		/* grabo datos de tareas reiterativas */
		for (j = 0; j < 24; j++)
			fprintf(fp,"%2d,",vector[i].puntdat->diario[j]);
		fprintf(fp,"%2d\n",vector[i].puntdat->diario[j]);
		j++;
		for (; j < 30; j++)
			fprintf(fp,"%2d,",vector[i].puntdat->diario[j]);
		fprintf(fp,"%2d,%D\n",vector[i].puntdat->diario[j],
							  vector[i].puntdat->a_alarm);

		for (j = 0;j < MAXENEDIA-1; j++)
			fprintf(fp,"%-2.2s",vector[i].puntdat->enedia[j]);
		fprintf(fp,"%-2.2s\n",vector[i].puntdat->enedia[MAXENEDIA-1]);

		for (j = 0; j < vector[i].puntdat->textsize; j++) {
			if (vector[i].puntdat->puntbuff[j] == '\0')
				vector[i].puntdat->puntbuff[j] = '\n';
		}
		/* ahora grabo la descripcion de la tarea */
		fwrite (vector[i].puntdat->puntbuff, 
										vector[i].puntdat->textsize,
										1,fp);
		free(vector[i].puntdat->puntbuff);
		free(vector[i].puntdat);
	}
	fclose(fp);
	fp = NULL;
}

/* 
Rutina de comparacion para el Locate
*/
static int Comp(struct clave *c1, struct clave *c2)
{ 
	if (c1->tipo > c2->tipo) 	return 1;

	if (c1->tipo < c2->tipo) 	return -1;

	if (c1->fecha > c2->fecha) 	return 1;

	if (c1->fecha < c2->fecha) 	return -1;

	if (c1->hora > c2->hora) 	return 1;

	if (c1->hora < c2->hora) 	return -1;

	return 0;
}

char *FTaskName(char *user, char *tareas)
{
	char HOME[128];

	/* Arma el path p/archivo de tareas en el directorio $HOME */
	strcpy(HOME, ReadEnv("HOME"));
	if (user) {
		if (strrchr(HOME, '/'))
			*(strrchr(HOME, '/')+1) = 0;
		strcat(HOME, user);
	}
	strcpy(tareas, HOME);

	return strcat(tareas, TAREAS);
}

void TodayTask(struct t_task *lista, int *dim)
{
	short d,m,y;
	struct clave aux;
	int ncomp = 0, err, i;
	int  hoja[6][7];

	DToDMY(Today(), &d, &m, &y);
	Cal(m, y, hoja);

	/* primero coloco las tareas reiterativas */
	aux.fecha= NULL_DATE;
	aux.hora = NULL_TIME;
	aux.tipo = TR;
	ReadTask(&aux, POR_MAYOR, &err);
	while (!err && aux.tipo==TR && ncomp<(*dim)) {
		if (Entra (Today(),hoja,aux.puntdat->semana,aux.puntdat->dia) ||
			Entra1(Today(),aux.puntdat->diario) || 
			Entra2(Today(),hoja, aux.puntdat->enedia)){
			(lista+ncomp)->hora   = aux.hora;
			(lista+ncomp)->alarma = aux.puntdat->alarma;
			strncpy((lista+ncomp)->titulo,aux.puntdat->titulo,LGTAREA);
			for (i = strlen((lista+ncomp)->titulo)-1;i>=0 &&
						(lista+ncomp)->titulo[i] == ' ';i--);
			(lista+ncomp)->titulo[i+1] = '\0';
			(lista+ncomp)->tipo   = aux.tipo;
			(lista+ncomp)->fecha  = aux.fecha;
			(lista+ncomp)->a_alarm= aux.puntdat->a_alarm;
			ncomp++;
		}
		ReadTask(&aux, POR_MAYOR, &err);
	}

	/* luego coloco las tareas temporales */
	aux.fecha= Today();
	aux.hora = NULL_TIME;
	aux.tipo = TT;
	ReadTask(&aux, POR_MAYOR, &err);
	while(!err && aux.fecha==Today() && aux.tipo==TT && ncomp<(*dim)){
		(lista+ncomp)->hora   = aux.hora;
		(lista+ncomp)->alarma = aux.puntdat->alarma;
		strncpy((lista+ncomp)->titulo,aux.puntdat->titulo,LGTAREA);
		for (i = strlen((lista+ncomp)->titulo)-1;i>=0 &&
					(lista+ncomp)->titulo[i] == ' ';i--);
		(lista+ncomp)->titulo[i+1] = '\0';
		(lista+ncomp)->tipo   = aux.tipo;
		(lista+ncomp)->fecha  = aux.fecha;
		(lista+ncomp)->a_alarm= aux.puntdat->a_alarm;
		ncomp++;
		ReadTask(&aux, POR_MAYOR, &err);
	}

	/* Ahora las ordeno */
	qsort((void *) lista, ncomp, sizeof(struct t_task),
			(IFPVCPVCP) Compar);
	
	/* entrego la cantidad real encontrada */
	*dim = ncomp;
}

void FindTask(void)
{
	char tareas[128];
	struct t_task lista[NTASK];
	int e, i, n_task = NTASK;
	TIME h = Hour();

	if (OpenAgenda(FTaskName(NULL, tareas), Today())==ERROR)
		return;

	TodayTask(lista, &n_task);

	for (i=0; i<n_task; i++) {
		struct clave aux1;
		TIME aux = lista[i].hora - (lista[i].alarma * 30);

		if (lista[i].alarma  == 0) continue;
		if (lista[i].a_alarm == Today()) continue;
		if (h >= aux && h <= lista[i].hora) {
			WiBeep();
			Warning("%.1T: %s",lista[i].hora,lista[i].titulo);

			/* actualizo la tarea */
			aux1.tipo  = lista[i].tipo;
			aux1.hora  = lista[i].hora;
			aux1.fecha = lista[i].fecha;
			ReadTask(&aux1, POR_IGUAL, &e);
			aux1.puntdat->a_alarm = Today();
			(void)AddTask(&aux1);
		}
	}

	CloseAgenda(tareas);
}

static int Compar(struct t_task *p1, struct t_task *p2)
{
	return p1->hora - p2->hora;
}

/*
Rutina que me indica dado el vector con los datos para calcular la
periodicidad diaria de una tarea reiterativa, si esta entra o no 
dentro de la fecha fecha_act
*/
bool Entra1(DATE fecha_act, int diario[])
{
	short d,m,y;
	int i;
	
	DToDMY(fecha_act, &d, &m, &y);

	for (i=0; i<31 ; i++) 
		if ( (short) diario[i] == d) 
			return TRUE;

	return FALSE;
}

/*
Rutina que me indica dado los vectores con los datos para calcular la
periodicidad semanal de una tarea reiterativa, si esta entra o no 
dentro de la fecha fecha_act
Datos importantes: Un mes puede tener 4 o 5 semanas.
				   La primer semana de un mes es la primera que comienza
				   en dia domingo.
				   La ultima semana de un mes puede comenzar en el y 
				   terminar en el proximo.
*/
bool Entra(DATE fecha_act, int hoja[][7], char semana[], char dia[])
{
	short d,m,y;
	int i, cantsem, semact, diact;
	int semant,csemant;
	bool sigo = FALSE;

	Numsem(fecha_act,hoja,&cantsem,&semact);

	/* verifico primero si esta en la semana del mes anterior */
	if (semact==0) {
		DToDMY(fecha_act-7, &d, &m, &y);
		Cal(m, y, hoja);
		Numsem(fecha_act,hoja,&csemant,&semant);
		DToDMY(fecha_act, &d, &m, &y);
		Cal(m, y, hoja);
	}

	/* verifico luego si esta en la semana de este mes */
	for (i=0;i<5 && semana[i]!='\0'; i++) {
		if (semana[i]=='*' || (semana[i] - '0')==(char)semact ||
			(semana[i]=='u' && (cantsem==semact || semact==0)) ||
			(semact==0 && (semana[i] - '0')==(char)csemant)) {
			sigo = TRUE;
			break;
		}	
	}
	/* luego verifico el dia */
	if (sigo) {
		if ((diact=fecha_act%7)==0) diact = 7;
		for (i=0;i<7 && dia[i]!=' ' && dia[i]!='\0'; i++)
			if (dia[i]=='*' || (dia[i] - '0')==(char)diact) 
				return TRUE;
	}
	return FALSE;
}

/* 
Retorna si la fecha_act esta dentro del dominio de la tarea 
reiterativa cuyo vector de datos es pasado como parametro.
Se convierte la fecha actual al par (orden, dia) y se trata de 
encontrar dentro de enedia.
*/
bool Entra2(DATE fecha_act, int hoja[][7], char enedia[][2])
{
	int i, semact, dia, pdia;

	/* obtengo la semana de la fecha act */
	Numsem(fecha_act, hoja, &i, &semact);

	/* primer dia del mes */
	if ((pdia=FirstMonthDay(fecha_act)%7)==0) pdia = 7;

	/* obtengo el dia */
	if ((dia=fecha_act%7)==0) dia = 7;

	/* hago la correccion de la semana */
	if (pdia!=7 && pdia<=dia) semact++;

	/* veo si coincide con los pares que me ingresaron */
	for (i=0;i<MAXENEDIA;i++)
		if ((enedia[i][0]-'0')==(char)semact && 
			(enedia[i][1]-'0')==(char)dia)
			return TRUE;

	return FALSE;
}

/*
Rutina que retorna la cantidad de samanas y el numero de semana 
de la fecha fecha_act
*/
static void Numsem(DATE fecha_act, int hoja[][7], int *cantsem,
		int *semact)
{
	short d,m,y;
	int c,f;

	DToDMY(fecha_act, &d, &m, &y);

	for (*cantsem = 0, f=0; f<6; f++)
		if (hoja[f][0]!=0) (*cantsem)++;

	for (f=0; f<6; f++) {
		for (c=0; c<7; c++)
			if (hoja[f][c]== (int) d) break;
		if (hoja[f][c]== (int) d) break;
	}
	if (hoja[0][0]==0) f--;
	*semact = f+1;
}

/*
Rutina que retorna en el parametro p el vector con los dias del mes
del los parametros m, y.
*/
void Cal(short m, short y, int p[][7])
{
	int j,f;
	DATE d = DMYToD(1, m, y);
	DATE l = LastMonthDay(d);

	for (f=0; f<6; f++) 
		for (j=0; j<7; j++)
			p[f][j] = 0;

	for (j=1,f=0;d<=l;d++,j++) {
		p[f][d%7] = j;
		if (d%7==6) f++;
	}
}
