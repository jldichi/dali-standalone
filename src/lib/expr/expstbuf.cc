/********************************************************************
* Copyright (c) 1992 InterSoft Co.  All Rights Reserved
* THIS IS UNPUBLISHED PROPRIETARY SOURCE CODE OF InterSoft Co.
* The copyright notice above does not evidence any
* actual or intended publication of such source code.
*
* $Id: expstbuf.cc,v 1.1.1.1 1996/04/16 18:51:56 gustavof Exp $
*
* DESCRIPTION
*********************************************************************/

#include <ifound/expr.h>
#include <ifound/itypes.h>
#include <ifound/stdc.h>
#include <ifound/priv/gfuncs.h>

Int Expr::store(void *b)
{
	char *aux = (char*)b;

	// primero guardo el string de la expr
	expr.store(aux);

	// ahora guardo si la expresion es compleja
	stlong(long(flags), aux);
	aux += sizeof(long);
	
	// ahora va la longitud del vector de shorts
	stlong(len, aux);
	aux += sizeof(long);

	union Datum buff; 
	if ((buff.s = vector)) {

		forever {

			// grabo el op
			stuint(*buff.s, aux);	
			aux += sizeof(UnShort);

			if (*buff.s == INST_NULL)
				break;
				
			switch(*buff.s++) {
			case INST_STRING:
				buff.str->store(aux);
				buff.str++;
				break;
			case INST_NUM:
				buff.num->store(aux);
				buff.num++;
				break;
			case INST_DATE: 
			case INST_TIME:
			case INST_INT:
				stlong(*buff.i, aux);
				aux += sizeof(long);
				buff.i++;
				break;
	 		case INST_BOOL:
				stuint(*buff.s, aux);
				aux += sizeof(UnShort);
				buff.s++;
				break;
			}
		}
	}
	return stlen;
}

