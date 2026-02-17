/*********************************************************************
* Copyright (c) 1987 InterSoft Co.  All Rights Reserved
* THIS IS UNPUBLISHED PROPRIETARY SOURCE CODE OF InterSoft Co.
* The copyright notice above does not evidence any
* actual or intended publication of such source code.
*
*
* $Id: wmcom.h,v 1.5 2004/02/12 19:31:36 diegoa Exp $
*
*
* DESCRIPTION:
*      Definition & interface file for IPC
*
*********************************************************************/

#ifndef WMCOM_H
#define WMCOM_H

#ifdef HAVE_PRAGMA_PACK
#	define pack(4)
#endif

#define WM_DUMMY		 	 0
#define WM_STOP	 			 1
#define WM_WICREATE			 2
#define WM_WIDELETE			 3
#define WM_WIDELETEALL		 4
#define WM_WISWITCHTO		 5
#define WM_WIREFRESH		 6
#define WM_WIREDRAW			 7
#define WM_WISETBORDER		 8
#define WM_WISTATUSLINE		 9
#define WM_WIGETATTR		10
#define WM_WIORGCOL		 	11
#define WM_WIORGROW			12
#define WM_WILINE			13
#define WM_WICOL			14
#define WM_WIWIDTH			15
#define WM_WIHEIGHT			16
#define WM_WIINCHAR			17
#define WM_WIINATTR			18
#define WM_WIGETTAB			19
#define WM_WISETTAB			20
#define WM_WIPARENT			21
#define WM_WIFLUSH			22
#define WM_WIGETC			23
#define WM_WIGETS			24
#define WM_WIKEYHELP		25
#define WM_WIINTERRUPTS		26
#define WM_WICURRENT		27
#define WM_WICREATEPRO		28
#define WM_WIDELETEPRO		29
#define WM_WISETTTY			30
#define WM_WIRESETTTY		31
#define WM_WIGETFIELD		32
#define WM_WIAPLHELP		33
#define WM_WISETAPLHELP		34
#define WM_WIPROCESS		35
#define WM_WISETRAW			36
#define WM_WIKEYREADY		37
#define WM_WISETSERVICES	38
#define WM_WIMOVE			39
#define WM_WIRESIZE			40
#define WM_WISERVICE		41

/* +++IXSCA+++ */

#define WM_WIGETSESSION		42
#define WM_WISETSESSION		43

/* ---IXSCA--- */

#define WM_COM_BUFF_SIZE   1024

#ifdef __cplusplus
extern "C" {
#endif

extern union _WmPar {
	short		i;
	long		l;
	double		f;
	unsigned char *s;
	unsigned char u;
} DLLDEPEND *wm_par;

struct _wm_commands {
	void (*fp)(void);
	char *i_types;
	char *o_types;
	int  wm_rem;
};

DLLDEPEND extern FPI ExecWm;

/* ++ Prototypes ++ */

struct _wm_commands *_WmCommands(int *n);

/* -- Prototypes -- */

#ifdef __cplusplus
}
#endif

/* archivo p/retornar al shell */
#define RETSHELL "retshell"

/* resultados posibles del FindProc	*/
#define F_STOP		0
#define F_GETCMD	1
#define F_EXECMD	2

/* tipos de seleccion 	*/
#define SEL_NEW		0
#define SEL_SAME	1
#define SEL_BACKGR	2
#define SEL_SUSPEND	3

/* status de un proceso */
#define PROC_ACT	0
#define PROC_SUSP	1
#define PROC_WAIT	2
#define PROC_SHELL	3

/* define para resultado del mensajes interrumpidos (ipc) */
#define INTPTS 	1

/* longitud maxima para el nombre de un proceso */
#define LONG_NAME  			48
#define LONG_MODULE		 	16
#define LONG_VERSION		32

struct process {
	int pid;				/* pid del proceso 					*/
	int st;					/* status del proceso				*/
	struct process *parent;	/* puntero al proceso padre			*/
	window curr_win;		/* window activa del proceso		*/
	char name[LONG_NAME];	/* titulo de la tarea				*/
	char module[LONG_MODULE];	/* modulo de la tareas				*/
	char version[LONG_VERSION];	/* version de la tarea				*/
	char *buff;				/* puntero al buffer p/reestablecer	*/
	bool intpts;			/* tiene habilitado interrupts 		*/
	int  raw;				/* esta trabajando en modo raw		*/
};

extern struct process *curr_proc;	/* definda en wmtask.c		*/
extern int curr_pid;		/* definida en wmalone.c			*/
extern bool curr_int;		/* definida en wminit.c				*/
extern int curr_raw;		/* definida en wmgetc.c				*/

#ifdef HAVE_PRAGMA_PACK
#	define pack()
#endif

#endif
