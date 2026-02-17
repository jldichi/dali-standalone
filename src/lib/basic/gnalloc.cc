/********************************************************************
* Copyright (c) 1987 InterSoft Co.  All Rights Reserved
* THIS IS UNPUBLISHED PROPRIETARY SOURCE CODE OF InterSoft Co.
* The copyright notice above does not evidence any
* actual or intended publication of such source code.
*
* $Id: gnalloc.cc,v 1.3 2007/03/07 18:47:05 hardaiz Exp $
*
* DESCRIPTION
*
* ROUTINE        |  PURPOSE
*----------------+---------------------------------------------------
* alloc_default  | Imprime mensaje de Error, y fin programa 
* SetAllocHandler| Define a user defined alloc handler. 
* def_Alloc      | Function used to obtain Memory from O.S.
*********************************************************************/

#include <string.h>
#ifdef _AIX
#include <strings.h>
#endif
#include <stdlib.h>

#include <ideafix.h>
#include <ideafix/priv/gndefs.h>
#include <ifound/priv/macros.h>

/* ++ Prototypes ++ */

static void alloc_default(char *filename, int lineno, unsigned int n);

/* -- Prototypes -- */


#ifdef CHK_HEAP
int mem_debug = 0;

static void *first = NULL;
static long nblock = 0;

struct header {
	struct header *p;
};

static void showpool(ptr)
struct header *ptr;
{
	struct header *aux;
	unsigned flg;

	aux = ptr = ptr->p;
	do {

		flg = (unsigned) aux & 1;
		aux = (struct header *)((char *) aux - flg);
		(void) fprintf(stderr, "%8lx %d\n", aux,flg);
		aux = aux->p;
	} while (aux != ptr);
	fprintf(stderr, "\n");
}

extern "C" void chkheap()
{
	if (mem_debug && first)
		showpool((struct header *) ((char *)first-sizeof(first)));
}
#endif

#ifdef NEED_CLOSE_FOR_SYNC
static void _flush(FILE *stream)
{
     int duphandle;

     /* flush the stream's internal buffer */
     fflush(stream);

     /* make a duplicate file handle */
     duphandle = dup(fileno(stream));

     /* close the duplicate handle to flush\
        the DOS buffer */
     close(duphandle);
}
#endif

/*
 * Imprime mensaje de Error, y fin programa
 */
static void alloc_default(char *filename, int lineno, unsigned n)
{
	char *msg = _GnErrmsg(NOMEM);
	if (msg[0] == '@')
		msg = "%s(%d) : No more memory.  Need %d bytes";
#if IDEAWIN
	WiDialog(WD_OK|WD_ERROR, 0, "Error", msg, filename, lineno, n);
#else
	fprintf(stderr, msg, filename, lineno, n);
	getchar();
#endif
	stop(1 | ABNORMAL_STOP);
}

/* Public variables defined in this module                          */

static FPCPIU alloc_handler_ = NULL;

inline FPCPIU alloc_handler()
{
	if (alloc_handler_ == NULL)
		alloc_handler_ = alloc_default;
	return alloc_handler_;
}

inline void alloc_handler(FPCPIU ah)
{
	alloc_handler_ = ah;
}

extern "C" FPCPIU SetAllocHandler(FPCPIU f)
{
	FPCPIU aux = alloc_handler();
	alloc_handler(f);
	return aux;
}

#ifdef NEED_MEM_DEBUG
static unsigned long _TotAllocMem = 0;
static bool init_debug = FALSE;
#endif
static bool AllocDebug = FALSE;
static FILE *alloc_log = NULL;

extern "C" void SetAlloc(bool flag)
{
	if (flag && alloc_log == NULL) {
		alloc_log = fopen("alloc.log", "w");
		flag = alloc_log != NULL;
	}
	AllocDebug = flag;
}

