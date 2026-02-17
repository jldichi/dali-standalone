//- Copyright Notice
// -----------------------------------------------------------------------
// (C) Copyright 1998 InterSoft S.A.  All Rights Reserved
// THIS IS UNPUBLISHED PROPRIETARY SOURCE CODE OF InterSoft S.A.
// The copyright notice above does not evidence any actual or intended 
// publication of such source code.
//
// $Id: dbmsprop.h,v 1.4 2002/03/20 17:12:06 hardaiz Exp $
// -----------------------------------------------------------------------

#ifndef	DBMSPROPERTIES_H
#define	DBMSPROPERTIES_H

#include <sqldb/fwork/object.h>
#include <ifound/str.h>
#include <sqldb/metadata/mdmisc.h>

/**
 * This class is used to check DBMS dependencies and restrictions.
 * 
 * @author	Eduardo Chiocconi
 */
class DBMSProperties : public Object {

public:
	DBMSProperties(DbmsType dp);
	~DBMSProperties();

	bool isReservedWord(const String &w);

	// Debug
	void dump() const;
	String convertToString() const;
	Object *clone() const;
	inline bool isVerboseOn() { return (sqlVerbose_d == true); };

private:
	DbmsType dbms_d;
	bool sqlVerbose_d;

	void init();

	DBMSProperties(const DBMSProperties &b);
	DBMSProperties &operator=(const DBMSProperties &b);
};

struct ReservedWords {
	char *word_d;
};

typedef struct ReservedWords ReservedWords;

#endif	// DBMSPROPERTIES_H
