// --------------------------------------------------------------------------
// Copyright (c) 1995 InterSoft Co.  All Rights Reserved
// THIS IS UNPUBLISHED PROPRIETARY SOURCE CODE OF InterSoft Co.
// The copyright notice above does not evidence any actual or intended 
// publication of such source code.
//
// $Id: flock.cc,v 1.4 2004/09/29 18:33:18 damianm Exp $
//
// Description:
// FileLocker class implementation.  Used to lock files via a central
// control file.
// --------------------------------------------------------------------------

#include <unistd.h>
#include <sys/types.h>

#include <ifound.h>
#include <ifound/stdc.h>
#include <ifound/priv/gfuncs.h>
#include <flock.h>

#ifndef __NT__
const char *fileName = "/tmp/dali.lks";
#else
const char *fileName = "c:\\tmp\\dali.lks";
#endif
const Int lockTimeout = 600;

declare(PtrArray, FileLockEntry);
implement(PtrArray, FileLockEntry);
declare(PtrArrayCursor, FileLockEntry);
implement(PtrArrayCursor, FileLockEntry);

#include <sys/types.h>
#include <sys/stat.h>

#ifdef HAVE_UNDERSCORE_NAMES
#	define stat _stat
#endif

static void
getDeviceAndInode(const String &fName, Int &device, Int &inode)
{
	device = ERR;
	inode = ERR;
	struct stat st;
	if (statFile(fName, &st) == OK) {
		device = Int(st.st_dev);
		inode = Int(st.st_ino);
	}
}

bool
FileLockEntry::isLocked(Time t) const
{
	return t.seconds() - lastRefresh_d.seconds() <= tmOut_d;
}

FileLocker::FileLocker()
:	file_d(NULL),
	entries_d(*tr_new PtrArray(FileLockEntry)(0, 8))
{
}

FileLocker::~FileLocker()
{
	clear();
	tr_delete(&entries_d);
}

void
FileLocker::clear()
{
	for (PtrArrayCursor(FileLockEntry) c(entries_d); c; ++c) {
		if (*c != NULL)
			tr_delete(*c);
	}
	entries_d.reAlloc(0);
}

bool
FileLocker::readCtrlFile()
{
	clear();

	now_d = Time(NOW);

	// create (if not already created) file with read/write permissions
	// for everyone
#ifndef __NT__
	int prevUmask = umask(0);
#endif
	file_d = fopen(fileName, "r+");
	if (file_d == NULL) {
		file_d = fopen(fileName, "w+");
		if (file_d == NULL) {
#ifndef __NT__
			umask(prevUmask);
#endif
			return false;
		}
	}
#ifndef __NT__
	umask(prevUmask);

	::lockFile(fileno(file_d), true, false, 0, 0);
#endif
	char charLine[2048];

	while (fgets(charLine, sizeof(charLine), file_d) != NULL) {

		charLine[strlen(charLine) - 1] = '\0';

		// end of file marker?
		if (charLine[0] == '\0')
			break;

		String line(charLine);
		String file = line.field(0, "\t");
		String device = line.field(1, "\t");
		String inode = line.field(2, "\t");
		String owner = line.field(3, "\t");
		String node = line.field(4, "\t");
		String pid = line.field(5, "\t");
		String tmout = line.field(6, "\t");
		String dateStr = line.field(7, "\t");
		String timeStr = line.field(8, "\t");
		Time t = toTime(toDate(dateStr)) + toTime(timeStr);
		entries_d.add(tr_new FileLockEntry(file, toInt(device), toInt(inode),
				owner, node, toInt(pid), toInt(tmout), t));
	}
	return true;
}

