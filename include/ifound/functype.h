/***********************************************************************
* Copyright (c) 1991 InterSoft Co.  All Rights Reserved
* THIS IS UNPUBLISHED PROPRIETARY SOURCE CODE OF InterSoft Co.
* The copyright notice above does not evidence any
* actual or intended publication of such source code.
*
* $Id: functype.h,v 1.1.1.1 1996/04/16 18:51:31 gustavof Exp $
*
* DESCRIPTION
* Function typedefs
*********************************************************************/

#ifndef FUNCTYPE_H
#define FUNCTYPE_H

#include <ifound/istdarg.h>

/* +++ Prototypes +++ */

#ifdef __cplusplus
extern "C" {
#endif

typedef void (*FP)(void);
typedef int (*IFP)(void);
typedef long (*LFP)(void);
typedef void (*FPI)(int);
typedef void (*FPICPVL)(int, char *, va_list);
typedef void (*FPCCPVL)(const char *, va_list);
typedef void (*FPVP)(void *);
typedef void (*FPCP)(char *);
typedef void (*FPC)(char);
typedef void (*FPCPCP)(char *, char *);
typedef void (*FPCPPI)(char **, int);
typedef void (*FPCPCPI)(char *, char *, int);
typedef void (*FPCPIU)(char*, int, unsigned);
typedef void (*FPLICP)(long, int, char *);
typedef void (*FPLCPCP)(long, char *, char *);
typedef void (*FCPVA)(char *, ...);
typedef int (*IFPCPCP)(char *, char *);
typedef int (*IFPICPVL)(int, char *, va_list);
typedef int (*IFPICCPVL)(int, const char *, va_list);
typedef int (*IFPCP)(char *);
typedef int (*IFPICP)(int, char*);
typedef int (*IFPCPCPI)(char*, char*, int);
typedef int (*IFPICPCP)(int, char*, char*);
typedef int (*IFPL)(long);
typedef int (*IFV)(void *);
typedef int (*IFCPVA)(char *, ...);
typedef int (*IFIVA)(int, ...);
typedef char *(*CPFPCP)(char *);
typedef char *(*CPFPCPI)(char *, int);
typedef char *(*CPFPCPCP)(char *, char *);
typedef char *(*CPFPCPCPI)(char *, char *, int);
typedef char *(*CPFPLCP)(long, char *);
typedef int (*EHAND)(int, int);
typedef int (*IFPI)(int);
typedef int (*IFPCCPCCP)(char const *, char const *);
typedef int (*IFPICCPCCP)(int, char const *, char const *);
typedef int (*IFPCCP)(char const *);
typedef int (*IFPCCPI)(char const *, int);
typedef char const *(*CCPFPCCP)(char const *);
typedef int (*IFPVCPVCP)(void const *, void const *);

#ifdef __cplusplus
}
#endif

/* --- Prototypes --- */

#endif
