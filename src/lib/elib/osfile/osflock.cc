/********************************************************************
* Copyright (c) 1991 InterSoft Co.  All Rights Reserved
* THIS IS UNPUBLISHED PROPRIETARY SOURCE CODE OF InterSoft Co.
* The copyright notice above does not evidence any
* actual or intended publication of such source code.
*
* $Id: osflock.cc,v 1.3 1997/11/26 18:06:16 guiller Exp $
*
* DESCRIPTION
* Implementation of OsFiles
*********************************************************************/

#include <ifound.h>
#include <ifound/priv/gfuncs.h>
#include <local/osfile.h>
#include <local/phyfile.h>
#include <ifound/stdc.h>

#include <errno.h>
#include <signal.h>
#ifdef HAVE_UNISTD_H
#	include <unistd.h>
#endif

/* +++ CONFIG +++ */

// Time to wait before displaying a message
#define LOCK_WAIT_SECS		3

/* --- CONFIG --- */

Int OsFile::read(void *buff, Long offset, Int length, Int mode)
{
	return physFile(offset)->read(buff, Int(offset), length, mode);
}

Int OsFile::lock(Int mode)
{
	checkMethod("lock");
	return pfiles()->lock(mode);
}

Int PhysFile::lock(Int mode)
{
	if (lock(0, 0, mode))
		return IO_LOCKED;
	nlocks_d = 1;
	return true;
}

static int had_alarm = false;

static RETSIGTYPE set_alarm(int)
{
	had_alarm = true;
}

Int OsFile::lockOwner(Long offset, Int length)
{
	return physFile(offset)->lockOwner(Int(offset), length);
}

Int OsFile::lock(Long offset, Int length, Int mode)
{
	return physFile(offset)->lock(Int(offset), length, mode);
}

void OsFile::free(Long offset, Int length)
{
	physFile(offset)->free(Int(offset), length);
}

void PhysFile::free(Int offset, Int length)
{
	updateFd("free");
	do {
#ifndef __NT__
    	if (lseek(fd_d, offset, 0) != ERR &&
    			::freeFile(fd_d, ERR, length) != ERR) {
#else
		if (lseek(fd_d, offset, 0) != ERR) { // to do (to add physical locking in NT)
#endif
			if (length == 0 || nlocks_d-- <= 0)
				nlocks_d = 0;
			break;
		}
	} while (osError("free"));
}

Int PhysFile::read(void *buff, Int offset, Int length, Int mode)
{
	if (lock(offset, length, mode)) return IO_LOCKED;
	if (mode & OsFile::LOCK) nlocks_d++;

	Int nread = read(buff, offset, length);
	if (nread == ERR && (mode&OsFile::LOCK) )
		free(offset, length);
	return nread;
}

Int PhysFile::lock(Int offset, Int length, Int mode)
{
	int ret;
	SigHandlerT old_handler = NULL;
	bool deadlock = false, msg = false;

	mode &= (OsFile::TEST|OsFile::LOCK);		// Preserve only lock flags
	if (!mode) return false;

#ifdef HAVE_FULL_SIGNALS
	updateFd("lock");

	do {
		if (lseek(fd_d, offset, 0) != ERR) {

			forever {
				if (!had_alarm && mode == OsFile::LOCK) {
					old_handler = SigSet(SIGALRM, set_alarm);
					alarm(LOCK_WAIT_SECS);
				}
				ret = ::lockFile(fd_d, mode&OsFile::LOCK, mode&OsFile::TEST, 
								 ERR, length);
				if (ret == ERR && errno == EDEADLK && !deadlock) {
					flushAll();
					deadlock = true;
					continue;
				}
				if (mode == OsFile::LOCK) {
					alarm(0);
					SigSet(SIGALRM, old_handler);
				}
				if (had_alarm && !msg) {
					tryLockMsg(true);
					msg = true;
				}
				else {
					if (msg) tryLockMsg(false);
					had_alarm = false;
					break;
				}
			}
		
			if (ret != ERR)
				break;

			if (errno == EACCES || errno == EAGAIN)
				return true;
		}
	} while (osError("lock"));

	return false;
#else
	return true;
#endif
}

Int PhysFile::lockOwner(Int offset, Int length)
{
	updateFd("lockOwner");
#ifndef __NT__
	return ::fdLockOwner(fd_d, offset, length);
#else
	return getpid();
#endif
}

