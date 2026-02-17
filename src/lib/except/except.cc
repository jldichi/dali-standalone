/********************************************************************
* Copyright (c) 1992 InterSoft Co.  All Rights Reserved
* THIS IS UNPUBLISHED PROPRIETARY SOURCE CODE OF InterSoft Co.
* The copyright notice above does not evidence any
* actual or intended publication of such source code.
*
* $Id: except.cc,v 1.6 2000/03/13 15:37:07 diegon Exp $
*
* DESCRIPTION:
* Exception implementation
*********************************************************************/

#ifdef __GNUC__
#pragma implementation
#endif

#ifdef __NT__
#	include <stdlib.h>
#endif

#include <ifound.h>
#include <essentia/except.h>
#include <essentia/connect.h>

static Exception *table[E_LAST] = { NULL };
static Exception *last_ = NULL;

void Exception::last(const Exception &e)
{
	assert(last_ != NULL);
#ifndef __VC__
	tr_delete(last_);
#else
	tr_delete(&last_);
#endif
	last_ = e.clone();
}

const Exception &Exception::last()
{
	assert(last_ != NULL);
	return *last_;
}

Exception *Exception::getException(UnShort eid)
{
	return eid < E_LAST ? table[eid] : NULL;
}

void Exception::init(bool all)
{
	if (last_ == NULL) {
		last_ = tr_new Exception();
		table[E_NULL] = tr_new Exception(E_NULL);

		if (all) {
			table[E_DEADLOCK]				= tr_new DeadLock;
			table[E_LOCKTABLEFULL]			= tr_new LockTableFull;
			table[E_NOPRIVILEGE]			= tr_new NoPrivilege;
			table[E_LOCKED]					= tr_new Locked;
			table[E_NOINTEGRITY]			= tr_new NoIntegrity;
			table[E_BEINGUSED]				= tr_new BeingUsed;
			table[E_NOTIFYTABLEFULL]		= tr_new NotifyTableFull;
			table[E_BADVERSION]				= tr_new BadVersion;
			table[E_CLLOCKTABLEFULL]		= tr_new ClLockTableFull;
			table[E_NOINDEXES]				= tr_new NoIndexes;
			table[E_INVALIDINDEXNUM]		= tr_new InvalidIndexNum;
			table[E_INVALIDTABLE]			= tr_new InvalidTable;
			table[E_INTRANS]				= tr_new InTrans;
			table[E_NOTINTRANS]				= tr_new NotInTrans;
			table[E_MODLOCKED]				= tr_new ModLocked;
			table[E_CANTCONVERTTABLE]		= tr_new CantConvertTable;
			table[E_INVALIDOBJECTID]		= tr_new InvalidObjectId;
			table[E_MSGOUTOFBOUNDS]			= tr_new MsgOutOfBounds;
			table[E_INVALIDCLIENTID]		= tr_new InvalidClientId;
			table[E_SHADOWERROR]			= tr_new ShadowError;
			table[E_NOTIFICATION]			= tr_new Notification;
			table[E_INVALIDMSGONOBJECT]		= tr_new InvalidMsgOnObject;
			table[E_TOOMANYCLIENTS]			= tr_new TooManyClients;
			table[E_SERVERISDEAD]			= tr_new ServerIsDead;
			table[E_INCOMPATIBLEVERSIONS]	= tr_new IncompatibleVersions;
			table[E_INVALIDFORMIRROR]		= tr_new InvalidForMirror;
			table[E_ZOMBIECLIENT]			= tr_new ZombieClient;
			table[E_DUPLICATEDKEY]			= tr_new DuplicatedKey;
			table[E_PENDLOCKNOTIF]			= tr_new PendLockNotif;
			table[E_KEYDOESNOTEXIST]		= tr_new KeyDoesNotExist;
			table[E_INVALIDTIMESTAMP]		= tr_new InvalidTimeStamp;
			table[E_OPTTRANSCORRUPT]		= tr_new OptTransCorrupt;
			table[E_INVALIDLOGIN]			= tr_new InvalidLogin;
			table[E_TOOMANYATTACHS]			= tr_new TooManyAttachs;
			table[E_INVALIDDATA]			= tr_new InvalidData;
			table[E_ADMINMODE]				= tr_new AdminMode;
		}
	}
}

