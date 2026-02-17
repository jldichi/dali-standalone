/********************************************************************
* Copyright (c) 1987 InterSoft Co.  All Rights Reserved
* THIS IS UNPUBLISHED PROPRIETARY SOURCE CODE OF InterSoft Co.
* The copyright notice above does not evidence any
* actual or intended publication of such source code.
*
* $Id: ical.h,v 1.1.1.1 1996/04/16 18:51:38 gustavof Exp $
*
* DESCRIPTION
* Agenda/calendar application
*********************************************************************/

/* message configuration */
#define	IC_LEYCAL	1
#define	IC_LEYLAY	2
#define	IC_LEYPOP	3
#define	IC_LEYHLP	4
#define IC_HLPHOR	5
#define IC_HLPREI	6
#define IC_HLPTAR	7
#define IC_HLPPOP	8
#define IC_HLPHLP	9
#define IC_HLPFER	10	
#define IC_HLPPEN	11	
#define IC_ERRSUP	12	
#define IC_YES		13	
#define IC_NO		14	
#define IC_TASK		15	
#define IC_ERRFIL	16	
#define IC_ERROVR	17	


/*********************    CONFIGURACION    ***************************/

/************   PARA EL MANEJO DEL ARCHIVO DE TAREAS   ***************/
#define  MAXCAR    78  /* cant. de carac. x linea del arch. tareas   */
#define  MAXTAREA 100  /* cantidad maxima de tareas                  */
#define  TAREAS   "/tareas.dat"  /* Arch. de tareas		 			 */
#define  FIN_DESC  1   /* carac. que indica fin de descri. de tareas */
#define  POR_IGUAL 2   /* modo por igual para el ABM de tareas       */
#define  POR_MAYOR 3   /* modo por mayor para el ABM de tareas 		 */ 
#define  POR_MENOR 4   /* modo por menor para el ABM de tareas		 */
#define	 DIA_DEPUR 35  /* cantidad de dias para la depuracion		 */

/***********************    TIPOS DE TAREAS   ************************/
#define  TT		  't'  /* tareas temporales   						 */
#define  TP		  'p'  /* tareas pendientes							 */
#define  TR		  'r'  /* tareas reiterativas						 */

/**************************    VENTANAS          *********************/
#define  LARG_CAL  9   				  /* largo del calendario		 */
#define  ANCH_CAL  21  				  /* ancho del calendario   	 */
#define  LARG_LAY  14  				  /* largo del layout			 */
#define  ANCH_LAY  54  				  /* ancho del layout		     */
#define  LARG_POP  10				  /* largo del popup			 */
#define  ANCH_POP  45  				  /* ancho del popup			 */
									  /* leyenda de tareas pend.     */
#define  LARG_HLP  7   				  /* largo del help				 */
#define  ANCH_HLP  41  				  /* ancho del help				 */

/**************************    LONGITUDES        *********************/
#define  LGTAREA   30  /* logitud del titulo de la tarea    		 */ 
#define  MAXENEDIA 35  /* cantidad maxima de pares Orden-Dia p/reiter*/
#define  NTASK	   20  /* cantidad de tareas diarias				 */

/*
 * struct que forma parte de la tabla en memoria del archivo de tareas
 */
struct datos {
	TIME dura;
	int  alarma;
	char titulo[LGTAREA+1];
	char semana[5];
	char dia[7];
	int  diario[31];
	char enedia[MAXENEDIA][2];
	DATE a_alarm;
	int  textsize;
	char *puntbuff;
};

/* 
 * struct que conforma la tabla en memoria donde almaceno el 
 * archivo de tareas
 */
struct clave {
	char tipo;
	DATE fecha;
	TIME hora;
	struct datos *puntdat;
};

/* 
 * struct que contrendra las tareas de hoy (para alarma)
 */
struct t_task {
	char tipo;
	DATE fecha;
	TIME hora;
	int alarma;
	DATE a_alarm;
	char titulo[LGTAREA+1];
};

/* ++ Prototypes ++ */

char *_EMsg(int msg);
char * FTaskName(char *user, char *tareas);
int OpenAgenda(char *tareas, DATE today);
void AbmTareas(char tipo);
void AbmReiter(void);
void CloseAgenda(char *tareas);
void Cal(short  m, short y, int  p[][7]);
void ReadTask(struct clave *punt_cla, int modo, int *e);
bool AddTask(struct clave *punt_cla);
void DeleteTask(struct clave *punt_cla);
void TodayTask(struct t_task *lista, int *dim);
bool Entra1(DATE fecha_act, int  diario[]);
bool Entra(DATE fecha_act, int hoja[][7], char semana[], char dia[]);
bool Entra2(DATE fecha_act, int hoja[][7], char enedia[][2]);
void LlenarLayout(void);

/* -- Prototypes -- */

