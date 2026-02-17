/********************************************************************
* Copyright (c) 1987 InterSoft Co.  All Rights Reserved
* THIS IS UNPUBLISHED PROPRIETARY SOURCE CODE OF InterSoft Co.
* The copyright notice above does not evidence any
* actual or intended publication of such source code.
*
*
* $Id: opers.h,v 1.1.1.1 1996/04/16 18:51:38 gustavof Exp $
*
*
* DESCRIPTION:
*      Definitions for operators types. 
*
* Files that must be included before this:
* ----------------------------------------
*
*
*
*********************************************************************/


#ifndef _OPERS_H
#define _OPERS_H

/* Relational operators types */

typedef long relop_type;

#define VARIABLE_ARGS   0xFF
#define OperArgs(op)	((int)((unsigned long)op>>16))
#define Oper(op)   ((int)(op&0xFFFF))

#define	OP_NULL        	 (relop_type) 0

/* Relational operators */
#define	OP_EQ          	0x20001
#define	OP_NE          	0x20002
#define	OP_GT          	0x20003
#define	OP_LT          	0x20004
#define	OP_GE          	0x20005
#define	OP_LE          	0x20006
#define	OP_IN          0xFF0007
#define	OP_NOT_IN      0xFF0008
#define	OP_BETWEEN     	0x30009
#define	OP_NOT_BETWEEN 	0x3000A
#define	OP_LIKE        	0x2000B
#define	OP_NOT_LIKE    	0x2000C
#define	OP_ISNULL  		0x1000D
#define	OP_NOT_ISNULL  	0x1000E


/* Arithmetic Operators */
#define	OP_ADD    		0x20100
#define	OP_DIF    		0x20101
#define	OP_MULT    		0x20102
#define	OP_DIV    		0x20103
#define	OP_REM    		0x20104
#define	OP_POW    		0x20105
#define OP_UMIN			0x10106		/* Unary minus			*/
#define	OP_CAT			0x20107		/* Concatenaion of strings */
#define OP_SUBST		0x30108		/* Substring			*/
#define OP_SUBIND		0x20109		/* Reference to vector element */

/* Functions			*/

#define OP_SUM 		 	0x10200
#define OP_AVG 		 	0x10201
#define OP_COUNT 	 	0x10202
#define OP_MIN 		 	0x10203
#define OP_MAX 		 	0x10204
#define OP_RUNSUM 	 	0x10205
#define OP_RUNAVG 	 	0x10206
#define OP_RUNCOUNT	 	0x10207
#define OP_RUNMIN 	 	0x10208
#define OP_RUNMAX 	 	0x10209
#define OP_TRUNC		0x1020A
#define OP_ROUND		0x1020B
#define OP_ABS			0x1020C
#define OP_LENGTH 	 	0x1020D
#define OP_UPPER	 	0x1020E
#define OP_LOWER 	 	0x1020F

#define OP_DAYNAME 	 	0x10210
#define OP_MONTHNAME 	0x10211
#define OP_DAY 		 	0x10212
#define OP_MONTH 	 	0x10213
#define OP_YEAR 	 	0x10214
#define OP_IN_DESCR  	0x10215
#define OP_STRING		0x10216
#define OP_NUM			0x10217
#define OP_MINOR		0xFF0218
#define OP_MAJOR		0xFF0219

#define OP_NUMTOSTR		0x1021A
#define OP_DATETOSTR	0x1021B
#define OP_TIMETOSTR	0x1021C
#define OP_FLOATTOSTR	0x1022A

#define OP_ADJDEC		0x2021D
#define	OP_PUSHFLD 		0x2021E
#define	OP_PUSHSTR 		0x2021F

#define OP_INTBL		0x50220
#define OP_NOT_INTBL    0x50221
#define OP_INSCL		0x50222
#define OP_NOT_INSCL	0x50223
#define OP_ALLANY		0x50224
#define OP_NOT_ALLANY	0x50225
#define OP_EXISTS		0x50226
#define OP_NOT_EXISTS	0x50227
#define OP_SCLSUBQ		0x50228

#define	OP_KEYHELP		0x10229
#define OP_STRTONUM		0x10230
#define OP_STRTODATE	0x10231
#define OP_STRTOTIME	0x10232
#define OP_STRTOFLOAT	0x10233 
#define OP_INT			0x10234

/* Logical Operators */

#define	OP_OR    		0x20300
#define	OP_AND    		0x20301
#define	OP_NOT    		0x10302
#define OP_IF			0x30303

#endif