void Exception::clear()
{
	if (last_ != NULL) {
		tr_delete(last_);
		last_ = NULL;
		for (Int i = E_NULL; i < E_LAST && table[i]; i++)
			tr_delete(table[i]);
	}
}

String Exception::message() const
{
	return NULL_STRING;
}

void Exception::fill(Connection *) { }

void Exception::send(Connection *) const { }

void Exception::raise() const
{
	error(message());
}

#ifdef DEBUG
void Exception::printLast()
{
	String tmp = last().message();
	if (!tmp)
		tmp = "NULL";
	fprintf(stderr, "lastException: %s\n", toCharPtr(tmp));
}
#endif

// class Deadlock

DeadLock::DeadLock(const String &pn)
:	Exception(E_DEADLOCK),
	pname(pn)
{ }

DeadLock::DeadLock()
:	Exception(E_DEADLOCK),
	pname(NULL_STRING)
{ }

DeadLock::~DeadLock() { }

void DeadLock::fill(Connection *c)
{
	*c >> pname;
}

void DeadLock::send(Connection *c) const
{
	*c << pname;
}

String DeadLock::message() const
{
	return String(libMsg("DEADLOCK"))+" "+pname;
}

// end of DeadLock class


// LockTableFull class

LockTableFull::LockTableFull(Int sz)
:	Exception(E_LOCKTABLEFULL),
	size(sz)
{ }

LockTableFull::LockTableFull()
:	Exception(E_LOCKTABLEFULL),
	size(ERR)
{ }

LockTableFull::~LockTableFull() { }

void LockTableFull::fill(Connection *c)
{
	*c >> size;
}

void LockTableFull::send(Connection *c) const
{
	*c << size;
}

String LockTableFull::message() const
{
	return String(libMsg("LOCKTABLEFULL"))+" "+toString(size);
}

// end of LockTableFull class

// ClLockTableFull class

ClLockTableFull::ClLockTableFull(Int sz)
:	Exception(E_CLLOCKTABLEFULL),
	size(sz)
{ }

ClLockTableFull::ClLockTableFull()
:	Exception(E_CLLOCKTABLEFULL),
	size(ERR)
{ }

ClLockTableFull::~ClLockTableFull() { }

void ClLockTableFull::fill(Connection *c)
{
	*c >> size;
}

void ClLockTableFull::send(Connection *c) const
{
	*c << size;
}

String ClLockTableFull::message() const
{
	return String(libMsg("CLLOCKTABLEFULL"))+" "+toString(size);
}

// end of ClLockTableFull class

// NotifyTableFull class

NotifyTableFull::NotifyTableFull(Int sz)
:	Exception(E_NOTIFYTABLEFULL),
	size(sz)
{ }

NotifyTableFull::NotifyTableFull()
:	Exception(E_NOTIFYTABLEFULL),
	size(ERR)
{ }

NotifyTableFull::~NotifyTableFull() { }

void NotifyTableFull::fill(Connection *c)
{
	*c >> size;
}

void NotifyTableFull::send(Connection *c) const
{
	*c << size;
}

String NotifyTableFull::message() const
{
	return String(libMsg("NOTIFYTABLEFULL"))+" "+toString(size);
}

// end of NotifyTableFull class

// NoPrivilege class

/**
 * This is the constructor of NoPrivilege Class.
 *
 * @param priv Permission that is being respected.
 * @param o Owner of the Object (schema or table).
 * @param u User that is trying to perform operation over the restricted Object.
 * @param n Name of the restricted Object. 
 * @param isSchema Specifies whether the Object is a Table or a Schema.
 */
NoPrivilege::NoPrivilege(Int priv, Int o, Int u, const String& n, bool isSche)
:	Exception(E_NOPRIVILEGE),
	privg(priv),
	owner(o),
	user(u),
	objName(n),
	isSchema(isSche)
{
}

NoPrivilege::NoPrivilege() 
:	Exception(E_NOPRIVILEGE),
	privg(ERR),
	owner(ERR),
	user(ERR),
	objName(NULL_STRING)
{ 
}

