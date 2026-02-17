/********************************************************************
* Copyright (c) 1991 InterSoft Co.  All Rights Reserved
* THIS IS UNPUBLISHED PROPRIETARY SOURCE CODE OF InterSoft Co.
* The copyright notice above does not evidence any
* actual or intended publication of such source code.
*
* $Id: filelock.cc,v 1.1.1.1 1996/04/16 18:51:58 gustavof Exp $
*
* DESCRIPTION
* lock and free operating system files
*********************************************************************/

#include <ifound.h>
#include <ifound/stdc.h>

#if HAVE_REAL_LOCKS

#if HAVE_UNISTD_H
#	include <unistd.h>
#endif

#if HAVE_FCNTL
#	include <fcntl.h>
#	include <errno.h>
#endif

int lockFile(int fd, bool lock, bool test, long offset, long len)
{
#if HAVE_LOCKF
	int lmode;

	if (!test && !lock)
		return 0;

	lmode = (test ? (lock ? F_TLOCK : F_TEST) : F_LOCK);

	if (offset != ERR)
		lseek(fd, offset, 0);

	return lockf(fd, lmode, len);
#else
#	if HAVE_FCNTL
		struct flock lk;

		if (!test && !lock)
			return 0;

		if (offset == ERR) {
			lk.l_start = 0;
			lk.l_whence = 1;
		} else {
			lk.l_start = offset;
			lk.l_whence = 0;
		}
		lk.l_len = len;

		if (test && !lock) {
			lk.l_type = F_WRLCK;
			errno = ERR;
			if (fcntl(fd, F_GETLK, &lk) == ERR || lk.l_type != F_UNLCK) {
				if (errno == ERR)
					errno = EACCES;
				return ERR;
			}
			return OK;
		}

		lk.l_type = F_WRLCK;
		errno = ERR;
		if (fcntl(fd, test ? F_SETLK : F_SETLKW, &lk) == ERR) {
			if (errno == ERR)
				errno = EACCES;
			return ERR;
		}
		return OK;
#	endif
#endif
}

int freeFile(int fd, long offset, long len)
{
#if HAVE_LOCKF
	if (offset != ERR)
		lseek(fd, offset, 0);
	return (lockf(fd, F_ULOCK, len) == 0);
#else
#	if HAVE_FCNTL
		struct flock lk;

		if (offset == ERR) {
			lk.l_start = 0;
			lk.l_whence = 1;
		} else {
			lk.l_start = offset;
			lk.l_whence = 0;
		}
		lk.l_len = len;
		lk.l_type = F_UNLCK;
		return fcntl(fd, F_SETLK, &lk);
#	endif
#endif
}

Int fdLockOwner(int fd, long offset, long len)
{
#if HAVE_FCNTL
	struct flock lk;

	if (offset == ERR) {
		lk.l_start = 0;
		lk.l_whence = 1;
	} else {
		lk.l_start = offset;
		lk.l_whence = 0;
	}
	lk.l_len = len;
	lk.l_type = F_WRLCK;
	if (fcntl(fd, F_GETLK, &lk) == ERR)
		return ERR;

	return Int(lk.l_pid);
#else
	return ERR;
#endif
}

#endif	// HAVE_REAL_LOCK
