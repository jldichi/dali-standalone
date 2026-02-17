/********************************************************************
* Copyright (c) 1991 InterSoft Co.  All Rights Reserved
* THIS IS UNPUBLISHED PROPRIETARY SOURCE CODE OF InterSoft Co.
* The copyright notice above does not evidence any
* actual or intended publication of such source code.
*
* $Id: timedefs.h,v 1.1.1.1 1996/04/16 18:51:35 gustavof Exp $
*
* DESCRIPTION
* Date/Time handling function prototypes
*********************************************************************/

#ifndef PRIV_TIMEDEFS_H
#define PRIV_TIMEDEFS_H

typedef long LTIME;
typedef long LDATE;

/* Useful constants */

#define	SECONDS_DAY			((long)(60L*60L*24L))
#define	SECONDS_HOUR		((long)(60L*60L))
#define SECONDS_MINUTE		(60L)

/* +++ Prototypes +++ */

void _date_to_dmy(LDATE idate, long *day, long *month, long *year);
LDATE _dmy_to_date(long day, long month, long year);
LDATE _str_to_date(const char *sd, DateFmt fmt);
const char *_date_to_str(LDATE l, char *buf, DateFmt fmt);
long _date_to_osdate(LDATE idate);
LDATE _osdate_to_date(long l);

void _time_to_dhms(LTIME i, long *d, long *h, long *m, long *s);
LTIME _dhms_to_time(long d, long h, long m, long s);
LTIME _str_to_time(const char *sd);
const char *_time_to_str(LTIME l, char *buf, TimeFmt tfmt);
long _time_to_ostime(LTIME itime);
LTIME _ostime_to_time(long l);

/* --- Prototypes --- */

#endif
