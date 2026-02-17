/********************************************************************
* Copyright (c) 1994 InterSoft Co.  All Rights Reserved
* THIS IS UNPUBLISHED PROPRIETARY SOURCE CODE OF InterSoft Co.
* The copyright notice above does not evidence any
* actual or intended publication of such source code.
*
* $Id: dbexprn.h,v 1.1.1.1 1996/04/16 18:51:39 gustavof Exp $
*
* DESCRIPTION:
*********************************************************************/

#ifndef DBEXPRN_H
#define DBEXPRN_H

#include <ifound/expr.h>

class String;

#if HAVE_PRAGMA_PACK
#	pragma pack(4)
#endif

class DbExpr : public Expr {

// private:

	DbSetRep *set_;
	DbMemberRep *memberRep_;
	char *buffer_;
	
	void resolveOthers(Instruction op);
	Int mapField(const String &fName);
	
	DbSetRep *set()			{ assert(set_ != NULL); return set_;			}
	char *buffer()			{ assert(buffer_ != NULL); return buffer_;		}
	DbMemberRep *memberRep(){ assert(memberRep_ != NULL); return memberRep_;}

public:

	DbExpr(DbSetRep *s = NULL, DbMemberRep *m = NULL,
			char *b = NULL);
	DbExpr(const String &txt, DbSetRep *s = NULL, DbMemberRep *m = NULL, 
			char *buffer = NULL);
	~DbExpr();

	void set(DbSetRep *s, DbMemberRep *m, char *buffer);
};

#if HAVE_PRAGMA_PACK
#	pragma pack()
#endif

#endif
