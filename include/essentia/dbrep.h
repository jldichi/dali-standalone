/********************************************************************
* Copyright (c) 1992 InterSoft Co.  All Rights Reserved
* THIS IS UNPUBLISHED PROPRIETARY SOURCE CODE OF InterSoft Co.
* The copyright notice above does not evidence any
* actual or intended publication of such source code.
*
* $Id: dbrep.h,v 1.1.1.1 1996/04/16 18:51:47 gustavof Exp $
*
* DESCRIPTION
* Class definitions for Internal data base Representations
*********************************************************************/

#ifndef DBREP_H
#define DBREP_H

#include <ifound.h>

#ifdef __GNUC__
#pragma interface
#endif


#include <ifound/str.h>
#include <essentia/dbmcom.h>
#include <essentia/dbclobj.h>
#include <ifound/anyvalue.h>
#include <essentia/criobj.h>

class DbConnection; 

class DbTableRep;
class gtype(PtrArray, DbTableRep);

class SchRep : public ClientObject, public CriticalObject
{
	friend class Schema;

// private:

	Int flags;
	Int status;

	// Array de Tablas del Esquema
	gtype(PtrArray,DbTableRep) &tarray;

public:

	// constructores y destructores
	SchRep(const String &nm, Int f);
	~SchRep();

	void create(const String &descr);
	
	DbTableRep *getDbTableRep(const String &nm, Int v);
	DbTableRep *getDbTableRep(Int oid);
	void delDbTableRep(DbTableRep *dtr);
	void addDbTableRep(DbTableRep *dtr);
	void delDbTableRep(const String &nm);

	bool ok() const;

	String tables();
};

class DbTable;
class DbIndex;
class DbFieldRep;
class gtype(PtrArray,DbFieldRep);
class gtype(PtrArray,DbIndex);
class TabExpr;
class InCheck;

class DbTableRep : public ClientObject {

	friend class DbTable;

// private:

	UnInt cid;				// class id of this table
	Int nflds;				// cantidad de campos 
	Int vers;				// version de la tabla

	// arreglo de estructura de campos
	gtype(PtrArray,DbFieldRep) &farray;

	SchRep *schr;			// esquema al que pertenece

	// arreglo de indices
	gtype(PtrArray,DbIndex) &iarray;

public:

	// constructores y destructores
	DbTableRep(SchRep *sr, const String &nm, Int v = 0, Int nf = 0);
	~DbTableRep();

	DbFieldRep *getDbFieldRep(const String &nm);
	DbFieldRep *getDbFieldRep(Int n);
	DbIndex    *getDbIndex(const String &indname = NULL_STRING);
	DbIndex    *getDbIndex(Int n);

	SchRep     *getSchRep();

	Int field(DbFieldRep *dfr);

	Int indexNumber(DbIndex *ir);

	Int delIndex(DbIndex *ix);
	Int addIndex(DbIndex *ix);                    
	
	Int version();

	bool close();
};

class DbFieldRep {

	friend class DbField;
	friend class DbTable;

// private:

	String f_name;		// nombre del campo
	String f_mask;		// mascara del campo
	TabExpr *def;		// valor default del campo
	SchRep *schr;		// puntero el schema
	DbTableRep *tabr;	// puntero a la tabla
	Int f_type;			// tipo de campo
	Int dim;			// dimension del campo
	Int len;			// longitud del campo
	Int ndec;			// decimales del campo
	Int inds;			// indices al que pertence el campo
	InCheck *inchk;		// in table del campo
	TabExpr *f_expr;	// check del campo
    
    void operator=(const DbFieldRep &)  ;
    
public:

	// constructores y destructores
	DbFieldRep();
	DbFieldRep(const DbFieldRep &dfr);
	DbFieldRep(const String &nm, Int l, Int nd, Int flgs,
			   const String &mask, TabExpr *dflt, Int type);
	~DbFieldRep();

	// rutina que agrega un indice el campo
	void addIndex(Int nind);
	
	// rutina para construirlo a partir de la conexion
	void cons(DbConnection *con, SchRep *sr, DbTableRep *tr);

	// retorna los datos del campo
	Int getType() const { return f_type; }

	// retorna el nombre del campo
	const String &name() const { return f_name; }

	// retorna el default del campo
	TabExpr *deflt() 	   	   { return def; }

	// retorna la mascara del campo
	const String &mask() const { return f_mask; }

	// retorna la cantidad de decimales
	Int nDec() const { return ndec; }

	// Is the field valid?
	bool valid(DbTable *tab) const;

	// numero de campo dentro de la tabla
	Int nField() const;

	InCheck *inCheck() const { return inchk; }
};

#endif // DBREP_H
