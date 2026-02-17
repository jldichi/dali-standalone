/********************************************************************
* Copyright (c) 1992 InterSoft Co.  All Rights Reserved
* THIS IS UNPUBLISHED PROPRIETARY SOURCE CODE OF InterSoft Co.
* The copyright notice above does not evidence any
* actual or intended publication of such source code.
*
* $Id: exprbld.cc,v 1.5 2004/09/29 15:12:49 damianm Exp $
*
* DESCRIPTION
*********************************************************************/

#include <strings.h>

#include <ifound/expr.h>
#include <ifound/itypes.h>
#include <ifound/stdc.h>

declare(PtrArray, ExprFunction);
implement(PtrArray, ExprFunction);
declare(PtrArrayCursor, ExprFunction);
implement(PtrArrayCursor, ExprFunction);

#define ALIGNMENT	(sizeof(void*))

//#define BUFFER_LEN	1024
#define BUFFER_LEN	2048


static UnShort buffer[BUFFER_LEN] = { 0 };
static union Datum buff = { 0 };
static UnShort *endbuff_ = NULL;

static inline UnShort *endbuff()
{
	if (endbuff_ == NULL)
		endbuff_ = buffer + BUFFER_LEN;
	return endbuff_;
}

static void align(Expr *e, Instruction it)
{
	assert(ALIGNMENT%sizeof(UnShort) == 0);

	if ((buff.s-buffer) % (ALIGNMENT/sizeof(UnShort))) {
		for (int pad = (ALIGNMENT/sizeof(UnShort)) - (buff.s-buffer) % 
				  	   (ALIGNMENT/sizeof(UnShort)); pad; pad--)
			e->code(Instruction(pad == 1 ? it : INST_PAD));
	} else {
		e->code(INST_PAD);
		align(e, it);
	}
}

// rutinas y datos para la interface con el parser

static Expr *e = NULL;

int exprpar_parse();
void setExprLine(const char *s);

void codeString(const char *str)
{
	assert(e != NULL);
	e->code(String(str));
}

void codeNum(const char *str)
{
	assert(e != NULL);
	e->code(Num(str));
}

void codeInstruction(Instruction i)
{
	assert(e != NULL);
	e->code(i);	
}

void codeFieldVal(const char *str)
{
	assert(e != NULL);
	e->code(String(str));
	e->code(Int(ERR));
	e->code(INST_FIELDVAL);
}

void codeIdentifier(const char *str)
{
	assert(e != NULL);
	e->code(String(str));
	e->code(Int(ERR));
	e->code(INST_IDENTIFIER);
}

void codeGetEnv(const char *str)
{
	assert(e != NULL);
	e->code(String(str));
	e->code(INST_GETENV);
}

void codeIn(Int in_nflds)
{
	assert(e != NULL);
	e->code(in_nflds);
	e->code(INST_IN);
}

void codeVoid()
{
	assert(e != NULL);
	e->code(INST_VOID);
}

void codeFunction(const char *func)
{
	assert(e != NULL);
	String f = func;
	Int funcId = e->addFunction(f);
	assert(funcId != ERROR);
	e->code(f);
	e->code(funcId);
	e->code(INST_FUNCTION);
}

void codeMsg(const char *tab, const char *sec, const char *msg)
{
	assert(e != NULL);
	e->code(String(tab));
	e->code(String(sec));
	e->code(String(msg));
	e->code(INST_MSG);
}

bool isMsgTab(const char *id)
{
	assert(e != NULL);
	return e->isMsgTab(id);
}

Expr::Expr(const Expr &ex)
:	funcs_(ex.funcs_),
	expr(ex.expr),
	vector(NULL),
	len(ex.len),
	stlen(ex.stlen),
	flags(ex.flags),
	msgChecker(ex.msgChecker)
{
/*
	for (PtrArrayCursor(ExprFunction) curs(*(ex.funcs_)); curs; ++curs) {
		assert(*curs != NULL);
		funcs_->add(tr_new ExprFunction(**curs));
	}
*/	
	if (len) {
		vector = tr_new UnShort[len];
		mCopy(vector, ex.vector, len * sizeof(UnShort));
		connectStrings();
	}
	if (!ex.sval)
		sval = NULL;
	else {
		sval = tr_new AnyValue;
		*sval = *(ex.sval);
	}
}

