/********************************************************************
* Copyright (c) 1991 InterSoft Co.  All Rights Reserved
* THIS IS UNPUBLISHED PROPRIETARY SOURCE CODE OF InterSoft Co.
* The copyright notice above does not evidence any
* actual or intended publication of such source code.
*
* $Id: gfuncs.h,v 1.4 1998/04/21 19:17:12 guiller Exp $
*
* DESCRIPTION:
* General purpose private function prototypes
*********************************************************************/

#ifndef PRIV_GFUNCS_H
#define PRIV_GFUNCS_H

#include <ifound.h>
#include <ifound/functype.h>
#include <ifound/priv/macros.h>

class String;
class Time;
#ifdef __VC__
struct ProcessConfig;
#else
class ProcessConfig;
#endif

#ifdef HAVE_SYS_TIMEB_H
	struct timeb;
	Int _osTimeZoneAdj(struct timeb *);
#else
	Int _osTimeZoneAdj(Int);
#endif

FILE *openDataDirFile(const String &data, const String &language, 
					  const String &file);

Int _osTime();
bool isBigEndian();
Int ldlong(register const void *p);
void stlong(register Int l, register void *p);
double lddbl(register const void *p);
void stdbl(double d, register void *p);
const char *readEnvVar(const char *v);
void putEnvVar(const char *v);
char *longToStr(Int v, register char *cp, int base);
void floatToStr(double val, register char *buff, int dec, int tipo);
int doFormat(IFPICP ofunct, char *funct_arg, const char *fmt, va_list ap);
bool findInPath(const String &name, String ext, const String &path,
		String &fullname);
String findInPath(const String &filename, const String &path);
bool isAbsolutePath(const String &path);
ProcessConfig *_processConfig();
void setArgs(int argc, char **argv);
int lockFile(int fd, bool lock, bool test, long offset, long len);
int freeFile(int fd, long offset, long len);
long fdLockOwner(int fd, long offset, long len);
UnShort ldUnShort(FILE *fp);
short ldShort(FILE *fp);
signed char ldChar(FILE *fp);
Int ldInt(FILE *fp);
String ldString(FILE *fp);
bool ldBool(FILE *fp);
void stString(FILE *fp, const String &str);
void stInt(FILE *fp, Int n);
void stInt(Int l, char *&p);
Int ldInt(const char *&p);
void stUnShort(FILE *fp, UnShort n);
void stShort(FILE *fp, short n);
void stChar(FILE *fp, signed char);
void stBool(FILE *fp, bool v);
short ntbioskey(int flag);
extern "C" void ntbeep(int frec, int lapse);

#endif
