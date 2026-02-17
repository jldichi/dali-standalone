/********************************************************************
* Copyright (c) 1995 InterSoft Co.  All Rights Reserved
* THIS IS UNPUBLISHED PROPRIETARY SOURCE CODE OF InterSoft Co.
* The copyright notice above does not evidence any
* actual or intended publication of such source code.
*
* phyfile.cc,v 1.1.1.1 1996/04/16 18:52:41 gustavof Exp
*
*********************************************************************/

#ifdef __GNUC__
#pragma implementation
#endif

#include <local/osfile.h>
#include <local/phyfile.h>

#include <ifound/stdc.h>

// OS dependant includes

#include <errno.h>
#include <fcntl.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <unistd.h>

#if HAVE_ASYNC_IO
#	include <sys/asynch.h>

extern "C" int aiowrite(int fd, char *bufp, int bufs, int offset,
		int whence, aio_result_t *resultp);
extern "C" aio_result_t *aiowait(struct timeval *);

#endif

/* +++ CONFIG +++ */

#ifdef __NT__
#	define CREAT_MODE	(S_IWRITE|S_IREAD)
#elif HAVE_USERS
#	define CREAT_MODE	(S_IRUSR|S_IWUSR|S_IRGRP|S_IWGRP|S_IROTH|S_IWOTH)
#else
#	define CREAT_MODE	(S_IWRITE|S_IREAD)
#endif

#ifdef HAVE_FCNTL
#	define CLOSE_ON_EXEC(fd)	fcntl((fd), F_SETFD, 1)
#else
#	define CLOSE_ON_EXEC(fd)	0
#endif

/* --- CONFIG --- */

Int PhysFile::maxFiles_sd = ERR;
Int PhysFile::openFiles_sd = 0;

PhysFile *PhysFile::first_sd = NULL;

Int PhysFile::nReads_sd = 0;
Int PhysFile::nWrites_sd = 0;

PhysFile::PhysFile()
:	path_d(NULL_STRING),
	fd_d(ERR),
	nlocks_d(0),
	nWr_d(0),
	nRd_d(0),
	flags_d(0),
	osfile_d(NULL),
	next_d(NULL),
	prev_d(NULL)
{
}

PhysFile::~PhysFile()
{
}

// Connect a virtual file on the top of the chain

void PhysFile::connect()
{
	if (first_sd == NULL)
		prev_d = next_d = this;
	else {
		next_d = first_sd;
		prev_d = first_sd->prev_d;
		prev_d->next_d = this;
		next_d->prev_d = this;
	}
	first_sd = this;
}

// Disconnect file from the chain

void PhysFile::disconnect()
{
	if (this == first_sd)
		first_sd = next_d;

	if (this == first_sd) first_sd = NULL;
	else {
		prev_d->next_d = next_d;
		next_d->prev_d = prev_d;
	}
	next_d = prev_d = NULL;
}

void PhysFile::open(OsFile *o, const String &p)
{
	osfile_d = o;
	path_d	= p;
}

void PhysFile::close()
{
	if (fd_d != ERR) {
		_global_close(fd_d);
		fd_d = ERR;
		nlocks_d = 0;
		--openFiles_sd;
		disconnect();
	}
}

void PhysFile::updateFd(const char *sys_call, bool ret)
{
	if (osfile()->name() == NULL_STRING) {
		errno = EBADF;
		while (osError(sys_call))
			;
		return;
	}

	// connect the file at the beginning of the chain

	if (first_sd != this) {
		if (fd_d != ERR) disconnect();
		connect();
	}

	if (fd_d == ERR) {
		if (maxFiles_sd != ERR && openFiles_sd >= maxFiles_sd)
			freeAnFd(sys_call);

		String tmp = fullName();
		forever {
			osfile()->errNo(0);
			fd_d = _global_open(toCharPtr(tmp), flags_d, CREAT_MODE);

			if (fd_d != ERR && CLOSE_ON_EXEC(fd_d) != ERR) {
				flags_d &= ~(O_EXCL|O_TRUNC);
				break;
			}
			if (errno == EMFILE) {
				// free these two files descriptors for other applications
				freeAnFd(sys_call);
				freeAnFd(sys_call);
				freeAnFd(sys_call);
				freeAnFd(sys_call);
				maxFiles_sd = openFiles_sd;
				// free this one for me
				freeAnFd(sys_call);
				freeAnFd(sys_call);
				freeAnFd(sys_call);
				continue;
			}
			osfile()->errNo(errno);
			if (ret || !osError(sys_call)) break;
		}

		// if test flag is on and couldn't open file,
		// disconnect it from the chain

		if (ret && fd_d == ERR)
			disconnect();
		else
			++openFiles_sd;	// we have one more osfile opened
	}
}

