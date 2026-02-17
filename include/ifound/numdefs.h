/***********************************************************************
* Copyright (c) 1991 InterSoft Co.  All Rights Reserved
* THIS IS UNPUBLISHED PROPRIETARY SOURCE CODE OF InterSoft Co.
* The copyright notice above does not evidence any
* actual or intended publication of such source code.
*
* $Id: numdefs.h,v 1.1.1.1 1996/04/16 18:51:32 gustavof Exp $
*
* DESCRIPTION
* Class definitions for Num class.
*********************************************************************/

#ifndef	NUMDEFS_H
#define	NUMDEFS_H


#include <ifound/num.h>

#ifdef __cplusplus
extern "C" {
#endif

/* ++ Prototypes ++ */

extern void NumShiftExp(const NUM *a, NUM *b, short places);
extern NUM *NumNorm(NUM *a);
extern void MakeNumFromLong(NUM *a, long j);
extern void NumExpFmt(const NUM *a, char *p, short n);
extern long NumTotDigits(const NUM *num, long *nint, long *ndec);
extern void stnum(NUM *a, char *p);
extern NUM* ldnum(NUM *a, char *p);

/* -- Prototypes -- */

#ifdef __cplusplus
}
#endif


/* 
	Nums are sent in the following way
	exponent, length, short0 = first Int 
	short1, short2 = second Int
	etc.
	This macro is used just for efficiency.
*/

#define SENDNUM(CONN, SENDINT, N)										\
	do {																\
		register const NUM *__n__ = (const NUM *) &(N);					\
		(CONN).SENDINT((Int(__n__->exp) << 24) |						\
					   (Int(__n__->len) << 16) |				  		\
					   (__n__->len ? __n__->s[0] : 0)); 				\
		Int j;															\
		for (j = 1; j + 1 < __n__->len; j += 2)							\
			(CONN).SENDINT(Int(__n__->s[j]) << 16 |						\
						   Int(__n__->s[j+1]));							\
																		\
		if (j < __n__->len)												\
			(CONN).SENDINT(Int(__n__->s[j]) << 16);						\
																		\
	} while (0)

#define RECEIVENUM(CONN, RECEIVEINT, N, CNT)							\
	do {																\
		register NUM *__n__ = (NUM *) &(N);								\
		Int i;															\
																		\
		/* Receive first int */											\
		(CONN).RECEIVEINT(i);											\
		if (i != SPECIAL_VALUE) {										\
    		CNT = ERR;													\
		} else {														\
			(CONN).RECEIVEINT(CNT);										\
			(CONN).RECEIVEINT(i);										\
		}																\
																		\
		__n__->exp = char(i >> 24);										\
																		\
		if (__n__->exp == -MAX_EXP || __n__->exp == 0 ||				\
				__n__->exp == MAX_EXP)									\
			__n__->len = 0;												\
		else {															\
			__n__->len = char((i >> 16) & 0x00ff);						\
			if (__n__->len)												\
				__n__->s[0] = short(i & 0x0000ffff);					\
			Int j;														\
			for (j = 1; j + 1 < __n__->len; j += 2) {					\
				(CONN).RECEIVEINT(i);									\
				__n__->s[j]   = short(i >> 16);							\
				__n__->s[j+1] = short(i & 0x0000ffff);					\
			}															\
																		\
			if (j < __n__->len) {										\
				(CONN).RECEIVEINT(i);									\
				__n__->s[j] = short(i >> 16);							\
			}															\
		}																\
	} while (0)

#endif
