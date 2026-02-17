/********************************************************************
* Copyright (c) 1992 InterSoft Co.  All Rights Reserved
* THIS IS UNPUBLISHED PROPRIETARY SOURCE CODE OF InterSoft Co.
* The copyright notice above does not evidence any
* actual or intended publication of such source code.
*
* misc.cc,v 1.1.1.1 1996/04/16 18:51:57 gustavof Exp
*
* DESCRIPTION
*********************************************************************/

#include <ifound.h>
#include <ifound/dirdefs.h>
#include <ifound/stdc.h>
#include <signal.h>
#include <cfix.h>

#if !defined(HAVE_FULL_SIGNALS)

// FPI __SigIntHandler = Stop;
FPI __SigIntHandler = stop;

static bool BREAK;

extern "C" void CallIntHandler(void)
{
	BREAK = true;
	return;
}

extern "C" void AssertInt(void)
{
	static bool active = false;

	if (active || !BREAK) return;

	active = true;
	if (__SigIntHandler && __SigIntHandler != (FPI) SIG_IGN)
		(*__SigIntHandler)(0);
	BREAK = false;
	active = false;
}

#endif

#if HAVE_BAD_CVT
extern "C" FCVTRETTYPE eCvt(double value, int ndigits, int *decpt, int *sign)
{
	FCVTRETTYPE ret;
	if (value == 0.0) {
		ret = ecvt(0.1, ndigits, decpt, sign);
		if (ndigits >= 0) {
			*ret = '0';
		}
	} else
		ret = ecvt(value, ndigits, decpt, sign);

	ret[ndigits] = '\0';
	return ret;
}

extern "C" FCVTRETTYPE fCvt(double value, int ndigits, int *decpt, int *sign)
{
	FCVTRETTYPE ret;
	if (value == 0.0) {
		ret = fcvt(0.1, ndigits, decpt, sign);
		if (ndigits >= 0) {
			*ret = '0';
		}
	} else
		ret = fcvt(value, ndigits, decpt, sign);

	if (ndigits + *decpt >= 0)
		ret[ndigits + *decpt] = '\0';
	return ret;
}

#endif /* HAVE_BAD_CVT */

extern "C" SigHandlerT Signal(int sig, SigHandlerT fp)
{
#if HAVE_FULL_SIGNALS
# if HAVE_SIGSET
    return sigset(sig, fp);
#	else
#		if !defined(HAVE_RESTARTABLE_SYSCALLS) || !defined(HAVE_SIGVEC)
 	        return signal(sig, fp);
#		else
    struct sigvec vec;
    static SigHandlerT ret;
    if (sigvec(sig, NULL, &vec) == ERR)
    	return NULL;
    ret = vec.sv_handler;
    vec.sv_handler = fp;
    vec.sv_flags |= SV_INTERRUPT;
    if (sigvec(sig, &vec, NULL) == ERR)
    	return NULL;
    return ret;
#		endif
#	endif
#else	/* HAVE_FULL_SIGNALS */
//	static FPI DefHandler = Stop;
	static FPI DefHandler = stop;
	FPI OldHandler = __SigIntHandler;
	AssertInt();
	if (sig != SIGINT)
		return NULL;
	if (fp == SIG_DFL) {
		__SigIntHandler = FPI(DefHandler);
		AssertInt();
	} else {
		AssertInt();
		__SigIntHandler = fp;
		if (fp != SIG_IGN)
			DefHandler = fp;
	}
	return OldHandler;
#endif
}

#if HAVE_CLOSEDIR_BUG
extern "C" int closeDir(DIR *d)
{
	int fd;

	if (d == NULL)
		return 0;

	fd = d->dd_fd;
	if (d->dd_buf != NULL)
		free(d->dd_buf);

	free(d);
	return close(fd);
}
#endif

bool isBigEndian()
{
	Int i = 1;
	return bool(((char *) &i)[sizeof(Int)-1] != 1);
}

#if !defined(HAVE_MEMMOVE)

extern "C" void *memMove(void *dest, const void *src, SizeT len)
{
	register char *d = (char *) dest;
	register const char *s = (const char *) src;
	register int offs = s - d;

	if (offs == 0 || len == 0)
		return dest;

	assert(len > 0);

	if (offs > 0) {
		if (offs < len) {
			/* OK, copy n chunks of offs bytes each */
			do {
				mCopy(d, s, offs);
				d += offs;
				s += offs;
				len -= offs;
			} while (len >= offs);
			if (len > 0)
				mCopy(d, s, len);
			return dest;
		}
	} else {
		offs = -offs;
		if (offs < len) {
			d += len;
			s += len;
			do {
				d -= offs;
				s -= offs;
				mCopy(d, s, offs);
				len -= offs;
			} while (len >= offs);
			if (len > 0)
				mCopy(d - len, s - len, len);
			return dest;
		}
	}

	/* We're sure it's a non-overlapping copy, use standard copy */
	mCopy(dest, src, len);
	return dest;
}

#endif /* !defined(HAVE_BCOPY) && !defined(HAVE_MEMMOVE) */

#ifdef HAVE_MEMCPYBUG

extern "C" void *memCopy(void *dest, const void *src, SizeT len)
{
	register char *d = (char *) dest;
	register const char *s = (const char *) src;
	register SizeT i=0;	

	for(i=0; i < len; i++)
		((char *) d)[i] = ((const char *) s)[i];

	return dest;
}

#endif
