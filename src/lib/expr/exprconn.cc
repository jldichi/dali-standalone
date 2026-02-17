/********************************************************************
* Copyright (c) 1992 InterSoft Co.  All Rights Reserved
* THIS IS UNPUBLISHED PROPRIETARY SOURCE CODE OF InterSoft Co.
* The copyright notice above does not evidence any
* actual or intended publication of such source code.
*
* $Id: exprconn.cc,v 1.1.1.1 1996/04/16 18:51:56 gustavof Exp $
*
* DESCRIPTION
*********************************************************************/

#include <ifound/expr.h>
#include <ifound/itypes.h>
#include <essentia/connect.h>
#include <ifound/stdc.h>

Connection &operator<<(Connection &c, const Expr &e)
{
	//  primero mando el string con la expresion
	c << e.expr;

	// ahora van los flags de la expresion
	c << Int(e.flags);
	
	// ahora mando la expresion compilada
	Datum ptr;
	ptr.s = e.vector;

	if (ptr.s) {
		while (*ptr.s) {
			c << Int(*ptr.s);
			switch (*ptr.s++) {
			case INST_STRING:
				c << *ptr.str++;
				break;
			case INST_NUM:
				c << *ptr.num++;
				break;
			case INST_INT:
			case INST_TIME:
			case INST_DATE:
				c << *ptr.i++;
				break;
			case INST_BOOL:
				c << Int(*ptr.s++);
				break;
			}
		}
	}

	// mando el fin de la secuencia
	return c << Int(INST_NULL);
}

Connection &operator>>(Connection &c, Expr &e)
{
	// obtengo el string con la expresion
	c >> e.expr;

	Int i;
	String s;
	Num n;

	e.begin();
	
	// ahora los flags de la expresion
	c >> i;
	e.flags = ExprFlags(i);

	// ahora obtengo la expresion compilada
	Instruction inst;
	c >> i; inst = Instruction(i);

	while (inst) {
		switch (inst) {
		case INST_STRING:
			c >> s; 
			e.code(s);
			break;
		case INST_NUM:
			c >> n;
			e.code(n);
			break;
		case INST_INT:
			c >> i;
			e.code(i);
			break;
		case INST_TIME:
			c >> i;
			e.code(toTime(i));
			break;
		case INST_DATE:
			c >> i;
			e.code(toDate(i));
			break;
		case INST_BOOL:
			c >> i;
			e.code(bool(i));
			break;
		default:
			e.code(inst);
			break;
		}
		c >> i;	inst = Instruction(i);
	}

	// fin de la expresion
	e.code(inst);

	e.end();

	return c;
}
