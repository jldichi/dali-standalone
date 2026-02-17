/********************************************************************
* Copyright (c) 1994 InterSoft Co.  All Rights Reserved
* THIS IS UNPUBLISHED PROPRIETARY SOURCE CODE OF InterSoft Co.
* The copyright notice above does not evidence any
* actual or intended publication of such soucone code.
*
* $Id: dbfield.h,v 1.1.1.1 1996/04/16 18:51:39 gustavof Exp $
*
* DESCRIPTION
*********************************************************************/

#ifndef DBFIELD_H
#define DBFIELD_H

#include <ifound.h>

#ifdef	__GNUC__
#	pragma interface
#endif

class DbMemberRep;
class DbConnection;
class AnyValue;

class DbField {
	
protected:
	
	DbMemberRep *member;
	
	virtual void sendOne(DbConnection &con, char *buff, Int row) = 0;

public:

	DbField(DbMemberRep *m)
	: member(m) { }

	virtual void receive(DbConnection &con, char *buffer, Int offset = 0,
						 Int len = ERR) = 0;
	virtual void send(DbConnection &con, char *buffer, Int offset = 0, 
					  Int len = ERR);

	virtual void send(DbConnection &con, const AnyValue &av) = 0;

	static DbField *build(DbMemberRep *member);
};

class DbStringField : public DbField {

private :

	void sendOne(DbConnection &, char *, Int) { }

public:

	DbStringField(DbMemberRep *m) : DbField(m) { }
	
	void receive(DbConnection &con, char *buffer, Int offset = 0,
				 Int len = ERR);
	void send(DbConnection &con, char *buffer, Int offset = 0, Int len = ERR);

	void send(DbConnection &con, const AnyValue &av);
	
};

class DbDateField : public DbField {

private:

	void sendOne(DbConnection &con, char *buffer, Int row);

public:

	DbDateField(DbMemberRep *m) : DbField(m) { }

	void receive(DbConnection &con, char *buffer, Int offset = 0, 
				 Int len = ERR);

	void send(DbConnection &con, const AnyValue &av);
};

class DbTimeField : public DbField {

private:

	void sendOne(DbConnection &con, char *buffer, Int row);

public:

	DbTimeField(DbMemberRep *m) : DbField(m) { }

	void receive(DbConnection &con, char *buffer, Int offset = 0, 
				 Int len = ERR);

	void send(DbConnection &con, const AnyValue &av);
};

class DbBoolField : public DbField {

private:

	void sendOne(DbConnection &con, char *buffer, Int row);

public:

	DbBoolField(DbMemberRep *m) : DbField(m) { }

	void receive(DbConnection &con, char *buffer, Int offset = 0, 
				 Int len = ERR);

	void send(DbConnection &con, const AnyValue &av);
};

class DbFloatField : public DbField {

private:

	void sendOne(DbConnection &con, char *buffer, Int row);

public:

	DbFloatField(DbMemberRep *m) : DbField(m) { }

	void receive(DbConnection &con, char *buffer, Int offset = 0, 
				 Int len = ERR);

	void send(DbConnection &con, const AnyValue &av);
};

class DbNumField : public DbField {

private:

	void sendOne(DbConnection &con, char *buffer, Int row);

public:

	DbNumField(DbMemberRep *m) : DbField(m) { }

	void receive(DbConnection &con, char *buffer, Int offset = 0, 
				 Int len = ERR);

	void send(DbConnection &con, const AnyValue &av);
};

#endif
