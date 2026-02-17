/********************************************************************
* Copyright (c) 1994 InterSoft Co.  All Rights Reserved
* THIS IS UNPUBLISHED PROPRIETARY SOURCE CODE OF InterSoft Co.
* The copyright notice above does not evidence any
* actual or intended publication of such source code.
*
* $Id: formexpr.h,v 1.5 1998/11/19 13:37:27 ernestod Exp $
*
* DESCRIPTION
* Class definitions for FormExpr Type.
*********************************************************************/
 
 
#ifndef FORMEXPR_H
#define FORMEXPR_H

#include <ifound/expr.h>
#include <ifound/functype.h>

#if HAVE_PRAGMA_PACK
#	pragma pack(4)
#endif
	
class FmFieldRep;
class FormRep;
class DbExpr;

enum FormOper {
	fm_op_avg,
	fm_op_sum,
	fm_op_min,
	fm_op_max,
	fm_op_count,
};

class FormExpr : public Expr {

#ifdef DECLARE_execKeyHelp	
	friend void execKeyHelp(ExprStack &s);
#endif
#ifdef DECLARE_resolveAccum	
	friend bool resolveAccum(ExprStack &s);
#endif
#ifdef DECLARE_execMulti	
	friend void execMulti(FormOper op, ExprStack &s);
#endif
#ifdef DECLARE_execDescr	
	friend void execDescr(ExprStack &s);
#endif
	
// private:

	// field to treat as 'this'
	FmFieldRep *field;

	// target field, to be used to format expression result
	FmFieldRep *tfield; 

	UnShort row;
	char *thisval;
	IFPCCP fp;
	
	void resolveOthers(Instruction op);
	void resolveMulti(Instruction op, FmFieldRep *f);
	void init(CCPFPCCP fp);
	
	String description(FmFieldRep *f);

	Int mapField(const String &fName);
	
	void addFunctions();

	static gtype (PtrArray,ExprFunction) *gfuncs_;

public:

	FormExpr(FmFieldRep *f = NULL, UnShort r = 0);
	FormExpr(char *buff);
	FormExpr(const String &txt, FmFieldRep *f, UnShort r, IFPCCP mc = NULL);
	FormExpr(const DbExpr &e, CPFPLCP fp, Int table);
	~FormExpr();

	AnyValue eval();
	const char *formatValue(const String &s);
	void setInfo(FmFieldRep *f = NULL, UnShort r = 0, char *val = NULL,
			FmFieldRep *tf = NULL);
	const char *execute();

	bool hasMultFields(FormRep *fp);

	int resolveFields(IFPCCP f);

	FormExpr *getFormExpr();

	FmFieldRep *thisField();
	UnShort getRow();
	
	String getMsg(const String &tab, const String &sec, const String &msg);
};

#if HAVE_PRAGMA_PACK
#	pragma pack()
#endif

#endif // FORMEXPR_H