bool Expr::resolveFields()
{
	if (vector) {
		union Datum dbuff;
		dbuff.s = vector; 
		if (resolveFields(dbuff) == Instruction(ERROR)) {
			destroy();
			return false;
		}
	}
	return true;
}

Instruction Expr::resolveFields(Datum &dbuff)
{
	Int err = false;
	Int *fldNo = NULL;
	String *fldName = NULL;
	while (!err && *dbuff.s != INST_NULL && *dbuff.s != INST_ARGSEPAR &&
		   *dbuff.s != INST_ARGEND) {
		switch(*dbuff.s++) {
		// if fldName is NULL means that it's a 'this' field
		case INST_STRING:
			fldName = dbuff.str++;
			break;
		// It's an identifier, I must search if it's a field or a
		// variable (in this order) and patch it
		case INST_IDENTIFIER:
			assert(fldName != NULL && fldNo != NULL);
			dbuff.s--;
			if ((*fldNo = mapField(*fldName)) != ERR)
				*dbuff.s++ = INST_FIELDVAL;
			else {
				if ((*fldNo = mapVariable(*fldName)) != ERR)
					*dbuff.s++ = INST_VARIABLE;
				else
					err = true;
			}
			fldNo = NULL;
			fldName = NULL;
			break;
		case INST_FIELDVAL:
			assert(fldName != NULL && fldNo != NULL);
			if ((*fldNo = mapField(*fldName)) == ERR)
				err = true;
			fldNo = NULL;
			fldName = NULL;
			break;
		case INST_NUM:
			dbuff.num++;
			break;
		case INST_DATE: 
		case INST_TIME:
			dbuff.i++;
			break;
		case INST_INT:
			fldNo = dbuff.i++;
			break;
 		case INST_BOOL:
			dbuff.s++;
			break;
		case INST_MSG:
			err = !resolveArgs(dbuff);
			break;
		case INST_FUNCTION:
			err = !resolveFunction(*fldNo, dbuff);
			break;
		}
	}
	if (err)
		return Instruction(ERROR);
	else {
		switch(*dbuff.s) {
		case INST_NULL:
		case INST_ARGEND:
			return Instruction(*dbuff.s);
		case INST_ARGSEPAR:
			{
				Instruction inst = Instruction(*dbuff.s++);
				return inst;
			}
		default:
			assert(0);
			return INST_NULL;
		}
	}
}

bool Expr::resolveFunction(Int funcNo, Datum &dbuff)
{
	ExprFunction *f = (*funcs_)[funcNo];
	assert(f != NULL);

	// if the function has resolve function pointer
	// it has to resolve each argument 
	if (f->resolve != NULL) {
		ExprStack es(this, dbuff);
		bool ret = f->resolve(es);
		skipUnusedArgs(dbuff);
		return ret;
	}
	return resolveArgs(dbuff);
}

bool Expr::resolveArgs(Datum &dbuff)
{
	Instruction inst; 	
	while ((inst = resolveFields(dbuff)) != INST_ARGEND) {
		assert(inst != INST_NULL);
		if (inst == Instruction(ERROR))
			return false;
	}

	skipUnusedArgs(dbuff);

	return true;
}

Expr::Expr(const String &txt, IFPCCP mc)
:	funcs_(NULL),
	expr(txt),
	vector(NULL),
	len(0),
	stlen(0),
	flags((enum ExprFlags) 0),
	sval(NULL),
	msgChecker(mc)
{
	assert((sizeof(String)%sizeof(UnShort)) == 0 &&
		   (sizeof(Num)%sizeof(UnShort)) == 0);

	compile(txt);
}

void Expr::compile(const String &txt)
{
	if (txt != NULL_STRING)
		parse(txt);
	else {
		begin();
		code(INST_NULL);
		end();
	}
}


void Expr::parse(const String &s)
{
	destroy();
	expr = s;

	begin();
	e = this;
	setExprLine(toCharPtr(s));

	// if parsing fails expression is marked as invalid
	if (exprpar_parse())
		destroy();
	else {
		code(INST_NULL);
		end();
	}
	e = NULL;
}

void Expr::code(const Num &n)
{
	assert(buff.s && buff.num+1 < (Num*)(endbuff()-1));
	align(this, INST_NUM);
	*buff.num++ = n;
	stlen += n.storeLength();
}

