/********************************************************************
* Copyright (c) 1994 InterSoft Co.  All Rights Reserved
* THIS IS UNPUBLISHED PROPRIETARY SOURCE CODE OF InterSoft Co.
* The copyright notice above does not evidence any
* actual or intended publication of such source code.
*
* $Id: exprldf.cc,v 1.3 2007/03/07 15:42:11 hardaiz Exp $
*
* DESCRIPTION
*********************************************************************/

#include <string.h>
#ifdef _AIX
#include <strings.h>
#endif

#include <ifound/expr.h>
#include <ifound/itypes.h>
#include <ifound/stdc.h>
#include <ifound/priv/gfuncs.h>

void Expr::load(FILE *f)
{
	if (len)
		destroy();

	stlen = 0;
	
	// leo el string de la expr.
	expr = ldString(f);
	stlen += expr.storeLength();
	
	// ahora los flags de la expresion
	flags = (enum ExprFlags) ldInt(f);
	stlen += sizeof(Int);
		
	// ahora la longitud del vector
	len = ldInt(f);
	stlen += sizeof(Int);
	
	if (len) {

		union Datum buff; 
		buff.s = vector = tr_new UnShort[len];
		String str;

		Int *funcNo = NULL;
		String *funcName = NULL;

		forever {

			// leo el op
			*buff.s = ldUnShort(f);	
			stlen += sizeof(UnShort);
			
			// si es el op NULL termino
			if (*buff.s == INST_NULL)
				break;
				
			switch(*buff.s++) {
			case INST_STRING:
				mCopy(buff.str, &str, sizeof(String));
				buff.str->connect();
				*buff.str = ldString(f);
				stlen += buff.str->storeLength();
				funcName = buff.str++;
				break;
			case INST_NUM:
				*buff.num = ldNum(f);
				stlen += buff.num->storeLength();
				buff.num++;
				break;
			case INST_DATE: 
			case INST_TIME:
				*buff.i = ldInt(f);
				stlen += sizeof(Int);
				buff.i++;
				break;
			case INST_INT:
				*buff.i = ldInt(f);
				stlen += sizeof(Int);
				funcNo = buff.i++;
				break;
	 		case INST_BOOL:
				*buff.s = ldUnShort(f);
				stlen += sizeof(UnShort);
				buff.s++;
				break;
			case INST_FUNCTION:
				// resolve each new function
				assert(funcNo != NULL && funcName != NULL);
				*funcNo = addFunction(*funcName);
				funcNo = NULL;
				funcName = NULL;
				break;
			}
		}
	}
}