NoPrivilege::~NoPrivilege() 
{
}

void NoPrivilege::fill(Connection *c)
{
	*c >> privg >> owner >> user >> objName;
}

void NoPrivilege::send(Connection *c) const
{
	*c << privg << owner << user << objName;
}

String NoPrivilege::message() const
{
	return doFormat(libMsg("NOPRIVILEGE"), toCharPtr(permName(isSchema)), 
				user, toCharPtr(objName), owner);
}

/**
 * This method returns the 'name' of the privilege not respected.
 *
 * @param isSchePriv specifies whether the permission not respected is
 *                    related to a Table or a Schema.
 */
String NoPrivilege::permName(Int isSchePriv) const
{
	if (isSchePriv) {
		switch (privg) {
			case PRIVG_USE:
				return libMsg("USE_PERM");
			case PRIVG_TEMP:
				return libMsg("TEMP_PERM");
			case PRIVG_MANIP:
				return libMsg("MANIP_PERM");
			case PRIVG_ALTER:
				return libMsg("ALTER_PERM");
			default:
				//exit(0);
				assert(0);
				return(0);
		}	
	}
	else {
		switch (privg) {
			case PRIVG_INSERT:
				return libMsg("INSERT_PERM");
			case PRIVG_DELETE:
				return libMsg("DELETE_PERM");
			case PRIVG_UPDATE:
				return libMsg("UPDATE_PERM");
			case PRIVG_SELECT:
				return libMsg("SELECT_PERM");
			default:
				//exit(0);
				assert (0);
				return (0);
		}	
	}
}
// end of NoPrivilege class

// Locked class

Locked::Locked(const String &own)
:	Exception(E_LOCKED),
	owner_d(own)
{ }

Locked::Locked()
:	Exception(E_LOCKED),
	owner_d(NULL_STRING)
{ }

Locked::~Locked() { }

void Locked::fill(Connection *c)
{
	*c >> owner_d;
}

void Locked::send(Connection *c) const
{
	*c << owner_d;
}

String Locked::message() const
{
	String theOwner = owner_d;
	if (theOwner == NULL_STRING)
		theOwner = libMsg("UNKNOWNM");
	return toString(libMsg("LOCKED"), toCharPtr(theOwner));
}

// end of Locked class


// NoIntegrity class

NoIntegrity::NoIntegrity()
:	Exception(E_NOINTEGRITY)
{ }

NoIntegrity::~NoIntegrity() { }

String NoIntegrity::message() const
{
	return libMsg("NOINTEGRITY");
}

// end of NoIntegrity class


// BeingUsed class

BeingUsed::BeingUsed()
:	Exception(E_BEINGUSED),
	nref(ERR)
{ }

BeingUsed::BeingUsed(Int n)
:	Exception(E_BEINGUSED),
	nref(n)
{ }

BeingUsed::~BeingUsed() { }

String BeingUsed::message() const
{
	return String(libMsg("BEINGUSED"))+": "+toString(nref);
}

void BeingUsed::fill(Connection *c)
{
	*c >> nref;
}

void BeingUsed::send(Connection *c) const
{
	*c << nref;
}

// end of BeingUsed class

// BadVersion class

BadVersion::BadVersion(Int n)
:	Exception(E_BADVERSION),
	num(n)
{ }

BadVersion::BadVersion()
:	Exception(E_BADVERSION),
	num(ERR)
{ }

BadVersion::~BadVersion() { }

void BadVersion::fill(Connection *c)
{
	*c >> num;
}

void BadVersion::send(Connection *c) const
{
	*c << num;
}

String BadVersion::message() const
{
	return String(libMsg("BADVERSION"))+" "+toString(num);
}

// end of BadVersion class

// NoIndexes class

NoIndexes::NoIndexes() 
:	Exception(E_NOINDEXES)
{ }

NoIndexes::~NoIndexes() { }

String NoIndexes::message() const
{
	return String(libMsg("NOINDEXES"));
}

// end of NoIndexes class

// InvalidIndexNum class

InvalidIndexNum::InvalidIndexNum(Int ind)
:	Exception(E_INVALIDINDEXNUM),
	indNum(ind)
{ }

InvalidIndexNum::~InvalidIndexNum() { }

