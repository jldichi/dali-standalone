/********************************************************************
* Copyright (c) 1993 InterSoft Co.  All Rights Reserved
* THIS IS UNPUBLISHED PROPRIETARY SOURCE CODE OF InterSoft Co.
* The copyright notice above does not evidence any
* actual or intended publication of such soucone code.
*
* $Id: ixfield.h,v 1.1.1.1 1996/04/16 18:51:38 gustavof Exp $	
*
* DESCRIPTION
*********************************************************************/

#ifndef IXFIELD_H
#define IXFIELD_H

#ifdef __GNUC__
#pragma interface
#endif

struct s_field;
class DbConnection;

class IxField {
	
protected :
	
	struct s_field *fp;
	
	virtual int ilength(int l) = 0;
	virtual void sendOne(DbConnection &con, char *buff) = 0;

public:

	IxField(struct s_field *f)	{ fp = f; }

	virtual void receive(DbConnection &con, char *buff) = 0;
	virtual void send(DbConnection &con, char *buff, int l, bool);
};

class IxStringField : public IxField {

private :

	int ilength(int l);
	void sendOne(DbConnection &con, char *buff);

public:

	IxStringField(struct s_field *f) : IxField(f) { }
	
	void receive(DbConnection &con, char *buff);
	void send(DbConnection &con, char *buff, int l, bool z);
	
};

class IxDateField : public IxField {

private:

	int ilength(int l);
	void sendOne(DbConnection &con, char *buff);

public:

	IxDateField(struct s_field *f) : IxField(f) { }

	void receive(DbConnection &con, char *buff);
};

class IxTimeField : public IxField {

private:

	int ilength(int l);
	void sendOne(DbConnection &con, char *buff);

public:

	IxTimeField(struct s_field *f) : IxField(f) { }

	void receive(DbConnection &con, char *buff);
};

class IxBoolField : public IxField {

private:

	int ilength(int l);
	void sendOne(DbConnection &con, char *buff);

public:

	IxBoolField(struct s_field *f) : IxField(f) { }

	void receive(DbConnection &con, char *buff);
};

class IxByteField : public IxField {

private:

	int ilength(int l);
	void sendOne(DbConnection &con, char *buff);

public:

	IxByteField(struct s_field *f) : IxField(f) { }

	void receive(DbConnection &con, char *buff);
};

class IxShortField : public IxField {

private:

	int ilength(int l);
	void sendOne(DbConnection &con, char *buff);

public:

	IxShortField(struct s_field *f) : IxField(f) { }

	void receive(DbConnection &con, char *buff);
};

class IxLongField : public IxField {

private:

	int ilength(int l);
	void sendOne(DbConnection &con, char *buff);

public:

	IxLongField(struct s_field *f) : IxField(f) { }

	void receive(DbConnection &con, char *buff);
};

class IxDoubleField : public IxField {

private:

	int ilength(int l);
	void sendOne(DbConnection &con, char *buff);

public:

	IxDoubleField(struct s_field *f) : IxField(f) { }

	void receive(DbConnection &con, char *buff);
};

class IxNumField : public IxField {

private:

	int ilength(int l);
	void sendOne(DbConnection &con, char *buff);

public:

	IxNumField(struct s_field *f) : IxField(f) { }

	void receive(DbConnection &con, char *buff);
};

class IxIntegerField : public IxField {

private:

	int ilength(int l);
	void sendOne(DbConnection &con, char *buff);

public:

	IxIntegerField(struct s_field *f) : IxField(f) { }

	void receive(DbConnection &con, char *buff);
};

class IxBinaryField : public IxField {

private:

	int ilength(int l);
	void sendOne(DbConnection &con, char *buff);

public:

	IxBinaryField(struct s_field *f) : IxField(f) { }

	void receive(DbConnection &con, char *buff);
};

#endif
