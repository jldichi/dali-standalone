/***********************************************************************
* Copyright (c) 1991 InterSoft Co.  All Rights Reserved
* THIS IS UNPUBLISHED PROPRIETARY SOURCE CODE OF InterSoft Co.
* The copyright notice above does not evidence any
* actual or intended publication of such source code.
*
* $Id: num.h,v 1.2 1999/07/23 20:39:25 eduardoc Exp $
*
* DESCRIPTION
* Class definitions for Num class.
*********************************************************************/

#ifndef	NUM_H
#define	NUM_H

#include <ifound/osdep.h>

#if HAVE_PRAGMA_PACK
#	pragma pack(4)
#endif

#define NUM_SHORTS	7
#define DIGITS_BASE	4
#define NUM_BASE	10000
#define MAX_EXP		100
#define BASE_EXP	50

/* NumToStr default length */
#define NTSDEF		255

#define NUM_STORE_LEN	(NUM_SHORTS*sizeof(short)+sizeof(short))

#ifdef __cplusplus
extern "C" {
#endif

typedef struct NUM {
	short s[NUM_SHORTS];
	signed char exp;
	char len;
} NUM;

/* ++ Prototypes ++ */

/* Operations on NUMs */

extern NUM *NumAdd(NUM *a, const NUM *b);
extern NUM *NumAddL(NUM *a, const Int b);
extern NUM *NumAddF(NUM *a, const double b);

extern NUM *NumSub(NUM *a, const NUM *b);
extern NUM *NumSubL(NUM *a, const Int b);
extern NUM *NumSubF(NUM *a, const double b);

extern NUM *NumMul(NUM *a, const NUM *b);
extern NUM *NumMulL(NUM *a, const Int b);
extern NUM *NumMulF(NUM *a, const double b);

extern NUM *NumDiv(NUM *a, const NUM *b);
extern NUM *NumDivL(NUM *a, const Int b);
extern NUM *NumDivF(NUM *a, const double b);

extern Int NumCmp(const NUM *a, const NUM *b);
extern Int NumCmpL(const NUM *a, const Int b);
extern Int NumCmpF(const NUM *a, const double b);

extern NUM *NumShift(NUM *a, Int i);
extern void NumRound(NUM *a, Int ndec);
extern void NumTrunc(NUM *a, Int ndec, short up);
extern Int NumNthDig(const NUM *a, Int n);

/* Conversions */

extern void ByteToNum(signed char c, NUM *n);
extern void IToNum(short i, NUM *n);
extern void LToNum(Int l, NUM *n);
extern void FToNum(double f, NUM *n);
extern void StrToNum(const char *s, NUM *n);
extern Int NumToL(const NUM *n);
extern double NumToF(const NUM *n);
extern const char *NumToStr(const NUM *num, Int n, Int precision);

/* -- Prototypes -- */

#define Nabs(pa) ((pa)->exp = (pa)->exp<0 ? -(pa)->exp : (pa)->exp)
#define NumIsNull(pa)	((pa)->exp == -MAX_EXP ? (pa)->len == 0 : 0)
#define NumIsMin(pa)	NumIsNull(pa)
#define NumIsMax(pa)	((pa)->exp == MAX_EXP && (pa)->len == 0)
#define NumIsZero(pa)	((pa)->exp == 0 && (pa)->len == 0)
#define DoNumNeg(pa)	((pa)->exp = -(pa)->exp, pa)
#define DoNumNull(pa)	((pa)->exp = -MAX_EXP, (pa)->len = 0, pa)
#define DoNumMax(pa)	((pa)->exp = MAX_EXP , (pa)->len = 0, pa)
#define DoNumMin(pa)	(DoNumNull(pa))

#ifdef __cplusplus
}
#endif

#if HAVE_PRAGMA_PACK
#	pragma pack()
#endif

#endif
