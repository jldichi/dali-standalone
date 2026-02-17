/********************************************************************
* Copyright (c) 1992 InterSoft Co.  All Rights Reserved
* THIS IS UNPUBLISHED PROPRIETARY SOURCE CODE OF InterSoft Co.
* The copyright notice above does not evidence any
* actual or intended publication of such source code.
*
* $Id: dbr.h,v 1.1.1.1 1996/04/16 18:51:48 gustavof Exp $
*
* DESCRIPTION
* Class definitions for db library.
*********************************************************************/

#ifndef DBR_H
#define DBR_H

#include <ifound.h>

#ifdef __GNUC__
#pragma interface
#endif

#include <ifound/itypes.h>
#include <essentia/dbconst.h>
#include <ifound/anyvalue.h>
#include <essentia/dbclobj.h>
#include <essentia/pobject.h>

#define doTransaction	\
		for (Schema::beginTransaction(); !Schema::didTransaction(); \
				Schema::endTransaction())

#define doSyncTransaction	\
		for (Schema::beginTransaction(); !Schema::didTransaction(); \
				Schema::endTransaction(true))

class DbConnection;   
class LocalConnection;
class DbTableRep;
class DbFieldRep;
class SchRep;
class Schema;
class DbTable;

class DbField
{

	friend class DbTable;

protected:

	DbFieldRep	*r;		// field representation
	DbTable		*tab;	// table to which I belong

	DbTable		*relTab;

	DbField(const DbField &f) : r(NULL), tab(NULL), relTab(NULL) { };

public:

	// constructors & destructors
	DbField(DbFieldRep &dfr, DbTable &t);
	DbField(const String &nm, Int len, Int ndec, Int flags,
		const String &mask, const String &def, Int type);
	DbField() : r(NULL), tab(NULL), relTab(NULL) { }
	virtual ~DbField();

	void modif();
	void setModified();
	const String &name() const;
	Int getType() const;
	const String &typeName() const;
	bool ok() const { return bool(r); }
	virtual String toString() const = 0;
	virtual void operator=(const String &s) = 0;
	virtual void receive(DbConnection *con) = 0;
	virtual void send(DbConnection *con) const = 0;
	virtual const char *methodName() const = 0;
	virtual void send(DbConnection *con, const AnyValue &av) const = 0;
	virtual AnyValue value() = 0;
	DbFieldRep *fieldRep() { return r; }
	bool valid() const;
	bool linkTo(DbTable *t);
	SchRep *schrep();
	DbTable *relatedTab() { return relTab; }
	virtual String format() const = 0;
	Int length() const;
	virtual DbField *clone() const = 0;
};

DbField *consDbField(DbFieldRep &f, DbTable &tab);

class DbDate : public Date, public DbField
{

	friend DbField *consDbField(DbFieldRep &f, DbTable &tab);

// private:

	// constructors & destructors
	DbDate(DbFieldRep &dfr, DbTable &t);

public:

	DbDate(const DbDate & d) 	{ *this = d; }
	DbDate(const String &nm, Int flags = 0,
		   const String &mask = NULL_STRING,
		   const String &def  = NULL_STRING);
	~DbDate();

	String toString() const;
	void operator=(const Date &d);
	void operator=(const String &s);
	void receive(DbConnection *con);
	void send(DbConnection *con) const;
	void send(DbConnection *con, const AnyValue &av) const;
	AnyValue value();
	const char *methodName() const;
	String format() const;
	DbField *clone() const { return new DbDate(*this); }
};

class DbNum : public Num, public DbField
{

	friend DbField *consDbField(DbFieldRep &f, DbTable &tab);

// private:

	// constructors & destructors
	DbNum(DbFieldRep &dfr, DbTable &t);

public:

	DbNum(const DbNum & n) 	{ *this = n; }
	DbNum(const String &nm, Int len, Int ndec = 0,
			Int flags = 0, const String &mask = NULL_STRING,
			const String &def = NULL_STRING);
	~DbNum();

	String toString() const;
	void operator=(const Num &n);
	void operator=(const String &s);
	void operator=(Int n);
	void receive(DbConnection *con);
	void send(DbConnection *con) const;
	void send(DbConnection *con, const AnyValue &av) const;
	AnyValue value();
	const char *methodName() const;
	String format() const;
	DbField *clone() const { return new DbNum(*this); }
};

class DbString : public String, public DbField
{

	friend DbField *consDbField(DbFieldRep &f, DbTable &tab);

// private:

	// constructors & destructors
	DbString(DbFieldRep &dfr, DbTable &t);

public:

	DbString(const DbString &s)	{ *this = s; }
	DbString(const String &nm, Int len, Int flags = 0,
			const String &mask = NULL_STRING,
			const String &def = NULL_STRING);
	~DbString();

	// for invalid field
	DbString() { r = NULL; }
	static DbString INVALIDFIELD;

	String toString() const;
	void operator=(const String &s);
	void receive(DbConnection *con);
	void send(DbConnection *con) const;
	void send(DbConnection *con, const AnyValue &av) const;
	AnyValue value();
	const char *methodName() const;
	String format() const;
	DbField *clone() const { return new DbString(*this); }
};