void Expr::code(Instruction rt)
{
	//fprintf(stderr,"buff.s: %d - endbuff: %d\n", buff.s, endbuff());
	assert(buff.s && buff.s < endbuff());
	*buff.s++ = UnShort(rt);
	stlen += sizeof(UnShort);
	switch(rt) {
	case INST_FIELDVAL:
	case INST_VARIABLE:
	case INST_IDENTIFIER:
		flags = ExprFlags(HAVE_LVALUE|flags);
		// intentionally fall down
	case INST_FUNCTION:
	case INST_HOUR:
	case INST_TODAY:
		flags = ExprFlags(HAVE_COMPLEX|flags);
		break;
	case INST_GETENV:
		flags = ExprFlags(HAVE_ENVVARS|flags);
		break;
	case INST_VOID:
		flags = ExprFlags(HAVE_VOID|flags);
		break;
	case INST_MSG:
		flags = ExprFlags(HAVE_MSG|flags);
		break;
	default:
		break;
	}
}

Int Expr::address()
{
	if (vector == NULL)
		return ERR;

	union Datum dbuff;
	dbuff.s = vector;

	Int *addrNo;
	String *addrName;

	if (findFullAddress(addrNo, addrName, dbuff) == ERR)
		return ERR;
	
	return *addrNo;
}

String Expr::symAddress()
{
	if (vector == NULL)
		return NULL_STRING;

	union Datum dbuff;
	dbuff.s = vector;

	Int *addrNo;
	String *addrName;

	if (findFullAddress(addrNo, addrName, dbuff) == ERR)
		return NULL_STRING;

	return *addrName;
}

Instruction Expr::findFullAddress(Int *&addrNo , String *&addrName, 
								  Datum &dbuff)
{
	// Skip pads
	while (*dbuff.s == INST_PAD)
		dbuff.s++;

	if (*dbuff.s++ != INST_STRING)
		return Instruction(ERR);

	addrName = dbuff.str++;

	// Skip pads
	while (*dbuff.s == INST_PAD)
		dbuff.s++;

	if (*dbuff.s++ != INST_INT)
		return Instruction(ERR);

	addrNo = dbuff.i++;

	// Skip pads
	while (*dbuff.s == INST_PAD)
		dbuff.s++;

	if (*dbuff.s != INST_FIELDVAL && *dbuff.s != INST_IDENTIFIER && 
									 *dbuff.s != INST_VARIABLE)
		return Instruction(ERR);

	dbuff.s++;

	// Skip pads
	while (*dbuff.s == INST_PAD)
		dbuff.s++;

	switch (*dbuff.s) {
	case INST_ARGSEPAR:
		{
			UnShort ret = *dbuff.s;
			dbuff.s++;
			return Instruction(ret);
		}
		break;
	case INST_ARGEND:
	case INST_NULL:
		return Instruction(*dbuff.s);
	default:
		return Instruction(ERR);
	}
}

void Expr::code(const Time &t)
{
	assert(buff.s && buff.i+1 < (Int*)(endbuff()-1));
	align(this, INST_TIME);
	*buff.i++ = toInt(t);
	stlen += sizeof(Int);
}

void Expr::code(const Date &d)
{
	assert(buff.s && buff.i+1 < (Int*)(endbuff()-1));
	align(this, INST_DATE);
	*buff.i++ = toInt(d);
	stlen += sizeof(Int);
}

void Expr::code(bool b)
{
	assert(buff.s && buff.s < endbuff()-1);
	*buff.s++ = UnShort(INST_BOOL);
	*buff.s++ = UnShort(b);
	stlen += 2*sizeof(UnShort);
}

void Expr::code(Int i)
{
	assert(buff.s && buff.i+1 < (Int*)(endbuff()-1));
	align(this, INST_INT);
	*buff.i++ = i;
	stlen += sizeof(Int);
}

void Expr::code(const String &s)
{
	assert(buff.s && buff.str+1 < (String*)(endbuff()-1));
	align(this, INST_STRING);
	mCopy(buff.str, &s, sizeof(String));
	buff.str->connect();
	buff.str++;
	stlen += s.storeLength();
}

void Expr::begin()
{
	destroy();
	buff.s = buffer;
}

void Expr::end()
{
	len = buff.s-buffer;

	if (len == 0) return;

	vector = tr_new UnShort[len];
	mCopy(vector, buffer, len*sizeof(UnShort));
	stlen += (2*sizeof(long)) + expr.storeLength();
}
