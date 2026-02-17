/********************************************************************
* Copyright (c) 1987 InterSoft Co.  All Rights Reserved
* THIS IS UNPUBLISHED PROPRIETARY SOURCE CODE OF InterSoft Co.
* The copyright notice above does not evidence any
* actual or intended publication of such source code.
*
*
* $Id: wmdef.h,v 1.1.1.1 1996/04/16 18:51:37 gustavof Exp $
*
*
* DESCRIPTION:
*      Header del WM
*
* Files that must be included before this:
* ----------------------------------------
* wi.h
*
*
*********************************************************************/

#if HAVE_PRAGMA_PACK
#	pragma pack(4)
#endif

/*
 * This union hold a parameter received by the WM
 * from a user process
 */
union param{
	int i;
	long l;
	char *s;
};

/*
 * This is the structure of a process. The process is a
 * UNIX process that comunicates with the WM.
 */
struct process {
	int pid;
	char name[MAX_PROCESS_NAME];
	char module[MAX_MODULE_CHARS];
	char version[MAX_VERSION_CHARS];
	bool interrupts;
	struct process *prev;
};

/*
 * A task is the parent of a process. Tipycally the
 * menu utility is a task. There can be many tasks, but
 * only one 'active'. 
 */
#define PAR_BUF_SIZ 126
struct task {
	struct process *p;
	union param parameters[10];
	char buff[PAR_BUF_SIZ];
	FILE *userin, *userout;
};

/* Maximun number of tasks			*/
#define MAX_TASKS	4

/* Code								*/
#define NEW_TASK	2


#ifdef WMTASK
struct task tasks[MAX_TASKS], *ptask, *active_task;
jmp_buf before_wrk, begin_wrk, on_wrk;
#else
extern struct task tasks[], *ptask, *active_task;
extern jmp_buf before_wrk, begin_wrk, on_wrk;
#endif

#if HAVE_PRAGMA_PACK
#	pragma pack()
#endif

