/********************************************************************
* Copyright (c) 1987 InterSoft Co.  All Rights Reserved
* THIS IS UNPUBLISHED PROPRIETARY SOURCE CODE OF InterSoft Co.
* The copyright notice above does not evidence any
* actual or intended publication of such source code.
*
* $Id: stprintf.cc,v 1.4 2004/09/29 15:59:55 damianm Exp $
*
* DESCRIPTION
*
* ROUTINE       |  PURPOSE
*---------------+---------------------------------------------------
*printf
*fprintf
*********************************************************************/

#include <signal.h>
#include <string.h>
#include <ifound/priv/gfuncs.h>
#include <ideafix/priv/gndefs.h>
#include <ideafix.h>
#include <cfix.h>

#if IDEADLL
#	include <mswi/ideadll.h>
#endif

/* ++ Prototypes ++ */

static int spsub(int c, char **str);

/* -- Prototypes -- */

/* +++ CONFIG +++ */
#define OVERFLOW_IMAGE "(Overflow)"
/* --- CONFIG --- */

extern "C" int printf(const char *fmt, ...)
{
	va_list ap;
	int ret;
	va_start(ap, fmt);
	ret = doFormat((IFPICP)fputc, (char*)stdout, fmt, ap);
	va_end(ap);
	return ret;
}

extern "C" int fprintf(FILE *f, char const *fmt, ...)
{
	va_list ap;
	int ret;

	va_start(ap, fmt);
	ret = doFormat((IFPICP)fputc, (char*)f, fmt, ap);
	va_end(ap);
	return ret;
}

static int aputc(int c, FILE *stream)
{
	return fputc(toascii(c), stream);
}

extern "C" int aprintf(FILE *f, char const *fmt, ...)
{
	va_list ap;
	int ret;

	va_start(ap, fmt);
	ret = doFormat((IFPICP)aputc, (char*)f, fmt, ap);
	va_end(ap);
	return ret;
}

extern "C" SPRINTFRETTYPE sprintf(char *str, const char *fmt, ...)
{
	va_list ap;
	int ret;

	va_start(ap, fmt);
	ret = doFormat((IFPICP)spsub, (char *) &str, fmt, ap);
	*str = '\0';
	va_end(ap);
	return (SPRINTFRETTYPE)ret;
}


/* Private Functions												*/

static int spsub(int c, char **str)
{
	return (*(*str)++ = (char) c) & 0xff;
}

/*
 * Convert short to string.
 */
extern "C" void IToStr(short i, char *buf)
{
	if (i == NULL_SHORT) 
		buf[0] = 0;
	else if (i == MAX_SHORT)
		(void) strcpy(buf, HIGH_VAL_IMAGE);
	else
		LToStr(Int(i), buf);
}

/*
 * Convert Int to string.
 */
extern "C" void LToStr(Int l, char *buf)
{
	char aux[32];

	if (l == NULL_LONG)
		buf[0] = 0;
	else if (l == MAX_LONG)
		(void) strcpy(buf, HIGH_VAL_IMAGE);
	else
		(void) strcpy(buf, longToStr(l, aux+sizeof(aux)-1, -10));
}

/*
 * Convert double to string.
 */
extern "C" void FToStr(double d, char *buf)
{
	if (d == NULL_DOUBLE)
		buf[0] = 0;
	else if (d == MAX_DOUBLE)
		(void) strcpy(buf, HIGH_VAL_IMAGE);
	else if (d < 1e16 && d > -1e16)
		floatToStr(d, buf, 0, 1);
	else
		(void) strcpy(buf, OVERFLOW_IMAGE);
}

extern "C" void FmtFloat(char *buff, double f, int len)
{
	if (f == NULL_DOUBLE) {
		buff[0] = '\0';
		return;
	}
	sprintf(buff, "%*.*g", len, len , f);
}
