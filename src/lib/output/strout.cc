/********************************************************************
* Copyright (c) 1994 InterSoft Co.  All Rights Reserved
* THIS IS UNPUBLISHED PROPRIETARY SOURCE CODE OF InterSoft Co.
* The copyright notice above does not evidence any
* actual or intended publication of such source code.
*
* $Id: strout.cc,v 1.8 2004/09/29 17:19:42 damianm Exp $
*
* DESCRIPTION
*********************************************************************/

#include <string.h>

#include <ideafix.h>
#include <cfix.h>
#include <ifound/priv/gfuncs.h>
#include <ideafix/priv/strout.h>
#include <ideafix/priv/gndefs.h>

#include <signal.h>

static bool _broken_pipe = false;		// (pablog)
static void (*_fpsave)() = NULL;		// (pablog)

#ifdef HAVE_FULL_SIGNALS
static void _BrokenPipe(void)
{
	_broken_pipe = true;
	SigSet(SIGPIPE, (FPI) _BrokenPipe);
}
#endif

void StreamOutput::_LogInFile()
{
}

Int StreamOutput::_GetCmd(bool )
{
	return OK;
}

void StreamOutput::_Refresh()
{
}

Int StreamOutput::_CalcWidth(Int offs, bool cont)
{
	Int i, width;

	if ( ! cont ) return twidth - offs;

	i = pwidth / 2;
	for (width = pwidth; width > i; width--)
		if (lines[0][offs+width] == C_VERT) break;

	if (width <= i) return pwidth;
	return width;
}

void StreamOutput::_OutTitle(bool h)
{
	Int zone, i;
	UnChar *s, *p, outline[512], aux[512];
	Int n;

	n = pwidth + 2;
	if (n >= 512) n = 511;

	if (!fp) return;		// file not opened

	zone = 0;
	p = aux;
	if (h)
		s = (UnChar*)toCharPtr(heading);
	else
		s = (UnChar*)toCharPtr(footing);
	mSet(outline, ' ', n);
	outline[n] = '\0';

	forever {
		switch (*s) {
		case '\0' :
		case '\t' :
		case '\n' :
			*p = 0;
			p  = aux;
			i  = strlen((char*)aux);
			if (i > n) i = n;
			if (zone == 0)
				strncpy((char*)outline, (char*)aux, i);
			else if (zone == 1)
				strncpy((char*)outline + (n-i)/2, (char*)aux, i);
			else
				strncpy((char*)outline + n-i, (char*)aux, i);
			if (*s == '\t') zone++;
			else {
				for (i = 0; i < leftmarg; i++) fputc(' ', fp);
				StrToAscii((char*)outline);
				(void) fputs((char*)outline, fp);
				fputc('\n', fp);
				if (*s == '\0') return;
				zone = 0;
				mSet(outline, ' ', n);
				outline[n] = '\0';
			}
			s++;
			break;
		case '#' :
			switch (*++s) {
			case '\0' : 
				continue;
			case 'D'  :
				DToStr(Today(), (char *) p, DFMT_SEPAR);
				p += strlen((char *) p);				
				break;
			case 'T'  :
				TToStr(Hour(), (char *) p, TFMT_SEPAR);
				p += strlen((char *) p);
				break;
			case 'P'  :
				sprintf((char*)p, "%d",  pageno);
				p += strlen((char *) p);
				break;
			default :
				*p++ = *s;
			}
			s++;
			break;
		case '$' :
			if (isdigit(*++s)) {
				for (i = 0; isdigit(*s); s++)
					i = i*10 + *s - '0';
				if (i < ProcNArgs()) {
					(void) strcpy((char *) p, ProcArgs()[i]);
					p += strlen((char *) p);
				}
			}
			else {
				FPCPCPI OldHandler = SetReadEnvHandler(NULLFP); 
				UnChar *p1 = p;
				if (isalpha((UnChar) *s))
					while (isalnum((UnChar) *s)) *p1++ = *s++; 
				else if (*s == '{') {
					s++;
					while (*s && *s != '}') *p1++ = *s++; 
					if (*s) s++;
				}
				*p1 = '\0';

				if ((p1 = (UnChar *) ReadEnv((char*)p)) != NULL) {
					(void) strcpy((char*)p, (char*)p1);
					p += strlen((char*)p);
				}
				(void)SetReadEnvHandler(OldHandler); 
			}
			break;
		default :
			*p++ = *s++;
			break;
		}
	}
}

