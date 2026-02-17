/*********************************************************************
* Copyright (c) 1987 InterSoft Co.  All Rights Reserved
* THIS IS UNPUBLISHED PROPRIETARY SOURCE CODE OF InterSoft Co.
* The copyright notice above does not evidence any
* actual or intended publication of such source code.
*
* $Id: tm.h,v 1.2 1998/04/20 18:51:06 guiller Exp $
*
* DESCRIPTION
* Definition & interface file for TM routines
*
* Files that must be included before this:
* ----------------------------------------
*********************************************************************/

#ifndef TM_H
#define TM_H

/* Formatos de fechas y horas */

/* Orden de despliegue AMERICAN = mm/dd/aa, INTERNAT = dd/mm/aa */

#define DFMT_INTERNAT	0	
#define DFMT_AMERICAN	1	

/* Forma de desplegar */

#define DFMT_SEPAR		0001
#define DFMT_YEAR4		0002
#define DFMT_MONTHNAME	0004
#define DFMT_LMONTHNAME	0010	
#define DFMT_DAYNAME	0020
#define DFMT_LDAYNAME	0040
#define DFMT_NEXPAND	0100
#define DFMT_STANDARD	(DFMT_SEPAR)

#define TFMT_SEPAR		0001
#define TFMT_SECONDS	0002
#define TFMT_HS12		0004
#define TFMT_STANDARD	(TFMT_SEPAR)
#define TFMT_NEXPAND	0100

/* Macros de manejo de fechas y horas */

#define HOURS(n)	((int) (((n) - 1)/1800))
#define MINUTES(n)	((int) ((((n) - 1) % 1800) / 30))
#define SECONDS(n)	((int) ((((n) - 1) % 30) * 2))

#define RSECONDS(n)				((long) (((n) - 1) * 2))
#define TIME_FROM_RSECONDS(s)	((TIME) (((s) / 2) + 1))

/* extern variables */

#ifdef __cplusplus
extern "C"{
#endif

extern DLLDEPEND char *TODAY_STR, *HOUR_STR, *DAY_STR, *MONTH_STR, *YEAR_STR;

#ifdef __cplusplus
}
#endif

/* Numeracion de errores para _TmErrmsg */

#define DAYS		0
#define MONTHS		7
#define SEP_DATE	19

/* Correccion de sintaxis */
#define IsHoliday(x)	IsHolliday(x)

/* ++ Prototypes ++ */

#ifdef __cplusplus
extern "C" {
#endif

extern void SetDateFmt(int n);
extern int GetDateFmt(void);
extern char *DayName(DATE n);
extern char *MonthName(int n);
extern DATE StrNXToD(char *fecha);
extern DATE StrToD(char *fecha);
extern DATE DMYToD(short day, short month, short year);
extern void DToStr(DATE ifecha, char *fecha, int format);
extern void DToDMY(DATE ifecha, short *day, short *month, short *year);
extern short Day(DATE fecha);
extern short Month(DATE fecha);
extern short Year(DATE fecha);
extern TIME StrToT(char *s);
extern TIME StrNXToT(char *s);
extern void TToStr(TIME t, char *dest, int fmt);
extern DATE FirstMonthDay(DATE f);
extern int HalfMonth84(DATE d);
extern DATE Today(void);
extern TIME Hour(void);
extern char *IsHolliday(DATE fecha);
extern DATE AddMonth(DATE fecha_act, int cantidad);
extern DATE LastMonthDay(DATE f);
extern long MonthDiff(DATE a, DATE b);
extern long Week(DATE d);

#ifdef __cplusplus
}
#endif

/* -- Prototypes -- */
#endif
