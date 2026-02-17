/********************************************************************
* Copyright (c) 1987 InterSoft Co.  All Rights Reserved
* THIS IS UNPUBLISHED PROPRIETARY SOURCE CODE OF InterSoft Co.
* The copyright notice above does not evidence any
* actual or intended publication of such source code.
*
* $Id: wmstty.cc,v 1.4 2010/09/28 19:57:49 hardaiz Exp $
*
* DESCRIPTION:
* Descripcion del proposito general del modulo
*
* ROUTINE       |  PURPOSE
*---------------+---------------------------------------------------
* _WiSettty		 Set input mode.
* _WiResettty	 Reset input mode.
* _KeyReady		 Checks if keyboard contains characters
*********************************************************************/

#include <ideafix.h>
#include <cfix.h>
#include <ideafix/priv/gndefs.h>
#include <wm.h>

#ifdef HAVE_TTY

#if defined(HAVE_TERMIO_H) || defined(HAVE_TERMIOS_H)
#ifdef HAVE_TERMIOS_H
#	include <termios.h>
#	include <unistd.h>
	static struct termios tsold;
#	include <sys/ioctl.h>
#else
#	include <termio.h>
#endif

#if defined(_AIX) || defined(SunSol)
#include <unistd.h>
#endif

int saved_int_char;

#ifdef HAVE_JOB_CONTROL

#	ifdef HAVE_SYSTTOLD_H
#		include <sys/ttold.h>
#		include <sys/filio.h>
#	endif /* HAVE_SYSTTOLD_H */

#	ifdef HAVE_BSDTTY_H
#		include <bsdtty.h>
#	endif /* HAVE_BSDTTY_H */

#	ifdef HAVE_LTCHARS
//struct ltchars ltold;
//struct ltchars ltnew = {0xff, 0xff, 0xff, 0xff, 0xff, 0xff};
extern struct ltchars ltold;
extern struct ltchars ltnew;
#	endif

#endif /* HAVE_JOB_CONTROL */

void _WiSettty(void)
{
#ifdef HAVE_TERMIOS_H
	struct termios t;
	long vdisable;

	vdisable = fpathconf(0, _PC_VDISABLE);
	tcgetattr(0, &t);
	mCopy(&tsold, &t, sizeof(struct termios));
	t.c_lflag &= ~(ICANON|XCASE|ECHO|ECHOE|ECHOK|ECHONL|NOFLSH);
	t.c_lflag |= ISIG;
	t.c_cc[VMIN] = 1;
	t.c_cc[VTIME] = 1;
	t.c_iflag &= ~ICRNL;
	t.c_oflag ^= OPOST;
#ifdef HAVE_JOB_CONTROL
	t.c_cc[VSUSP] = vdisable;
#endif
	tcsetattr(0, TCSANOW, &t);
#else
	struct termio t;

	/* Modo de lectura de la terminal */
	ioctl(0, TCGETA, &t);

#	ifdef HAVE_JOB_CONTROL
#		ifdef HAVE_LTCHARS
			ioctl(0, TIOCGLTC, &ltold);
			ioctl(0, TIOCSLTC, &ltnew);
#		else
			t.c_cc[VSWTCH] = CNSWTCH;
#		endif
#	endif
	t.c_lflag &= ~(ICANON|XCASE|ECHO|ECHOE|ECHOK|ECHONL|NOFLSH);
	t.c_lflag |= ISIG;
	t.c_cc[VMIN] = 1;
	t.c_cc[VTIME] = 1;
	t.c_iflag &= ~ICRNL;
	t.c_oflag ^= OPOST;
	ioctl(0, TCSETAF, &t);
#endif /* HAVE_TERMIOS_H */
	saved_int_char = SetIntChar(wm_intr_char);
	tty_set = TRUE;
}

void _WiResettty(void)
{
#ifdef HAVE_TERMIOS_H
	tcsetattr(0, TCSANOW, &tsold);
#else
	struct termio t;

	ioctl(0, TCGETA, &t);
	t.c_lflag |= ISIG | ICANON | ECHO | ECHOE | ECHOK;
	t.c_cc[VEOF] = 4;
	t.c_cc[VEOL] = 0;
	t.c_iflag |= ICRNL;
	t.c_oflag |= OPOST;
#	ifdef HAVE_JOB_CONTROL
#		ifdef HAVE_LTCHARS
			ioctl(0, TIOCSLTC, &ltold);
#		else
			t.c_cc[VSWTCH] = CNSWTCH;
#		endif
#	endif
	ioctl(0, TCSETAW, &t);
#endif
	SetIntChar(saved_int_char);
	tty_set = FALSE;
}

int _KeyReady(void)
{
	int n;

#ifdef HAVE_FIONREAD
	ioctl(0, FIONREAD, &n);
#else
#ifdef HAVE_FIORDCHK
	n = ioctl(0, FIORDCHK, &n);
#else
	n = 0;
#endif
#endif
	return n;
}

#else /* HAVE_TERMIO_H */

#include <sgtty.h>

void _WiSettty(void)
{
	struct sgttyb t;

	ioctl(0, TIOCGETP, &t);
	t.sg_flags |= CBREAK;
	t.sg_flags &= (~ECHO & ~CRMOD);
	t.sg_flags |= RAW;
	ioctl(0, TIOCSETP, &t);
}

void _WiResettty(void)
{
	struct sgttyb t;
	ioctl(0, TIOCGETP, &t);
	t.sg_flags &= ~CBREAK;
	t.sg_flags |= ECHO | CRMOD;
	t.sg_flags &= ~RAW;
	ioctl(0, TIOCSETP, &t);
}
#endif /* HAVE_TERMIO_H */ 

#endif /* HAVE_TTY */


#ifndef HAVE_TTY

void _WiSettty(void)
{
	/* Dummy */
}

void _WiResettty(void)
{
	ClrKeybBuff();
}

int _KeyReady(void)
{
	return kbhit();
}
#endif
