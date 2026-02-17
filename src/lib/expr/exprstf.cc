/********************************************************************
* Copyright (c) 1994 InterSoft Co.  All Rights Reserved
* THIS IS UNPUBLISHED PROPRIETARY SOURCE CODE OF InterSoft Co.
* The copyright notice above does not evidence any
* actual or intended publication of such source code.
*
* $Id: exprstf.cc,v 1.1.1.1 1996/04/16 18:51:56 gustavof Exp $
*
* DESCRIPTION
*********************************************************************/

#include <ifound/expr.h>
#include <ifound/itypes.h>
#include <ifound/stdc.h>
#include <ifound/priv/gfuncs.h>

void Expr::store(FILE *f)
{
	// primero guardo el string de la expr
	stString(f, expr);

	// ahora guardo si la expresion es compleja
	stInt(f, Int(flags));
	
	// ahora va la longitud del vector de shorts
	stInt(f, len);

	union Datum buff; 

	if ((buff.s = vector)) {

		forever {

			// grabo el op
			stUnShort(f, *buff.s);

			if (*buff.s == INST_NULL)
				break;
				
			switch(*buff.s++) {
			case INST_STRING:
				stString(f, *buff.str);
				buff.str++;
				break;
			case INST_NUM:
				stNum(f, *buff.num);
				buff.num++;
				break;
			case INST_DATE: 
			case INST_TIME:
			case INST_INT:
				stInt(f, *buff.i);
				buff.i++;
				break;
	 		case INST_BOOL:
				stUnShort(f, *buff.s);
				buff.s++;
				break;
			}
		}
	}
}

