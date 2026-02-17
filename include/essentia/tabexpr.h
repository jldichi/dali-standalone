/********************************************************************
* Copyright (c) 1992 InterSoft Co.  All Rights Reserved
* THIS IS UNPUBLISHED PROPRIETARY SOURCE CODE OF InterSoft Co.
* The copyright notice above does not evidence any
* actual or intended publication of such source code.
*
* $Id: tabexpr.h,v 1.1.1.1 1996/04/16 18:51:48 gustavof Exp $
*
* DESCRIPTION
* Class definitions for TabExpr Type.
*********************************************************************/

#ifndef TABEXPR_H
#define TABEXPR_H

#include <ifound.h>

#ifdef __GNUC__
#pragma interface
#endif


#include <ifound/expr.h>

class DbTable;

class TabExpr : public Expr {

// private:

	DbTable *table;
	Int nfield;

	void resolveOthers(Instruction op);

public:

	explicit TabExpr(DbTable *t = NULL, Int n = ERR);
	explicit TabExpr(const String &txt, DbTable *t = NULL, Int n = ERR);
	~TabExpr();

	void setInfo(DbTable *t, Int n);

};

#endif // TABEXPR_H
