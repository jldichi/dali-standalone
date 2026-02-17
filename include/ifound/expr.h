/********************************************************************
* Copyright (c) 1992 InterSoft Co.  All Rights Reserved
* THIS IS UNPUBLISHED PROPRIETARY SOURCE CODE OF InterSoft Co.
* The copyright notice above does not evidence any
* actual or intended publication of such source code.
*
* $Id: expr.h,v 1.4 2002/03/15 18:55:11 hardaiz Exp $
*
* DESCRIPTION
* Class definitions for Expr Type.
*********************************************************************/

#ifndef EXPR_H
#define EXPR_H

#include <ifound.h>

#ifdef __GNUC__
#pragma interface
#endif

#include <ifound/instruct.h>
#include <ifound/anyvalue.h>
#include <ifound/str.h>

class Num;
class Date;
class Time;
class Connection;
class Expr;
class ReportExpr;
class FormExpr;
class FmExpr;

union Datum {
	UnShort *s;
	Int 	*i;
	String  *str;
	Num		*num;
};

enum ExprFlags {
	HAVE_LVALUE		= 1 << 0,
	HAVE_COMPLEX	= 1 << 1,
	HAVE_ENVVARS	= 1 << 2,
	HAVE_VOID		= 1 << 3,
	HAVE_MSG		= 1 << 4,
};

class ExprStack {

	Expr *expr;
	Datum &ptr;
	enum {
		process,
		noargs,
		finished,
	} status;
		
public:
	
	ExprStack(Expr *e, Datum &p);
	
	void push(const AnyValue &av);
	Int reference();
	String referenceName();
	AnyValue value();

	bool getAddress(Int *&addrNo, String *&addrName);
	
	bool haveArgs();

	Expr *expression();

	inline bool wasExecuted() const;
	inline bool noMoreArgs() const;
};

typedef void (*FPES)(ExprStack &);
typedef bool (*BFPES)(ExprStack &);

struct ExprFunction {
	String name;
	FPES fp;	
	BFPES resolve;
	inline explicit ExprFunction(const String &fname, FPES f = NULL,
			BFPES r = NULL);
	inline ExprFunction(const ExprFunction &ef);
};

class gtype(PtrArray, ExprFunction);

class Expr {

	friend class ExprStack;
	
// private:

	// routines for damn strings
	void disconnectStrings();
	void connectStrings();

protected:

	// stack operations
	virtual void getEnv();
	virtual void in();	
	virtual void Not();
	virtual void And();
	virtual void Or();
	virtual void between();
	virtual void equal();
	virtual void notEqual();
	virtual void conditionalIf();
	virtual void when();
	virtual void greater();
	virtual void lessThan();
	virtual void greaterOrEqual();
	virtual void lessOrEqual();
	virtual void add();
	virtual void diff();
	virtual void mult();
	virtual void div();
	virtual void umin();
	virtual void execToday();
	virtual void execHour();
	virtual void execFunction(Datum &ptr);
	virtual void execMsg(Datum &ptr);

	// AUTUMN
	void deleteStack();

	gtype(PtrArray, ExprFunction) *funcs_;
	void addDefFunctions();

	// routine to check whether a field exists or not
	virtual Int mapField(const String &);

	// routine to check whether a variable exists or not
	virtual Int mapVariable(const String &);

	String		expr;		// source string
	UnShort		*vector;	// pointer to compiled expression 
	Int			len;		// vector length
	Int			stlen;		// store length
	ExprFlags	flags;		// expression flags
	AnyValue	*sval;		// if the expression is simple this value
	IFPCCP		msgChecker;	// function that tells me if the identifier is
							// or is not a msgtable

	// stack operations
	AnyValue pop();
	const AnyValue &top();
	void top(const AnyValue &av);
	void push(const AnyValue &av);
	void resetStack();
	
	virtual void resolveOthers(Instruction op);

	bool resolveFunction(Int funcNo, Datum &buff);
	bool resolveArgs(Datum &dbuff);
	Instruction resolveFields(Datum &buff);
	
	inline void operator=(const Expr &);

	void destroy();

	// low level left value routines
	Instruction findFullAddress(Int *&addrNo, String *&addrName, Datum &ptr);

	Instruction execute(Datum &ptr);

	void skipUnusedArgs(Datum &ptr);
	
public:

	explicit Expr(const String &txt, IFPCCP mc = NULL);
	Expr();
	Expr(const Expr &e);
	virtual ~Expr();

	void parse(const String &txt);
	void compile(const String &txt);

	bool operator !();
	operator int();

	bool isNull();

	inline const String &getExpr() const;
	inline Int storeLength() const;

	inline bool isComplex()	const;
	inline bool haveLValue() const;
	inline bool haveEnvVars() const;
	inline bool canBeVoid()	const;
	inline bool haveMsgs() const;
	
	Int addFunction(const String &fname, FPES f = NULL, BFPES r = NULL);

	// left value routines
	Int address();
	String symAddress();

	// read and write methods from/to a buffer
	// funciones para grabarse y leerse de un buffer
	Int store(void *buff);
	void load(void *buff);

	// read and write methods from/to a	FILE
	void store(FILE *);
	void load(FILE *);

	// methods to compile expressions
	void begin();
	void end();
	void code(const String &s);
	void code(const Num &n);
	void code(Instruction rt);
	void code(const Time &t);
	void code(const Date &d);
	void code(bool b);
	void code(Int i);

	AnyValue eval();

	bool resolveFields();

	// methods to send and receive an expression from/to a connection
	friend Connection &operator<<(Connection &c, const Expr &e);
	friend Connection &operator>>(Connection &c, Expr &e);

	virtual ReportExpr *getReportExpr();
	virtual FormExpr *getFormExpr();
	virtual FmExpr *getFmExpr();

	virtual String getMsg(const String &tab, const String &sec,
						  const String &msg);

	bool isMsgTab(const String &id);
	
#ifdef ISDEBUG
	void debugExpr();
#endif
};

#include <ifound/expr.icc>

#endif // EXPR_H
