/********************************************************************
* Copyright (c) 1992 InterSoft Co.  All Rights Reserved
* THIS IS UNPUBLISHED PROPRIETARY SOURCE CODE OF InterSoft Co.
* The copyright notice above does not evidence any
* actual or intended publication of such source code.
*
* $Id: expr.cc,v 1.4 2002/03/15 18:56:42 hardaiz Exp $
*
* DESCRIPTION:
*********************************************************************/

#ifdef __GNUC__
#pragma implementation
#endif

#include <ifound/expr.h>
#include <ifound/itypes.h>
#include <ifound/stdc.h>

/*
	Information format within compiled expression array.

  1 short  2 shorts 1 short                          1 short   1 short 1 short
 +--------+--------+----------+---------+-----------+---------+-------+---------+
 |INST_INT| NFIELD |INST_FIELD|OPERAND1 |[OPERAND2] |OPERATION| [NOT] |INST_NULL|
 +--------+--------+----------+---------+-----------+---------+-------+---------+
 0 ..... n

  OPERAND = {(INST_(STRING|NUM|DATE|TIME|INT|BOOL)+DATO)|INST_(HOUR|TODAY)}

 */
declare(PtrArray, ExprFunction);
implement(PtrArray, ExprFunction);
declare(PtrArrayCursor, ExprFunction);
implement(PtrArrayCursor, ExprFuncion);

Expr::Expr()
:	funcs_(NULL),
	expr(NULL_STRING)
{
	assert((sizeof(String)%sizeof(UnShort)) == 0 &&
		   (sizeof(Num)%sizeof(UnShort)) == 0);
	

	vector  	= NULL ;
	len     	= 0;
	stlen   	= 0;
	flags		= (enum ExprFlags) 0;
	sval		= NULL;
	msgChecker	= NULL;
}

void Expr::resolveOthers(Instruction)
{ 
}

void Expr::connectStrings()
{
	if (vector) {
		Datum buff; 
		buff.s = vector;
		while (*buff.s) {
			switch(*buff.s++) {
			case INST_STRING:
				buff.str->connect();
				buff.str++;
				break;
			case INST_NUM:
				buff.num++;
				break;
			case INST_DATE: 
			case INST_TIME:
			case INST_INT:
				buff.i++;
				break;
	 		case INST_BOOL:
				buff.s++;
				break;
			}
		}
	}
}

void Expr::disconnectStrings()
{
	if (vector) {
		Datum buff; 
		buff.s = vector;
		while (*buff.s) {
			switch(*buff.s++) {
			case INST_STRING:
				buff.str->disconnect();
				buff.str++;
				break;
			case INST_NUM:
				buff.num++;
				break;
			case INST_DATE: 
			case INST_TIME:
			case INST_INT:
				buff.i++;
				break;
	 		case INST_BOOL:
				buff.s++;
				break;
			}
		}
	}
}

void Expr::destroy()
{
	if (vector) {
		disconnectStrings();
		tr_delete_vec(vector);
		vector = NULL;
		len = 0;
		stlen = 0;
		flags = (enum ExprFlags) 0;
		if (sval) {
			tr_delete(sval);
			sval = NULL;
		}
	}
}

Expr::~Expr()
{
	destroy();
	deleteStack();
/*
	for (PtrArrayCursor(ExprFunction) curs(*funcs_); curs; ++curs) {
		if (*curs)
			tr_delete(*curs);
	}
	tr_delete(funcs_);
*/
}

bool Expr::operator !()
{
	return isNull();
}

Expr::operator int()
{
	return !isNull();
}

bool Expr::isNull()
{
	return vector == NULL || *vector == UnShort(INST_NULL);
}

Int Expr::mapField(const String &)
{
	return ERR;
}

Int Expr::mapVariable(const String &)
{ 
 	return ERR;
}

