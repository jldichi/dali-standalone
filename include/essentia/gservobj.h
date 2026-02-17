/********************************************************************
* Copyright (c) 1992 InterSoft Co.  All Rights Reserved
* THIS IS UNPUBLISHED PROPRIETARY SOURCE CODE OF InterSoft Co.
* The copyright notice above does not evidence any
* actual or intended publication of such source code.
*
* $Id: gservobj.h,v 1.3 1997/09/24 15:26:23 eduardoc Exp $
*
* DESCRIPTION
* Class definitions for Generic Server Object type.
*********************************************************************/

#ifndef GSERVOBJ_H
#define GSERVOBJ_H

#include <ifound.h>

#ifdef __GNUC__
#pragma interface
#endif

#ifdef ISDEBUG
#	include <essentia/svdebug.h>
#endif

// Mask (leave high bit untouched for shadow convention!)
// Binary: 10001111 11111111 11111111 11111111
#define OBJID_MASK	(0x8fffffff)

class SvObject;
class ShadowObject;
class ShTube;
class Exception;
class GServObj;
class gtype(PtrArray, GServObj);
class Client;

const Int STAT_INFO_WIDTH = 12;
const Int STAT_INFO_OBJ_WIDTH = 50;

class GServObjStats {
public:
	enum sttype { 
		invalid,
		add,
		del,
		fetch,
		find,
		get,
		put,
		read,
		write,
		nChits, 	// number of cache hits
		nCtries,	// number of cache tries
	};
private:
	sttype	stattype_sd;
	Int	ntimes_sd;
	Int extrainfo_sd;
	static const char *statname[];
public:	
	Int nTimes() { return ntimes_sd; }
    Int nTimes(Int n) { return ntimes_sd += n; }
	void resetNTimes(){ ntimes_sd = 0; }
	Int extraInfo(Int n) { return extrainfo_sd +=n; }
	Int extraInfo() { return extrainfo_sd; }
    sttype statType() { return stattype_sd; }
    void statType(sttype type)  {	stattype_sd = type; }
	static const char * statTypeDescr(sttype t) { 
//		hasta que este terminado
//		assert(t>= invalid && t <= write);
		if (t< invalid || t > write)
			return NULL;

			
		return statname[t]; 
	}
	GServObjStats() { ntimes_sd = 0; stattype_sd = invalid; extrainfo_sd = 0; }

};


class gtype(PtrArray, GServObjStats);
typedef void (GServObj::*SvMethod)(Client *);

class GServObj {

	useIdCheck(GServObj_id);

// private:

	// object's id number
	Int objectId; 

	GServObj(const GServObj &);
    void operator=(const GServObj &);
    
protected:

	// Server objects
	static gtype(PtrArray, GServObj) *_objects;
	static Int nObjects;
	bool accessed;
	gtype (PtrArray, GServObjStats) *gsobjsts_sd;	

public:

	// Constructors & destructors
	GServObj();
	virtual ~GServObj();
	gtype (PtrArray, GServObjStats) *getSObjs() { return gsobjsts_sd;}

	// Execute a desired operation on the object
	void execute(Int operation, Client *cl);

	// Set object's id number
	void setId(Int n);

	// Get object's id number
	Int getId() { return objectId; }
	bool wasAccessed() { return accessed; }
	// Every possible operation should be declared here
	// (because of compiler bug at least one must not be inline)
	virtual void defa(Client *cl) = 0;
	virtual void attach(Client *cl);
	virtual void detach(Client *cl);
	virtual void lock(Client *cl);
	virtual void free(Client *cl);
	virtual void clear(Client *cl);
	virtual void rename(Client *cl);
	virtual void getInfo(Client *cl);
	virtual void getTables(Client *cl);
	virtual void connect(Client *cl);
	virtual void disConnect(Client *cl);
	virtual void getClientsInfo(Client *cl);
	virtual void findRecord(Client *cl);
	virtual void getRecord(Client *cl);
	virtual void putRecord(Client *cl);
	virtual void delRecord(Client *cl);
	virtual void addRecord(Client *cl);
	virtual void addRecordTest(Client *cl);
	virtual void updateRecord(Client *cl);
	virtual void getNextBlock(Client *cl);
	virtual void getPrevBlock(Client *cl);
	virtual void setRange(Client *cl);
	virtual void freeRecord(Client *cl);
	virtual void create(Client *cl);
	virtual void drop(Client *cl);
	virtual void build(Client *cl);
	virtual void insKey(Client *cl);
	virtual void shut(Client *cl);
	virtual void stop(Client *cl);
	virtual void checkPoint(Client *cl);
	virtual void begTransaction(Client *cl);
	virtual void endTransaction(Client *cl);
	virtual void readyToEnd(Client *cl);
	virtual void abortTransaction(Client *cl);
	virtual void newVersion(Client *cl);
	virtual void cardinal(Client *cl);
	virtual void setAttrib(Client *cl);
	virtual void shadowDbg(Client *cl);
	virtual void invalid(Client *cl);
	virtual void monitor(Client *cl);
	virtual void getDataFiles(Client *cl);
	virtual void getVersInfo(Client *cl);
	virtual void setPrivg(Client *cl);
	virtual void alter(Client *cl);
	virtual void copy(Client *cl);
	virtual void replace(Client *cl);
	virtual void estimation(Client *cl);
	virtual void activation(Client *cl);
	virtual void objMethodExec(Client *cl);
	virtual void dbg(Client *cl);
 	virtual void getMethInfo(String &a);

	static const String &msgName(Int n);
	virtual const String &name() const;

	virtual Int objectType() const = 0;
	static GServObj *&object(Int id);
	static bool validObject(Int id);

	virtual SvObject	 *getSvObject();
	virtual ShadowObject *getShadowObject();
	virtual ShTube		 *getShTube();
	
	// +++ Debug +++
	virtual String objType() = 0;
	// --- Debug ---
};

#endif // GSERVOBJ