bool
FileLocker::writeCtrlFile()
{
	assert(file_d != NULL);

	fseek(file_d, 0, 0);
	for (PtrArrayCursor(FileLockEntry) c(entries_d); c; ++c) {
		if (*c != NULL) {
			Time time = c->lastRefresh();
			Date date = toDate(time);
			if (c->isLocked(now_d)) {
				String line = c->file();
				line += "\t";
				line += toString(c->device());
				line += "\t";
				line += toString(c->inode());
				line += "\t";
				line += c->owner();
				line += "\t";
				line += c->node();
				line += "\t";
				line += toString(c->pid());
				line += "\t";
				line += toString(c->timeOut());
				line += "\t";
				line += toString(date);
				line += "\t";
				line += toString(time);
				line += NEW_LINE;
				fputs(toCharPtr(line), file_d);
			}
		}
	}

	// mark end of file
#ifdef HAVE_TRUNCATE
	ftruncate(fileno(file_d), ftell(file_d));
#else
	fputs(NEW_LINE, file_d);
#endif
#ifndef __NT__ // to do (add physical locking in NT)
	::freeFile(fileno(file_d), 0, 0);
#endif
	fclose(file_d);

	file_d = NULL;

	clear();

	return true;
}

void
FileLocker::killAll(const String &owner)
{
	for (PtrArrayCursor(FileLockEntry) c(entries_d); c; ++c) {
		if (*c != NULL && c->owner() == owner) {
		    tr_delete(*c);
		    *c = NULL;
		}
	}
}

bool
FileLocker::keepAlive(const String &fileName,
		const String &node, Int pid)
{
	String lNode = node;
	String lUser;
	Int lPid = pid;
	return lockFile(fileName, lUser, lNode, lPid, ERR, true);
}

bool
FileLocker::lockFile(const String &fileName, String &owner,
		String &node, Int &pid, Int kAlive, bool lock)
{
	assert(file_d != NULL);
	Int inode;
	Int device;
	getDeviceAndInode(fileName, device, inode);
	if (device == ERR || inode == ERR)
		return false;

	FileLockEntry *entry = find(device, inode);

	// No entry... it is not locked
	if (entry == NULL) {
		if (lock) {
			entries_d.add(tr_new FileLockEntry(fileName, device, inode,
					owner, node, pid, Int(kAlive * 1.5), now_d));
		}
		return true;
	}

	// is the lock still alive?
	if (entry->isLocked(now_d)) {
		// if it is not ourselves, return error
		if (entry->pid() != pid || entry->node() != node) {
			pid = entry->pid();
			node = entry->node();
			owner = entry->owner();
			return false;
		}
	}
	else {
		// if it wasn't locked, and we are keeping alive... there is
		// a potential problem!
		if (kAlive == ERR) {
#ifndef __NT__
			FILE *f = fopen("/tmp/flocker.problems", "a+");
#else
			FILE *f = fopen("c:\\tmp\\flocker.problems", "a+");
#endif
			fprintf(f, "Trying to keep alive a lock that was not owned!\n");
			fprintf(f, "REAL OWNER IS: '%S', file = '%S', lastRefresh = '%s'\n",
					&entry->owner(), &entry->file(),
					toCharPtr(entry->lastRefresh()));
			fprintf(f, "KEEPALIVE ATTEMPT: lastRefresh = '%s'\n",
					toCharPtr(now_d));
			fclose(f);
			return false;
		}
		entry->node(node);
		entry->pid(pid);
		entry->timeOut(Int(kAlive * 1.5));
	}
	if (lock) {
		entry->lastRefresh(now_d);
	}

	return true;
}

bool
FileLocker::freeFile(const String &fileName, const String &node, Int pid)
{
	assert(file_d != NULL);
	Int inode;
	Int device;
	getDeviceAndInode(fileName, device, inode);
	if (device == ERR || inode == ERR)
		return false;

	FileLockEntry *entry = find(device, inode);

	// No entry... it is not locked
	if (entry == NULL)
		return true;

	// is the lock still alive?
	if (entry->isLocked(now_d)) {
		// not owner, can't free!
		if (entry->pid() != pid || entry->node() != node)
			return false;
	}

	entries_d.del(entry);

	return true;
}

FileLockEntry *
FileLocker::find(Int device, Int inode)
{
	for (PtrArrayCursor(FileLockEntry) c(entries_d); c; ++c) {
		if (*c && c->device() == device && c->inode() == inode)
			return *c;
	}
	return NULL;
}
