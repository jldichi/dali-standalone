/********************************************************************
* Copyright (c) 1987 InterSoft Co.  All Rights Reserved
* THIS IS UNPUBLISHED PROPRIETARY SOURCE CODE OF InterSoft Co.
* The copyright notice above does not evidence any
* actual or intended publication of such source code.
*
* $Id: st.h,v 1.1.1.1 1996/04/16 18:51:46 gustavof Exp $
*
* DESCRIPTION
* Interface & definition file for ST routines.
*********************************************************************/

#ifndef ST_H
#define ST_H

/******************* Constantes simbolicas *******************/
#define REXP_NOCASE	1
#define REXP_EMBEDDED 2

#define ENV_VAR_CODE	01
#define FLD_CODE		02

/******************* Macros Usuales *******************/

#define StrEq(x,y) (!StrCmp((char *) x,(char *) y))
#define StrNotEq(x,y) StrCmp((char *) x,(char *) y)
#define StrNEq(x,y,n) (!StrNCmp((char *) x,(char *) y,n))
#define StrNNotEq(x,y,n) StrNCmp((char *) x,(char *) y,n)
#define str_eq(x,y) (!strcmp((char *) x,(char *) y))
#define str_ne(x,y) strcmp((char *) x,(char *) y)
#define strn_eq(x,y,n) (!strncmp((char *) x,(char *) y,n))
#define strn_ne(x,y,n) strncmp((char *) x,(char *) y,n)

/* ++ Prototypes ++ */

#ifdef __cplusplus
extern "C" {
#endif

/* These group of functions are only defined if we're not 
   compiling under TC because they are already prototyped
   by the TC includes */

#ifndef HAVE_STRSTR

extern char *strstr(const char *s1, const char *s2);

#endif


extern int _FindCharCode(char *name);
extern char *_CharName(unsigned int n);
extern int CompileMask(char *mask, char *tstmask, char *omask);
extern int SizeOfMask(char *mask);
extern int StrCmp(void *s1, void *s2);
extern int StrNCmp(void *s1, void *s2, int n);
extern void SetStrCmp(IFPCPCP function);
extern void SetStrNCmp(IFPCPCPI function);
extern int tolower(int x);
extern void StrToLower(char *p);
extern int toupper(int x);
extern void StrToUpper(char *p);
extern void StrToAscii(char *p);
extern void StrDecode(UChar *dest, UChar *s);
extern int StrDspLen(char *p);
extern void IToStr(short i, char *buf);
extern void LToStr(long l, char *buf);
extern void FToStr(double d, char *buf);
extern void FmtNum(char *obuff, char *ibuff, int length, int ndec, int comma);
extern void FmtFloat(char *buff, double f, int len);
extern double FRnd(double v, int nd);
extern void StrMask(char *obuff, char *mask, char *ibuff);
extern void NumToTxt(double m, void *x, int f, int c, int uno, char *adelante, char *atras);
extern bool RexpMatch(UChar *a, UChar *b, int flags);
extern char *strfld(char *dest, char *s1, char *s2, int n);
extern short StrToI(char *s);
extern long StrToL(char *s);
extern double StrToF(char *s);
extern char *StrTxt(char *s, int n);
extern UChar TstChrMask(UChar chr, char mask, char* value);
extern bool MaskIsNum(char *mask);
extern char *StrTrim(char *s);

#ifdef __cplusplus
}
#endif

/* -- Prototypes -- */

#endif