extern "C" void *def_Alloc(unsigned nbytes, char *filename, int lineno)
{
	static bool indef_Alloc = FALSE;
	void *p;
	
	/* Corrupted Alloc heap */
	if (indef_Alloc)
		alloc_default(filename, lineno, nbytes);

    indef_Alloc = TRUE;
    
#ifdef NEED_MEM_DEBUG

	if ( !init_debug ) {
		init_debug = TRUE;
		if ( getenv("DEBUG_MEMORY") ) SetAlloc(TRUE);
		else SetAlloc(FALSE);
	}
	if (AllocDebug) {
		_TotAllocMem += nbytes;
		(void) fprintf(alloc_log,
		   "+ %-20s, %4d, %5lu, %7lu, %7lu",
		   filename, lineno, (long)nbytes, _TotAllocMem, farcoreleft());
		_flush(alloc_log);
	}
#endif
	
	/*	this wierd thing is because ANSI does not define the
		behaviour of malloc when asking for 0 bytes of memory
		so it's implementation dependant, some return NULL, and
		others return a valid pointer */

	if (!nbytes) nbytes++;
#ifndef DOSDEB
	p = malloc(nbytes);
#else
	p = malloc(nbytes+4);
	*((long *) p) = MEMCHECK;
	((long *) p)++;
#endif

#ifdef C_HEAP
	if (first == NULL) first = p;
#endif

	if (p) {
		mZero(p, nbytes);
#ifdef NEED_MEM_DEBUG
		if (AllocDebug) {
			fprintf(alloc_log, "%9lx\n", p);
			_flush(alloc_log);
		}
#endif
	}
	else if (alloc_handler() != NULL)
		alloc_handler()(filename, lineno, nbytes);

	indef_Alloc = FALSE;
	
	return p;
}


extern "C" void def_Free(void *p, char *filename, int lineno)
{
#ifdef NEED_MEM_DEBUG
	if (AllocDebug) {
#ifdef HAVE_HEAP_WALK
		struct heapinfo hi;
		int found = 0;

		/* buscar el pointer que se va a liberar y ver el tamano */
		hi.ptr = NULL;
		while (heapwalk(&hi) == _HEAPOK) {
			if (hi.ptr == p) {
				found = 1;
				break;
			}
		}

		if (found) {
			_TotAllocMem -= hi.size;
			fprintf(alloc_log,
				   "- %-20s, %4d, %5lu, %7lu, %7lu, %9lx\n",
				   filename, lineno, hi.size, _TotAllocMem, p);
		}
		else {
			fprintf(alloc_log,
				   "- %-20s, %4d, Trying to free an unmalloced pointer\n",
				   filename, lineno);
		}
#else
		fprintf(alloc_log,
		   "- %-20s, %4d, %7lu, %9lx\n",
		   filename, lineno, _TotAllocMem, p);
#endif
		_flush(alloc_log);
	}
#endif

	if (p) {
#ifdef DOSDEB
			if (*(--((long *) p)) == MEMCHECK) {
				*((long *) p) = BAD_MEM_BLOCK;
				free(p);
			}
#ifdef DOSDEB1
			else {
					if (*((long *) p) == BAD_MEM_BLOCK)
		 				printf("Block already Free %s %d\n", filename,
		 						lineno);
					else
		 				printf("Free Error %s %d\n", filename, lineno);

					fflush(stdout);
					/*
					 * Don't let crash my program
					 * free(p);
					 */
			}
#endif
#else
		 free(p);
#endif
	}
}

#ifdef DOSDEB1
extern "C" void def_MemChk(void *p, char *pointer, char *filename, int lineno)
{
	if (p) {
		if (*(--((long *) p)) == MEMCHECK) {
			printf("Pointer %s %d %s %x is OK\n", filename, lineno,
					pointer, p);
			return;
		}
		if (*((long *) p) == BAD_MEM_BLOCK)
			printf("Block already Free (on check) %s %d pointer %s %x\n",
					filename, lineno, pointer, p);
		else
			printf("Free Error (on check) %s %d pointer %s %x\n",
					filename, lineno, pointer, p);
	}
	else
		printf("MemChk : NULL POINTER (%s - %d) pointer %s %x\n",
				filename, lineno, pointer, p);
}
#endif
