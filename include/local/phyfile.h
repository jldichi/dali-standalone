/********************************************************************
* Copyright (c) 1995 InterSoft Co.  All Rights Reserved
* THIS IS UNPUBLISHED PROPRIETARY SOURCE CODE OF InterSoft Co.
* The copyright notice above does not evidence any
* actual or intended publication of such source code.
*
* $Id: phyfile.h,v 1.1.1.1 1996/04/16 18:51:50 gustavof Exp $
*
* DESCRIPTION
*
*********************************************************************/

#ifndef	PHYSFILE_H
#define	PHYSFILE_H

#ifdef __GNUC__
#	pragma interface
#endif

#include <ifound.h>
#include <ifound/itypes.h>

class PhysFile {

//private:

	String	path_d;				// path to the file
	short	fd_d;				// file descriptor
	UnShort	nlocks_d; 			// numbers of locks on the file
	Int nWr_d;					// Number of writes since last sync
	Int nRd_d;					// Number of reads since last sync
	UnShort	flags_d;			// open and create flags
	
	OsFile *osfile_d;			// OsFile which I belong to
	
	PhysFile *next_d;
	PhysFile *prev_d;
	static PhysFile *first_sd;
	
	static Int nReads_sd;
	static Int nWrites_sd;

	static Int maxFiles_sd;		// max number of physfiles we can use
	static Int openFiles_sd;	// number of currently open files

public:

	PhysFile();
	~PhysFile();
	
	void open(OsFile *o, const String &p);
	void close();

	void updateFd(const char *sys_call, bool ret = false);
	void freeAnFd(const char *sys_call);

	void tryLockMsg(bool disp);
	void unLink();

	void connect();
	void disconnect();

	Int	 osError(const String &sys_call);
		
	bool flush();
	bool syncCache();

	Int size();
	Time mtime();
	Int owner();

	static String fullName(const String &p, const String &n, const String &e);
	String fullName();

	static void syncAllCache();
	static void flushAll();

	static void waitForWrites();

	Int awrite(const void *buff, Int offset, Int length);
	Int write(const void *buff, Int offset, Int length);
	Int read(void *buff, Int offset, Int length, Int mode);
	Int read(void *buff, Int offset, Int length);

	Int sync();

	Int lock(Int offset, Int length, Int mode);
	Int lock(Int mode);
	Int lockOwner(Int offset, Int length);
	
	void free(Int offset, Int length);

	void rename(const String &n, const String &e);
	void rename(const String &p, const String &n, const String &e);

	short fd();

	void clearPath();
	
	void resetCounters()	{ nWr_d = nRd_d = 0;							}
	OsFile *osfile()		{ assert(osfile_d != NULL); return osfile_d;	}
	const String &path()	{ return path_d;								}
	bool isOpen()			{ return (next_d != NULL && prev_d != NULL);	}
	void flags(Int f)		{ flags_d = f;									}
	
	static Int nReads()		{ return nReads_sd;								}
	static Int nWrites()	{ return nWrites_sd;							}
};

#endif