// Search for a 'closable' file and close it.

void PhysFile::freeAnFd(const char *sys_call)
{
	register PhysFile *q;

	// First search for a file without locks

	for (q = first_sd->prev_d; q != first_sd; q = q->prev_d) {
		if (q->nlocks_d <= 0) {
			q->close();
			return;
		}
	}

	// Now search for a flushable file

	for (q = first_sd->prev_d; q != first_sd; q = q->prev_d) {
		if (q->flush()) {
			q->close();
			return;
		}
	}

	// Error too many locks
#if HAVE_REAL_LOCKS
	errno = ENOLCK;
#else
	errno = EDEADLOCK;
#endif
	while (osError(sys_call))
		;
}

void PhysFile::syncAllCache()
{
	PhysFile *q = first_sd;

	if (q != NULL) {
		do {
			q->syncCache();
			q = q->next_d;
		} while(q != first_sd);
	}
}

void PhysFile::flushAll()
{
	PhysFile *q = first_sd;

	if (q != NULL) {
		do {
			q->flush();
			q = q->next_d;
		} while(q != first_sd);
	}
}

bool PhysFile::flush()
{
	// this is not a good idea, maybe
	// OsFile::flush could call PhysFile::flush again. (pablog)
	return osfile()->flush();
}

bool PhysFile::syncCache()
{
	// this is not a good idea, maybe
	// OsFile::syncCache could call PhysFile::syncCache again. (pablog)
	return osfile()->syncCache();
}

static inline String bldErrStr(const char *scall, void const *b,
		Int off, Int l)
{
	String syscall(scall);
	return syscall + "(" + toString(Int(b)) + ", " + toString(off) +
			", " + toString(l) + ")";
}

Int PhysFile::write(const void *buff, Int offset, Int length)
{
	Int nwrit;
	Int l = length;

	do {
		updateFd("write");
		forever {
			if (lseek(fd_d, offset, 0) == ERR) break;

			nwrit = _global_write(fd_d, buff, unsigned(l));

			if (nwrit == l) {
				nWr_d++;
				nWrites_sd++;
				return length;
			}
			if (nwrit == ERR) break;

			l -= nwrit;
			offset += nwrit;
		}
	} while (osError(fullName() + ": " +
			bldErrStr("write", buff, offset, length)));
	return ERR;
}

Int PhysFile::awrite(const void *buff, Int offset, Int length)
{
#if HAVE_ASYNC_IO
	Int nwrit;
	Int l = length;
	aio_result_t aior;

	do {
		updateFd("awrite");

		forever {
			if (aiowrite(fd_d, (char *) buff, unsigned(length),
						 offset, 0, &aior) == ERR)
				break;
			nwrit = aior.aio_return;

			if (nwrit == l) {
				nWr_d++;
				nWrites_sd++;
				return length;
			}
			if (nwrit == ERR) break;

			l -= nwrit;
			offset += nwrit;
		}
	} while (osError(fullName() + ": " +
			bldAWriteStr(buff, offset, length)));
	return ERR;
#else
	return write(buff, offset, length);
#endif
}

Int PhysFile::read(void *buff, Int offset, Int length)
{
	updateFd("read");
	do {
    	if (lseek(fd_d, offset, 0) != ERR) {
			Int nread;
			if ((nread = _global_read(fd_d, buff, unsigned(length))) != ERR) {
				nRd_d++;
				nReads_sd++;
				return nread;
			}
		}
	} while (osError(fullName() + ": " +
			bldErrStr("read", buff, offset, length)));
	return ERR;
}

Int PhysFile::sync()
{
#ifdef HAVE_FSYNC
	// No writes on this file, no need to sync it
	if (nWr_d == 0) return OK;
	updateFd("sync");
	do {
		if (fsync(fd_d) != ERR) {

			resetCounters();

			return OK;
		}
	} while (osError("sync"));
#else
	resetCounters();
#endif
	return OK;
}

void PhysFile::waitForWrites()
{
#if HAVE_ASYNC_IO
	while (long(::aiowait(NULL)) != ERR)
		;
	if (errno == EINVAL)
		return;
	error("waitForWrites");
#endif
}

short PhysFile::fd()
{
	updateFd("fd");
	return fd_d;
}

void PhysFile::clearPath()
{
	path_d = NULL_STRING;
}

String PhysFile::fullName(const String &p, const String &n, const String &e)
{
	String fileName = p;
	if (!fileName.isNull())
		fileName += PATH_SLASH;
	fileName += n;
	if (!e.isNull()) fileName += String(".") + e;
	return fileName;
}

String PhysFile::fullName()
{
	return fullName(path_d, osfile()->name(), osfile()->ext());
}