void InvalidIndexNum::fill(Connection *c)
{
	*c >> indNum;
}

void InvalidIndexNum::send(Connection *c) const
{
	*c << indNum;
}

String InvalidIndexNum::message() const
{
	return String(libMsg("INVALIDINDEXNUM"))+" "+toString(indNum);
}

// end of InvalidIndexNum class


// InvalidTable class

InvalidTable::InvalidTable(Int n)
:	Exception(E_INVALIDTABLE),
	id(n)
{ }

InvalidTable::InvalidTable()
:	Exception(E_INVALIDTABLE),
	id(ERR)
{ }

InvalidTable::~InvalidTable() { }

String InvalidTable::message() const
{
	return String(libMsg("INVALIDTABLE"))+" "+toString(id);
}

void InvalidTable::fill(Connection *c)
{
	*c >> id;
}

void InvalidTable::send(Connection *c) const
{
	*c << id;
}

// end of InvalidTable class


// InTrans class

InTrans::InTrans()
:	Exception(E_INTRANS)
{ }

InTrans::~InTrans() { }

String InTrans::message() const
{
	return libMsg("INTRANS");
}

// end of InTrans class


// NotInTrans class

NotInTrans::NotInTrans() 
:	Exception(E_NOTINTRANS)
{ }

NotInTrans::~NotInTrans() { }

String NotInTrans::message() const
{
	return libMsg("NOTINTRANS");
}

// end of NotInTrans class


// ModLocked class

ModLocked::ModLocked(const String &own)
:	Exception(E_MODLOCKED),
	owner(own)
{ }

ModLocked::ModLocked()
:	Exception(E_MODLOCKED),
	owner(NULL_STRING)
{ }

ModLocked::ModLocked(const Locked &e)
:	Exception(E_MODLOCKED),
	owner(e.owner())
{
}

ModLocked::~ModLocked() { }

void ModLocked::fill(Connection *c)
{
	*c >> owner;
}

void ModLocked::send(Connection *c) const
{
	*c << owner;
}

String ModLocked::message() const
{
	String theOwner = owner;
	if (theOwner == NULL_STRING)
		theOwner = libMsg("UNKNOWNM");
	return toString(libMsg("MODLOCKED"), toCharPtr(theOwner));
}

// end of ModLocked class

// CantConvertTable class

CantConvertTable::CantConvertTable(const String &msg)
:	errMsg(msg),
	Exception(E_CANTCONVERTTABLE)
{
}

CantConvertTable::CantConvertTable()
:	errMsg(NULL_STRING),
	Exception(E_CANTCONVERTTABLE)
{ }

CantConvertTable::~CantConvertTable() { }

void CantConvertTable::fill(Connection *c)
{
	*c >> errMsg;
}

void CantConvertTable::send(Connection *c) const
{
	*c << errMsg;
}

String CantConvertTable::message() const
{
	if (errMsg != NULL_STRING)
		return errMsg;
	return libMsg("TRANSCORRUPT");
}

// end of CantConvertTable class

// InvalidObjectId class

InvalidObjectId::InvalidObjectId(Int n)
:	oId(n),
	Exception(E_INVALIDOBJECTID)
{
}

InvalidObjectId::InvalidObjectId()
:	oId(ERR),
	Exception(E_INVALIDOBJECTID)
{ }

InvalidObjectId::~InvalidObjectId() { }

void InvalidObjectId::fill(Connection *c)
{
	*c >> oId;
}

void InvalidObjectId::send(Connection *c) const
{
	*c << oId;
}

String InvalidObjectId::message() const
{
	return String(libMsg("INVALIDOBJECTID")) + "(" + toString(oId) + ")";
}

// end of InvalidObjectId class

// MsgOutOfBounds class

MsgOutOfBounds::MsgOutOfBounds(Int mc)
:	msgCode(mc),
	Exception(E_MSGOUTOFBOUNDS)
{
}

MsgOutOfBounds::MsgOutOfBounds()
:	msgCode(ERR),
	Exception(E_MSGOUTOFBOUNDS)
{ }

MsgOutOfBounds::~MsgOutOfBounds() { }

void MsgOutOfBounds::fill(Connection *c)
{
	*c >> msgCode;
}