Instruction Expr::execute(Datum &ptr)
{
	while (*ptr.s != INST_NULL && *ptr.s != INST_ARGSEPAR && 
		   *ptr.s != INST_ARGEND) {
		Instruction op;
		switch (op = Instruction(*ptr.s++)) {
		case INST_NUM:
			push(*ptr.num++);
			break;
		case INST_STRING:
			push(*ptr.str++);
			break;
		case INST_TIME:
			push(toTime(*ptr.i++));
			break;
		case INST_DATE:
			push(toDate(*ptr.i++));
			break;
		case INST_INT:
			push(Num(*ptr.i++));
			break;
		case INST_GETENV:
			getEnv();
			break;
		case INST_TODAY:
			execToday();
			break;
		case INST_HOUR:
			execHour();
			break;
		case INST_VOID:
			push(AnyValue());
			break;
		case INST_IN:
			in();	
			break;
		case INST_NOT:
			Not();
			break;
		case INST_OR:
			Or();
			break;
		case INST_AND:
			And();
			break;
		case INST_BETWEEN:
			between();
			break;
		case INST_EQ:
			equal();
			break;
		case INST_NE:
			notEqual();
			break;
		case INST_GT:
			greater();
			break;
		case INST_LT:
			lessThan();
			break;
		case INST_GE:
			greaterOrEqual();
			break;
		case INST_LE:
			lessOrEqual();
			break;
		case INST_ADD:
			add();
			break;
		case INST_DIF:
			diff();
			break;
		case INST_MULT:
			mult();
			break;
		case INST_DIV:
			div();
			break;
		case INST_UMIN:
			umin();
			break;
		case INST_IF:
			conditionalIf();
			break;
		case INST_WHEN:
			when();
			break;
		case INST_PAD:
			break;
		case INST_FUNCTION:
			execFunction(ptr);
			break;
		case INST_MSG:
			execMsg(ptr);
			break;
		default:
			resolveOthers(op);
			break;			
		}
	}
	UnShort ret = *ptr.s;
	
	if (*ptr.s == INST_ARGSEPAR)
		ptr.s++;

	return Instruction(ret);
}

AnyValue Expr::eval()
{
	if (isNull())
		return NULL_STRING;

	if (sval)
		return *sval;
		
	resetStack();
	
	Datum ptr;
	ptr.s = vector;
	execute(ptr);
	const AnyValue &aux = top();
	if (!isComplex()) {
		sval  = tr_new AnyValue;
		*sval = aux;
	}

	return aux;
}

Int Expr::addFunction(const String &fname, FPES f, BFPES r)
{
	String fn = fname.toLower();
	for (PtrArrayCursor(ExprFunction) curs(*funcs_); curs; ++curs) {
		if (curs->name == fn) {
			if (f) curs->fp = f;
			if (r) curs->resolve = r;
			return funcs_->find(*curs);
		}
	}
	return funcs_->add(tr_new ExprFunction(fn, f, r));
}

ReportExpr *Expr::getReportExpr()
{
	return NULL;
}

FormExpr *Expr::getFormExpr()
{
	return NULL;
}

FmExpr *Expr::getFmExpr()
{
	return NULL;
}

bool Expr::isMsgTab(const String &id)
{
	return msgChecker != NULL && msgChecker(toCharPtr(id)); 
}

#ifdef DEBUG
#include <stdio.h>
void Expr::debugExpr()
{
	String aux = "nil";

	if (vector) {
		aux = NULL_STRING;
		short  s;
		Datum buff; 
		buff.s = vector;

		while (*buff.s) {
			switch(s = *buff.s++) {
			case INST_STRING:
				aux += "\"";
				aux += *buff.str++;
				aux += "\"";
				break;
			case INST_NUM:
				aux += toString(*buff.num++);
				break;
			case INST_DATE: 
				aux += toString(toDate(*buff.i++));
				break;
			case INST_TIME:
				aux += toString(toTime(*buff.i++));
				break;
			case INST_INT:
				aux += toString(*buff.i++);
				break;
	 		case INST_BOOL:
				aux += toString(*buff.s++);
				break;
			default:
				aux += "INST(";
				aux += toString(s);
				aux += ")";
			}
			aux += " -- ";
		}
	}
	fprintf(stderr, "'%s'\n", toCharPtr(aux));
}
#endif
