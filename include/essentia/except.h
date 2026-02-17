/********************************************************************
* Copyright (c) 1992 InterSoft Co.  All Rights Reserved
* THIS IS UNPUBLISHED PROPRIETARY SOURCE CODE OF InterSoft Co.
* The copyright notice above does not evidence any
* actual or intended publication of such source code.
*
* $Id: except.h,v 1.2 1997/09/24 15:27:18 eduardoc Exp $
*
* DESCRIPTION
* Class definitions for Exception type.
*********************************************************************/

#ifndef EXCEPT_H
#define EXCEPT_H

#include <ifound.h>

#ifdef __GNUC__
#pragma interface
#endif

#include <ifound/itypes.h>
#include <essentia/spriv.h>

enum {
	E_NULL,
	E_DEADLOCK,
	E_LOCKTABLEFULL,
	E_NOPRIVILEGE,
	E_LOCKED,
	E_NOINTEGRITY,
	E_BEINGUSED,
	E_NOTIFYTABLEFULL,
	E_BADVERSION,
	E_CLLOCKTABLEFULL,
	E_NOINDEXES,
	E_INVALIDINDEXNUM,
	E_INVALIDTABLE,
	E_INTRANS,
	E_NOTINTRANS,
	E_MODLOCKED,
	E_CANTCONVERTTABLE,
	E_INVALIDOBJECTID,
	E_MSGOUTOFBOUNDS,
	E_INVALIDCLIENTID,
	E_SHADOWERROR,
	E_NOTIFICATION,
	E_INVALIDMSGONOBJECT,
	E_TOOMANYCLIENTS,
	E_SERVERISDEAD,
	E_INCOMPATIBLEVERSIONS,
	E_INVALIDFORMIRROR,
	E_ZOMBIECLIENT,
	E_DUPLICATEDKEY,
	E_PENDLOCKNOTIF,
	E_KEYDOESNOTEXIST,
	E_INVALIDTIMESTAMP,
	E_OPTTRANSCORRUPT,
	E_INVALIDLOGIN,
	E_TOOMANYATTACHS,
	E_INVALIDDATA,
	E_ADMINMODE,

	// Must be the last one!
	E_LAST,
};

class Connection;

class Locked;

class Exception
	// Exception simulation class
{

// private:

	// To prevent it's use
	Exception &operator=(const Exception &e); 

protected:

	Int id;

public:

	explicit Exception(Int i = E_NULL)			{ id = i; }
	Exception(const Exception &e) : id(e.id)	{ }
	virtual ~Exception()						{ }
	static void last(const Exception &e);
	static const Exception &last();

	Int code() const					{ return id; }

	operator int() const				{ return id; } 


	virtual String message() const;

	virtual void fill(Connection *c);
	virtual void send(Connection *c) const;

	virtual void raise() const;
	virtual Exception *clone() const		{ return new Exception(*this);	}
	virtual bool isPendNotification() const	{ return false; 				}

	static void init(bool all);
	static void clear();

	static Exception *getException(UnShort id);

	virtual const Locked *getLocked() const	{ return NULL;					}
	
#ifdef ISDEBUG
	// for debugging only
	static void printLast();
#endif
};

#define lastException	(Exception::last())

class DeadLock : public Exception {

// private:

	String pname;

public:

	DeadLock();
	explicit DeadLock(const String &pname);
	DeadLock(const DeadLock &dl)
	:	Exception(dl), pname(dl.pname) { }
	~DeadLock();
	Exception *clone() const { return new DeadLock(*this); }

	void fill(Connection *c);
	void send(Connection *c) const;
	String message() const;
};

class LockTableFull : public Exception {

// private:

	Int size;

public:

	LockTableFull();
	explicit LockTableFull(Int sz);
	LockTableFull(const LockTableFull &ltf)
	:	Exception(ltf), size(ltf.size) { }
	~LockTableFull();
	Exception *clone() const { return new LockTableFull(*this); }

	void fill(Connection *c);
	void send(Connection *c) const;
	String message() const;
};

class ClLockTableFull : public Exception {

// private:

	Int size;

public:

	ClLockTableFull();
	explicit ClLockTableFull(Int sz);
	ClLockTableFull(const ClLockTableFull &cltf)
	:	Exception(cltf), size(cltf.size) { }
	~ClLockTableFull();
	Exception *clone() const { return new ClLockTableFull(*this); }

	void fill(Connection *c);
	void send(Connection *c) const;
	String message() const;
};

class NotifyTableFull : public Exception {

// private:

	Int size;

public:

	NotifyTableFull();
	explicit NotifyTableFull(Int sz);
	NotifyTableFull(const NotifyTableFull &ntf)
	:	Exception(ntf), size(ntf.size) { }
	~NotifyTableFull();
	Exception *clone() const { return new NotifyTableFull(*this); }