class DbTime : public Time, public DbField
{

	friend DbField *consDbField(DbFieldRep &f, DbTable &tab);

// private:

	// constructors & destructors
	DbTime(DbFieldRep &dfr, DbTable &t);

public:

	DbTime(const DbTime &t) 	{ *this = t; }
	DbTime(const String &nm, Int flags = 0,
			const String &mask = NULL_STRING,
			const String &def = NULL_STRING);
	~DbTime();

	String toString() const;

	void operator=(const Time &t);
	void operator=(const String &s);
	void receive(DbConnection *con);
	void send(DbConnection *con) const;
	void send(DbConnection *con, const AnyValue &av) const;
	AnyValue value();
	const char *methodName() const;
	String format() const;
	DbField *clone() const { return new DbTime(*this); }
};

class DbBool : public Boolean, public DbField
{

	friend DbField *consDbField(DbFieldRep &f, DbTable &tab);

// private:

	// constructors & destructors
	DbBool(DbFieldRep &dfr, DbTable &t);

public:

	DbBool(const DbBool &b)	{ *this = b; }
	DbBool(const String &nm, Int flags = 0,
			const String &mask = NULL_STRING,
			const String &def = NULL_STRING);
	~DbBool();

	String toString() const;

	void operator=(const Boolean &v);
	void operator=(const String &s);
	void receive(DbConnection *con);
	void send(DbConnection *con) const;
	void send(DbConnection *con, const AnyValue &av) const;
	AnyValue value();
	const char *methodName() const;
	String format() const;
	DbField *clone() const { return new DbBool(*this); }
};


class DbPointer : public ObjectId, public DbField
{

	friend DbField *consDbField(DbFieldRep &f, DbTable &tab);

// private:

	// constructors & destructors
	DbPointer(DbFieldRep &dfr, DbTable &t);

public:

	DbPointer(const DbPointer &b)	{ *this = b; }
	DbPointer(const String &nm, Int flags = 0);
	~DbPointer();

	String toString() const;

	void operator=(const ObjectId &v);
	void operator=(const String &s);
	void receive(DbConnection *con);
	void send(DbConnection *con) const;
	void send(DbConnection *con, const AnyValue &av) const;
	AnyValue value();
	const char *methodName() const;
	String format() const;
	DbField *clone() const { return new DbPointer(*this); }
};

class DbIndexField
{

	friend class DbIndex;

// private:

	DbFieldRep *fld;
	Int offset;
	Int length;
	Int flags;

	DbIndexField(DbFieldRep *f);
	DbIndexField(const DbIndexField &dif);
	DbFieldRep *field() const;
	Int nField();
	
	void operator=(const DbIndexField &dif);

public:

	DbIndexField(DbField &f, Int off = 0, Int l = ERR, 
			IndFldFlags f = IndFldFlags(IND_NOT_NULL|IND_ASCENDING));
};

class gtype(PtrArray,DbIndexField);

class DbIndex : public ClientObject
{

	friend class DbTable;
	friend class DbTableRep;
	friend class DbCursor;
	friend class DbIntegrity;

// private:

	gtype(PtrArray, DbIndexField) &ifarray;

	DbTableRep *tabrep;
	DbTable    *table;
	bool bld;
	IndFlags iflags;

	void send(DbTable &table) const;
	void receive(DbTable &table) const;
	Int number() const;

	// To construct it from a connection
	DbIndex(DbConnection *c,  DbTableRep *tr);

	// index creation methods
	void create(const String &name);
	void build(DbCursor &c, const String &name, IFPI func);

	bool isDup() const 			{ return iflags&IND_DUP; }
	Int field(Int n) const;

	void setInvalid()			{ objectId = ERR; }

    DbIndex(const DbIndex &);
    void operator=(const DbIndex &);

public:

	DbIndex(DbTable &t, IndFlags flags = IND_UNIQUE);
	~DbIndex();

	// index creation methods
	void build(const String &name = NULL_STRING, IFPI func = NULL);
	void build(DbCursor &c, IFPI func = NULL);

	// add a particular key to the index
	bool addKey();

	// return number of fields
	Int nFields() const;

	// index creation operator
	DbIndex &operator<<(const DbIndexField &dif);

};

class DbTable
{

	friend class DbField;
	friend class DbIndex;
	friend class DbCursor;
	friend class DbIntegrity;

// private:

	DbTableRep *r;				// ptr to table representation
	const DbIndex *accessIndex;	// index to use for gets
	Int currRec;				// current record number
	Int modinds;				// Indexes that were modified
	Int version;

	// arreglo de los campos
	DbField **farray;

	DbIndex *getDbIndex(Int nind);

	SchRep *getSchRep();

	void addTable(SchRep *s, const String &nm, Int version,
			DbField &f = DbString::INVALIDFIELD);
	void delTable();

	void setRecord(DbConnection &c);

	Int indexNumber(DbIndex *ir);
	bool valid() const;
	void getRelated(Int mode);

