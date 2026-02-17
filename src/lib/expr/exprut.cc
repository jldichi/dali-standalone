/********************************************************************
* Copyright (c) 1992 InterSoft Co.  All Rights Reserved
* THIS IS UNPUBLISHED PROPRIETARY SOURCE CODE OF InterSoft Co.
* The copyright notice above does not evidence any
* actual or intended publication of such source code.
*
* $Id: exprut.cc,v 1.4 2002/03/15 18:57:36 hardaiz Exp $
*
* DESCRIPTION
*********************************************************************/

#include <ifound/expr.h>
#include <ifound/itypes.h>
#include <ifound/stdc.h>
#include <ifound/msgtable.h>

#include <ifound/priv/gfuncs.h>

String argVal(Int n);

declare(PtrArray, ExprFunction);
implement(PtrArray, ExprFunction);
declare(PtrArrayCursor, ExprFunction);
implement(PtrArrayCursor, ExprFuncion);

#define STACK_LEN	1024

static AnyValue *_stack = NULL;
static AnyValue *_last = NULL;
static AnyValue *_endstack = NULL;

static void createStack() 
{
	if (!_stack) {
		_stack = tr_new AnyValue[STACK_LEN];
		_last = _stack;
		_endstack = _stack + STACK_LEN;
	}
}
//AUTUMN
static void cleanStack()
{
	if(_stack) {
		delete [] _stack;
		_stack = NULL;
		_last = NULL;
		_endstack = NULL;
	}
}

static AnyValue *&last() 
{
	createStack();
	return _last;
}

static AnyValue *&endstack() 
{
	createStack();
	return _endstack;
}

static AnyValue *&stack() 
{
	createStack();
	return _stack;
}

void Expr::resetStack()
{
	last() = stack();
}

AnyValue Expr::pop()
{
	assert(last() > stack());
	return *--last();
}

const AnyValue &Expr::top()
{
	assert(last() > stack());
	return *(last()-1);
}

void Expr::top(const AnyValue &av)
{
	assert(last() > stack());
	(*(last()-1)).set(av);
}

void Expr::push(const AnyValue &av)
{
	assert(last() < endstack());
    last()->set(av);
	last()++;
}

void Expr::execToday()
{
	push(Date(TODAY));
}

void Expr::execHour()
{
	push(Time(NOW));
}

void Expr::getEnv()
{
	String aux = toString(pop());
	
	if (isdigit(aux[0]))
		push(argVal(toInt(aux)));
	else
		push(aux.getEnv());
}

void Expr::in()
{
	bool res = false;

	Int l = toInt(pop());

	// (pablog)
	const AnyValue &av = *((&top()) - l);

	for (; l; l--)
		if (cmp(av, pop()) == 0)
			res = true;

	top(res);
}

void Expr::Not()
{
	bool cond = toBool(pop());
	push(!cond);
}

void Expr::Or()
{
	AnyValue av1 = pop();
	AnyValue av2 = pop();
	push(toBool(av1) || toBool(av2));
}

void Expr::And()
{
	AnyValue av1 = pop();
	AnyValue av2 = pop();
	push(toBool(av1) && toBool(av2));
}

void Expr::between()
{
	AnyValue op2 = pop();
	AnyValue op1 = pop();
	AnyValue val = pop();
	push(bool(cmp(val, op1) >= 0 && cmp(val, op2) <= 0));
}

void Expr::conditionalIf()
{
	AnyValue expr1 = pop();
	AnyValue expr2 = pop();
	AnyValue cond  = pop();
	if (toBool(cond))
		push(expr2);
	else
		push(expr1);
}

void Expr::when()
{
	AnyValue ex = pop();
	AnyValue cond = pop();

	if (!toBool(cond))
		top(ex);
}

void Expr::equal()
{
	AnyValue av1 = pop();
	AnyValue av2 = pop();
	push(bool(cmp(av2, av1) == 0));
}

void Expr::notEqual()
{
	AnyValue av1 = pop();
	AnyValue av2 = pop();
	push(bool(cmp(av2, av1) != 0));
}

void Expr::greater()
{
	AnyValue av1 = pop();
	AnyValue av2 = pop();
	push(bool(cmp(av2, av1) > 0));
}

void Expr::lessThan()
{
	AnyValue av1 = pop();
	AnyValue av2 = pop();
	push(bool(cmp(av2, av1) < 0));
}

void Expr::greaterOrEqual()
{
	AnyValue av1 = pop();
	AnyValue av2 = pop();
	push(bool(cmp(av2, av1) >= 0));
}

void Expr::lessOrEqual()
{
	AnyValue av1 = pop();
	AnyValue av2 = pop();
	push(bool(cmp(av2, av1) <= 0));
}

void Expr::add()
{
	AnyValue av1 = pop();
	AnyValue av2 = pop();	
	if (!av1.isNull() || av2.isString())
		av2 += av1;
	else
		av2.clear();
	push(av2);
}

void Expr::diff()
{
	AnyValue av1 = pop();
	AnyValue av2 = pop();
	if (av1.isNull())
		av2.clear();
	if (!av2.isNull())
		av2 -= av1;
	push(av2);
}

void Expr::mult()
{
	AnyValue av1 = pop();
	AnyValue av2 = pop();
	if (av1.isNull())
		av2.clear();
	if (!av2.isNull())
		av2 *= av1;
	push(av2);
}

