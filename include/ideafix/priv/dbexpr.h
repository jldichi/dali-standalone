/********************************************************************
* Copyright (c) 1992 InterSoft Co.  All Rights Reserved
* THIS IS UNPUBLISHED PROPRIETARY SOURCE CODE OF InterSoft Co.
* The copyright notice above does not evidence any
* actual or intended publication of such source code.
*
*
* $Id: dbexpr.h,v 1.2 1996/09/16 19:48:45 gustavof Exp $
*
*
* DESCRIPTION:
*				Class definitions for DbExpr Type.
*
*********************************************************************/

#ifndef DBEXPR_H
#define DBEXPR_H


#include <ifound/expr.h>

#if HAVE_PRAGMA_PACK
#	pragma pack(4)
#endif

class DbExpr : public Expr {

// private:

	Int field;
	UnShort row;
	
	void resolveOthers(Instruction op);
	Int mapField(const String &fName);
	static gtype (PtrArray, ExprFunction) *gfuncs_;
public:

	DbExpr(Int f = ERROR, UnShort r = 0);
	DbExpr(const String &txt, Int f = ERROR, UnShort r = 0);
	~DbExpr();

	void setInfo(Int f = ERROR, UnShort r = 0);

	Int getField();
};

#if HAVE_PRAGMA_PACK
#	pragma pack()
#endif

#endif // TABEXPR_H