Int StreamOutput::_OutPage(bool more, bool)
{
	if (!fp || _broken_pipe)
		return ERR;
		
	static char *dummy = NULL;
	bool cont = true;
	Int  offs = 0;

	while (cont) {
		cont = offs+pwidth < twidth;
		Int wth  = _CalcWidth(offs, cont);
		Int lmarg  = leftmarg + (pwidth+1 - wth) / 2;

		/* Print The Heading */
		Int n;
		for (n = topmarg; n; n--) fputc('\n', fp)
			;
		_OutTitle(true);

		/* Print the top */

		Int i;
		for (i = 0; i < lmarg; i++)
			fputc(' ', fp);

		fputc(offs ? C_LARROW : C_D_UL, fp);

		if (lines == NULL) {
			if (!dummy) dummy = tr_new char[1]; // it'll be deleted by ~GridOutput()
			assert(dummy);
			dummy[0] = '\0';
			lines = (UnChar **) dummy;
		}

		UnChar *p = &lines[0][offs];
		for (i = 0; i < wth; i++, p++)
			fputc(*p == C_VERT ? C_DS_TOP : C_D_HORIZ, fp);
		fputc(cont ? C_RARROW : C_D_UR, fp);
		fputc('\n', fp);

		/* Print the body */

		for (n = 0; n < currline; n++) {
			bool sep = n == hlines;
			for (i = 0; i < lmarg; i++) fputc(' ', fp);
			fputc(offs ? (sep ? C_LARROW : ' ')
						 : (sep ? C_DS_LEFT: C_D_VERT), fp);

			p =  & lines[n][offs];

			for (i = 0; i < wth; i++,p++)
				fputc( (*p >= 192) ? toascii(*p) : *p ,fp);

			fputc(cont ? (sep ? C_RARROW   : ' ')
					   : (sep ? C_DS_RIGHT : C_D_VERT), fp);
			fputc('\n', fp);
		}

		/* Print the bottom */

		for (i = 0; i < lmarg; i++) fputc(' ', fp);
		fputc(offs ? C_LARROW : C_D_LL, fp);
		p =  & lines[0][offs];
		for (i = 0; i < wth; i++, p++)
			fputc(*p == C_VERT ? C_DS_BOT : C_D_HORIZ, fp);
		fputc(cont ? C_RARROW : C_D_LR, fp);
		fputc('\n', fp);

		offs += wth;

		/* Print the footing */
        
		_OutTitle(false);

		if (more || cont)
			fputc('\f', fp);
	}
	return OK;
}

Int StreamOutput::_CountLf(char *s)
{
	Int lf = 0;

	if (s[0] == 0)
		return 0;
	while (*s)
		if (*s++ == '\n') lf++;
	return lf+1;
}

StreamOutput::StreamOutput(Int destination, const String &arg, 
						   const String &h, const String &f, Int fwidth, 
						   Int flen, Int topm, Int botm, Int leftm, 
						   Int ncopies)
:	heading(h),
	footing(f)
{
	_broken_pipe = false;		// (pablog)

	items  = 0;
	ncols  = 0;
	hlines = -1;
	dest   = UnShort(destination);

	switch (dest) {
#ifdef HAVE_PIPES
	case OUT_PIPE:
		fp = popen((char*)toCharPtr(arg), "w") ;
		_fpsave = (FP)SigSet(SIGPIPE, (FPI) _BrokenPipe);	// (pablog)
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

	if (fp) {
		Int aux;

		aux = fwidth-2-leftm; 
		assert(aux >= 0);
		pwidth  = (UnShort) aux;
		
		aux = (flen-2-topm-botm-_CountLf((char*)toCharPtr(heading))-
								_CountLf((char*)toCharPtr(footing)));
		assert(aux >= 0);
		pheight = (UnShort) aux; 
		
		assert(topm >= 0);
		topmarg = (UnShort) topm;

		assert(leftm >= 0);
		leftmarg= (UnShort) leftm;
	}
}						   

StreamOutput::~StreamOutput()
{
	_OutPage(false, false);
	switch (dest) {
#ifdef HAVE_PIPES
	case OUT_PIPE 	:
		(void) pclose(fp);
		(void) SigSet(SIGPIPE, _fpsave);		// (pablog)
		break;
#endif
	case OUT_FILE	:
		if (fp && fp != stdout) fclose(fp);
		break;
	case OUT_PRINTER :
		ClosePrinter(fp);
		break;
	default:
		assert(0);
		break;
	}
}