void Expr::div()
{
	AnyValue av1 = pop();
	AnyValue av2 = pop();
	if (av1.isNull())
		av2.clear();
	if (!av2.isNull())
		av2 /= av1;
	push(av2);
}

void Expr::umin()
{
	AnyValue av = pop();
	if (!av.isNull())
		av *= Num(Int(-1));
	push(av);
}

ExprStack::ExprStack(Expr *e, Datum &p)
:	expr(e),
	ptr(p),
	status(process)
{
	if (!haveArgs())
		status = noargs;
}
	
bool ExprStack::haveArgs()
{
	assert(*ptr.s != INST_NULL);
	return *ptr.s != INST_ARGEND;
}

bool ExprStack::getAddress(Int *&addrNo, String *&addrName)
{
	if (status == process) {
		switch(expr->findFullAddress(addrNo, addrName, ptr)) {
		case ERROR:
			return false;
		case INST_ARGEND:
			status = noargs;
		default:
			return true;
		}
	}
	return false;
}

Int ExprStack::reference()
{
	Int ref = ERR;
	if (status == process) {
		assert(expr != NULL);
		Int *addrNo;
		String *addrName;
		Instruction inst = expr->findFullAddress(addrNo, addrName, ptr);
		switch(inst) {
		case INST_ARGEND:
			status = noargs;
		case INST_ARGSEPAR:
			ref = *addrNo;			
			break;
		case ERR:
			break;
		default:
			assert(0);
			break;
		}
	}
	return ref;
}

String ExprStack::referenceName()
{
	String name = NULL_STRING;
	if (status == process) {
		assert(expr != NULL);
		Int *addrNo;
		String *addrName;
		Instruction inst = expr->findFullAddress(addrNo, addrName, ptr);
		switch(inst) {
		case INST_ARGEND:
			status = noargs;
		case INST_ARGSEPAR:
			name = *addrName;			
			break;
		case ERR:
			break;
		default:
			assert(0);
			break;
		}
	}
	return name;
}

void Expr::skipUnusedArgs(Datum &buff)
{
	while (*buff.s != INST_NULL && *buff.s != INST_ARGEND) {
		switch(*buff.s++) {
		case INST_STRING:
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
	assert(*buff.s == INST_ARGEND);
	buff.s++;
}

AnyValue ExprStack::value()
{
	AnyValue val;
	if (status == process) {
		assert(expr != NULL);
		Instruction inst = expr->execute(ptr);
		assert(inst != INST_NULL);
		if (inst == INST_ARGEND)
			status = noargs;
		val = expr->pop();
	}
	return val;
}

void ExprStack::push(const AnyValue &av)
{
	assert(expr != NULL);
	if (status != finished) {
		expr->skipUnusedArgs(ptr);
		status = finished;
	}
	expr->top(av);
}

Expr *ExprStack::expression()
{
	return expr;
}

void Expr::execFunction(Datum &ptr)
{
	Int funcId = toInt(pop());
	ExprFunction *ef = (*funcs_)[funcId];
	ExprStack es(this, ptr);
	if (ef->fp == NULL)
		error(libMsg("UNBOUND_FUNC"), toCharPtr(ef->name), toCharPtr(expr));
	ef->fp(es);
	if (!es.wasExecuted())
		skipUnusedArgs(ptr);
}

#define MAX_ARGS	10

void Expr::execMsg(Datum &ptr)
{
	String msg = toString(pop()), sec = toString(pop()), 
		   tab = toString(top());

	String args[MAX_ARGS];
	String message = getMsg(tab, sec, msg);
	ExprStack es(this, ptr);
	for (Int i = 0; i < MAX_ARGS && !es.noMoreArgs(); i++)
		args[i] = toString(es.value());

	// resolve variable args and push the message
	es.push(message.replaceArgs(args));
}

String Expr::getMsg(const String &, const String &, const String &)
{
	return NULL_STRING;
}

static void execMonthName(ExprStack &s)
{
	s.push(toDate(s.value()).monthName());
}

static void execDayName(ExprStack &s)
{
	s.push(toDate(s.value()).dayOfWeekName());
}

static void execToNum(ExprStack &s)
{
	s.push(toNum(s.value()));
}

static void execToFloat(ExprStack &s)
{
	s.push(toNum(s.value()));
}

static void execToDate(ExprStack &s)
{
	s.push(toDate(s.value()));
}

static void execToTime(ExprStack &s)
{
	s.push(toTime(s.value()));
}

static void execToString(ExprStack &s)
{
	s.push(toString(s.value()));
}

static void execMonth(ExprStack &s)
{
	s.push(Num(toDate(s.value()).month()));
}

static void execDay(ExprStack &s)
{
	s.push(Num(toDate(s.value()).day()));
}

static void execYear(ExprStack &s)
{
	s.push(Num(toDate(s.value()).year()));
}

void Expr::addDefFunctions()
{
	addFunction("time",			execToTime);
	addFunction("date",			execToDate);
	addFunction("num",			execToNum);
	addFunction("float",		execToFloat);
	addFunction("string",		execToString);
	addFunction("monthname",	execMonthName);
	addFunction("dayname",		execDayName);
	addFunction("month",		execMonth); 
	addFunction("day",			execDay);
	addFunction("year",			execYear);
}

void Expr::deleteStack(void) 
{
	cleanStack();
}