	void fill(Connection *c);
	void send(Connection *c) const;
	String message() const;
};

class NoPrivilege : public Exception {

// private:

	Int privg;
	Int owner;
	Int user;
	String objName;
	bool isSchema;

public:

	NoPrivilege();
	explicit NoPrivilege(Int priv, Int o = ERR, Int u = ERR, 
			const String& n = NULL_STRING, bool isSchema = false);
	NoPrivilege(const NoPrivilege &np)
	:	Exception(np), privg(np.privg), owner(np.owner), 
		user(np.user), objName(np.objName), isSchema(np.isSchema) { }
	~NoPrivilege();
	Exception *clone() const { return new NoPrivilege(*this); }

	void fill(Connection *c);
	void send(Connection *c) const;
	String message() const;
	String permName(Int isSchePriv) const;
};


class Locked : public Exception {

// private:

	String owner_d;

public:

	Locked();
	explicit Locked(const String &own);
	Locked(const Locked &l)
	:	Exception(l), owner_d(l.owner_d)	
	{ }
	~Locked();
	Exception *clone() const 		{ return new Locked(*this);	}

	void fill(Connection *c);
	void send(Connection *c) const;
	String message() const;
	
	const String &owner() const		{ return owner_d;			}

	const Locked *getLocked() const	{ return this;				}
};


class NoIntegrity : public Exception {

// private:

public:

	NoIntegrity();
	NoIntegrity(const NoIntegrity &ni) : Exception(ni) { }
	~NoIntegrity();
	Exception *clone() const { return new NoIntegrity(*this); }

	String message() const;
};


class BeingUsed : public Exception {

// private:

	Int nref;

public:

	BeingUsed();
	explicit BeingUsed(Int n);
	BeingUsed(const BeingUsed &bu)
	:	Exception(bu), nref(bu.nref) { }
	~BeingUsed();
	Exception *clone() const { return new BeingUsed(*this); }

	void fill(Connection *c);
	void send(Connection *c) const;
	String message() const;
};

class BadVersion : public Exception {

// private:

	Int num;

public:

	BadVersion();
	explicit BadVersion(Int n);
	BadVersion(const BadVersion &bv)
	:	Exception(bv), num(bv.num) { }
	~BadVersion();
	Exception *clone() const { return new BadVersion(*this); }

	void fill(Connection *c);
	void send(Connection *c) const;
	String message() const;
};

class NoIndexes : public Exception {

// private:

public:

	NoIndexes();
	NoIndexes(const NoIndexes &ni) : Exception(ni) { }
	~NoIndexes();
	Exception *clone() const { return new NoIndexes(*this); }

	String message() const;
};


class InvalidIndexNum : public Exception {

// private:

	Int indNum;

public:

	explicit InvalidIndexNum(Int n = ERR);
	InvalidIndexNum(const InvalidIndexNum &iin)
	:	Exception(iin), indNum(iin.indNum) { }
	~InvalidIndexNum();
	Exception *clone() const { return new InvalidIndexNum(*this); }

	void fill(Connection *c);
	void send(Connection *c) const;

	String message() const;
};


class InvalidTable : public Exception {

// private:

	Int id;

public:

	explicit InvalidTable(Int n);
	InvalidTable();
	InvalidTable(const InvalidTable &it)
	:	Exception(it), id(it.id) { }
	~InvalidTable();
	Exception *clone() const { return new InvalidTable(*this); }

	void fill(Connection *c);
	void send(Connection *c) const;
	String message() const;
};


class InTrans : public Exception {

// private:

public:

	InTrans();
	InTrans(const InTrans &it) : Exception(it) { }
	~InTrans();
	Exception *clone() const { return new InTrans(*this); }

	String message() const;
};


class NotInTrans : public Exception {

// private:

public:

	NotInTrans();
	NotInTrans(const NotInTrans &nit) : Exception(nit) { }
	~NotInTrans();
	Exception *clone() const { return new NotInTrans(*this); }

	String message() const;
};

class ModLocked : public Exception {

// private:

	String owner;

public:

	ModLocked();
	explicit ModLocked(const String &own);
	explicit ModLocked(const Locked &e);
	ModLocked(const ModLocked &l)
	:	Exception(l), owner(l.owner) { }
	~ModLocked();
	Exception *clone() const { return new ModLocked(*this); }

	void fill(Connection *c);
	void send(Connection *c) const;
	String message() const;
};

class CantConvertTable : public Exception {

// private:

	String errMsg;

public:

	CantConvertTable();
	explicit CantConvertTable(const String &msg);
	CantConvertTable(const CantConvertTable &nit)
	:	Exception(nit), errMsg(nit.errMsg) { }
	~CantConvertTable();
	Exception *clone() const { return new CantConvertTable(*this); }

	void fill(Connection *c);
	void send(Connection *c) const;

