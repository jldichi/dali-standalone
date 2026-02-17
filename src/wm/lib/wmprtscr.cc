/********************************************************************
* Copyright (c) 1987 InterSoft Co.  All Rights Reserved
* THIS IS UNPUBLISHED PROPRIETARY SOURCE CODE OF InterSoft Co.
* The copyright notice above does not evidence any
* actual or intended publication of such source code.
*
* MODULE & VERSION : @(#)wmprtscr.c	3.11
* DATE             : 90/11/22
* TIME             : 10:39:27
*
* DESCRIPTION:
*      Descripcion del proposito general del modulo
*
* ROUTINE       |  PURPOSE
*---------------+---------------------------------------------------
*_print_scr		 Screen dump to printer.
*
*********************************************************************/

#include <ideafix.h>
#include <cfix.h>

#include <ifound/str.h>

#include <ideafix/priv/wmcom.h>
#include <ideafix/priv/gndefs.h>
#include <wm.h>

#include <ifound/priv/gfuncs.h>

#include <signal.h>

/* Private Functions												*/
/*
 * Print_scr : Screen Dump to Printer
 *
 */
#ifdef NO_SERVICES

void _print_scr(void) { }

#else

/* ++ Prototypes ++ */

static void print(VS_CELL cell);

/* -- Prototypes -- */

FILE *out;
int no_cap = 0;

void _print_scr(void)
{
	int fa,ca;
	int row, col, cursor;
	FPCPCPI savefp1;
	SigHandlerT savefp;
	char *output;

	savefp1 = SetReadEnvHandler((FPCPCPI)NULLFP);
	output = ReadEnv("printer");
	(void) SetReadEnvHandler(savefp1);

	if (output==NULL) {
		Warning(_WmErrmsg("PRN_NOT_FOUND"));	
		return;
	}

#ifdef HAVE_FULL_SIGNALS
	savefp = SigSet(SIGCLD, SIG_DFL);
#endif
	if ((out = OpenPrinter(1, "")) != NULL) {
		fa = vsrow;
		ca = vscol;
		cursor = vscursor;
		vscursor = 2;
		vs_moveto(0, 0);
		_vs_update(1, 0);
		for (row=0; row<VS_LINES; row++) {
			for (col=0; col<VS_COLS; col++) 
				print(vscreen_get(row,col));
			(void) fputc('\n', out);
			vs_moveto((short)row, 0);
			_vs_update(1, 0);
		}
		vscursor = (short)cursor;
		ClosePrinter(out);
		vs_moveto((short)fa, (short)ca);
		_vs_update(1, 0);
#ifdef HAVE_FULL_SIGNALS
		(void) SigSet(SIGCLD, savefp);
#endif
	}	
	else 
		WiMsg(_WmErrmsg("NO_FILE_PRTSCR"));
}

static void print(VS_CELL cell)
{
	static attr_type actattr = (attr_type) 0;
	attr_type attr;
	UChar     c;

	attr = VS_ATTR(cell); /* get attribute of cell */
	c    = VS_CHAR(cell); /* get character */

	/* 
	 *  Same attribute THEN send character to printer
	 *                 ELSE set new attribute & send char
	 */
	if (no_cap)  /* Printer with no capabilities */
		(void) fputc((char) c, out);
	else {
		if (attr == actattr)
			(void) fputc((char) c, out);
		else {
			actattr = attr;
			if ((attr&A_BOLD) || (attr&A_UNDERLINE) || 
				(attr&A_REVERSE)) {
				if ((attr&A_BOLD) == A_BOLD) 
					(void) fprintf(out, "\033[1m");
				if ((attr&A_UNDERLINE) == A_UNDERLINE) 
					(void) fprintf(out, "\033[4m");
				if ((attr&A_REVERSE) == A_REVERSE)
					(void) fprintf(out, "\033[7m");
			}
			else 
				(void) fprintf(out, "\033[m");
			(void) fputc((char) c, out);
		}
	}
}
#endif /* NO_SERVICES */
