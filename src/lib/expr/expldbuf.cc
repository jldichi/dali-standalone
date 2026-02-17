/********************************************************************
* Copyright (c) 1992 InterSoft Co.  All Rights Reserved
* THIS IS UNPUBLISHED PROPRIETARY SOURCE CODE OF InterSoft Co.
* The copyright notice above does not evidence any
* actual or intended publication of such source code.
*
* $Id: expldbuf.cc,v 1.7 2008/07/04 21:36:22 hardaiz Exp $
*
* DESCRIPTION
*********************************************************************/

#include <string.h>
#include <ifound/expr.h>
#include <ifound/itypes.h>
#include <ifound/stdc.h>
#include <ifound/priv/gfuncs.h>

void Expr::load(void *b)
{
	if (len)
		destroy();

	const char *aux = (const char*)b;

	// leo el string de la expr.
	expr.load(aux);

	// ahora los flags de la expresion
	flags = (enum ExprFlags) Int(ldlong(aux));
	aux += sizeof(long);
	
	// ahora la longitud del vector
	len = ldlong(aux);
	aux += sizeof(long);

	if (len) {

		union Datum buff; 
		buff.s = vector = tr_new UnShort[len];
		String str;

		Int *funcNo = NULL;
		String *funcName = NULL;
		
		forever {

			// leo el op
			*buff.s = lduint(aux);	
			aux += sizeof(UnShort);

			// si es el op NULL termino
			if (*buff.s == INST_NULL)
				break;
				
			switch(*buff.s++) {
			case INST_STRING:
				mCopy(buff.str, &str, sizeof(String));
				buff.str->connect();
				buff.str->load(aux);
				funcName = buff.str;
				buff.str++;
				break;
			case INST_NUM:
				buff.num->load(aux);
				buff.num++;
				break;
			case INST_DATE: 
			case INST_TIME:
				*buff.i = ldlong(aux);
				aux += sizeof(long);
				buff.i++;
				break;
			case INST_INT:
				*buff.i = ldlong(aux);
				aux += sizeof(long);
				funcNo = buff.i++;
				break;
	 		case INST_BOOL:
				*buff.s = lduint(aux);
				aux += sizeof(UnShort);
				buff.s++;
				break;
			case INST_FUNCTION:
				// resolve each tr_new function
				assert(funcNo != NULL && funcName != NULL);
				*funcNo = addFunction(*funcName);
				funcNo = NULL;
				funcName = NULL;
				break;
			}
		}
	}
	stlen = Int(aux - (char*) b);
}

