/********************************************************************
* Copyright (c) 1994 InterSoft Co.  All Rights Reserved
* THIS IS UNPUBLISHED PROPRIETARY SOURCE CODE OF InterSoft Co.
* The copyright notice above does not evidence any
* actual or intended publication of such source code.
*
* $Id: fmexpr.h,v 1.1.1.1 1996/04/16 18:51:39 gustavof Exp $
*
* DESCRIPTION
* Class definitions for FmExpr Type.
*********************************************************************/

#ifndef FMEXPR_H
#define FMEXPR_H

#include <ifound/expr.h>
#include <ifound/functype.h>

#if HAVE_PRAGMA_PACK
#	pragma pack(4)
#endif

class FmFieldObj;
class DbExpr;

enum FormOper {
	fm_op_avg,
	fm_op_sum,
	fm_op_min,
	fm_op_max,
	fm_op_count,
};

class FmExpr : public Expr {

#ifdef DECLARE_execKeyHelp	
	static friend void execKeyHelp(ExprStack &s);
#endif
#ifdef DECLARE_resolveAccum	
	static friend bool resolveAccum(ExprStack &s);
#endif
#ifdef DECLARE_execMulti	
	static friend void execMulti(FormOper op, ExprStack &s);
#endif
#ifdef DECLARE_execDescr	
	static friend void execDescr(ExprStack &s);
#endif

// private:
	FormRep *formRep;

	// field to treat as 'this'
	FmFieldObj *field;
	Int row;

	// target field, to be used to format expression result
	FmFieldObj *tfield; 

	AnyValue thisval;
	
//	IFPCCP fp;
	
	void resolveOthers(Instruction op);
	void resolveMulti(Instruction op, FmFieldObj *f);

//	void init(CCPFPCCP fp);

	String description(FmFieldObj *f);

	Int mapField(const String &fName);

	void addFunctions();

public:
	FmExpr(FmFieldObj *f, UnShort r = 0);
	FmExpr(const Expr &e, FmFieldObj *f, UnShort r = 0);
	FmExpr(const DbExpr &e, Int table, FmFieldObj *f, UnShort r = 0);
	FmExpr(const String &txt, FmFieldObj *f, UnShort r);
//	FmExpr(FmFieldObj *f, char *buff);
	~FmExpr();

	AnyValue eval();
	String formatValue(const AnyValue &v);
	void setInfo(UnShort r);
	void setInfo(FmFieldObj *f = NULL, UnShort r = 0, FmFieldObj *tf = NULL);
	void setInfo(FmFieldObj *f, UnShort r, 
			const String &val, FmFieldObj *tf = NULL);
	AnyValue execute();

	int resolveFields(IFPCCP f);

	virtual FmExpr *getFmExpr();
	virtual String getMsg(const String &tab, const String &sec,
			const String &msg);
};

#if HAVE_PRAGMA_PACK
#	pragma pack()
#endif

#endif // FMEXPR_H
