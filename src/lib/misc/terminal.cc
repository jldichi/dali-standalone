/********************************************************************
* Copyright (c) 1991 InterSoft Co.  All Rights Reserved
* THIS IS UNPUBLISHED PROPRIETARY SOURCE CODE OF InterSoft Co.
* The copyright notice above does not evidence any
* actual or intended publication of such source code.
*
* $Id: terminal.cc,v 1.3 2008/01/24 14:05:13 hardaiz Exp $
*
* DESCRIPTION
* Set/Reset terminal input mode
*********************************************************************/

#include <string.h>
#include <unistd.h>
#include <ifound.h>
#include <ifound/terminal.h>

#ifdef HAVE_TTY

#ifdef HAVE_TERMIOS_H
#	include <termios.h>
#	include <unistd.h>
#else
#	include <termio.h>
#endif

#ifdef HAVE_BSDTTY_H
#	include <bsdtty.h>
#endif

#include <ifound/stdc.h>

static int tty_set = false;		// terminal is not set

#ifdef HAVE_JOB_CONTROL

#ifdef HAVE_TERMIOS_H
	static cc_t ccold[NCCS] = { 0 };
#endif

#	ifdef HAVE_SYSTTOLD_H
#		include <sys/ttold.h>
#		include <sys/filio.h>
#	endif

#	ifdef HAVE_LTCHARS
struct ltchars ltold;
struct ltchars ltnew = {0xff, 0xff, 0xff, 0xff, 0xff, 0xff};
#	endif

#endif

void setTerminal()
{
#ifdef HAVE_TERMIOS_H
	struct termios t;
	long vdisable;

	vdisable = fpathconf(0, _PC_VDISABLE);
	tcgetattr(0, &t);
	mCopy(ccold, t.c_cc, NCCS * sizeof(cc_t));
	t.c_lflag = ISIG;
	t.c_cc[VMIN] = 1;
	t.c_cc[VTIME] = 1;
	t.c_iflag &= ~ICRNL;
	t.c_oflag ^= OPOST;
#	ifdef HAVE_JOB_CONTROL
		t.c_cc[VSUSP] = vdisable;
#	endif
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
#		endif /* HAVE_LTCHARS */
#	endif /* HAVE_JOB_CONTROL */
	t.c_lflag = ISIG;
	t.c_cc[VMIN] = 1;
	t.c_cc[VTIME] = 1;
	t.c_iflag &= ~ICRNL;
	t.c_oflag ^= OPOST;
	ioctl(0, TCSETAF, &t);
#endif /* HAVE_TERMIOS_H */
	tty_set = true;
}

void resetTerminal()
{
#ifdef HAVE_TERMIOS_H
	struct termios t;

	tcgetattr(0, &t);
	mCopy(t.c_cc, ccold, NCCS * sizeof(cc_t));
	tcsetattr(0, TCSANOW, &t);
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
#		endif /* HAVE_LTCHARS */
#	endif /* HAVE_JOB_CONTROL */
	ioctl(0, TCSETAW, &t);
#endif /* HAVE_TERMIOS_H */
	tty_set = false;
}

// Return true if terminal has been set, FLASE otherwise

bool terminalIsSet()
{
	return tty_set;
}

// Get a raw character from stdin

char getChar()
{
	char c;

	if (!tty_set) {
		setTerminal();
		_global_read(0, &c, 1);
		resetTerminal();
	}
	else _global_read(0, &c, 1);

	return c;
}

#endif			// UNIX

// end of terminal
