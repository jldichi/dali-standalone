// --------------------------------------------------------------------
// Copyright (c) 1995 InterSoft Co.  All Rights Reserved
// THIS IS UNPUBLISHED PROPRIETARY SOURCE CODE OF InterSoft Co.
// The copyright notice above does not evidence any actual or intended 
// publication of such source code.
//
// $Id: flock.h,v 1.1.1.1 1996/04/16 18:52:55 gustavof Exp $
//
// Description:
// --------------------------------------------------------------------

#ifndef FLOCK_H
#define FLOCK_H

#include <ifound.h>
#include <ifound/itypes.h>

class FileLockEntry
{

public:

	inline FileLockEntry(const String &fileName,
			Int device,
			Int inode,
			const String &owner,
			const String &node,
			Int pid,
			Int tmOut,
			Time lRefresh);

	inline const String &file() const;
	inline const String &owner() const;
	inline Time lastRefresh() const;
	inline void lastRefresh(Time t);
	inline void pid(Int p);
	inline void node(const String &nm);
	inline const String &node() const;
	inline Int pid() const;
	inline Int device() const;
	inline Int inode() const;
	inline Int timeOut() const;
	inline void timeOut(Int t);
	bool isLocked(Time t) const;

private:

	String file_d;
	Int device_d;
	Int inode_d;
	String owner_d;
	String node_d;
	Int pid_d;
	Int tmOut_d;
	Time lastRefresh_d;
};

class gtype(PtrArray, FileLockEntry);

class FileLocker
{

public:

	FileLocker();
	~FileLocker();

	// methods to read & write the control file
	bool readCtrlFile();
	bool writeCtrlFile();

	// methods to lock & free entries in the control file
	bool lockFile(const String &fName, String &owner,
			String &node,
			Int &pid,
			Int kAlive = 60,
			bool lk = true);
	bool keepAlive(const String &fName, const String &node, Int pid);
	bool freeFile(const String &fName, const String &node, Int pid);

	// Kill all entries for specified owner
	void killAll(const String &owner);

	FileLockEntry *find(Int device, Int inode);
	void clear();

private:

	FILE *file_d;
	gtype(PtrArray, FileLockEntry) &entries_d;
	Time now_d;

};

#include "flock.icc"

#endif // FLOCK_H
