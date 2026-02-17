/********************************************************************
* Copyright (c) 1991 InterSoft Co.  All Rights Reserved
* THIS IS UNPUBLISHED PROPRIETARY SOURCE CODE OF InterSoft Co.
* The copyright notice above does not evidence any
* actual or intended publication of such source code.
*
*
* $Id: instruct.h,v 1.1.1.1 1996/04/16 18:51:32 gustavof Exp $
*
*
* DESCRIPTION:
*
*      Definitions for operators types. 
*
*********************************************************************/

#ifndef INSTRUCT_H
#define INSTRUCT_H

#include <ifound.h>

#ifdef __GNUC__
#pragma interface
#endif


/* Relational operators types */

enum Instruction {
	INST_NULL,			/* Special operators */
	INST_PAD,
	INST_VOID,
	INST_ARGEND,
	INST_ARGSEPAR,

	INST_STRING,		/* Constants		 */
	INST_NUM,
	INST_TIME,
	INST_DATE,
	INST_BOOL,
	INST_INT,

	INST_HOUR,
	INST_TODAY,
	
	INST_FIELDVAL,
	INST_IDENTIFIER,
	INST_VARIABLE,	
		
	INST_GETENV,

	INST_FUNCTION,

	INST_EQ, 			/* Relational operators	*/
	INST_NE,
	INST_GT,
	INST_LT,
	INST_GE,
	INST_LE,
	INST_IN,
	INST_BETWEEN,

	INST_OR, 		/* Logic Operators 	*/
	INST_AND,
	INST_NOT,
	INST_IF,
	INST_WHEN,

	INST_ADD, 		/* Arithmetic Operators */
	INST_DIF,
	INST_MULT,
	INST_DIV,
	INST_UMIN,

	INST_MSG,
};

#endif
