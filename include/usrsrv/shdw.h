/********************************************************************
* Copyright (c) 1992 InterSoft Co.  All Rights Reserved
* THIS IS UNPUBLISHED PROPRIETARY SOURCE CODE OF InterSoft Co.
* The copyright notice above does not evidence any
* actual or intended publication of such source code.
*
* $Id: shdw.h,v 1.2 1997/09/12 19:16:36 juang Exp $
*
* DESCRIPTION
* Class definitions for Shadow type.
*********************************************************************/

#ifndef SHDW_H
#define SHDW_H

#ifdef HAVE_PRAGMA
#pragma interface
#endif

#include <usrsrv/shadobj.h>                
#include <essentia/dbconst.h>
#include <local/slog.h>
#include <ifound/str.h>

#ifdef DEBUG
#	define shIdCheck()		assert(typeIdentif == TYPE_IDENTIF)
#else
#	define shIdCheck()
#endif

#define IQLSRV_NAME		"iqlsrv"

#define SHTRACE														\
	do {															\
		stdMsg << __FILE__ << " (" << Int(__LINE__) << ")" << endl;	\
	} while (0)

// Este tipo debe ser conocido por shadow (shade.C), Shadow y ShadowConn ...
enum ExecMode {
	AUTOMATIC,
	MANLOCALCLI,
	MANREMOTECLI
};

class ShTube;
class gtype(PtrArray, ShadowObject);

class Shadow : public ShadowObject 
{

protected:
	static String serverName; 	// serverName (only ONE server is possible).
private:
	static Int shTubeId;	// index of tube as a servobj.
	String nname;			// client node name.
	String uname;			// client user name.
	String command;			// client remote command.
	String params;			// client parameters.
	String tname;			// client terminal name.
	bool secServer;			// client is a secondary server (dbibup, etc.).
	static bool someSignal;	// to control SIGUSR2 signal sending.
	static SysLog *sysLog;	// Msg Log
	static Int  debug_;		// Debug mode 
	static bool verbose_;	// Verbose mode
	static Int  usersrv_;	// Generic number (not for plain vanilla shadows)
	static String envfile_; // Environment file
    ExecMode mode_d;        // To conserve the mode that the server is running

	// Clean up shadow objects. (Tube and NameSpaces)
	static void clean();

	static void sigUsr1HndlDo(Int);
	static void sigUsr1HndlSet(Int);
	static void sendSignal();

	void snd(Int len, short f);
	void prep();

	ShTube *tube();
	ShTube *getShTube(Int Id);

	static Int getValidMsgCode();
	static Int getValidObjectId();
	
public:

	// Constructors & destructors
	Shadow(int argc, char **argv);
	~Shadow();

	// Virtual routines
	virtual void execute();
	virtual String service() const;

	// Shadow operations
	virtual void connect(Client *);			// connect
	virtual void attach(Client *);			// attach to schema
	virtual void detach(Client *);			// detach from schema
	virtual void disConnect(Client *);		// disconnect from shadow
	virtual void shadowDbg(Client *);		// debugging
	virtual void defa(Client *);			// default action (send to the server)

	inline Int objectType() const;
	inline String nodeName() const;
	inline String userName() const;
	
	// Error, Warning and Notice routines
	static void error(const String &);
	static void createSysLog();
	static int logError(int , const char *, const char *msg);
	static int logNotice(int , const char *, const char *msg);
	static int logWarning(int , const char *, const char *msg);

	inline static Int  debug();
	inline static bool verbose();
	inline static Int  usersrv();
	inline static const String &svname();
	inline static Int  tubeid();
	String objType();

};

#include <usrsrv/shdw.icc>

#endif