void MsgOutOfBounds::send(Connection *c) const
{
	*c << msgCode;
}

String MsgOutOfBounds::message() const
{
	return String(libMsg("MSGOUTOFBOUNDS")) + "(" + toString(msgCode) + ")";
}

// end of MsgOutOfBounds class

// InvalidClientId class

InvalidClientId::InvalidClientId(Int c)
:	cId(c),
	Exception(E_INVALIDCLIENTID)
{
}

InvalidClientId::InvalidClientId()
:	cId(ERR),
	Exception(E_INVALIDCLIENTID)
{ }

InvalidClientId::~InvalidClientId() { }

void InvalidClientId::fill(Connection *c)
{
	*c >> cId;
}

void InvalidClientId::send(Connection *c) const
{
	*c << cId;
}

String InvalidClientId::message() const
{
	return String(libMsg("INVALIDCLIENTID")) + "(" + toString(cId) + ")";
}

// end of InvalidClientId class

// ZombieClient class

ZombieClient::ZombieClient(Int c)
:	cId(c),
	Exception(E_ZOMBIECLIENT)
{
}

ZombieClient::ZombieClient()
:	cId(ERR),
	Exception(E_ZOMBIECLIENT)
{ }

ZombieClient::~ZombieClient() { }

void ZombieClient::fill(Connection *c)
{
	*c >> cId;
}

void ZombieClient::send(Connection *c) const
{
	*c << cId;
}

String ZombieClient::message() const
{
	return String(libMsg("ZOMBIECLIENT")) + "(" + toString(cId) + ")";
}

// end of ZombieClient class

// ShadowError class

ShadowError::ShadowError()
:	Exception(E_SHADOWERROR)
{ }

ShadowError::~ShadowError() { }

void ShadowError::fill(Connection *c)
{
	*c >> msg;
}

void ShadowError::send(Connection *c) const
{
	*c << msg;
}

String ShadowError::message() const
{
	if (msg != NULL_STRING) return msg;
	return libMsg("SHADOWERROR");
}

// end of ShadowError class

// Notification class

Notification::Notification()
:	Exception(E_NOTIFICATION)
{ }

Notification::~Notification() { }

String Notification::message() const
{
	return libMsg("NOTIFICATION");
}

// end of Notification class


// InvalidMsgOnObject class

InvalidMsgOnObject::InvalidMsgOnObject(Int o, Int m)
:	obj(o),
	msg(m),
	Exception(E_INVALIDMSGONOBJECT)
{
}

InvalidMsgOnObject::InvalidMsgOnObject()
:	obj(ERR),
	msg(ERR),
	Exception(E_INVALIDMSGONOBJECT)
{ }

InvalidMsgOnObject::~InvalidMsgOnObject() { }

void InvalidMsgOnObject::fill(Connection *c)
{
	*c >> obj >> msg;
}

void InvalidMsgOnObject::send(Connection *c) const
{
	*c << obj << msg;
}

String InvalidMsgOnObject::message() const
{
	return String(libMsg("INVALIDMSGONOBJECT")) + "(" + toString(obj) +
			", " + toString(msg) + ")";
}

// end of InvalidMsgOnObject class

// TooManyClients class

TooManyClients::TooManyClients(Int mc)
:	Exception(E_TOOMANYCLIENTS),
	maxClients(mc)
{ }

TooManyClients::~TooManyClients() { }

void TooManyClients::fill(Connection *c)
{
	*c >> maxClients;
}

void TooManyClients::send(Connection *c) const
{
	*c << maxClients;
}

String TooManyClients::message() const
{
	return String(libMsg("TOOMANYCLIENTS"))+" "+toString(maxClients);
}

// end of TooManyClients class

// ServerIsDead class

ServerIsDead::ServerIsDead(const String &sv)
:	Exception(E_SERVERISDEAD),
	serverName(sv)
{ }

ServerIsDead::~ServerIsDead() { }

String ServerIsDead::message() const
{
	return toString(libMsg("SERVERDEAD"), toCharPtr(serverName));
}

// end of ServerIsDead class

// IncompatibleVersions class