	String message() const;
};

class InvalidObjectId : public Exception {

// private:

	Int oId;

public:

	InvalidObjectId();
	explicit InvalidObjectId(Int o);
	InvalidObjectId(const InvalidObjectId &nit)
	:	Exception(nit), oId(nit.oId) { }
	~InvalidObjectId();
	Exception *clone() const { return new InvalidObjectId(*this); }

	void fill(Connection *c);
	void send(Connection *c) const;

	String message() const;
};

class MsgOutOfBounds : public Exception {

// private:

	Int msgCode;

public:

	explicit MsgOutOfBounds(Int mc);
	MsgOutOfBounds();
	MsgOutOfBounds(const MsgOutOfBounds &nit)
	:	Exception(nit), msgCode(nit.msgCode) { }
	~MsgOutOfBounds();
	Exception *clone() const { return new MsgOutOfBounds(*this); }

	void fill(Connection *c);
	void send(Connection *c) const;

	String message() const;
};

class InvalidClientId : public Exception {

// private:

	Int cId;

public:

	InvalidClientId();
	explicit InvalidClientId(Int c);
	InvalidClientId(const InvalidClientId &nit)
	:	Exception(nit), cId(nit.cId) { }
	~InvalidClientId();
	Exception *clone() const { return new InvalidClientId(*this); }

	void fill(Connection *c);
	void send(Connection *c) const;

	String message() const;
};

class ZombieClient : public Exception {

// private:

	Int cId;

public:

	ZombieClient();
	explicit ZombieClient(Int c);
	ZombieClient(const ZombieClient &nit)
	:	Exception(nit), cId(nit.cId) { }
	~ZombieClient();
	Exception *clone() const { return new ZombieClient(*this); }

	void fill(Connection *c);
	void send(Connection *c) const;

	String message() const;
};

class ShadowError : public Exception {

// private:

	String msg;
	
public:

	ShadowError();
	explicit ShadowError(const String &m) : msg(m), Exception(E_SHADOWERROR)
	{ }
	ShadowError(const ShadowError &nit) : Exception(nit) { }
	~ShadowError();
	Exception *clone() const { return new ShadowError(*this); }

	void fill(Connection *c);
	void send(Connection *c) const;

	String message() const;
};

class Notification : public Exception {

// private:

public:

	Notification();
	Notification(const Notification &nit) : Exception(nit) { }
	~Notification();
	Exception *clone() const { return new Notification(*this); }

	String message() const;
};


class InvalidMsgOnObject : public Exception {

// private:

	Int obj;
	Int msg;

public:

	InvalidMsgOnObject();
	InvalidMsgOnObject(Int o, Int m);
	InvalidMsgOnObject(const InvalidMsgOnObject &nit) : Exception(nit) { }
	~InvalidMsgOnObject();
	Exception *clone() const { return new InvalidMsgOnObject(*this); }

	void fill(Connection *c);
	void send(Connection *c) const;

	String message() const;
};


class TooManyClients : public Exception {

// private:

	Int maxClients;

public:

	explicit TooManyClients(Int n = ERR);
	TooManyClients(const TooManyClients &tmc)
	:	Exception(tmc)
	{ maxClients = tmc.maxClients; }
	~TooManyClients();
	Exception *clone() const { return new TooManyClients(*this); }

	void fill(Connection *c);
	void send(Connection *c) const;

	String message() const;
};

class ServerIsDead : public Exception {

	String serverName;

public:

	explicit ServerIsDead(const String &sv = NULL_STRING);
	ServerIsDead(const ServerIsDead &sid)
	:	Exception(sid) { }
	~ServerIsDead();
	Exception *clone() const { return new ServerIsDead(*this); }

	String message() const;
};

class IncompatibleVersions : public Exception {

// private:

	Int cVers;
	Int sVers;

public:

	IncompatibleVersions();
	IncompatibleVersions(Int cV, Int sV);
	IncompatibleVersions(const IncompatibleVersions &icv)
	:	Exception(icv), cVers(icv.cVers), sVers(icv.sVers) { }
	~IncompatibleVersions();
	Exception *clone() const { return new IncompatibleVersions(*this); }

	void fill(Connection *c);
	void send(Connection *c) const;

	String message() const;
};

class InvalidForMirror : public Exception {

	String essMsg;

public:

	explicit InvalidForMirror(const String &eMsg = NULL_STRING);
	InvalidForMirror(const InvalidForMirror &ifm)
	:	Exception(ifm) { }
	~InvalidForMirror();
	Exception *clone() const { return new InvalidForMirror(*this); }

	void fill(Connection *c);
	void send(Connection *c) const;

	String message() const;
};

class DuplicatedKey : public Exception {

// private:

	String indName;
	String keyStr;

public:

	explicit DuplicatedKey(const String &iName = NULL_STRING,
			const String &str = NULL_STRING);
	DuplicatedKey(const DuplicatedKey &dk)
	:	Exception(dk), indName(dk.indName), keyStr(dk.keyStr) { }
	~DuplicatedKey();
	Exception *clone() const { return new DuplicatedKey(*this); }

	void fill(Connection *c);
	void send(Connection *c) const;

	String message() const;
};

// This class is an abstract one.  No PendNotification exceptions should
// be instantiated.
class PendNotification : public Exception {

public:

	PendNotification(const PendNotification &pn)
	:	Exception(pn) { }
	explicit PendNotification(Int theId) : Exception(theId) { }
	bool isPendNotification() const { return true; }
};

class PendLockNotif : public PendNotification {

// private:

	String tableName;
	String clientName;

public:

	explicit PendLockNotif(const String &tName = NULL_STRING,
			const String &cName = NULL_STRING);
	PendLockNotif(const PendLockNotif &n)
	:	PendNotification(n), tableName(n.tableName),
		clientName(n.clientName)
	{ }
	~PendLockNotif();
	Exception *clone() const { return new PendLockNotif(*this); }

	void fill(Connection *c);
	void send(Connection *c) const;

	String message() const;
};

class KeyDoesNotExist : public Exception {

// private:

	String indName;
	String keyStr;

public:

	explicit KeyDoesNotExist(const String &iName = NULL_STRING,
			const String &str = NULL_STRING);
	KeyDoesNotExist(const KeyDoesNotExist &nk)
	:	Exception(nk), indName(nk.indName), keyStr(nk.keyStr) { }
	~KeyDoesNotExist();
	Exception *clone() const { return new KeyDoesNotExist(*this); }

	void fill(Connection *c);
	void send(Connection *c) const;

	String message() const;
};

class InvalidTimeStamp : public Exception {

// private:

	String tableName;
	Time cTS;
	Time mTS;
	Time sTS;

public:

	InvalidTimeStamp();
	InvalidTimeStamp(const String &tName, const Time &cts, const Time &mts,
			const Time &sts);
	InvalidTimeStamp(const InvalidTimeStamp &its)
	:	Exception(its), tableName(its.tableName), cTS(its.cTS),
		mTS(its.mTS), sTS(its.sTS) { }
	~InvalidTimeStamp();
	Exception *clone() const { return new InvalidTimeStamp(*this); }

	void fill(Connection *c);
	void send(Connection *c) const;

	String message() const;
};

class OptTransCorrupt : public Exception {

// private:

	String tableName;
	Time mTS;
	Time tTS;

public:

	OptTransCorrupt();
	OptTransCorrupt(const String &tName, const Time &mts,
			const Time &tts);
	OptTransCorrupt(const OptTransCorrupt &its)
	:	Exception(its), tableName(its.tableName), mTS(its.mTS),
		tTS(its.tTS) { }
	~OptTransCorrupt();
	Exception *clone() const { return new OptTransCorrupt(*this); }

	void fill(Connection *c);
	void send(Connection *c) const;

	String message() const;
};

class InvalidLogin : public Exception {

// private:

	String usr;
	String reason;

public:

	InvalidLogin();
	InvalidLogin(const String &u, const String &r);
	InvalidLogin(const InvalidLogin &ip)
	:	Exception(ip), usr(ip.usr), reason(ip.reason)
	{ }
	~InvalidLogin();
	Exception *clone() const { return new InvalidLogin(*this); }

	void fill(Connection *c);
	void send(Connection *c) const;

	String message() const;
};

#ifdef max
#	undef max
#endif

class TooManyAttachs : public Exception {

// private:

	Int max;

public:

	TooManyAttachs();
	explicit TooManyAttachs(Int sz);
	TooManyAttachs(const TooManyAttachs &tma)
	:	Exception(tma), max(tma.max) { }
	~TooManyAttachs();
	Exception *clone() const { return new TooManyAttachs(*this); }

	void fill(Connection *c);
	void send(Connection *c) const;
	String message() const;
};

class InvalidData : public Exception {

// private:

	String descr;

public:

	InvalidData();
	explicit InvalidData(const String &descr);
	InvalidData(const InvalidData &id)
	:	Exception(id), descr(id.descr) { }
	~InvalidData();
	Exception *clone() const { return new InvalidData(*this); }

	void fill(Connection *c);
	void send(Connection *c) const;
	String message() const;
};

class AdminMode : public Exception {

// private:

	String user;

public:

	AdminMode();
	explicit AdminMode(const String &descr);
	AdminMode(const AdminMode &am)
	:	Exception(am), user(am.user) { }
	~AdminMode();
	Exception *clone() const { return new AdminMode(*this); }

	void fill(Connection *c);
	void send(Connection *c) const;
	String message() const;
};

#endif
