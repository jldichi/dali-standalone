/********************************************************************
* Copyright (c) 1994 InterSoft Co.  All Rights Reserved
* THIS IS UNPUBLISHED PROPRIETARY SOURCE CODE OF InterSoft Co.
* The copyright notice above does not evidence any
* actual or intended publication of such source code.
*
* $Id: repexpr.h,v 1.2 1996/09/16 19:48:47 gustavof Exp $
*
* DESCRIPTION
* Class definitions for ReportExpr Type.
*********************************************************************/

#ifndef REPEXPR_H
#define REPEXPR_H

#ifdef __GNUC__
#	pragma interface
#endif

#include <ifound/expr.h>
#include <ifound/functype.h>

#if HAVE_PRAGMA_PACK
#	pragma pack(4)
#endif

class ReportRep;

class ReportExpr : public Expr {
	
// private:

	ReportRep *rp;
	RpZoneRep *zrep;
	bool prevVal;
			
	void getEnv();
	void resolveOthers(Instruction op);

	Int mapField(const String &);
	Int mapVariable(const String &);

	void execToday();
	void execHour();
	static gtype (PtrArray,ExprFunction) *gfuncs_;
public:

	ReportExpr(ReportRep *r, RpZoneRep *zr = NULL);
	ReportExpr(const String &txt, ReportRep *r, RpZoneRep *zr = NULL);
	~ReportExpr();
	
	AnyValue eval(bool pv);
	AnyValue eval();
	
	ReportExpr *getReportExpr();

	ReportRep *reportRep()	{ return rp; }
	RpZoneRep *zoneRep()	{ return zrep; }

	void addFunctions();
};

class RpZoneExpr;
void clearAliases();
bool addAlias(const String &aliasName, RpZoneExpr *ze);
RpZoneExpr *findAlias(const String &aliasName);
RpZoneExpr *findExpr(ReportRep *rp, const String &name);

#if HAVE_PRAGMA_PACK
#	pragma pack()
#endif

#endif // REPEXPR_H