IncompatibleVersions::IncompatibleVersions()
:	Exception(E_INCOMPATIBLEVERSIONS),
	cVers(ERR),
	sVers(ERR)
{ }

IncompatibleVersions::IncompatibleVersions(Int cV, Int sV)
:	Exception(E_INCOMPATIBLEVERSIONS),
	cVers(cV),
	sVers(sV)
{ }

IncompatibleVersions::~IncompatibleVersions() { }

void IncompatibleVersions::fill(Connection *c)
{
	*c >> cVers >> sVers;
}

void IncompatibleVersions::send(Connection *c) const
{
	*c << cVers << sVers;
}

String IncompatibleVersions::message() const
{
#ifdef __VC__
	char xxx[1024];
	sprintf(xxx, toCharPtr(libMsg("INCOMPATIBLEVERSIONS")), cVers, sVers);
	return toString(xxx);
#else
 	return doFormat(libMsg("INCOMPATIBLEVERSIONS"), long(cVers), long(sVers));
 #endif
}

// end of IncompatibleVersions class

// InvalidForMirror class

InvalidForMirror::InvalidForMirror(const String &eMsg)
:	Exception(E_INVALIDFORMIRROR), essMsg(eMsg)
{ }

InvalidForMirror::~InvalidForMirror() { }

String InvalidForMirror::message() const
{
	return toString(libMsg("INVALIDFORMIRROR"), toCharPtr(essMsg));
}

void InvalidForMirror::fill(Connection *c)
{
	*c >> essMsg;
}

void InvalidForMirror::send(Connection *c) const
{
	*c << essMsg;
}

// end of InvalidForMirror class

// DuplicatedKey class

DuplicatedKey::DuplicatedKey(const String &iName, const String &kStr)
:	Exception(E_DUPLICATEDKEY), indName(iName), keyStr(kStr)
{ }

DuplicatedKey::~DuplicatedKey() { }

String DuplicatedKey::message() const
{
	return toString(libMsg("DUPLICATEDKEY"), toCharPtr(indName),
			toCharPtr(keyStr));
}

void DuplicatedKey::fill(Connection *c)
{
	*c >> indName >> keyStr;
}

void DuplicatedKey::send(Connection *c) const
{
	*c << indName << keyStr;
}

// end of DuplicatedKey class

// PendLockNotif class

PendLockNotif::PendLockNotif(const String &tName, const String &cName)
:	PendNotification(E_PENDLOCKNOTIF), tableName(tName), clientName(cName)
{ }

PendLockNotif::~PendLockNotif() { }

String PendLockNotif::message() const
{
	return toString(libMsg("PENDLOCKNOTIF"), toCharPtr(tableName),
			toCharPtr(clientName));
}

void PendLockNotif::fill(Connection *c)
{
	*c >> tableName >> clientName;
}

void PendLockNotif::send(Connection *c) const
{
	*c << tableName << clientName;
}

// end of PendLockNotif class

// KeyDoesNotExist class

KeyDoesNotExist::KeyDoesNotExist(const String &iName, const String &kStr)
:	Exception(E_KEYDOESNOTEXIST), indName(iName), keyStr(kStr)
{ }

KeyDoesNotExist::~KeyDoesNotExist() { }

String KeyDoesNotExist::message() const
{
	return toString(libMsg("KEYDOESNOTEXIST"), toCharPtr(indName),
			toCharPtr(keyStr));
}

void KeyDoesNotExist::fill(Connection *c)
{
	*c >> indName >> keyStr;
}

void KeyDoesNotExist::send(Connection *c) const
{
	*c << indName << keyStr;
}

// end of KeyDoesNotExist class

// InvalidTimeStamp class

InvalidTimeStamp::InvalidTimeStamp()
:	Exception(E_INVALIDTIMESTAMP)
{ }

InvalidTimeStamp::InvalidTimeStamp(const String &tName, const Time &cts,
		const Time &mts, const Time &sts)
:	Exception(E_INVALIDTIMESTAMP), tableName(tName), cTS(cts), mTS(mts),
	sTS(sts)
{ }

InvalidTimeStamp::~InvalidTimeStamp() { }

