/********************************************************************
* Copyright (c) 1994 InterSoft Co.  All Rights Reserved
* THIS IS UNPUBLISHED PROPRIETARY SOURCE CODE OF InterSoft Co.
* The copyright notice above does not evidence any
* actual or intended publication of such source code.
*
* $Id: delimout.cc,v 1.3 1997/03/18 16:05:30 eduardoc Exp $
*
* DESCRIPTION
*********************************************************************/


#include <ideafix.h>
#include <cfix.h>
#include <ideafix/priv/delimout.h>
#include <ifound/priv/gfuncs.h>
#include <signal.h>
#include <ideafix/priv/gndefs.h>

static bool _broken_pipe = false;		// (pablog)
static void (*_fpsave)() = NULL;		// (pablog)

#ifdef HAVE_FULL_SIGNALS
static void _BrokenPipe(void)
{
	_broken_pipe = true;
	(void) SigSet(SIGPIPE, (FPI) _BrokenPipe);
}
#endif

DelimitedOutput::DelimitedOutput(Int destination, const String &arg, 
								 const String &sep, Int ncopies)
{
	_broken_pipe = false;		// (pablog)
	items = 0;
	ncols = 0;
	hlines = -1;
	dest  = UnShort(destination);

	switch (dest) {
#ifdef HAVE_PIPES
	case OUT_PIPE:
		fp = popen((char*)toCharPtr(arg), "w") ;
		_fpsave = (FP)SigSet(SIGPIPE, (FPI) _BrokenPipe);
		break;
#endif
	case OUT_PRINTER:
		fp = OpenPrinter(ncopies, (char*)toCharPtr(arg));
		break;
	default :
		if (arg != NULL_STRING) 
			fp = (FILE*)fopen((char*)toCharPtr(arg), "w");
		else
			fp = (FILE*)stdout;
		break;
	}
	if (!fp)
		error(__RpErrmsg("E_RP_NO_OUTPUT"), toCharPtr(arg));

	if (fp) {
		if (sep != NULL_STRING) {
			fsep = sep[0];
			rsep = sep[1];
		} else {
			fsep = '\t';
			rsep = '\n';
		}
	}
}

DelimitedOutput::~DelimitedOutput()
{
	switch (dest) {
#ifdef HAVE_PIPES
	case OUT_PIPE 	:
		(void) pclose(fp);
		(void) SigSet(SIGPIPE, _fpsave);
		break;
#endif
	case OUT_FILE	:
		if (fp && fp!=stdout) fclose(fp);
		break;
	case OUT_PRINTER :
		ClosePrinter(fp);
		break;
	default:
		assert(0);
		break;
	}
}

Int DelimitedOutput::operator<<(const char *bp)
{
	if (_broken_pipe)
		return ERR;
	String s = bp;
	s = s.toAscii();

	fputs(toCharPtr(s), fp);
	if (++currcol < ncols) fputc(fsep, fp);
	else {
		fputc(rsep, fp);
		currcol = 0;
		items++;
	}
	return OK;
}