	bool addKey(DbIndex *si);
	void setIndex(const DbIndex *si) { accessIndex = si; }

    DbTable(const DbTable &);
    void operator=(const DbTable &);

public:

	// constructores y destructores
	DbTable();
	DbTable(const String &nm);
	DbTable(const String &s, const String &nm,
			DbField &fld = DbString::INVALIDFIELD);
	DbTable(const String &s, const String &nm,
			Int version, DbField &fld = DbString::INVALIDFIELD);
	DbTable(const String &s, const String &nm, const String &desc,
			Int nf, Int flags = 0, Int maxrecs = 0);
	~DbTable();

	// to access an index
	DbIndex *getDbIndex(const String &indname);

	bool rename(const String &newname);

	void clear();

	Int lock(Int mode);
	void free();

	const String &name();
	Int nFields() const;

	// methods to return a field
	DbField &field(Int i);
	DbField &field(const String &s);
	DbField &field(const char *cs);
	DbField &field(const DbFieldRep *fr);

	// return field index given a name
	Int getFieldNumber(const String &name);

	DbNum &numField(const String &s);
	DbNum &nf(Int i);
	DbString &stringField(const String &s);
	DbString &sf(Int i);
	DbDate &dateField(const String &s);
	DbDate &df(Int i);
	DbTime &timeField(const String &s);
	DbTime &tf(Int i);
	DbBool &boolField(const String &s);
	DbBool &bf(Int i);
	DbPointer &pointerField(const String &s);
	DbPointer &pf(Int i);

#if 0
	operator ObjectRef();
	operator void *() { ObjectId(*this) ? this : 0; }
#endif

	bool ok() const;
	bool operator!() const;

	// Methods to send & receive fields from & to the buffer
	void send() const;
	void receive();

	// methods to access the database
	Int find(Int mode = THIS_KEY, Int nparts = 0);
	Int get(Int mode = THIS_KEY, Int nparts = 0);
	Int freeRecord(Int n, Int nparts = 0);
	void init();
	bool add();
	bool update();
	void del();
	void put();
	void setIndex(Int i);
	void setIndex(const String &s);
	bool is(DbField &f);

	// Table creation methods
	DbTable &operator<<(const DbField &);

	// drop an index
	bool drop(const String &indname);
};

typedef bool (*BFPTABLE)(DbTable &);

class DbCursor : private ClientObject
{

	friend class DbIndex;

// private:

	DbTable *tab;				// pointer to table
	DbIndex *index;				// pointer to index
	LocalConnection &lcon;		// connection used for buffering
	Int nrec;					// number of records available
	bool morerecs;				// more records available
	AnyValue *rangefrom;		// from range values
	AnyValue *rangeto;			// to range values
	AnyValue *currField;		// pointer for range building
	Int rnflds;					// number of fields in range
	Int flags;					// cursor flags
	BFPTABLE valid;				// Function to validate a record
	Int lmode;					// flags for the lock

	// send search range
	void sendRange();

	void addField(const AnyValue &av);
	void setCond(int f);

	void resetValues();

	// build an index using this cursor
	void buildIndex(Int indexNo, IFPI func);

	void buildCursor(DbTable &t, DbIndex &ind, Int f, void *v);

    DbCursor(const DbCursor &);
    void operator=(const DbCursor &);

public:

	// constructors & destructors

	DbCursor(DbTable &t, const String &indname, Int f = IO_NOT_LOCK,
			 void *v=NULL);
	DbCursor(DbIndex &ind, Int f = IO_NOT_LOCK, void *v=NULL);
	~DbCursor();

	// operators for range setting
	DbCursor &operator<(const AnyValue &av);
	DbCursor &operator<=(const AnyValue &av);
	DbCursor &operator>=(const AnyValue &av);
	DbCursor &operator>(const AnyValue &av);
	DbCursor &operator,(const AnyValue &av);

	// miscelaneous methods
	void reset(Int f = ERR);
	bool fetch();
	bool fetchBack()  { return false; }
	bool operator++() { return fetch(); }
	bool operator--() { return fetchBack(); }
	Int cardinal();
};

class SchRep;

class Schema
{

	friend class DbTable;
	friend class DbField;

// private:

	SchRep *r;			// schema representation

	Schema(SchRep *sr);
	void delSchema();

public:

	// constructors & destructors
	Schema();
	Schema(const Schema &sch);
	Schema(const String &name, Int flag = 0);
	~Schema();

	// schema creation
	void create(const String &descr);

	Int lock();
	void free();			

	Schema switchTo();

	const String &name();

	bool ok() const;
	bool operator!() const;

	Schema &operator=(const Schema &sch);

	StrCursor tables();

	static Schema current;

	// Static members
	static Schema curr();	
	static SchRep *findRep(const String &nm);
	static SchRep *createRep(const String &nm, Int flags = 0);

	// Delete a table
	bool drop(const String &tabName);
	
	// Transaction handling
	static void beginTransaction();
	static void endTransaction(bool sync = false);
	static void abortTransaction();
	static bool didTransaction();
};

#endif
