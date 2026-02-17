/********************************************************************
* Copyright (c) 1992 InterSoft Co.  All Rights Reserved
* THIS IS UNPUBLISHED PROPRIETARY SOURCE CODE OF InterSoft Co.
* The copyright notice above does not evidence any
* actual or intended publication of such source code.
*
* $Id: dbconn.h,v 1.4 2002/03/15 20:40:04 hardaiz Exp $
*
* DESCRIPTION
* Class definitions for Internal data base Representations
*********************************************************************/

#ifndef DBCONN_H
#define DBCONN_H

#include <ifound.h>

#ifdef __GNUC__
#pragma interface
#endif

#include <essentia/dbconst.h>
#include <ifound/str.h>
#include <essentia/dbmcom.h>
#include <essentia/dbclobj.h>
#include <essentia/connect.h>
#include <ifound/anyvalue.h>
#include <essentia/criobj.h>
#include <ifound/tstamp.h>
#include <ifound/action2.h>

declare2(BaseAction, StringRef, StringRef);

#define ACCESS_SCHEMA_SEPAR	"\t"

// This two are defined at cnffile.h too ...
#define ICON		'i'
#define RCON		'r'

class IpcConnection;

typedef void (*NotifHandlerT)(const String &theMsg, ...);

class DbConnection : public ClientObject, public Connection {

public: // Make ConnMode known to DbConnection itself.

	/*
		noAbort	   : Report error (ie: status_ = ERR).
		abortOnInit: Abort if error occurs at connection
		             initialization time (creation,  connection, find.*).
		abortOnRun : Abort if error occurs when a successfully created
		             connection fails (filling, flushing, ...).

		Equivalences with preceding version abort_ value:
                         bool abort_         | ConnMode abort_
                         --------------------+--------------------
                         true  (at init time)| abortOnInit
                         true  (at run time) | abortOnRun
                         false (at init time)| !abortOnInit
                         false (at run time) | !abortOnRun

		Apologize : It seems that, once again, this enum should have been
		            a group of macros (because of pipeing) ...
	*/

	enum ConnMode {
		noAbort		= 0,
		abortOnInit	= 1,
		abortOnRun	= 2,
		abortAlways	= abortOnInit|abortOnRun,
		noBinary	= 4,
	};

private:

	static bool userBeganTrans;

	// for the connection list
	static DbConnection *head_;
	DbConnection *next_;

	char type;			// connection type
	Int serverId;		// server id
	String serverName_;	// server's ONLY name
	TimeStamp transTS;	// time stamp assigned by this server to transaction
	bool inTrans;		// is in transaction
	bool readyToEnd_;	// is ready to end

	void setValues();
	virtual bool stopSv();

	DbConnection(const DbConnection &);

protected:

	MsgBuff rbuffer; 	// Real message buffer
	Int nMsgs;			// Messages queued
	Int retValue_;		// function's return value.
	Int actBuffSize;	// actual buffer's contents size
	bool isValid_;		// true if it's a valid connection
	bool hasLocks;		// true if client may have locks on this server
	Int status_;		// Connection creation status
	ConnMode abort_;	// If I want to abort if I have problems

	virtual void _flush();
	virtual Int _fill();
	void flushBuff();
	void _iflush();

	void delFromList();

	virtual bool connExists(char, const String &, Int, const String &);

public:

	DbConnection(char t, const String &hst, Int id,
			const String &servName = NULL_STRING,
#ifndef __VC__
			ConnMode abort = abortAlways);
#else
			ConnMode abort = ConnMode(abortOnInit|abortOnRun));
#endif
	virtual ~DbConnection();

	// server method initialization routine
	virtual void begin(Int io, SvMessage so);

	// server method clean up routine
	inline void end();

	// method parameter push routines
	virtual Connection &operator<<(Int i);
	virtual Connection &operator<<(const String &s);
	virtual Connection &operator<<(const char *cp);
	virtual void push(void *buff, Int i);

	// method parameter pop routines
	virtual Connection &operator>>(Int &i);
	virtual Connection &operator>>(String &s);
	virtual Connection &operator>>(char *&s);
	virtual void pop(void *, Int);

	virtual bool isInvert();
	virtual void connect(bool = false);
	virtual bool waitForServerHalt();

	// server method execution routine
	Int execute();
	operator Int();

	inline char getType();
	inline Int status();

	static void invalidate();
	static bool optimisticTransaction();
	static void beginTransaction();
	static Int endTransaction(bool sync);
	static Int abortTransaction();
	static bool didTransaction();
	static inline bool userBegTrans();
	static inline void userBegTrans(bool b);
	static bool transOk();
	bool begTrans();
	void endTrans(bool sync = false);
	inline bool inTransaction() const;
	inline void inTransaction(bool v);
	inline bool readyToEnd() const;
	inline void readyToEnd(bool v);
	inline const String &serverName();
	inline bool isValid();
	inline void didLock();
	inline TimeStamp &transTimeStamp();
	inline Int nMessages() const;

	static inline DbConnection *head();
	inline DbConnection *next();

	// static methods
	static IpcConnection *findIpc(Int id, String svName,
			bool doConnect = true, bool secConnection = false,
			ConnMode abort = abortAlways);
	static DbConnection *find(char t, const String &nm, Int id,
			String svName, bool doConnect = true,
			bool secConnection = false,
			ConnMode abort = abortAlways,
			String schname = NULL_STRING,
			BaseAction(StringRef, StringRef) *query = NULL);
	static DbConnection *findSchema(const String &schName,
			bool doConnect = true,
			ConnMode abort = abortAlways);
	static DbConnection *findServer(const String &svName,
			bool doConnect = true, ConnMode abort = abortAlways);
	static DbConnection *createSecConn(const String &svName);
	static void dispose(DbConnection *con);
    static bool addSchema(const String &schname);
    static bool delSchema(const String &schname);
	static bool stopServer(const String &sname);
	static String accessiblesServers();
	static String accessiblesSchemas(const String &svName = NULL_STRING);
	static bool multiServerLock(DbConnection *c);
	static bool inTransac();
	static NotifHandlerT setNotifHandler(NotifHandlerT nHandler);
};

#include <essentia/dbconn.icc>

//void setProcessValues(Int clientVers, const String &nname,
//		const String &command, const String &params, const String &tname);
void getProcessValues(Int &clientVers, String &nname, String &command,
		String &params, String &tname);

#endif
