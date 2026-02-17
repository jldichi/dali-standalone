/********************************************************************
* Copyright (c) 1987 InterSoft Co.  All Rights Reserved             
* THIS IS UNPUBLISHED PROPRIETARY SOURCE CODE OF InterSoft Co.      
* The copyright notice above does not evidence any                  
* actual or intended publication of such source code.               
*
* $Id: gndefs.h,v 1.3 2002/03/15 19:16:00 hardaiz Exp $
*
* DESCRIPTION
* General definitions.
* Interface & definition header for GN routines.
*
* Files that must be included before this:
* ----------------------------------------
* stdio.h
*********************************************************************/

#ifndef GNDEFS_H
#define GNDEFS_H

#include <ifound/stdc.h>

/* ++ Prototypes ++ */

#define	_DbErrmsg(msg)	__DbErrmsg(quote(msg))
#define	_FmErrmsg(msg)	__FmErrmsg(quote(msg))
#define	_GnErrmsg(msg)	__GnErrmsg(quote(msg))
#define	_RpErrmsg(msg)	__RpErrmsg(quote(msg))
#define	_StErrmsg(msg)	__StErrmsg(quote(msg))
#define	_WiErrmsg(msg)	__WiErrmsg(quote(msg))

/*
   This union is used to save in a file, pointers to a value table.
   In the file 'offset', is the offset relative to the begining of the 
   file. When you load it in memory 'strval' has the address
   for strings hence :

   x.strval	= x.offset + <address of value , tbltable>.
*/

typedef union {
	char	*strval;
	long	offset;
} value_type;

#ifdef __cplusplus
extern "C" {
#endif

	extern char	*__DbErrmsg(const char *msg);
	extern char	*__FmErrmsg(const char *msg);
	extern char	*__GnErrmsg(const char *msg);
	extern char	*__RpErrmsg(const char *msg);
	extern char	*__StErrmsg(const char *msg);
	extern char	*__WiErrmsg(const char *msg);
	extern void	_ConvHandler(char *function, char *reason);
	extern int 	CheckPerm(int level);
	extern int _OutOfRange(double val, int len);
	extern void _Banner(const char *prog, char *ideaVer);
	extern char *_IdeaVersion(char dbe);
	extern int sopen(char *p, int a, int s, int m);
	extern DLLIMPORT int _flsbuf(_FLSBUFPARAM1TYPE, FILE *);
	extern DLLIMPORT int _filbuf(FILE *);
	extern int chmod(char const *, ModeT) __THROW;
	extern int seteuid(UidT) __THROW;
	extern int setegid(GidT) __THROW;
	extern struct group *getgrgid(GidT);
	extern struct group *getgrnam(char const *g);
	extern int cftime(char *buf, CFTIMEPARAM2TYPE fmt, const long *clock);
	extern char *mktemp(char *) __THROW;
	extern DLLIMPORT char *gcvt(double, GCVTPARAM2TYPE, char*) __THROW;
	extern int tgetnum(char *x);
	extern char *tgetstr(char *x, char **buf);
	extern int tgetflag(char *x);
	extern int tgetent(char *a, char *b);
	extern char *tgoto(char *str, int c, int f);
	extern void tputs(const char *str, int n, IFP f);
	extern char *tparm(char *, int, ...);
	extern FILE *tmpfile(void);

#ifdef  __cplusplus
}
#endif

/* -- Prototypes -- */

#endif