String InvalidTimeStamp::message() const
{
	String cts = toString(cTS);
	String mts = toString(mTS);
	String sts = toString(sTS);
	return toString(libMsg("INVALIDTIMESTAMP"), toCharPtr(tableName),
			toCharPtr(cts), toCharPtr(mts), toCharPtr(sts));
}

void InvalidTimeStamp::fill(Connection *c)
{
	*c >> tableName >> cTS >> mTS >> sTS;
}

void InvalidTimeStamp::send(Connection *c) const
{
	*c << tableName << cTS << mTS << sTS;
}

// end of InvalidTimeStamp class

// OptTransCorrupt class

OptTransCorrupt::OptTransCorrupt()
:	Exception(E_OPTTRANSCORRUPT)
{ }

OptTransCorrupt::OptTransCorrupt(const String &tName, const Time &mts,
		const Time &tts)
:	Exception(E_OPTTRANSCORRUPT), tableName(tName), mTS(mts), tTS(tts)
{ }

OptTransCorrupt::~OptTransCorrupt() { }

String OptTransCorrupt::message() const
{
	String mts = toString(mTS);
	String tts = toString(tTS);
	return toString(libMsg("OPTTRANSCORRUPT"), toCharPtr(tableName),
			toCharPtr(mts), toCharPtr(tts));
}

void OptTransCorrupt::fill(Connection *c)
{
	*c >> tableName >> mTS >> tTS;
}

void OptTransCorrupt::send(Connection *c) const
{
	*c << tableName << mTS << tTS;
}

// end of OptTransCorrupt class

// InvalidLogin class

InvalidLogin::InvalidLogin()
:	Exception(E_INVALIDLOGIN)
{ }

InvalidLogin::InvalidLogin(const String &u, const String &r)
:	Exception(E_INVALIDLOGIN), usr(u), reason(r)
{ }

InvalidLogin::~InvalidLogin() { }

String InvalidLogin::message() const
{
	return toString(libMsg("INVALIDLOGIN"), toCharPtr(usr),
			toCharPtr(reason));
}

void InvalidLogin::fill(Connection *c)
{
	*c >> usr >> reason;
}

void InvalidLogin::send(Connection *c) const
{
	*c << usr << reason;
}

// end of InvalidLogin class

// TooManyAttachs class

TooManyAttachs::TooManyAttachs(Int m)
:	Exception(E_TOOMANYATTACHS),
	max(m)
{ }

TooManyAttachs::TooManyAttachs()
:	Exception(E_TOOMANYATTACHS),
	max(ERR)
{ }

TooManyAttachs::~TooManyAttachs() { }

void TooManyAttachs::fill(Connection *c)
{
	*c >> max;
}

void TooManyAttachs::send(Connection *c) const
{
	*c << max;
}

String TooManyAttachs::message() const
{
#ifdef __VC__
	char xxx[1024];
	sprintf(xxx, toCharPtr(libMsg("TOOMANYATTACHS")), max);
	return toString(xxx);
#else
	return doFormat(libMsg("TOOMANYATTACHS"), max);
#endif
}

// end of TooManyAttachs class

// InvalidData class

InvalidData::InvalidData(const String &d)
:	Exception(E_INVALIDDATA),
	descr(d)
{ }

InvalidData::InvalidData()
:	Exception(E_INVALIDDATA),
	descr(NULL_STRING)
{ }

InvalidData::~InvalidData() { }

void InvalidData::fill(Connection *c)
{
	*c >> descr;
}

void InvalidData::send(Connection *c) const
{
	*c << descr;
}

String InvalidData::message() const
{
	return toString(libMsg("INVALIDDATA"), toCharPtr(descr));
}

// end of InvalidData class

// AdminMode class

AdminMode::AdminMode(const String &u)
:	Exception(E_ADMINMODE),
	user(u)
{ }

AdminMode::AdminMode()
:	Exception(E_ADMINMODE),
	user(NULL_STRING)
{ }

AdminMode::~AdminMode() { }

void AdminMode::fill(Connection *c)
{
	*c >> user;
}

void AdminMode::send(Connection *c) const
{
	*c << user;
}

String AdminMode::message() const
{
	return toString(libMsg("ADMINMODE"), toCharPtr(user));
}

// end of AdminMode class

